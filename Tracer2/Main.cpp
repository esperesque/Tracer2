#pragma once

#include "Vec3.h"
#include "Camera.h"
#include "Scene.h"
#include "Sphere.h"
#include "Rectangle.h"
#include "Triangle.h"
#include "Material.h"

#include <math.h>
#include <iostream>
#include <vector>
#include <time.h>



int main()
{
	// Define some colors
	
	const Color BLUE(0, 0, 1);
	const Color RED(1, 0, 0);
	const Color ORANGE(1, 0.4, 0.1);
	const Color GREEN(0, 1, 0);
	const Color DARK_GREEN(0.1, 0.4, 0.1);
	const Color DARK_GRAY(0.3, 0.3, 0.3);
	const Color MID_GRAY(0.5, 0.5, 0.5);
	const Color LIGHT_GRAY(0.7, 0.7, 0.7);
	const Color WHITE(1, 1, 1);
	const Color BLACK(0, 0, 0);

	// Old way of definining materials
	Material MIRROR(MaterialType::MIRROR);
	Material LAMBERT(MaterialType::LAMBERT);
	Material LIGHT(MaterialType::LIGHT);
	Material CHECKERS(MaterialType::CHECKERS);
	Material GLASS(MaterialType::GLASS);

	Material orange_matte(false, 0.4, ORANGE, 0, 0); // Orange material with a rho of 0.4
	Material other_matte(false, 0.4, Color(0.4, 0.6, 1.0), 0, 0); // IDK what color, rho of 0.4
	Material floor_material(false, 0.5, Color(0.2, 0.6, 0.4), 0, 0);
	Material wall_material(false, 0.6, Color(0.3, 0.2, 0.7), 0, 0);
	Material white_wall(false, 1, WHITE, 0, 0);
	Material mirror(true, 1, WHITE, 0, 0); // mirrors should be white according to Mark's slides
	Material frosted_mirror(true, 1, WHITE, 0.4, 0); // mirror with a perturbed reflection
	Material white_light(false, 0, WHITE, 0, 20);
	Material red_cornell(false, 1, RED, 0, 0);
	Material green_cornell(false, 1, GREEN, 0, 0);
	Material glass(false, 1, WHITE, 0, 0);
	glass.is_transparent = true;

	// Define scene 0, Mark's Room
	Scene scene0;

	Rectangle roof(Point3D(10, -6, 5), Point3D(10, 6, 5), Point3D(0, -6, 5), Point3D(0, 6, 5), floor_material);
	Rectangle floor(Point3D(0, -6, -5), Point3D(0, 6, -5), Point3D(10, -6, -5), Point3D(10, 6, -5), floor_material);
	Triangle roof_triangle_front(Point3D(10, -6, 5), Point3D(10, 6, 5), Point3D(13, 0, 5), floor_material);
	Triangle roof_triangle_back(Point3D(0, -6, 5), Point3D(-3, 0, 5), Point3D(0, 6, 5), floor_material);
	Triangle floor_triangle_front(Point3D(10, -6, -5), Point3D(13, 0, -5), Point3D(10, 6, -5), floor_material);
	Triangle floor_triangle_back(Point3D(0, -6, -5), Point3D(0, 6, -5), Point3D(-3, 0, -5), floor_material);

	// Front walls
	Rectangle r_wall_f(Point3D(13, 0, -5), Point3D(10, 6, -5), Point3D(13, 0, 5), Point3D(10, 6, 5), white_wall);
	Rectangle l_wall_f(Point3D(10, -6, -5), Point3D(13, 0, -5), Point3D(10, -6, 5), Point3D(13, 0, 5), white_wall);
	// Side walls
	Rectangle r_wall_s(Point3D(10, 6, -5), Point3D(0, 6, -5), Point3D(10, 6, 5), Point3D(0, 6, 5), wall_material);
	Rectangle l_wall_s(Point3D(0, -6, -5), Point3D(10, -6, -5), Point3D(0, -6, 5), Point3D(10, -6, 5), wall_material);
	// Back walls
	Rectangle r_wall_b(Point3D(0, 6, -5), Point3D(-3, 0, -5), Point3D(0, 6, 5), Point3D(-3, 0, 5), white_wall);
	Rectangle l_wall_b(Point3D(-3, 0, -5), Point3D(0, -6, -5), Point3D(-3, 0, 5), Point3D(0, -6, 5), white_wall);

	// Objects apart from the walls
	Rectangle right_mirror(Point3D(12.2, 1.2, -4), Point3D(10.4, 4.8, -4), Point3D(12.2, 1.2, 4), Point3D(10.4, 4.8, 4), mirror);
	Sphere glass_ball1(Point3D(9, -2, -3.4), 1.6, glass);
	Sphere glass_ball2(Point3D(8.5, 4, 1.5), 0.9, glass);
	Sphere glass_ball3(Point3D(6, -2, 1.0), 1.2, glass);

	//Sphere test_ball(Point3D(5, -3, -3.4), 1.6, LAMBERT, GREEN);
	Sphere test_ball2(Point3D(9, -2, -3.4), 1.6, orange_matte);

	// A tetrahedron
	Point3D tr0(5, 1.8, -4);
	Point3D tr1(8, 0.8, -4);
	Point3D tr2(8, 4.8, -4);
	Point3D tr3(8, 1.8, -2);
	Triangle tetra_bottom(tr0, tr1, tr2, red_cornell);
	Triangle tetra_left(tr3, tr1, tr0, red_cornell);
	Triangle tetra_back(tr3, tr2, tr1, red_cornell);
	Triangle tetra_right(tr3, tr0, tr2, red_cornell);

	// Lights
	// old Rectangle ceiling_light(Point3D(8, -1, 4.99), Point3D(10, -1, 4.99), Point3D(8, 1, 4.99), Point3D(10, 1, 4.99), LIGHT, WHITE);
	Rectangle ceiling_light(Point3D(5, 1, 4.99), Point3D(5, -1, 4.99), Point3D(7, 1, 4.99), Point3D(7, 1, 4.99), white_light);
	Rectangle right_wall_light(Point3D(9, 5.99, -1), Point3D(8, 5.99, -1), Point3D(9, 5.99, 0), Point3D(8, 5.99, 0), white_light); // fucks up, unused
	Rectangle ceiling_light2(Point3D(7.5, 2, 4.99), Point3D(7.5, 3, 4.99), Point3D(6.5, 2, 4.99), Point3D(6.5, 2, 4.99), LIGHT, WHITE);
	Rectangle back_light(Point3D(-1.5, -0.5, 3), Point3D(-1.5, 0.5, 3), Point3D(-1.5, -0.5, 4), Point3D(-1.5, 0.5, 4), LIGHT, WHITE);

	// Include every object in this vector to add it to the scene
	std::vector<Shape*> scene0_objects = { &roof, &floor, &roof_triangle_front, &roof_triangle_back, &floor_triangle_front, &floor_triangle_back,
		&l_wall_f, &r_wall_f, &r_wall_s, &l_wall_s, &r_wall_b, &l_wall_b, 
		&tetra_bottom, &tetra_left, &tetra_back, &tetra_right, &right_mirror,
		&ceiling_light, &test_ball2};

	for (int i = 0; i < scene0_objects.size(); i++) {
		//std::cout << "\nAdding obj " << i;
		scene0.add_shape(scene0_objects[i]);
	}

	// Define cameras, take pictures
	Vec3 eye_position(0, 0, -1);

	Camera cam1(eye_position, 800, 800);
	cam1.set_aa(3); //set anti-aliasing
	cam1.set_srays(8); //set shadow rays
	cam1.set_depth(7); // set maximum recursive depth

	clock_t tStart = clock();

	cam1.take_picture(scene0, "aa3_srays8_depth7_shadow");

	printf("\nTime taken: %.2fs\n", ((double)clock() - tStart) / CLOCKS_PER_SEC);
}