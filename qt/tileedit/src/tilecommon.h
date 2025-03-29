#pragma once

#include "imagetools.h"
#include <vector>
#include <string>
#ifndef TILECOMMON_H
#define TILECOMMON_H

inline int nLat(int lvl) { return (lvl < 4 ? 1 : 1 << (lvl - 4)); }
inline int nLng(int lvl) { return (lvl < 4 ? 1 : 1 << (lvl - 3)); }

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


#endif // TILECOMMON_H
