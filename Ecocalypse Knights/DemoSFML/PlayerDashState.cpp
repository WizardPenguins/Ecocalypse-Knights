#include "stdafx.h"
#include "PlayerDashState.h"
#include "Easings.h"

void PlayerDashState::Init()
{
	(bool)(player->lastDir == LEFT) ? player->deaclageSprite = { -38,-60 } : player->deaclageSprite = { 38,-60 };
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::DASH];
	player->currentAnime.CreateLeftToRight(player->sprite, 9, 1, player->sprite.getTexture()->getSize());
	(bool)(player->inAir) ? player->startPosEasing = player->posInAir : player->startPosEasing = player->pos;
	if (player->playerIsShrinks)
	{
		(bool)(player->lastDir == LEFT) ? ((bool)(player->inAir) ? player->endPosEasing = { player->pos.x - (DASH_DISTANCE * SHRINKS),player->posInAir.y } : player->endPosEasing = { player->pos.x - (DASH_DISTANCE * SHRINKS),player->pos.y }) : ((bool)(player->inAir) ? player->endPosEasing = { player->pos.x + (DASH_DISTANCE * SHRINKS),player->posInAir.y } : player->endPosEasing = { player->pos.x + (DASH_DISTANCE * SHRINKS),player->pos.y });
	}
	else
	{
		(bool)(player->lastDir == LEFT) ? ((bool)(player->inAir) ? player->endPosEasing = { player->pos.x - DASH_DISTANCE,player->posInAir.y } : player->endPosEasing = { player->pos.x - DASH_DISTANCE,player->pos.y }) : ((bool)(player->inAir) ? player->endPosEasing = { player->pos.x + DASH_DISTANCE,player->posInAir.y } : player->endPosEasing = { player->pos.x + DASH_DISTANCE,player->pos.y });
	}
	player->timerEasing = 0.f;
	player->timerBetweenDash = TIMERDASH;
	player->canDash = false;
	player->dashNumber++;
}

void PlayerDashState::Move(float _dt)
{

	if (player->currentAnime.currentFrame != player->currentAnime.maxFrame)
	{
		sf::Vector2f temp{ player->pos.x , player->posInAir.y };
		(bool)(player->inAir) ? temp = Easings::Lerp(player->startPosEasing, player->endPosEasing,  Easings::OutExpo(player->timerEasing)) : player->pos = Easings::Lerp(player->startPosEasing, player->endPosEasing, Easings::OutExpo(player->timerEasing));
		if (player->inAir)
		{
			player->pos.x = temp.x;
		}
		player->timerEasing += _dt;
	}
	else
	{
	
		player->timerEasing = 0.f;
		(bool)(player->inAir) ? player->SetPlayerState(JUMP) : player->SetPlayerState(RUN);
	}
}

void PlayerDashState::Update(float _dt)
{
	(bool)(player->lastDir == LEFT) ? player->deaclageSprite = { 240,-60 } : player->deaclageSprite = { -240,-60 };
	Move(_dt);
	UpdateAnim(_dt);
}

void PlayerDashState::UpdateAnim(float _dt)
{
	bool temp = true;
	player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 14, 0, temp, false);
}

void PlayerDashState::PlayerDashStateRun(float _dt)
{
	Update(_dt);
}

PlayerDashState::PlayerDashState(Player* _player)
{
	this->player = _player;
}