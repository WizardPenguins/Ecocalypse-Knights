#include "stdafx.h"
#include "PlayerHitGroundState.h"

void PlayerHitGroundState::Init()
{
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { 3 ,47 } : this->player->deaclageSprite = { -30 ,47 };
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::HIT_GROUND];
	player->currentAnime.CreateLeftToRight(player->sprite, 5, 1, player->sprite.getTexture()->getSize());
}

void PlayerHitGroundState::Update(float _dt)
{
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { 30 ,47 } : this->player->deaclageSprite = { -30 ,47 };
	if (player->currentAnime.currentFrame != player->currentAnime.maxFrame)
	{
		UpdateAnim(_dt);
	}
	else
	{
		player->SetPlayerState(IDLE);
	}
}

void PlayerHitGroundState::UpdateAnim(float _dt)
{
	bool temp = true;
	player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 19, 0, temp, false);
}

void PlayerHitGroundState::PlayerHitGroundStateRun(float _dt)
{
	Update(_dt);
}

PlayerHitGroundState::PlayerHitGroundState(Player* _player)
{
	this->player = _player;
}