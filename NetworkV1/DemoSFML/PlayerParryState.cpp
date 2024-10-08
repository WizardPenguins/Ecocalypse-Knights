#include "stdafx.h"
#include "PlayerParryState.h"
#include "Network.h"

void PlayerParryState::Init()
{
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::PARRY];
	player->currentAnime.CreateLeftToRight(player->sprite, 16, 3, player->sprite.getTexture()->getSize());
	this->player->deaclageSprite = { 0 ,0 };
	*player->hitbox = sf::RectangleShape{ sf::Vector2f{ 173.f,160.f } };
	sf::Vector2f nouvelleTaille = { (float)((player->hitbox->getSize().x) / 2.f), (float)((player->hitbox->getSize().y) - (player->hitbox->getSize().y / 8)) };
	player->hitbox->setOrigin(nouvelleTaille);

	player->shieldSprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::SHIELDSPRITE];
	player->shieldAnime.CreateLeftToRight(player->shieldSprite, 10, 1, player->shieldSprite.getTexture()->getSize());
	player->isParrying = true;
}

void PlayerParryState::Move(float _dt)
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
				player->pos.y += (player->parrySpeed.y + player->GetAgility() * 20) * _dt;
			}
			else if (xPos1 < -15)
			{
				player->pos.y -= (player->parrySpeed.y + player->GetAgility() * 20) * _dt;
			}

			if (yPos1 > 15)
			{
				player->pos.x += (player->parrySpeed.x + player->GetAgility() * 20) * _dt;
			}
			else if (yPos1 < -15)
			{
				player->pos.x -= (player->parrySpeed.x + player->GetAgility() * 20) * _dt;
			}
		}
		else
		{
			if (xPos1 > 15)
			{
				player->pos.x += (player->parrySpeed.x + player->GetAgility() * 20) * _dt;
			}
			else if (xPos1 < -15)
			{
				player->pos.x -= (player->parrySpeed.x + player->GetAgility() * 20) * _dt;
			}

			if (yPos1 > 15)
			{
				player->pos.y += (player->parrySpeed.y + player->GetAgility() * 20) * _dt;
			}
			else if (yPos1 < -15)
			{
				player->pos.y -= (player->parrySpeed.y + player->GetAgility() * 20) * _dt;
			}
		}

		if (xPos1 >= -15 && xPos1 <= 15 && yPos1 >= -15 && yPos1 <= 15 && !player->shieldIsBreaking)
		{
			player->parryIdle = true;
			player->parryLeftFrontRun = false;
			player->parryRightFrontRun = false;
		}
		else if (!player->shieldIsBreaking)
		{
			if (xPos1 > 15)
			{
				if (player->lastDir == LEFT)
				{
					player->parryIdle = false;
					player->parryLeftFrontRun = false;
					player->parryRightFrontRun = false;
				}
				else if (player->lastDir == RIGHT)
				{
					player->parryIdle = false;
					player->parryLeftFrontRun = false;
					player->parryRightFrontRun = true;
				}
			}
			else
			{
				if (player->lastDir == LEFT)
				{
					player->parryIdle = false;
					player->parryLeftFrontRun = true;
					player->parryRightFrontRun = false;
				}
				else if (player->lastDir == RIGHT)
				{
					player->parryIdle = false;
					player->parryLeftFrontRun = false;
					player->parryRightFrontRun = false;
				}
			}
		}
		else if (player->shieldIsBreaking)
		{
			player->SetPlayerState(KNOCKDOWN);
		}
	}

	// for rezo
	if (player->parryIdle)
	{
		UpdateAnim(_dt / 1.2f, 0);
	}
	else if (player->lastDir == LEFT && !player->parryIdle && !player->parryLeftFrontRun && !player->parryRightFrontRun)
	{
		UpdateAnim(_dt, 2);
	}
	else if (player->lastDir == RIGHT && !player->parryIdle && !player->parryLeftFrontRun && player->parryRightFrontRun)
	{
		UpdateAnim(_dt, 1);
	}
	else if (player->lastDir == LEFT && !player->parryIdle && player->parryLeftFrontRun && !player->parryRightFrontRun)
	{
		UpdateAnim(_dt, 1);
	}
	else if (player->lastDir == RIGHT && !player->parryIdle && !player->parryLeftFrontRun && !player->parryRightFrontRun)
	{
		UpdateAnim(_dt, 2);
	}
}

void PlayerParryState::Update(float _dt, bool& _multiplayer, Network& _network)
{
	player->shieldAnime.currentFrame = std::abs(player->shieldLife - 9);

	Move(_dt);
	if (!_multiplayer)
	{
		if (sf::Joystick::getIdentification(player->id).name == "Wireless Controller")
		{
			if (!sf::Joystick::isButtonPressed(player->id, (unsigned int)ControllerButtonPS4::L2))
			{
				player->isParrying = false;
				*player->hitbox = sf::RectangleShape{ sf::Vector2f{ 127.f,164.f } };
				sf::Vector2f nouvelleTaille = { (float)((player->hitbox->getSize().x) / 2.f), (float)((player->hitbox->getSize().y) - (player->hitbox->getSize().y / 8)) };
				player->hitbox->setOrigin(nouvelleTaille);
				player->SetPlayerState(IDLE);
			}
		}
		else
		{
			if (!sf::Joystick::isAxisPressed(player->id, sf::Joystick::Axis::TRIGGERS_L2_R2, sf::Joystick::PLUS))
			{
				player->isParrying = false;
				*player->hitbox = sf::RectangleShape{ sf::Vector2f{ 127.f,164.f } };
				sf::Vector2f nouvelleTaille = { (float)((player->hitbox->getSize().x) / 2.f), (float)((player->hitbox->getSize().y) - (player->hitbox->getSize().y / 8)) };
				player->hitbox->setOrigin(nouvelleTaille);
				player->SetPlayerState(IDLE);
			}
		}
	}
	else
	{
		if ((_network.state == NetworkState::CLIENT && player->id == 0) || (_network.state == NetworkState::HOST && player->id == 0))
		{
			if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
			{
				if (!sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::L2))
				{
					player->isParrying = false;
					*player->hitbox = sf::RectangleShape{ sf::Vector2f{ 127.f,164.f } };
					sf::Vector2f nouvelleTaille = { (float)((player->hitbox->getSize().x) / 2.f), (float)((player->hitbox->getSize().y) - (player->hitbox->getSize().y / 8)) };
					player->hitbox->setOrigin(nouvelleTaille);
					player->SetPlayerState(IDLE);
				}
			}
			else if (sf::Joystick::getIdentification(0).name != "Wireless Controller")
			{
				if (!sf::Joystick::isAxisPressed(0, sf::Joystick::Axis::TRIGGERS_L2_R2, sf::Joystick::PLUS))
				{
					player->isParrying = false;
					*player->hitbox = sf::RectangleShape{ sf::Vector2f{ 127.f,164.f } };
					sf::Vector2f nouvelleTaille = { (float)((player->hitbox->getSize().x) / 2.f), (float)((player->hitbox->getSize().y) - (player->hitbox->getSize().y / 8)) };
					player->hitbox->setOrigin(nouvelleTaille);
					player->SetPlayerState(IDLE);
				}
			}
		}
	}
}

void PlayerParryState::UpdateAnim(float _dt, int _lineAnime)
{
	bool temp = true;
	bool temp2 = false;
	player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 16, _lineAnime, temp, true);

	sf::IntRect rect = sf::IntRect(player->currentAnime.currentFrame * player->currentAnime.frameSize.x, _lineAnime * player->currentAnime.frameSize.y + 1, player->currentAnime.frameSize.x, player->currentAnime.frameSize.y);
	player->sprite.setTextureRect(rect);

	player->shieldAnime.UpdateLeftToRight(player->shieldSprite, _dt, 5, 0, temp2, false);
}

void PlayerParryState::PlayerParryStateRun(float _dt, bool& _multiplayer, Network& _network)
{
	Update(_dt, _multiplayer, _network);
}

PlayerParryState::PlayerParryState(Player* _player)
{
	this->player = _player;
}