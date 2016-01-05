#include "stdafx.h"

#include "light.h"




Vect position;
Vect direction;
Color color;
int strength;
int radius;

Light::Light() {
}

Light::Light(Vect _position, Color _color) {
	position = _position;
	color = _color;
}

Vect Light::getPosition() {
	return position;
}

void Light::setPosition(Vect _position) {

}

Color Light::getColor() {
	return color;
}

void Light::setColor(Color _color) {

}

