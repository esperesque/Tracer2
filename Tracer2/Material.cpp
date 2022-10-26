#include "Material.h"
#include "Tracer.h"
#include "Vec3.h"
#include "Scene.h"
#include "Ray.h"
#include "Shape.h"

Color Material::scatter(Scene& myscene, const Ray& r, hit_record& rec, int depth) {
	if (reflectivity == 1) {
		// Completely reflective, so we do not need to cast shadow rays
	}
	if (type == MaterialType::LAMBERT) {
		return Color(0.2, 1, 0.2);
	}
	else if (type == MaterialType::LIGHT) {
		return Color(1, 1, 1);
	}
	else if (type == MaterialType::CHECKERS) {
		return Color(0, 0, 0.5);
	}
	else {
		// reflect as if a mirror
		auto t = rec.t;
		Ray ref_ray(r.at(t), unit_vector(r.get_direction() - 2 * dot(r.get_direction(), rec.normal) * rec.normal));
		return trace_ray(myscene, ref_ray, depth - 1);
	}
}

// Get the surface color from direct light and such
Color Material::get_radiance(Scene myscene, Ray& r, hit_record rec) {
	if (!mirror) {
		// do direct light calculations
		return direct_light(myscene, 3, r.at(rec.t), rec.normal, color);
	}
	return color;
}

Ray* Material::reflect_ray(Scene& myscene, const Ray& r, hit_record& rec, int depth) {
	
	Ray* ref_ray;
	
	if (mirror) {
		// Perfect mirror reflection (todo: add perturbation)
		ref_ray = new Ray(r.at(rec.t), unit_vector(r.get_direction() - 2 * dot(r.get_direction(), rec.normal) * rec.normal));
		ref_ray->depth = depth;
	}
	else {
		Vec3 new_normal = unit_vector(rec.normal + 0.5*Vec3::random_in_unit_sphere());
		ref_ray = new Ray(r.at(rec.t), unit_vector(r.get_direction() - 2 * dot(r.get_direction(), new_normal) * new_normal));
		ref_ray->depth = depth;
	}

	
	return ref_ray;
}