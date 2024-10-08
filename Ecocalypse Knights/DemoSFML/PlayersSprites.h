#ifndef PLAYER_SPRITES_H
#define PLAYER_SPRITES_H

#include "stdafx.h"
constexpr int NB_PLAYERS = 4;

enum class AllSprite
{
	IDLE,
	ATTACK1,
	ATTACK2,
	ATTACK3,
	DEATH,
	HEAVY_ATTACK_UP,
	HEAVY_ATTACK_DOWN,
	HIT,
	HIT_GROUND,
	JUMP,
	MAGIC,
	MAGIC_DISTANCE,
	MAGIC_AOE,
	PARRY,
	REVIVE,
	PRAY,
	RUN,
	BOW,
	ARROW,
	AURA,
	AURA_PARTICLE,
	KNOCKDOWN,
	SHIELDSPRITE,
	DASH,
	TAUNT,
	NB_SPRITES
};

struct PlayersSprites
{
	sf::Sprite playerSprite[NB_PLAYERS][(int)AllSprite::NB_SPRITES] = {};
	void LoadSprites(void);
};

#endif // !PLAYER_SPRITES_H



