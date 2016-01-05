#include "stdafx.h"
#include "color.h"

#define INCH_METER_FACTOR 39.3701
#pragma pack(1)

struct BitmapFileHeader // BitmapFileHeader
{
      char         bmtype[2];     // 2 bytes - 'B' 'M'
      unsigned int iFileSize;     // 4 bytes
      short int    reserved1;     // 2 bytes
      short int    reserved2;     // 2 bytes
      unsigned int iOffsetBits;   // 4 bytes
 
};// End of BitmapFileHeader structure - size of 14 bytes

struct BitmapInfoHeader // BitmapInfoHeader
{
      unsigned int iSizeHeader;    // 4 bytes - 40
      unsigned int iWidth;         // 4 bytes
      unsigned int iHeight;        // 4 bytes
      short int    iPlanes;        // 2 bytes
      short int    iBitCount;      // 2 bytes
      unsigned int Compression;    // 4 bytes
      unsigned int iSizeImage;     // 4 bytes
      unsigned int iXPelsPerMeter; // 4 bytes
      unsigned int iYPelsPerMeter; // 4 bytes
      unsigned int iClrUsed;       // 4 bytes
      unsigned int iClrImportant;  // 4 bytes
 
};// End of BitmapInfoHeader structure - size 40 bytes
// disable 1 byte boundary packing
#pragma pack()

int saveBitmap (const char *filename, int width, int height, int dpi, const RGBType *bitmap);
