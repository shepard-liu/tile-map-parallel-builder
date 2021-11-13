

#include <iostream>
#include "src/TileMapScheduler.h"
#include "src/RasterMeta.h"
#include <filesystem>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;
#define DUARATION(__START__, __END__) std::chrono::duration_cast<std::chrono::milliseconds>(__END__ - __START__).count()

int main() {
    const std::string datasetPath = "D:\\RS_Files\\TileTestTiff_2.tiff";
    const std::string tileMapDir = "C:\\Users\\10579\\Desktop\\Cache\\TileTestTiff_2";
    TileMapScheduler tileMapScheduler(datasetPath, tileMapDir, 4, -1);

    auto begin = Clock::now();
    tileMapScheduler.run();
    auto end = Clock::now();

    std::cout << "Elapsed time: " << DUARATION(begin, end);

    //// Generate testing raster image for tile map building
    //GDALAllRegister();

    //auto pInDataset = static_cast<GDALDataset*>(GDALOpen("D:\\RS_Files\\GF6_WFV_8bit.tiff", GA_ReadOnly));

    //// Read the full image into memory
    //RasterMeta meta(pInDataset);

    //std::cout << meta.getPixels() << " " << meta.bands << " " << meta.dataType << '\n';
    //size_t bufSize = meta.getPixels() * 3 * GDALGetDataTypeSizeBytes(meta.dataType);

    //std::cout << bufSize;
    //uchar* buffer = new uchar[bufSize];
    //int bandMap[3]{ 1,2,3 };

    //pInDataset->RasterIO(
    //    GF_Read,
    //    0, 0,
    //    meta.width, meta.height,
    //    buffer,
    //    meta.width, meta.height,
    //    meta.dataType,
    //    3,
    //    bandMap, 0, 0, 0);

    //// Resample and write the dataset into memory 
    //GDALDriver* pDriver = GetGDALDriverManager()->GetDriverByName("GTiff");

    //int outWidth = 60469, outHeight = 56563;

    //auto pOutDataset = pDriver->Create("D:\\RS_Files\\TileTestTiff_2.tiff", outWidth, outHeight, 3, GDT_Byte, nullptr);

    //pOutDataset->RasterIO(GF_Write, 0, 0, outWidth, outHeight, buffer, meta.width, meta.height, meta.dataType, 3, nullptr, 0, 0, 0);

    return 0;
}
