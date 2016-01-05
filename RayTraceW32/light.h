#ifndef _LIGHT_H
#define _LIGHT_H

#include "stdafx.h"
#include "vect.h"
#include "color.h"

class Light {
private:
	Vect position;
	Vect direction;
	Color color;
	int strength;
	int radius;

public:
	Light();
	Light(Vect position, Color color);
	Vect getPosition();
	void setPosition(Vect position);

	Color getColor();
	void setColor(Color color);
};

#endif
