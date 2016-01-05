#ifndef __SCENE_H_
#define __SCENE_H_

#include "stdafx.h"
#include <vector>
#include <string>
#include "object.h"
#include "light.h"
#include "camera.h"

class Scene {
private:
	Camera* sceneCamera;
	Color backgroundColor;
	std::vector<Object*> sceneObjects;
	std::vector<Light*> sceneLights;


public:
	Scene();
	Scene(std::string filename);
	~Scene();

	std::vector<Object*> getObjectList();
	std::vector<Light*> getLightList();
	Camera* getCamera();

	Color getBackgroundColor();
	void setBackgroundColor(Color bgColor);
	void addObject(Object* object);
	void addLight(Light* light);
	void setCamera(Camera* camera);

	void loadSample1();
};


#endif
