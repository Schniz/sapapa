[@deriving make]
type t = {
  client: HttpClient.t,
  databaseName: UrlComponent.t,
  name: UrlComponent.t,
};

type with_rev = {
  _id: string,
  _rev: string,
  doc: t,
};
let id = wr => wr._id;
let rev = wr => wr._rev;

let getPath = t =>
  UrlComponent.(
    Printf.sprintf("%s/%s", encoded(t.databaseName), encoded(t.name))
  );

[@deriving show]
type get_error = [
  | `Invalid_revision
  | `Read_not_allowed
  | `Document_not_found
];

let get = t => {
  open HttpClient;
  let%lwt response = HttpClient.get(~path=getPath(t), t.client);

  switch (response.status) {
  | 400 => Lwt.return_error(`Invalid_revision)
  | 401 => Lwt.return_error(`Read_not_allowed)
  | 404 => Lwt.return_error(`Document_not_found)
  | status when Cohttp.Code.is_success(status) =>
    Lwt.return_ok(response.body)
  | status =>
    Printf.sprintf("Unexpected status code from document: %d", status)
    |> failwith
  };
};

/* 400 Bad Request – Invalid request body or parameters */
/* 401 Unauthorized – Write privileges required */
/* 404 Not Found – Specified database or document ID doesn’t exists */
/* 409 Conflict – Document with the specified ID already exists or specified revision is not latest for target document */

[@deriving show]
type create_error = [
  | `Invalid_request_body
  | `Write_not_allowed
  | `Document_not_found
  | `Conflict
];
[@deriving yojson({strict: false})]
type create_result = {
  id: string,
  rev: string,
};

let withRev = (~rev, t) => {
  _id: UrlComponent.encoded(t.name),
  _rev: rev,
  doc: t,
};

let create = (~body, t) => {
  let%lwt response = HttpClient.put(~path=getPath(t), ~body, t.client);
  switch (response.status) {
  | 400 => Lwt.return_error(`Invalid_request_body)
  | 401 => Lwt.return_error(`Write_not_allowed)
  | 404 => Lwt.return_error(`Document_not_found)
  | 409 => Lwt.return_error(`Conflict)
  | status when Cohttp.Code.is_success(status) =>
    switch (create_result_of_yojson(response.body)) {
    | Ok(res) => Lwt.return_ok({_id: res.id, _rev: res.rev, doc: t})
    | Error(err) => Lwt.fail_with("Got an unknown return value: " ++ err)
    }
  | status =>
    Lwt.fail_with("Status isn't acceptable: " ++ string_of_int(status))
  };
};

let update = (~body: Yojson.Safe.t, full) =>
  switch (body) {
  | `Assoc(entries) =>
    let newEntries = [
      ("_id", `String(full._id)),
      ("_rev", `String(full._rev)),
      ...entries,
    ];
    create(~body=`Assoc(newEntries), full.doc);
  | _ => Lwt.return_error(`Invalid_request_body)
  };
