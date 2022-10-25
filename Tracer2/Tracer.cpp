#include "Tracer.h"
#include "Scene.h"

Color trace_ray(Scene myscene, const Ray& r, int depth) {
	// Maximum recursive depth reached, return black
	if (depth <= 0) {
		return Color(0, 0, 0);
	}

	Shape* nearest_object = nullptr;
	double lowest_t = 10000;
	Color last_color(0, 0, 0);
	hit_record rec;
	hit_record nearest_rec; // to store the hit record specifically for the nearest object hit

	double tex_u = 0;
	double tex_v = 0;

	// Check every shape in myscene for a collision
	for (int i = 0; i < myscene.get_size(); i++) {

		if (myscene[i]->collision(r, rec)) {
			if (rec.t < lowest_t && rec.t > 0.001) {
				// Update nearest object
				nearest_rec = rec;
				nearest_object = myscene[i];
			}
		}
	}

	if (nearest_object != nullptr) {
		return(nearest_object->get_material().scatter(myscene, r, nearest_rec, depth));
	}

	// no object found, return black
	return Color(0, 0, 0);
}

Color path_tracer(Scene myscene, Ray& r) {
	// Build a ray path and save the final ray
	//std::cout << "\nRay sent from " << r.get_origin();

	Ray last_ray = build_path(myscene, r);
	//std::cout << "\nDepth of the last ray is " << last_ray.depth;

	//std::cout << "\nPrinting raypath:";
	//print_raypath(r);
	//print_reverse_raypath(last_ray);

	return terminate_ray(myscene, last_ray);
}

void print_raypath(Ray& first_ray) {
	Ray* r = &first_ray;
	int i = 1;
	while (true) {
		std::cout << "\nDepth of ray " << i << ": " << r->depth;
		i++;
		r = r->next_ray;
		if (r == nullptr) {
			std::cout << "\nr was null at ray " << i;
			break;
		}
	}
}

void print_reverse_raypath(Ray& last_ray) {
	Ray* r = &last_ray;
	int i = last_ray.depth + 1;
	while (true) {
		std::cout << "\nDepth of ray " << i << ": " << r->depth;
		i--;
		r = r->prev_ray;
		if (r == nullptr) break;
	}
}

Ray build_path(Scene myscene, Ray& origin_ray) {
	//std::cout << "\nProcessing ray of depth " << origin_ray.depth;
	if (origin_ray.prev_ray == nullptr) {
		//std::cout << "\nPrevious ray is null";
	}
	else {
		//std::cout << "\nPrevious ray has depth " << origin_ray.prev_ray->depth;
	}
	if (origin_ray.depth >= 7) {
		// sanity check
		return origin_ray;
	}

	hit_record rec;
	hit_record nearest_rec; // to store the hit record specifically for the nearest object hit
	double lowest_t = 10000;
	Shape* nearest_object = nullptr;
	Ray* ref_ray;

	// Check every shape in myscene for a collision
	for (int i = 0; i < myscene.get_size(); i++) {

		if (myscene[i]->collision(origin_ray, rec)) {
			if (rec.t < lowest_t && rec.t > 0.001) {
				// Update nearest object
				nearest_rec = rec;
				lowest_t = rec.t;
				nearest_object = myscene[i];
			}
		}
	}

	if (nearest_object != nullptr) {
		// There was a collision
		//std::cout << "\nCollision occured at: ";
		//std::cout << origin_ray.at(nearest_rec.t);

		if (nearest_object->get_material().light()) {
			// terminate path
			origin_ray.radiance = Color(1, 1, 1);
			return origin_ray;
		}
		else {
			// TEMP: reflect as if a perfect mirror and add to the ray path
			// THIS IS WRONG AND SHOULD BE REPLACED WITH A MATERIAL-SPECIFIC SCATTER FUNCTION
			auto t = nearest_rec.t;
			//std::cout << "\nOrigin of the reflected ray: ";
			//std::cout << origin_ray.at(t);
			ref_ray = new Ray(origin_ray.at(t), unit_vector(origin_ray.get_direction() - 2 * dot(origin_ray.get_direction(), nearest_rec.normal) * nearest_rec.normal));
			origin_ray.next_ray = ref_ray;
			ref_ray->prev_ray = &origin_ray;
			ref_ray->depth = origin_ray.depth + 1;
			//std::cout << "\nConnected ray of depth " << origin_ray.depth << " to ray of depth " << ref_ray->depth;
			//std::cout << "\nOrigin ray now connects to ray of depth " << origin_ray.next_ray->depth;
			return build_path(myscene, *ref_ray);
		}
	}
	else {
		// No collision, terminate ray
		origin_ray.radiance = Color(1, 0, 0);
		return origin_ray;
	}
}

Color terminate_ray(Scene myscene, Ray& r) {
	// If the ray terminates on a light -- the radiance is (1, 1, 1)
	// If the ray terminates on a lambertian surface, the radiance is the brightness of the surface after direct light calculation

	if (r.prev_ray == nullptr) {
		// This is the final ray - we can just return the radiance of this ray.
		return r.radiance;
	}
	else {
		//std::cout << "\nTerminating " << r;
		return terminate_ray(myscene, *r.prev_ray);
	}	
}