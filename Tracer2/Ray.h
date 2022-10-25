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

	//Used for printing to console.
	inline friend std::ostream& operator<<(std::ostream& out, const Ray& r) {
		out << "\nRay depth:" << r.depth;
		out << "\nRay origin: " << r.origin;
		return out;
	}

public:
	Ray* next_ray = nullptr;
	Ray* prev_ray = nullptr;
	int depth = 0;
	Color radiance = Color(0, 0, 0);
private:
	Point3D origin;
	Vec3 direction;
};