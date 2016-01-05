#include "stdafx.h"

#include <cstdlib>
#include <limits>
#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>

#include "kernel.h"


using namespace std;



RGBType* renderTile(Scene* scene, KernelSettings* settings, int xTile, int yTile, int tileSize, int objCount, int lightCount) {
	RGBType* tileResult = new RGBType[tileSize * tileSize];
	Ray* cameraRay = new Ray();
	Color pixelColor = Color(0, 0, 0);
	int tileResultIndex = 0;
	int xOffset = xTile * tileSize;
	int yOffset = yTile * tileSize;

	for (int y = 0; y < tileSize; y++) {
		for (int x = 0; x < tileSize; x++) {
			tileResultIndex++;
			// shoot multiple rays per pixel;
			for (int r = 0; r < settings->raysPerPixel; r++) {
				scene->getCamera()->getRandomRayForPixel(cameraRay, x + xOffset, y + yOffset);
				// shade pixel
				pixelColor = pixelColor.add(traceRay(cameraRay, scene, objCount, lightCount, settings->maxBounces, settings->ambientLighting).scale(1.0 / settings->raysPerPixel));
			}

			tileResult[tileResultIndex] = pixelColor.getRGB();
		}
	}

	delete cameraRay;
	return tileResult;
}


void renderSceneThread(Scene* scene, int width, int wStart, int wEnd, int height, int hStart, int hEnd, int objectCount, int lightCount, RGBType* renderBuffer, KernelSettings* settings) {
	Ray* cameraRay = new Ray();

	for (int y = hStart; y < hEnd; y++) {
		for (int x = wStart; x < wEnd; x++) {

			int i = y * width + x;
			Color pixelColor = Color(0,0,0);

			// shoot multiple rays per pixel;
			for(int r = 0; r < settings->raysPerPixel; r++) {
				
				scene->getCamera()->getRandomRayForPixel(cameraRay, x, y);

				// shade pixel
				pixelColor = pixelColor.add(traceRay(cameraRay, scene, objectCount, lightCount, settings->maxBounces, settings->ambientLighting).scale(1.0 / settings->raysPerPixel));
			}

			renderBuffer[i] = pixelColor.getRGB();
		}
	}

	delete cameraRay;
}

void renderScene(Scene* scene, RGBType* renderBuffer, KernelSettings* settings) {
	int height = scene->getCamera()->getScreenHeight();
	int width = scene->getCamera()->getScreenWidth();
	int objectCount = scene->getObjectList().size();
	int lightCount = scene->getLightList().size();

	int chunkSize = width / settings->threads;
	std::thread* threadPool = new std::thread[settings->threads];
	cout << endl;
	// start threads
	for(int t = 0; t < settings->threads; t++) {
		int wStart = t * chunkSize;
		int wEnd = ((t+1) * chunkSize);
		int hStart = 0;
		int hEnd = height;

		cout << "\tthread " << t << " started to render from x:" << wStart << "..." << wEnd-1 << " y:" << hStart << "..." << hEnd-1 << endl; 
		threadPool[t] = thread(renderSceneThread, scene, width, wStart, wEnd, height, hStart, hEnd, objectCount, lightCount, renderBuffer, settings);
	}
	/*
	// wait for threads to finish
	for(int t = 0; t < settings->threads; t++) {
		threadPool[t].join();
		cout << "\tthread " << t << " finished rendering." << endl;
	}
	
	delete[] threadPool;
	*/
}



Color traceRay(Ray* ray, Scene* scene, int objectCount, int lightCount, int maxBounces, double ambientLighting) {
	Color shaderResult (0,0,0);
	Ray* shadowRay = new Ray();
	Ray* reflectionRay = new Ray();
	RayIntersection primaryObjectHit = testRayIntersection(ray, scene, objectCount, 0, MINIMAL_DISTANCE);

	if(primaryObjectHit.objectId >= 0) {
		Object* sceneObject = scene->getObjectList().at(primaryObjectHit.objectId);
		// calulate brightness and specularity
		Color specular (0,0,0);
		Color diffuse (0,0,0);
		Color reflection (0,0,0);
		
		Vect intersectionPoint = primaryObjectHit.location;
		Color ambient = sceneObject->getColorAt(intersectionPoint).scale(ambientLighting);
		shadowRay->setOrigin(intersectionPoint);

		Vect normalAtIntersection = sceneObject->getNormalAt(intersectionPoint);
		Vect reflectionDirection = ray->getReflectingDirection(normalAtIntersection);
		

		double lightImportance = 1;
		if(lightCount > 1) {
			lightImportance = 1.0 / lightCount;
		}

		for(int lightIndex = 0; lightIndex < lightCount; lightIndex++) {
			Light *lightSource = scene->getLightList().at(lightIndex);
			Vect pointToLightVect = lightSource->getPosition().subtract(intersectionPoint);
			Vect lightDirection = pointToLightVect.normalize();

			// Test if light is visible or hidden to render shadows
			double diffuseFactor = normalAtIntersection.dot(lightDirection);
			double specularFactor = reflectionDirection.dot(lightDirection);
			bool shadowed = false;

			if(diffuseFactor > 0) // object is facing the light source - add diffuse
			{
				// check if it is shadowed;
				shadowRay->setDirection(lightDirection);
				double lightDistance = pointToLightVect.magnitude();
				shadowed = (testRayIntersection(shadowRay, scene, objectCount, lightDistance, MINIMAL_DISTANCE).objectId > -1);
			}

			if(!shadowed && diffuseFactor > 0) {
				diffuse = diffuse.add(sceneObject->getColorAt(intersectionPoint).scale(diffuseFactor));
			}

			if(!shadowed && specularFactor > 0 && sceneObject->getMaterial().getRoughness() > 0) {
				specular = specular.add(lightSource->getColor().scale(pow(specularFactor, 10) * sceneObject->getMaterial().getRoughness()));
			}
			
		}		
		

		if(maxBounces > 0 && sceneObject->getMaterial().getReflectivity() > 0) {
			reflectionRay->setOrigin(intersectionPoint);
			reflectionRay->setDirection(reflectionDirection);
			reflection = traceRay(reflectionRay, scene, objectCount, lightCount, --maxBounces, ambientLighting).scale(sceneObject->getMaterial().getReflectivity() * 0.25);
		}


		shaderResult = ambient.add(diffuse).add(specular).add(reflection).normalize();
	} else {
		shaderResult = scene->getBackgroundColor();
	}

	delete shadowRay;
	delete reflectionRay;

	return shaderResult;
}


RayIntersection testRayIntersection(Ray* ray, Scene* scene, int objectCount, double maximalDistance, double minimalDistance) {
	// Init 
	RayIntersection IntersectionResult;
	IntersectionResult.distance = -1;
	IntersectionResult.objectId = -1;

	if(maximalDistance <= 0) {
		maximalDistance = (std::numeric_limits<double>::max)();
	}

	// find the closes object in camera ray direction. 
	for(int objIndex = 0; objIndex < objectCount; objIndex++) {
		double objDistance = scene->getObjectList().at(objIndex)->findIntersection(ray);
		if(objDistance > minimalDistance && objDistance < maximalDistance) {
			// intersection detected. check if this intersection is not behind another one
			if(((IntersectionResult.distance == -1) || (objDistance < IntersectionResult.distance))) {
				IntersectionResult.objectId = objIndex;
				IntersectionResult.distance = objDistance;
			}
		}
	}

	if(IntersectionResult.distance > 0) {
		IntersectionResult.location = ray->getOrigin().add(ray->getDirection().multiply(IntersectionResult.distance));
	}

	return IntersectionResult;
}
