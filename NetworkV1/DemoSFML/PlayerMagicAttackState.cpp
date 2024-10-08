#include "stdafx.h"
#include "PlayerMagicAttackState.h"
#include "Audio.h"

void PlayerMagicAttackState::Init()
{
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { 60 ,0 } : this->player->deaclageSprite = { -60 ,0 };
	player->hasCastMagick = false;

	if (player->magicAttackIsDistance)
	{
		player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::MAGIC];
		player->currentAnime.CreateLeftToRight(player->sprite, 13, 1, player->sprite.getTexture()->getSize());
	}
	else
	{
		player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::MAGIC];
		player->currentAnime.CreateLeftToRight(player->sprite, 13, 1, player->sprite.getTexture()->getSize());
		sf::IntRect rect = sf::IntRect(0, 0, player->sprite.getTexture()->getSize().x / 13, player->sprite.getTexture()->getSize().y);
		player->sprite.setTextureRect(rect);

		player->compteurLongMagickManaConsume = 0;

		if ((player->playerSelected == 1 && !player->viperSpellIsActive) || (player->playerSelected == 2 && !player->iceSpellIsActive) || (player->playerSelected == 0) || (player->playerSelected == 3))
		{
			player->spriteAOE = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::MAGIC_AOE];
			player->animeAOE.CreateLeftToRight(player->spriteAOE, 6, 8, player->spriteAOE.getTexture()->getSize());
			sf::IntRect rect2 = sf::IntRect(0, 0, player->spriteAOE.getTexture()->getSize().x / 6, player->spriteAOE.getTexture()->getSize().y / 8);
			player->spriteAOE.setTextureRect(rect2);
			player->compteurLineAOEMagic = 0;

			player->hitBoxAOE.hitbox = new sf::RectangleShape;
			player->hitBoxAOE.hitbox->setFillColor(sf::Color::Transparent);
			player->hitBoxAOE.hitbox->setOutlineThickness(-1.f);
			player->hitBoxAOE.typeAOE = (TypeAOE)player->playerSelected;
			player->hitBoxAOE.isActive = false;
			player->hitBoxAOE.dir = player->lastDir;
		}

		if (player->playerSelected == 1 && !player->viperSpellIsActive)
		{
			player->viperSpellIsActive = false;

			if (player->lastDir == LEFT)
			{
				player->hitBoxAOE.hitbox->setPosition({ player->pos.x - 120 - player->spriteAOE.getTexture()->getSize().x / 6, player->pos.y - 100 });
			}
			else
			{
				player->hitBoxAOE.hitbox->setPosition({ player->pos.x + 120, player->pos.y - 100 });
			}
		}

		if (player->playerSelected == 2 && !player->iceSpellIsActive)
		{
			player->iceSpellIsActive = false;
			if (player->lastDir == LEFT)
			{
				player->hitBoxAOE.hitbox->setPosition({ player->pos.x - 60 - player->spriteAOE.getTexture()->getSize().x / 6, player->pos.y - 160 });
			}
			else
			{
				player->hitBoxAOE.hitbox->setPosition({ player->pos.x + 60, player->pos.y - 160 });
			}

		}
	}
}

void PlayerMagicAttackState::AddDistanceMagic(std::vector<Projectile>* _projectile, Audio& _audio)
{
	if (player->currentAnime.currentFrame == 7 && !player->hasCastMagick)
	{
		player->hasCastMagick = true;
		if (player->mana >= 1)
		{
			_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_LIGHT_SPELL_SCREAM_1");

			int dir;
			if (player->lastDir == RIGHT)
			{
				dir = -1;
			}
			else
			{
				dir = 1;
			}

			Projectile temp = { dir, {player->pos.x, player->pos.y - 80}, true };
			temp.sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::MAGIC_DISTANCE];
			temp.projectileAnime = new Animation();
			temp.playerId = player->id;
			temp.autor = AutorProjectile::PLAYER;
			temp.posAutor = { player->pos.x,player->pos.y };
			temp.damage = player->stats[(int)Stats::WISDOM] * 10; // + state magie
			sf::IntRect rect;
			sf::Vector2f newOrigin;
			switch (player->playerSelected)
			{
			case 0:
				temp.nbFrameX = 46;
				temp.projectileAnime->CreateLeftToRight(temp.sprite, temp.nbFrameX, 1, temp.sprite.getTexture()->getSize());
				rect = sf::IntRect(0, 0, temp.sprite.getTexture()->getSize().x / temp.nbFrameX, temp.sprite.getTexture()->getSize().y);
				temp.sprite.setTextureRect(rect);
				temp.speedAnime = 20;
				temp.projetileType = TypeProjectile::DISTANCE_MAGIC_FIRE;
				temp.hitBox = new sf::RectangleShape(sf::Vector2f{ (float)(temp.sprite.getTexture()->getSize().x / temp.nbFrameX),(float)(temp.sprite.getTexture()->getSize().y) });
				break;
			case 1:
				temp.nbFrameX = 46;
				temp.projectileAnime->CreateLeftToRight(temp.sprite, temp.nbFrameX, 1, temp.sprite.getTexture()->getSize());
				rect = sf::IntRect(0, 0, temp.sprite.getTexture()->getSize().x / temp.nbFrameX, temp.sprite.getTexture()->getSize().y);
				temp.sprite.setTextureRect(rect);
				temp.speedAnime = 20;
				temp.projetileType = TypeProjectile::DISTANCE_MAGIC_POISON;
				temp.hitBox = new sf::RectangleShape(sf::Vector2f{ (float)(temp.sprite.getTexture()->getSize().x / temp.nbFrameX),(float)(temp.sprite.getTexture()->getSize().y) });
				break;
			case 2:
				temp.nbFrameX = 46;
				temp.projectileAnime->CreateLeftToRight(temp.sprite, temp.nbFrameX, 1, temp.sprite.getTexture()->getSize());
				rect = sf::IntRect(0, 0, temp.sprite.getTexture()->getSize().x / temp.nbFrameX, temp.sprite.getTexture()->getSize().y);
				temp.sprite.setTextureRect(rect);
				temp.speedAnime = 20;
				temp.projetileType = TypeProjectile::DISTANCE_MAGIC_ICE;
				temp.hitBox = new sf::RectangleShape(sf::Vector2f{ (float)(temp.sprite.getTexture()->getSize().x / temp.nbFrameX),(float)(temp.sprite.getTexture()->getSize().y) });
				break;
			case 3:
				temp.nbFrameX = 46;
				temp.projectileAnime->CreateLeftToRight(temp.sprite, temp.nbFrameX, 1, temp.sprite.getTexture()->getSize());
				rect = sf::IntRect(0, 0, temp.sprite.getTexture()->getSize().x / temp.nbFrameX, temp.sprite.getTexture()->getSize().y);
				temp.sprite.setTextureRect(rect);
				temp.speedAnime = 20;
				temp.projetileType = TypeProjectile::DISTANCE_MAGIC_ELECTRICAL;
				temp.hitBox = new sf::RectangleShape(sf::Vector2f{ (float)(temp.sprite.getTexture()->getSize().x / temp.nbFrameX),(float)(temp.sprite.getTexture()->getSize().y) });
				break;
			}

			if (dir == 1)
			{
				temp.sprite.setScale({ -1,1 });
				temp.hitBox->setScale({ -1, 1 });
			}
			_projectile->push_back(temp);
			player->mana--;
		}
		else
		{
			_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_NOMANA_SCREAM_1");
		}
	}
}

void PlayerMagicAttackState::UseAOEMagicFire(void)
{
	player->hitBoxAOE.hitbox->setPosition(player->spriteAOE.getPosition());
	if (player->hitBoxAOE.isActive)
	{
		player->hitBoxAOE.hitbox->setOutlineColor(sf::Color::Magenta);
	}
	else
	{
		player->hitBoxAOE.hitbox->setOutlineColor(sf::Color::White);
	}


	if (player->animeAOE.currentFrame == 0 && player->compteurLineAOEMagic == 0)
	{
		player->hitBoxAOE.hitbox->setSize({ 0,0 });
	}
	else if (player->animeAOE.currentFrame == 1 && player->compteurLineAOEMagic == 0)
	{
		player->hitBoxAOE.hitbox->setSize({ 370.f / 5,131.f });
	}
	else if (player->animeAOE.currentFrame == 2 && player->compteurLineAOEMagic == 0)
	{
		player->hitBoxAOE.hitbox->setSize({ 370.f / 2.7f ,131.f });
	}
	else if (player->animeAOE.currentFrame == 3 && player->compteurLineAOEMagic == 0)
	{
		player->hitBoxAOE.hitbox->setSize({ 370.f / 3.f * 1.9f,131.f });
	}
	else if (player->animeAOE.currentFrame == 4 && player->compteurLineAOEMagic == 0)
	{
		player->hitBoxAOE.hitbox->setSize({ 370.f - 38.f,131.f });
	}
	else
	{
		player->hitBoxAOE.hitbox->setSize({ 370.f,131.f });
	}
}

void PlayerMagicAttackState::UseAOEMagicElectricity(void)
{
	player->hitBoxAOE.hitbox->setPosition(player->spriteAOE.getPosition());
	if (player->hitBoxAOE.isActive)
	{
		player->hitBoxAOE.hitbox->setOutlineColor(sf::Color::Magenta);
	}
	else
	{
		player->hitBoxAOE.hitbox->setOutlineColor(sf::Color::White);
	}


	if (player->animeAOE.currentFrame == 0 && player->compteurLineAOEMagic == 0)
	{
		player->hitBoxAOE.hitbox->setSize({ 0,0 });
	}
	else if (player->animeAOE.currentFrame == 1 && player->compteurLineAOEMagic == 0)
	{
		player->hitBoxAOE.hitbox->setSize({ 513.f / 5,57.f });
	}
	else if (player->animeAOE.currentFrame == 2 && player->compteurLineAOEMagic == 0)
	{
		player->hitBoxAOE.hitbox->setSize({ 513.f / 2.7f ,57.f });
	}
	else if (player->animeAOE.currentFrame == 3 && player->compteurLineAOEMagic == 0)
	{
		player->hitBoxAOE.hitbox->setSize({ 513.f / 3.f * 1.9f,57.f });
	}
	else if (player->animeAOE.currentFrame == 4 && player->compteurLineAOEMagic == 0)
	{
		player->hitBoxAOE.hitbox->setSize({ 513.f - 38.f,57.f });
	}
	else
	{
		player->hitBoxAOE.hitbox->setSize({ 513.f,57.f });
	}
}

void PlayerMagicAttackState::UseAOEMagic(Audio& _audio)
{
	if (player->mana >= 1 && player->compteurLongMagickManaConsume < 3)
	{
		switch (player->playerSelected)
		{
		case 0:
			UseAOEMagicFire();
			break;
		case 3:
			UseAOEMagicElectricity();
			break;
		}
	}
	else if ((player->mana == 0 || player->compteurLongMagickManaConsume == 3) && player->currentAnime.currentFrame < 5)
	{
		_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_NOMANA_SCREAM_1");
	}
}

void PlayerMagicAttackState::Update(float _dt, std::vector<Projectile>* _projectile, Audio& _audio)
{
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { -80 ,80 } : this->player->deaclageSprite = { 80 ,80 };
	UpdateAnim(_dt, _audio);

	if (player->mana != 0 && player->playerSelected != 1 && player->playerSelected != 2 && player->compteurLongMagickManaConsume != 3)
	{
		player->timerAddMana = TIMER_ADD_MANA;
	}

	if (player->magicAttackIsDistance)
	{
		AddDistanceMagic(_projectile, _audio);
	}
	else
	{
		UseAOEMagic(_audio);
	}

	if (player->hitBoxAOE.isActive)
	{
		player->hitBoxAOE.hitbox->setOutlineColor(sf::Color::Magenta);
	}
	else
	{
		player->hitBoxAOE.hitbox->setOutlineColor(sf::Color::White);;
	}

	if (player->currentAnime.currentFrame == player->currentAnime.maxFrame)
	{
		player->SetPlayerState(IDLE);
	}
}

void PlayerMagicAttackState::DisplayFireAOE(sf::RenderWindow& _window)
{
	if (player->currentAnime.currentFrame == 5 && !player->magicAttackIsDistance && player->mana > 0 && player->compteurLongMagickManaConsume < 3)
	{
		if (player->lastDir == LEFT)
		{
			player->spriteAOE.setScale({ -1,1 });
			player->hitBoxAOE.hitbox->setScale({ -1,1 });
			player->hitBoxAOE.hitbox->setPosition({ player->pos.x - 45 , player->pos.y - 120 });
			Utils::BlitSprite(player->spriteAOE, { player->pos.x - 45 , player->pos.y - 120 }, _window);
		}
		else
		{
			player->spriteAOE.setScale({ 1,1 });
			player->hitBoxAOE.hitbox->setScale({ 1,1 });
			player->hitBoxAOE.hitbox->setPosition({ player->pos.x + 45 , player->pos.y - 120 });
			Utils::BlitSprite(player->spriteAOE, { player->pos.x + 45 , player->pos.y - 120 }, _window);
		}
	}
}

void PlayerMagicAttackState::DisplayFireAOE(sf::RenderTexture& _textureRed, sf::RenderTexture& _textureBlue)
{
	if (player->currentAnime.currentFrame == 5 && !player->magicAttackIsDistance && player->mana > 0 && player->compteurLongMagickManaConsume < 3)
	{
		if (player->lastDir == LEFT)
		{
			player->spriteAOE.setScale({ -1,1 });
			player->hitBoxAOE.hitbox->setScale({ -1,1 });
			player->hitBoxAOE.hitbox->setPosition({ player->pos.x - 45 , player->pos.y - 120 });

			Utils::BlitSprite(player->spriteAOE, { player->pos.x + (TROAD_GAP * (-1 * player->pos.y + WORKING_HEIGHT) / 100.f) - 45 , player->pos.y - 120 }, _textureRed);
			Utils::BlitSprite(player->spriteAOE, { player->pos.x - (TROAD_GAP * (-1 * player->pos.y + WORKING_HEIGHT) / 100.f) - 45 , player->pos.y - 120 }, _textureBlue);
		}
		else
		{
			player->spriteAOE.setScale({ 1,1 });
			player->hitBoxAOE.hitbox->setScale({ 1,1 });
			player->hitBoxAOE.hitbox->setPosition({ player->pos.x + 45 , player->pos.y - 120 });

			Utils::BlitSprite(player->spriteAOE, { player->pos.x + (TROAD_GAP * (-1 * player->pos.y + WORKING_HEIGHT) / 100.f) + 45 , player->pos.y - 120 }, _textureRed);
			Utils::BlitSprite(player->spriteAOE, { player->pos.x - (TROAD_GAP * (-1 * player->pos.y + WORKING_HEIGHT) / 100.f) + 45 , player->pos.y - 120 }, _textureBlue);
		}
	}
}

void PlayerMagicAttackState::DisplayElectricityAOE(sf::RenderWindow& _window)
{
	if (player->currentAnime.currentFrame == 5 && !player->magicAttackIsDistance && player->mana > 0 && player->compteurLongMagickManaConsume < 3)
	{
		if (player->lastDir == LEFT)
		{
			player->spriteAOE.setScale({ -1,1 });
			player->hitBoxAOE.hitbox->setScale({ -1,1 });
			player->hitBoxAOE.hitbox->setPosition({ player->pos.x - 45 , player->pos.y - 80 });

			Utils::BlitSprite(player->spriteAOE, { player->pos.x - 45 , player->pos.y - 80 }, _window);
		}
		else
		{
			player->spriteAOE.setScale({ 1,1 });
			player->hitBoxAOE.hitbox->setScale({ 1,1 });
			player->hitBoxAOE.hitbox->setPosition({ player->pos.x + 45 , player->pos.y - 80 });
			Utils::BlitSprite(player->spriteAOE, { player->pos.x + 45 , player->pos.y - 80 }, _window);
		}
	}
}

void PlayerMagicAttackState::DisplayElectricityAOE(sf::RenderTexture& _textureRed, sf::RenderTexture& _textureBlue)
{
	if (player->currentAnime.currentFrame == 5 && !player->magicAttackIsDistance && player->mana > 0 && player->compteurLongMagickManaConsume < 3)
	{
		if (player->lastDir == LEFT)
		{
			player->spriteAOE.setScale({ -1,1 });
			player->hitBoxAOE.hitbox->setScale({ -1,1 });
			player->hitBoxAOE.hitbox->setPosition({ player->pos.x - 45 , player->pos.y - 80 });

			Utils::BlitSprite(player->spriteAOE, { player->pos.x + (TROAD_GAP * (-1 * player->pos.y + WORKING_HEIGHT) / 100.f) - 45 , player->pos.y - 80 }, _textureRed);
			Utils::BlitSprite(player->spriteAOE, { player->pos.x - (TROAD_GAP * (-1 * player->pos.y + WORKING_HEIGHT) / 100.f) - 45 , player->pos.y - 80 }, _textureBlue);
		}
		else
		{
			player->spriteAOE.setScale({ 1,1 });
			player->hitBoxAOE.hitbox->setScale({ 1,1 });
			player->hitBoxAOE.hitbox->setPosition({ player->pos.x + 45 , player->pos.y - 80 });

			Utils::BlitSprite(player->spriteAOE, { player->pos.x + (TROAD_GAP * (-1 * player->pos.y + WORKING_HEIGHT) / 100.f) + 45 , player->pos.y - 80 }, _textureRed);
			Utils::BlitSprite(player->spriteAOE, { player->pos.x - (TROAD_GAP * (-1 * player->pos.y + WORKING_HEIGHT) / 100.f) + 45 , player->pos.y - 80 }, _textureBlue);
		}
	}
}

void PlayerMagicAttackState::Display(sf::RenderWindow& _window)
{
	switch (player->playerSelected)
	{
	case 0:
		DisplayFireAOE(_window);
		break;
	case 3:
		DisplayElectricityAOE(_window);
		break;
	}
}

void PlayerMagicAttackState::Display(sf::RenderTexture& _textureRed, sf::RenderTexture& _textureBlue)
{
	switch (player->playerSelected)
	{
	case 0:
		DisplayFireAOE(_textureRed, _textureBlue);
		break;
	case 3:
		DisplayElectricityAOE(_textureRed, _textureBlue);
		break;
	}
}

void PlayerMagicAttackState::UpdateAnimeFire(float _dt)
{
	if ((player->currentAnime.currentFrame < 5 || player->mana == 0 || player->compteurLongMagickManaConsume == 3) && !player->magicAttackIsDistance)
	{
		bool temp = true;
		player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 9 + player->stats[(int)Stats::WISDOM], 0, temp, false);
		player->hitBoxAOE.isActive = false;
	}
	else if (!player->magicAttackIsDistance && player->mana > 0 && player->currentAnime.currentFrame == 5 && player->compteurLongMagickManaConsume < 3)
	{
		player->currentAnime.currentFrame = 5;
		player->hitBoxAOE.isActive = true;
		bool temp = true;
		player->animeAOE.UpdateLeftToRight(player->spriteAOE, _dt, 20, player->compteurLineAOEMagic, temp, true);
		if (player->animeAOE.currentFrame == 5)
		{
			player->animeAOE.currentFrame = 0;

			player->compteurLineAOEMagic++;
			if (player->compteurLineAOEMagic % 2 == false) // divissible par 2
			{
				player->mana--;
				player->compteurLongMagickManaConsume++;
			}
			if (player->compteurLineAOEMagic == 8)
			{
				player->compteurLineAOEMagic = 1;
			}
		}
	}
	else
	{
		bool temp = true;
		player->currentAnime.UpdateLeftToRight(player->sprite, _dt,9 + player->stats[(int)Stats::WISDOM], 0, temp, false);
		player->hitBoxAOE.isActive = false;
	}
}

void PlayerMagicAttackState::UpdateAnimePoison(float _dt)
{
	bool temp = true;
	player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 9 + player->stats[(int)Stats::WISDOM], 0, temp, false);
	if (player->currentAnime.currentFrame == 5 && player->mana > 0 && !player->viperSpellIsActive && player->compteurLongMagickManaConsume < 3)
	{
		player->viperSpellIsActive = true;
	}
}

void PlayerMagicAttackState::UpdateAnimeIce(float _dt, Audio& _audio)
{
	bool temp = true;
	player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 9 + player->stats[(int)Stats::WISDOM], 0, temp, false);
	if (player->currentAnime.currentFrame == 5 && player->mana > 0 && !player->iceSpellIsActive && player->compteurLongMagickManaConsume < 3)
	{
		player->iceSpellIsActive = true;
	}
}

void PlayerMagicAttackState::UpdateAnimeElectricity(float _dt)
{
	if ((player->currentAnime.currentFrame < 5 || player->mana == 0 || player->compteurLongMagickManaConsume == 3) && !player->magicAttackIsDistance)
	{
		bool temp = true;
		player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 9 + player->stats[(int)Stats::WISDOM], 0, temp, false);
		player->hitBoxAOE.isActive = false;
	}
	else if (!player->magicAttackIsDistance && player->mana > 0 && player->currentAnime.currentFrame == 5 && player->compteurLongMagickManaConsume < 3)
	{
		player->currentAnime.currentFrame = 5;
		player->hitBoxAOE.isActive = true;
		bool temp = true;
		player->animeAOE.UpdateLeftToRight(player->spriteAOE, _dt, 20, player->compteurLineAOEMagic, temp, true);
		if (player->animeAOE.currentFrame == 5)
		{
			player->animeAOE.currentFrame = 0;

			player->compteurLineAOEMagic++;
			if (player->compteurLineAOEMagic % 2 == false) // divissible par 2
			{
				player->mana--;
				player->compteurLongMagickManaConsume--;
			}
			if (player->compteurLineAOEMagic == 8)
			{
				player->compteurLineAOEMagic = 1;
			}
		}
	}
	else
	{
		bool temp = true;
		player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 9 + player->stats[(int)Stats::WISDOM], 0, temp, false);
		player->hitBoxAOE.isActive = false;
	}
}

void PlayerMagicAttackState::UpdateAnim(float _dt, Audio& _audio)
{
	if (!player->magicAttackIsDistance)
	{
		switch (player->playerSelected)
		{
		case 0:
			UpdateAnimeFire(_dt);
			break;
		case 1:
			UpdateAnimePoison(_dt);
			break;
		case 2:
			UpdateAnimeIce(_dt, _audio);
			break;
		case 3:
			UpdateAnimeElectricity(_dt);
			break;
		}
	}
	else
	{
		bool temp = true;
		player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 9 + player->stats[(int)Stats::WISDOM], 0, temp, false);
	}
}

PlayerMagicAttackState::PlayerMagicAttackState(Player* _player)
{
	this->player = _player;
}