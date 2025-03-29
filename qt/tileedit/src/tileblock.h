#pragma once

#ifndef TILEBLOCK_H
#define TILEBLOCK_H

#include "tilecommon.h"
#include "tile.h"
#include "elevtile.h"
#include "imagetools.h"
#include "elevdata.h"

struct ElevDisplayParam;
struct ElevData;

class DXT1TileBlock : public TileBlock
{
public:
	DXT1TileBlock(int lvl, int ilat0, int ilat1, int ilng0, int ilng1);
	virtual void ExtractImage(Image &img, TileMode mode, int exmin = -1, int exmax = -1, int eymin = -1, int eymax = -1) const
	{ img = m_idata; }
	const Image &getData() const { return m_idata; }
	Image &getData() { return m_idata; }

protected:
	Image m_idata;
};


class SurfTileBlock : public DXT1TileBlock
{
public:
	static SurfTileBlock *Load(int lvl, int ilat0, int ilat1, int ilng0, int ilng1);
	SurfTileBlock(int lvl, int ilat0, int ilat1, int ilng0, int ilng1);
	virtual Tile *copyTile(int ilat, int ilng) const;
	virtual bool copyTile(int ilat, int ilng, Tile *tile) const;
	virtual void syncTile(int ilat, int ilng);
};


class MaskTileBlock : public DXT1TileBlock
{
public:
	static MaskTileBlock *Load(int lvl, int ilat0, int ilat1, int ilng0, int ilng1);
	virtual Tile *copyTile(int ilat, int ilng) const;
	virtual bool copyTile(int ilat, int ilng, Tile *tile) const;
	virtual void ExtractImage(Image &img, TileMode mode, int exmin = -1, int exmax = -1, int eymin = -1, int eymax = -1) const;

protected:
	MaskTileBlock(int lvl, int ilat0, int ilat1, int ilng0, int ilng1);
};


class ElevTileBlock : public TileBlock
{
public:
	static ElevTileBlock *Load(int lvl, int ilat0, int ilat1, int ilng0, int ilng1);
	ElevTileBlock(int lvl, int ilat0, int ilat1, int ilng0, int ilng1);
	ElevTileBlock(const ElevTileBlock &etileblock);
	static void setElevDisplayParam(const ElevDisplayParam *elevDisplayParam);
	bool setTile(int ilat, int ilng, const Tile *tile);
	ElevData &getData() { return m_edata; }
	ElevData &getBaseData() { return m_edataBase; }
	virtual Tile *copyTile(int ilat, int ilng) const;
	virtual bool copyTile(int ilat, int ilng, Tile *tile) const;
	void Save();
	void SaveMod();
	void ExportPNG(const std::string &fname, double vmin, double vmax);
	void setWaterMask(const MaskTileBlock *mtileblock);
	double nodeElevation(int ndx, int ndy) const;
	double nodeModElevation(int ndx, int ndy) const;
	void dataChanged(int exmin = -1, int exmax = -1, int eymin = -1, int eymax = -1);
	bool isModified() const { return m_isModified; }
	void RescanLimits();

	/**
	 * \brief Map edits to the tileblock back to one of the individual tiles
	 */
	void syncTile(int ilat, int ilng);

	/**
	* \brief Propagate edits in the boundary overlap zones to the neighbour tiles
	*/
	void MatchNeighbourTiles();

	void ExtractImage(Image &img, TileMode mode, int exmin = -1, int exmax = -1, int eymin = -1, int eymax = -1) const;

protected:
	void ExtractModImage(Image &img, TileMode mode, int exmin = -1, int exmax = -1, int eymin = -1, int eymax = -1) const;

private:
    ElevData m_edata;
	ElevData m_edataBase;
	std::vector<bool> m_waterMask;
	bool m_isModified;
	static const ElevDisplayParam *s_elevDisplayParam;
};


#endif // !TILEBLOCK_H
