#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "ModuleSceneIntro.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{
	if (car != nullptr)
	{
		delete car;
		car = nullptr;
	}	
}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	
	car = new VehicleInfo;
	playerTime.Start();
	// Car properties ----------------------------------------
	car->chassis_size.Set(3, 1.5, 6);
	car->chassis_offset.Set(0, 1.5, 0);
	car->cabin_size.Set(3,0.25,3);
	car->cabin_offset.Set(0, 4, -1.5);
	car->headlight_size.Set(0.5,0.5,0.5);
	car->headlight_offset.Set(1, 1.5, 3);
	car->headlight2_size.Set(0.5, 0.5, 0.5);
	car->headlight2_offset.Set(-1, 1.5, 3);
	car->stickLeftDown_size.Set(0.25,1.7,0.25);
	car->stickLeftDown_offset.Set(1.38,3.09,-2.88);
	car->stickLeftUp_size.Set(0.25, 1.7, 0.25);
	car->stickLeftUp_offset.Set(1.38, 3.09, -0.12);
	car->stickRightDown_size.Set(0.25, 1.7, 0.25);
	car->stickRightDown_offset.Set(-1.38, 3.09, -2.88);
	car->stickRightUp_size.Set(0.25, 1.7, 0.25);
	car->stickRightUp_offset.Set(-1.38, 3.09, -0.12);
	car->mass = 500.0f;
	car->suspensionStiffness = 15.88f;
	car->suspensionCompression = 0.83f;
	car->suspensionDamping = 10.88f;
	car->maxSuspensionTravelCm = 1000.0f;
	car->frictionSlip = 50.5;
	car->maxSuspensionForce = 6000.0f;

	// Wheel properties ---------------------------------------
	float connection_height = 1.2f;
	float wheel_radius = 0.8f;
	float wheel_width = 0.5f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car->chassis_size.x*0.5f;
	float half_length = car->chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car->num_wheels = 4;
	car->wheels = new Wheel[car->num_wheels];

	// FRONT-LEFT ------------------------
	car->wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car->wheels[0].direction = direction;
	car->wheels[0].axis = axis;
	car->wheels[0].suspensionRestLength = suspensionRestLength;
	car->wheels[0].radius = wheel_radius;
	car->wheels[0].width = wheel_width;
	car->wheels[0].front = true;
	car->wheels[0].drive = true;
	car->wheels[0].brake = false;
	car->wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car->wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car->wheels[1].direction = direction;
	car->wheels[1].axis = axis;
	car->wheels[1].suspensionRestLength = suspensionRestLength;
	car->wheels[1].radius = wheel_radius;
	car->wheels[1].width = wheel_width;
	car->wheels[1].front = true;
	car->wheels[1].drive = true;
	car->wheels[1].brake = false;
	car->wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car->wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car->wheels[2].direction = direction;
	car->wheels[2].axis = axis;
	car->wheels[2].suspensionRestLength = suspensionRestLength;
	car->wheels[2].radius = wheel_radius;
	car->wheels[2].width = wheel_width;
	car->wheels[2].front = false;
	car->wheels[2].drive = false;
	car->wheels[2].brake = true;
	car->wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car->wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car->wheels[3].direction = direction;
	car->wheels[3].axis = axis;
	car->wheels[3].suspensionRestLength = suspensionRestLength;
	car->wheels[3].radius = wheel_radius;
	car->wheels[3].width = wheel_width;
	car->wheels[3].front = false;
	car->wheels[3].drive = false;
	car->wheels[3].brake = true;
	car->wheels[3].steering = false;
	
	vehicle = App->physics->AddVehicle(*car);
	Restart(Nmap);
	vehicle->sirens[0].color = Red;
	vehicle->sirens[1].color = Blue;
	vehicle->sirens[2].color = Yellow;
	vehicle->sirens[3].color = Yellow;
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	Control();
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);
	vehicle->Render();
	UI(reset);
	if (emergency)
	{
		countlight++;
		if (countlight <= 10)
		{
			vehicle->sirens[0].color = Blue;
			vehicle->sirens[1].color = Red;
		}
		else if (countlight >= 11 && countlight <= 20)
		{
			vehicle->sirens[0].color = Red;
			vehicle->sirens[1].color = Blue;
		}
		else
		{
			countlight = 0;
		}
	}
	return UPDATE_CONTINUE;
}

float ModulePlayer::ShowTime()
{
	return playerTime.Read() / 1000;
}

void ModulePlayer::Restart(int map)
{
		mat4x4 matrix;
		vehicle->SetTransform(matrix.M);
		Stop();
		for (int i = 0; i < 70; ++i)
		{

			if (App->scene_intro->circuit[i] == 1)
			{
				int j = i / 7 * 30;
				int q = (i % 7 * 30) + map*(8*30);
				vehicle->SetPos(q, 2, j);
				break;
			}
			
			
		}
}

void ModulePlayer::WinAchieved()
{
	//TO DO: put the music audio to the inital value (10) 
	Mix_VolumeMusic(10);
	SetScore();
	Restart(Nmap);	
	playerTime.Start();
	App->scene_intro->lvltime.Start();
	App->player->clue = false;
	App->scene_intro->win = true;
	controls = true;
	reset = 0;
}

void ModulePlayer::UI(int reset)
{
	char title[80];

	//For each level there are different clues that activate when the player stays in the same level for an excessive amount of time
	switch (reset)
	{
	case 0:
		if (clue == true)
		{
			sprintf_s(title, "Clue: I think is pretty obvious what you have to do, maybe you are just bad");
		}
		else
		{
			sprintf_s(title, "LEVEL 0 %.1f Km/h Time: %.0f Best Time: %.0f", vehicle->GetKmh(), ShowTime(), bestTime);
		}
		break;
	case 1:
		if (clue == true)
		{
			sprintf_s(title, "Clue: Try jumping over that green thing");
		}
		else
		{
			sprintf_s(title, "LEVEL 1 %.1f Km/h Time: %.0f Best Time: %.0f", vehicle->GetKmh(), ShowTime(), bestTime);
		}
		
		break;
	case 2:
		if (clue == true)
		{
			sprintf_s(title, "Clue: Always looking the RIGHT side of life!");
		}
		else
		{
			sprintf_s(title, "LEVEL 2 %.1f Km/h Time: %.0f Best Time: %.0f", vehicle->GetKmh(), ShowTime(), bestTime);
		}	
		break;
	case 3:
		if (clue == true)
		{
			sprintf_s(title, "Clue : Have you tried driving in the middle?");
		}
		else
		{
			sprintf_s(title, "LEVEL 3 %.1f Km/h Time: %.0f Best Time: %.0f", vehicle->GetKmh(), ShowTime(), bestTime);
		}
		break;
	case 4:
		if (clue == true)
		{
			sprintf_s(title, "Clue : Try to think OUTSIDE the box");
		}
		else
		{
			sprintf_s(title, "LEVEL 4 %.1f Km/h Time: %.0f Best Time: %.0f", vehicle->GetKmh(), ShowTime(), bestTime);
		}
		break;
	case 5:
		if (clue == true)
		{
			sprintf_s(title, "Clue: Dude, just drive like a normal person until you arrive in the yellow square, its not that hard");
		}
		else
		{
			sprintf_s(title, "LEVEL 5 %.1f Km/h Time: %.0f Best Time: %.0f", vehicle->GetKmh(), ShowTime(), bestTime);
		}
		break;
	case 6:
		if (clue == true)
		{
			sprintf_s(title, "Clue: Dont LOOK BEHIND you, theres a monster");
		}
		else
		{
			sprintf_s(title, "LEVEL 6 %.1f Km/h Time: %.0f Best Time: %.0f", vehicle->GetKmh(), ShowTime(), bestTime);
		}
		break;
	case 10:
		sprintf_s(title, "YOU'VE WON! Your Time: %.0f Best Time: %.0f", ShowTime(), bestTime);
		break;
	default:
		break;
	}
	App->window->SetTitle(title);
}

void ModulePlayer::SetScore()
{
	if (bestTime == 0)
	{
		bestTime = ShowTime();
	}
	if (ShowTime() < bestTime)
	{
		bestTime = ShowTime();
	}
}

void ModulePlayer::Stop()
{
	vehicle->GetBody()->setAngularVelocity({ 0, 0, 0 });
	vehicle->GetBody()->setLinearVelocity({ 0, 0, 0 });
}

void ModulePlayer::Control()
{
	// 130 max vel
	turn = acceleration = brake = 0.0f;

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT && controls)
	{
		acceleration = MAX_ACCELERATION;
		App->scene_intro->count = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT && controls)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT && controls)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT && controls)
	{
		acceleration = -MAX_ACCELERATION;
		vehicle->sirens[2].color = Red;
		vehicle->sirens[3].color = Red;
	}
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_UP)
	{
		vehicle->sirens[2].color = Yellow;
		vehicle->sirens[3].color = Yellow;
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && controls)
	{
		brake = BRAKE_POWER;
	}
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		Restart(Nmap);
	}

	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_REPEAT && controls)
	{
		App->scene_intro->LevelSelector(0);
	}
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT && controls)
	{
		App->scene_intro->LevelSelector(1);
	}
	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT && controls)
	{
		App->scene_intro->LevelSelector(2);
	}
	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT && controls)
	{
		App->scene_intro->LevelSelector(3);
	}
	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT && controls)
	{
		App->scene_intro->LevelSelector(4);
	}
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_REPEAT && controls)
	{
		App->scene_intro->LevelSelector(5);
	}
	if (App->input->GetKey(SDL_SCANCODE_6) == KEY_REPEAT && controls)
	{
		App->scene_intro->LevelSelector(6);
	}
	if (App->input->GetKey(SDL_SCANCODE_7) == KEY_REPEAT && controls)
	{
		App->scene_intro->LevelSelector(7);
	}
	if (App->input->GetKey(SDL_SCANCODE_8) == KEY_REPEAT && controls)
	{
		App->scene_intro->LevelSelector(8);
	}
	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_REPEAT && controls)
	{
		App->scene_intro->LevelSelector(9);
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN && controls)
	{
		emergency = !emergency;
	}
}