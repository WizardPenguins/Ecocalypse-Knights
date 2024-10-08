#include "stdafx.h"
#include "PlayerHeavyAttackState.h"
#include "Audio.h"

void PlayerHeavyAttackState::Init()
{
	if (player->inAir)
	{
		player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::HEAVY_ATTACK_DOWN];
		player->currentAnime.CreateLeftToRight(player->sprite, 18, 1, player->sprite.getTexture()->getSize());
		player->heavyAttackHitbox.setSize(sf::Vector2f{ 50,160 });
		player->heavyAttackGoUp = false;

		player->timerEasing = 0.f;
		player->startPosEasing = player->posInAir;
		player->endPosEasing = player->pos;
		player->canActiveHitBox = true;
	}
	else
	{
		player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::HEAVY_ATTACK_UP];
		player->currentAnime.CreateLeftToRight(player->sprite, 19, 1, player->sprite.getTexture()->getSize());

		player->heavyAttackGoUp = true;
		player->posInAir = player->pos;

		player->timerEasing = 0.f;
		player->startPosEasing = player->pos;
		if (player->GetIsSrinks())
		{
			player->endPosEasing = { player->pos.x,player->pos.y - (JUMPDISTANCE * SHRINKS) };
		}
		else
		{
			player->endPosEasing = { player->pos.x,player->pos.y - JUMPDISTANCE };
		}
		player->maxUpDistanceHeavyAttack = false;
		player->canActiveHitBox = true;
		player->nbAttackInAir = 3;
	}
	if (player->heavyAttackGoUp)
	{
		(bool)(player->lastDir == LEFT) ? player->deaclageSprite = { -38,135 } : player->deaclageSprite = { 38,135 };
	}
	else
	{
		(bool)(player->lastDir == LEFT) ? player->deaclageSprite = { -40,100 } : player->deaclageSprite = { 40,100 };
	}
	player->heavyAttackHitbox.setFillColor(sf::Color::Transparent);
}

void PlayerHeavyAttackState::Input(Audio& _audio)
{
	if (sf::Joystick::isConnected(player->id))
	{
		if (player->inAir && player->heavyAttackGoUp)
		{
			if (player->currentAnime.currentFrame > 13)
			{
				if (sf::Joystick::getIdentification(player->id).name == "Wireless Controller")
				{
					if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::SQUARE) && player->lightAttackButtonRealeased && player->nbAttackInAir != 0)
					{
						player->lightAttackButtonRealeased = false;
						player->heavyAttackHitboxActived = false;
						player->nbAttackInAir--;
						player->SetPlayerState(LIGHTATTACK);
						_audio.PlaySoundNotRepeated("LIGHT_ATTACK", 100.f);
						_audio.PlaySoundNotRepeated("PLAYER_ATTACK_SCREAM", 100.f);
					}
					if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::TRIANGLE) && player->heavyAttackButtonRealeased)
					{
						player->heavyAttackButtonRealeased = false;
						player->heavyAttackHitboxActived = false;
						player->SetPlayerState(HEAVYATTACK);
						//_audio.PlaySoundNotRepeated("HEAVY_ATTACK", 100.f);
					}
				}
				else
				{
					if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::X) && player->lightAttackButtonRealeased && player->nbAttackInAir != 0)
					{
						player->lightAttackButtonRealeased = false;
						player->heavyAttackHitboxActived = false;
						player->nbAttackInAir--;
						player->SetPlayerState(LIGHTATTACK);
						_audio.PlaySoundNotRepeated("LIGHT_ATTACK", 100.f);
						_audio.PlaySoundNotRepeated("PLAYER_ATTACK_SCREAM", 100.f);
					}
					if (sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonXBOX::Y) && player->heavyAttackButtonRealeased)
					{
						player->heavyAttackButtonRealeased = false;
						player->heavyAttackHitboxActived = false;
						player->SetPlayerState(HEAVYATTACK);
						//_audio.PlaySoundNotRepeated("HEAVY_ATTACK", 100.f);
					}
				}
			}
		}
	}
}

void PlayerHeavyAttackState::Move(float _dt)
{
	if (player->inAir)
	{
		if (!player->heavyAttackGoUp)
		{

			(bool)(player->lastDir == RIGHT) ? player->heavyAttackHitbox.setPosition(sf::Vector2f{ player->posInAir.x + 80, player->posInAir.y - 125 }) : player->heavyAttackHitbox.setPosition(sf::Vector2f{ player->posInAir.x - player->heavyAttackHitbox.getSize().x - 80, player->posInAir.y - 125 });
		}
		else
		{
			(bool)(player->currentAnime.currentFrame < 12) ? player->heavyAttackHitbox.setSize(sf::Vector2f{ 120,170 }) : player->heavyAttackHitbox.setSize(sf::Vector2f{ 120,170 });
			if (player->currentAnime.currentFrame < 12)
			{
				(bool)(player->lastDir == RIGHT) ? player->heavyAttackHitbox.setPosition(sf::Vector2f{ player->posInAir.x, player->posInAir.y - 160 }) : player->heavyAttackHitbox.setPosition(sf::Vector2f{ player->posInAir.x, player->posInAir.y - 160 });
			}
			else
			{
				(bool)(player->lastDir == RIGHT) ? player->heavyAttackHitbox.setPosition(sf::Vector2f{ player->posInAir.x - player->heavyAttackHitbox.getSize().x / 2 + 30, player->posInAir.y - 180 }) : player->heavyAttackHitbox.setPosition(sf::Vector2f{ player->posInAir.x - player->heavyAttackHitbox.getSize().x / 2 - 30, player->posInAir.y - 180 });
			}

			if (player->currentAnime.currentFrame > 6)
			{
				if (sf::Joystick::isConnected(player->id))
				{
					// Joystick Move
					if (sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_X) > 15)
					{
						player->pos.x += (player->realSpeed.x + player->GetAgility() * 20) / 1.4f * _dt;
						player->lastDir = RIGHT;
					}

					if (sf::Joystick::getAxisPosition(player->id, sf::Joystick::Axis::LEFT_JOYSTICK_X) < -15)
					{
						player->pos.x -= (player->realSpeed.x + player->GetAgility() * 20) / 1.4f * _dt;
						player->lastDir = LEFT;
					}

					player->posInAir.x = player->pos.x;
				}
			}
		}
	}
	else
	{
		if (!player->heavyAttackGoUp)
		{
			player->heavyAttackHitbox.setSize({160,120});
			(bool)(player->lastDir == RIGHT) ? player->heavyAttackHitbox.setPosition(sf::Vector2f{ player->pos.x + 10, player->pos.y - 80 }) : player->heavyAttackHitbox.setPosition(sf::Vector2f{ player->pos.x - player->heavyAttackHitbox.getSize().x - 10, player->pos.y - 80 });
		}
		else
		{
			if (player->lastDir == RIGHT)
			{
				player->heavyAttackHitbox.setPosition(sf::Vector2f{ player->pos.x + 50, player->pos.y - 80 });
			}
			else if (player->lastDir == LEFT)
			{
				player->heavyAttackHitbox.setPosition(sf::Vector2f{ player->pos.x - player->heavyAttackHitbox.getSize().x - 50, player->pos.y - 80 });
			}
		}
	}
}

void PlayerHeavyAttackState::Update(float _dt, Audio& _audio)
{
	if (player->heavyAttackGoUp)
	{
		(bool)(player->lastDir == LEFT) ? player->deaclageSprite = { -38,135 } : player->deaclageSprite = { 38,135 };
	}
	else
	{
		(bool)(player->lastDir == LEFT) ? player->deaclageSprite = { -75,136 } : player->deaclageSprite = { 75,136 };
	}
	player->posInAir.x = player->pos.x;

	Input(_audio);
	Move(_dt);
	UpdateAnim(_dt);

	if (player->inAir && !player->heavyAttackGoUp)
	{
		if (player->timerEasing < 1.f)
		{
			player->posInAir = Easings::Lerp(player->startPosEasing, player->endPosEasing, Easings::InCubic(player->timerEasing));
			player->timerEasing += _dt * 2.5f;
		}
		else
		{
			player->heavyAttackHitboxActived = false;
			player->timerEasing = 0.f;
			player->posInAir.y = player->pos.y;
			player->inAir = false;
			_audio.PlaySoundNotRepeated("HEAVY_ATTACK_BAM", 100.f);
		}

		if (player->currentAnime.currentFrame == player->currentAnime.maxFrame)
		{
			player->SetPlayerState(IDLE);
		}
	}
	else if (player->inAir && player->heavyAttackGoUp)
	{
		if (player->timerEasing < 1.f && !player->maxUpDistanceHeavyAttack)
		{
			player->posInAir = Easings::Lerp(player->startPosEasing, player->endPosEasing, Easings::OutQuart(player->timerEasing));
			player->timerEasing += _dt;
		}
		else if (player->timerEasing < 1.f && player->maxUpDistanceHeavyAttack)
		{
			player->posInAir = Easings::Lerp(player->startPosEasing, player->endPosEasing, Easings::InExpo(player->timerEasing));
			player->timerEasing += _dt * 1.4f;
		}
		else if (player->timerEasing >= 1.f && player->maxUpDistanceHeavyAttack)
		{
			player->SetPlayerState(HIT_GROUND);

			player->canActiveHitBox = false;
			player->heavyAttackHitboxActived = false;
			player->timerEasing = 0.f;
			player->posInAir.y = player->pos.y;
			player->inAir = false;
			_audio.PlaySoundNotRepeated("HEAVY_ATTACK_BAM", 100.f);
		}
		else
		{
			player->timerEasing = 0.f;
			player->startPosEasing = player->posInAir;
			player->endPosEasing = player->pos;
			player->maxUpDistanceHeavyAttack = true;
		}
	}
	else
	{
		if (player->currentAnime.currentFrame == player->currentAnime.maxFrame)
		{
			player->SetPlayerState(IDLE);
			player->heavyAttackHitboxActived = false;
			player->timerEasing = 0.f;
			player->posInAir.y = player->pos.y;
			player->inAir = false;
		}
	}
	UpdateHitbox();
}

void PlayerHeavyAttackState::UpdateHitbox()
{
	if (!player->heavyAttackGoUp)
	{
		(bool)player->canActiveHitBox ? player->heavyAttackHitboxActived = true : player->heavyAttackHitboxActived = false;
	}
	else if (player->heavyAttackGoUp)
	{
		if (player->currentAnime.currentFrame < 7)
		{
			player->heavyAttackHitboxActived = false;
		}
		else if ((player->currentAnime.currentFrame > 9 && player->currentAnime.currentFrame < 12))
		{
			player->heavyAttackHitboxActived = false;
			player->canActiveHitBox = true;
		}
		else if (player->currentAnime.currentFrame > 13)
		{
			player->heavyAttackHitboxActived = false;
		}
		else
		{
			(bool)player->canActiveHitBox ? player->heavyAttackHitboxActived = true : player->heavyAttackHitboxActived = false;
		}

		if (player->currentAnime.currentFrame > 6)
		{
			player->inAir = true;
		}
	}
}

void PlayerHeavyAttackState::UpdateAnim(float _dt)
{
	if (player->heavyAttackGoUp)
	{
		if (player->currentAnime.currentFrame < 15)
		{
			bool temp = true;
			player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 14, 0, temp, false);
		}
		else if (player->maxUpDistanceHeavyAttack)
		{
			bool temp = true;
			if (player->timerEasing < 0.8f && player->currentAnime.currentFrame == 18)
			{
				player->currentAnime.currentFrame = 16;
			}
			player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 8, 0, temp, false);
		}
	}
	else if (!player->heavyAttackGoUp)
	{
		if (player->inAir)
		{
			if (player->currentAnime.currentFrame != 4)
			{
				bool temp = true;
				player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 16, 0, temp, false);
			}
		}
		else if (!player->inAir)
		{
			bool temp = true;
			player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 23, 0, temp, false);

			if (player->currentAnime.currentFrame >= 7)
			{
				player->canActiveHitBox = false;
			}
		}
	}
}

void PlayerHeavyAttackState::PlayerHeavyAttackStateRun(float _dt, Audio& _audio)
{
	Update(_dt, _audio);
}

PlayerHeavyAttackState::PlayerHeavyAttackState(Player* _player)
{
	this->player = _player;
}