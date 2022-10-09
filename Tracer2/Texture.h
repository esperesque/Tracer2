#pragma once

#include "Vec3.h"

class Texture {
public:
	virtual Color value(double u, double v, Point3D p) = 0;
};

class Solid_Color : public Texture {
public:
	// Default constructor
	Solid_Color() = default;

	Solid_Color(Color c) : color_value(c) {}

	Solid_Color(double red, double green, double blue) : Solid_Color(Color(red, green, blue)) {}

private:
	Color color_value;
};