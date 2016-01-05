#ifndef __SPHERE_H_
#define __SPHERE_H_

#include "stdafx.h"
#include "object.h"
#include "vect.h"
#include "material.h"


class Sphere : public Object {
private:
	double size;

public:
	Sphere();
	Sphere(Vect position, double size, Material material);

	void setSize(double size);
	double getSize();
	
	virtual Vect getNormalAt(Vect point);
	virtual double findIntersection(Ray* ray);
	virtual Color getColorAt(Vect point);
};



#endif /* __SPHERE_H_ */
