#include "stdafx.h"

#include "plane.h"


Plane::Plane() {

}

Plane::Plane(Vect n, Vect p, Material m) {
	normal = n.normalize();
	position = p;
	material = m;
}


Vect Plane::getNormal() { 
	return normal; 
}


void Plane::setNormal(Vect _normal) { 
	this->normal = _normal.normalize();
}


Vect Plane::getNormalAt(Vect v)	{ 
	return getNormal(); 
}


Color Plane::getColorAt(Vect point) {
	return material.getDiffuseColor();
}


double Plane::findIntersection(Ray* ray)	{
	double distance = -1;
	double a = ray->getDirection().dot(this->normal);

	// if(a == 0) : Ray is parallel. Return -1
	if(a != 0) {
		distance = normal.dot(position.subtract(ray->getOrigin())) / a;
	}

	return distance;
}

