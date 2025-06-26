#pragma once


// windows_compat.h
#ifndef WINDOWS_COMPAT_H
#define WINDOWS_COMPAT_H

#include <cstdint>
#include <cstddef>

// Tipos básicos de Windows
typedef uint8_t     BYTE;
typedef uint8_t     UINT8;
typedef uint16_t    WORD;
typedef uint32_t    DWORD;
typedef int32_t     LONG;
typedef uint64_t    QWORD;
typedef int64_t     LONGLONG;
typedef uint64_t    ULONGLONG;

typedef int         BOOL;
typedef uint16_t    WCHAR;
typedef char*       LPSTR;
typedef const char* LPCSTR;
typedef wchar_t*    LPWSTR;
typedef const wchar_t* LPCWSTR;
typedef void*       LPVOID;
typedef const void* LPCVOID;

// Macros comunes
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL nullptr
#endif

// Handle genérico
typedef void* HANDLE;

// Resultados
typedef uint32_t HRESULT;
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#define FAILED(hr) (((HRESULT)(hr)) < 0)

// Combinación de macros para crear valores DWORD
#define MAKEWORD(a, b)      ((WORD)(((BYTE)((a) & 0xff)) | ((WORD)((BYTE)((b) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)((a) & 0xffff)) | ((DWORD)((WORD)((b) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)((DWORD_PTR)(l) & 0xffff))
#define HIWORD(l)           ((WORD)((DWORD_PTR)(l) >> 16))
#define LOBYTE(w)           ((BYTE)((w) & 0xff))
#define HIBYTE(w)           ((BYTE)((w) >> 8))

// Para punteros 32/64 bits
#if defined(__x86_64__) || defined(_M_X64)
typedef uint64_t DWORD_PTR;
#else
typedef uint32_t DWORD_PTR;
#endif

#endif // WINDOWS_COMPAT_H
