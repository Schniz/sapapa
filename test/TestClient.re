open TestFramework;
open Sapapa;

describe("Client", ({test, _}) => {
  let asyncTest = (name, fn) => test(name, x =>
                                   fn(x) |> Lwt_main.run
                                 );

  asyncTest("version", ({expect, _}) => {
    let%lwt version = testClient() |> Client.version;
    expect.string(version).toMatch("^\\d+.\\d+.\\d+$");
    Lwt.return_unit;
  });

  test("users database is _users database", ({expect, _}) => {
    let handwritten =
      testClient() |> Client.db(~name=UrlComponent.make("_users"));
    let usersDb = testClient() |> Client.usersDb;
    expect.equal(usersDb, handwritten);
  });
});
