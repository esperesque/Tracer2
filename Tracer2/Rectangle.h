#pragma once

#include "Shape.h"
#include "Vec3.h"

#include <iostream>

/*
	The vertexes on a rectangle object are arranged as follows (with the normal pointing "out" of the screen)

	v2=======v3
	 \\
	  \\
	   \\
		\\
		 \\
		  \\
		   \\
			\\
	v0=======v1

	v2 -> v0 -> v1 should give the normal according to the right-hand rule
*/

class Rectangle : public Shape {
public:
	// Default constructor
	Rectangle() = default;

	// Constructor with arguments
	Rectangle(Point3D inv0, Point3D inv1, Point3D inv2, Point3D inv3, Material m, Color clr) : v0(inv0), v1(inv1), v2(inv2), v3(inv3), mat(m), color(clr) {
		edge0 = calculate_edge(v0, v2);
		edge1 = calculate_edge(v0, v1);
		normal = unit_vector(cross(edge0, edge1));
	}

	// Check if there is a collision with the ray r
	bool collision(const Ray& r, hit_record& rec) override {
		// Step 1: Find where the ray intersects the infinite plane containing the rectangle

		// t = (v-s)N / d*N
		// s is the ray origin, d is the direction
		// v is v0
		double t = dot(v0 - r.get_origin(), normal) / dot(r.get_direction(), normal);

		// Step 2: check if the intersection point is on the rectangle

		Point3D xi = r.get_origin() + t * r.get_direction();

		double a = dot((xi - v0), edge0) / dot(edge0, edge0);
		double b = dot((xi - v0), edge1) / dot(edge1, edge1);

		if (a >= 0 && a <= 1 && b >= 0 && b <= 1) {
			rec.t = t;
			rec.normal = normal;
			rec.p = r.at(t);
			// also determine rec.u and rec.v
			rec.u = a;
			rec.v = b;
			rec.set_face_normal(r, normal);
			return true;
		}
		else {
			return false;
		}
	}

	// calculate the edge between two vertex points
	Vec3 calculate_edge(Point3D ver0, Point3D ver1) {
		return Vec3(ver1 - ver0);
	}

	Material get_material() { return mat; }

	Point3D get_random_point() {
		double u = random_double();
		double v = random_double();

		Point3D random_point = v0 + u * edge0 + v * edge1;
		return random_point;
	}

	Color get_color() {
		return color;
	}

private:
	Point3D v0, v1, v2, v3;
	Vec3 edge0, edge1;
	Vec3 normal;
	Color color;
	Material mat;
};