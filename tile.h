#pragma once

#ifndef TILE_H
#define TILE_H

#include "windows_compat.h"
#include <vector>
#include "ddsread.h"
#include "ZTreeMgr.h"
#include "cmap.h"



#define TILE_SURFSTRIDE 512


enum TileMode {
    TILEMODE_NONE,
    TILEMODE_SURFACE,
    TILEMODE_WATERMASK,
    TILEMODE_NIGHTLIGHT,
    TILEMODE_ELEVATION,
    TILEMODE_ELEVMOD
};

enum TileLoadMode {
    TILELOADMODE_USEGLOBALSETTING,
    TILELOADMODE_DIRECTONLY,
    TILELOADMODE_ANCESTORSUBSECTION,
    TILELOADMODE_ANCESTORINTERPOLATE
};


struct ElevData {
    DWORD width;              // elevation grid width (including padding)
    DWORD height;             // elevation grid height (including padding)
    std::vector<double> data; // elevation grid data [m]
    double dmin, dmax;        // min, max tile elevation [m]
    double dres;              // target elevation resolution [m] (must be 2^n with integer n)
    ElevData();
    ElevData(const ElevData &edata);
    ElevData &operator=(const ElevData &edata);
    double nodeValue(int ix, int iy) const;
    void setNodeValue(int ix, int iy, double v);
    ElevData SubTile(const std::pair<DWORD, DWORD> &xrange, const std::pair<DWORD, DWORD> &yrange);
    void RescanLimits();
};

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

inline int nLat(int lvl) { return (lvl < 4 ? 1 : 1 << (lvl - 4)); }
inline int nLng(int lvl) { return (lvl < 4 ? 1 : 1 << (lvl - 3)); }

void ensureLayerDir(const char *rootDir, const char *layer, int lvl, int ilat);

class TileBlock;

class Tile
{
public:
    Tile(int lvl, int ilat, int ilng);
    Tile(const Tile &tile);
    int Level() const { return m_lvl; }
    int iLat() const { return m_ilat; }
    int iLng() const { return m_ilng; }
    int subLevel() const { return m_sublvl; }
    int subiLat() const { return m_subilat; }
    int subiLng() const { return m_subilng; }
    int nLat() const { return ::nLat(m_lvl); }
    int nLng() const { return ::nLng(m_lvl); }

    virtual void set(const Tile *tile);
    void setLevel(int lvl) { m_lvl = lvl; }
    void setiLat(int ilat) { m_ilat = ilat; }
    void setiLng(int ilng) { m_ilng = ilng; }
    void setSubLevel(int lvl) { m_sublvl = lvl; }
    void setSubiLat(int ilat) { m_subilat = ilat; }
    void setSubiLng(int ilng) { m_subilng = ilng; }

    virtual const std::string Layer() const = 0;

    static void setRoot(const std::string &root);
    static const std::string &root() { return s_root; }
    static void setOpenMode(int mode);
    static void setGlobalLoadMode(TileLoadMode mode);

    virtual bool mapToAncestors(int minlvl) const { return false; }

protected:
    void ensureLayerDir();
    void ensureTmpLayerDir();

    int m_lvl;
    int m_ilat;
    int m_ilng;
    int m_sublvl;
    int m_subilat;
    int m_subilng;
    std::pair<DWORD, DWORD> lat_subrange;
    std::pair<DWORD, DWORD> lng_subrange;

    static std::string s_root;
    static int s_openMode;
    static TileLoadMode s_globalLoadMode;
};

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

class TileBlock
{
public:
    TileBlock(int lvl, int ilat0, int ilat1, int ilng0, int ilng1);
    TileBlock(const TileBlock &tileblock);
    virtual ~TileBlock();
    int Level() const { return m_lvl; }
    int minSubLevel() const;
    int iLat0() const { return m_ilat0; }
    int iLat1() const { return m_ilat1; }
    int iLng0() const { return m_ilng0; }
    int iLng1() const { return m_ilng1; }
    int nLat() const { return (m_lvl < 4 ? 1 : 1 << (m_lvl - 4)); }
    int nLng() const { return (m_lvl < 4 ? 1 : 1 << (m_lvl - 3)); }
    int iLng_norm(int ilng) const { int ilngn = ilng; int nlng = nLng(); while (ilngn < 0) ilngn += nlng; while (ilngn >= nlng) ilngn -= nlng; return ilngn; }
    int nLatBlock() const { return m_nblocklat; }
    int nLngBlock() const { return m_nblocklng; }
    int nBlock() const { return m_nblocklat * m_nblocklng; }

    virtual void ExtractImage(Image &img, TileMode mode, int exmin = -1, int exmax = -1, int eymin = -1, int eymax = -1) const = 0;

    virtual bool setTile(int ilat, int ilng, const Tile *tile) { return false; }

    /**
     * \brief Returns a pointer to one of the constituent tiles, using lat/lng index pair
     */
    const Tile *getTile(int ilat, int ilng) const;
    Tile *_getTile(int ilat, int ilng);

    /**
     * \brief Returns a pointer to one of the constituent tiles, unsing linear index
     */
    const Tile *getTile(int idx) const;

    /**
     * \brief Copies one of the constituent tiles and returns a pointer to the copy.
     *
     * The caller owns the copy and is responsible for deleting it.
     */
    virtual Tile *copyTile(int ilat, int ilng) const = 0;

    /**
     * \brief Copy one of the constituent tiles into an existing tile.
     */
    virtual bool copyTile(int ilat, int ilng, Tile *tile) const = 0;

    /**
    * \brief Map edits to the tileblock back to one of the individual tiles
    */
    virtual void syncTile(int ilat, int ilng) {};

    /**
    * \brief Map edits to the tileblock back to the individual tiles
    *
    * This must be called before calling Save() or SaveMod() to make sure the tiles are up to date.
    */
    void syncTiles();

    /**
     * \brief Returns true if at least one of the tiles has been synthesized from an ancestor
     */
    bool hasAncestorData() const;

    /**
    * \brief Recursively map edits to parents down to level minlvl
    *
    * Requires SyncTiles to have been called.
    */
    bool mapToAncestors(int minlvl) const;

protected:

    int m_lvl;
    int m_ilat0, m_ilat1;
    int m_ilng0, m_ilng1;
    int m_nblocklat, m_nblocklng;

    std::vector<Tile*> m_tile; ///< constituent tiles
};


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

#define TILE_FILERES 256
#define TILE_ELEVSTRIDE (TILE_FILERES+3)



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

#endif // TILE_H
