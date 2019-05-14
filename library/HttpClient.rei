[@deriving show]
type t;

type response = {
  body: Yojson.Safe.t,
  headers: Cohttp.Header.t,
  status: int,
};

type head_response = {
  headers: Cohttp.Header.t,
  status: int,
};

let make: (~headers: Cohttp.Header.t=?, ~baseUrl: string, unit) => t;
let get: (~path: string, t) => Lwt.t(response);
let post: (~path: string, ~body: Yojson.Safe.t, t) => Lwt.t(response);
let put: (~path: string, ~body: Yojson.Safe.t, t) => Lwt.t(response);
let delete: (~path: string, ~body: Yojson.Safe.t, t) => Lwt.t(response);
let head: (~path: string, t) => Lwt.t(head_response);
