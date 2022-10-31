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

Ray* Material::transparent(Scene& myscene, const Ray& r, hit_record& rec){
	float n1 = 1;
	float n2 = rec.RI;
	n2 = 1.5;

	float cost = dot(rec.normal, r.get_direction() * -1);

	Vec3 N = rec.normal;
	/* I'm unsure about this so I commented it out. We already have a way to check if the ray hit a front-facing
	*  surface using the front_face bool inside hit_record, and using it instead gave better (but still incorrect) reflections
	if (cost < 0)
		cost = -cost;
	else
	{
		std::swap(n1, n2);
		N = N * -1;
	}
	*/

	if (!rec.front_face) {
		// Modify refraction ratio if it's not a front face (ray exits object)
		std::swap(n1, n2);
		N = N * -1;
	}
	else {
		cost = -cost;
	}

	Vec3 R = unit_vector(r.get_direction() - 2 * dot(r.get_direction(), N) * N);; //perfect reflection
	double R0 = pow((n1 - n2) / (n1 + n2), 2);
	Vec3 tmp = ((n1 / n2) * r.get_direction() + N * (-n1 / n2) * dot(r.get_direction(), N)) 
			- sqrt(1 - pow((n1 / n2), 2) * (1 - pow(dot(r.get_direction(), N), 2))); //refract.
	
	if (tmp.get_x() < 0 || tmp.get_y() < 0 || tmp.get_z() < 0)
		tmp = { 0,0,0 };

	// drefr = Rd0 + N (-R(N*d0) - sqrt(1 - R^2(1 - (N*d0)^2))

	Vec3 T = unit_vector(tmp); //refraction

	Vec3 d_ref = R0 * r.get_direction() + N * (-R0 * (dot(N, r.get_direction()) - sqrt(1 - (pow(R0, 2) * (1 - pow(dot(N, r.get_direction()), 2))))));
	d_ref = unit_vector(d_ref);

	double R_coeff = R0 + (1 - R0) * pow(1 - cos(cost), 5); //reflection coeff
	double T_coeff =  1 - R_coeff; //transmission coeff

	double rand = random_double();
	//std::cout << "\nReflection coefficient was " << R_coeff;
	//std::cout << "\nRefraction vector: " << d_ref;

	if (rand <= R_coeff && false) {
		Ray* refl_ray = new Ray(r.at(rec.t), R);
		return refl_ray;	//Trace the reflection ray
	}
	else {
		Ray* refr_ray = new Ray(r.at(rec.t), d_ref);
		return refr_ray; 	//Trace the refraction ray
	}
}