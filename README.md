
# reason_couchdb_manager


[![CircleCI](https://circleci.com/gh/yourgithubhandle/reason_couchdb_manager/tree/master.svg?style=svg)](https://circleci.com/gh/yourgithubhandle/reason_couchdb_manager/tree/master)


**Contains the following libraries and executables:**

```
reason_couchdb_manager@0.0.0
│
├─test/
│   name:    TestReason_couchdb_manager.exe
│   require: reason_couchdb_manager/library
│
├─library/
│   library name: reason_couchdb_manager/library
│   require:
│
└─executable/
    name:    Reason_couchdb_managerApp.exe
    require: reason_couchdb_manager/library
```

## Developing:

```
npm install -g esy
git clone <this-repo>
esy install
esy build
```

## Running Binary:

After building the project, you can run the main binary that is produced.

```
esy x Reason_couchdb_managerApp.exe 
```

## Running Tests:

```
# Runs the "test" command in `package.json`.
esy test
```
