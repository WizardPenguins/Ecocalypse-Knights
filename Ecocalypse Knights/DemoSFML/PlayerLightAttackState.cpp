#include "stdafx.h"
#include "PlayerLightAttackState.h"
#include "Audio.h"

void PlayerLightAttackState::Init()
{
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { -30 ,16 } : this->player->deaclageSprite = { 30 ,16 };
	player->comboNumber++;
	if (player->comboNumber > 3)
	{
		player->comboNumber = 1;
	}
	if (player->comboNumber == 1)
	{
		player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::ATTACK1];

		player->currentAnime.CreateLeftToRight(player->sprite, 5, 1, player->sprite.getTexture()->getSize());
	}
	else if (player->comboNumber == 2)
	{
		player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::ATTACK2];
		player->currentAnime.CreateLeftToRight(player->sprite, 4, 1, player->sprite.getTexture()->getSize());
	}
	else if (player->comboNumber == 3)
	{
		player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::ATTACK3];
		player->currentAnime.CreateLeftToRight(player->sprite, 8, 1, player->sprite.getTexture()->getSize());
	}

	switch (player->comboNumber)
	{
	case 1:
		player->lightAttackHitbox.setSize(sf::Vector2f{ 120,50 });
		break;
	case 2:
		player->lightAttackHitbox.setSize(sf::Vector2f{ 110,160 });
		break;
	case 3:
		player->lightAttackHitbox.setSize(sf::Vector2f{ 160,160 });
		break;
	}

	player->lightAttackHitbox.setFillColor(sf::Color::Transparent);
	player->lightAttackHitbox.setOutlineThickness(-1.f);
	player->canActiveHitBox = true;
	player->timerCombo = TIMER_COMBO;
}

void PlayerLightAttackState::Move(float _dt)
{
	if (player->inAir)
	{

		switch (player->comboNumber)
		{
		case 1:
			if (player->lastDir == RIGHT)
			{
				player->lightAttackHitbox.setPosition(sf::Vector2f{ player->posInAir.x + 20, player->posInAir.y - 50 });
			}
			else if (player->lastDir == LEFT)
			{
				player->lightAttackHitbox.setPosition(sf::Vector2f{ player->posInAir.x - 20 - player->lightAttackHitbox.getSize().x, player->posInAir.y - 50 });
			}
			break;
		case 2:
			if (player->lastDir == RIGHT)
			{
				player->lightAttackHitbox.setPosition(sf::Vector2f{ player->posInAir.x + 20, player->posInAir.y - 155 });
			}
			else if (player->lastDir == LEFT)
			{
				player->lightAttackHitbox.setPosition(sf::Vector2f{ player->posInAir.x - 20 - player->lightAttackHitbox.getSize().x, player->posInAir.y - 155 });
			}
			break;
		case 3:
			if (player->lastDir == RIGHT)
			{
				player->lightAttackHitbox.setPosition(sf::Vector2f{ player->posInAir.x + 20, player->posInAir.y - 155 });
			}
			else if (player->lastDir == LEFT)
			{
				player->lightAttackHitbox.setPosition(sf::Vector2f{ player->posInAir.x - 20 - player->lightAttackHitbox.getSize().x, player->posInAir.y - 155 });
			}
			break;
		}


		float xPos1 = sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_X);
		float yPos1 = sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_Y);
		// Joystick Move
		if (xPos1 > 15)
		{
			player->pos.x += player->realSpeed.x / 1.5f * _dt;
			player->lastDir = RIGHT;
		}
		if (xPos1 < -15)
		{
			player->pos.x -= player->realSpeed.x / 1.5f * _dt;
			player->lastDir = LEFT;
		}
		player->posInAir.x = player->pos.x;
	}
	else
	{
		switch (player->comboNumber)
		{
		case 1:
			if (player->lastDir == RIGHT)
			{
				if (player->currentAnime.currentFrame < 2)
				{
					player->pos.x += player->realSpeed.x / 2 * _dt;
				}
				player->lightAttackHitbox.setPosition(sf::Vector2f{ player->pos.x + 20, player->pos.y - 50 });
			}
			else if (player->lastDir == LEFT)
			{
				if (player->currentAnime.currentFrame < 2)
				{
					player->pos.x -= player->realSpeed.x / 2 * _dt;
				}
				player->lightAttackHitbox.setPosition(sf::Vector2f{ player->pos.x - 20 - player->lightAttackHitbox.getSize().x, player->pos.y - 50 });
			}
			break;
		case 2:
			if (player->lastDir == RIGHT)
			{
				if (player->currentAnime.currentFrame <= 2)
				{
					player->pos.x += player->realSpeed.x / 3 * _dt;
				}
				player->lightAttackHitbox.setPosition(sf::Vector2f{ player->pos.x + 20, player->pos.y - 155 });
			}
			else if (player->lastDir == LEFT)
			{
				if (player->currentAnime.currentFrame <= 2)
				{
					player->pos.x -= player->realSpeed.x / 2 * _dt;
				}
				player->lightAttackHitbox.setPosition(sf::Vector2f{ player->pos.x - 20 - player->lightAttackHitbox.getSize().x, player->pos.y - 155 });
			}
			break;
		case 3:
			if (player->lastDir == RIGHT)
			{
				if (player->currentAnime.currentFrame <= 2)
				{
					player->pos.x += player->realSpeed.x / 4 * _dt;
				}
				player->lightAttackHitbox.setPosition(sf::Vector2f{ player->pos.x + 20, player->pos.y - 155 });
			}
			else if (player->lastDir == LEFT)
			{
				if (player->currentAnime.currentFrame <= 2)
				{
					player->pos.x -= player->realSpeed.x / 4 * _dt;
				}
				player->lightAttackHitbox.setPosition(sf::Vector2f{ player->pos.x - 20 - player->lightAttackHitbox.getSize().x, player->pos.y - 155 });
			}
			break;
		}
	}
}


void PlayerLightAttackState::Update(float _dt)
{
	Move(_dt);
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { -30 ,16 } : this->player->deaclageSprite = { 30 ,16 };
	UpdateAnim(_dt);
	if (player->currentAnime.currentFrame == player->currentAnime.maxFrame)
	{
		player->lightAttackHitboxActived = false;
		player->asPlayedSound = false;
		if (player->inAir)
		{
			if (player->freefallWave)
			{
				player->SetPlayerState(FREEFALL);
			}
			else
			{
				player->SetPlayerState(JUMP);
			}
			
		}
		else
		{
			if (player->freefallWave)
			{
				player->SetPlayerState(FREEFALL);
			}
			else
			{
				player->SetPlayerState(IDLE);
			}
		}
	}
	UpdateHitBox();
}

void PlayerLightAttackState::UpdateAnim(float _dt)
{
	bool temp = true;
	switch (player->comboNumber)
	{
	case 1:
		player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 15, 0, temp, false);
		break;
	case 2:
		player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 13, 0, temp, false);
		break;
	case 3:
		player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 20, 0, temp, false);
		break;
	}
}

void PlayerLightAttackState::UpdateHitBox()
{
	if (player->currentAnime.currentFrame < 2 && (player->comboNumber == 1 || player->comboNumber == 2))
	{
		player->lightAttackHitboxActived = false;
	}
	else if (player->currentAnime.currentFrame < 5 && player->comboNumber == 3)
	{
		player->lightAttackHitboxActived = false;
	}
	else
	{
		if (player->canActiveHitBox)
		{
			player->lightAttackHitboxActived = true;
		}
		else
		{
			player->lightAttackHitboxActived = false;
		}
	}

	if (player->lightAttackHitboxActived)
	{
		player->lightAttackHitbox.setOutlineColor(sf::Color::Magenta);
	}
	else
	{
		player->lightAttackHitbox.setOutlineColor(sf::Color::White);
	}
}

void PlayerLightAttackState::PlayerLightAttackStateRun(float _dt)
{
	Update(_dt);
}

PlayerLightAttackState::PlayerLightAttackState(Player* _player)
{
	this->player = _player;
}