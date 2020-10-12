/*
 * objects.h
 *
 *  Created on: Sep. 28, 2020
 *      Author: lawu103
 */

#ifndef OBJECTS_H_
#define OBJECTS_H_

#include "geometry.h"

class Sphere {
private:
	Vec3f center;
	float radius;
	Vec3f color;

public:
	Sphere(): center(0, 0, 0), radius(0), color(255, 0, 0) {}
	Sphere(Vec3f C, float R, Vec3f Color): center(C), radius(R), color(Color) {}
	~Sphere() {}

	Vec3f get_color() const { return color; }
	float ray_intersection(const Vec3f& origin, const Vec3f& dir) const;
};

#endif /* OBJECTS_H_ */
