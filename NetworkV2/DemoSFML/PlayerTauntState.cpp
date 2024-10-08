#include "stdafx.h"
#include "PlayerTauntState.h"

void PlayerTauntState::Init()
{
	/*this->player->deaclageSprite = { 0,0 };
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::HIT];*/
}

void PlayerTauntState::Update(float _dt)
{

}

void PlayerTauntState::UpdateAnim(float _dt)
{

}

void PlayerTauntState::PlayerTauntStateRun(float _dt)
{
	Update(_dt);
}

PlayerTauntState::PlayerTauntState(Player* _player)
{
	this->player = _player;
}