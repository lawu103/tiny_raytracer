/*
 * objects.cpp
 *
 *  Created on: Sep. 28, 2020
 *      Author: lawu103
 */

#include "objects.h"

/*
 * Solves a quadratic for the intersections of the ray with the sphere, as shown in
 * https://gabrielgambetta.com/computer-graphics-from-scratch/basic-ray-tracing.html
 */
bool Sphere::ray_intersects(const Vec3f& origin, const Vec3f& dir) const {
	const Vec3f CO = origin - center;
	float a = dir*dir;
	float b = 2*(CO*dir);
	float c = CO*CO - radius*radius;
	float discriminant = b*b - 4*a*c;
	if (discriminant < 0) {
		return false;
	}
	return true;
}


