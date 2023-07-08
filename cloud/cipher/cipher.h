#pragma once
#include <Windows.h>
#include <stdint.h>

class c_cipher_light
{
public:
	void crypt(void* buffer, int size, uint8_t* key_128, DWORD si);
	void decrypt(void* buffer, int size, uint8_t* key_128, DWORD si);
	void static_breaker_crypt(uint8_t* buffer, int size, uint8_t* key_128);
	void static_breaker_decrypt(uint8_t* buffer, int size, uint8_t* key_128);
};

extern c_cipher_light* g_cipher_light;