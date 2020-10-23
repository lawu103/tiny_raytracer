/*
 * geometry.h
 *
 *  Created on: Sep. 27, 2020
 *      Author: lawu103
 */

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <math.h>

#include <cassert>
#include <limits>
#include <vector>

template<size_t dim, typename T> class Vec {
private:
	T elements[dim];

public:
	Vec() {
		for (size_t i = 0; i < dim; ++i) {
			elements[i] = std::numeric_limits<T>::max();
		}
	}

	~Vec() {}

	T& operator[](size_t i) {
		assert(i < dim);
		return elements[i];
	}

	Vec& operator=(const Vec& a) {
		for (size_t i = 0; i < dim; ++i) {
			elements[i] = a[i];
		}
	}
};

template<typename T> class Vec<3, T> {
private:
	T x, y, z;

public:
	Vec(): x(T()), y(T()), z(T()) {}
	Vec(T X, T Y, T Z): x(X), y(Y), z(Z) {}
	~Vec() {}

	const float length() const {
		return sqrt((*this)*(*this));
	}

	const T& operator[](const size_t i) const {
		assert(i < 3);
		return i == 0 ? x : (i == 1 ? y : z);
	}

	T& operator[](size_t i) {
		assert(i < 3);
		return i == 0 ? x : (i == 1 ? y : z);
	}
};

typedef Vec<3, float> Vec3f;

template<size_t dim, typename T> Vec<dim, T> operator+(Vec<dim, T> a, const Vec<dim, T>& b) {
	for (size_t i = 0; i < dim; ++i) {
		a[i] += b[i];
	}
	return a;
}

template<size_t dim, typename T> Vec<dim, T> operator-(Vec<dim, T> a, const Vec<dim, T>& b) {
	for (size_t i = 0; i < dim; ++i) {
		a[i] -= b[i];
	}
	return a;
}

template<size_t dim, typename T> T operator*(const Vec<dim, T>& a, const Vec<dim, T>& b) {
	T dotProd = T();
	for (size_t i = 0; i < dim; ++i) {
		dotProd += a[i]*b[i];
	}
	return dotProd;
}

template<size_t dim, typename T> Vec<dim, T> operator*(const float& c, Vec<dim, T> a) {
	for (size_t i = 0; i < dim; ++i) {
		a[i] *= c;
	}
	return a;
}

#endif /* GEOMETRY_H_ */
