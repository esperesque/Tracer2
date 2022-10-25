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
	const Color GREEN(0, 1, 0);
	const Color DARK_GREEN(0.1, 0.4, 0.1);
	const Color DARK_GRAY(0.3, 0.3, 0.3);
	const Color MID_GRAY(0.5, 0.5, 0.5);
	const Color LIGHT_GRAY(0.7, 0.7, 0.7);
	const Color WHITE(1, 1, 1);

	Material MIRROR(MaterialType::MIRROR);
	Material LAMBERT(MaterialType::LAMBERT);
	Material LIGHT(MaterialType::LIGHT);
	Material CHECKERS(MaterialType::CHECKERS);
	Material GLASS(MaterialType::GLASS);
	//Material MIRROR(true);
	//Material LAMBERT(false);

	// Define scene 0, Mark's Room
	Scene scene0;

	Rectangle roof(Point3D(10, -6, 5), Point3D(10, 6, 5), Point3D(0, -6, 5), Point3D(0, 6, 5), LAMBERT, LIGHT_GRAY);
	Rectangle floor(Point3D(0, -6, -5), Point3D(0, 6, -5), Point3D(10, -6, -5), Point3D(10, 6, -5), CHECKERS, LIGHT_GRAY);
	Triangle roof_triangle_front(Point3D(10, -6, 5), Point3D(10, 6, 5), Point3D(13, 0, 5), LAMBERT, LIGHT_GRAY);
	Triangle roof_triangle_back(Point3D(0, -6, 5), Point3D(-3, 0, 5), Point3D(0, 6, 5), LAMBERT, LIGHT_GRAY);
	Triangle floor_triangle_front(Point3D(10, -6, -5), Point3D(13, 0, -5), Point3D(10, 6, -5), LAMBERT, LIGHT_GRAY);
	Triangle floor_triangle_back(Point3D(0, -6, -5), Point3D(0, 6, -5), Point3D(-3, 0, -5), LAMBERT, LIGHT_GRAY);

	// Front walls
	Rectangle r_wall_f(Point3D(13, 0, -5), Point3D(10, 6, -5), Point3D(13, 0, 5), Point3D(10, 6, 5), LAMBERT, MID_GRAY);
	Rectangle l_wall_f(Point3D(10, -6, -5), Point3D(13, 0, -5), Point3D(10, -6, 5), Point3D(13, 0, 5), LAMBERT, MID_GRAY);
	// Side walls
	Rectangle r_wall_s(Point3D(10, 6, -5), Point3D(0, 6, -5), Point3D(10, 6, 5), Point3D(0, 6, 5), LAMBERT, MID_GRAY);
	Rectangle l_wall_s(Point3D(0, -6, -5), Point3D(10, -6, -5), Point3D(0, -6, 5), Point3D(10, -6, 5), LAMBERT, DARK_GREEN);
	// Back walls
	Rectangle r_wall_b(Point3D(0, 6, -5), Point3D(-3, 0, -5), Point3D(0, 6, 5), Point3D(-3, 0, 5), LAMBERT, MID_GRAY);
	Rectangle l_wall_b(Point3D(-3, 0, -5), Point3D(0, -6, -5), Point3D(-3, 0, 5), Point3D(0, -6, 5), LAMBERT, MID_GRAY);

	// Objects apart from the walls
	Sphere test_ball(Point3D(8, -2, -3.4), 1.6, LAMBERT, GREEN);
	Rectangle right_mirror(Point3D(12.2, 1.2, -4), Point3D(10.4, 4.8, -4), Point3D(12.2, 1.2, 4), Point3D(10.4, 4.8, 4), MIRROR, WHITE);
	Sphere glass_ball(Point3D(5, -2.5, -3.7), 1.3, GLASS, WHITE);
	Sphere glass_ball2(Point3D(6, 2.8, -3.3), 1.7, GLASS, WHITE);
	Sphere center_ball(Point3D(6, 0, -3.7), 1.3, GLASS, WHITE);

	// Lights
	// old Rectangle ceiling_light(Point3D(8, -1, 4.99), Point3D(10, -1, 4.99), Point3D(8, 1, 4.99), Point3D(10, 1, 4.99), LIGHT, WHITE);
	Rectangle ceiling_light(Point3D(10, -1, 4.99), Point3D(10, 1, 4.99), Point3D(8, -1, 4.99), Point3D(8, 1, 4.99), LIGHT, WHITE);
	Rectangle right_wall_light(Point3D(9, 5.99, -1), Point3D(8, 5.99, -1), Point3D(9, 5.99, 0), Point3D(8, 5.99, 0), LIGHT, WHITE); // fucks up, unused
	Rectangle ceiling_light2(Point3D(7.5, 2, 4.99), Point3D(7.5, 3, 4.99), Point3D(6.5, 2, 4.99), Point3D(6.5, 2, 4.99), LIGHT, WHITE);
	Rectangle back_light(Point3D(-1.5, -0.5, 3), Point3D(-1.5, 0.5, 3), Point3D(-1.5, -0.5, 4), Point3D(-1.5, 0.5, 4), LIGHT, WHITE);

	// Include every object in this vector to add it to the scene
	std::vector<Shape*> scene0_objects = { &roof, &floor, &roof_triangle_front, &roof_triangle_back, &floor_triangle_front, &floor_triangle_back,
		&l_wall_f, &r_wall_f, &r_wall_s, &l_wall_s, &r_wall_b, &l_wall_b, &test_ball, &right_mirror,
		&ceiling_light, &right_wall_light};

	for (int i = 0; i < scene0_objects.size(); i++) {
		//std::cout << "\nAdding obj " << i;
		scene0.add_shape(scene0_objects[i]);
	}

	// Define scene 1, a basic scene with some spheres
	Scene scene1;

	Sphere sphere1(Point3D(4, 0, 0), 0.4, LAMBERT, BLUE);
	Sphere sphere2(Point3D(3, 1, 0), 0.4, LAMBERT, BLUE);
	Rectangle rect1(Point3D(8, -5, -5), Point3D(8, 5, -5), Point3D(8, -5, 5), Point3D(8, 5, 5), LAMBERT, RED); // back wall
	Sphere sphere3(Point3D(5, -1.5, 0), 0.6, LAMBERT, GREEN);

	scene1.add_shape(&sphere1);
	scene1.add_shape(&sphere2);
	scene1.add_shape(&rect1);
	scene1.add_shape(&sphere3);

	// scene0.add_shape(&sphere2);

	// Define scene 2, similar to the example from ray tracing in one weekend
	Rectangle floor_rect(Point3D(0, -10, -5), Point3D(0, 10, -5), Point3D(20, -10, -5), Point3D(20, 10, -5), LAMBERT, BLUE);
	Rectangle mirror(Point3D(20, -16, -3), Point3D(20, 16, -3), Point3D(20, -16, 10), Point3D(20, 16, 10), MIRROR, WHITE);
	Sphere aSphere(Point3D(8, 0, -2), 3, LAMBERT, GREEN);

	//std::cout << "\nMirror material has material: " << MIRROR << "\n";
	//std::cout << "\nMirror object has material: " << mirror.get_material() << "\n";

	std::vector<Shape*> scene2_objects = { &floor_rect, &aSphere, &mirror };
	Scene scene2;
	for (int i = 0; i < scene2_objects.size(); i++) {
		scene2.add_shape(scene2_objects[i]);
	}

	// Define cornell_scene, a cornell box

	Rectangle cornell_floor(Point3D(-5, -5, -5), Point3D(-5, 5, -5), Point3D(10, -5, -5), Point3D(10, 5, -5), LAMBERT, LIGHT_GRAY);
	Rectangle cornell_ceiling(Point3D(-5, 5, 5), Point3D(-5, -5, 5), Point3D(10, 5, 5), Point3D(10, -5, 5), LAMBERT, LIGHT_GRAY);
	Rectangle cornell_backwall(Point3D(10, -5, -5), Point3D(10, 5, -5), Point3D(10, -5, 5), Point3D(10, 5, 5), LAMBERT, LIGHT_GRAY);
	Rectangle cornell_rwall(Point3D(10, 5, -5), Point3D(-5, 5, -5), Point3D(10, 5, 5), Point3D(-5, 5, 5), LAMBERT, RED);
	Rectangle cornell_lwall(Point3D(-5, -5, -5), Point3D(10, -5, -5), Point3D(-5, -5, 5), Point3D(10, -5, 5), LAMBERT, GREEN);
	Rectangle cornell_light(Point3D(6.5, -1, 4.99), Point3D(6.5, 1, 4.99), Point3D(4.5, -1, 4.99), Point3D(4.5, 1, 4.99), LIGHT, WHITE);

	Rectangle c_box_front(Point3D(5, -2, -5), Point3D(6, 0, -5), Point3D(5, -2, -3), Point3D(6, 0, -3), LAMBERT, WHITE);
	Rectangle c_box_top(Point3D(5, -2, -3), Point3D(6, 0, -3), Point3D(7, -3, -3), Point3D(8, -1, -3), LAMBERT, WHITE);
	Rectangle c_box_left(Point3D(7, -3, -5), Point3D(5, -2, -5), Point3D(7, -3, -3), Point3D(5, -2, -3), LAMBERT, WHITE);

	Sphere c_sphere(Point3D(4.8, 2.3, -4), 1.0, LAMBERT, WHITE);

	std::vector<Shape*> cornell_scene_objects = { &cornell_floor, &cornell_ceiling, &cornell_backwall, &cornell_rwall, &cornell_lwall,
		&c_box_front, &c_box_top, &c_box_left, &c_sphere,
		&cornell_light };
	Scene cornell_scene;
	for (int i = 0; i < cornell_scene_objects.size(); i++) {
		cornell_scene.add_shape(cornell_scene_objects[i]);
	}

	// Define cameras, take pictures
	Vec3 eye_position(0, 0, -1);

	Camera cam1(eye_position, 800, 600);
	cam1.set_aa(1);
	cam1.set_srays(3);
	cam1.set_depth(3); // set maximum recursive depth

	clock_t tStart = clock();

	cam1.take_picture(scene0, "material_test");
	//cam1.render_normals(cornell_scene, "cornell_normals");

	printf("\nTime taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	//cam1.render_normals(scene0, "scene0_normals_v2");
	//cam1.render_normals(scene2, "scene2_normals_v2");
	//cam1.take_picture(scene2, "result1");

	//Camera cam2(eye_position, 800, 600);
	//cam2.set_aa(1);
	//cam2.take_picture(scene2, "result2");

	//auto secondCamera = Camera(eye_position, 1600, 900);
	//secondCamera.take_picture(scene0, "result2");
}