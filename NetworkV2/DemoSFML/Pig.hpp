#ifndef PIG_HPP
#define PIG_HPP

#pragma region Parameters
#define TimerAttack 2.f
#define DistMinAttack 610.f
#define DistAxeY 30.f
#define FinishAttackBehindPlayer 400.f
#define SpeedFadeDeath 100
#define DistMinMove 2.f
#define GoalPos 500.f
#define KnockbackForce 1.f
#define DistanceJumpY 200.f
#define TimerKnockBack 0.75f
#define DeathPosMoveX 2.f
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

class Pig
{
public:
	Pig(Mob* mob,PigSprite* _sprite);
	~Pig() = default;

	void Update(std::vector<Player*>& _tabPlayer, float _dt, Wave _wave, Audio& _audio);
	void Display(sf::RenderWindow& _window);
	void Display(sf::RenderTexture& _red, sf::RenderTexture& _blue);

	void SetState(PigState _state);
	sf::Vector2f GetDimSprite();
	void SetDrawFx(bool _bool);
	void InitNewAnim();

private:
	void DisplayFx(sf::RenderWindow& _window);
	void DisplayEntity(sf::RenderWindow& _window);
	void DisplayStatus(sf::RenderWindow& _window);	
	
	void DisplayFx(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayEntity(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayStatus(sf::RenderTexture& _red, sf::RenderTexture& _blue);

	void UpdateInUp(float _dt);
	void UpdateInFall(float _dt);
	void UpdateKnockback(float _dt);
	void UpdateKnockdown(float _dt);
	void UpdatePosKnockback();
	void CreateAnimFx();
	void CreateAnimFxHit();
	void UpdateAnim(float _dt);
	void DestroyAttack();
	void Death(Audio& _audio);
	void UpdateDirection(sf::Vector2f _pos);
	void UpdateTargetPlayer(std::vector<Player*>& _player);
	void Move(sf::Vector2f _posPlayer, float _dt);
	void MoveAttack(sf::Vector2f _posPlayer, float _dt);
	void UpdateTimer(float _dt);
	void ChangeState(sf::Vector2f _pos);
	bool CanAttack(sf::Vector2f _pos);
	void UpdateDeath(float _dt);
	void Attack(float _dt);
	void EndAttack(sf::Vector2f _posPlayer);

	Mob* mob;

	PigSprite sprite;
	PigState lastState;
	PigState state;

	sf::Vector2f posDeath;
	sf::Vector2f posAttack;

	int targetPlayer;
	int ligneFx;

	bool drawFx;
	bool drawFx2;

	float timerAttack;
};

#endif // !PIG_HPP