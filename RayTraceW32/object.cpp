#include "stdafx.h"

#include "object.h"

/*
	Vect getPosition();
	void setPosition(Vect p);

	virtual Material getMaterial();
	virtual void setMaterial(Material _material);

	virtual Vect getNormalAt(Vect point);
	virtual double findIntersection(Ray* ray);
	virtual Color getColorAt(Vect point);
*/


Object::Object() {

}


Vect Object::getPosition() { 
	return position; 
}


void Object::setPosition(Vect _position) { 
	this->position = _position; 
}


Material Object::getMaterial() {
	return material;
}


void Object::setMaterial(Material _material) {
	material = _material;
}


Color Object::getColorAt(Vect point) {
	return Color(point);
}

Vect Object::getNormalAt(Vect point) {
	return Vect(-1,-1,-1);
}

double Object::findIntersection(Ray* ray) {
	return -1;
}

