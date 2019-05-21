[@deriving show]
type t = string;
let make = str => Uri.pct_encode(str);
let make_unsafe = str => str;
let encoded = t => t;
let decoded = t => Uri.pct_decode(t);
