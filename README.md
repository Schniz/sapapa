# Sapapa 😌

[![Build Status](https://dev.azure.com/galstar0385/sapapa/_apis/build/status/Schniz.sapapa?branchName=master)](https://dev.azure.com/galstar0385/sapapa/_build/latest?definitionId=5&branchName=master)

> A soothing CouchDB client for native Reason and OCaml. Just Relax. 😌

Under the hood, it calmly leverages [Yojson](https://github.com/ocaml-community/yojson) for JSON parsing and [Lwt](https://github.com/ocsigen/lwt) for non-blocking I/O. Every API call returns a `result` type which has a strongly-typed error in return, so you know exactly what failed, in order to build safer apps.

In the future, we may want to support different JSON engines and async libraries (like Jane Street's Async), but for now, this gives a fast and safe environment.

The state of mind behind this library is that you can get `Client` can give you a _reference_ to a `Database`, whether it exists or not, and a `Database` can give you a _reference_ to a `Document`, whether it exists or not:

```reason
let client =
  Sapapa.Client.make(~baseUrl="http://couchdb-instance.example.com:5984");

let db =
  Sapapa.Client.db(~name=Sapapa.UrlComponent.make("my_first_database")); // <- doesn't create a db, just a nice reference

Sapapa.Database.create(db) // <- this creates the db!
|> Lwt.map(res =>
     switch (res) {
     | Ok(db) => print_endline("Database created!")
     | Error(error) =>
       print_endline(
         "Database creation failed: "
         ++ Sapapa.Database.show_create_error(error),
       )
     }
   );
```

## Installation:

```bash
esy add sapapa
```

Annotated file tree:

```
sapapa
├─test/      <- these are the tests
└─library/   <- this is the source code
```

## Contribution:

```
npm install -g esy
git clone git@github.com:Schniz/sapapa.git
esy install
```

## Running Tests:

```
docker-compose up -d
esy test --watch
```
