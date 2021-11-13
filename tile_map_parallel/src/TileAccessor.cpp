#include "TileAccessor.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>

TileAccessor::TileAccessor(std::string _tileMapDir, int _tileWidth, int _tileHeight) :
        tileMapDir(_tileMapDir), tileWidth(_tileWidth), tileHeight(_tileHeight) {
    // Initialize Driver
    pDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
}

TileAccessor::TileAccessor(const TileAccessor& theOther) :
    tileMapDir(theOther.tileMapDir),
    tileWidth(theOther.tileWidth), tileHeight(theOther.tileHeight),
    tileLevels(theOther.tileLevels), pDriver(theOther.pDriver) {}

TileAccessor& TileAccessor::operator=(const TileAccessor& theOther)
{
	if (&theOther == this)
        return *this;
    tileMapDir = theOther.tileMapDir;
    tileWidth = theOther.tileWidth;
    tileHeight = theOther.tileHeight;
    tileLevels = theOther.tileLevels;
    pDriver = theOther.pDriver;
    return *this;
}

#include <omp.h>
void TileAccessor::saveTile(const Tile *pTile, int posX, int posY, int level) const {

    //----- Determine if the tile actually exists -----//
    assert(posX >= 0 && posY >= 0 && level >= 0 && level < tileLevels.size());

    if (!pTile || posX >= tileLevels[level].tileNumX || posY >= tileLevels[level].tileNumY)
        return;

    //----- Save tile to disk -----//

    // Locate target directory
    std::filesystem::path tilePath(tileMapDir);
    tilePath.append(std::to_string(level)).append(std::to_string(posX));
    
    // Create directory if not exist
    std::filesystem::create_directories(tilePath);

    // Append file name to path
    tilePath.append(std::to_string(posY) + ".tiff");

    pTile->save(tilePath.string(), pDriver);

#ifndef NO_DEBUG
    std::stringstream ss;
    ss << "Thread#" << omp_get_thread_num() << " Tile ( " << posX << ", " << posY << " ) at level " << level << " Saved to disk" << '\n';
    std::cout << ss.str();
#endif // !NO_DEBUG

}
