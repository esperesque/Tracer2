#pragma once

#include <iostream>
#include <cmath>

#include "Utility.h"

using std::sqrt;
using std::pow;

class Vec3 {
public:
	//Default construtor
	Vec3() : x(0), y(0), z(0) {}

	//Construtor
	Vec3(double x0, double y0, double z0) : x(x0), y(y0), z(z0) {}

	//Default destructor
	~Vec3() = default;

	//Return the x-value
	double get_x() const {
		return x;
	}

	//Return the y-value
	double get_y() const {
		return y;
	}

	//Return the z-value
	double get_z() const {
		return z;
	}

	//Some random shit operator.
	Vec3 operator-() const {
		return Vec3(-x, -y, -z);
	}

	//Operator for addition
	Vec3& operator+=(const Vec3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	//Operator for multiplication
	Vec3& operator*=(const double t) {
		x *= t;
		y *= t;
		z *= t;
		return *this;
	}

	//Operator for division.
	Vec3& operator/=(const double t) {
		return *this *= 1 / t;
	}

	//used for normalizing. Return the denominator.
	double length() const {
		return sqrt((pow(x, 2) + pow(y, 2) + pow(z, 2)));
	}

	// Utility functions

	//Used for printing to console.
	inline friend std::ostream& operator<<(std::ostream& out, const Vec3& v) {
		return out << v.x << ' ' << v.y << ' ' << v.z;
	}

	inline friend Vec3 operator+(const Vec3& u, const Vec3& v) {
		return Vec3(u.x + v.x, u.y + v.y, u.z + v.z);
	}

	inline friend Vec3 operator-(const Vec3& u, const Vec3& v) {
		return Vec3(u.x - v.x, u.y - v.y, u.z - v.z);
	}

	inline friend Vec3 operator*(const Vec3& u, const Vec3& v) {
		return Vec3(u.x * v.x, u.y * v.y, u.z * v.z);
	}

	inline friend Vec3 operator*(double t, const Vec3& v) {
		return Vec3(t * v.x, t * v.y, t * v.z);
	}

	inline friend Vec3 operator*(const Vec3& v, double t) {
		return t * v;
	}

	inline friend Vec3 operator/(Vec3 v, double t) {
		return (1 / t) * v;
	}

	//Calculate the dot product.
	inline friend double dot(const Vec3& u, const Vec3& v) {
		return u.x * v.x + u.y * v.y + u.z * v.z;
	}

	//Calculate the cross product.
	inline friend Vec3 cross(const Vec3& u, const Vec3& v) {
		return Vec3(u.y * v.z - u.z * v.y,
			u.z * v.x - u.x * v.z,
			u.x * v.y - u.y * v.x);
	}

	//Normalize the vector
	inline friend Vec3 unit_vector(Vec3 v) {
		return v / v.length();
	}

	// Returns a vector with random values in the 0-1 range
	inline static Vec3 random() {
		return Vec3(random_double(), random_double(), random_double());
	}

	// Returns a vector with random values in a specified range
	inline static Vec3 random(double min, double max) {
		return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	// Return a vector to a random point in a unit sphere
	inline static Vec3 random_in_unit_sphere() {
		while (true) {
			auto p = Vec3::random(-1, 1);
			if (pow(p.length(), 2) >= 1) continue;
			return p;
		}
	}

	inline static Vec3 random_unit_vector() {
		return unit_vector(random_in_unit_sphere());
	}

	// Calculates the refracted ray given two vectors and the fraction of the refractive indices
	inline static Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
		auto cos_theta = fmin(dot(-uv, n), 1.0);
		Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
		Vec3 r_out_parallel = -sqrt(fabs(1.0 - pow(r_out_perp.length(), 2))) * n;
		return r_out_perp + r_out_parallel;
	}

	inline static Vec3 reflect(const Vec3& v, const Vec3& n) {
		return v - 2 * dot(v, n) * n;
	}

private:
	double x;
	double y;
	double z;
};

//Type alias
using Point3D = Vec3;
using Color = Vec3;