module Headers = {
  let defaults = Cohttp.Header.init_with("Accept", "application/json");
  /* let merge = (headerA: Cohttp.Header.t, headerB: Cohttp.Header.t) => { */
  /*   Cohttp.Header.fold( */
  /*     (key, value, header) => Cohttp.Header.add(header, key, value), */
  /*     headerB, */
  /*     headerA, */
  /*   ); */
  /* }; */

  let pp = x => {
    let str =
      Cohttp.Header.to_string(x)
      |> CCString.lines
      |> List.map(line => "    " ++ line)
      |> String.concat("\n");
    "Headers {\n" ++ str ++ "}";
  };
};

[@deriving show]
type t = {
  baseUrl: Uri.t,
  headers:
    [@printer (fmt, x) => fprintf(fmt, "%s", Headers.pp(x))] Cohttp.Header.t,
};

type response = {
  body: Yojson.Safe.t,
  headers: Cohttp.Header.t,
  status: int,
};

type head_response = {
  headers: Cohttp.Header.t,
  status: int,
};

let make = (~headers=Headers.defaults, ~baseUrl, ()) => {
  baseUrl: Uri.of_string(baseUrl),
  headers,
};

let get = (~path, client) => {
  open Lwt.Infix;
  let%lwt (response, body) =
    Uri.with_path(client.baseUrl, path)
    |> Cohttp_lwt_unix.Client.get(~headers=client.headers);
  let status = Cohttp.Response.status(response) |> Cohttp.Code.code_of_status;
  let%lwt body = Cohttp_lwt.Body.to_string(body) >|= Yojson.Safe.from_string;
  Lwt.return({body, status, headers: Cohttp.Response.headers(response)});
};

let delete = (~path, ~body as json, client) => {
  open Lwt.Infix;
  let body = Yojson.Safe.to_string(json) |> Cohttp_lwt.Body.of_string;
  let%lwt (response, body) =
    Uri.with_path(client.baseUrl, path)
    |> Cohttp_lwt_unix.Client.delete(~body, ~headers=client.headers);
  let status = Cohttp.Response.status(response) |> Cohttp.Code.code_of_status;
  let%lwt body = Cohttp_lwt.Body.to_string(body) >|= Yojson.Safe.from_string;
  Lwt.return({body, status, headers: Cohttp.Response.headers(response)});
};

let post = (~path, ~body as json, client) => {
  open Lwt.Infix;
  let body = Yojson.Safe.to_string(json) |> Cohttp_lwt.Body.of_string;
  let%lwt (response, body) =
    Uri.with_path(client.baseUrl, path)
    |> Cohttp_lwt_unix.Client.post(~body, ~headers=client.headers);
  let status = Cohttp.Response.status(response) |> Cohttp.Code.code_of_status;
  let%lwt body = Cohttp_lwt.Body.to_string(body) >|= Yojson.Safe.from_string;
  Lwt.return({body, status, headers: Cohttp.Response.headers(response)});
};

let put = (~path, ~body as json, client) => {
  open Lwt.Infix;
  let body = Yojson.Safe.to_string(json) |> Cohttp_lwt.Body.of_string;
  let%lwt (response, body) =
    Uri.with_path(client.baseUrl, path)
    |> Cohttp_lwt_unix.Client.put(~body, ~headers=client.headers);
  let status = Cohttp.Response.status(response) |> Cohttp.Code.code_of_status;
  let%lwt body = Cohttp_lwt.Body.to_string(body) >|= Yojson.Safe.from_string;
  Lwt.return({body, status, headers: Cohttp.Response.headers(response)});
};

let head = (~path, client) => {
  let%lwt response =
    Uri.with_path(client.baseUrl, path)
    |> Cohttp_lwt_unix.Client.head(~headers=client.headers);
  let status = Cohttp.Response.status(response) |> Cohttp.Code.code_of_status;
  Lwt.return({status, headers: Cohttp.Response.headers(response)});
};
