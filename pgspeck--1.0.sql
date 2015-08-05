-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pgspeck" to load this file. \quit

CREATE FUNCTION pgspeck_encrypt32(plaintext int8, key int8)
	RETURNS int8
	AS 'pgspeck', 'pgspeck_encrypt32' LANGUAGE c STRICT;

CREATE FUNCTION pgspeck_decrypt32(ciphertext int8, key int8)
	RETURNS int8
	AS 'pgspeck', 'pgspeck_decrypt32' LANGUAGE c STRICT;

CREATE FUNCTION pgspeck_encrypt48(plaintext int8, key1 int8, key2 int8)
	RETURNS int8
	AS 'pgspeck', 'pgspeck_encrypt48' LANGUAGE c STRICT;

CREATE FUNCTION pgspeck_decrypt48(ciphertext int8, key1 int8, key2 int8)
	RETURNS int8
	AS 'pgspeck', 'pgspeck_decrypt48' LANGUAGE c STRICT;

