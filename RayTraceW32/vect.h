#ifndef _VECT_H
#define _VECT_H

#include "stdafx.h"
#include "math.h"
#include <string>
#include <sstream>


class Vect {
private:
	double x, y, z, length, normalizedX, normalizedY, normalizedZ;
	bool normalized;

public:
	Vect();
	Vect(double x, double y, double z);

	double getX();
	double getY();
	double getZ();


	double magnitude();
	Vect normalize();
	Vect negative();
	double dot(Vect v);
	Vect cross(Vect v);
	Vect add(Vect v);
	Vect subtract(Vect v);
	Vect multiply(double scalar);

	std::string toString();
};
#endif
