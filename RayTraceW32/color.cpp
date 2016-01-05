#include "stdafx.h"

#include "color.h"

using namespace std;

Color::Color() {
	this->red = 0;
	this->green = 0;
	this->blue = 0;
}


Color::Color(Vect vector) {
	this->red = std::abs(vector.getX());
	this->green = std::abs(vector.getY());
	this->blue = std::abs(vector.getZ());
}

Color::Color(RGBType rgb) {
	this->red = rgb.r;
	this->green = rgb.g;
	this->blue = rgb.b;
}

Color::Color(double r, double g, double b) {
	this->red = r;
	this->green = g;
	this->blue = b;
}


Color Color::normalize() {
	/*
	double maxVal = fmax(fmax(red,blue),green);
	if(maxVal > 1) {
		return Color(red/maxVal, green/maxVal, blue/maxVal);
	}
	*/

	return Color(min(1.0, red), min(1.0, green), min(1.0, blue));
}

double Color::getRed() {return red;}
double Color::getGreen() {return green;}
double Color::getBlue() {return blue;}

void Color::setRed(double r) {this->red = r;}
void Color::setGreen(double g) {this->green = g;}
void Color::setBlue(double b) {this->blue = b;}

RGBType Color::getRGB() {
	RGBType rgb = RGBType();
	rgb.r = red;
	rgb.g = green;
	rgb.b = blue;
	return rgb;
}

string Color::toString() {
	stringstream result;
	result <<  "( red:" << red << " , green:" << green << " , blue:" << blue <<  " )";
	return result.str();
}


double Color::getBrightness() {
	return (red + green + blue)/3;
}

Color Color::scale(double scale) {
	return Color(red*scale, green*scale, blue*scale);
}

Color Color::add(Color _color) {
	return Color(red + _color.getRed() , green + _color.getGreen() ,blue + _color.getBlue());
}


Color Color::multiply(Color _color) {
	return Color(red * _color.getRed() , green * _color.getGreen() ,blue * _color.getBlue());
}


Color Color::average(Color _color) {
	return add(_color).scale(0.5);
}

Color Color::getGrayscale() {
	double brightness = this->normalize().getBrightness();
	return Color(brightness,brightness,brightness);
}