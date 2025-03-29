#pragma once

#ifndef ELEVDATA_H
#define ELEVDATA_H

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


struct ElevData {
    DWORD width;              // elevation grid width (including padding)
    DWORD height;             // elevation grid height (including padding)
    std::vector<double> data; // elevation grid data [m]
    double dmin, dmax;        // std::min, std::max tile elevation [m]
    double dres;              // target elevation resolution [m] (must be 2^n with integer n)
    ElevData();
    ElevData(const ElevData &edata);
    ElevData &operator=(const ElevData &edata);
    double nodeValue(int ix, int iy) const;
    void setNodeValue(int ix, int iy, double v);
    ElevData SubTile(const std::pair<DWORD, DWORD> &xrange, const std::pair<DWORD, DWORD> &yrange);
    void RescanLimits();
};

#endif // ELEVDATA_H
