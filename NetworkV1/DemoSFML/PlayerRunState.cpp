#include "stdafx.h"
#include "PlayerRunState.h"
#include "Camera.h"
#include "Audio.h"
#include "Network.h"

void PlayerRunState::Move(float _dt)
{
	if (sf::Joystick::isConnected(player->id))
	{
		float xPos1 = sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_X);
		float yPos1 = sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_Y);

		// Joystick Move

		if (player->status == Status::OnFuzzy)
		{
			if (xPos1 > 15)
			{
				player->pos.y += (player->realSpeed.y + player->GetAgility()* 20) * _dt;
			}
			if (xPos1 < -15)
			{
				player->pos.y -= (player->realSpeed.y + player->GetAgility() * 20) * _dt;
			}
			if (yPos1 > 15)
			{
				player->pos.x += (player->realSpeed.x + player->GetAgility() * 20) * _dt;
				player->lastDir = RIGHT;
			}
			if (yPos1 < -15)
			{
				player->pos.x -= (player->realSpeed.x + player->GetAgility() * 20) * _dt;
				player->lastDir = LEFT;
			}
		}
		else
		{
			if (xPos1 > 15)
			{
				player->pos.x += (player->realSpeed.x + player->GetAgility() * 20) * _dt;
				player->lastDir = RIGHT;
			}
			if (xPos1 < -15)
			{
				player->pos.x -= (player->realSpeed.x + player->GetAgility() * 20) * _dt;
				player->lastDir = LEFT;
			}
			if (yPos1 > 15)
			{
				player->pos.y += (player->realSpeed.y + player->GetAgility() * 20) * _dt;
			}
			if (yPos1 < -15)
			{
				player->pos.y -= (player->realSpeed.y + player->GetAgility() * 20) * _dt;
			}
		}

		if (xPos1 > -15 && xPos1 < 15 && yPos1 > -15 && yPos1 < 15)
		{
			player->SetPlayerState(IDLE);
		}
	}
}

void PlayerRunState::Init()
{
	this->player->deaclageSprite = { 0 ,0 };
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::RUN];
	player->currentAnime.CreateLeftToRight(player->sprite, 8, 1, player->sprite.getTexture()->getSize());
}

void PlayerRunState::Update(float _dt, Audio& _audio, bool& _inCinematic)
{
	UpdateAnim(_dt);
	if (!_inCinematic)
	{
		Move(_dt);

		this->player->deaclageSprite = { 0 ,0 };
		Input(_audio);
	}
}

void PlayerRunState::UpdateAnim(float _dt)
{
	bool temp = true;
	player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 18, 0, temp, true);
}

void PlayerRunState::Input(Audio& _audio)
{
	if (sf::Joystick::isConnected(player->id))
	{
		if (sf::Joystick::getIdentification(player->id).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::CROSS))
			{
				player->SetPlayerState(JUMP);
				if (!player->inAir)
				{
					player->posInAir.y = player->pos.y;
				}
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::SQUARE) && player->magicAttackButtonReleased && player->auraIsActived)
			{
				player->magicAttackButtonReleased = false;
				player->magicAttackIsDistance = true;
				player->SetPlayerState(MAGICATTACK);
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::SQUARE) && player->lightAttackButtonRealeased && !player->auraIsActived)
			{
				player->lightAttackButtonRealeased = false;
				player->SetPlayerState(LIGHTATTACK);
				_audio.PlaySoundNotRepeated("LIGHT_ATTACK", 100.f);
				if (rand() % 2 >= 1)
				{
					_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_LIGHT_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1), 100.f);
				}
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::TRIANGLE) && player->heavyAttackButtonRealeased && !player->auraIsActived)
			{
				player->heavyAttackButtonRealeased = false;
				player->SetPlayerState(HEAVYATTACK);
				_audio.PlaySoundNotRepeated("HEAVY_ATTACK", 100.f);
				_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_HEAVY_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1), 100.f);
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::TRIANGLE) && player->magicAttackButtonReleased2 && player->auraIsActived && player->AOEMagicUnlock)
			{
				player->magicAttackButtonReleased2 = false;
				player->magicAttackIsDistance = false;
				player->SetPlayerState(MAGICATTACK);
				_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_HEAVY_SPELL_SCREAM_1");

			}

			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::L2) && player->parryButtonReleased && !player->shieldIsBreaking)
			{
				player->parryButtonReleased = false;
				player->SetPlayerState(PARRY);
			}

			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::R1))
			{
				_audio.PlaySoundNotRepeated("ARROW_SHOOT", 100.f);
				player->SetPlayerState(BOWATTACK);
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::CIRCLE) && player->canDash)
			{
				player->SetPlayerState(DASH);
			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::A))
			{
				player->SetPlayerState(JUMP);
				if (!player->inAir)
				{
					player->posInAir.y = player->pos.y;
				}
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::X) && player->magicAttackButtonReleased && player->auraIsActived)
			{
				player->magicAttackButtonReleased = false;
				player->magicAttackIsDistance = true;
				player->SetPlayerState(MAGICATTACK);
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::X) && player->lightAttackButtonRealeased && !player->auraIsActived)
			{
				player->lightAttackButtonRealeased = false;
				player->SetPlayerState(LIGHTATTACK);
				_audio.PlaySoundNotRepeated("LIGHT_ATTACK", 100.f);
				if (rand() % 2 >= 1)
				{
					_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_LIGHT_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1), 100.f);
				}
			} 
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::Y) && player->heavyAttackButtonRealeased && !player->auraIsActived)
			{
				player->heavyAttackButtonRealeased = false;
				player->SetPlayerState(HEAVYATTACK);
				_audio.PlaySoundNotRepeated("HEAVY_ATTACK", 100.f);
				_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_HEAVY_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1), 100.f);
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::Y) && player->magicAttackButtonReleased2 && player->auraIsActived && player->AOEMagicUnlock)
			{
				player->magicAttackButtonReleased2 = false;
				player->magicAttackIsDistance = false;
				player->SetPlayerState(MAGICATTACK);
				_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_HEAVY_SPELL_SCREAM_1");
			}
			else if (sf::Joystick::isAxisPressed(player->id, sf::Joystick::Axis::TRIGGERS_L2_R2, sf::Joystick::PLUS) && player->parryButtonReleased && !player->shieldIsBreaking)
			{
				player->parryButtonReleased = false;
				player->SetPlayerState(PARRY);
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::RB))
			{
				_audio.PlaySoundNotRepeated("ARROW_SHOOT", 100.f);
				player->SetPlayerState(BOWATTACK);
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::B) && player->canDash)
			{
				player->SetPlayerState(DASH);
			}
		}
	}
}

void PlayerRunState::PlayerRunStateRun(float _dt, Audio& _audio, bool& _inCinematic)
{
	Update(_dt, _audio, _inCinematic);
}

PlayerRunState::PlayerRunState(Player* _player)
{
	this->player = _player;
}