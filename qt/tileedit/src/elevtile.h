
#ifndef ELEVTILE_H
#define ELEVTILE_H

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

#include "tile.h"
#include "cmap.h"
#include "elevdata.h"
#include "ZTreeMgr.h"


#define TILE_FILERES 256
#define TILE_ELEVSTRIDE (TILE_FILERES+3)

class MaskTile;

struct ElevDisplayParam {
	CmapName cmName;
	bool useWaterMask;
	bool autoRange;
	double rangeMin;
	double rangeMax;

	ElevDisplayParam() {
		cmName = CMAP_GREY;
		useWaterMask = false;
		autoRange = true;
		rangeMin = 0.0;
		rangeMax = 1000.0;
	}
};


class ElevTile : public Tile {
	friend class TileBlock;
	friend class ElevTileBlock;

public:
	ElevTile(const ElevTile &etile);
	static ElevTile *Load(int lvl, int ilat, int ilng);
	static ElevTile *InterpolateFromAncestor(int lvl, int ilat, int ilng, const Cmap *cm = 0);
	static void setTreeMgr(const ZTreeMgr *mgr, const ZTreeMgr *modMgr = 0);
	const std::string Layer() const { return std::string("Elev"); }
	double nodeElevation(int ndx, int ndy);

	virtual void set(const Tile *tile);

	ElevData &getData() { return m_edata; }
	const ElevData &getData() const { return m_edata; }
	ElevData &getBaseData() { return m_edataBase; }
	const ElevData &getBaseData() const { return m_edataBase; }
	bool isModified() const { return m_modified; }
	void dataChanged(int exmin = -1, int exmax = -1, int eymin = -1, int eymax = -1);
	void Save();
	void SaveMod();
	void MatchNeighbourTiles();
	bool mapToAncestors(int minlvl) const;

	/**
	 * \brief Interpolate the tile to the next resolution level
	 */
	TileBlock *ProlongToChildren() const;

	void setWaterMask(const MaskTile *mtile);

protected:
	ElevTile(int lvl, int ilat, int ilng);
	bool Load(bool directOnly = false);
	bool InterpolateFromAncestor();
	void LoadSubset();
	void LoadData(ElevData &edata, int lvl, int ilat, int ilng);
	void LoadModData(ElevData &edata, int lvl, int ilat, int ilng);
	void RescanLimits();

private:
	ElevData m_edata;
	ElevData m_edataBase;
	bool m_modified;
	std::vector<bool> m_waterMask;

	static const ZTreeMgr *s_treeMgr;
	static const ZTreeMgr *s_treeModMgr;
};

#endif // !ELEVTILE_H
