[@deriving show]
type t;
type http_errors;

let makeWithHeaders: (~headers: Cohttp.Header.t=?, ~baseUrl: string, unit) => t;
let make: (~baseUrl: string) => t;

/** Get a database handler by name */
let db: (~name: UrlComponent.t, t) => Database.t;

/** The node users database */
let usersDb: t => Database.t;

/** Get the server version */
let version: t => Lwt.t(string);
