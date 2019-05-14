type t;
let make:
  (
    ~client: HttpClient.t,
    ~databaseName: UrlComponent.t,
    ~name: UrlComponent.t
  ) =>
  t;

type with_rev;
let id: with_rev => string;
let rev: with_rev => string;

let withRev: (~rev: string, t) => with_rev;

[@deriving show]
type get_error = [
  | `Invalid_revision
  | `Read_not_allowed
  | `Document_not_found
];
let get: t => Lwt.t(result(Yojson.Safe.t, get_error));

[@deriving show]
type create_error = [
  | `Invalid_request_body
  | `Write_not_allowed
  | `Document_not_found
  | `Conflict
];
type create_result = {
  id: string,
  rev: string,
};
let create:
  (~body: Yojson.Safe.t, t) => Lwt.t(result(with_rev, create_error));

let update:
  (~body: Yojson.Safe.t, with_rev) => Lwt.t(result(with_rev, create_error));
