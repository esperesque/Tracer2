#include "Ray.h"
#include "Scene.h"

bool Ray::is_blocked(Scene& room, double distance) {

	//double lowest_t = 10000;

	hit_record rec;

	for (int i = 0; i < room.get_size(); i++) {
		if (room[i]->collision(*this, rec)) {
			if (room[i]->get_material().glass()) {
				// ignore glass material when casting shadow rays - hacky solution, in the future calculate refraction/reflection
				continue;
			}
			if (rec.t > 0.1) {
				// using a higher than normal tmin to prevent the light source itself or very near objects from blocking
				// Update nearest object
				// 
				// Collision detected. Was it with an object between the light source and surface?
				
				double delta = distance - rec.t;
				if (delta > 0.1) {
					return true;
				}

				//nearest_object = myscene[i];
				//lowest_t = rec.t;
				//object_normal = rec.normal;
			}
		}
	}

	return false;
}