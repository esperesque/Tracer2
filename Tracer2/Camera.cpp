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

			pixels[index++] = static_cast<int>(255.999 * pixel_color.get_x());
			pixels[index++] = static_cast<int>(255.999 * pixel_color.get_y());
			pixels[index++] = static_cast<int>(255.999 * pixel_color.get_z());
		}
	}
	
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

Color Camera::trace_normals(Scene myscene, const Ray& r) const {
	Shape* nearest_object = nullptr;
	Vec3 object_normal;
	double lowest_t = 10000;

	for (int i = 0; i < myscene.get_size(); i++) {
		hit_record rec;
		if (myscene[i]->collision(r, rec)) {
			if (rec.t < lowest_t && rec.t > 0.001) {
				// Update nearest object
				nearest_object = myscene[i];
				lowest_t = rec.t;
				object_normal = rec.normal;
			}
			//return myscene[i]->get_color();
		}
	}

	if (nearest_object != nullptr) {
		return 0.5 * (object_normal + Color(1, 1, 1));
	}
	return Color(0, 0, 0);
}

Color Camera::send_ray(Scene myscene, const Ray& r, int depth) const {

	// Maximum recursive depth reached, return black
	if (depth <= 0) {
		return Color(0, 0, 0);
	}

	Shape* nearest_object = nullptr;
	Vec3 object_normal;
	bool front_face;
	double lowest_t = 10000;
	Color last_color(0, 0, 0);

	double tex_u = 0;
	double tex_v = 0;

	// Check every shape in myscene for a collision
	for (int i = 0; i < myscene.get_size(); i++) {
		hit_record rec;
		if (myscene[i]->collision(r, rec)) {
			if (rec.t < lowest_t && rec.t > 0.001) {
				// Update nearest object
				nearest_object = myscene[i];
				lowest_t = rec.t;
				object_normal = rec.normal;
				tex_u = rec.u;
				tex_v = rec.v;
				front_face = rec.front_face;
			}
			//return myscene[i]->get_color();
		}
	}

	//std::cout << "\nTex u: " << tex_u << " Tex v: " << tex_v;
	Color surface_color;

	if (nearest_object != nullptr) {
		if (nearest_object->get_material().is_mirror()) {
			// object is a mirror, so get a perfect reflection
			//Formula is: d - 2(dot(d,n))n
			auto t = lowest_t;
			Ray ref_ray(r.at(t), unit_vector(r.get_direction() - 2 * dot(r.get_direction(), object_normal) * object_normal));
			return send_ray(myscene, ref_ray, depth - 1);
		}
		else if (nearest_object->get_material().light()) {
			return nearest_object->get_color();
		}
		else if (nearest_object->get_material().checkers()) {
			// Define the two colors of the checkerboard pattern
			Color even_value(0.3, 0.15, 0.8);
			Color odd_value(1, 1, 1);

			// Define the period - higher values yield smaller tiles.
			double period = 16*M_PI;

			// Determine the tile using XOR on trigonometric functions.
			if ((sin(tex_u*period) > 0) != (cos(tex_v*period) > 0)) {
				surface_color = even_value;
			}
			else {
				surface_color = odd_value;
			}
		}
		else if (nearest_object->get_material().glass()) {
			double index_of_refraction = 1.5; // hardcoded for testing purposes, should be defined in the material later

			double refraction_ratio = front_face ? (1.0 / index_of_refraction) : index_of_refraction;

			auto t = lowest_t;

			Vec3 unit_direction = unit_vector(r.get_direction());
			double cos_theta = fmin(dot(-unit_direction, object_normal), 1.0);
			double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
			bool cannot_refract = refraction_ratio * sin_theta > 1.0;

			Vec3 direction;

			if (cannot_refract) {
				direction = Vec3::reflect(unit_direction, object_normal);
			}
			else {
				direction = Vec3::refract(unit_direction, object_normal, refraction_ratio);
			}
				

			//Vec3 refracted = Vec3::refract(unit_direction, object_normal, refraction_ratio);

			Ray ref_ray(r.at(t), direction);
			return send_ray(myscene, ref_ray, depth - 1);
		}
		else {
			surface_color = nearest_object->get_color();
			//std::cout << "\nHit a lambertian surface";
			// continue as usual?
		}

		Color obj_color(0, 0, 0);

		for (int i = 0; i < myscene.get_lights(); i++) {
			// Do shadow ray casting for every light source in the scene
			auto light = myscene.ls(i);
			for (int j = 0; j < shadow_rays; j++) {
				// 1: get a random point on the surface of the light source
				Point3D light_point = light->get_random_point();

				// Cast a shadow ray and check if it is blocked
				Ray sray(light_point, unit_vector(r.at(lowest_t) - light_point));
				Vec3 distance_vector = light_point - r.at(lowest_t);
				double distance = distance_vector.length();
				//Ray sray(r.at(lowest_t), unit_vector(light_point - r.at(lowest_t)));
				if (!sray.is_blocked(myscene, distance)) {

					Vec3 ls_normal(0, 0, -1); // figure out how to compute this later

					double cosOmegaX = dot(object_normal, distance_vector) / distance;
					double cosOmegaY = -dot(ls_normal, distance_vector) / distance;

					double mod = (cosOmegaX * cosOmegaY) / pow(distance, 2);

					if (mod < 0) {
						mod = 0;
					}

					//obj_color += mod * nearest_object->get_color();
					obj_color += mod * surface_color;
				}
				else {
					// nada
				}
			}
			

		}

		double lum = 50;

		//double color_scale = 1.0 / (myscene.get_lights() * shadow_rays);
		double color_scale = lum / (myscene.get_lights() * shadow_rays);
		obj_color *= color_scale;

		// uncomment this for direct light only
		//return obj_color;

		Point3D target = r.at(lowest_t) + object_normal + Vec3::random_unit_vector();
		Ray ref_ray(r.at(lowest_t), target - r.at(lowest_t));

		// This adds indirect light by reflecting a ray in a random direction. Using a constant 0.3 which is *very wrong* and should be replaced
		// with a BRDF - once implemented
		return obj_color + 0.3 * send_ray(myscene, ref_ray, depth - 1);

		last_color = nearest_object->get_color();
		// Get the color of the nearest object
		

		//Color this_color = 0.25 * nearest_object->get_color();
		//return this_color + (0.5 * send_ray(myscene, ref_ray, depth - 1));
		return 0.5 * send_ray(myscene, ref_ray, depth - 1);
		/*
		if (rand > 0.1) {
			return 0.5 * send_ray(myscene, ref_ray, depth-1);
		}
		else {
			return nearest_object->get_color();
		}
		*/
		
		//return nearest_object->get_color();
	}
	else {
		
		Vec3 unit_direction = unit_vector(r.get_direction());
		auto t = 0.5 * (unit_direction.get_y() + 1.0);
		//return (1.0 - t) * Color(1.0, 1.0, 1.0) + t*last_color;
		return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
		//return Color(0, 0, 0);
	}

	// if none found, return black
	return Color(0, 0, 0);
}

Color Camera::ray_color(const Ray& r) const {
	Vec3 unit_direction = unit_vector(r.get_direction());
	auto t = 0.5 * (unit_direction.get_y() + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}