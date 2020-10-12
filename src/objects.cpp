/*
 * objects.cpp
 *
 *  Created on: Sep. 28, 2020
 *      Author: lawu103
 */

#include "objects.h"

/*
 * Solves a quadratic for the intersections of the ray with the sphere, as shown in
 * https://gabrielgambetta.com/computer-graphics-from-scratch/basic-ray-tracing.html.
 * The float returned is the t parameter for the vector equation of dir.
 */
float Sphere::ray_intersection(const Vec3f& origin, const Vec3f& dir) const {
	const Vec3f CO = origin - center;
	float a = dir*dir;
	float b = 2*(CO*dir);
	float c = CO*CO - radius*radius;
	float discriminant = b*b - 4*a*c;
	if (discriminant < 0) {
		return std::numeric_limits<float>::max();
	}
	float t1 = (-b + sqrt(discriminant))/(2*a);
	float t2 = (-b - sqrt(discriminant))/(2*a);
	return std::min(t1, t2);
}


