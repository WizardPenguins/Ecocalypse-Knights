#ifndef ENTITY_H
#define ENTITY_H

#include "stdafx.h"
#include"Common.h"
#include "Utils.h"

#define BASE_MAX_LIFE 24
#define BASE_MAX_MANA 3

enum class RaceMob
{
	HazelNut,
	Bird,
	Mushroom,
	Pig,
	Boar,
	Witch,
};

enum class MagicUse
{
	None,
	Fire,
	Poison,
	Ice,
	Elec,
	Fuzzy,
};

class Entity
{
public:
	Entity() = default;
	~Entity() = default;

	sf::Vector2f GetPos();
	int GetXp();
	int GetMaxXp();
	int GetDamage();
	int GetLife();
	int GetLastLife();
	int GetMaxLife();
	int GetMana();
	int GetMaxMana();
	int GetGold();
	int GetLastGold();

	void SetMana(int _manaNbr);
	void SetMaxMana(int _manaNbr);
	void SetXp(int _xp);
	void SetMaxXp(int _xp);
	void SetLife(int _life);
	void SetLastLife(void);
	void SetMaxLife(int _life);
	void SetPos(sf::Vector2f _pos);
	void SetSpeed(sf::Vector2f _speed);
	void SetPosInAir(sf::Vector2f _pos);
	void SetDamage(int _damage);
	void SetGold(int _gold);
	void SetLastGold(void);

	void AddGold(int _gold);
	void AddXp(int _xp);
	void AddLife(int _life);
	void AddDamage(int _damage);
	void AddSpeed(float _speed);

	sf::RectangleShape* GetHitBox();

	virtual void Init(GameData& _gameData) {};
	virtual void Update(GameData& _gameData) {};
	virtual void Display(GameData& _gameData) {};
	virtual void Move(GameData& _gameData) {};
	virtual void UpdateHitBox() { this->hitbox->setPosition(this->pos); };
	virtual void UpdateAnim() {};

protected:
	sf::Vector2f pos;
	sf::Vector2f speed;
	sf::Vector2f parrySpeed;
	sf::Vector2f posInAir;
	int life = NULL;
	int lastLife = NULL;
	int maxLife = 24;
	int damaged = 1;
	int xp = 2;
	int maxXp = 100;
	sf::RectangleShape* hitbox;
	int mana = 3;
	int maxMana = 3;
	int gold = 0;
	int lastGold = gold;
};

#endif



