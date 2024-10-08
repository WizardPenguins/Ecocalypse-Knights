#include "stdafx.h"
#include "Common.h"
#include "Animation.h"
#include "Entity.h"

#include "MobSprite.hpp"
#include "PlayersSprites.h"
#include "Upgrade.h"

void Projectile::Update(float _dt)
{
	if (asAnime)
	{
		bool temp = true;
		projectileAnime->UpdateLeftToRight(sprite, _dt, speedAnime, 0, temp, false);
	}
	pos.x -= _dt * PROJECTILE_SPEED * direction;
}

GameData::GameData()
{
	//font
	GameFont = new sf::Font();
	GameFont->loadFromFile("Assets/Fonts/RulesFont.otf");
}

void GameData::InitSprite()
{
	//status
	this->statusSprite = new StatusSprite;
	this->statusSprite->InitAnimStatus();

	//HazelNut
	this->hazelSprite = new HazelSprite;
	this->hazelSprite->InitAnimHazel();

	//Mushroom
	this->mushroomSprite = new MushroomSprite;
	this->mushroomSprite->InitAnimMushroom();

	//Bird
	this->birdSprite = new BirdSprite;
	this->birdSprite->InitAnimBird();

	//scrarecrow
	this->scarecrowSprite = new ScarecrowSprite;
	this->scarecrowSprite->InitAnimScarecrow();

	//Pig
	this->pigSprite = new PigSprite;
	this->pigSprite->InitAnimPig();

	//boar
	this->boarSprite = new BoarSprite;
	this->boarSprite->InitAnimBoar();

	//Witch
	this->witchSprite = new WitchSprite;
	this->witchSprite->InitAnimWitch();

	//player
	this->playersSprite = new PlayersSprites;
	this->playersSprite->LoadSprites();

	//HUD
	this->heartHit = Utils::LoadSprite("Assets/Images/Hud/QuarterHeart.png", CENTER);
	this->commingSoon = Utils::LoadSprite("Assets/Images/Menu/ComingSoon.png", TOP_LEFT);

	//circle invoc into wave
	InitCircleInvoc();
	
	//Flamme
	InitFlame();

	//Inventory
	InitSpriteInventory();

	this->leaf = Utils::LoadSprite("Assets/Images/Background/Map3/LeafParticle.png", CENTER);
	this->corruption = Utils::LoadSprite("Assets/Images/Background/Map3/CorruptParticle.png", CENTER);
}

void GameData::InitFlame()
{
	this->flame = Utils::LoadSprite("Assets/Images/Background/Anim/Fire.png", TOP_LEFT);
	this->animFlame = new Animation;
	sf::IntRect rect = sf::IntRect(0, 0, 1100 / 11, 188);
	this->flame.setTextureRect(rect);
	this->animFlame->CreateLeftToRight(this->flame, 11, 1, { 1100,188 });

	this->flame.setPosition(922, 715);
}

void GameData::InitCircleInvoc()
{
	this->circleInvoc = Utils::LoadSprite("Assets/Images/Entities/Mobs/Invoc.png", CENTER_FOOT, 10,1);
}

void GameData::InitSpriteInventory()
{
	spriteIconHealthPotion = Utils::LoadSprite("Assets/Images/hud/IconHealthPotion.png", TOP_LEFT);
	spriteIconSpeedBuffPotion = Utils::LoadSprite("Assets/Images/hud/IconSpeedBuffPotion.png", TOP_LEFT);
	spriteIconStrengthBuffPotion = Utils::LoadSprite("Assets/Images/hud/IconStrengthBuffPotion.png", TOP_LEFT);
	spriteIconMagicBuffPotion = Utils::LoadSprite("Assets/Images/hud/IconMagicBuffPotion.png", TOP_LEFT);
}

void GameData::ChangeState(unsigned char _newState)
{
	this->lastState = this->gameState;
	this->gameState = _newState;
	this->timerButton = TimerBetweenState;
}

void GameData::ChangeLevelState(LevelState _newLevelState)
{
	this->levelState = _newLevelState;
	this->timerButton = TimerBetweenState;
}