#ifndef _COLOR_H
#define _COLOR_H

#include "stdafx.h"
#include "vect.h"
#include <string>
#include <sstream>
#include <cmath>

struct RGBType {
	double r,g,b;
};


class Color {
private:
	double red, green, blue;

public:
	Color();
	Color(RGBType rgb);
	Color(Vect vector);
	Color(double r, double g, double b);
	Color normalize();

	double getRed();
	double getGreen();
	double getBlue();

	void setRed(double r);
	void setGreen(double g);
	void setBlue(double b);

	double getBrightness();
	Color scale(double scale);
	Color add(Color _color);
	Color multiply(Color _color);
	Color average(Color _color);
	Color getGrayscale();

	RGBType getRGB();
	std::string toString();

};





#endif
