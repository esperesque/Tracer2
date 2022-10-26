#pragma once

#include "Material.h"
#include "Ray.h"

struct hit_record {
	// struct for storing data to be passed back to the renderer after collision detection
	Point3D p;	// collision point
	Vec3 normal; // the surface normal at the point of collision
	double t; // distance along the ray where the intersection occurs
	double u;
	double v;
	bool front_face;

	inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
		front_face = dot(r.get_direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Shape {
public:
	// Default constructor
	Shape() = default;

	virtual bool collision(const Ray& r, hit_record& rec) = 0;

	virtual Color get_color() { return Color(0, 0, 0); }

	virtual Material get_material() { return mat; }

	// Return a random point on the surface
	virtual Point3D get_random_point() = 0;

	//Used to return surface normal
	virtual Vec3 get_normal() = 0;

	virtual double get_area() = 0;

private:
	Color color;
	Material mat;
};