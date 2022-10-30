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
		//ref_ray = new Ray(transparent(myscene, r, rec));
	}
	else if (is_transparent) {
		ref_ray = transparent(myscene, r, rec);
	}
	else {
		ref_ray = getRandomDirection(r, rec);
	}

	ref_ray->depth = depth;
	return ref_ray;
}

Ray* Material::getRandomDirection(const Ray& r, hit_record& rec){
	double p = random_double();
	Ray* rayOut = new Ray();
	int n = 1;

	for (int i = 0; i < n; i++) {
		double yi = random_double();
		double yi_next = random_double();
		double azimuth = 2 * M_PI * yi / p;
		double inclination = acos(sqrt(yi_next));

		//Russian roulette
		if (azimuth > 2 * M_PI && r.depth > 1) {
			n = i + 1;
			break;
		}

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
		rayOut = new Ray(r.at(rec.t), directionRay);
	}
	return rayOut;
}

Ray* Material::transparent(Scene& myscene, const Ray& r, hit_record& rec){
	float n1 = 1;
	float n2 = rec.RI;
	float cost = dot(rec.normal, r.get_direction() * -1);

	Vec3 N = rec.normal;
	if (cost < 0)
		cost = -cost;
	else
	{
		std::swap(n1, n2);
		N = N * -1;
	}

	Vec3 R = unit_vector(r.get_direction() - 2 * dot(r.get_direction(), N) * N);; //perfect reflection
	Vec3 tmp = ((n1 / n2) * r.get_direction() + N * (-n1 / n2) * dot(r.get_direction(), N)) 
				- sqrt(1 - pow((n1 / n2), 2) * (1 - pow(dot(r.get_direction(), N), 2))); //refract.
	if (tmp.get_x() < 0 || tmp.get_y() < 0 || tmp.get_z() < 0)
		tmp = { 0,0,0 };

	Vec3 T = unit_vector(tmp); //refraction
	
	//Ray refl_ray2 = build_path(myscene, refl_ray);	//Trace the reflection ray
	//Ray refr_ray2 = build_path(myscene, refr_ray); 	//Trace the refraction ray

	double R0 = pow((n1 - n2) / (n1 + n2), 2);
	double R_coeff = R0 + (1 - R0) * pow(1 - cos(cost), 5); //reflection coeff
	double T_coeff = R_coeff - 1; //transmission coeff
	double rand = random_double();

	if (rand <= R_coeff) {
		Ray* refl_ray = new Ray(r.at(rec.t), R);
		return refl_ray;	//Trace the reflection ray
	}
	else {
		Ray* refr_ray = new Ray(r.at(rec.t), T);
		return refr_ray; 	//Trace the refraction ray
	}
}