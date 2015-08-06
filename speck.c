#include "pgspeck.h"


#define _CIRCULAR_SHIFT(val, n, wordsize, op1, op2) (((val) op1 (n)) | ((val) op2 ((wordsize) - (n))))

#define ROL16(val, n) _CIRCULAR_SHIFT(val, n, 16, <<, >>)
#define ROR16(val, n) _CIRCULAR_SHIFT(val, n, 16, >>, <<)

#define ROL24(val, n) (_CIRCULAR_SHIFT(val, n, 24, <<, >>) & 0xFFFFFF)
#define ROR24(val, n) (_CIRCULAR_SHIFT(val, n, 24, >>, <<) & 0xFFFFFF)


static void speck_key_expand32(const int64 K, uint16 *k);
static void speck_key_expand48(const int64 *K, uint32 *k);


static void
speck_key_expand32(const int64 K, uint16 *k)
{
	int i;
	uint16 l[24];

	l[2] = (K & 0xFFFF000000000000) >> 48;
	l[1] = (K & 0x0000FFFF00000000) >> 32;
	l[0] = (K & 0x00000000FFFF0000) >> 16;

	for (i = 0; i < 22-1; ++i)
	{
		l[i+3] = ((uint16) (k[i] + ROR16(l[i], 7))) ^ i;
		k[i+1] = (ROL16(k[i], 2) ^ l[i+3]);
	}
}

uint32
speck_encrypt32(const uint32 xy, const int64 K)
{
	uint32 i;
	uint16 ki;
	uint16 lr[3];
	int lp;
	uint16 x = (xy & 0xFFFF0000) >> 16, y = (xy & 0xFFFF);

	lp = 0;
	lr[2] = (K & 0xFFFF000000000000) >> 48;
	lr[1] = (K & 0x0000FFFF00000000) >> 32;
	lr[0] = (K & 0x00000000FFFF0000) >> 16;
	ki    = (K & 0x000000000000FFFF) >> 0;

	x = (uint16) (ROR16(x, 7) + y);
	x ^= ki;
	y = ROL16(y, 2) ^ x;

	for (i = 0; i < 22-1; i++)
	{
		lr[lp] = ((uint16_t) (ki + ROR16(lr[lp], 7))) ^ i;
		ki = (ROL16(ki, 2) ^ lr[lp]);
		lp = (lp + 1) % 3;

		x = (uint16) (ROR16(x, 7) + y);
		x ^= ki;
		y = ROL16(y, 2) ^ x;
	}
	return (x << 16) | y;
}

uint32
speck_decrypt32(const uint32 xy, const int64 K)
{
	signed int i;
	uint16 k[22];
	uint16 x = (xy & 0xFFFF0000) >> 16, y = (xy & 0xFFFF);

	k[0] = (K & 0xFFFF);
	speck_key_expand32(K, k);

	for (i = 21; i >= 0; i--)
	{
		y ^= x;
		y = ROR16(y, 2);
		x = (uint16) ((x ^ k[i]) - y);
		x = ROL16(x, 7);
	}
	return (x << 16) | y;
}

static void
speck_key_expand48(const int64 *K, uint32 *k)
{
	int i;
	uint32 l[25];

	l[2] = (K[0] & 0xFFFFFF000000) >> 24;
	l[1] = (K[0] & 0x000000FFFFFF) >> 0;
	l[0] = (K[1] & 0xFFFFFF000000) >> 24;

	for (i = 0; i < 23-1; ++i)
	{
		l[i+3] = ((k[i] + ROR24(l[i], 8)) & 0xFFFFFF) ^ i;
		k[i+1] = (ROL24(k[i], 3) ^ l[i+3]);
	}
}

int64
speck_encrypt48(const int64 xy, const int64 *K)
{
	int i;
	uint32 k[23];
	uint32 x = (xy & 0xFFFFFF000000) >> 24, y = (xy & 0xFFFFFF);

	k[0] = (K[1] & 0xFFFFFF);
	speck_key_expand48(K, k);

	for (i = 0; i < 23; i++)
	{
		x = ((ROR24(x, 8) + y) & 0xFFFFFF);
		x ^= k[i];
		y = ROL24(y, 3) ^ x;
	}
	return (((int64) x) << 24) | (int64) y;
}

int64
speck_decrypt48(const int64 xy, const int64 *K)
{
	signed int i;
	uint32 k[23];
	uint32 x = (xy & 0xFFFFFF000000) >> 24, y = (xy & 0xFFFFFF);

	k[0] = (K[1] & 0xFFFFFF);
	speck_key_expand48(K, k);

	for (i = 22; i >= 0; i--)
	{
		y ^= x;
		y = ROR24(y, 3);
		x = (((x ^ k[i]) - y) & 0xFFFFFF);
		x = ROL24(x, 8);
	}
	return (((int64) x) << 24) | (int64) y;
}
