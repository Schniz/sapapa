open TestFramework;
open Sapapa;

describe("Document", ({test, _}) => {
  let db =
    testClient() |> Client.db(~name=UrlComponent.make("test_documents"));

  let resetDb = () => {
    let lwtIgnore = lwt => {
      open Lwt.Infix;
      try%lwt (lwt >|= ignore) {
      | _ => Lwt.return_unit
      };
    };

    let%lwt _ = Database.delete(db) |> lwtIgnore;
    Database.create(db) |> lwtIgnore;
  };

  let asyncTest = (name, fn) =>
    test(name, x =>
      Lwt_main.run(
        {
          let%lwt _ = resetDb();
          fn(x);
        },
      )
    );

  asyncTest("creates a document", ({expect}) => {
    let%lwt _ = resetDb();
    let docName = "user_a";
    let doc = db |> Database.doc(~id=UrlComponent.make(docName));
    let bodyData = [("hello", `String("world"))];
    let body = `Assoc(bodyData);
    let%lwt savedResponse =
      switch%lwt (doc |> Document.create(~body)) {
      | Ok(x) => Lwt.return(x)
      | Error(x) => Lwt.fail_with(Document.show_create_error(x))
      };

    expect.equal(savedResponse |> Document.id, docName);

    let%lwt savedDoc =
      switch%lwt (Document.get(doc)) {
      | Ok(x) => Yojson.Safe.sort(x) |> Lwt.return
      | Error(x) => Lwt.fail_with(Document.show_get_error(x))
      };

    let fullBody =
      `Assoc([
        ("_rev", `String(savedResponse |> Document.rev)),
        ("_id", `String(savedResponse |> Document.id)),
        ...bodyData,
      ])
      |> Yojson.Safe.sort;

    expect.equal(
      savedDoc |> Yojson.Safe.pretty_to_string,
      fullBody |> Yojson.Safe.pretty_to_string,
    );

    Lwt.return_unit;
  });

  asyncTest("updates", ({expect, _}) => {
    let%lwt _ = resetDb();
    let doc =
      testClient()
      |> Client.db(~name=UrlComponent.make("test_documents"))
      |> Database.doc(~id=UrlComponent.make("test_update"));
    let body =
      Yojson.Safe.from_string(
        {|
                    {"greeting": "hello"}
                    |},
      );
    let%lwt creation =
      doc |> Document.create(~body) |> Lwt.map(CCResult.get_exn);

    let full = doc |> Document.withRev(~rev=Document.rev(creation));
    let newBody = Yojson.Safe.from_string({| {"greeting": "hi"} |});
    let%lwt update =
      full |> Document.update(~body=newBody) |> Lwt.map(CCResult.get_exn);

    let%lwt updatedDoc =
      switch%lwt (Document.get(doc)) {
      | Ok(x) => Yojson.Safe.sort(x) |> Lwt.return
      | Error(x) => Lwt.fail_with(Document.show_get_error(x))
      };

    let fullBody =
      `Assoc([
        ("_rev", `String(update |> Document.rev)),
        ("_id", `String(creation |> Document.id)),
        ("greeting", `String("hi")),
      ])
      |> Yojson.Safe.sort;

    expect.equal(
      updatedDoc |> Yojson.Safe.pretty_to_string,
      fullBody |> Yojson.Safe.pretty_to_string,
    );

    Lwt.return_unit;
  });

  ();
});
