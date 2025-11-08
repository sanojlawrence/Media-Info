#ifndef ANDROID_COMPAT_H
#define ANDROID_COMPAT_H

#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

// Windows type definitions for Android
typedef int32_t INT;
typedef uint32_t UINT;
typedef int32_t LONG;
typedef uint32_t DWORD;
typedef uint16_t WORD;
typedef uint8_t BYTE;
typedef char CHAR;
typedef unsigned char UCHAR;
typedef int BOOL;

// String function mappings
#define strnicmp strncasecmp
#define stricmp strcasecmp

// Socket compatibility
#define WSAGetLastError() errno
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define closesocket close

#endif // ANDROID_COMPAT_H
