#include "Material.h"
#include "Tracer.h"
#include "Vec3.h"
#include "Scene.h"

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