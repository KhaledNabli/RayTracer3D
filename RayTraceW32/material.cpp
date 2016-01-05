#include "stdafx.h"

#include "material.h"



Material::Material() {

}

Material::Material(Color _diffuseColor, double _roughness, double _reflectivity) {
	diffuseColor = _diffuseColor;
	roughness = _roughness;
	reflectivity = _reflectivity;
}

Color Material::getDiffuseColor() {
	return diffuseColor;
}

void Material::setDiffuseColor(Color _diffuseColor) {
	diffuseColor = _diffuseColor;
}

double Material::getRoughness() {
	return roughness;
}

void Material::setRoughness(double _roughness) {
	roughness = _roughness;
}

double Material::getReflectivity() {
	return reflectivity;
}

void Material::setReflectivity(double _reflectivity) {
	reflectivity = _reflectivity;
}
