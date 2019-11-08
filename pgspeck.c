#include "pgspeck.h"
#include "postgres.h"
#include "funcapi.h"
#include "fmgr.h"


PG_MODULE_MAGIC;

Datum pgspeck_encrypt32(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(pgspeck_encrypt32);

Datum
pgspeck_encrypt32(PG_FUNCTION_ARGS)
{
	int64 plaintext = PG_GETARG_INT64(0);
	int64 key = PG_GETARG_INT64(1);

	PG_RETURN_INT64(speck_encrypt32((uint32) plaintext, key));
}

Datum pgspeck_decrypt32(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(pgspeck_decrypt32);

Datum
pgspeck_decrypt32(PG_FUNCTION_ARGS)
{
	int64 ciphertext = PG_GETARG_INT64(0);
	int64 key = PG_GETARG_INT64(1);

	PG_RETURN_INT64(speck_decrypt32((uint32) ciphertext, key));
}


Datum pgspeck_encrypt48(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(pgspeck_encrypt48);

Datum
pgspeck_encrypt48(PG_FUNCTION_ARGS)
{
	int64 plaintext = PG_GETARG_INT64(0);
	int64 key1 = PG_GETARG_INT64(1);
	int64 key2 = PG_GETARG_INT64(2);
	const int64 keys[2] = { key1, key2 };

	if ((key1 >> 48) != 0 || (key2 >> 48) != 0)
		ereport(ERROR,
				(errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
				 errmsg("only the lowermost 48 bits of key's components can be non-zero")));

	PG_RETURN_INT64(speck_encrypt48(plaintext, keys));
}

Datum pgspeck_decrypt48(PG_FUNCTION_ARGS);
PG_FUNCTION_INFO_V1(pgspeck_decrypt48);

Datum
pgspeck_decrypt48(PG_FUNCTION_ARGS)
{
	int64 ciphertext = PG_GETARG_INT64(0);
	int64 key1 = PG_GETARG_INT64(1);
	int64 key2 = PG_GETARG_INT64(2);
	const int64 keys[2] = { key1, key2 };

	if ((key1 >> 48) != 0 || (key2 >> 48) != 0)
		ereport(ERROR,
				(errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
				 errmsg("only the lowermost 48 bits of key's components can be non-zero")));

	PG_RETURN_INT64(speck_decrypt48(ciphertext, keys));
}
