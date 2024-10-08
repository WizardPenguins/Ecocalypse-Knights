#include "stdafx.h"
#include "PlayerEatByBossState.h"

void PlayerEatByBossState::Init()
{
	/*this->player->deaclageSprite = { 0,0 };
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::HIT];*/
}

void PlayerEatByBossState::Update(float _dt)
{
	player->timerDamageEatByBoss += _dt;
	if (player->timerDamageEatByBoss >= 1.5f)
	{
		player->AddLife(2);
		player->timerDamageEatByBoss = 0.f;
	}
}

void PlayerEatByBossState::PlayerEatByBossStateRun(float _dt)
{
	Update(_dt);
}

PlayerEatByBossState::PlayerEatByBossState(Player* _player)
{
	this->player = _player;
}
