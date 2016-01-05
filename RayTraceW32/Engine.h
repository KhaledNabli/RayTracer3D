#pragma once

#include <thread>
#include "color.h"
#include "scene.h"


typedef void(*onRenderFinish)(int);

struct EngineSettings {
	int maxBounces = 1;
	double minimalDistance = 0.01;
	double ambientLighting = 0.05;
	int minRaysPerPixel = 1;
	int maxRayPerPixel = 10;
	int threads = 4;
	int tileSize = 16;
	int xTiles = 10;
	int yTiles = 10;
};

class Engine
{
private:
	std::thread* threadPool;
	EngineSettings* settings;
	Scene* scene;
	int screenHeight;
	int screenWidth;
	int totalTiles;
	int finishedTiles;

public:
	int* tileStatusMap;
	RGBType* rendererBuffer;

	Engine(EngineSettings* settings, Scene* scene);
	~Engine();

	RGBType* renderTile(int tileId, onRenderFinish callback);

	RGBType* render();

	void renderThread(int threadId, int* tileList, int listSize);
	void saveOutput();
};

