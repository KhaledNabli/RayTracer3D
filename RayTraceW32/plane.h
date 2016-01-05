#ifndef __PLANE_H_
#define __PLANE_H_

#include "stdafx.h"
#include "object.h"
#include "vect.h"
#include "color.h"



class Plane : public Object {
private:
	Vect normal;

public:
	Plane();
	Plane(Vect n, Vect p, Material material);

	Vect getNormal();
	void setNormal(Vect n);

	virtual Vect getNormalAt(Vect point);
	virtual double findIntersection(Ray* ray);
	virtual Color getColorAt(Vect point);
};



#endif /* __PLANE_H_ */
