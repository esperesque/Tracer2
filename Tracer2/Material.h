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
	Material(double ref) : reflectivity(ref) {}

	Material(MaterialType mat) : type(mat){}

	//Material(bool isMirror) : mir(isMirror) {}

	// Calculate surface interactions (reflection/transmission) and recursively call trace_ray
	Color scatter(Scene& myscene, const Ray& r, hit_record& rec, int depth);

	bool mirror() {
		return(type == MaterialType::MIRROR);
	}

	bool light() {
		return(type == MaterialType::LIGHT);
	}

	bool checkers() {
		return(type == MaterialType::CHECKERS);
	}

	bool glass() {
		return(type == MaterialType::GLASS);
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

private:
	// Material properties: Reflectivity
	//bool mir;
	MaterialType type = MaterialType::LAMBERT;
	double reflectivity;
};