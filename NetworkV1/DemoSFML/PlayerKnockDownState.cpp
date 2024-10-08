#include "stdafx.h"
#include "PlayerKnockDownState.h"

void PlayerKnockDownState::InitGroundEasingPos(void)
{
	player->startPosEasing = player->pos;
	player->endPosEasing = { player->pos.x, player->pos.y - 100 };
	player->timerEasing = 0.f;
	player->canKnockDownHit = false;
}

void PlayerKnockDownState::Init(void)
{
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::KNOCKDOWN];
	player->currentAnime.CreateLeftToRight(player->sprite, 15, 1, player->sprite.getTexture()->getSize());
	player->lightAttackHitboxActived = false;
	player->heavyAttackHitboxActived = false;
	this->player->deaclageSprite = { 0, 16 };
	player->timerEasing = 0.f;
	player->canKnockDownHit = true;
	player->timerBugResetState = 0.f;
	player->compteurEasing = 0;

	if (player->propultionKnockDown)
	{
		player->haspropulsed = true;

		(bool)(player->inArena) ? player->endPosEasing = { player->pos.x , player->pos.y - JUMPDISTANCE * 1.3f } : player->endPosEasing = { player->pos.x , player->pos.y - JUMPDISTANCE * 1.8f };
		if (player->inAir)
		{
			player->startPosEasing = player->posInAir;
		}
		else
		{
			player->startPosEasing = player->pos;
		}
		player->inAir = true;
	}
	else if (player->inAir && !player->propultionKnockDown)
	{
		player->startPosEasing = player->posInAir;
		player->endPosEasing = player->pos;
		player->crossProductRatio = ((player->pos.y - player->posInAir.y) * 1.f) / JUMPDISTANCE;
	}
	else if (!player->inAir)
	{
		InitGroundEasingPos();
	}

	if (player->pvpHeavyAttackDown)
	{
		player->startPosEasing = player->posInAir;
		player->endPosEasing = player->pos;
	}

	player->endKnockDown = false;
	player->knockDownBounce = 0;
	player->timerStandUp = 0.f;
	player->heavyAttackGoUp = false;
	player->endJump = true;
	player->isParrying = false;

}

void PlayerKnockDownState::InAirKnock(float _dt)
{
	if (player->inAir)
	{
		player->timerWaitComboInAir -= _dt;
		if (player->propultionKnockDown)
		{
			if (player->timerEasing <= 0.7f)
			{
				player->posInAir = Easings::Lerp({ player->pos.x ,player->startPosEasing.y }, { player->pos.x ,player->endPosEasing.y }, Easings::OutQuad(player->timerEasing));
				player->timerEasing += _dt * 3.f;
				if (player->lastDir == LEFT)
				{
					player->pos.x += 70 * _dt;
				}
				else
				{
					player->pos.x -= 70 * _dt;
				}
			}
			else
			{
				player->startPosEasing = player->posInAir;
				player->endPosEasing = player->pos;
				player->timerEasing = 0.f;
				(bool)(player->inArena) ? player->crossProductRatio = ((player->pos.y - player->posInAir.y) * 1.f) / JUMPDISTANCE * 1.3f : player->crossProductRatio = ((player->pos.y - player->posInAir.y) * 1.f) / JUMPDISTANCE * 1.8f;
				player->propultionKnockDown = false;
			}
		}
		else
		{
			if (player->pvpHeavyAttackDown)
			{
				if (player->timerEasing < 1.f)
				{
					player->posInAir = Easings::Lerp({ player->pos.x ,player->startPosEasing.y }, { player->pos.x ,player->endPosEasing.y }, player->timerEasing);
					player->timerEasing += _dt + _dt * player->compteurEasing;
					player->compteurEasing++;
				}
				else
				{
					player->inAir = false;
					player->posInAir = player->pos;
					player->pvpHeavyAttackDown = false;
					InitGroundEasingPos();
				}
			}
			else
			{
				if (player->timerEasing <= 1)
				{
					player->posInAir = Easings::Lerp({ player->pos.x ,player->startPosEasing.y }, { player->pos.x ,player->endPosEasing.y }, Easings::InBack(player->timerEasing));
					player->timerEasing += (_dt / player->crossProductRatio) * 2.5f;
				}
				else
				{
					player->inAir = false;
					player->posInAir = player->pos;
					InitGroundEasingPos();
				}

				if (player->haspropulsed)
				{
					if (player->lastDir == LEFT)
					{
						player->pos.x += 150 * _dt;
					}
					else
					{
						player->pos.x -= 150 * _dt;
					}
				}
				else
				{
					if (player->lastDir == LEFT)
					{
						player->pos.x += 200 * _dt;
					}
					else
					{
						player->pos.x -= 200 * _dt;
					}
				}
			}
		}
	}
}

void PlayerKnockDownState::GroundKnock(float _dt)
{
	//je travaille avec la posInAir pour pas bouger l'ombre
	if (!player->inAir)
	{
		if (player->timerEasing <= 1)
		{
			if (player->knockDownBounce == 0 || player->knockDownBounce == 2)
			{
				player->posInAir = Easings::Lerp({ player->pos.x ,player->startPosEasing.y }, { player->pos.x ,player->endPosEasing.y }, Easings::OutBack(player->timerEasing));
			}
			else
			{
				player->posInAir = Easings::Lerp({ player->pos.x ,player->startPosEasing.y }, { player->pos.x ,player->endPosEasing.y }, player->timerEasing);
			}
			player->timerEasing += _dt * 8.f;



			if (player->lastDir == LEFT)
			{
				player->pos.x += 300 * _dt;
			}
			else
			{
				player->pos.x -= 300 * _dt;
			}
		}
		else
		{
			if (player->knockDownBounce == 0 || player->knockDownBounce == 2)
			{
				player->startPosEasing = { player->pos.x , player->posInAir.y };
				player->endPosEasing = player->pos;
				player->timerEasing = 0.f;
			}

			if (player->knockDownBounce == 1)
			{
				player->endPosEasing = { player->pos.x , player->posInAir.y - 50 };
				player->startPosEasing = player->pos;
				player->timerEasing = 0.f;
			}

			if (player->knockDownBounce >= 3)
			{
				player->timerStandUp += _dt;
				if (player->timerStandUp >= 0.6f)
				{
					player->endKnockDown = true;
				}

				if (player->lastDir == LEFT)
				{
					player->pos.x += (8 / player->timerStandUp) * _dt;
				}
				else
				{
					player->pos.x -= (8 / player->timerStandUp) * _dt;
				}
			}
			player->knockDownBounce++;

		}


		if (player->endKnockDown)
		{
			player->SetPlayerState(IDLE);
			player->haspropulsed = false;
		}
	}
}

void PlayerKnockDownState::Update(float _dt)
{
	this->player->deaclageSprite = { 0, 16 };

	if (Utils::Distance(player->posInAir, player->pos) > 3000)
	{
		player->posInAir.y = player->pos.y;
		player->inAir = false;
		player->SetPlayerState(HIT);
	}

	player->timerBugResetState += _dt;
	if (player->timerBugResetState > 5)
	{
		player->timerBugResetState = 0.f;
		player->SetPlayerState(IDLE);
	}

	UpdateAnim(_dt);
	InAirKnock(_dt);
	GroundKnock(_dt);
}

void PlayerKnockDownState::UpdateAnim(float _dt)
{
	bool temp = true;
	if (player->inAir && player->propultionKnockDown)
	{
		player->currentAnime.currentFrame = 2;
		temp = false;
	}
	else if (player->inAir && !player->propultionKnockDown)
	{
		player->currentAnime.currentFrame = 3;
		temp = false;
	}


	player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 18, 0, temp, false);
}

PlayerKnockDownState::PlayerKnockDownState(Player* _player)
{
	this->player = _player;
}