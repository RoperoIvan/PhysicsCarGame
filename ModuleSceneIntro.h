#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_CIRCUITSIZE 600

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
	int Size(int* vec);
	void LoadCircuit(int* lvlcircuit, int* circuitx, int poscircuit);
	void LevelSelector(int lvlnumber);
public:
	
	p2DynArray<PhysBody3D*> pb_cubes;
	p2DynArray<Cube>s_cubes;

	p2DynArray<PhysBody3D*> pb_limits;
	p2DynArray<Cube>s_limits;

	p2DynArray<PhysBody3D*> pb_endlvl;
	p2DynArray<Cube>s_endlvl;

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


	Timer reset, lvltime;
	bool win = true;
	bool move = true;
	int count2 = 0;
	int count3 = 0;
	int circuit[MAX_CIRCUITSIZE];
	int count = 0;
	int countclue = 0;
	int lvlfx = 0;
	int gamewinfx = 0;
};
