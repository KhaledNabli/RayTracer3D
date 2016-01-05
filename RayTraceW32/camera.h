#ifndef _CAMERA_H
#define _CAMERA_H

#include "stdafx.h"
#include "vect.h"
#include "ray.h"

class Camera {
private:
	Vect position, direction, rightAxes, downAxes, pixelDirection;
	int screenWidth, screenHeight;
	double focalLength; // in mm
	double sensorSize, sensorHeight; // in mm
	double depthOfField, dofSize;
	double focusDistance; // in meter

public:

	Camera();

	Vect getPosition();
	Vect getDirection();
	Vect getCameraRight();
	Vect getCameraDown();



	void setDirection(Vect direction);
	void setPosition(Vect position);
	void setResolution(int width, int height);
	void setScreenHeight(int height);
	void setScreenWidth(int width);
	double getSensorSize();
	void setSensorSize(double size);
	double getFocalLength();
	void setFocalLength(double focalLength);
	double getDepthOfField();
	void setDepthOfField(double depthOfField);
	double getFocusDistance();
	void setFocusDistance(double focusDistance);


	int getScreenHeight();
	int getScreenWidth();
	void lookAt(Vect point);
	void focusAt(Vect point);

	Vect getPixelDirection(double x, double y);

	void getRandomRayForPixel(Ray* ray, double x, double y);
	void setupCamera();
};

#endif
