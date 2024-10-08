#include "stdafx.h"
#include "PlayerJumpState.h"
#include "Camera.h"
#include "Audio.h"

void PlayerJumpState::Init()
{
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::JUMP];
	player->inAir = true;
	this->player->deaclageSprite = { 0, 40 };

	if (player->lastState == LIGHTATTACK || player->lastState == DASH)
	{
		player->currentAnime.currentFrame = 14;
		player->timerEasing = 0.f;
		this->player->startPosEasing = this->player->posInAir;
		this->player->endPosEasing = this->player->pos;
		player->endJump = true;
	}
	else
	{
		player->timerEasing = 0.f;
		player->startPosEasing = player->pos;
		if (!player->playerIsShrinks)
		{
			player->endPosEasing = { player->pos.x, player->pos.y - JUMPDISTANCE };
		}
		else
		{
			player->endPosEasing = { player->pos.x, player->pos.y - JUMPDISTANCE*SHRINKS };
		}
		player->endJump = false;
		player->nbAttackInAir = 3;
	}
	player->currentAnime.CreateLeftToRight(player->sprite, 17, 1, player->sprite.getTexture()->getSize());
}

void PlayerJumpState::Move(float _dt)
{
	this->player->deaclageSprite = { 0, 40 };

	if (player->currentAnime.currentFrame >= 4)
	{
		if (sf::Joystick::isConnected(player->id))
		{
			// Joystick Move
			if (player->status == Status::OnFuzzy)
			{
				if (sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_Y) > 15)
				{
					player->pos.x += (player->realSpeed.x + player->GetAgility() * 20) * _dt;
					player->lastDir = RIGHT;
				}
				if (sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_Y) < -15)
				{
					player->pos.x -= (player->realSpeed.x + player->GetAgility() * 20) * _dt;
					player->lastDir = LEFT;
				}
			}
			else
			{
				if (sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_X) > 15)
				{
					player->pos.x += (player->realSpeed.x + player->GetAgility() * 20) * _dt;
					player->lastDir = RIGHT;
				}
				if (sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_X) < -15)
				{
					player->pos.x -= (player->realSpeed.x + player->GetAgility() * 20) * _dt;
					player->lastDir = LEFT;
				}
			}

		}

		//if player move in air set his pos
		player->posInAir.x = player->pos.x;

		//change pos in air 
		if (player->timerEasing < 1.f && player->endJump) //when falling
		{
			player->posInAir = Easings::Lerp({ player->posInAir.x,player->startPosEasing.y }, { player->posInAir.x,player->endPosEasing.y }, Easings::InCubic(player->timerEasing));
			player->timerEasing += _dt * JUMPRATIO * 1.3f;
		}
		else if (player->timerEasing < 1.f && !player->endJump) //when jumping
		{
			player->posInAir = Easings::Lerp({ player->posInAir.x,player->startPosEasing.y }, { player->posInAir.x,player->endPosEasing.y }, Easings::OutQuart(player->timerEasing));
			player->timerEasing += _dt * JUMPRATIO;
		}
		else if (player->timerEasing >= 1.f && player->endJump || player->posInAir.y > player->pos.y) //if timer falling end
		{
			player->posInAir.y = player->pos.y;

			player->inAir = false;

			player->timerEasing = 0.f;

			player->SetPlayerState(HIT_GROUND);
		}
		else //if timer jumping end
		{
			player->timerEasing = 0.f;
			player->startPosEasing = player->posInAir;
			player->endPosEasing = player->pos;
			player->endJump = true;
		}
	}
}

void PlayerJumpState::Update(float _dt, Audio& _audio)
{
	UpdateAnim(_dt);
	Input(_audio);
	Move(_dt);
}

void PlayerJumpState::UpdateAnim(float _dt)
{
	bool temp = true;
	player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 17, 0, temp, false);
}

void PlayerJumpState::Input(Audio& _audio)
{
	if (sf::Joystick::isConnected(player->id))
	{
		if (sf::Joystick::getIdentification(player->id).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::SQUARE) && player->lightAttackButtonRealeased && player->nbAttackInAir != 0)
			{
				player->lightAttackButtonRealeased = false;
				if (player->timerEasing >= 0.5f && !player->endJump)
				{
					player->nbAttackInAir--;
					player->SetPlayerState(LIGHTATTACK);
					_audio.PlaySoundNotRepeated("LIGHT_ATTACK", 100.f);
					_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_LIGHT_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1));
				}
				else if (player->timerEasing <= 0.5f && player->endJump)
				{
					player->nbAttackInAir--;
					player->SetPlayerState(LIGHTATTACK);
					_audio.PlaySoundNotRepeated("LIGHT_ATTACK", 100.f);
					_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_LIGHT_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1));
				}
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::TRIANGLE) && player->heavyAttackButtonRealeased)
			{
				player->heavyAttackButtonRealeased = false;
				if (player->timerEasing >= 0.5f && !player->endJump)
				{
					player->SetPlayerState(HEAVYATTACK);
				}
				else if (player->timerEasing <= 0.5f && player->endJump)
				{
					player->SetPlayerState(HEAVYATTACK);
				}
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::CIRCLE) && player->canDash)
			{
				player->SetPlayerState(DASH);
			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::X) && player->lightAttackButtonRealeased && player->nbAttackInAir != 0)
			{
				player->lightAttackButtonRealeased = false;
				if (player->timerEasing >= 0.5f && !player->endJump)
				{
					player->nbAttackInAir--;
					player->SetPlayerState(LIGHTATTACK);
					_audio.PlaySoundNotRepeated("LIGHT_ATTACK");
					_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_LIGHT_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1));
				}
				else if (player->timerEasing <= 0.5f && player->endJump)
				{
					player->nbAttackInAir--;
					player->SetPlayerState(LIGHTATTACK);
					_audio.PlaySoundNotRepeated("LIGHT_ATTACK");
					_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_LIGHT_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1));
				}
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::Y) && player->heavyAttackButtonRealeased)
			{
				player->heavyAttackButtonRealeased = false;
				if (player->timerEasing >= 0.5f && !player->endJump)
				{
					player->SetPlayerState(HEAVYATTACK);
				}
				else if (player->timerEasing <= 0.5f && player->endJump)
				{
					player->SetPlayerState(HEAVYATTACK);
				}
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::B) && player->canDash)
			{
				player->SetPlayerState(DASH);
			}
		}
	}
}

void PlayerJumpState::PlayerJumpStateRun(float _dt, Audio& _audio)
{
	Update(_dt, _audio);
}

PlayerJumpState::PlayerJumpState(Player* _player)
{
	this->player = _player;
}