#include "cipher.h"

#include "..\props.h"

#ifndef VIRTUALIZER_FISH_WHITE_START
#define VIRTUALIZER_FISH_WHITE_START \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43\
  __asm _emit 0x56\
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0x6A \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43\
  __asm _emit 0x56\
  __asm _emit 0x20 \
  __asm _emit 0x20 
#endif

#ifndef VIRTUALIZER_FISH_WHITE_END
#define VIRTUALIZER_FISH_WHITE_END \
  __asm _emit 0xEB \
  __asm _emit 0x10 \
  __asm _emit 0x43\
  __asm _emit 0x56\
  __asm _emit 0x20 \
  __asm _emit 0x20 \
  __asm _emit 0xFA \
  __asm _emit 0x01 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x00 \
  __asm _emit 0x43\
  __asm _emit 0x56\
  __asm _emit 0x20 \
  __asm _emit 0x20 
#endif

c_cipher_light* g_cipher_light = new c_cipher_light;

__forceinline static uint32_t rotl(uint32_t value, int shift)
{
    int val_1 = (value << shift);
    int val_2 = (value >> (32 - shift));

    return val_1 | val_2;
}

__forceinline static void cipher_quarterround(uint32_t* y0, uint32_t* y1, uint32_t* y2, uint32_t* y3)
{
    *y1 = *y1 ^ rotl(*y0 + *y3, 7);
    *y2 = *y2 ^ rotl(*y1 + *y0, 9);
    *y3 = *y3 ^ rotl(*y2 + *y1, 13);
    *y0 = *y0 ^ rotl(*y3 + *y2, 18);
}

__forceinline static void cipher_rowround(uint32_t y[16])
{
    cipher_quarterround(&y[0], &y[1], &y[2], &y[3]);
    cipher_quarterround(&y[5], &y[6], &y[7], &y[4]);
    cipher_quarterround(&y[10], &y[11], &y[8], &y[9]);
    cipher_quarterround(&y[15], &y[12], &y[13], &y[14]);
}

__forceinline static void cipher_columnround(uint32_t x[16])
{
    cipher_quarterround(&x[0], &x[4], &x[8], &x[12]);
    cipher_quarterround(&x[5], &x[9], &x[13], &x[1]);
    cipher_quarterround(&x[10], &x[14], &x[2], &x[6]);
    cipher_quarterround(&x[15], &x[3], &x[7], &x[11]);
}

__forceinline static void cipher_doubleround(uint32_t x[16])
{
    cipher_quarterround(&x[0], &x[4], &x[8], &x[12]);
    cipher_quarterround(&x[5], &x[9], &x[13], &x[1]);
    cipher_quarterround(&x[10], &x[14], &x[2], &x[6]);
    cipher_quarterround(&x[15], &x[3], &x[7], &x[11]);
    cipher_quarterround(&x[0], &x[1], &x[2], &x[3]);
    cipher_quarterround(&x[5], &x[6], &x[7], &x[4]);
    cipher_quarterround(&x[10], &x[11], &x[8], &x[9]);
    cipher_quarterround(&x[15], &x[12], &x[13], &x[14]);
}

__forceinline static uint32_t cipher_littleendian(uint8_t* b)
{
    int val_1 = b[0];
    int val_2 = ((uint_fast16_t)b[1] << 8);
    int val_3 = ((uint_fast32_t)b[2] << 16);
    int val_4 = ((uint_fast32_t)b[3] << 24);

    return val_1 + val_2 + val_3 + val_4;
}

__forceinline static void cipher_rev_littleendian(uint8_t* b, uint32_t w)
{
    b[0] = w;
    b[1] = w >> 8;
    b[2] = w >> 16;
    b[3] = w >> 24;
}

__forceinline static void cipher_hash(uint8_t seq[64])
{
    int i;

    uint32_t x[16];
    uint32_t z[16];

    for (i = 0; i < 16; ++i)
    {
        x[i] = z[i] = cipher_littleendian(seq + (4 * i));
    }

    for (i = 0; i < 10; ++i)
    {
        cipher_doubleround(z);
    }

    for (i = 0; i < 16; ++i) {

        z[i] += x[i];

        cipher_rev_littleendian(seq + (4 * i), z[i]);
    }
}

__forceinline static void cipher_expand16(uint8_t* k,
    uint8_t n[16],
    uint8_t keystream[64])
{
    int i, j;

    uint8_t t[4][4] = {
      { 'j', 'k', 'g', 'h' },
      { 'b', 'z', 's', 'i' },
      { 'o', 't', '[', 'a' },
      { '1', '2', 'a', 'w' }
    };

    for (i = 0; i < 64; i += 20)
    {
        for (j = 0; j < 4; ++j)
        {
            keystream[i + j] = t[i / 20][j];
        }
    }

    for (i = 0; i < 16; ++i) {
        keystream[4 + i] = k[i];
        keystream[44 + i] = k[i];
        keystream[24 + i] = n[i];
    }
    cipher_hash(keystream);
}


__forceinline static void cipher_expand32(uint8_t* k,
    uint8_t n[16],
    uint8_t keystream[64])
{
    int i, j;
    uint8_t o[4][4] = {
      { 'p', 'o', 'q', 'u' },
      { 'x', 'h', 'z', 'm' },
      { 'l', 'a', '-', '2' },
      { '1', 'd', 't', 'r' }
    };

    for (i = 0; i < 64; i += 20)
    {
        for (j = 0; j < 4; ++j)
        {
            keystream[i + j] = o[i / 20][j];
        }
    }
    for (i = 0; i < 16; ++i) {
        keystream[4 + i] = k[i];
        keystream[44 + i] = k[i + 16];
        keystream[24 + i] = n[i];
    }

    cipher_hash(keystream);
}

void c_cipher_light::crypt(void* buffer, int size, uint8_t* key_128, DWORD si)
{
    VIRTUALIZER_FISH_WHITE_START

    uint8_t keystream[64];
    uint8_t n[16] = { 0 };
    uint32_t i;

    for (i = 0; i < 8; ++i)
    {
        n[i] = REQUEST_SIGNATURE[i];
    }

    if (si % 64 != 0) 
    {
        cipher_rev_littleendian(n + 8, si / 64);
        cipher_expand16(key_128, n, keystream);
    }

    for (i = 0; i < size; ++i) 
    {
        if ((si + i) % 64 == 0) 
        {
            cipher_rev_littleendian(n + 8, ((si + i) / 64));
            cipher_expand16(key_128, n, keystream);
        }

        ((uint8_t*)buffer)[i] ^= keystream[(si + i) % 64];
    }

    static_breaker_crypt((uint8_t*)buffer, size, key_128);

    VIRTUALIZER_FISH_WHITE_END
}

void c_cipher_light::decrypt(void* buffer, int size, uint8_t* key_128, DWORD si)
{
    VIRTUALIZER_FISH_WHITE_START

    uint8_t keystream[64];
    uint8_t n[16] = { 0 };
    uint32_t i;

    for (i = 0; i < 8; ++i)
    {
        n[i] = REQUEST_SIGNATURE[i];
    }

    if (si % 64 != 0)
    {
        cipher_rev_littleendian(n + 8, si / 64);
        cipher_expand16(key_128, n, keystream);
    }

    static_breaker_decrypt((uint8_t*)buffer, size, key_128);

    for (i = 0; i < size; ++i)
    {
        if ((si + i) % 64 == 0)
        {
            cipher_rev_littleendian(n + 8, ((si + i) / 64));
            cipher_expand16(key_128, n, keystream);
        }

        ((uint8_t*)buffer)[i] ^= keystream[(si + i) % 64];
    }

    VIRTUALIZER_FISH_WHITE_END
}

void c_cipher_light::static_breaker_crypt(uint8_t* buffer, int size, uint8_t* key_128)
{
    BYTE key_hash = 0;

    for (int i = 0; i < 16; ++i)
       key_hash += key_128[i];

    for (int i = 0; i < size; ++i)
    {
        buffer[i] ^= key_hash + (i + key_hash);
        key_hash ^= i % 2 == 0 ? buffer[i] + i : -buffer[i] + i;
    }
}

void c_cipher_light::static_breaker_decrypt(uint8_t* buffer, int size, uint8_t* key_128)
{
    VIRTUALIZER_FISH_WHITE_START

    BYTE key_hash = 0;

    for (int i = 0; i < 16; ++i)
        key_hash += key_128[i];

    for (int i = 0; i < size; ++i)
        key_hash ^= i % 2 == 0 ? buffer[i] + i : -buffer[i] + i;

    for (int i = size - 1; i != -1; --i)
    {
        key_hash ^= i % 2 == 0 ? buffer[i] + i : -buffer[i] + i;
        buffer[i] ^= key_hash + (i + key_hash);
    }

    VIRTUALIZER_FISH_WHITE_END
}
