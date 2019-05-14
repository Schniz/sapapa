[@deriving show]
type t;
type http_errors;

let make: (~headers: Cohttp.Header.t=?, ~baseUrl: string, unit) => t;
let db: (~name: UrlComponent.t, t) => Database.t;
let usersDb: t => Database.t;
let version: t => Lwt.t(string);
