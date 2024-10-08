#ifndef PLAYERDEADSTATE_H
#define PLAYERDEADSTATE_H
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"
#include "Easings.h"

class PlayerDeadState
{
public:

	//Constructor/Destructor
	PlayerDeadState(Player* _player);
	~PlayerDeadState() = default;

	//Function
	void Init();
	void Update(float _dt);
	void PlayerDeadStateRun(float _dt);
	void UpdateAnim(float _dt);

	Player* player;
};

#endif // !PLAYERDEADSTATE_H

