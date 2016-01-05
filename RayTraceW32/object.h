#ifndef __OBJECT_H_
#define __OBJECT_H_

#include "stdafx.h"
#include "material.h"
#include "color.h"
#include "ray.h"




class Object {
protected:
	Vect position;
	Material material;

public:
	Object();

	Vect getPosition();
	void setPosition(Vect p);

	Material getMaterial();
	void setMaterial(Material _material);

	virtual Vect getNormalAt(Vect point);
	virtual double findIntersection(Ray* ray);
	virtual Color getColorAt(Vect point);
};




#endif /* __OBJECT_H_ */
