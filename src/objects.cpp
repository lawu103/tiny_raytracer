/*
 * objects.cpp
 *
 *  Created on: Sep. 28, 2020
 *      Author: lawu103
 */

#include "objects.h"

bool Sphere::ray_intersects(const Vec3f& origin, const Vec3f& dir) const {
	const Vec3f centerToOrigin = origin - center;
	float a = dir*dir;
	float b = 2*(centerToOrigin*dir);
	float c = centerToOrigin*centerToOrigin - radius*radius;
	float discriminant = b*b - 4*a*c;
	if (discriminant < 0) {
		return false;
	}
	return true;
}


