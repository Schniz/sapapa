open TestFramework;
open Sapapa;

describe("Database", ({test, _}) => {
  let asyncTest = (name, fn) => test(name, x =>
                                   fn(x) |> Lwt_main.run
                                 );

  asyncTest("exists", ({expect}) => {
    let%lwt _ =
      Lwt.catch(
        () =>
          testClient() |> Client.usersDb |> Database.create |> Lwt.map(ignore),
        _ => Lwt.return_unit,
      );

    let%lwt exists = testClient() |> Client.usersDb |> Database.exists;
    expect.bool(exists).toBe(true);
    Lwt.return_unit;
  });

  asyncTest("creates and destroys databases", ({expect}) => {
    let database =
      testClient()
      |> Client.db(~name=UrlComponent.make("test_database_create_destroy"));

    let%lwt _ =
      if%lwt (Database.exists(database)) {
        Database.delete(database);
      } else {
        Lwt.return_ok(database);
      };

    let%lwt dbExists = Database.exists(database);
    expect.bool(dbExists).toBe(false);

    let%lwt _ =
      switch%lwt (Database.create(database)) {
      | Ok(_) => Lwt.return_unit
      | Error(_) => Lwt.fail_with("Unexpected database")
      };

    let%lwt dbExists = Database.exists(database);
    expect.bool(dbExists).toBe(true);

    let%lwt _ =
      switch%lwt (Database.delete(database)) {
      | Ok(_) => Lwt.return_unit
      | Error(_) => Lwt.fail_with("Unexpected database")
      };

    let%lwt dbExists = Database.exists(database);
    expect.bool(dbExists).toBe(false);

    Lwt.return_unit;
  });
});
