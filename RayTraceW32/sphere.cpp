#include "stdafx.h"

#include "sphere.h"

Sphere::Sphere() {
	
}

Sphere::Sphere(Vect _position, double _size, Material _material) {
	this->position = _position;
	this->size = _size;
	this->material = _material;
}


void Sphere::setSize(double _size) {
	this->size = _size;
}

double Sphere::getSize() {
	return this->size;
}


Vect Sphere::getNormalAt(Vect p) {
	return p.subtract(position).normalize();
}


Color Sphere::getColorAt(Vect point) {
	return material.getDiffuseColor();
}

/*
double Sphere::findIntersection(Ray* ray) { 

	// https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
	double distance;
	Vect o = ray->getOrigin();
	Vect l = ray->getDirection();
	Vect c = position;
	double r = size / 2;

	Vect o_c = o.subtract(c); // o - c
	double discriminant = pow(l.dot(o_c),2) - pow(o_c.magnitude(),2) + pow(r,2);


	if(discriminant > 0) {
		// there is one point of intersection
		double a = (-1 * l.dot(o_c));
		double b = sqrt(discriminant);
	 	double distanceMin = std::min(a+b, a-b);
	 	double distanceMax = std::max(a+b, a-b);
	 	if(distanceMin > 0)
	 		distance = distanceMin;
	 	else if (distanceMin < 0 && distanceMax > 0) 
	 		distance = distanceMax;
	 	else
	 		distance = -1;
	} else {
		// there is no intersection
		distance = -1;
	}

	return distance; 
}; 

*/


double Sphere::findIntersection(Ray* ray) { 

	// https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
	double distance;
	double r = size / 2;

	Vect o_c = ray->getOrigin().subtract(position);
	double discriminant = pow(ray->getDirection().dot(o_c),2) - pow(o_c.magnitude(),2) + pow(r,2);


	if(discriminant > 0) {
		// there is one point of intersection
		double a = ray->getDirection().dot(o_c) * -1;
		double b = sqrt(discriminant);
		double amb = a-b;
		double apb = a+b;
	 	if(amb > 0)
	 		distance = amb;
	 	else if (amb < 0 && apb > 0) 
	 		distance = apb;
	 	else
	 		distance = -1;
	} else {
		// there is no intersection
		distance = -1;
	}

	return distance; 
}; 



