[@deriving show]
type t = string;
let make = str => Netencoding.Url.encode(str);
let make_unsafe = str => str;
let encoded = t => Netencoding.Url.decode(t);
