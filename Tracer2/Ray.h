#pragma once

#include "Vec3.h"

class Scene;

class Ray {
public:
	// Default constructor
	Ray() = default;

	// Constructor with arguments
	Ray(const Point3D& orig, const Vec3& dir) : origin(orig), direction(dir) {}

	// Return the ray origin
	Point3D get_origin() const { return origin; }

	// Return the ray direction
	Vec3 get_direction() const { return direction; }

	// Return the point interpolated at t
	Point3D at(double t) const { return origin + t * direction; }

	// used for shadow ray casting
	bool is_blocked(Scene& room, double distance);

private:
	Point3D origin;
	Vec3 direction;
};