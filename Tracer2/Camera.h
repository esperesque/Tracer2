#pragma once

class Vec3;
class Ray;

#include <string>
#include "Vec3.h"

class Scene;

class Camera {
public:
	// Default constructor
	Camera() = default;

	// Constructor with arguments
	Camera(Vec3& ep, int w, int h, double foc = 1.0) : eye_position(ep), width(w), height(h), focal_length(foc) {
		// Initialize dependent parameters
		aspect_ratio = (double)width / (double)height;
		viewport_width = viewport_height * aspect_ratio;

		horizontal = Vec3(0, viewport_width, 0); // the y-axis is our horizontal direction
		vertical = Vec3(0, 0, viewport_height); // the z-axis is our vertical direction

		lower_left_corner = eye_position - horizontal / 2 - vertical / 2 + Vec3(focal_length, 0, 0); //Lower left corner, should return (0, -1, -1)
	}

	// Render a picture with the name "filename.png"
	void take_picture(Scene myscene, std::string filename) const;

	void render_normals(Scene myscene, std::string filename) const;

	// Set the degree of antialiasing (samples per pixel)
	void set_aa(int new_aa);

	void set_srays(int new_srays);

	// Render the normals of objects as colors for debug purposes
	Color trace_normals(Scene myscene, const Ray& r) const;

	// Send a ray and get its color
	Color send_ray(Scene myscene, const Ray& r, int depth) const;

	// Test function, delete later
	Color ray_color(const Ray& r) const;

private:
	// Output image parameters
	int width;
	int height;
	int samples_per_pixel = 10;
	int shadow_rays = 10;
	int max_depth = 5; // maximum recursive depth
	const int channels = 3; // no reason yet to support other channel counts

	// Internal viewport parameters
	Point3D eye_position;
	Point3D lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
	double aspect_ratio;
	double viewport_height = 2.0;
	double viewport_width;
	double focal_length;
};