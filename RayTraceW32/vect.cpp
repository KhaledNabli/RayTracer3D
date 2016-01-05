#include "stdafx.h"

#include "vect.h"

using namespace std;

Vect::Vect() {
}

Vect::Vect(double _x, double _y, double _z) {
	this->x = _x;
	this->y = _y;
	this->z = _z;
	this->length = 0;
	this->normalized = false;
}

double Vect::getX() { return x; }
double Vect::getY() { return y; }
double Vect::getZ() { return z; }

string Vect::toString() {
	stringstream result;
	result <<  "( " << x << " , " << y << " , " << z <<  " )";
	return result.str();
}


double Vect::magnitude() {
	if(length == 0) {
		length = sqrt((x*x) + (y*y) + (z*z));
	}

	return length;
}

Vect Vect::normalize() {
	if(!normalized) {
		magnitude();
		this->normalizedX = x / length;
		this->normalizedY = y / length;
		this->normalizedZ = z / length;
	}

	return Vect(normalizedX, normalizedY, normalizedZ);
}

Vect Vect::negative() {
	return Vect(-x, -y, -z);
}

double Vect::dot(Vect v) {
	return x*v.getX() + y*v.getY() + z*v.getZ();
}

Vect Vect::cross(Vect v) {
	double _x = y * v.getZ() - z * v.getY();
	double _y = z * v.getX() - x * v.getZ();
	double _z = x * v.getY() - y * v.getX();
	return Vect(_x, _y, _z);
}

Vect Vect::add(Vect v) {
	double _x = x + v.getX();
	double _y = y + v.getY();
	double _z = z + v.getZ();
	return Vect(_x, _y, _z);
}

Vect Vect::subtract(Vect v) {
	double _x = x - v.getX();
	double _y = y - v.getY();
	double _z = z - v.getZ();
	return Vect(_x, _y, _z);
}

Vect Vect::multiply(double scalar) {
	double _x = x * scalar;
	double _y = y * scalar;
	double _z = z * scalar;
	return Vect(_x, _y, _z);
}
