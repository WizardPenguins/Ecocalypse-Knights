#include "stdafx.h"
#include "Player.hpp"
#include "PlayerRunState.h"
#include "PlayerJumpState.h"
#include "PlayerKnockDownState.h"
#include "PlayerLightAttackState.h"
#include "PlayerIdleState.h"
#include "PlayerBowAttackState.h"
#include "PlayerDeadState.h"
#include "PlayerHitState.h"
#include "PlayerHitGroundState.h"
#include "PlayerHeavyAttackState.h"
#include "PlayerMagicAttackState.h"
#include "PlayerParryState.h"
#include "PlayerPrayState.h"
#include "PlayerReviveState.h"
#include "PlayerEatByBossState.h"
#include "PlayerFreeFallState.h"
#include "PlayerDashState.h"
#include "PlayerTauntState.h"
#include "Paralax.h"
#include "Camera.h"
#include "Audio.h"
#include "Network.h"
#include "Upgrade.h"

void Player::InitStatsPlayer()
{
	for (int i = 0; i < STATS_NBR; i++)
	{
		stats[i] = 1;
	}
	this->skillPoints = 0;
	this->SetStatsValue();
}

Player::Player(GameData& _gameData, int _id)
{
	this->speed = { 300.f , 150.f };
	this->parrySpeed = { 75.f , 65.5f };
	//si le joueur operant est en multiplayer on remplace son id 0 par celle qu'il a pour le serveur afin de lui donner sa position de depart
	if (_gameData.multiplayer)
	{
		int id = 0;
		if (_gameData.network->state == NetworkState::CLIENT)
		{
			id = _gameData.network->client->id;
		}
		switch (id)
		{
		case 0:
			this->pos = { 500.f, 900.f };
			break;
		case 1:
			this->pos = { 500.f, 800.f };
			break;
		case 2:
			this->pos = { 400.f, 900.f };
			break;
		case 3:
			this->pos = { 400.f, 800.f };
			break;
		}
	}
	else
	{
		switch (_id)
		{
		case 0:
			this->pos = { 500.f, 900.f };
			break;
		case 1:
			this->pos = { 500.f, 800.f };
			break;
		case 2:
			this->pos = { 400.f, 900.f };
			break;
		case 3:
			this->pos = { 400.f, 800.f };
			break;
		}
	}
	this->state = IDLE;

	this->damaged = 10;
	this->hitbox = new sf::RectangleShape{ sf::Vector2f{127.f,164.f} };
	sf::Vector2f nouvelleTaille = { (float)((this->hitbox->getSize().x) / 2.f), (float)((this->hitbox->getSize().y) - (this->hitbox->getSize().y / 8)) };
	this->hitbox->setOrigin(nouvelleTaille);
	this->shieldFX = Utils::LoadSprite("Assets/Images/Entities/Players/ShieldFX.png", CENTER, 4, 1);
	this->shieldFXAnime.CreateLeftToRight(this->shieldFX, 4, 1, this->shieldFX.getTexture()->getSize());
	this->id = _id;
	this->allSprite = _gameData.playersSprite;
	this->animeStatu = *_gameData.statusSprite;
	this->animeStatu.anim = new Animation();
	this->hasCastMagick = false;
	this->hasShootArrow = false;
	this->heavyAttackGoUp = false;
	this->heavyAttackHitboxActived = false;
	this->lightAttackHitboxActived = false;
	this->magicAttackHitboxActivated = false;
	this->inAir = false;
	this->inArena = _gameData.arenaIsChose;
	this->lastState = IDLE;
	this->level = 1;
	this->xp = 0;
	this->SetLastLife();
	InitStatsPlayer();
	SetStatsValue();
	this->mana = maxMana;
	this->life = maxLife;
	this->lastLife = life;
	this->gold = 100;
	this->lastGold = gold;

	this->playerJumpState = new PlayerJumpState(this);
	this->playerRunState = new PlayerRunState(this);
	this->playerKnockDownState = new PlayerKnockDownState(this);
	this->playerIdleState = new PlayerIdleState(this);
	this->playerHitState = new PlayerHitState(this);
	this->playerHitGroundState = new PlayerHitGroundState(this);
	this->playerDeadState = new PlayerDeadState(this);
	this->playerLightAttackState = new PlayerLightAttackState(this);
	this->playerHeavyAttackState = new PlayerHeavyAttackState(this);
	this->playerMagicAttackState = new PlayerMagicAttackState(this);
	this->playerParryState = new PlayerParryState(this);
	this->playerBowAttackState = new PlayerBowAttackState(this);
	this->playerPrayState = new PlayerPrayState(this);
	this->playerReviveState = new PlayerReviveState(this);
	this->playerEatByBossState = new PlayerEatByBossState(this);
	this->playerFreeFallState = new PlayerFreeFallState(this);
	this->playerDashState = new PlayerDashState(this);
	this->playerTauntState = new PlayerTauntState(this);
}

Player::~Player()
{

}

void Player::Move(float _dt, Audio& _audio, bool& _inCinematic)
{
	switch (state)
	{
	case JUMP:
		playerJumpState->PlayerJumpStateRun(_dt, _audio);
		break;
	case RUN:
		playerRunState->PlayerRunStateRun(_dt, _audio, _inCinematic);
		break;
	case KNOCKDOWN:
		playerKnockDownState->Update(_dt);
		break;
	case IDLE:
		playerIdleState->PlayerIdleStateRun(_dt, _audio);
		break;
	case HIT:
		playerHitState->PlayerHitStateRun(_dt, _audio);
		break;
	case HIT_GROUND:
		playerHitGroundState->PlayerHitGroundStateRun(_dt);
		break;
	case DEAD:
		playerDeadState->PlayerDeadStateRun(_dt);
		break;
	case REVIVE:
		playerReviveState->PlayerReviveStateRun(_dt);
		break;
	case PRAY:
		playerPrayState->PlayerPrayStateRun(_dt);
		break;
	case EAT_BY_BOSS:
		playerEatByBossState->PlayerEatByBossStateRun(_dt);
		break;
	case FREEFALL:
		playerFreeFallState->PlayerFreeFallStateRun(_dt, _audio);
		break;
	case DASH:
		playerDashState->PlayerDashStateRun(_dt);
		break;
	case TAUNT:
		playerTauntState->PlayerTauntStateRun(_dt);
		break;
	}
}

void Player::Fight(float _dt, std::vector<Projectile>* _projectile, Audio& _audio, bool& _multiplayer, Network& _network)
{
	switch (state)
	{
	case LIGHTATTACK:
		playerLightAttackState->PlayerLightAttackStateRun(_dt);
		break;
	case HEAVYATTACK:
		playerHeavyAttackState->PlayerHeavyAttackStateRun(_dt, _audio);
		break;
	case BOWATTACK:
		playerBowAttackState->Update(_dt, _projectile);
		break;
	case PARRY:
		playerParryState->Update(_dt, _multiplayer, _network);
		break;
	case MAGICATTACK:
		playerMagicAttackState->Update(_dt, _projectile, _audio);
		break;
	}
}

void Player::Init()
{
	playerIdleState->Init();
}

void Player::Update(GameData& _gameData, float _dtSlowMo, bool _dontMoveArena)
{
	if (isLoading)
	{
		UpdatePlayerInfos();
		UpdatePlayerTimers(_gameData);

		if (!_gameData.inCinematic)
		{
			if (!_dontMoveArena)
			{
				Input(*_gameData.audio, _gameData.timerButton);
				Move(_dtSlowMo, *_gameData.audio, _gameData.inCinematic);
				Fight(_dtSlowMo, &_gameData.projectile, *_gameData.audio, _gameData.multiplayer, *_gameData.network);
			}
		}
		UpdateInventory(_dtSlowMo);
		UpdateStatus(_dtSlowMo);
		UpdateViperAnime(_dtSlowMo);
		UpdateIceSpellAnime(_dtSlowMo);
		UpdateViperHitBox();
		UpdateIceSpellHitBox();

		if (state == IDLE || state == RUN || state == JUMP || state == PARRY || state == LIGHTATTACK || state == MAGICATTACK)
		{
			if (auraIsActived)
			{
				UpdateAnime(_dtSlowMo);
			}
		}

		UpdatePlayerMapColliding(_gameData);
		UpdateCinematic(_gameData);
		LevelUp();
		UpdatePlayerSound(_gameData);

		//set Hitbos pos at end of Updates
		(bool)(this->inAir) ? (bool)(this->heavyAttackGoUp) ? hitbox->setPosition(sf::Vector2f{ this->pos.x,this->posInAir.y }) : hitbox->setPosition(sf::Vector2f{ this->posInAir }) : hitbox->setPosition(pos);
	}
	else if (!isLoading)
	{
		LoadPlayerAtBegening();
	}
}

void Player::Display(Audio& _audio, sf::RenderTexture& _textureRed, sf::RenderTexture& _textureBlue, Camera* _camera, LevelState& _levelState)
{
	//display magic AOE
	if (this->state == MAGICATTACK)
	{
		this->playerMagicAttackState->Display(_textureRed, _textureBlue);
	}

	//display aura
	if (state == IDLE || state == RUN || state == JUMP || state == PARRY || state == LIGHTATTACK || state == MAGICATTACK)
	{
		if (auraIsActived && !inAir)
		{
			Utils::BlitSprite(auraSprite, { pos.x + (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f), pos.y }, 0, _textureRed);
			Utils::BlitSprite(auraSprite, { pos.x - (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f), pos.y }, 0, _textureBlue);
		}
		else if (auraIsActived && inAir)
		{
			Utils::BlitSprite(auraSprite, { posInAir.x + (TROAD_GAP * (-1 * posInAir.y + WORKING_HEIGHT) / 100.f),  posInAir.y }, 0, _textureRed);
			Utils::BlitSprite(auraSprite, { posInAir.x - (TROAD_GAP * (-1 * posInAir.y + WORKING_HEIGHT) / 100.f),  posInAir.y }, 0, _textureBlue);
		}
	}

	if (showPotionAnime)
	{
		if (!inAir)
		{
			Utils::BlitSprite(potionEffect, { pos.x + (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f), pos.y }, 0, _textureRed);
			Utils::BlitSprite(potionEffect, { pos.x - (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f), pos.y }, 0, _textureBlue);
		}
		else if (inAir)
		{
			Utils::BlitSprite(potionEffect, { posInAir.x + (TROAD_GAP * (-1 * posInAir.y + WORKING_HEIGHT) / 100.f),  posInAir.y }, 0, _textureRed);
			Utils::BlitSprite(potionEffect, { posInAir.x - (TROAD_GAP * (-1 * posInAir.y + WORKING_HEIGHT) / 100.f),  posInAir.y }, 0, _textureBlue);
		}
	}

	//display player
	(bool)(this->lastDir == LEFT) ? sprite.setScale({ -1,1 }) : sprite.setScale({ 1,1 });

	if (_levelState == LEVEL3)
	{
		_camera->TpPlayerLevel3(this, playerIsShrinks);

	}
	if (!playerIsShrinks)
	{
		realDeaclageSprite = deaclageSprite;
		auraSprite.setScale(1.f, 1.f);
		auraParticuleSprite.setScale(1.f, 1.f);
	}
	else
	{
		realDeaclageSprite = deaclageSprite * SHRINKS;
		auraSprite.setScale(SHRINKS, SHRINKS);
		auraParticuleSprite.setScale(SHRINKS, SHRINKS);

	}

	(bool)(this->inAir) ? (bool)(this->heavyAttackGoUp) ? this->sprite.setPosition(this->pos.x + this->deaclageSprite.x, this->posInAir.y + this->deaclageSprite.y) : this->sprite.setPosition(this->posInAir + this->deaclageSprite) : this->sprite.setPosition(this->pos + this->deaclageSprite);
	if (this->state == KNOCKDOWN)
	{
		this->sprite.setPosition(this->pos.x + this->deaclageSprite.x + (TROAD_GAP * (-1 * posInAir.y + this->deaclageSprite.y + WORKING_HEIGHT) / 100.f), this->posInAir.y + this->deaclageSprite.y);
	}
	if (this->state != EAT_BY_BOSS)
	{
		_textureRed.draw(this->sprite);
	}

	(bool)(this->inAir) ? (bool)(this->heavyAttackGoUp) ? this->sprite.setPosition(this->pos.x + this->deaclageSprite.x - (TROAD_GAP * (-1 * posInAir.y + this->deaclageSprite.y + WORKING_HEIGHT) / 100.f), this->posInAir.y + this->deaclageSprite.y) : this->sprite.setPosition({ this->posInAir.x + this->deaclageSprite.x - (TROAD_GAP * (-1 * posInAir.y + this->deaclageSprite.y + WORKING_HEIGHT) / 100.f),this->posInAir.y + this->deaclageSprite.y }) : this->sprite.setPosition({ this->pos.x + this->deaclageSprite.x - (TROAD_GAP * (-1 * pos.y + this->deaclageSprite.y + WORKING_HEIGHT) / 100.f),this->pos.y + this->deaclageSprite.y });
	if (this->state == KNOCKDOWN)
	{
		this->sprite.setPosition(this->pos.x + this->deaclageSprite.x - (TROAD_GAP * (-1 * posInAir.y + this->deaclageSprite.y + WORKING_HEIGHT) / 100.f), this->posInAir.y + this->deaclageSprite.y);
	}
	if (this->state != EAT_BY_BOSS)
	{
		_textureBlue.draw(this->sprite);
	}

	//display ShieldFx
	if (showShieldFx)
	{
		(bool)(this->lastDir == LEFT) ? Utils::BlitSprite(shieldFX, { (pos.x - 60) + (TROAD_GAP),(pos.y - 80) + TROAD_GAP }, 0, _textureBlue) : Utils::BlitSprite(shieldFX, { (pos.x + 60) + (TROAD_GAP),(pos.y - 80) + TROAD_GAP }, 0, _textureBlue);
		(bool)(this->lastDir == LEFT) ? Utils::BlitSprite(shieldFX, { (pos.x - 60) - (TROAD_GAP),(pos.y - 80) - TROAD_GAP }, 0, _textureRed) : Utils::BlitSprite(shieldFX, { (pos.x + 60) - (TROAD_GAP),(pos.y - 80) - TROAD_GAP }, 0, _textureRed);
	}

	//display auraParticule
	if (state == IDLE || state == RUN || state == JUMP || state == PARRY || state == LIGHTATTACK || state == MAGICATTACK)
	{
		if (auraIsActived && !inAir)
		{
			Utils::BlitSprite(auraParticuleSprite, { pos.x + (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f), pos.y }, 0, _textureBlue);
			Utils::BlitSprite(auraParticuleSprite, { pos.x - (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f), pos.y }, 0, _textureRed);
		}
		else if (auraIsActived && inAir)
		{
			Utils::BlitSprite(auraParticuleSprite, { posInAir.x + (TROAD_GAP * (-1 * posInAir.y + WORKING_HEIGHT) / 100.f),  posInAir.y }, 0, _textureBlue);
			Utils::BlitSprite(auraParticuleSprite, { posInAir.x - (TROAD_GAP * (-1 * posInAir.y + WORKING_HEIGHT) / 100.f),  posInAir.y }, 0, _textureRed);
		}
	}

	//display status
	if (status != Status::None)
	{
		if (!playerIsShrinks)
		{
			animeStatu.spriteAnim[(int)(status)-1].sprite.setScale(sf::Vector2f{ 0.5f,0.5f });
			if (this->status == Status::OnPoison)
			{
				animeStatu.spriteAnim[(int)(status)-1].sprite.setScale(sf::Vector2f{ 0.25f,0.25f });
			}
			(bool)(this->inAir) ? animeStatu.spriteAnim[(int)(status)-1].sprite.setPosition(sf::Vector2f{ this->posInAir.x + (TROAD_GAP * (-1 * this->posInAir.y - 125.f + WORKING_HEIGHT) / 100.f),this->posInAir.y - 125.f }) : animeStatu.spriteAnim[(int)(status)-1].sprite.setPosition(sf::Vector2f{ this->pos.x + (TROAD_GAP * (-1 * this->pos.y - 125.f + WORKING_HEIGHT) / 100.f),this->pos.y - 125.f });
			_textureRed.draw(animeStatu.spriteAnim[(int)(status)-1].sprite);

			(bool)(this->inAir) ? animeStatu.spriteAnim[(int)(status)-1].sprite.setPosition(sf::Vector2f{ this->posInAir.x - (TROAD_GAP * (-1 * this->posInAir.y - 125.f + WORKING_HEIGHT) / 100.f),this->posInAir.y - 125.f }) : animeStatu.spriteAnim[(int)(status)-1].sprite.setPosition(sf::Vector2f{ this->pos.x - (TROAD_GAP * (-1 * this->pos.y - 125.f + WORKING_HEIGHT) / 100.f),this->pos.y - 125.f });
			_textureBlue.draw(animeStatu.spriteAnim[(int)(status)-1].sprite);
		}
		else
		{
			animeStatu.spriteAnim[(int)(status)-1].sprite.setScale(sf::Vector2f{ 0.5f * SHRINKS,0.5f * SHRINKS });
			(bool)(this->inAir) ? animeStatu.spriteAnim[(int)(status)-1].sprite.setPosition(sf::Vector2f{ this->posInAir.x * SHRINKS + (TROAD_GAP * (-1 * (this->posInAir.y - 125.f) * SHRINKS + WORKING_HEIGHT) / 100.f),(this->posInAir.y - 125.f) * SHRINKS }) : animeStatu.spriteAnim[(int)(status)-1].sprite.setPosition(sf::Vector2f{ this->pos.x * SHRINKS + (TROAD_GAP * (-1 * (this->pos.y - 125.f) * SHRINKS + WORKING_HEIGHT) / 100.f),(this->pos.y - 125.f) * SHRINKS });
			_textureRed.draw(animeStatu.spriteAnim[(int)(status)-1].sprite);

			(bool)(this->inAir) ? animeStatu.spriteAnim[(int)(status)-1].sprite.setPosition(sf::Vector2f{ this->posInAir.x * SHRINKS - (TROAD_GAP * (-1 * (this->posInAir.y - 125.f) * SHRINKS + WORKING_HEIGHT) / 100.f),(this->posInAir.y - 125.f) * SHRINKS }) : animeStatu.spriteAnim[(int)(status)-1].sprite.setPosition(sf::Vector2f{ this->pos.x * SHRINKS - (TROAD_GAP * (-1 * (this->pos.y - 125.f) * SHRINKS + WORKING_HEIGHT) / 100.f),(this->pos.y - 125.f) * SHRINKS });
			_textureBlue.draw(animeStatu.spriteAnim[(int)(status)-1].sprite);
		}
	}

	//display Shield bar
	if (state == PARRY || shieldIsBreaking)
	{
		if (!playerIsShrinks)
		{
			shieldSprite.setScale(1.f, 1.f);
			(bool)(lastDir == LEFT) ? shieldSprite.setPosition({ pos.x - 70 + (TROAD_GAP * (-1 * (pos.y - 230 + 10 * sinf(timerSin * 6)) + WORKING_HEIGHT) / 100.f), pos.y - 230 + 10 * sinf(timerSin * 6) }) : shieldSprite.setPosition({ pos.x + 70 + (TROAD_GAP * (-1 * pos.y - 230 + 10 * sinf(timerSin * 6) + WORKING_HEIGHT) / 100.f), pos.y - 230 + 10 * sinf(timerSin * 6) });
			_textureRed.draw(shieldSprite);

			(bool)(lastDir == LEFT) ? shieldSprite.setPosition({ pos.x - 70 - (TROAD_GAP * (-1 * (pos.y - 230 + 10 * sinf(timerSin * 6)) + WORKING_HEIGHT) / 100.f), pos.y - 230 + 10 * sinf(timerSin * 6) }) : shieldSprite.setPosition({ pos.x + 70 - (TROAD_GAP * (-1 * pos.y - 230 + 10 * sinf(timerSin * 6) + WORKING_HEIGHT) / 100.f), pos.y - 230 + 10 * sinf(timerSin * 6) });
			_textureBlue.draw(shieldSprite);
		}
		else
		{
			shieldSprite.setScale(SHRINKS, SHRINKS);
			(bool)(lastDir == LEFT) ? shieldSprite.setPosition({ pos.x - 70 + (TROAD_GAP * (-1 * (pos.y - 230 + 10 * sinf(timerSin * 6)) + WORKING_HEIGHT) / 100.f), pos.y - 230 + 10 * sinf(timerSin * 6) }) : shieldSprite.setPosition({ pos.x + 70 + (TROAD_GAP * (-1 * pos.y - 230 + 10 * sinf(timerSin * 6) + WORKING_HEIGHT) / 100.f), pos.y - 230 + 10 * sinf(timerSin * 6) });
			_textureRed.draw(shieldSprite);

			(bool)(lastDir == LEFT) ? shieldSprite.setPosition({ pos.x - 70 - (TROAD_GAP * (-1 * (pos.y - 230 + 10 * sinf(timerSin * 6)) + WORKING_HEIGHT) / 100.f), pos.y - 230 + 10 * sinf(timerSin * 6) }) : shieldSprite.setPosition({ pos.x + 70 - (TROAD_GAP * (-1 * pos.y - 230 + 10 * sinf(timerSin * 6) + WORKING_HEIGHT) / 100.f), pos.y - 230 + 10 * sinf(timerSin * 6) });
			_textureBlue.draw(shieldSprite);
		}
	}
}

void Player::Display(sf::RenderWindow& _window, Audio& _audio, Camera* _camera, LevelState& _levelState)
{
	//display magic AOE
	if (this->state == MAGICATTACK)
	{
		this->playerMagicAttackState->Display(_window);
	}

	//display aura
	if (state == IDLE || state == RUN || state == JUMP || state == PARRY || state == LIGHTATTACK || state == MAGICATTACK)
	{
		if (auraIsActived && !inAir)
		{
			Utils::BlitSprite(auraSprite, { pos }, 0, _window);
		}
		else if (auraIsActived && inAir)
		{
			Utils::BlitSprite(auraSprite, { posInAir }, 0, _window);
		}
	}

	//display player
	(bool)(this->lastDir == LEFT) ? sprite.setScale({ -1,1 }) : sprite.setScale({ 1,1 });


	if (_levelState == LEVEL3)
	{
		_camera->TpPlayerLevel3(this, playerIsShrinks);

	}
	if (!playerIsShrinks)
	{
		realDeaclageSprite = deaclageSprite;
		auraSprite.setScale(1.f, 1.f);
		auraParticuleSprite.setScale(1.f, 1.f);
	}
	else
	{
		realDeaclageSprite = deaclageSprite * SHRINKS;
		auraSprite.setScale(SHRINKS, SHRINKS);
		auraParticuleSprite.setScale(SHRINKS, SHRINKS);
	}

	(bool)(this->inAir) ? (bool)(this->heavyAttackGoUp) ? this->sprite.setPosition(this->pos.x + this->realDeaclageSprite.x, this->posInAir.y + this->realDeaclageSprite.y) : this->sprite.setPosition(this->posInAir + this->realDeaclageSprite) : this->sprite.setPosition(this->pos + this->realDeaclageSprite);
	if (this->state == KNOCKDOWN)
	{
		this->sprite.setPosition(this->pos.x + this->realDeaclageSprite.x, this->posInAir.y + this->realDeaclageSprite.y);
	}

	if (this->state != EAT_BY_BOSS)
	{
		_window.draw(this->sprite);
	}

	//display ShieldFx
	if (showShieldFx)
	{
		(bool)(this->lastDir == LEFT) ? Utils::BlitSprite(shieldFX, { pos.x - 60, pos.y - 80 }, 0, _window) : Utils::BlitSprite(shieldFX, { pos.x + 60, pos.y - 80 }, 0, _window);
	}

	//display auraParticule
	if (state == IDLE || state == RUN || state == JUMP || state == PARRY || state == LIGHTATTACK || state == MAGICATTACK)
	{
		if (auraIsActived && !inAir)
		{
			Utils::BlitSprite(auraParticuleSprite, { pos }, 0, _window);
		}
		else if (auraIsActived && inAir)
		{
			Utils::BlitSprite(auraParticuleSprite, { posInAir }, 0, _window);
		}
	}

	//display potion effect
	if (showPotionAnime)
	{
		if (!inAir)
		{
			Utils::BlitSprite(potionEffect, { pos }, 0, _window);
		}
		else if (inAir)
		{
			Utils::BlitSprite(potionEffect, { posInAir }, 0, _window);
		}
	}

	//display status
	if (status != Status::None)
	{
		if (!playerIsShrinks)
		{
			animeStatu.spriteAnim[(int)(status)-1].sprite.setScale(sf::Vector2f{ 0.5f,0.5f });
			if (this->status == Status::OnPoison)
			{
				animeStatu.spriteAnim[(int)(status)-1].sprite.setScale(sf::Vector2f{ 0.25f,0.25f });
			}
			(bool)(this->inAir) ? animeStatu.spriteAnim[(int)(status)-1].sprite.setPosition(sf::Vector2f{ this->posInAir.x,this->posInAir.y - 125.f }) : animeStatu.spriteAnim[(int)(status)-1].sprite.setPosition(sf::Vector2f{ this->pos.x,this->pos.y - 125.f });
			_window.draw(animeStatu.spriteAnim[(int)(status)-1].sprite);
		}
		else
		{
			animeStatu.spriteAnim[(int)(status)-1].sprite.setScale(sf::Vector2f{ 0.5f * SHRINKS,0.5f * SHRINKS });
			(bool)(this->inAir) ? animeStatu.spriteAnim[(int)(status)-1].sprite.setPosition(sf::Vector2f{ this->posInAir.x * SHRINKS,(this->posInAir.y - 125.f) * SHRINKS }) : animeStatu.spriteAnim[(int)(status)-1].sprite.setPosition(sf::Vector2f{ this->pos.x * SHRINKS,(this->pos.y - 125.f) * SHRINKS });
			_window.draw(animeStatu.spriteAnim[(int)(status)-1].sprite);
		}
	}

	//display Shield bar
	if (state == PARRY || shieldIsBreaking)
	{
		if (!playerIsShrinks)
		{
			shieldSprite.setScale(1.f, 1.f);
			(bool)(lastDir == LEFT) ? shieldSprite.setPosition({ pos.x - 70, pos.y - 230 + 10 * sinf(timerSin * 6) }) : shieldSprite.setPosition({ pos.x + 70, pos.y - 230 + 10 * sinf(timerSin * 6) });
			_window.draw(shieldSprite);
		}
		else
		{
			shieldSprite.setScale(SHRINKS, SHRINKS);
			(bool)(lastDir == LEFT) ? shieldSprite.setPosition({ pos.x - (70 * SHRINKS), pos.y - (230 + 10 * sinf(timerSin * 6 * SHRINKS)) * SHRINKS }) : shieldSprite.setPosition({ pos.x + (70 * SHRINKS), pos.y - (230 + 10 * sinf(timerSin * 6 * SHRINKS)) * SHRINKS });

			_window.draw(shieldSprite);
		}
	}
}

void Player::InitNewAnimStatus(void)
{
	if (animeStatu.spriteAnim[(int)(status)-1].nbAnim.x != 0)
	{
		sf::IntRect rect = sf::IntRect(0, 0, (int)(animeStatu.spriteAnim[(int)(status)-1].animDim.x / animeStatu.spriteAnim[(int)(status)-1].nbAnim.x), (int)(animeStatu.spriteAnim[(int)(status)-1].nbAnim.y));
		animeStatu.anim->CreateLeftToRight(animeStatu.spriteAnim[(int)(status)-1].sprite, (unsigned char)(animeStatu.spriteAnim[(int)(status)-1].nbAnim.x), (unsigned char)(animeStatu.spriteAnim[(int)(status)-1].nbAnim.y), animeStatu.spriteAnim[(int)(status)-1].animDim);
		animeStatu.spriteAnim[(int)(status)-1].sprite.setTextureRect(rect);

		hitbox->setSize(sf::Vector2f{ animeStatu.spriteAnim[(int)(status)-1].animDim.x / animeStatu.spriteAnim[(int)(status)-1].nbAnim.x , animeStatu.spriteAnim[(int)(status)-1].animDim.y / animeStatu.spriteAnim[(int)(status)-1].nbAnim.y });
		hitbox->setOrigin(hitbox->getSize().x / 2, hitbox->getSize().y / 2);
		this->lastStatus = this->status;
	}
}

void Player::UpdateStatus(float _dt)
{
	if (status != lastStatus)
	{
		InitNewAnimStatus();
	}

	if (status != Status::None)
	{
		bool temp = true;
		animeStatu.anim->UpdateLeftToRight(animeStatu.spriteAnim[(int)(status)-1].sprite, _dt / 2, animeStatu.spriteAnim[(int)(status)-1].speed, 0, temp, true);
		lastStatus = status;

		if ((timerStatus < TIMERSTATUS - 1 && timerStatus > TIMERSTATUS - 1 - _dt) || (timerStatus < TIMERSTATUS - 2 && timerStatus > TIMERSTATUS - 2 - _dt) || (timerStatus < TIMERSTATUS - 3 && timerStatus > TIMERSTATUS - 3 - _dt))
		{
			life -= 1;
		}
		timerStatus -= _dt;

		if (timerStatus <= 0)
		{
			timerStatus = TIMERSTATUS;
			status = Status::None;
			lastStatus = Status::None;
		}
	}
}

void Player::UpdatePlayerInfos(void)
{
	posInAir.x = pos.x;
	lastPos = pos;

	if (Utils::Distance(pos, posInAir) > WORKING_HEIGHT)
	{
		inAir = false;
		posInAir = pos;
		SetPlayerState(IDLE);
	}

	if (!playerIsShrinks)
	{
		realSpeed = speed;
	}
	else
	{
		realSpeed = speed * SHRINKS;
	}

	if (!inAir && state != KNOCKDOWN)
	{
		posInAir.y = pos.y;
	}
}

void Player::UpdatePlayerTimers(GameData& _gameData)
{
	parryTimerSound += _gameData.dt;
	timerSound += _gameData.dt;
	timerSin += _gameData.dt;
	timerShieldInvicibility -= _gameData.dt;

	isManaRecoveringFinished = false;
	if (mana < maxMana && !viperSpellIsActive && !iceSpellIsActive)
	{

		timerAddMana -= _gameData.dt;
		if (timerAddMana <= 0)
		{

			timerAddMana = TIMER_ADD_MANA;
			isManaRecoveringFinished = true;
			mana++;
		}
	}

	if (!shieldHisHit && shieldLife != MAXSHIELDLIFE)
	{
		timerShieldLife -= _gameData.dt;
		if (timerShieldLife <= 0)
		{
			shieldLife++;
			shieldIsBreaking = false;
			timerShieldLife = TIMERSHIELD;
		}
	}
	else if (shieldHisHit)
	{
		shieldHisHit = false;
		showShieldFx = true;
		timerShieldLife = TIMERSHIELD;
		shieldLife--;
		if (shieldLife <= 0)
		{
			shieldIsBreaking = true;
			shieldAnime.currentFrame = 9;
			shieldAnime.UpdateLeftToRight(shieldSprite, _gameData.dt, 8, 0, shieldHisHit, false);
		}
	}

	if (showShieldFx)
	{
		if (shieldFXAnime.currentFrame == shieldFXAnime.maxFrame)
		{
			showShieldFx = false;
			shieldFXAnime.currentFrame = 0;
		}

		bool temp = true;
		shieldFXAnime.UpdateLeftToRight(shieldFX, _gameData.dt, 7, 0, temp, false);
	}

	if (comboNumber >= 1)
	{
		timerCombo -= _gameData.dt;
		if (timerCombo <= 0)
		{
			timerCombo = TIMER_ADD_MANA;
			comboNumber = 0;
		}
	}

	if (!canDash && timerBetweenDash > 0 && state != DASH)
	{
		if (stats[(int)Stats::AGILITY] < 5)
		{
			timerBetweenDash -= _gameData.dt;
			if (timerBetweenDash <= 0)
			{
				canDash = true;
				dashNumber = 0;
				timerBetweenDash = TIMERDASH;
			}
		}
		else if ((stats[(int)Stats::AGILITY] >= 5 && stats[(int)Stats::AGILITY] < 10))
		{
			if (dashNumber == 2)
			{
				timerBetweenDash -= _gameData.dt;
				if (timerBetweenDash <= 0)
				{
					canDash = true;
					dashNumber = 0;
					timerBetweenDash = TIMERDASH;
				}
			}
			else
			{
				canDash = true;
			}
		}
		else if ((stats[(int)Stats::AGILITY] >= 10))
		{
			if (dashNumber == 3)
			{
				timerBetweenDash -= _gameData.dt;
				if (timerBetweenDash <= 0)
				{
					canDash = true;
					dashNumber = 0;
					timerBetweenDash = TIMERDASH;
				}
			}
			else
			{
				canDash = true;
			}
		}
	}

	if (viperSpellIsActive || iceSpellIsActive)
	{
		timerAddMana -= _gameData.dt * 4;
		if (timerAddMana <= 0.f)
		{
			timerAddMana = TIMER_ADD_MANA;
			mana--;
			compteurLongMagickManaConsume++;
		}
	}

	if (_gameData.levelState == LEVEL1)
	{
		if (_gameData.tempImage1.getPixel(pos.x, pos.y) == sf::Color::Red)
		{
			timerInRed += _gameData.dt;
		}
		else
		{
			timerInRed = 0.f;
		}
	}

	if (_gameData.levelState == LEVEL2)
	{
		if (_gameData.tempImage2.getPixel(pos.x, pos.y) == sf::Color::Red)
		{
			timerInRed += _gameData.dt;
		}
		else
		{
			timerInRed = 0.f;
		}
	}

	if (_gameData.levelState == LEVEL3)
	{
		if (_gameData.tempImage3.getPixel(pos.x, pos.y) == sf::Color::Red)
		{
			timerInRed += _gameData.dt;
		}
		else
		{
			timerInRed = 0.f;
		}
	}
}

void Player::UpdatePlayerMapColliding(GameData& _gameData)
{
	if (_gameData.levelState == LEVEL1)
	{
		if (_gameData.tempImage1.getPixel((unsigned int)this->pos.x, (unsigned int)this->pos.y) == sf::Color::Red || this->pos.x > _gameData.camera->GetView().getCenter().x + _gameData.camera->GetView().getSize().x / 2 || this->pos.x < _gameData.camera->GetView().getCenter().x - _gameData.camera->GetView().getSize().x / 2)
		{
			this->pos = lastPos;
		}
	}
	else if (_gameData.levelState == LEVEL2)
	{
		if (_gameData.tempImage2.getPixel((unsigned int)this->pos.x, (unsigned int)this->pos.y) == sf::Color::Red || this->pos.x > _gameData.camera->GetView().getCenter().x + _gameData.camera->GetView().getSize().x / 2 || this->pos.x < _gameData.camera->GetView().getCenter().x - _gameData.camera->GetView().getSize().x / 2)
		{
			this->pos = lastPos;
		}
	}
	else if (_gameData.levelState == LEVEL3)
	{
		if (_gameData.tempImage3.getPixel(this->pos.x, this->pos.y) == sf::Color::Red)
		{
			this->pos = lastPos;
		}

		if (_gameData.actualWave == WAVE6 && _gameData.tabMob.size() == 0)
		{
			collideWithYellow = false;
		}

		if (_gameData.tempImage3.getPixel(this->pos.x, this->pos.y) == sf::Color(255, 0, 255) && collideWithYellow)
		{
			this->pos = lastPos;
		}

		if (_gameData.tempImage3.getPixel(this->pos.x, this->pos.y) == sf::Color::Green)
		{
			_gameData.inCinematic = true;
			currentPhaseCinematic = Cinematic::BACK;
		}
	}
}

void Player::ChangePosYPlayerCinematic(GameData& _gameData, float _moy1, float _moy2, float _moy3, float _moy4)
{
	switch (_gameData.tabPlayers.size())
	{
	case 1:
		posYCinematic = (_moy4 + _moy1) / 2;
		break;
	case 2:
		if (id == 0)
		{
			posYCinematic = (_moy2 + _moy4) / 2;
		}
		else if (id == 1)
		{
			posYCinematic = (_moy1 + _moy3) / 2;
		}
		break;
	case 3:
		if (id == 0)
		{
			posYCinematic = _moy4;
		}
		else if (id == 1)
		{
			posYCinematic = (_moy1 + _moy4) / 2;
		}
		else if (id == 2)
		{
			posYCinematic = _moy1;
		}
		break;
	case 4:
		if (id == 0)
		{
			posYCinematic = _moy4;
		}
		else if (id == 1)
		{
			posYCinematic = _moy3;
		}
		else if (id == 2)
		{
			posYCinematic = _moy2;
		}
		else if (id == 3)
		{
			posYCinematic = _moy1;
		}
		break;
	}
}

void Player::UpdateCinematic(GameData& _gameData)
{
	if (_gameData.levelState == LEVEL3)
	{
		if (_gameData.inCinematic)
		{
			if (GetState() != RUN)
			{
				posInAir = pos;
				inAir = false;
				SetPlayerState(RUN);
			}
			playerRunState->PlayerRunStateRun(_gameData.dt, *_gameData.audio, _gameData.inCinematic);

			if (currentPhaseCinematic == Cinematic::BACK)
			{
				if (currentEassing == 1)
				{
					if (!posAssigned)
					{
						posAssigned = true;
						timerCinematic = 0.f;
						startPosCinematic = pos;

						if (pos.x < 7321)
						{
							endPosCinematic = { 7321.f, 3427.f };
						}
						else
						{
							endPosCinematic = { 8225.f, 3163.f };
						}

					}

					if (timerCinematic <= 1.f)
					{
						pos = Easings::Lerp(startPosCinematic, endPosCinematic, timerCinematic);
						timerCinematic += _gameData.dt / 3;
					}
					else
					{
						currentEassing = 2;
						posAssigned = false;
						pos = { 7610.f, (1655.f + (float)(id * 10)) };
					}
				}
				else if (currentEassing == 2)
				{
					if (!posAssigned)
					{
						posAssigned = true;
						timerCinematic = 0.f;
						startPosCinematic = pos;

						posYMoy4 = 1655.f;
						posYMoy3 = 1645.f;
						posYMoy2 = 1635.f;
						posYMoy1 = 1625.f;

						ChangePosYPlayerCinematic(_gameData, posYMoy1, posYMoy2, posYMoy3, posYMoy4);

						endPosCinematic = { 7275.f, posYCinematic };
						lastDir = LEFT;
					}

					if (timerCinematic <= 1.f)
					{
						pos = Easings::Lerp(startPosCinematic, endPosCinematic, timerCinematic);
						timerCinematic += _gameData.dt / 1.3f;
					}
					else
					{
						currentEassing = 1;
						posAssigned = false;
						currentPhaseCinematic = Cinematic::FRONT;
					}
				}
			}
			else if (currentPhaseCinematic == Cinematic::FRONT)
			{
				if (currentEassing == 1)
				{
					if (!posAssigned)
					{
						posAssigned = true;
						timerCinematic = 0.f;
						startPosCinematic = pos;

						posYMoy4 = 2034.f;
						posYMoy3 = 1984.f;
						posYMoy2 = 1934.f;
						posYMoy1 = 1884.f;

						ChangePosYPlayerCinematic(_gameData, posYMoy1, posYMoy2, posYMoy3, posYMoy4);

						endPosCinematic = { 7218.f, posYCinematic };
						lastDir = RIGHT;
					}

					if (timerCinematic <= 1.f)
					{
						pos = Easings::Lerp(startPosCinematic, endPosCinematic, timerCinematic);
						timerCinematic += _gameData.dt / 2.f;
					}
					else
					{
						currentEassing = 2;
						posAssigned = false;
					}
				}
				else if (currentEassing == 2)
				{
					if (!posAssigned)
					{
						posAssigned = true;
						timerCinematic = 0.f;
						startPosCinematic = pos;

						posYMoy4 = 1695.f;
						posYMoy3 = 1645.f;
						posYMoy2 = 1595.f;
						posYMoy1 = 1545.f;

						ChangePosYPlayerCinematic(_gameData, posYMoy1, posYMoy2, posYMoy3, posYMoy4);

						endPosCinematic = { 8028.f, posYCinematic };
					}

					if (timerCinematic <= 1.f)
					{
						pos = Easings::Lerp(startPosCinematic, endPosCinematic, timerCinematic);
						timerCinematic += _gameData.dt / 3.f;
					}
					else
					{
						currentEassing = 1;
						posAssigned = false;
						_gameData.inCinematic = false;
					}
				}
			}
		}

		if (freefallStartCinematic)
		{
			if (timerEasingfreefallCinematic <= 1.f)
			{
				/*Easings::Lerp*/
			}
		}
		
		if (freefallEndCinematic)
		{

		}
	}
}

void Player::LoadPlayerAtBegening(void)
{
	isLoading = true;
	playerIdleState->Init();
	auraSprite = allSprite->playerSprite[playerSelected][(int)AllSprite::AURA];
	auraParticuleSprite = allSprite->playerSprite[playerSelected][(int)AllSprite::AURA_PARTICLE];
	auraAnime.CreateLeftToRight(auraSprite, 20, 2, auraSprite.getTexture()->getSize());
	auraParticuleAnime.CreateLeftToRight(auraParticuleSprite, 20, 2, auraParticuleSprite.getTexture()->getSize());
	sf::IntRect rect = sf::IntRect(0, 0, auraSprite.getTexture()->getSize().x / 20, auraSprite.getTexture()->getSize().y / 2);
	sf::IntRect rect2 = sf::IntRect(0, 0, auraParticuleSprite.getTexture()->getSize().x / 20, auraParticuleSprite.getTexture()->getSize().y / 2);
	auraSprite.setTextureRect(rect);
	auraParticuleSprite.setTextureRect(rect2);
}

void Player::UpdatePlayerSound(GameData& _gameData)
{
	if (state == RUN)
	{
		if (timerSound > 0.45f)
		{
			//random pitch between -0.1f and 0.1f
			float randomPitch = (rand() / (float)RAND_MAX) / 5 - 0.1f;
			_gameData.audio->SetPitch("WALK_PLAYER_FIRST_FEET", 1 + randomPitch);
			_gameData.audio->PlaySoundRepeated("WALK_PLAYER_FIRST_FEET", 100.f);
			timerSound = 0.f;
		}
	}
	else if (state == JUMP)
	{
		if (hasLanded)
		{
			_gameData.audio->PlaySoundNotRepeated("PLAYER_" + color[GetPlayerSelected()] + "_JUMP_SCREAM_1", 100.f);

			hasLanded = false;
		}
	}
	else if (state == HIT_GROUND)
	{
		_gameData.audio->PlaySoundNotRepeated("PLAYER_" + color[GetPlayerSelected()] + "_LAND_SCREAM_1", 100.f);

		hasLanded = true;
	}
}

void Player::UpdateInventory(float _dt)
{
	if (useItem)
	{
		if (hasAPotion)
		{
			hasAPotion = false;
			life = maxLife;
			potionIsHeal = true;
			potionEffect = Utils::LoadSprite("Assets/Images/Entities/Players/PotionEffect/Life.png", CENTER_FOOT, 12, 1);
			potionAnime.CreateLeftToRight(potionEffect, 12, 1, potionEffect.getTexture()->getSize());
		}
		else if (hasASpeedBuff)
		{
			hasASpeedBuff = false;
			stats[(int)Stats::AGILITY] += 3;
			hasUseSpeedBuff = true;
			potionIsHeal = false;
			potionEffect = Utils::LoadSprite("Assets/Images/Entities/Players/PotionEffect/Speed.png", CENTER_FOOT, 19, 1);
			potionAnime.CreateLeftToRight(potionEffect, 19, 1, potionEffect.getTexture()->getSize());
		}
		else if (hasAStrengthBuff)
		{
			hasAStrengthBuff = false;
			stats[(int)Stats::STRENGTH] += 3;
			hasUseStrengthBuff = true;
			potionIsHeal = false;
			potionEffect = Utils::LoadSprite("Assets/Images/Entities/Players/PotionEffect/Strength.png", CENTER_FOOT, 19, 1);
			potionAnime.CreateLeftToRight(potionEffect, 19, 1, potionEffect.getTexture()->getSize());
		}
		else if (hasAMagicBuff)
		{
			hasAMagicBuff = false;
			stats[(int)Stats::WISDOM] += 3;
			hasUseMagicBuff = true;
			potionIsHeal = false;
			potionEffect = Utils::LoadSprite("Assets/Images/Entities/Players/PotionEffect/Magic.png", CENTER_FOOT, 19, 1);
			potionAnime.CreateLeftToRight(potionEffect, 19, 1, potionEffect.getTexture()->getSize());
		}
		useItem = false;
	}

	if (timerItemUse > 0.f || potionIsHeal)
	{
		showPotionAnime = true;
	}
	else
	{
		showPotionAnime = false;
	}

	if ((hasUseSpeedBuff || hasUseStrengthBuff || hasUseMagicBuff) && timerItemUse < 30.f)
	{
		timerItemUse += _dt;
	}
	else if (hasUseSpeedBuff && timerItemUse >= 30.f)
	{
		hasUseSpeedBuff = false;
		stats[(int)Stats::AGILITY] -= 3;
		timerItemUse = 0.f;
	}
	else if (hasUseStrengthBuff && timerItemUse >= 30.f)
	{
		hasUseStrengthBuff = false;
		stats[(int)Stats::STRENGTH] -= 3;
		timerItemUse = 0.f;
	}
	else if (hasUseMagicBuff && timerItemUse >= 30.f)
	{
		hasUseMagicBuff = false;
		stats[(int)Stats::WISDOM] -= 3;
		timerItemUse = 0.f;
	}

	if (showPotionAnime)
	{
		bool temp = true;
		if (potionIsHeal)
		{
			if (potionAnime.currentFrame == potionAnime.maxFrame)
			{
				potionIsHeal = false;
				showPotionAnime = false;
			}
			potionAnime.UpdateLeftToRight(potionEffect, _dt, 10, 0, temp, false);
		}
		else
		{
			if (potionAnime.currentFrame == potionAnime.maxFrame)
			{
				potionAnime.currentFrame = 7;
			}
			potionAnime.UpdateLeftToRight(potionEffect, _dt, 10, 0, temp, true);
		}
	}
}

void Player::UpdateViperAnime(float _dt)
{
	if (viperSpellIsActive)
	{
		bool temp = true;
		animeAOE.UpdateLeftToRight(spriteAOE, _dt, 8, compteurLineAOEMagic, temp, false);

		if (animeAOE.currentFrame >= 3 && compteurLineAOEMagic == 0)
		{
			hitBoxAOE.isActive = true;
		}

		if (animeAOE.currentFrame == 5 && mana > 0 && compteurLongMagickManaConsume < 3)
		{
			animeAOE.currentFrame = 0;
			compteurLineAOEMagic++;
		}

		if (compteurLineAOEMagic == 7 && animeAOE.currentFrame == 3 && mana > 0 && compteurLongMagickManaConsume < 3)
		{
			animeAOE.currentFrame = 4;
			compteurLineAOEMagic = 0;
		}

		if (mana <= 0 || compteurLongMagickManaConsume >= 3)
		{
			if (hitBoxAOE.isActive)
			{
				compteurLineAOEMagic = 7;
				animeAOE.currentFrame = 1;
			}
			hitBoxAOE.isActive = false;
		}

		if (compteurLineAOEMagic == 7 && animeAOE.currentFrame == 5)
		{
			viperSpellIsActive = false;
		}
	}
}

void Player::UpdateIceSpellAnime(float _dt)
{
	if (iceSpellIsActive)
	{
		bool temp = true;
		animeAOE.UpdateLeftToRight(spriteAOE, _dt, 8, compteurLineAOEMagic, temp, false);

		if (animeAOE.currentFrame >= 3 && compteurLineAOEMagic == 0)
		{
			hitBoxAOE.isActive = true;
		}

		if (animeAOE.currentFrame == 5 && mana > 0 && compteurLongMagickManaConsume < 3)
		{
			animeAOE.currentFrame = 0;
			compteurLineAOEMagic++;
			if (compteurLineAOEMagic == 6)
			{
				animeAOE.currentFrame = 0;
				compteurLineAOEMagic = 5;
			}
		}

		if (mana <= 0 || compteurLongMagickManaConsume >= 3)
		{
			if (hitBoxAOE.isActive)
			{
				compteurLineAOEMagic = 7;
				animeAOE.currentFrame = 1;
			}
			hitBoxAOE.isActive = false;
		}

		if (compteurLineAOEMagic == 7 && animeAOE.currentFrame == 5)
		{
			iceSpellIsActive = false;
		}
	}
}

void Player::UpdateAnime(float _dt)
{
	bool temp = true;

	if (compteurLineAura == 0 && auraAnime.currentFrame == auraAnime.maxFrame)
	{
		compteurLineAura = 1;
		auraAnime.currentFrame = 0;
	}

	auraAnime.UpdateLeftToRight(auraSprite, _dt, 12, compteurLineAura, temp, true);
	auraParticuleAnime.UpdateLeftToRight(auraParticuleSprite, _dt, 12, compteurLineAura, temp, true);
}

void Player::UpdateViperHitBox(void)
{
	if (viperSpellIsActive)
	{
		if (hitBoxAOE.isActive)
		{
			hitBoxAOE.hitbox->setOutlineColor(sf::Color::Magenta);
		}
		else
		{
			hitBoxAOE.hitbox->setOutlineColor(sf::Color::White);
		}

		hitBoxAOE.hitbox->setSize({ 348.f,161.f });
	}
}

void Player::UpdateIceSpellHitBox(void)
{
	if (iceSpellIsActive)
	{
		if (hitBoxAOE.isActive)
		{
			hitBoxAOE.hitbox->setOutlineColor(sf::Color::Magenta);
		}
		else
		{
			hitBoxAOE.hitbox->setOutlineColor(sf::Color::White);
		}

		hitBoxAOE.hitbox->setSize({ spriteAOE.getTexture()->getSize().x / 6.f,spriteAOE.getTexture()->getSize().y / 8.f });
	}
}

void Player::DisplayViperSpell(sf::RenderWindow& _window)
{
	if (viperSpellIsActive)
	{
		Utils::BlitSprite(spriteAOE, hitBoxAOE.hitbox->getPosition(), 0, _window);
	}
}

void Player::DisplayViperSpell(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	if (viperSpellIsActive)
	{
		Utils::BlitSprite(spriteAOE, sf::Vector2f{ hitBoxAOE.hitbox->getPosition().x + (TROAD_GAP * (-1 * hitBoxAOE.hitbox->getPosition().y + WORKING_HEIGHT) / 100.f),hitBoxAOE.hitbox->getPosition().y }, 0, _red);
		Utils::BlitSprite(spriteAOE, sf::Vector2f{ hitBoxAOE.hitbox->getPosition().x - (TROAD_GAP * (-1 * hitBoxAOE.hitbox->getPosition().y + WORKING_HEIGHT) / 100.f),hitBoxAOE.hitbox->getPosition().y }, 0, _blue);
	}
}

void Player::DisplayIceSpell(sf::RenderWindow& _window)
{
	if (iceSpellIsActive)
	{
		if (hitBoxAOE.dir == LEFT)
		{
			spriteAOE.setScale(-1.f, 1.f);
			spriteAOE.setPosition(hitBoxAOE.hitbox->getPosition().x + spriteAOE.getTexture()->getSize().x / 6.f, hitBoxAOE.hitbox->getPosition().y);
		}
		else
		{
			spriteAOE.setScale(1.f, 1.f);
			spriteAOE.setPosition(hitBoxAOE.hitbox->getPosition());
		}
		Utils::BlitSprite(spriteAOE, spriteAOE.getPosition(), 0, _window);
	}
}

void Player::DisplayIceSpell(sf::RenderTexture& _textureRed, sf::RenderTexture& _textureBlue)
{
	if (iceSpellIsActive)
	{
		if (hitBoxAOE.dir == LEFT)
		{
			spriteAOE.setScale(-1.f, 1.f);
			spriteAOE.setPosition(hitBoxAOE.hitbox->getPosition().x + spriteAOE.getTexture()->getSize().x / 6.f, hitBoxAOE.hitbox->getPosition().y);
		}
		else
		{
			spriteAOE.setScale(1.f, 1.f);
			spriteAOE.setPosition(hitBoxAOE.hitbox->getPosition());
		}

		Utils::BlitSprite(spriteAOE, sf::Vector2f{ spriteAOE.getPosition().x + (TROAD_GAP * (-1 * spriteAOE.getPosition().y + WORKING_HEIGHT) / 100.f),spriteAOE.getPosition().y }, 0, _textureRed);
		Utils::BlitSprite(spriteAOE, sf::Vector2f{ spriteAOE.getPosition().x - (TROAD_GAP * (-1 * spriteAOE.getPosition().y + WORKING_HEIGHT) / 100.f),spriteAOE.getPosition().y }, 0, _textureBlue);
	}
}

void Player::Input(Audio& _audio, float _timer)
{
	if (sf::Joystick::isConnected(id))
	{
		if (sf::Joystick::getIdentification(id).name == "Wireless Controller" && _timer <= 0)
		{
			if (sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonPS4::R2))
			{
				auraIsActived = true;
			}
			else
			{
				auraIsActived = false;
			}

			if (!sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonPS4::SQUARE) && !lightAttackButtonRealeased)
			{
				lightAttackButtonRealeased = true;
			}
			if (!sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonPS4::TRIANGLE) && !heavyAttackButtonRealeased)
			{
				heavyAttackButtonRealeased = true;
			}
			if (!sf::Joystick::isAxisPressed(id, sf::Joystick::TRIGGERS_L2_R2, sf::Joystick::PLUS) && !parryButtonReleased)
			{
				parryButtonReleased = true;
			}
			if (!sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonPS4::SQUARE) && !magicAttackButtonReleased && auraIsActived)
			{
				magicAttackButtonReleased = true;
			}
			if (!sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonPS4::TRIANGLE) && !magicAttackButtonReleased2 && auraIsActived)
			{
				magicAttackButtonReleased2 = true;
			}
			if (!sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonPS4::L1) && !useItemButtonReleased)
			{
				useItemButtonReleased = true;
			}
			else if (sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonPS4::L1) && useItemButtonReleased)
			{
				useItemButtonReleased = false;
				useItem = true;
			}
			if (!sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonPS4::CROSS) && !JumpButtonReleased)
			{
				JumpButtonReleased = true;
			}
		}
		else
		{
			if (sf::Joystick::isAxisPressed(id, sf::Joystick::TRIGGERS_L2_R2, sf::Joystick::MINUS))
			{
				auraIsActived = true;
			}
			else
			{
				auraIsActived = false;
				compteurLineAura = 0;
			}

			if (!sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonXBOX::X) && !lightAttackButtonRealeased)
			{
				lightAttackButtonRealeased = true;
			}
			if (!sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonXBOX::Y) && !heavyAttackButtonRealeased)
			{
				heavyAttackButtonRealeased = true;
			}

			if (!sf::Joystick::isAxisPressed(id, sf::Joystick::TRIGGERS_L2_R2, sf::Joystick::MINUS) && !parryButtonReleased)
			{
				parryButtonReleased = true;
			}
			if (!sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonXBOX::X) && !magicAttackButtonReleased && auraIsActived)
			{
				magicAttackButtonReleased = true;
			}
			if (!sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonXBOX::Y) && !magicAttackButtonReleased2 && auraIsActived)
			{
				magicAttackButtonReleased2 = true;
			}
			if (!sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonXBOX::LB) && !useItemButtonReleased)
			{
				useItemButtonReleased = true;
			}
			else if (sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonXBOX::LB) && useItemButtonReleased)
			{
				useItemButtonReleased = false;
				useItem = true;
			}
			if (!sf::Joystick::isButtonPressed(id, (unsigned int)ControllerButtonXBOX::A) && !JumpButtonReleased)
			{
				JumpButtonReleased = true;
			}
		}
	}
}

int Player::GetID()
{
	return this->id;
}

void Player::SetID(int _id)
{
	this->id = _id;
}

bool Player::GetInAir()
{
	return this->inAir;
}

PlayerStates Player::GetState()
{
	return this->state;
}

void Player::SetInAir(bool _inAir)
{
	this->inAir = _inAir;
}

int Player::GetLevel()
{
	return this->level;
}

void Player::SetLevel(int _level)
{
	this->level = _level;
}

void Player::LevelUp()
{
	while (this->xp >= this->maxXp)
	{
		this->level++;
		this->skillPoints += 2;
		this->xp -= this->maxXp;
		this->maxXp = this->maxXp + this->level * 20;
	}
}

void Player::SetPlayerState(PlayerStates _state)
{
	lastState = state;
	state = _state;

	switch (state)
	{
	case RUN:
		playerRunState->Init();
		break;
	case JUMP:
		playerJumpState->Init();
		break;
	case IDLE:
		playerIdleState->Init();
		break;
	case LIGHTATTACK:
		playerLightAttackState->Init();
		break;
	case HEAVYATTACK:
		playerHeavyAttackState->Init();
		break;
	case BOWATTACK:
		playerBowAttackState->Init();
		break;
	case HIT:
		playerHitState->Init();
		break;
	case HIT_GROUND:
		playerHitGroundState->Init();
		break;
	case KNOCKDOWN:
		playerKnockDownState->Init();
		break;
	case DEAD:
		playerDeadState->Init();
		break;
	case PARRY:
		playerParryState->Init();
		break;
	case MAGICATTACK:
		playerMagicAttackState->Init();
		break;
	case PRAY:
		playerPrayState->Init();
		break;
	case REVIVE:
		playerReviveState->Init();
		break;
	case EAT_BY_BOSS:
		playerEatByBossState->Init();
		break;
	case FREEFALL:
		playerFreeFallState->Init();
		break;
	case DASH:
		playerDashState->Init();
		break;
	case TAUNT:
		playerTauntState->Init();
		break;
	}
}

sf::RectangleShape* Player::GetLightAttack()
{
	return &this->lightAttackHitbox;
}

sf::RectangleShape* Player::GetHeavyAttack()
{
	return &this->heavyAttackHitbox;
}

bool Player::GetLightAttackActived()
{
	return lightAttackHitboxActived;
}

bool Player::GetHeavyAttackActived()
{
	return heavyAttackHitboxActived;
}

bool Player::GetLightAttackButton()
{
	return lightAttackButtonRealeased;
}

bool Player::GetHeavyAttackButton()
{
	return heavyAttackButtonRealeased;
}

bool Player::GetMagicAttackButton()
{
	return magicAttackButtonReleased;
}

bool Player::GetParryState()
{
	return isParrying;
}

bool Player::GetCanBeRevive()
{
	return canBeRevive;
}

bool Player::GetHasFinishPray()
{
	return hasFinishPray;
}

sf::Sprite Player::GetSpriteAOE()
{
	return spriteAOE;
}

sf::Sprite& Player::GetSprite()
{
	return sprite;
}

PlayerStates Player::GetLastState()
{
	return lastState;
}

int Player::GetLastDir()
{
	return lastDir;
}

sf::Vector2f Player::GetPos()
{
	return pos;
}

sf::Vector2f Player::GetSpeed()
{
	return speed;
}

sf::Vector2f Player::GetPosInAir()
{
	return posInAir;
}

void Player::SetPosInAir(sf::Vector2f _posInAir)
{
	this->posInAir = _posInAir;
}

int Player::GetLife()
{
	return life;
}

int Player::GetMaxLife()
{
	return maxLife;
}

int Player::GetDamage()
{
	return damaged;
}

int Player::GetXp()
{
	return xp;
}

int Player::GetMaxXp()
{
	return maxXp;
}

int Player::GetMana()
{
	return mana;
}

int Player::GetMaxMana()
{
	return maxMana;
}

int Player::GetPlayerSelected()
{
	return this->playerSelected;
}

bool Player::GetTeam()
{
	return this->playerTeam;
}

float Player::GetParryTimer()
{
	return this->parryTimerSound;
}

int Player::GetComboNumber()
{
	return this->comboNumber;
}

void Player::SetLightAttackButton(bool _bool)
{
	this->lightAttackButtonRealeased = _bool;
}

void Player::SetHeavyAttackButton(bool _bool)
{
	this->heavyAttackButtonRealeased = _bool;
}

void Player::SetMagicAttackButton(bool _bool)
{
	this->magicAttackButtonReleased = _bool;
}

void Player::SetCanBeRevive(bool _bool)
{
	this->canBeRevive = _bool;
}

void Player::SetHasFinishPray(bool _bool)
{
	this->hasFinishPray = _bool;
}

void Player::SetLastState(PlayerStates _state)
{
	this->lastState = _state;
}

void Player::SetLastDir(int _dir)
{
	this->lastDir = _dir;
}

void Player::SetParryTimer(float _timer)
{
	this->parryTimerSound = _timer;
}

void Player::SetPlayerSelected(int _playerSelected)
{
	this->playerSelected = _playerSelected;
}

void Player::SetPlayerTeam(int _playerTeam)
{
	this->playerTeam = _playerTeam;
}

int Player::GetPlayerTeam(void)
{
	return this->playerTeam;
}

void Player::SetNbAttackInAir(int _nb)
{
	this->nbAttackInAir = _nb;
}

void Player::SetPropultionKnockDown(bool _bool)
{
	this->propultionKnockDown = _bool;
}

void Player::SetSpriteColor(sf::Color _color)
{
	this->sprite.setColor(_color);
}

void Player::SetShieldIsHit(bool _bool)
{
	if (this->timerShieldInvicibility <= 0.f)
	{
		this->shieldHisHit = _bool;
		this->timerShieldInvicibility = 0.4f;
	}
}

void Player::SetHeavyAttackGoUp(bool _bool)
{
	this->heavyAttackGoUp = _bool;
}

void Player::SetComboNumber(int _combo)
{
	this->comboNumber = _combo;
}

void Player::SetParryState(bool _bool)
{
	this->isParrying = _bool;
}

bool Player::GetHeavyAttackGoUp()
{
	return heavyAttackGoUp;
}

sf::Vector2f Player::GetViperSpellInfo()
{
	return sf::Vector2f{ (float)animeAOE.currentFrame, (float)compteurLineAOEMagic };
}

void Player::SetCanActiveHitbox(bool _bool)
{
	this->canActiveHitBox = _bool;
}

void Player::SetStatus(Status _status)
{
	this->status = _status;
	this->timerStatus = TIMERSTATUS;
}

void Player::DesactivateHitbox(void)
{
	this->lightAttackHitboxActived = false;
	this->heavyAttackHitboxActived = false;
	this->hitBoxAOE.isActive = false;
	this->iceSpellIsActive = false;
	this->viperSpellIsActive = false;
	this->isParrying = false;
}

void Player::ResetShieldLife(void)
{
	this->shieldLife = MAXSHIELDLIFE;
}

bool Player::GetAuraActivated()
{
	return this->auraIsActived;
}

void Player::SetAuraActivated(bool _isActivated)
{
	this->auraIsActived = _isActivated;
}

bool Player::GetMagicAttackAOE()
{
	return magicAttackIsDistance;
}

void Player::SetMagicAttackAOE(bool _isAOE)
{
	this->magicAttackIsDistance = _isAOE;
}

void Player::SetIceMagicAOE(bool _isIceActive)
{
	this->iceSpellIsActive = _isIceActive;
}

void Player::SetPoisonMagicAOE(bool _isPoisonActive)
{
	this->viperSpellIsActive = _isPoisonActive;
}

bool Player::GetShieldIsBreaking()
{
	return this->shieldIsBreaking;
}

void Player::SetShieldIsBreaking(bool _isBreaking)
{
	this->shieldIsBreaking = _isBreaking;
}

int Player::GetShieldLife()
{
	return this->shieldLife;
}

unsigned char Player::GetCurrentFrame()
{
	return this->currentAnime.currentFrame;
}

void Player::SetShieldLife(int _life)
{
	this->shieldLife = _life;
}

Status Player::GetStatus(void)
{
	return this->status;
}

AOEMagicAttack Player::GetAOEMagic()
{
	return this->hitBoxAOE;
}

void Player::SetAOEMagic(AOEMagicAttack _aoeMagic)
{
	this->hitBoxAOE = _aoeMagic;
}

bool Player::GetIceSpellHere()
{
	return this->iceSpellIsActive;
}

bool Player::GetPoisonSpellActive()
{
	return this->viperSpellIsActive;
}

bool Player::GetIdleParry(void)
{
	return this->parryIdle;
}

bool Player::GetRightParry(void)
{
	return this->parryRightFrontRun;
}

bool Player::GetLeftParry(void)
{
	return this->parryLeftFrontRun;
}

void Player::SetIdleParry(bool _bool)
{
	this->parryIdle = _bool;
}

void  Player::SetRightParry(bool _bool)
{
	this->parryRightFrontRun = _bool;
}

void Player::SetLeftParry(bool _bool)
{
	this->parryLeftFrontRun = _bool;
}

bool Player::GetMagicAttackDistance()
{
	return this->magicAttackIsDistance;
}

void Player::SetMagicAttackDistance(bool _isDistance)
{
	this->magicAttackIsDistance = _isDistance;
}

bool Player::GetPropulstionKnockDown(void)
{
	return this->propultionKnockDown;
}

bool Player::GetIsManaRecoveringFinished()
{
	return this->isManaRecoveringFinished;
}

bool Player::GetCanKnockDownHit(void)
{
	return this->canKnockDownHit;
}

bool Player::GetPVPHeavyAttackDown(void)
{
	return this->pvpHeavyAttackDown;
}
void Player::SetPVPHeavyAttackDown(bool _var)
{
	this->pvpHeavyAttackDown = _var;
}

bool Player::GetIsSrinks(void)
{
	return this->playerIsShrinks;
}

bool Player::GetIsDrawnBehindTree(void)
{
	if (currentPhaseCinematic == Cinematic::FRONT)
	{
		return false;
	}
	else if (currentPhaseCinematic == Cinematic::BACK)
	{
		return true;
	}
}

bool Player::GetFreeFall(void)
{
	return this->freefallWave;
}

void Player::SetFreeFall(bool _bool)
{
	this->freefallWave = _bool;
	this->freefallStartCinematic = _bool;
	this->freefallEndCinematic = !_bool;
	this->startPosEasingfreefallCinematic = pos;
	(bool)(this->freefallEndCinematic) ? this->endPosEasingfreefallCinematic = { pos.x, pos.y + (float)(WORKING_HEIGHT * 1.5f) } : this->endPosEasingfreefallCinematic = { pos.x, pos.y + WORKING_HEIGHT };
}

int Player::GetRealSpeed(void)
{
	return (int)(realSpeed.x + GetAgility() * 20);
}

float Player::GetTimerInRed(void)
{
	return timerInRed;
}

////////////////////////Player Stats Functions/////////////////////////////////

void Player::AddStrength()
{
	if (this->skillPoints >= 1 && this->stats[(int)Stats::STRENGTH] < 10)
	{
		this->skillPoints--;
		(this->stats[(int)Stats::STRENGTH])++;
	}
}

void Player::RemoveStrength()
{
	if (this->stats[(int)Stats::STRENGTH] > 1 && this->stats[(int)Stats::STRENGTH] <= 10)
	{
		this->skillPoints++;
		(this->stats[(int)Stats::STRENGTH])--;
	}
}

void Player::AddWisdom()
{
	if (this->skillPoints >= 1 && this->stats[(int)Stats::WISDOM] < 10)
	{
		this->skillPoints--;
		(this->stats[(int)Stats::WISDOM])++;

		if (this->stats[(int)Stats::WISDOM] >= 10)
		{
			this->maxMana = BASE_MAX_MANA + 2;
			this->AOEMagicUnlock = true;
		}
		else if (this->stats[(int)Stats::WISDOM] >= 5 && this->stats[(int)Stats::WISDOM] < 10)
		{
			this->maxMana = BASE_MAX_MANA + 1;
			this->AOEMagicUnlock = true;
		}
		else if (this->stats[(int)Stats::WISDOM] < 5)
		{
			this->AOEMagicUnlock = false;
		}
	}
	this->mana = maxMana;
}

void Player::RemoveWisdom()
{
	if (this->stats[(int)Stats::WISDOM] > 1 && this->stats[(int)Stats::WISDOM] <= 10)
	{
		this->skillPoints++;
		(this->stats[(int)Stats::WISDOM])--;

		if (this->stats[(int)Stats::WISDOM] >= 10)
		{
			this->maxMana = BASE_MAX_MANA + 3;
			this->AOEMagicUnlock = true;
		}
		else if (this->stats[(int)Stats::WISDOM] >= 5 && this->stats[(int)Stats::WISDOM] < 10)
		{
			this->maxMana = BASE_MAX_MANA + 1;
			this->AOEMagicUnlock = true;
		}
		else if (this->stats[(int)Stats::WISDOM] < 5)
		{
			this->maxMana = BASE_MAX_MANA;
			this->AOEMagicUnlock = false;
		}
	}
	this->mana = maxMana;
}

void Player::AddAgility()
{
	if (this->skillPoints >= 1 && this->stats[(int)Stats::AGILITY] < 10)
	{
		this->skillPoints--;
		(this->stats[(int)Stats::AGILITY])++;
	}
}

void Player::RemoveAgility()
{
	if (this->stats[(int)Stats::AGILITY] > 1 && this->stats[(int)Stats::AGILITY] <= 10)
	{
		this->skillPoints++;
		(this->stats[(int)Stats::AGILITY])--;
	}
}

void Player::AddVigor()
{
	if (this->skillPoints >= 1 && this->stats[(int)Stats::VIGOR] < 10)
	{
		this->skillPoints--;
		(this->stats[(int)Stats::VIGOR])++;
		if (this->stats[(int)Stats::VIGOR] < 5)
		{
			this->maxLife += 1;
		}
		if (this->stats[(int)Stats::VIGOR] >= 5 && this->stats[(int)Stats::VIGOR] <= 10)
		{
			this->maxLife += 3;
		}
		else if (this->stats[(int)Stats::VIGOR] >= 10)
		{
			this->maxLife += 6;
		}
	}
	this->life = maxLife;
}

void Player::RemoveVigor()
{
	if (this->stats[(int)Stats::VIGOR] > 1 && this->stats[(int)Stats::VIGOR] <= 10)
	{
		this->skillPoints++;
		(this->stats[(int)Stats::VIGOR])--;
		if (this->stats[(int)Stats::VIGOR] < 5)
		{
			this->maxLife -= 1;
		}
		if (this->stats[(int)Stats::VIGOR] >= 5 && this->stats[(int)Stats::VIGOR] <= 10)
		{
			this->maxLife -= 3;
		}
		else if (this->stats[(int)Stats::VIGOR] >= 10)
		{
			this->maxLife -= 6;
		}
	}
	this->life = maxLife;
}

void Player::AddSkillPoints()
{
	this->skillPoints++;
}

void Player::SetStatsValue(void)
{
	maxLife = 24;
	if (this->stats[(int)Stats::VIGOR] == 1)
	{
		this->maxLife = 24;
	}
	else if (this->stats[(int)Stats::VIGOR] > 1)
	{
		for (int i = 1; i <= (int)(this->stats[(int)Stats::VIGOR]); i++)
		{
			if (i < 5)
			{
				this->maxLife += 1;
			}
			if (i >= 5 && i <= 10)
			{
				this->maxLife += 3;
			}
			else if (i >= 10)
			{
				this->maxLife += 6;
			}
		}
	}

	if (this->stats[(int)Stats::WISDOM] >= 10)
	{
		this->maxMana = BASE_MAX_MANA + 3;
		this->AOEMagicUnlock = true;
	}
	else if (this->stats[(int)Stats::WISDOM] >= 5 && this->stats[(int)Stats::WISDOM] < 10)
	{
		this->maxMana = BASE_MAX_MANA + 1;
		this->AOEMagicUnlock = true;
	}
	else if (this->stats[(int)Stats::WISDOM] < 5)
	{
		this->AOEMagicUnlock = false;
	}

	this->mana = maxMana;
	this->life = maxLife;
}

int Player::GetStrength()
{
	return this->stats[(int)Stats::STRENGTH];
}

int Player::GetStrengthDamage()
{
	if (this->stats[(int)Stats::STRENGTH] == 1)
	{
		return 10;
	}
	else if (this->stats[(int)Stats::STRENGTH] < 5)
	{
		return 10 + this->stats[(int)Stats::STRENGTH] * 2;
	}
	else if (this->stats[(int)Stats::STRENGTH] >= 5 && this->stats[(int)Stats::STRENGTH] < 10)
	{
		return 10 + this->stats[(int)Stats::STRENGTH] * 4;
	}
	else if (this->stats[(int)Stats::STRENGTH] >= 10)
	{
		return 10 + this->stats[(int)Stats::STRENGTH] * 6;
	}
}

int Player::GetWisdom()
{
	return this->stats[(int)Stats::WISDOM];
}

int Player::GetAgility()
{
	return this->stats[(int)Stats::AGILITY];
}

int Player::GetVigor()
{
	return this->stats[(int)Stats::VIGOR];
}

void Player::SetStrength(int _strength)
{
	this->stats[(int)Stats::STRENGTH] = _strength;
}

void Player::SetWisdom(int _wisdom)
{
	this->stats[(int)Stats::WISDOM] = _wisdom;
}

void Player::SetAgility(int _agility)
{
	this->stats[(int)Stats::AGILITY] = _agility;
}

void Player::SetVigor(int _vigor)
{
	this->stats[(int)Stats::VIGOR] = _vigor;
}

int* Player::GetStats()
{
	return this->stats;
}

int Player::GetSkillPoints()
{
	return this->skillPoints;
}

void Player::SetSkillPoints(int _skillPoints)
{
	this->skillPoints = _skillPoints;
}

////////////////////////Player Iventory Functions/////////////////////////////////
bool Player::GetHasPotion()
{
	return hasAPotion;
}

bool Player::GetHasSpeedBuff()
{
	return hasASpeedBuff;
}

bool Player::GetHasStrengthBuff()
{
	return hasAStrengthBuff;
}

bool Player::GetHasMagicBuff()
{
	return hasAMagicBuff;
}

void Player::SetHasPotion(bool _bool)
{
	hasAPotion = _bool;
}

void Player::SetHasSpeedBuff(bool _bool)
{
	hasASpeedBuff = _bool;
}

void Player::SetHasStrengthBuff(bool _bool)
{
	hasAStrengthBuff = _bool;
}

void Player::SetHasMagicBuff(bool _bool)
{
	hasAMagicBuff = _bool;
}