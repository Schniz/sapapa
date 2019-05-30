[@deriving (show, make)]
type t = {
  name: UrlComponent.t,
  client: HttpClient.t,
};

let name = t => t.name;
let exists = t => {
  let%lwt response =
    HttpClient.head(t.client, ~path=UrlComponent.encoded(t.name));
  HttpClient.(response.status) |> Cohttp.Code.is_success |> Lwt.return;
};

[@deriving show]
type create_error = [
  | `Invalid_database_name
  | `Not_an_admin
  | `Database_already_exists
];

let create = t => {
  let%lwt response =
    HttpClient.put(
      t.client,
      ~body=`Assoc([]),
      ~path=UrlComponent.encoded(t.name),
    );

  switch (response.status) {
  | 400 => Lwt.return_error(`Invalid_database_name)
  | 401 => Lwt.return_error(`Not_an_admin)
  | 412 => Lwt.return_error(`Database_already_exists)
  | status when Cohttp.Code.is_success(status) => Lwt.return_ok(t)
  | statusCode =>
    Printf.sprintf(
      "Got an undocumented status code for database creation: %d",
      statusCode,
    )
    |> failwith
  };
};

[@deriving show]
type delete_error = [
  | `Invalid_database_name
  | `Not_an_admin
  | `Database_doesnt_exist
];

let delete = t => {
  let%lwt response =
    HttpClient.delete(
      t.client,
      ~body=`Null,
      ~path=UrlComponent.encoded(t.name),
    );

  switch (response.status) {
  | 400 => Lwt.return_error(`Invalid_database_name)
  | 401 => Lwt.return_error(`Not_an_admin)
  | 404 => Lwt.return_error(`Database_doesnt_exist)
  | status when Cohttp.Code.is_success(status) => Lwt.return_ok(t)
  | status =>
    Printf.sprintf(
      "Got an undocumented status code for database deletion: %d",
      status,
    )
    |> failwith
  };
};

let doc = (~id, t) =>
  Document.make(~name=id, ~databaseName=t.name, ~client=t.client);
