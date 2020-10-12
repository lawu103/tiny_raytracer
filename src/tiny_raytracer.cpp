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

Vec3f canvasToPort(float canvasX, float canvasY, int canvasWidth, int canvasHeight,
		int portWidth, int portHeight, int portDepth) {
	return Vec3f(canvasX*portWidth/canvasWidth, canvasY*portHeight/canvasHeight, portDepth);
}

Vec3f traceRay(const Vec3f& camera, const Vec3f& dir, const Sphere& ball) {
	if (ball.ray_intersects(camera, dir)) {
		return ball.get_color();
	}
	return Vec3f(255, 255, 255);
}

int main() {
	Sphere ball(Vec3f(0, -1, 3), 1, Vec3f(255, 0, 0));

	const Vec3f camera(0, 0, 0);
	const int canvasWidth = 1024;
	const int canvasHeight = 1024;
	const int portWidth = 1;
	const int portHeight = 1;
	const int portDepth = 1;

	vector<Vec3f> framebuffer(canvasWidth*canvasHeight);

	// Fill in color values
	for (int j = 0; j < canvasHeight; ++j) {
		for (int i = 0; i < canvasWidth; ++i) {
			float canvasX = i - canvasWidth/2.f;
			float canvasY = canvasHeight/2.f - j;
			Vec3f dir = canvasToPort(canvasX, canvasY, canvasWidth, canvasHeight,
					portWidth, portHeight, portDepth);
			Vec3f color = traceRay(camera, dir, ball);
			framebuffer[j*canvasWidth + i] = color;
		}
	}

	// Output to file
	ofstream ofs;
	ofs.open("./test.ppm", ofstream::binary);
	ofs << "P6\n" << canvasWidth << " " << canvasHeight << "\n255\n";
	for (int i = 0; i < canvasWidth*canvasHeight; ++i) {
		for (int j = 0; j < 3; ++j) {
			ofs << (char)(framebuffer[i][j]);
		}
	}
	ofs.close();

	return 0;
}
