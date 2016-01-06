#include "stdafx.h"
#include "Engine.h"
#include "kernel.h"
#include "bitmap.h"
#include <iostream>
#include <algorithm>     // std::shuffle
#include <vector>        // std::array
#include <cstdlib>       // std::rand, std::srand
#include <ctime>         // std::time

void displayTile(int);


Engine::Engine(EngineSettings * _settings, Scene* _scene)
{
	this->settings = _settings;
	this->scene = _scene;
	threadPool = new std::thread[settings->threads];
	rendererBuffer = nullptr;
	tileStatusMap = nullptr;
	raysProduced = 0;

}

void Engine::Reload() {
	if (status == 2) { 
		// Engine is currently rendering.
		// Don´t do anything!

		return;
	}

	status = 0;

	if (tileStatusMap != nullptr) {
		delete[] tileStatusMap;
	}

	if (rendererBuffer != nullptr) {
		delete[] rendererBuffer;
	}

	this->screenWidth = settings->tileSize * settings->xTiles; //_scene->getCamera()->getScreenWidth();
	this->screenHeight = settings->tileSize * settings->yTiles; //_scene->getCamera()->getScreenHeight()
	totalTiles = settings->xTiles * settings->yTiles;
	finishedTiles = 0;
	rendererBuffer = new RGBType[screenWidth*screenHeight];
	tileStatusMap = new int[totalTiles];
	raysProduced = 0;

	scene->getCamera()->setScreenHeight(this->screenHeight);
	scene->getCamera()->setScreenWidth(this->screenWidth);
	scene->getCamera()->setupCamera();

	
}


Engine::~Engine()
{
	delete[] rendererBuffer;
	delete[] threadPool;
}

RGBType * Engine::renderTile(int tileId, onRenderFinish callbackFunction)
{
	Ray* cameraRay = new Ray();
	
	//int tileResultIndex = 0;
	
	// mark this tile as rendering in process:
	tileStatusMap[tileId] = 1;

	int xTile = tileId % settings->xTiles;
	int yTile = (tileId - xTile) / settings->xTiles;

	int xOffset = xTile * this->settings->tileSize;
	int yOffset = yTile * this->settings->tileSize;

	int objCount = (int) this->scene->getObjectList().size();
	int lightCount = (int) this->scene->getLightList().size();
	long raysProducedPerTile = 0;
	 
	for (int y = 0; y < this->settings->tileSize; y++) {
		for (int x = 0; x < this->settings->tileSize; x++) {
			int realX = x + xOffset;
			int realY = y + yOffset;

			// first ray is generated anyway.
			scene->getCamera()->getRandomRayForPixel(cameraRay, realX, realY);
			Color pixelColor = traceRay(cameraRay, scene, objCount, lightCount, settings->maxBounces, settings->ambientLighting);
			int rayCount = 2;

			// shoot 2nd and more rays per pixel;
			for (rayCount = 2; rayCount <= settings->maxRayPerPixel; rayCount++) {
				scene->getCamera()->getRandomRayForPixel(cameraRay, realX, realY);
				Color tracedPixel = traceRay(cameraRay, scene, objCount, lightCount, settings->maxBounces, settings->ambientLighting);
				double oldBrightness = pixelColor.getBrightness();
				pixelColor = pixelColor.add(tracedPixel).scale(0.5);
				double newBrightness = pixelColor.getBrightness();
				double pixelVariance = abs(oldBrightness - newBrightness);

				if (rayCount >= settings->minRaysPerPixel && 
					pixelVariance <= settings->maxVariance) {
						break;
				}
			} // for each ray

			raysProducedPerTile += rayCount;

			Color resultColor = pixelColor;
			//Color resultColor = Color(1,1,1).scale(1.0 - (rayCount / settings->maxRayPerPixel));
			//tileResult[tileResultIndex] = pixelColor.getRGB();
			int bufferIndex = realY * screenWidth + realX;
			rendererBuffer[bufferIndex] = resultColor.getRGB();

		} // for x pixels
	} // for y pixels

	raysProduced += raysProducedPerTile;

	delete cameraRay;
	tileStatusMap[tileId] = 2;
	finishedTiles++;
	callbackFunction(tileId);
	return nullptr;
}

RGBType * Engine::render()
{
	std::thread launcherThread = std::thread(&Engine::startRenderThreads, this);
	launcherThread.detach();
	return nullptr;
}

void Engine::startRenderThreads() {
	status = 2;
	totalTiles = settings->xTiles * settings->yTiles;
	int tilesPerThread = totalTiles / settings->threads;
	finishedTiles = 0;


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

	for (int threadId = 0; threadId < settings->threads; threadId++) {
		threadPool[threadId].join();
		threadPool[threadId].~thread();
	}
	double raysPerPixel = raysProduced / (screenWidth * screenHeight);
	std::wstring concatted_stdstr = L"Rendereing finished producing " + std::to_wstring(raysPerPixel) + L" rays per pixel.";
	LPCWSTR messageText = concatted_stdstr.c_str();

	MessageBox(NULL, messageText, L"Finished", MB_OK | MB_ICONINFORMATION);

	status = 1;
	return;
}

void Engine::renderThread(int threadId, int* tileList, int listSize)
{
	// shuffle
	std::vector<int> tilesToRender;

	// shuffle
	for (int i = 0; i < listSize; i++) {
		tilesToRender.push_back(tileList[i]);
	}

	std::random_shuffle(tilesToRender.begin(), tilesToRender.end());

	for (std::vector<int>::iterator tile = tilesToRender.begin(); tile != tilesToRender.end(); ++tile) {
		renderTile(*tile, displayTile);
	}
}

void Engine::saveOutput() {
	saveBitmap("c:\\usr\\render.bmp", screenWidth, screenHeight, 72, rendererBuffer);
}
