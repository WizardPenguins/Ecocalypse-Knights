#ifndef MOBSPRITE_HPP
#define MOBSPRITE_HPP

#include "stdafx.h"
#include "Animation.h"
#include "Entity.h"

#define NbStatus 5

struct SpriteAnim
{
	sf::Sprite sprite;
	sf::Vector2u animDim;
	sf::Vector2f nbAnim;
	float speed;
	bool loop;
};

struct StatusSprite
{
	Animation* anim;
	SpriteAnim spriteAnim[5] = {};

	void InitAnimStatus();
private: void InitStatus(SpriteAnim& _sprite, std::string _status);

private: std::string enumStatus[5] = { "Fire", "Poison","Ice","Electricity","Fuzzy"};
};

struct BoarSprite
{
	Animation* anim;
	Animation* animStun;
	SpriteAnim spriteAnim[(int)(BoarState::NbState)];
	SpriteAnim spriteStun;

	Animation* animSlash;
	SpriteAnim spriteSlash[2];//0 = horizon ::: 1 = verti
	float dir;
	sf::RectangleShape* slashHitBox;

	void InitAnimBoar();
private : void InitAllAnimBoar(SpriteAnim& _sprite, std::string _state);
private : void InitSlash(SpriteAnim& _sprite, std::string _state);
private : void InitSlash2(SpriteAnim& _sprite, std::string _state);
private : void InitStun(SpriteAnim& _sprite);

private : std::string enumMobState[(int)(BoarState::NbState)] = { "Idle", "Target","Back", "AttackNormal", "AttackH", "AttackV","AttackZenitsu", "Shield", "KnockBack", "Back", "Death"};
};

struct HazelSprite
{
	Animation* anim;
	Animation* animFx;
	SpriteAnim spriteAnim[(int)(HazelState::NbState)];
	SpriteAnim spriteFx;

	void InitAnimHazel();
	void InitCorruptedMob();
private: void InitAllAnimHazel(SpriteAnim& _sprite, std::string _state);
private: void InitFxAnim();
private: void InitAnimCorrupted(SpriteAnim& _sprite, std::string _state, std::string _race);

private: std::string enumMobState[(int)(HazelState::NbState)] = { "Idle","Target","Attack","Death","Hit","Hit","Hit","Hit" };

};

struct MushroomSprite
{
	Animation* anim;
	Animation* animFx;
	SpriteAnim spriteAnim[(int)(MushroomState::NbState)];
	SpriteAnim spriteFx;

	void InitAnimMushroom();
private: void InitAllAnimMushroom(SpriteAnim& _sprite, std::string _state);
private: void InitFxAnim();

private: std::string enumMobState[(int)(MushroomState::NbState)] = { "Idle","Target","Attack","Death","Hit","Hit","Hit","Hit" };

};

struct PigSprite
{
	Animation* anim;
	Animation* animFx;
	Animation* animFxHit;
	SpriteAnim spriteAnim[(int)(PigState::NbState)];
	SpriteAnim spriteFx;
	SpriteAnim spriteFxHit;

	void InitAnimPig();
private: void InitAllAnimPig(SpriteAnim& _sprite, std::string _state);
private: void InitFxAnim();
private: void InitFxHitAnim(SpriteAnim& _sprite);

private: std::string enumMobState[(int)(PigState::NbState)] = { "Target","Target","Death","Hit","Hit","Hit","Hit" };
};

struct BirdSprite
{
	Animation* anim;
	Animation* animProjectile;
	Animation* animFx;
	SpriteAnim spriteAnim[(int)(MushroomState::NbState)];
	SpriteAnim spriteAnimprojectile;
	SpriteAnim spriteFx;

	void InitAnimBird();
private: void InitAllAnimBird(SpriteAnim& _sprite, std::string _state);
private: void InitProjectile(SpriteAnim& _sprite);
private: void InitHitFx(SpriteAnim& _sprite);

private: std::string enumMobState[(int)(BirdState::NbState)] = { "Idle","Attack","Death","Hit" };

};

struct WitchSprite
{
	HazelSprite* hazelnutCorrupted;

	Animation* anim;
	SpriteAnim spriteAnim[(int)(WitchState::NbState)];

	Animation* animAttack;
	Animation* animFx;
	Animation* animInvoc;
	Animation* animScream;

	SpriteAnim spriteCircleInvoc;
	SpriteAnim spriteFxCircleInvoc;
	SpriteAnim spriteBrambleInvoc;
	SpriteAnim spriteCrystalInvoc[(int)CrystalState::NbState];
	SpriteAnim spriteCrystalHit;
	SpriteAnim spriteShield;
	SpriteAnim spritePlante;
	SpriteAnim spritePlanteDeath;
	SpriteAnim spriteScream;
	
	sf::RectangleShape* rectFx;

	int line = NULL; // only for the scream fx

	void InitAnimWitch();
private: void InitAllAnimWitch(SpriteAnim& _sprite, std::string _state);
private: void InitCircleInvoc(SpriteAnim& _sprite);
private: void InitCircleFxInvoc(SpriteAnim& _sprite);
private: void InitBrambleInvoc(SpriteAnim& _sprite);
private: void InitCrystalsInvoc(SpriteAnim& _sprite, int _i);
private: void InitShield(SpriteAnim& _sprite);
private: void InitHitCrystal(SpriteAnim& _sprite);
private: void InitSpritePlante(SpriteAnim& _sprite);
private: void InitSpritePlanteDeath(SpriteAnim& _sprite);
private: void InitScream(SpriteAnim& _sprite);
private: void InitCorruptedHazelnut(HazelSprite& _mobSprite);

private: std::string enumMobState[(int)(WitchState::NbState)] = { "Idle","Bramble","Bramble","Idle","Bramble","Scream","Death","Normal" };
};

struct ScarecrowSprite
{
	Animation* anim;
	SpriteAnim spriteAnim[3] = {};

	void InitAnimScarecrow();
private: void InitScarecrow(SpriteAnim& _sprite, std::string _state);

private: std::string enumState[3] = { "Idle", "Hit", "Death"};
};

#endif // !MOBSPRITE_HPP