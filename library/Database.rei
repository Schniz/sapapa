[@deriving show]
type t;

[@deriving show]
type create_error = [
  | `Invalid_database_name
  | `Not_an_admin
  | `Database_already_exists
];

[@deriving show]
type delete_error = [
  | `Invalid_database_name
  | `Not_an_admin
  | `Database_doesnt_exist
];

let make: (~name: UrlComponent.t, ~client: HttpClient.t) => t;
let name: t => UrlComponent.t;
let exists: t => Lwt.t(bool);
let create: t => Lwt.t(result(t, create_error));
let delete: t => Lwt.t(result(t, delete_error));
let doc: (~id: UrlComponent.t, t) => Document.t;

/* let get: (~key, t) => Document.t; */
