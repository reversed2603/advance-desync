#pragma once

#define LOADER_VERSION xorstr_("1")

#define SERVER_IP "37.230.137.208"
//#define SERVER_IP "192.168.0.105"

#define SERVER_PORT 2580

#define SECURITY_CHECKS_ENABLED 0
#define BAN_ENABLED 1

#define REQUEST_SIGNATURE "\x92\xC7\x59\x0E\xF6\x31\x1C\xD9"
#define CLIENT_KEY (uint8_t*)"\xE1\x46\x38\xD5\x25\x4F\x11\x8E\x6A\x34\x81\xA0\xDC\x19\x5B\xC4"

#include "..\valve\interfaces\fnv1a.h"