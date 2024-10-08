#include "stdafx.h"
#include "PlayerReviveState.h"

void PlayerReviveState::Init()
{
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::REVIVE];
	player->currentAnime.CreateLeftToRight(player->sprite, 9, 2, player->sprite.getTexture()->getSize());
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { -125 ,0 } : this->player->deaclageSprite = { 125 ,0 };

	player->timerEasing = 0.f;
	player->startPosEasing = player->pos;
	if (player->lastDir == LEFT)
	{
		player->endPosEasing = { player->pos.x - 150 , player->pos.y };
	}
	else
	{
		player->endPosEasing = { player->pos.x + 150, player->pos.y };
	}
	player->compteurLineRevive = 0;
}

void PlayerReviveState::Update(float _dt)
{
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { -125 ,0 } : this->player->deaclageSprite = { 125 ,0 };
	UpdateAnim(_dt);
	if (player->currentAnime.currentFrame != player->currentAnime.maxFrame)
	{
		if (player->timerEasing < 1)
		{
			player->pos = Easings::Lerp(player->startPosEasing, player->endPosEasing, Easings::InCirc(player->timerEasing));
			player->timerEasing += _dt;
		}
	}
}

void PlayerReviveState::UpdateAnim(float _dt)
{
	bool temp = true;
	if (player->currentAnime.currentFrame == 8)
	{
		player->compteurLineRevive = 1;
		player->currentAnime.currentFrame = 0;
	}

	if (player->compteurLineRevive == 0)
	{
		player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 12, player->compteurLineRevive, temp, false);
	}
	else if (player->currentAnime.currentFrame < 7 && player->compteurLineRevive == 1)
	{
		player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 12, player->compteurLineRevive, temp, false);
	}
	else
	{
		player->SetLife((int)(player->GetMaxLife() / 2));
		player->SetPlayerState(IDLE);
	}
}

void PlayerReviveState::PlayerReviveStateRun(float _dt)
{
	Update(_dt);
}

PlayerReviveState::PlayerReviveState(Player* _player)
{
	this->player = _player;
}