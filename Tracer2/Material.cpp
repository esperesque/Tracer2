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
double Material::get_radiance(Scene myscene, Ray& r, hit_record rec, int shadow_rays) {
	if (lum > 0 || mirror || is_transparent)
		return 1;
	else {
		// do direct light calculations
		return direct_light(myscene, shadow_rays, r.at(rec.t), rec.normal);
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
	Ray* rayOut;
	int n = 1;
	Vec3 directionRay(0, 0, 0);

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
		
	}
	rayOut = new Ray(r.at(rec.t), directionRay);
	return rayOut;
}

Ray* Material::transparent(Scene& myscene, const Ray& r, hit_record& rec) {
	double n1 = 1.0; //air
	double n2 = 1.5; //glass

	Vec3 N = unit_vector(rec.normal);
	Vec3 I = unit_vector(r.get_direction());

	//Inside or outside the glass object?
	if (!rec.front_face) {
		// Modify refraction ratio if it's not a front face (ray exits object)
		std::swap(n1, n2);
		N = N * -1;
	}

	//Schlicks law
	double cosTeta = dot(-I, N);
	double R0 = pow((n1 - n2) / (n1 + n2), 2);
	double R_coeff = R0 + (1 - R0) * pow(1 - cosTeta, 5); //reflection coeff
	double T_coeff = 1 - R_coeff; //transmission coeff

	double criticalAngle = asin(n1 / n2);
	double alpha = acos(cosTeta);

	Vec3 R = unit_vector(r.get_direction() - 2 * dot(r.get_direction(), N) * N);; //perfect reflection
	Ray* reflectionRay = new Ray(r.at(rec.t), R);

	// We need to test if the value of alpha gives a new transmitted ray
	if (alpha > criticalAngle) {
		// Totally reflected and no T exists
		return reflectionRay;
	}

	else {
		Vec3 T = unit_vector((n1 / n2) * I + N * (-(n1 / n2) * dot(N, I) - sqrt(1 - pow((n2 / n1), 2.0f) * (1.0f - pow(dot(N, I), 2)))));
		Ray* refractionRay = new Ray(r.at(rec.t), T);
		double rand = random_double();
		if (rand < R_coeff ) {
			return reflectionRay;	//Trace the reflection ray
		}
		
		return refractionRay;
	}
}