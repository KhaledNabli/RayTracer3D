#ifndef _RAY_H
#define _RAY_H

#include "stdafx.h"
#include "vect.h"

struct RayIntersection {
	int objectId;
	double distance;
	Vect location;
};

class Ray {
private:
	Vect origin, direction;

public:
	Ray();
	Ray(Vect origin, Vect direction);

	Vect getOrigin();
	void setOrigin(Vect _position);
	Vect getDirection();
	void setDirection(Vect _direction);
	Vect getReflectingDirection(Vect Normal);


};
#endif
