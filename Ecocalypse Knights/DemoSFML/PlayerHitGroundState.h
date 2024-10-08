#ifndef PLAYERHITGROUNDSTATE_H
#define PLAYERHITGROUNDSTATE_H
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"

class PlayerHitGroundState
{
public:

	//Constructor/Destructor
	PlayerHitGroundState(Player* _player);
	~PlayerHitGroundState() = default;

	//Function
	void Init();
	void Update(float _dt);
	void PlayerHitGroundStateRun(float _dt);
	void UpdateAnim(float _dt);

	Player* player;
};

#endif // !PLAYERHITGROUNDSTATE_H

