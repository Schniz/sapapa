[@deriving show]
type t = HttpClient.t;

module Requests = {
  [@deriving yojson({strict: false})]
  type root = {version: string};
};

type http_errors;

exception Cant_decode_result(string);

let makeWithHeaders = HttpClient.make;
let make = (~baseUrl) => makeWithHeaders(~baseUrl, ());
let db = (~name, client) => Database.make(~name, ~client);
let usersDb = db(~name=UrlComponent.make("_users"));
let version = t => {
  let%lwt {body, _} = HttpClient.get(t, ~path="/");
  switch (Requests.root_of_yojson(body)) {
  | Ok(x) => Lwt.return(x.version)
  | Error(err) => Lwt.fail(Cant_decode_result(err))
  };
};
