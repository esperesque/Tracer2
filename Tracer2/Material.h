#pragma once

#include <iostream>
#include "Vec3.h"

// forward declarations
struct hit_record;
class Ray;
class Scene;

enum class MaterialType {
	LAMBERT, LIGHT, MIRROR, CHECKERS, GLASS
};

class Material {
public:
	// Default constructor
	Material() = default;

	// Constructor with arguments
	Material(bool mir, double in_rho, Color clr, double pert, double in_lum) : mirror(mir), rho(in_rho), color(clr), perturbation(pert), lum(in_lum) {}

	Material(double ref) : reflectivity(ref) {}

	Material(MaterialType mat) : type(mat){}

	//Material(bool isMirror) : mir(isMirror) {}

	// Calculate surface interactions (reflection/transmission) and recursively call trace_ray
	Color scatter(Scene& myscene, const Ray& r, hit_record& rec, int depth);

	Color get_color() {
		return color;
	}

	Ray* reflect_ray(Scene& myscene, const Ray& r, hit_record& rec, int depth);

	Ray* getRandomDirection(const Ray& r, hit_record& rec);

	double get_radiance(Scene myscene, Ray& r, hit_record rec);

	double get_rho() {
		return rho;
	}

	bool is_mirror() {
		return(type == MaterialType::MIRROR);
	}

	bool light() {
		return(type == MaterialType::LIGHT || lum > 0);
	}

	bool checkers() {
		return(type == MaterialType::CHECKERS);
	}

	bool glass() {
		return(type == MaterialType::GLASS);
	}

	bool lambert() {
		return(type == MaterialType::LAMBERT);
	}

	friend std::ostream& operator<<(std::ostream& os, Material& mat) {
		if (mat.type == MaterialType::LAMBERT) {
			os << "Material: Lambertian";
		}
		else if (mat.type == MaterialType::MIRROR) {
			os << "Material: Mirror";
		}
		else {
			os << "Material: Unknown";
		}
		return os;
	}
public:
	double rho; // The probability of ray termination
	double perturbation; // The radius of a unit sphere in which the normal is dislocated
	bool mirror; // Whether the material reflects perfectly
	Color color; // Color of the material
	double lum; // Intensity of the light; set to 0 if material is not a light source

private:
	// Material properties: Reflectivity
	//bool mir;
	MaterialType type = MaterialType::LAMBERT;
	double reflectivity;
};