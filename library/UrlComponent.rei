[@deriving show]
type t;

/** Encode a string to a URL component */
let make: string => t;

/** Take an already-safe URL component string and makes it a UrlComponent.t */
let make_unsafe: string => t;

/** Returns a safe URL component string */
let encoded: t => string;
