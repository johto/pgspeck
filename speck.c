#include "pgspeck.h"


#define _CIRCULAR_SHIFT(val, n, wordsize, op1, op2) (((val) op1 (n)) | ((val) op2 ((wordsize) - (n))))

#define ROL16(val, n) _CIRCULAR_SHIFT(val, n, 16, <<, >>)
#define ROR16(val, n) _CIRCULAR_SHIFT(val, n, 16, >>, <<)

#define ROL24(val, n) (_CIRCULAR_SHIFT(val, n, 24, <<, >>) & 0xFFFFFF)
#define ROR24(val, n) (_CIRCULAR_SHIFT(val, n, 24, >>, <<) & 0xFFFFFF)

typedef struct {
	int64 K;
	uint16 k[22];
} ExpandedKeyCacheEntry32;

static void speck_key_expand32(const int64 K, ExpandedKeyCacheEntry32 *kce);
static void speck_key_expand48(const int64 *K, uint32 *k);


static ExpandedKeyCacheEntry32 keycache32;

static void
speck_key_expand32(const int64 K, ExpandedKeyCacheEntry32 *kce)
{
	const int rounds = 22;
	int i;
	uint16 l[24];

	l[2] = (K & 0xFFFF000000000000) >> 48;
	l[1] = (K & 0x0000FFFF00000000) >> 32;
	l[0] = (K & 0x00000000FFFF0000) >> 16;

	kce->k[0] = (K & 0xFFFF);
	for (i = 0; i <= rounds - 2; ++i)
	{
		l[i+3] = ((uint16) (kce->k[i] + ROR16(l[i], 7))) ^ i;
		kce->k[i+1] = (ROL16(kce->k[i], 2) ^ l[i+3]);
	}
	kce->K = K;
}

uint32
speck_encrypt32(const uint32 xy, const int64 K)
{
	const int rounds = 22;
	int i;
	uint16 x = (xy & 0xFFFF0000) >> 16, y = (xy & 0xFFFF);
	ExpandedKeyCacheEntry32 *kce;

	kce = &keycache32;
	if (keycache32.K != K)
		speck_key_expand32(K, kce);

	for (i = 0; i <= rounds - 1; i++)
	{
		x = (uint16) (ROR16(x, 7) + y);
		x ^= kce->k[i];
		y = ROL16(y, 2) ^ x;
	}
	return (x << 16) | y;
}

uint32
speck_decrypt32(const uint32 xy, const int64 K)
{
	const int rounds = 22;
	int i;
	uint16 x = (xy & 0xFFFF0000) >> 16, y = (xy & 0xFFFF);
	ExpandedKeyCacheEntry32 *kce;

	kce = &keycache32;
	if (keycache32.K != K)
		speck_key_expand32(K, kce);

	for (i = rounds - 1; i >= 0; i--)
	{
		y ^= x;
		y = ROR16(y, 2);
		x = (uint16) ((x ^ kce->k[i]) - y);
		x = ROL16(x, 7);
	}
	return (x << 16) | y;
}

static void
speck_key_expand48(const int64 *K, uint32 *k)
{
	const int rounds = 23;
	int i;
	uint32 l[25];

	l[2] = (K[0] & 0xFFFFFF000000) >> 24;
	l[1] = (K[0] & 0x000000FFFFFF) >> 0;
	l[0] = (K[1] & 0xFFFFFF000000) >> 24;

	for (i = 0; i <= rounds - 2; ++i)
	{
		l[i+3] = ((k[i] + ROR24(l[i], 8)) & 0xFFFFFF) ^ i;
		k[i+1] = (ROL24(k[i], 3) ^ l[i+3]);
	}
}

int64
speck_encrypt48(const int64 xy, const int64 *K)
{
	const int rounds = 23;
	int i;
	uint32 k[23];
	uint32 x = (xy & 0xFFFFFF000000) >> 24, y = (xy & 0xFFFFFF);

	k[0] = (K[1] & 0xFFFFFF);
	speck_key_expand48(K, k);

	for (i = 0; i <= rounds - 1; i++)
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
	const int rounds = 23;
	int i;
	uint32 k[23];
	uint32 x = (xy & 0xFFFFFF000000) >> 24, y = (xy & 0xFFFFFF);

	k[0] = (K[1] & 0xFFFFFF);
	speck_key_expand48(K, k);

	for (i = rounds - 1; i >= 0; i--)
	{
		y ^= x;
		y = ROR24(y, 3);
		x = (((x ^ k[i]) - y) & 0xFFFFFF);
		x = ROL24(x, 8);
	}
	return (((int64) x) << 24) | (int64) y;
}
