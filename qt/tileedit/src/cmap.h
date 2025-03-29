

#ifndef CMAP_H
#define CMAP_H

#ifdef __linux__
#include <stdint.h>
#include <wchar.h>

// Tipos enteros sin signo
#define DWORD uint32_t
#define ULONG uint32_t
#define ULONG64 uint64_t
#define BYTE uint8_t
#define WORD uint16_t
#define SHORT int16_t

// Tipos enteros con signo
#define LONG int32_t
#define LONGLONG int64_t
#define __int64 int64_t

// Punteros y handles
#define DWORD_PTR uintptr_t
#define LPVOID void*

// Cadenas
#define LPCSTR const char*
#define LPSTR char*
#define LPCWSTR const wchar_t*
#define LPWSTR wchar_t*

// Tipo BOOL
#define BOOL int

#else
// En Windows, usar los tipos originales
#include <windows.h>
#endif

enum CmapName {
	CMAP_GREY,
	CMAP_JET,
	CMAP_TOPO1,
	CMAP_TOPO2
};

typedef DWORD Cmap[256];

const Cmap &cmap(CmapName name);

#endif // !CMAP_H
