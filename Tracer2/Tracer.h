#pragma once

#include "Vec3.h"
#include <vector>

class Scene;
class Ray;

//std::vector<Ray*> ray_path;

// contains the actual ray tracing logic

Color trace_ray(Scene myscene, const Ray& r, int depth);

// The path tracer starts by building a path, then goes through the path in reverse and returns a color
Color path_tracer(Scene myscene, Ray& r);

void print_raypath(Ray& first_ray);

void print_reverse_raypath(Ray& last_ray);

// Takes a ray originating from the camera viewport, builds a ray path and then returns the final ray (termination)
Ray build_path(Scene myscene, Ray& origin_ray);

Color terminate_ray(Scene myscene, Ray& r);