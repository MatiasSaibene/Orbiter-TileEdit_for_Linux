

#ifndef IMAGETOOLS_H
#define IMAGETOOLS_H

#include <vector>
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

struct Image {
	std::vector<DWORD> data;
	DWORD width;
	DWORD height;

	Image() { width = height = 0; }
	Image &operator=(const Image &img);
	Image SubImage(const std::pair<DWORD, DWORD> &xrange, const std::pair<DWORD, DWORD> &yrange);
};

void match_histogram(Image &im1, const Image &im2);
void match_hue_sat(Image &im1, const Image &im2);

#endif // !IMAGETOOLS_H
