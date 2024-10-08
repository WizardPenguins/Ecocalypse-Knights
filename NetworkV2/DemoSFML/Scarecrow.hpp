#ifndef SCARECROW_HPP
#define SCARECROW_HPP

#include "stdafx.h"
#include "Entity.h"
#include "MobSprite.hpp"

#define LifeScarecrow 1'000

enum class StateScarecrow
{
	Idle,
	Hit,
	Death,
};

class Scarecrow : public Entity
{
public:
	Scarecrow(ScarecrowSprite& _mobSprite, StatusSprite& _statusSprite, sf::Vector2f _pos);
	~Scarecrow() = default;

	void Display(sf::RenderWindow& _window, float _dt);
	void Display(sf::RenderTexture& _red, sf::RenderTexture& _blue, float _dt);

	sf::Vector2f GetPos();
	Status GetStatus();
	StateScarecrow GetState();
	bool GetInitStatus();
	void SetTimerStatus(float);
	void SetStatus(Status);
	void SetState(StateScarecrow);
	bool GetCanDelete();

private:
	void UpdateAnim(float _dt);
	void InitNewAnim();
	void InitNewAnimStatus();
	void UpdateHitBox();
	void UpdateHit();
	void UpdateDeath();
	void DeathAnim();

	void DisplayStatus(sf::RenderWindow& _window);
	void DisplayStatus(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void UpdateStatus(float _dt);


	ScarecrowSprite sprite;
	StatusSprite statusSprite;

	StateScarecrow state;
	StateScarecrow lastState;

	Status status;

	float timerStatus = 0.f;
	int tickDamage = 0;
	int ligne = 0;
	bool initStatus = false;
	bool canDelete = false;
};

#endif // !SCARECROW_HPP