#include "stdafx.h"

#include "ray.h"


Ray::Ray(){
}

Ray::Ray(Vect _origin, Vect _direction) {
	this->origin = _origin;
	this->direction = _direction.normalize();
}

Vect Ray::getOrigin(){
	return this->origin;
}

Vect Ray::getDirection(){
	return this->direction;
}

Vect Ray::getReflectingDirection(Vect normal) {
	Vect negativeDirection = direction.negative();
	double angleToNormal = negativeDirection.dot(normal);
	Vect vectorToNormal = negativeDirection.subtract(normal.multiply(angleToNormal));
	Vect reflectingDirection = negativeDirection.add(vectorToNormal.negative().multiply(2));
	return reflectingDirection;
}

void Ray::setDirection(Vect _direction){
	this->direction = _direction.normalize();
}

void Ray::setOrigin(Vect _position){
	this->origin = _position;
}


