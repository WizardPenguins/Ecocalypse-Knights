#include "stdafx.h"
#include "Entity.h"

sf::Vector2f Entity::GetPos()
{
	return this->pos;
}

int Entity::GetXp()
{
	return this->xp;
}

int Entity::GetMaxXp()
{
	return this->maxXp;
}

int Entity::GetDamage()
{
	return this->damaged;
}

int Entity::GetLife()
{
	return this->life;
}

int Entity::GetLastLife()
{
	return this->lastLife;
}

int Entity::GetMaxLife()
{
	return this->maxLife;
}

int Entity::GetGold()
{
	return this->gold;
}

int Entity::GetLastGold()
{
	return this->lastGold;
}

void Entity::AddGold(int _gold)
{
	this->gold += _gold;
}

void Entity::SetPos(sf::Vector2f _pos)
{
	this->pos = _pos;
}

void Entity::AddXp(int _xp)
{
	this->xp += _xp;
}

void Entity::SetXp(int _xp)
{
	this->xp = _xp;
}

void Entity::SetMaxXp(int _maxXp)
{
	this->maxXp = _maxXp;
}

void Entity::AddLife(int _life)
{
	this->life -= _life;
}

void Entity::AddDamage(int _damage)
{
	this->damaged += _damage;
}

void Entity::AddSpeed(float _speed)
{
	this->speed.x += _speed;
	this->speed.y += _speed/3;
}

void Entity::SetLife(int _life)
{
	this->life = _life;
}

void Entity::SetLastLife(void)
{
	this->lastLife = this->life;
}

void Entity::SetLastGold(void)
{
	this->lastGold = this->gold;
}

void Entity::SetMaxLife(int _maxLife)
{
	this->maxLife = _maxLife;
}

sf::RectangleShape* Entity::GetHitBox()
{
	return this->hitbox;
}

int Entity::GetMana()
{
	return this->mana;
}

int Entity::GetMaxMana()
{
	return this->maxMana;
}

void Entity::SetMana(int _manaNbr)
{
	this->mana = _manaNbr;
}

void Entity::SetMaxMana(int _maxMana)
{
	this->maxMana = _maxMana;
}

void Entity::SetSpeed(sf::Vector2f _speed)
{
	this->speed = _speed;
}

void Entity::SetPosInAir(sf::Vector2f _pos)
{
	this->posInAir = _pos;
}

void Entity::SetDamage(int _damage)
{
	this->damaged = _damage;
}

void Entity::SetGold(int _gold)
{
	this->gold = _gold;
}