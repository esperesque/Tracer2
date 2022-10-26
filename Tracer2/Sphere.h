#pragma once

#include "Shape.h"
#include "Ray.h"
#include "Vec3.h"

class Sphere : public Shape {
public:
	// Default constructor
	Sphere() = default;

	// Constructor with arguments
	Sphere(Point3D orig, double rad, Material m, Color clr) : origin(orig), radius(rad), mat(m), color(clr) {}

	// Constructor without color, assumes color is defined within the material
	Sphere(Point3D orig, double rad, Material m) : origin(orig), radius(rad), mat(m) {}

	// Check if there is a collision with the ray r
	bool collision(const Ray& r, hit_record& rec) override {
		Vec3 oc = r.get_origin() - origin;
		auto a = pow(r.get_direction().length(), 2);
		auto half_b = dot(oc, r.get_direction());
		auto c = pow(oc.length(), 2) - radius * radius;

		auto discriminant = half_b * half_b - a * c;

		if (discriminant < 0) {
			return false;
		}
		auto sqrtd = sqrt(discriminant);

		// Find the nearest root

		auto root = (-half_b - sqrtd) / a;
		if (root < 0.01 || 10000 < root) {
			root = (-half_b + sqrtd) / a;
			if (root < 0.01 || 10000 < root)
				return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		rec.normal = (rec.p - origin) / radius;
		rec.u = 0.5; // temp
		rec.v = 0.5;
		Vec3 outward_normal = (rec.p - origin) / radius;
		rec.set_face_normal(r, outward_normal);

		return true;
	}

	Point3D get_random_point() {
		// need to figure out a way to return a point on the sphere that is facing the object. for now, set to whatever and assume
		// spherical light sources don't work.
		return Point3D(0, 0, 0);
	}

	Material get_material() { return mat; }

	Color get_color() {
		return color;
	}

private:
	Point3D origin;
	double radius;
	Color color;
	Material mat;
};