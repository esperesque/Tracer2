#pragma once

#include "Shape.h"
#include "Vec3.h"
#include "Ray.h"

class Triangle : public Shape {
public:
	// Default constructor
	Triangle() = default;

	// Constructor with arguments
	Triangle(Point3D ver0, Point3D ver1, Point3D ver2, Material m, Color clr) : v0(ver0), v1(ver1), v2(ver2), mat(m), color(clr) {
		edge0 = calculate_edge(v0, v1);
		edge1 = calculate_edge(v0, v2);
		normal = unit_vector(cross(edge0, edge1));
	}

	bool collision(const Ray& ray, hit_record& rec) override {
		const float EPSILON = 0.0001; //a very small value used to check if determinant is close to 0.

		Vec3 P = cross(ray.get_direction(), edge1); //Get the vector that is orthogonal to the ray-direction and one of the edges.
		float determinant = dot(edge0, P);

		if (determinant > -EPSILON && determinant < EPSILON)
			return false; //This ray is parallell to the triangle. 

		Point3D startPoint = ray.get_origin() - v0;
		float invDet = 1 / determinant; //Inverse determinant. 
		float u = dot(startPoint, P) * invDet; //Create world coordinate u.
		if (u < 0 || u > 1)
			return false;

		Vec3 q = cross(startPoint, edge0);
		float v = invDet * dot(ray.get_direction(), q); //Create world coordinate v.
		if (v < 0 || u + v > 1)
			return false;

		//Compute t, t is ued to find where the intersection point is on the line. 
		double t = invDet * dot(edge1, q);
		if (t > EPSILON) { //ray intersection
			rec.t = t;
			rec.normal = normal;
			rec.p = ray.at(t);
			rec.u = 0.5; // temp
			rec.v = 0.5;
			rec.set_face_normal(ray, normal);
			return true;
		}
		//This means there is a line intersection but not a ray intersection.
		return false;
	}

	Material get_material() { return mat; }

	Point3D get_random_point() {
		double u = random_double();
		double v = random_double();

		Point3D random_point = v0 + u * edge0 + v * edge1;
		// Check that the random point is actually on the triangle before returning.
		// Leaving this in for now since we're only planning to use rectangular light sources for the time being

		return random_point;
	}

	Color get_color() {
		return color;
	}

	// Calculate the edge between two vertex points
	Vec3 calculate_edge(Point3D ver0, Point3D ver1) {
		return Vec3(ver1 - ver0);
	}

private:
	Point3D v0, v1, v2;
	Vec3 edge0, edge1;
	Vec3 normal;
	Color color;
	Material mat;
};