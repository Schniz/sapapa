[@deriving show]
type t = string;
let make = str => Netencoding.Url.encode(str);
let encoded = t => Netencoding.Url.decode(t);
