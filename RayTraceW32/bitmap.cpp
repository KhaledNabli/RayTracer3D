#include "stdafx.h"

#include <fstream>
#include <cmath>

#include "bitmap.h"

int saveBitmap (const char *filename, int width, int height, int dpi, const RGBType *bitmap) {
	FILE *file;

	struct BitmapFileHeader fileHeader;
	struct BitmapInfoHeader infoHeader;

	int rowSize = (24 * width +31)/32*4;
	int rowPadding = rowSize - (width*3);
	int pixelArraySize = rowSize * height;
	unsigned char *rowPixelBuffer = new unsigned char[rowSize];
	int resolution = width * height;
	int ppm = static_cast<int>(dpi * INCH_METER_FACTOR);

	int fileSize = 54 + pixelArraySize;

	fileHeader.bmtype[0] = 'B';
	fileHeader.bmtype[1] = 'M';
	fileHeader.iFileSize = fileSize;
	fileHeader.iOffsetBits = 54;
	infoHeader.iSizeHeader = 40;
	infoHeader.iWidth = width;
	infoHeader.iHeight = height;
	infoHeader.iSizeImage = pixelArraySize;
	infoHeader.iXPelsPerMeter = ppm;
	infoHeader.iYPelsPerMeter = ppm;
	infoHeader.iBitCount = 24;
	infoHeader.iPlanes = 1;
	infoHeader.Compression = 0;
	infoHeader.iClrUsed = 0;
	infoHeader.iClrImportant = 0;


	file = fopen(filename, "wb");
	fwrite(&fileHeader, 1, 14, file);
	fwrite(&infoHeader, 1, 40, file);
	

	for (int y = 0; y < height; y++) {
		//cout << "render line " << y+1 << endl;
		for(int x = 0; x < width; x++) {



			int bmIndex = y * width + x;
			int rowIndex = x * 3;
			RGBType pixelColor = bitmap[bmIndex];
			// blue, green, red
			rowPixelBuffer[rowIndex+0] = (int) floor(pixelColor.b * 255);
			rowPixelBuffer[rowIndex+1] = (int) floor(pixelColor.g * 255);
			rowPixelBuffer[rowIndex+2] = (int) floor(pixelColor.r * 255);
		}

		fwrite(rowPixelBuffer, 1, rowSize, file);
	}
	fclose(file);
	delete(rowPixelBuffer);
	return fileSize;
}
