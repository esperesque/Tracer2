#pragma once

#include <iostream>

enum class MaterialType {
	LAMBERT, LIGHT, MIRROR, CHECKERS, GLASS
};

class Material {
public:
	// Default constructor
	Material() = default;

	// Constructor with arguments
	Material(MaterialType mat) : type(mat) {}

	//Material(bool isMirror) : mir(isMirror) {}

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
	//bool mir;
	MaterialType type;
};