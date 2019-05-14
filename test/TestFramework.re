include Rely.Make({
  let config =
    Rely.TestFrameworkConfig.initialize({
      snapshotDir: "test/_snapshots",
      projectDir: "",
    });
});

let testClient = () => Sapapa.Client.make(~baseUrl="http://localhost:5984", ());
