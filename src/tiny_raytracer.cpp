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

#define EPSILON_F numeric_limits<float>::epsilon()
#define MAX_F numeric_limits<float>::max()

/*
 * Converts a canvas xy position to a viewport xyz position
 */
Vec3f canvas_to_port(float canvasX, float canvasY, int canvasW, int canvasH, int portW, int portH, int portD) {
	return Vec3f(canvasX*portW/canvasW, canvasY*portH/canvasH, portD);
}

/*
 * Reflects R over N
 */
Vec3f reflect_ray(const Vec3f& R, const Vec3f& N) {
	return 2*(R*N)*N - R;
}

/*
 * Calculates total lighting on a point
 */
float compute_lighting(const Vec3f& dir, const Vec3f& P, const Vec3f& N, const size_t closestBallInd,
		const vector<Sphere>& balls, const Light& ambientLight, const vector<DirectedLight>& directedLights) {
	float i = ambientLight.get_intensity();
	for (const DirectedLight& directedLight: directedLights) {
		Vec3f L;
		if (directedLight.is_point()) {
			L = directedLight.get_v() - P;
		} else {
			L = -directedLight.get_v();
		}

		// Shadow
		bool isShadowed = false;
		for (const Sphere& ball: balls) {
			float t = ball.ray_intersection(P, L);
			if (t < MAX_F && t > 0) {
				isShadowed = true;
				break;
			}
		}

		if (!isShadowed) {
			// Diffuse
			float NdotL = N*L;
			if (NdotL > 0) {
				i += directedLight.get_intensity() * NdotL/(N.length()*L.length());
			}

			// Specular
			float s = balls[closestBallInd].get_specular();
			if (s > 0) {
				Vec3f R = reflect_ray(L, N);
				Vec3f V = -dir;
				float RdotV = R*V;
				if (RdotV > 0) {
					i += directedLight.get_intensity() * pow(RdotV/(R.length()*V.length()), s);
				}
			}
		}
	}
	return i;
}

/*
 * Returns the color a ray sees
 */
Vec3f trace_ray(const Vec3f& camera, const Vec3f& dir, const vector<Sphere>& balls, const Light& ambientLight,
				const vector<DirectedLight>& directedLights, const int recurseLimit) {
	float tmin = numeric_limits<float>::max();
	Vec3f color = Vec3f(0, 0, 0);
	int closestBallInd = -1;
	for (size_t i = 0; i < balls.size(); ++i) {
		float t = balls[i].ray_intersection(camera, dir);
		if (t < tmin && t >= 0) {
			tmin = t;
			closestBallInd = i;
		}
	}

	if (closestBallInd < 0) {
		return color;
	}

	Vec3f P = camera + tmin*dir;
	Vec3f N = P - balls[closestBallInd].get_center();
	N = 1/N.length() * N;
	float lighting = compute_lighting(dir, P, N, closestBallInd, balls, ambientLight, directedLights);
	color = lighting*balls[closestBallInd].get_color();

	float r = balls[closestBallInd].get_reflective();
	if (recurseLimit > 0 && r > 0) {
		Vec3f R = reflect_ray(-dir, N);
		color = (1 - r)*color + r*trace_ray(P, R, balls, ambientLight, directedLights, recurseLimit - 1);
	}

	return Vec3f(min(255.f, color[0]), min(255.f, color[1]), min(255.f, color[2]));
}

int main() {
	vector<Sphere> balls = {Sphere(Vec3f(0, -1, 3), 1, Vec3f(255, 0, 0), 500, 0.2),
							Sphere(Vec3f(-2, 0, 4), 1, Vec3f(0, 255, 0), 10, 0.4),
							Sphere(Vec3f(2, 0, 4), 1, Vec3f(0, 0, 255), 500, 0.3),
							Sphere(Vec3f(0, -5001, 0), 5000, Vec3f(255, 255, 0), 1000, 0.5)};

	// The sum of the intensities of our lights should be 1 if we don't want overexposure.
	Light ambientLight(0.2);
	vector<DirectedLight> directedLights = {DirectedLight(0.6, true, Vec3f(2, 1, 0)),
											DirectedLight(0.2, false, Vec3f(-1, -4, -4))};

	// The recursion limit for reflections
	const int recurseLimit = 3;

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
			Vec3f color = trace_ray(camera, dir, balls, ambientLight, directedLights, recurseLimit);
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
