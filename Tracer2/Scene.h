#pragma once

#include <vector>
#include "Shape.h"

// A collection of objects to be rendered

class Scene {
public:
	// Default constructor
	Scene() = default;

	Shape* operator[](int index) {
		return object_list[index];
	}

	// Get a light source by index
	Shape* ls(int index) {
		return light_list[index];
	}

	void add_shape(Shape* new_shape) {
		object_list.push_back(new_shape);
		size++;
		if (new_shape->get_material().light()) {
			light_list.push_back(new_shape);
			lights++;
		}
		
	}

	int get_size() const { return size; }

	int get_lights() const { return lights; }

private:
	std::vector<Shape*> object_list;	// Contains both the lights and other objects
	std::vector<Shape*> light_list;		// Contains only lights
	int size = 0;
	int lights = 0;
};