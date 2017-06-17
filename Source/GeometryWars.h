#pragma once
#include "Player_Ship.h"
#include "Enemy_Ship.h"
#include "Ship_Laser.h"
#include <Component/SimpleScene.h>
#include <include/glm.h>
using namespace std;
using namespace glm;

class GeometryWars : public SimpleScene
{
public:
	GeometryWars();
	~GeometryWars();

	void Init() override;

public:
	bool key_W, key_A, key_S, key_D, mouseClick;
	float current_time, last_time, spawn_time;
	int game_score;

	Player_Ship* my_ship;
	vector<Ship_Laser*> ship_lasers;
	vector<Enemy_Ship*> enemy_ships;
	bool game_over;
	float color;

private:
	ivec2 resolution;
	bool out_of_bounds(Ship_Laser* laser, vec2 res);
	bool isCollision(GameObject* obj1, Enemy_Ship* obj2);
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
};

