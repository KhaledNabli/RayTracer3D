#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "stdafx.h"
#include "ray.h"
#include "scene.h"


#define MINIMAL_DISTANCE 0.00001


struct KernelSettings {
	int maxBounces = 1;
	double minimalDistance = 0;
	double ambientLighting = 0.05;
	int raysPerPixel = 1;
	int threads = 1;
};



void renderScene(Scene* scene, RGBType* renderBuffer, KernelSettings* settings);

void renderSceneThread(Scene* scene, int width, int wStart, int wEnd, int height, int hStart, int hEnd, int objectCount, int lightCount, RGBType* renderBuffer, KernelSettings* settings);

Color traceRay(Ray* ray, Scene* scene, int objectCount, int lightCount, int maxBounces, double ambientLighting);

RayIntersection testRayIntersection(Ray* ray, Scene* scene, int objectCount, double maximalDistance, double minimalDistance);





#endif /* __KERNEL_H__ */
