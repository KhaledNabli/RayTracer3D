#ifndef __polygon_h_
#define __polygon_h_


#include "object.h"
#include "plane.h"



class Polygon : public Object {
private:
	Vect A, B, C, AB, AC; 
	double uu, vv, uv, determinator;
	Plane plane;

public:
	Polygon(Vect _A, Vect _B, Vect _C, Material _material);

	virtual Vect getNormalAt(Vect point);
	virtual double findIntersection(Ray* ray);
	virtual Color getColorAt(Vect point);
};






#endif
