#ifndef PLAYER_EAT_BY_BOSS_H
#define PLAYER_EAT_BY_BOSS_H
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"

class PlayerEatByBossState
{
public:

	//Constructor/Destructor
	PlayerEatByBossState(Player* _player);
	~PlayerEatByBossState() = default;

	//Function
	void Init();
	void Update(float _dt);
	void PlayerEatByBossStateRun(float _dt);

	Player* player;
};

#endif // !PLAYER_EAT_BY_BOSS_H

