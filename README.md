pgspeck
=======

pgspeck is a PostgreSQL extension for symmetrical encryption using the
[Speck](https://en.wikipedia.org/wiki/Speck_(cipher)) encryption algorithm.
Only 32-bit and 48-bit block sizes, and 64-bit and 96-bit keys (respectively)
are supported.  The primary use case is generating a random-looking sequence
from an increasing sequence.

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
-- of each argument are used.
CREATE FUNCTION pgspeck_encrypt48(plaintext int8, key1 int8, key2 int8)
	RETURNS int8
	AS 'pgspeck', 'pgspeck_encrypt48' LANGUAGE c STRICT;

-- Decrypts a value encrypted with pgspeck_encrypt48.
CREATE FUNCTION pgspeck_decrypt48(ciphertext int8, key1 int8, key2 int8)
	RETURNS int8
	AS 'pgspeck', 'pgspeck_decrypt48' LANGUAGE c STRICT;
```
