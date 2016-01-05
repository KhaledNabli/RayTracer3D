#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#include "stdafx.h"
#include "color.h"

class Material {
private:
	Color diffuseColor;
	double roughness;
	double reflectivity;

public:
	Material();
	Material(Color diffuseColor, double roughness, double reflectivity);

	Color getDiffuseColor();
	void setDiffuseColor(Color _dc);

	double getRoughness();
	void setRoughness(double);

	double getReflectivity();
	void setReflectivity(double);

};

#endif /* __MATERIAL_H_ */