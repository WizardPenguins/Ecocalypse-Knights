#include "stdafx.h"
#include "PlayerPrayState.h"

void PlayerPrayState::Init()
{
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { -15 ,37 } : this->player->deaclageSprite = { 15 ,37 };
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::PRAY];
	player->currentAnime.CreateLeftToRight(player->sprite, 19, 1, player->sprite.getTexture()->getSize());
	player->hasFinishPray = false;
}

void PlayerPrayState::Update(float _dt)
{
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { -15 ,37 } : this->player->deaclageSprite = { 15 ,37 };
	if (player->currentAnime.currentFrame != player->currentAnime.maxFrame)
	{
		UpdateAnim(_dt);
	}
	else
	{
		player->SetPlayerState(IDLE);
		player->hasFinishPray = false;
	}

	if (player->currentAnime.currentFrame == 14)
	{
		player->hasFinishPray = true;
	}
}

void PlayerPrayState::UpdateAnim(float _dt)
{
	bool temp = true;
	player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 7, 0, temp, false);
}

void PlayerPrayState::PlayerPrayStateRun(float _dt)
{
	Update(_dt);
}

PlayerPrayState::PlayerPrayState(Player* _player)
{
	this->player = _player;
}