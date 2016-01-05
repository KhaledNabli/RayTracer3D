#include "stdafx.h"
#include "polygon.h"

Polygon::Polygon(Vect _A, Vect _B, Vect _C, Material _material) {
	A = _A;
	B = _B;
	C = _C;
	AB = B.subtract(A);
	AC = C.subtract(A);
	material = _material;
	position = A;

	Vect normal = AB.cross(AC).normalize();
	plane = Plane(normal, position, _material);

	// pre calculated for findIntersection
	uu = AB.dot(AB);
	uv = AB.dot(AC);
	vv = AC.dot(AC);
	determinator = uv * uv - uu * vv;
}


Vect Polygon::getNormalAt(Vect point) {
	return plane.getNormalAt(point);
}


double Polygon::findIntersection(Ray* ray) {
	// algorithm based: http://geomalgorithms.com/a06-_intersect-2.html

	double planeDistance = plane.findIntersection(ray);
	if(planeDistance > 0) {
		Vect P = ray->getOrigin().add(ray->getDirection().multiply(planeDistance));
		Vect AP = P.subtract(A);
		double wu, wv, s, t;
		//double uu, uv, vv, wu, wv, determinator, s, t;
		// u: AB
		// v: AC
		// w: AP
		// uu = AB.dot(AB);
		// uv = AB.dot(AC);
		// vv = AC.dot(AC);
		wu = AP.dot(AB);
		wv = AP.dot(AC);
		//determinator = uv * uv - uu * vv;
		s = (uv * wv - vv * wu) / determinator;
		t = (uv * wu - uu * wv) / determinator;

		if(s < 0 || s > 1)
			return -1;
		
		if(t < 0 || s+t > 1)
			return -1;
	}
	return planeDistance;
}

Color Polygon::getColorAt(Vect point) {
	return material.getDiffuseColor();
}
