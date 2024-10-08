#include "stdafx.h"
#include "PlayerFreeFallState.h"
#include "Audio.h"

constexpr float DECELERATION = 300.f;

void PlayerFreeFallState::Init()
{
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::JUMP]; 
	player->currentAnime.CreateLeftToRight(player->sprite, 17, 1, player->sprite.getTexture()->getSize());
	player->inAir = false;
	this->player->deaclageSprite = { 0, 40 };
	player->freefallAnimeLeftToRight = true;
	player->currentAnime.currentFrame = 10;
}

void PlayerFreeFallState::Input(Audio& _audio)
{
	if (sf::Joystick::isConnected(player->id))
	{
		if (sf::Joystick::getIdentification(player->id).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::SQUARE) && player->lightAttackButtonRealeased)
			{
				player->lightAttackButtonRealeased = false;
				player->SetPlayerState(LIGHTATTACK);
				_audio.PlaySoundNotRepeated("LIGHT_ATTACK");
				if (rand() % 2 >= 1)
				{
					_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_LIGHT_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1));
				}
			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::X) && player->lightAttackButtonRealeased)
			{
				player->lightAttackButtonRealeased = false;
				player->SetPlayerState(LIGHTATTACK);
				_audio.PlaySoundNotRepeated("LIGHT_ATTACK");
				if (rand() % 2 >= 1)
				{
					_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_LIGHT_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1));
				}
			}
		}
	}
}

void PlayerFreeFallState::Move(float _dt)
{
	if (sf::Joystick::isConnected(player->id))
	{
		float xPos1 = sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_X);
		float yPos1 = sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_Y);
		float acceleration = player->realSpeed.x + player->GetAgility() * 20;
		
		if (xPos1 > 15)
		{
			player->pos.x += acceleration * _dt;
			player->lastDir = RIGHT;
			player->freefallGoLeft = true;
			if (yPos1 > -15 && yPos1 < 15)
			{
				player->deceleration.x += 2.5f;
				player->deceleration.y -= 1.5f;
			}
			else
			{
				player->deceleration.x += 1.25f;
				player->deceleration.y += 1.25f;
			}
			
			if (player->deceleration.x > DECELERATION)
			{
				player->deceleration.x = DECELERATION;
			}
			if (player->deceleration.y > DECELERATION)
			{
				player->deceleration.y = DECELERATION;
			}
		}
		else if (xPos1 < -15)
		{
			player->pos.x -= acceleration * _dt;
			player->lastDir = LEFT;
			player->freefallGoLeft = false;
			if (yPos1 > -15 && yPos1 < 15)
			{
				player->deceleration.x += 2.5f;
				player->deceleration.y -= 1.5f;
			}
			else
			{
				player->deceleration.x += 1.25f;
				player->deceleration.y += 1.25f;
			}

			if (player->deceleration.x > DECELERATION)
			{
				player->deceleration.x = DECELERATION;
			}
			if (player->deceleration.y > DECELERATION)
			{
				player->deceleration.y = DECELERATION;
			}
		}
		else
		{
			player->deceleration.x--;
			if (player->deceleration.x < 0)
			{
				player->deceleration.x = 0;
			}

			(bool)(player->freefallGoLeft) ? player->pos.x += player->deceleration.x * _dt : player->pos.x -= player->deceleration.x * _dt;
		}

		
		if (yPos1 > 15)
		{
			player->pos.y += acceleration * _dt;
			player->freefallGoUp = false;
			if (xPos1 > -15 && xPos1 < 15)
			{
				player->deceleration.y += 2.5f;
				player->deceleration.x -= 1.5f;
			}
			else
			{
				player->deceleration.x += 1.25f;
				player->deceleration.y += 1.25f;
			}

			if (player->deceleration.x > DECELERATION)
			{
				player->deceleration.x = DECELERATION;
			}
			if (player->deceleration.y > DECELERATION)
			{
				player->deceleration.y = DECELERATION;
			}
		}
		else if (yPos1 < -15)
		{
			player->pos.y -= acceleration * _dt;
			player->freefallGoUp = true;
			if (xPos1 > -15 && xPos1 < 15)
			{
				player->deceleration.y += 2.5f;
				player->deceleration.x -= 1.5f;
			}
			else
			{
				player->deceleration.x += 1.25f;
				player->deceleration.y += 1.25f;
			}

			if (player->deceleration.x > DECELERATION)
			{
				player->deceleration.x = DECELERATION;
			}
			if (player->deceleration.y > DECELERATION)
			{
				player->deceleration.y = DECELERATION;
			}
		}
		else
		{
			player->deceleration.y--;
			if (player->deceleration.y < 0)
			{
				player->deceleration.y = 0;
			}

			(bool)(player->freefallGoUp) ? player->pos.y -= player->deceleration.y * _dt : player->pos.y += player->deceleration.y * _dt;
		}
	}
}

void PlayerFreeFallState::Update(float _dt, Audio& _audio)
{
	this->player->deaclageSprite = { 0, 40 };

	Move(_dt);
	UpdateAnim(_dt);
	Input(_audio);	
}

void PlayerFreeFallState::UpdateAnim(float _dt)
{
	if (player->freefallAnimeLeftToRight)
	{
		bool temp = true;
		player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 10, 0, temp, false);
	}
	else if (!player->freefallAnimeLeftToRight)
	{
		player->currentAnime.UpdateRightToLeft(player->sprite, _dt, 10, 0, true, false);
	}

	if (player->freefallAnimeLeftToRight)
	{
		if (player->currentAnime.currentFrame == 13)
		{
			player->currentAnime.currentFrame = 4;
			player->freefallAnimeLeftToRight = false;
		}
	}
	else if (!player->freefallAnimeLeftToRight)
	{
		if (player->currentAnime.currentFrame == 7)
		{
			player->currentAnime.currentFrame = 10;
			player->freefallAnimeLeftToRight = true;
		}
	}
}

void PlayerFreeFallState::PlayerFreeFallStateRun(float _dt,Audio& _audio)
{
	Update(_dt, _audio);
}

PlayerFreeFallState::PlayerFreeFallState(Player* _player)
{
	this->player = _player;
}