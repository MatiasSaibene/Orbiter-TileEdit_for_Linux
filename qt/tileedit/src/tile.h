#pragma once

#ifndef TILE_H
#define TILE_H


#include "tilecommon.h"

#include "tileblock.h"

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
#include <vector>
#include "ddsread.h"
#include "ZTreeMgr.h"



#define TILE_SURFSTRIDE 512







void ensureLayerDir(const char *rootDir, const char *layer, int lvl, int ilat);




class DXT1Tile: public Tile
{
	friend class TileBlock;

public:
	DXT1Tile(int lvl, int ilat, int ilng);
	DXT1Tile(const DXT1Tile &tile);
	virtual void set(const Tile *tile);
	Image &getData() { return m_idata; }
	const Image &getData() const { return m_idata; }
	int TileSize() const;

protected:
	void SaveDXT1();
	void SavePNGtmp();
	bool LoadDXT1(const ZTreeMgr *mgr = 0, TileLoadMode mode = TILELOADMODE_USEGLOBALSETTING);
	bool LoadPNGtmp();
	void LoadSubset(const ZTreeMgr *mgr = 0);
	void LoadData(Image &im, int lvl, int ilat, int ilng, const ZTreeMgr *mgr);
	TileBlock *ProlongToChildren() const;

	Image m_idata;
};

class SurfTile: public DXT1Tile
{
public:
	SurfTile(int lvl, int ilat, int ilng);
	static SurfTile *Load(int lvl, int ilat, int ilng, TileLoadMode mode = TILELOADMODE_USEGLOBALSETTING);
	static SurfTile *InterpolateFromAncestor(int lvl, int ilat, int ilng);
	void Save();
	static void setTreeMgr(const ZTreeMgr *mgr);
	const std::string Layer() const { return std::string("Surf"); }
	bool mapToAncestors(int minlvl) const;
	
protected:
	bool InterpolateFromAncestor();
	static const ZTreeMgr *s_treeMgr;
};

class MaskTile : public DXT1Tile
{
public:
	static MaskTile *Load(int lvl, int ilat, int ilng);
	static void setTreeMgr(const ZTreeMgr *mgr);
	const std::string Layer() const { return std::string("Mask"); }

protected:
	MaskTile(int lvl, int ilat, int ilng);
	static const ZTreeMgr *s_treeMgr;
};

#endif // TILE_H
