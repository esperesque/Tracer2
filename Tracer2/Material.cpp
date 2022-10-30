#include "Material.h"
#include "Tracer.h"
#include "Vec3.h"
#include "Scene.h"
#include "Ray.h"
#include "Shape.h"

Color Material::scatter(Scene& myscene, const Ray& r, hit_record& rec, int depth) {
	return Color(0, 0, 0);
}

// Get the radiance from direct light and such
double Material::get_radiance(Scene myscene, Ray& r, hit_record rec) {
	if (lum > 0 || mirror)
		return 1;
	else {
		// do direct light calculations
		return direct_light(myscene, 3, r.at(rec.t), rec.normal);
	}
}

Ray* Material::reflect_ray(Scene& myscene, const Ray& r, hit_record& rec, int depth) {
	
	Ray* ref_ray;
	
	if (mirror) {
		// Perfect mirror reflection (todo: add perturbation)
		ref_ray = new Ray(r.at(rec.t), unit_vector(r.get_direction() - 2 * dot(r.get_direction(), rec.normal) * rec.normal));
	}
	else {
		Vec3 new_normal = unit_vector(rec.normal + 0.5*Vec3::random_in_unit_sphere());
		ref_ray = new Ray(r.at(rec.t), unit_vector(r.get_direction() - 2 * dot(r.get_direction(), new_normal) * new_normal));
	}

	ref_ray->depth = depth;
	return ref_ray;
}