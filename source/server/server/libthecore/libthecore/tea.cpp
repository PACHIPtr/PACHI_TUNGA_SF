#include "stdafx.h"
#define TEA_ROUND	32
#define DELTA		0x9E3779B9

char tea_nilbuf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

INLINE void tea_code(const DWORD sz, const DWORD sy, const DWORD* key, DWORD* dest)
{
	DWORD	y = sy, z = sz, sum = 0;

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	y += ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);
	sum += DELTA;
	z += ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);

	*(dest++) = y;
	*dest = z;
}

INLINE void tea_decode(const DWORD sz, const DWORD sy, const DWORD * key, DWORD * dest)
{
	DWORD y = sy, z = sz, sum = DELTA * TEA_ROUND;

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	z -= ((y << 4 ^ y >> 5) + y) ^ (sum + key[sum >> 11 & 3]);
	sum -= DELTA;
	y -= ((z << 4 ^ z >> 5) + z) ^ (sum + key[sum & 3]);

	*(dest++) = y;
	*dest = z;
}

int TEA_Encrypt(DWORD * dest, const DWORD * src, const DWORD * key, int size)
{
	int i;
	int resize;

	if (size % 8 != 0)
	{
		resize = size + 8 - (size % 8);
		memset((char*)src + size, 0, resize - size);
	}
	else
	{
		resize = size;
	}

	for (i = 0; i < resize >> 3; i++, dest += 2, src += 2)
	{
		tea_code(*(src + 1), *src, key, dest);
	}

	return (resize);
}

int TEA_Decrypt(DWORD * dest, const DWORD * src, const DWORD * key, int size)
{
	int i;
	int resize;

	if (size % 8 != 0)
	{
		resize = size + 8 - (size % 8);
	}
	else
	{
		resize = size;
	}

	for (i = 0; i < resize >> 3; i++, dest += 2, src += 2)
	{
		tea_decode(*(src + 1), *src, key, dest);
	}
	return (resize);
}