#ifndef PLAYERHITSTATE_H
#define PLAYERHITSTATE_H
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"
#include "Audio.h"

class PlayerHitState
{
public:

	//Constructor/Destructor
	PlayerHitState(Player* _player);
	~PlayerHitState() = default;

	//Function
	void Move();
	void Init();
	void Update(float _dt, Audio& _audio);
	void PlayerHitStateRun(float _dt, Audio& _audio);
	void UpdateAnim();

	void Input();
	Player* player;
};

#endif // !PLAYERHITSTATE_H

