#ifndef MUSHROOM_HPP
#define MUSHROOM_HPP

#pragma region Parameters
#define TimerAttack 2.f
#define DistMinAttack 200.f
#define DistAxeY 30.f
#define SpeedFadeDeath 100
#define GoalPos 150.f
#define KnockbackForce 1.f
#define DistanceJumpY 200.f
#define TimerKnockBack 0.75f
#define SpeedEasingJump 2
#define SpeedEasingFall 2
#pragma endregion

#pragma region Includes
#include "stdafx.h"
#include "Mob.hpp"
#include "MobSprite.hpp"
#include "Utils.h"
#include "Easings.h"
#include "Audio.h"
#include "Animation.h"
#pragma endregion

class Mushroom
{
public:
	Mushroom(Mob* _mob, MushroomSprite* _sprite);
	~Mushroom() = default;

	void Update(std::vector<Player*>& _tabPlayer, float _dt, Wave _wave, Audio& _audio);
	void Display(sf::RenderWindow& _window);
	void Display(sf::RenderTexture& _red, sf::RenderTexture& _blue);

	void SetState(MushroomState _state);
	void SetDrawFx(bool _drawFx);
	MushroomState GetState();
	sf::Vector2f GetDimSprite();
	int GetTargetPlayer();
	void SetTargetplayer(int _newTarget);

private:
	void DisplayFx(sf::RenderWindow& _window);
	void DisplayEntity(sf::RenderWindow& _window);
	void DisplayStatus(sf::RenderWindow& _window);

	void DisplayFx(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayEntity(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayStatus(sf::RenderTexture& _red, sf::RenderTexture& _blue);

	void UpdateTargetPlayer(std::vector<Player*>& _player);
	void Move(sf::Vector2f _posPlayer, float _dt);
	void Attack();
	void Death(Audio& _audio);
	void DestroyAttack();
	void InitNewAnim();
	void UpdateAnim(float _dt);
	void UpdateDirection(sf::Vector2f _pos);
	void UpdateTimer(float _dt);
	bool CanAttack(sf::Vector2f _pos);
	void ChangeState(sf::Vector2f _pos);
	void UpdateDeath(float _dt);
	void UpdateKnockback(float _dt);
	void UpdateKnockdown(float _dt);
	void UpdatePosKnockback();
	void UpdateInUp(float _dt);
	void UpdateInFall(float _dt);
	void CreateAnimFx();
	void SetPositionAttack();

	Mob* mob;
	MushroomSprite sprite;
	MushroomState lastState;
	MushroomState state;

	sf::Vector2f posDeath;

	int targetPlayer;

	bool drawFx;

	float timerAttack;
};
#endif // !MUSHROOM_HPP