#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;



class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void CreateFloor(vec3 scale, int posX, int posZ, int cir);
	void Painting();

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	p2DynArray<PhysBody3D*> pb_cubes;
	p2DynArray<Cube>s_cubes;

	p2DynArray<PhysBody3D*> pb_limits;
	p2DynArray<Cube>s_limits;

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	PhysBody3D* pb_victory;
	Cube sensor_victory;

	Timer reset;
	bool win = true;
	int circuit[70]{
		2,2,2,1,2,2,2,
		2,2,2,1,2,2,2,
		2,2,2,1,2,2,2,
		2,2,1,1,1,2,2,
		2,2,1,2,6,2,2,
		2,2,5,2,5,2,2,
		2,2,1,2,1,2,2,
		2,2,1,2,1,2,2,
		2,2,1,5,1,2,2,
		2,2,2,1,2,2,2,
	};
	Sphere chain[5];
	PhysBody3D* pb_chain[5];
};
