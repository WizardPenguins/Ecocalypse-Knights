#include "stdafx.h"
#include "PlayerIdleState.h"
#include "Audio.h"
#include "CharacterSelection.h"

void PlayerIdleState::Init()
{
	this->player->deaclageSprite = { 0,0 };
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::IDLE];
	player->currentAnime.CreateLeftToRight(player->sprite, 8, 1, player->sprite.getTexture()->getSize());
	player->freefallWave = false;
}

void PlayerIdleState::Update(float _dt, Audio& _audio)
{
	UpdateAnim(_dt);
	this->player->deaclageSprite = { 0,0 };
	Input(_audio);

	float xPos1 = sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_X);
	float yPos1 = sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_Y);

	// Joystick Move
	if (xPos1 > 15 || xPos1 < -15 || yPos1 > 15 || yPos1 < -15)
	{
		if (!player->playerDontMove)
		{
			player->SetPlayerState(RUN);
		}
	}
}

void PlayerIdleState::UpdateAnim(float _dt)
{
	bool temp = true;
	player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 10, 0, temp, true);
}

void PlayerIdleState::Input(Audio& _audio)
{
	if (sf::Joystick::isConnected(player->id))
	{
		if (sf::Joystick::getIdentification(player->id).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::CROSS) && player->JumpButtonReleased)
			{
				player->JumpButtonReleased = false;
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
				_audio.PlaySoundNotRepeated("LIGHT_ATTACK");
				if (rand() % 2 >= 1)
				{
					_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_LIGHT_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1));
				}
			}

			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::TRIANGLE) && player->heavyAttackButtonRealeased && !player->auraIsActived)
			{
				player->heavyAttackButtonRealeased = false;
				player->SetPlayerState(HEAVYATTACK);
				_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_HEAVY_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1));

			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::TRIANGLE) && player->magicAttackButtonReleased2 && player->auraIsActived && player->AOEMagicUnlock)
			{
				player->magicAttackButtonReleased2 = false;
				player->magicAttackIsDistance = false;
				player->SetPlayerState(MAGICATTACK);
				if (player->mana > 0)
				{
					_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_HEAVY_SPELL_SCREAM_1");
				}
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
				_audio.PlaySoundNotRepeated("PLAYER_DASH");
				player->SetPlayerState(DASH);
			}
			else if (sf::Joystick::isAxisPressed(player->id, sf::Joystick::Axis::DIR_CROSS_X, sf::Joystick::PLUS) || sf::Joystick::isAxisPressed(player->id, sf::Joystick::Axis::DIR_CROSS_X, sf::Joystick::MINUS))
			{
				player->SetPlayerState(TAUNT);
				_audio.PlaySoundNotRepeated("PLAYER_TAUNT");
			}
			else if (sf::Joystick::isAxisPressed(player->id, sf::Joystick::Axis::DIR_CROSS_Y, sf::Joystick::PLUS) || sf::Joystick::isAxisPressed(player->id, sf::Joystick::Axis::DIR_CROSS_Y, sf::Joystick::MINUS))
			{
				player->SetPlayerState(TAUNT);
				_audio.PlaySoundNotRepeated("PLAYER_TAUNT");
			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::A) && player->JumpButtonReleased)
			{
				player->JumpButtonReleased = false;
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
				_audio.PlaySoundNotRepeated("LIGHT_ATTACK");
				if (rand() % 2 >= 1)
				{
					_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_LIGHT_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1), 100.f);
				}
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::Y) && player->heavyAttackButtonRealeased && !player->auraIsActived)
			{
				player->heavyAttackButtonRealeased = false;
				player->SetPlayerState(HEAVYATTACK);
				_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_HEAVY_ATTACK_SCREAM_" + std::to_string(rand() % 3 + 1), 100.f);
			}
			else if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::Y) && player->magicAttackButtonReleased2 && player->auraIsActived && player->AOEMagicUnlock)
			{
				player->magicAttackButtonReleased2 = false;
				player->magicAttackIsDistance = false;
				player->SetPlayerState(MAGICATTACK);
				if (player->mana > 0)
				{
					_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_HEAVY_SPELL_SCREAM_1", 100.f);
				}
			}

			else if (sf::Joystick::isAxisPressed(player->id, sf::Joystick::TRIGGERS_L2_R2, sf::Joystick::PLUS) && player->parryButtonReleased && !player->shieldIsBreaking)
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
				_audio.PlaySoundNotRepeated("PLAYER_DASH");
				player->SetPlayerState(DASH);
			}
			else if (sf::Joystick::isAxisPressed(player->id, sf::Joystick::Axis::DIR_CROSS_X, sf::Joystick::PLUS) || sf::Joystick::isAxisPressed(player->id, sf::Joystick::Axis::DIR_CROSS_X, sf::Joystick::MINUS))
			{
				player->SetPlayerState(TAUNT);
				_audio.PlaySoundNotRepeated("PLAYER_TAUNT");
			}
			else if (sf::Joystick::isAxisPressed(player->id, sf::Joystick::Axis::DIR_CROSS_Y, sf::Joystick::PLUS) || sf::Joystick::isAxisPressed(player->id, sf::Joystick::Axis::DIR_CROSS_Y, sf::Joystick::MINUS))
			{
				player->SetPlayerState(TAUNT);
				_audio.PlaySoundNotRepeated("PLAYER_TAUNT");
			}
		}
	}
}

void PlayerIdleState::PlayerIdleStateRun(float _dt, Audio& _audio)
{
	Update(_dt, _audio);
}

PlayerIdleState::PlayerIdleState(Player* _player)
{
	this->player = _player;
}