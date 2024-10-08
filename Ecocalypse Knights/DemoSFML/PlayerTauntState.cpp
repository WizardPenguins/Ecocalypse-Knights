#include "stdafx.h"
#include "PlayerTauntState.h"

void PlayerTauntState::Init()
{
	this->player->deaclageSprite = { 0,0 };
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::TAUNT];
	player->currentAnime.CreateLeftToRight(player->sprite, 5, 1, player->sprite.getTexture()->getSize());
	player->timerTaunt = 0.f;
}

void PlayerTauntState::Update(float _dt)
{
	this->player->deaclageSprite = { 0,0 };
	player->timerTaunt += _dt;
	UpdateAnim(_dt);
	if (player->timerTaunt >= 1.f)
	{
		player->SetPlayerState(IDLE);
	}
}

void PlayerTauntState::UpdateAnim(float _dt)
{
	
	bool temp = true;
	player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 12, 0, temp, true);
}

void PlayerTauntState::PlayerTauntStateRun(float _dt)
{
	Update(_dt);
}

PlayerTauntState::PlayerTauntState(Player* _player)
{
	this->player = _player;
}