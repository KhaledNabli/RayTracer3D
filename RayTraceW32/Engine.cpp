#include "stdafx.h"
#include "Engine.h"
#include "kernel.h"
#include "bitmap.h"
#include <iostream>

void displayTile(int);


Engine::Engine(EngineSettings * _settings, Scene* _scene)
{
	this->settings = _settings;
	this->scene = _scene;
	this->screenWidth = _settings->tileSize * settings->xTiles; //_scene->getCamera()->getScreenWidth();
	this->screenHeight = _settings->tileSize * settings->yTiles; //_scene->getCamera()->getScreenHeight()
	rendererBuffer = new RGBType[screenWidth*screenHeight];
	threadPool = new std::thread[_settings->threads];
	int countTiles = settings->xTiles * settings->yTiles;
	tileStatusMap = new int[countTiles];
}

Engine::~Engine()
{
	delete rendererBuffer;
	delete threadPool;
}

RGBType * Engine::renderTile(int tileId, onRenderFinish callbackFunction)
{
	//RGBType* tileResult = new RGBType[this->settings->tileSize * this->settings->tileSize];
	Ray* cameraRay = new Ray();
	
	//int tileResultIndex = 0;
	
	// mark this tile as rendering in process:
	tileStatusMap[tileId] = 1;

	int xTile = tileId % settings->xTiles;
	int yTile = (tileId - xTile) / settings->xTiles;

	int xOffset = xTile * this->settings->tileSize;
	int yOffset = yTile * this->settings->tileSize;

	int objCount = this->scene->getObjectList().size();
	int lightCount = this->scene->getLightList().size();
	
	for (int y = 0; y < this->settings->tileSize; y++) {
		for (int x = 0; x < this->settings->tileSize; x++) {
			int realX = x + xOffset;
			int realY = y + yOffset;

			Color pixelColor = Color(0, 0, 0);
			int rayCount = 0;
			// shoot multiple rays per pixel;
			for (rayCount = 0; rayCount < settings->maxRayPerPixel; rayCount++) {
				scene->getCamera()->getRandomRayForPixel(cameraRay, realX, realY);
				Color tracedPixel = traceRay(cameraRay, scene, objCount, lightCount, settings->maxBounces, settings->ambientLighting);
				pixelColor = pixelColor.add(tracedPixel);
				if (rayCount > settings->minRaysPerPixel) {
					// check variance
					double oldBrightness = pixelColor.scale(1.0 / (rayCount +1)).getBrightness();
					double newBrightness = pixelColor.add(tracedPixel).scale(1.0 / (rayCount + 2)).getBrightness();
					double variance = abs(oldBrightness-newBrightness);
					if (variance <= 0.001) {
						break;
					}
				}
				else {
					
				}
			}
			std::cout << "Rendering Pixel: " << x << "," << y << " with " << rayCount << " Rays" << std::endl;

			Color resultColor = pixelColor.scale(1.0 / (rayCount + 1));
			//Color resultColor = Color(1,1,1).scale(1.0 - (rayCount / settings->maxRayPerPixel));
			//tileResult[tileResultIndex] = pixelColor.getRGB();
			int bufferIndex = realY * screenWidth + realX;
			rendererBuffer[bufferIndex] = resultColor.getRGB();

			//tileResultIndex++;
		}
	}

	delete cameraRay;
	tileStatusMap[tileId] = 2;
	finishedTiles++;
	callbackFunction(tileId);
	return nullptr;
}

RGBType * Engine::render()
{
	totalTiles = settings->xTiles * settings->yTiles;
	int tilesPerThread = totalTiles / settings->threads;


	for (int threadId = 0; threadId < settings->threads; threadId++) {
		int* tileList = new int[tilesPerThread];
		int tileListIndex = 0;
		for (int tileId = 0; tileId < totalTiles; tileId++) {
			tileStatusMap[tileId] = 0;
			if (tileId % settings->threads == threadId) {
				tileList[tileListIndex] = tileId;
				tileListIndex++;
			}
		}
		threadPool[threadId] = std::thread(&Engine::renderThread, this, threadId, tileList, tileListIndex);
	}
	

	return nullptr;
}

void Engine::renderThread(int threadId, int* tileList, int listSize)
{
	// split 
	for (int i = 0; i < listSize; i++) {
		this->renderTile(tileList[i], displayTile);
	}

}

void Engine::saveOutput() {
	saveBitmap("c:\\usr\\render.bmp", screenWidth, screenHeight, 72, rendererBuffer);
}
