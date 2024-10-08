#ifndef PLAYERJUMPSTATE_H
#define PLAYERJUMPSTATE_H
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"
#include "Easings.h"

class PlayerJumpState
{
public:

	//Constructor/Destructor
	PlayerJumpState(Player* _player);
	~PlayerJumpState() = default;

	//Function
	void Move(float _dt);
	void Init();
	void Update(float _dt, Audio& _audio);
	void PlayerJumpStateRun(float _dt, Audio& _audio);
	void UpdateAnim(float _dt);

	void Input(Audio& _audio);

	Player* player;
};

#endif // !PLAYERJUMPSTATE_H



