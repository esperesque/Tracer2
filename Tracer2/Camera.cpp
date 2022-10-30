#pragma once

#include "Camera.h"
#include "Ray.h"
#include "Scene.h"
#include "Shape.h"
#include "Utility.h"
#include "Material.h"
#include "Tracer.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__
#include "stb_image_write.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

void Camera::take_picture(Scene myscene, std::string filename) const {
	// Render an image and save it as filename.png
	uint8_t* pixels = new uint8_t[width * height * channels];
	int index = 0;
	double highest_value = 0;
	double* dbl_pixels = new double[width * height * channels];

	for (int i = height - 1; i >= 0; i--) {
		double percent = floor((double(height - i) / double(height)) * 100);
		std::cout << "\rRendering... " << percent << "% finished";

		for (int j = 0; j < width; j++) {

			Color pixel_color(0, 0, 0);

			for (int s = 0; s < samples_per_pixel; s++) {
				auto u = (double(j) + random_double()) / (width - 1);
				auto v = (double(i) + random_double()) / (height - 1);

				Ray r(eye_position, unit_vector(lower_left_corner + u * horizontal + v * vertical - eye_position));
				//pixel_color += send_ray(myscene, r, max_depth);
				//pixel_color += trace_ray(myscene, r, max_depth);
				//Ray last_ray = build_path(myscene, r);
				pixel_color += path_tracer(myscene, r);
			}

			auto scale = 1.0 / samples_per_pixel;
			pixel_color *= scale;
			
			//Color pixel_color = ray_color(r);
			//Color pixel_color = send_ray(myscene, r);

			//std::cout << "\nPixel color is " << pixel_color;

			if (pixel_color.get_x() > highest_value) {
				highest_value = pixel_color.get_x();
			}
			if (pixel_color.get_y() > highest_value) {
				highest_value = pixel_color.get_y();
			}
			if (pixel_color.get_z() > highest_value) {
				highest_value = pixel_color.get_z();
			}

			dbl_pixels[index++] = pixel_color.get_x();
			dbl_pixels[index++] = pixel_color.get_y();
			dbl_pixels[index++] = pixel_color.get_z();
			//pixels[index++] = static_cast<int>(255.999 * pixel_color.get_x());
			//pixels[index++] = static_cast<int>(255.999 * pixel_color.get_y());
			//pixels[index++] = static_cast<int>(255.999 * pixel_color.get_z());
		}
	}

	index = 0;
	for (int i = height - 1; i >= 0; i--) {
		for (int j = 0; j < width; j++) {
			pixels[index] = static_cast<int>(255.999 * (dbl_pixels[index++] / highest_value));
			pixels[index] = static_cast<int>(255.999 * (dbl_pixels[index++] / highest_value));
			pixels[index] = static_cast<int>(255.999 * (dbl_pixels[index++] / highest_value));
		}
	}
	
	//std::cout << "\nThe highest pixel color value was " << highest_value;
	filename += ".png";

	// Convert the string to a c-string
	const char* c = filename.c_str();
	stbi_write_png(c, width, height, channels, pixels, width * channels);
}

void Camera::render_normals(Scene myscene, std::string filename) const {
	// Render an image and save it as filename.png
	uint8_t* pixels = new uint8_t[width * height * channels];
	int index = 0;

	for (int i = height - 1; i >= 0; i--) {
		double percent = floor((double(height - i) / double(height)) * 100);
		std::cout << "\rRendering... " << percent << "% finished";

		for (int j = 0; j < width; j++) {

			Color pixel_color(0, 0, 0);

			for (int s = 0; s < samples_per_pixel; s++) {
				auto u = (double(j) + random_double()) / (width - 1);
				auto v = (double(i) + random_double()) / (height - 1);

				Ray r(eye_position, unit_vector(lower_left_corner + u * horizontal + v * vertical - eye_position));
				pixel_color = trace_normals(myscene, r);
			}

			auto scale = 1.0 / samples_per_pixel;
			pixel_color *= scale;

			//Color pixel_color = ray_color(r);
			//Color pixel_color = send_ray(myscene, r);



			//std::cout << "\nPixel color is " << pixel_color;

			pixels[index++] = static_cast<int>(255.999 * pixel_color.get_x());
			pixels[index++] = static_cast<int>(255.999 * pixel_color.get_y());
			pixels[index++] = static_cast<int>(255.999 * pixel_color.get_z());
		}
	}

	filename += ".png";

	// Checking the highest pixel color value

	double highest_pixel_value;
	
	index = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {

		}
	}

	// Convert the string to a c-string
	const char* c = filename.c_str();
	stbi_write_png(c, width, height, channels, pixels, width * channels);
}

void Camera::set_aa(int new_aa) {
	samples_per_pixel = new_aa;
}

void Camera::set_srays(int new_srays) {
	shadow_rays = new_srays;
}

// Debug function used to draw the normals as colors
Color Camera::trace_normals(Scene myscene, const Ray& r) const {
	Shape* nearest_object = nullptr;
	Vec3 object_normal;
	double lowest_t = std::numeric_limits<double>::max();

	for (int i = 0; i < myscene.get_size(); i++) {
		hit_record rec;
		if (myscene[i]->collision(r, rec)) {
			if (rec.t < lowest_t && rec.t > 0.001) {
				// Update nearest object
				nearest_object = myscene[i];
				lowest_t = rec.t;
				object_normal = rec.normal;
			}
		}
	}

	if (nearest_object != nullptr) {
		return 0.5 * (object_normal + Color(1, 1, 1));
	}
	return Color(0, 0, 0);
}

Color Camera::send_ray(Scene myscene, const Ray& r, int depth) const {
	// All this code has been moved to Tracer.cpp
	return Color(0, 0, 0);
}

Color Camera::ray_color(const Ray& r) const {
	Vec3 unit_direction = unit_vector(r.get_direction());
	auto t = 0.5 * (unit_direction.get_y() + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}