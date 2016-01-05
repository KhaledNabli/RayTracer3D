#include "stdafx.h"

#include "scene.h"
#include "sphere.h"
#include "plane.h"
#include "polygon.h"

Scene::Scene() {
	sceneCamera = new Camera();
	loadSample1();
}


Scene::~Scene() {
	sceneLights.clear();
	sceneObjects.clear();
	delete sceneCamera;
}

void Scene::loadSample1() {
		// Default Colors
	Color white (1,1,1);
	Color green (0.5, 1, 0.5);
	Color gray (0.1,0.1,0.1);
	Color black (0,0,0);
	Color maroon (0.5,0.25,0.25);
	Color red (1,0,0);
	Color yellow(1,1,0);
	Color blue(0.3,0.3,1);

	backgroundColor = green;
	Material greenSteel(green, 0.2,1);
	Material redSteel(red, 0,1);
	Material yellowSteel(yellow, 1,1);
	Material maroonSteel(maroon, 0.0,3);
	Material blackSky(black,0,1);
	Material greenWall(green, 0.1,0.6);
	Material whiteWall(white, 0.1,0.8);
	Material blueWall(blue, 0.1,1);
	Material redWall(red, 0.1,1);


	// Add Objects to Scene
	Object* sphereObj1 = new Sphere(Vect(100,-6,0), 20, redSteel);
	Object* sphereObj2 = new Sphere(Vect(100,10,25), 20, maroonSteel);
	Object* sphereObj3 = new Sphere(Vect(40,0,22), 5, greenSteel);
	Object* sphereObj4 = new Sphere(Vect(30,5,22), 5, yellowSteel);
	Object* sphereObj5 = new Sphere(Vect(25,-10,22), 5, whiteWall);

	Object*  floorPlane = new Plane(Vect(0,0,1), Vect(0,0,0), whiteWall);
	Object*  roofPlane 	= new Plane(Vect(0,0,-1), Vect(0,0,50), whiteWall);
	Object*  wallLeft 	= new Plane(Vect(0,1,0), Vect(0,-50,0), blueWall);
	Object*  wallRight 	= new Plane(Vect(0,-1,0), Vect(0,50,0), greenWall);
	Object*  wallBack 	= new Plane(Vect(-1,0,0), Vect(200,0,0), whiteWall);

	//Object*  triangle1 = new Polygon(Vect(80,-10,1), Vect(80,20,1), Vect(100,20,20), yellowSteel);
	//Object*  triangle2 = new Polygon(Vect(120,0,2), Vect(90,0,2), Vect(90,-30,2), yellowSteel);
	//Object*  triangle3 = new Polygon(Vect(30,-10,0), Vect(30,20,0), Vect(30,20,15), yellowSteel);
	//sceneObjects.push_back(triangle1);
	//sceneObjects.push_back(triangle2);


	sceneObjects.push_back(sphereObj1);
	sceneObjects.push_back(sphereObj2);
	sceneObjects.push_back(sphereObj3);
	sceneObjects.push_back(sphereObj4);
	sceneObjects.push_back(sphereObj5);

	sceneObjects.push_back(floorPlane);
	sceneObjects.push_back(roofPlane);
	sceneObjects.push_back(wallLeft);
	sceneObjects.push_back(wallRight);
	sceneObjects.push_back(wallBack);


	// Add Lights to Scene
	Light* sceneLight1 = new Light(Vect(+48,30,49.9), white);
	Light* sceneLight2 = new Light(Vect(+100,-30,34.9), yellow);
	sceneLights.push_back(sceneLight1);
	sceneLights.push_back(sceneLight2);

}


void Scene::setCamera(Camera* camera) {
	sceneCamera = camera;
}

Camera* Scene::getCamera() {
	return sceneCamera;
}

std::vector<Object*> Scene::getObjectList() {
	return sceneObjects;
} 

std::vector<Light*> Scene::getLightList() {
	return sceneLights;
}


void Scene::addObject(Object* object) {
	sceneObjects.push_back(object);
}

void Scene::addLight(Light* light) {
	sceneLights.push_back(light);
}

Color Scene::getBackgroundColor() {
	return backgroundColor;
}

void Scene::setBackgroundColor(Color bgColor) {
	backgroundColor = bgColor;
}