//============================================================================
// Name        : tiny_raytracer.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "geometry.h"
#include "objects.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

/*
 * Converts a canvas xy position to a viewport xyz position
 */
Vec3f canvas_to_port(float canvasX, float canvasY, int canvasW, int canvasH, int portW, int portH, int portD) {
	return Vec3f(canvasX*portW/canvasW, canvasY*portH/canvasH, portD);
}

/*
 * Calculates total lighting on a point
 */
float compute_lighting(const float tmin, const Vec3f& camera, const Vec3f& dir, const Sphere* closestBall,
		const Light& ambientLight, const vector<DirectedLight>& directedLights) {
	Vec3f P = camera + tmin*dir;
	Vec3f N = P - closestBall->get_center();
	N = 1/N.length() * N;

	float i = ambientLight.get_intensity();
	for (const DirectedLight& directedLight: directedLights) {
		Vec3f L;
		if (directedLight.is_point()) {
			L = directedLight.get_v() - P;
		} else {
			L = -1*directedLight.get_v();
		}

		// Diffuse
		float NdotL = N*L;
		if (NdotL > 0) {
			i += directedLight.get_intensity() * NdotL/(N.length()*L.length());
		}

		// Specular
		float s = closestBall->get_specular();
		if (s > 0) {
			Vec3f R = 2*(N*L)*N - L;
			Vec3f V = -1*dir;
			float RdotV = R*V;
			if (RdotV > 0) {
				i += directedLight.get_intensity() * pow(RdotV/(R.length()*V.length()), s);
			}
		}
	}
	return i;
}

/*
 * Returns the color a ray sees
 */
Vec3f trace_ray(const Vec3f& camera, const Vec3f& dir, const vector<Sphere>& balls, const Light& ambientLight,
				const vector<DirectedLight>& directedLights) {
	float tmin = numeric_limits<float>::max();
	Vec3f color = Vec3f(255, 255, 255);
	const Sphere* closestBall = NULL;
	for (const Sphere& ball: balls) {
		float t = ball.ray_intersection(camera, dir);
		if (t < tmin && t >= 0) {
			tmin = t;
			closestBall = &ball;
		}
	}
	if (closestBall) {
		float lighting = compute_lighting(tmin, camera, dir, closestBall, ambientLight, directedLights);
		color = lighting*closestBall->get_color();
	}
	return Vec3f(min(255.f, color[0]), min(255.f, color[1]), min(255.f, color[2]));
}

int main() {
	vector<Sphere> balls = {Sphere(Vec3f(0, -1, 3), 1, Vec3f(255, 0, 0), 500),
							Sphere(Vec3f(-2, 0, 4), 1, Vec3f(0, 255, 0), 10),
							Sphere(Vec3f(2, 0, 4), 1, Vec3f(0, 0, 255), 500),
							Sphere(Vec3f(0, -5001, 0), 5000, Vec3f(255, 255, 0), 1000)};

	// The sum of the intensities of our lights should be 1 if we don't want overexposure.
	Light ambientLight(0.2);
	vector<DirectedLight> directedLights = {DirectedLight(0.6, true, Vec3f(2, 1, 0)),
											DirectedLight(0.2, false, Vec3f(-1, -4, -4))};

	// Camera position in world coordinates. x and y are planar with the screen, z is into the screen.
	const Vec3f camera(0, 0, 0);

	// Canvas dimensions in pixels.
	const int canvasW = 1024;
	const int canvasH = 1024;

	// Viewport dimensions in world units. This should be proportionally the same as the canvas.
	// Note that the viewport is always fixed centered to the camera, so portD is a depth that's
	// relative to the camera.
	const int portW = 1;
	const int portH = 1;
	const int portD = 1;

	vector<Vec3f> framebuffer(canvasW*canvasH);

	// Fill in color values
	for (int j = 0; j < canvasH; ++j) {
		for (int i = 0; i < canvasW; ++i) {
			float canvasX = i - canvasW/2.f;	// treat center of canvas as origin
			float canvasY = canvasH/2.f - j;
			Vec3f dir = canvas_to_port(canvasX, canvasY, canvasW, canvasH, portW, portH, portD);
			Vec3f color = trace_ray(camera, dir, balls, ambientLight, directedLights);
			framebuffer[j*canvasW + i] = color;
		}
	}

	// Output to file
	ofstream ofs;
	ofs.open("./test.ppm", ofstream::binary);
	ofs << "P6\n" << canvasW << " " << canvasH << "\n255\n";
	for (int i = 0; i < canvasW*canvasH; ++i) {
		for (int j = 0; j < 3; ++j) {
			ofs << (char)(framebuffer[i][j]);
		}
	}
	ofs.close();

	cout << "All done!" << endl;

	return 0;
}
