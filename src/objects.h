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
	float specular;

public:
	Sphere(): center(0, 0, 0), radius(0), color(255, 0, 0), specular(0) {}
	Sphere(Vec3f C, float R, Vec3f Color, float S): center(C), radius(R), color(Color), specular(S) {}
	~Sphere() {}

	Vec3f get_center() const {return center; }
	Vec3f get_color() const { return color; }
	float get_specular() const { return specular; }
	float ray_intersection(const Vec3f& origin, const Vec3f& dir) const;
};

class Light {
protected:
	float intensity;
public:
	Light(): intensity(0) {}
	Light(float i): intensity(i) {}
	~Light() {}

	float get_intensity() const { return intensity; }
};

class DirectedLight : public Light {
private:
	bool isPoint;	// true if point light, false if directional light
	Vec3f v;		// for point lights, the position. For directional lights, the direction.
public:
	DirectedLight(): Light(0), isPoint(true), v(0, 0, 0) {}
	DirectedLight(float I, bool T, Vec3f V): Light(I), isPoint(T), v(V) {}
	~DirectedLight() {}

	bool is_point() const { return isPoint; }
	Vec3f get_v() const { return v; }
};

#endif /* OBJECTS_H_ */
