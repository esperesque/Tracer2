#include "Material.h"
#include "Tracer.h"
#include "Vec3.h"
#include "Scene.h"
#include "Ray.h"
#include "Shape.h"
# define M_PI           3.14159265358979323846  /* pi */

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
		ref_ray = getRandomDirection(r, rec);
	}

	ref_ray->depth = depth;
	return ref_ray;
}

Ray* Material::getRandomDirection(const Ray& r, hit_record& rec){
	double p = 0.25;
	double yi = random_double();
	double yi_next = random_double();
	double azimuth = 2 * M_PI * yi / p;
	double inclination = acos(sqrt(yi_next));

	//Conversion of hemispherical into cartesian
	double x0 = cos(azimuth) * sin(inclination);
	double y0 = sin(azimuth) * sin(inclination);
	double z0 = cos(inclination);

	//Certesian local system to world coordinate system
	Vec3 N = rec.normal;
	Vec3 X = N * (dot(N, r.get_direction())) - unit_vector(r.get_direction());
	Vec3 Z = N;
	Vec3 Y = cross(Z, X);

	//Calculations
	double x = x0 * X.get_x() + y0 * Y.get_x() + z0 * Z.get_x();
	double y = x0 * X.get_y() + y0 * Y.get_y() + z0 * Z.get_y();
	double z = x0 * X.get_z() + y0 * Y.get_z() + z0 * Z.get_z();

	Vec3 directionRay = { x,y,z };
	Ray* rayOut = new Ray(r.at(rec.t), directionRay);

	return rayOut;
}
