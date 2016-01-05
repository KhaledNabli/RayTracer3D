#include "stdafx.h"

#include "camera.h"
#include <iostream>


Camera::Camera() {
}


Vect Camera::getPosition() {
	return this->position;
}

Vect Camera::getDirection() {
	return this->direction;
}


Vect Camera::getCameraRight() {
	return this->rightAxes;
}


Vect Camera::getCameraDown() {
	return this->downAxes;
}


void Camera::setDirection(Vect _direction) {
	this->direction = _direction.normalize();
}

void Camera::setPosition(Vect _position) {
	this->position = _position;
}

void Camera::setFocalLength(double _focalLength) {
	focalLength = _focalLength;
}


double Camera::getFocalLength() {
	return focalLength;
}

void Camera::setSensorSize(double _size) {
	sensorSize = _size;
}


double Camera::getSensorSize() {
	return sensorSize;
}

void Camera::setDepthOfField(double dof) {
	depthOfField = dof;
}

double Camera::getDepthOfField() {
	return depthOfField;
}

void Camera::setFocusDistance(double _focusDistance) {
	focusDistance = _focusDistance;
}

double Camera::getFocusDistance() {
	return focusDistance;
}



void Camera::lookAt(Vect point) {
	this->direction = point.subtract(position).normalize();
}

void Camera::focusAt(Vect point) {
	this->focusDistance = point.subtract(position).magnitude();
}


void Camera::setupCamera() {
	Vect Z(0,0 ,1);
	pixelDirection = direction.multiply(focalLength / 1000);
	sensorHeight = sensorSize / screenWidth * screenHeight;
	dofSize = sensorSize / depthOfField;
	rightAxes = Z.cross(direction).normalize().multiply(sensorSize / 1000);
	downAxes = rightAxes.cross(direction).normalize().multiply(sensorHeight / 1000);
}


Vect Camera::getPixelDirection(double x, double y) {
	double rayOffsetX, rayOffsetY;
	x += ((double) rand() / (RAND_MAX));
	y += ((double) rand() / (RAND_MAX));

	rayOffsetX = (x / screenWidth) - 0.5;
	rayOffsetY = (y / screenHeight) - 0.5;
	return pixelDirection.add(rightAxes.multiply(rayOffsetX)).add(downAxes.multiply(rayOffsetY));
}


void Camera::getRandomRayForPixel(Ray* ray, double x, double y) {
	double randomeX = (((double) rand() / (RAND_MAX)) - 0.5) * dofSize;
	double randomeY = (((double) rand() / (RAND_MAX)) - 0.5) * dofSize;

	Vect focusPoint = getPixelDirection(x,y).normalize().multiply(focusDistance).add(position);
	Vect rayOrigin = position.add(rightAxes.multiply(randomeX)).add(downAxes.multiply(randomeY));
	Vect rayDirection = focusPoint.subtract(rayOrigin).normalize();

	ray->setOrigin(rayOrigin);
	ray->setDirection(rayDirection);
}



void Camera::setResolution(int _width, int _height) {
	this->screenWidth = _width;
	this->screenHeight = _height;
}


void Camera::setScreenHeight(int _height){
	this->screenHeight = _height;
}

void Camera::setScreenWidth(int _width) {
	this->screenWidth = _width;
}


int Camera::getScreenHeight(){
	return this->screenHeight;
}

int Camera::getScreenWidth() {
	return this->screenWidth;
}