#include "stdafx.h"
#include "PlayerDeadState.h"

void PlayerDeadState::Init()
{
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::DEATH];
	player->currentAnime.CreateLeftToRight(player->sprite, 9, 2, player->sprite.getTexture()->getSize());
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { -125 ,0 } : this->player->deaclageSprite = { 125 ,0 };

	player->timerStatus = TIMERSTATUS;
	player->status = Status::None;
	player->lastStatus = Status::None;
	
	player->compteurEasing = 0;
	player->canBeRevive = false;
	player->timerEasing = 0.f;

	if (player->inAir)
	{
		player->startPosEasing = player->posInAir;
		player->endPosEasing = player->pos;
		player->notMoveDeath = true;
	}
	else
	{
		player->notMoveDeath = false;
		player->startPosEasing = player->pos;
		if (player->lastDir == LEFT)
		{
			player->endPosEasing = { player->pos.x + 150 , player->pos.y };
		}
		else
		{
			player->endPosEasing = { player->pos.x - 150, player->pos.y };
		}
	}
	

	player->lightAttackHitboxActived = false;
	player->heavyAttackHitboxActived = false;
	player->heavyAttackGoUp = true;
	if (player->playerSelected == 0 || player->playerSelected == 3)
	{
		player->hitBoxAOE.isActive = false;
	}
	player->compteurLineDeath = 0;
}

void PlayerDeadState::Update(float _dt)
{
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { -125 ,0 } : this->player->deaclageSprite = { 125 ,0 };

	if (player->inAir)
	{
		if (player->timerEasing < 1)
		{
			player->posInAir = Easings::Lerp(player->startPosEasing, player->endPosEasing, player->timerEasing);
			player->timerEasing += _dt + _dt * player->compteurEasing;
			player->compteurEasing++;
			player->currentAnime.currentFrame = 2;
			sf::IntRect rect = sf::IntRect(player->currentAnime.currentFrame * player->sprite.getTexture()->getSize().x/9, 0 * player->sprite.getTexture()->getSize().y/2 + 1, player->sprite.getTexture()->getSize().x/9, player->sprite.getTexture()->getSize().y/2);
			player->sprite.setTextureRect(rect);
		}
		else
		{
			player->inAir = false;
			player->posInAir.y = player->pos.y;
			player->startPosEasing = player->pos;
			if (!player->notMoveDeath)
			{
				if (player->lastDir == LEFT)
				{
					player->endPosEasing = { player->pos.x + 150 , player->pos.y };
				}
				else
				{
					player->endPosEasing = { player->pos.x - 150, player->pos.y };
				}
			}
		}
	}
	else
	{
		UpdateAnim(_dt);
		if (player->currentAnime.currentFrame != player->currentAnime.maxFrame)
		{
			if (player->timerEasing < 1)
			{
				player->pos = Easings::Lerp(player->startPosEasing, player->endPosEasing, Easings::OutCirc(player->timerEasing));
				player->timerEasing += _dt;
			}
		}
	}
}

void PlayerDeadState::UpdateAnim(float _dt)
{
	bool temp = true;
	if (player->currentAnime.currentFrame == 8)
	{
		player->compteurLineDeath = 1;
		player->currentAnime.currentFrame = 0;
	}

	if (player->compteurLineDeath == 0)
	{
		player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 12, player->compteurLineDeath, temp, false);
	}
	else if (player->currentAnime.currentFrame < 7 && player->compteurLineDeath == 1)
	{
		player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 12, player->compteurLineDeath, temp, false);
	}
	else
	{
		player->canBeRevive = true;
	}
}

void PlayerDeadState::PlayerDeadStateRun(float _dt)
{
	Update(_dt);
}

PlayerDeadState::PlayerDeadState(Player* _player)
{
	this->player = _player;
}