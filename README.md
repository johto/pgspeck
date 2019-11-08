pgspeck
=======

[![Build Status](https://travis-ci.org/johto/pgspeck.svg?branch=master)](https://travis-ci.org/johto/pgspeck)

pgspeck is a PostgreSQL extension for symmetrical encryption using the
[Speck](https://en.wikipedia.org/wiki/Speck_(cipher)) encryption algorithm.
Only 32-bit and 48-bit block sizes, and 64-bit and 96-bit keys (respectively)
are supported.  The primary use case is generating a random-looking sequence
from an increasing sequence.  Requires PostgreSQL version 9.1 or later.

Example usage:

```SQL
CREATE FUNCTION my_secret_key()
RETURNS int8 IMMUTABLE
AS $$
    SELECT 1234567890987654321
$$ LANGUAGE sql;

CREATE SEQUENCE userid_plaintext_seq;

CREATE TABLE users (
    userid bigint NOT NULL
        DEFAULT pgspeck_encrypt32(nextval('userid_plaintext_seq'), my_secret_key()),
    PRIMARY KEY (userid)
);

=# INSERT INTO users DEFAULT VALUES RETURNING *;
   userid
------------
 3497276207
(1 row)

INSERT 0 1
=# INSERT INTO users DEFAULT VALUES RETURNING *;
   userid
------------
 1514490635
(1 row)

INSERT 0 1
```

The extension adds four functions:

```SQL
-- Encrypts a 32-bit plaintext using a 64-bit key.  Only the lowermost 32 bits
-- of the plaintext are used.  For the key, the full 64-bit range is effective.
CREATE FUNCTION pgspeck_encrypt32(plaintext int8, key int8)
	RETURNS int8
	AS 'pgspeck', 'pgspeck_encrypt32' LANGUAGE c STRICT;

-- Decrypts a value encrypted with pgspeck_encrypt32.
CREATE FUNCTION pgspeck_decrypt32(ciphertext int8, key int8)
	RETURNS int8
	AS 'pgspeck', 'pgspeck_decrypt32' LANGUAGE c STRICT;

-- Encrypts a 48-bit plaintext using a 96-bit key.  Only the lowermost 48 bits
-- of each key component can be non-zero.
CREATE FUNCTION pgspeck_encrypt48(plaintext int8, key1 int8, key2 int8)
	RETURNS int8
	AS 'pgspeck', 'pgspeck_encrypt48' LANGUAGE c STRICT;

-- Decrypts a value encrypted with pgspeck_encrypt48.
CREATE FUNCTION pgspeck_decrypt48(ciphertext int8, key1 int8, key2 int8)
	RETURNS int8
	AS 'pgspeck', 'pgspeck_decrypt48' LANGUAGE c STRICT;
```

This software is released under the MIT license.
