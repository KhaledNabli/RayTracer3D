#include "stdafx.h"

#include <iostream>
#include <chrono>

#include "bitmap.h"
#include "camera.h"
#include "scene.h"
#include "kernel.h"


using namespace std;

// read from argv
#define OUTPUT_WIDTH 160*12
#define OUTPUT_HEIGHT 90*12
#define RENDER_FILE_NAME "output\\RenderOut.bmp"


#define START_PROFILE start_time = std::chrono::high_resolution_clock::now();
#define COUT_PROFILE cout << "\ttook  " << (((int)std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count()/1000)) << " ms to run." << endl;


int main_old(int argc, char *argv[]) {
	auto start_time_begin = std::chrono::high_resolution_clock::now();
	auto start_time = std::chrono::high_resolution_clock::now();

	cout << endl << endl << "starting..." << endl;

	START_PROFILE
		cout << "read scene...";
	Scene* scene = new Scene();


	scene->getCamera()->setScreenHeight(OUTPUT_HEIGHT);
	scene->getCamera()->setScreenWidth(OUTPUT_WIDTH);
	scene->getCamera()->setFocalLength(24);
	scene->getCamera()->setSensorSize(35);

	scene->getCamera()->setDepthOfField(22);
	//scene->getCamera()->setFocusDistance(25);
	scene->getCamera()->focusAt(Vect(40, 0, 22));
	scene->getCamera()->setupCamera();
	RGBType *renderBuffer = new RGBType[OUTPUT_WIDTH * OUTPUT_HEIGHT];
	COUT_PROFILE


		START_PROFILE
		cout << "rendering scene...";
	KernelSettings* settings = new KernelSettings();
	settings->raysPerPixel = 4;
	settings->maxBounces = 0;
	settings->threads = 1;
	renderScene(scene, renderBuffer, settings);
	COUT_PROFILE


		START_PROFILE
		cout << "saving bitmap...";
	saveBitmap(RENDER_FILE_NAME, OUTPUT_WIDTH, OUTPUT_HEIGHT, 72, renderBuffer);
	COUT_PROFILE

		cout << "process finished...";
	auto end_time = std::chrono::high_resolution_clock::now();
	auto time = end_time - start_time_begin;
	std::cout << "\tand took " << (((int)std::chrono::duration_cast<std::chrono::microseconds>(time).count() / 1000)) << " ms to run.\n";
	delete[] renderBuffer;
  	delete scene;
  	delete settings;


  	cin.ignore();
	return 0;
}
