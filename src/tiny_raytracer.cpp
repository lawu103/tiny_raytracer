//============================================================================
// Name        : tiny_raytracer.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "geometry.h"
#include "objects.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

/*
 * Converts a canvas xy position to a viewport xyz position
 */
Vec3f canvasToPort(float canvasX, float canvasY, int canvasW, int canvasH, int portW, int portH, int portD) {
	return Vec3f(canvasX*portW/canvasW, canvasY*portH/canvasH, portD);
}

/*
 * Returns the color a ray sees
 */
Vec3f traceRay(const Vec3f& camera, const Vec3f& dir, const Sphere& ball) {
	if (ball.ray_intersects(camera, dir)) {
		return ball.get_color();
	}
	return Vec3f(255, 255, 255);
}

int main() {
	Sphere ball(Vec3f(0, -1, 3), 1, Vec3f(255, 0, 0));

	// Camera position in world coordinates
	const Vec3f camera(0, 0, 0);

	// Canvas dimensions
	const int canvasW = 1024;
	const int canvasH = 1024;

	// Viewport dimensions in world units. Note that the viewport is always fixed centred to the camera, so
	// portD is a depth that's relative to the camera.
	const int portW = 1;
	const int portH = 1;
	const int portD = 1;

	vector<Vec3f> framebuffer(canvasW*canvasH);

	// Fill in color values
	for (int j = 0; j < canvasH; ++j) {
		for (int i = 0; i < canvasW; ++i) {
			float canvasX = i - canvasW/2.f;
			float canvasY = canvasH/2.f - j;
			Vec3f dir = canvasToPort(canvasX, canvasY, canvasW, canvasH, portW, portH, portD);
			Vec3f color = traceRay(camera, dir, ball);
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

	return 0;
}
