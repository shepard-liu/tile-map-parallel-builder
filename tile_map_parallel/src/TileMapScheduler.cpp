#include "TileMapScheduler.h"
#include "omp.h"
#include "../libmorton/morton.h"
#include "TileMapBuilder.h"
#include "TileMemoryAccessor.h"
#include <iostream>

TileMapScheduler::TileMapScheduler(const std::string &_datasetPath, const std::string &_tileMapDir, int _maxThreadsNum,
                                   int _topLevelIndex, int _tileWidth, int _tileHeight) {
    GDALAllRegister();

    // Initialize main accessor
    this->pAccessor = new TileDatasetAccessor(_datasetPath, _tileMapDir, _tileWidth, _tileHeight);

    // Determine how many threads to be used
    this->maxThreadsNum = _maxThreadsNum <= 0 ? omp_get_num_procs() : _maxThreadsNum;

    // Determine top levelIndex
    this->topLevelIndex = _topLevelIndex < 0 ? static_cast<int>(pAccessor->getTopLevelIndex()) : _topLevelIndex;

#ifndef NO_DEBUG
    // Print pyramid information
    std::cout << "-----Level Meta Info-----\n";
    for (auto& level : pAccessor->getLevelMetas())
        std::cout << level.toString();
    std::cout << '\n';
#endif // !NO_DEBUG

}

TileMapScheduler::~TileMapScheduler() {
    if (pAccessor)delete pAccessor;
}


void TileMapScheduler::run() {

    //----- Initialization -----//

    //Find the most appropriate levelIndex to build on
    const std::vector<TilePyramidLevel> &levels = pAccessor->getLevelMetas();

    int builderLevelIndex = pAccessor->getTopLevelIndex();

    for (const TilePyramidLevel &level: levels) {
        if (level.tileNumX * level.tileNumY < maxThreadsNum) {
            builderLevelIndex = level.levelIndex - 1;
            break;
        }
    }

    const TilePyramidLevel &builderLevel = levels[builderLevelIndex];

    int builderLevelTilesNum = builderLevel.tileNumX * builderLevel.tileNumY;
    int builderLevelTilesVirtualNum = 
        static_cast<int>(pow(4, pAccessor->getTopLevelIndex() - builderLevelIndex));

    // Initialize builders
    std::vector<TileMapBuilder *> builders(builderLevelTilesVirtualNum, nullptr);
    // Accessor for every builder. GDALDatasets are not thread-safe, so assign
    // different handles for every builder
    std::vector<TileAccessor* > accessors(builderLevelTilesVirtualNum, nullptr);

#ifndef NO_DEBUG
    std::cout << "Running in " << maxThreadsNum << " Threads.\nBuilderLevel = " << builderLevelIndex << '\n';
    std::cout << "BuilderLevelTilesNum = " << builderLevelTilesNum << " BuilderLevelTilesVirtualNum = " << builderLevelTilesVirtualNum << '\n';
    std::cout << "Builder level tileNumX = " << builderLevel.tileNumX << ", tileNumY = " << builderLevel.tileNumY << "\n";
    std::cout << "ToplevelIndex = " << topLevelIndex << "\n";
#endif // !NO_DEBUG


    //----- Run builder on the builder level in parallel -----//

#pragma omp parallel for num_threads(maxThreadsNum) schedule(dynamic)
    for (int i = 0; i < builderLevelTilesVirtualNum; ++i) {

        uint_fast32_t x{}, y{};
        libmorton::morton2D_64_decode(i, x, y);

        // Determine if the tile exists
        if (x < static_cast<uint_fast32_t>(builderLevel.tileNumX) &&
            y < static_cast<uint_fast32_t>(builderLevel.tileNumY)) {
            // Initialize accessor from a copy of main accessor
            accessors[i] = new TileDatasetAccessor(*pAccessor);
            // Initialize builder
            builders[i] = new TileMapBuilder(
                x, y,
                builderLevelIndex,
                topLevelIndex,
                0,
                accessors[i]);
            // Run builder
            builders[i]->run();
        }
    }

    //----- Post processing -----//

    // If the builder merging procedure stops right on the builder level or
    // lower than the builder level, then no more merging needed
    if (topLevelIndex <= builderLevelIndex) {
        // Save all the tiles to disk and release memory
        for (TileMapBuilder *&pBuilder: builders) {

            if (!pBuilder) continue;

            pBuilder->saveAndReleaseAllTilesInStack();
        }
    } else {
        // Enables a single-threaded, in memory merging builder to merge the tiles up to
        // the specified top level. The final builder builds on the theoretical peak level
        // and uses a special accessor to get tiles from memory at builderLevel(which is the
        // final builder's bottom level)

        std::vector<Tile *> tiles;
        tiles.reserve(builderLevelTilesVirtualNum);

        // Gather all the tiles on the builder level
        for (TileMapBuilder *&pBuilder: builders) {
            if (pBuilder) {
                std::stack<TileStackElement> &tileStack = pBuilder->getTileStack();
                tiles.push_back(tileStack.top().pTile);
            } else {
                tiles.push_back(nullptr);
            }
        }

        // Initialize in-memory tile accessor
        auto *pMemAccessor = new TileMemoryAccessor(tiles,
                                                    pAccessor->getLevelMetas(),
                                                    pAccessor->getTileMapDir(),
                                                    pAccessor->getTileWidth(),
                                                    pAccessor->getTileHeight());
        // Create final builder
        auto* pFinalBuilder = new TileMapBuilder(
            0, 0,
            (int)pMemAccessor->getTopLevelIndex(),
            topLevelIndex,
            builderLevelIndex,
            pMemAccessor);

        pFinalBuilder->run();
        pFinalBuilder->saveAndReleaseAllTilesInStack();

        // Release allocated memory
        delete pFinalBuilder, pMemAccessor;
        pFinalBuilder = nullptr;
        pMemAccessor = nullptr;
    }

    //----- Finalize -----//

    for (TileMapBuilder*& pBuilder : builders) {
        if (!pBuilder) continue;
        delete pBuilder;
        pBuilder = nullptr;
    }

    for(TileAccessor*& pAccessor : accessors){
        if (!pAccessor) continue;
		delete pAccessor;
        pAccessor = nullptr;
    }
}
