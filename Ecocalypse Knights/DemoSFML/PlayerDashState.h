#ifndef PLAYERDASH_H
#define PLAYERDASH_H
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"
constexpr float DASH_DISTANCE = 600.f;

class PlayerDashState
{
public:

	//Constructor/Destructor
	PlayerDashState(Player* _player);
	~PlayerDashState() = default;

	//Function
	void Init();
	void Move(float _dt);
	void Update(float _dt);
	void PlayerDashStateRun(float _dt);
	void UpdateAnim(float _dt);

	Player* player;
};

#endif // !PLAYERDASH_H