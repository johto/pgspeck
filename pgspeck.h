#ifndef __PG_SPECK_SPECK_HEADER__
#define __PG_SPECK_SPECK_HEADER__

#include "postgres.h"

extern uint32 speck_encrypt32(const uint32 xy, const int64 K);
extern uint32 speck_decrypt32(const uint32 xy, const int64 K);
extern int64 speck_encrypt48(const int64 xy, const int64 *K);
extern int64 speck_decrypt48(const int64 xy, const int64 *K);

#endif
