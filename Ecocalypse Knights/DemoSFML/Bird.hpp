#ifndef BIRD_HPP
#define BIRD_HPP

#pragma region Includes
#include "stdafx.h"
#include "Mob.hpp"
#include "Animation.h"
#include "Easings.h"
#include "Audio.h"
#pragma endregion

#pragma region Parameters
#define TimerAttack 3.f
#define DistAxeY 15.f
#define SpeedFadeDeath 100
#define TimerHit 0.25f
#pragma endregion

class Bird
{
public:
	Bird(Mob* _mob, BirdSprite* _birdSprite);
	~Bird() = default;

	void Update(std::vector<Player*>& _tabPlayer, std::vector<Projectile>& _tabProj, float _dt, Wave _wave, Audio& _audio);
	void Display(sf::RenderWindow& _window);
	void Display(sf::RenderTexture& _red, sf::RenderTexture& _blue);

	void SetState(BirdState _state);
	void SetDrawFx(bool _bool);
private:

	void InitNewAnim();
	void UpdateDirection(sf::Vector2f _pos);
	void UpdateAnim(float _dt);
	void DisplayFx(sf::RenderWindow& _window);
	void DisplayEntity(sf::RenderWindow& _window);
	void DisplayEntity(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayStatus(sf::RenderWindow& _window);
	void DisplayFx(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayStatus(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void IsDeath(Audio& _audio);
	void UpdateDeath(float _dt);
	void ChangeState(float _posPlayer);
	bool CanFire(float _player);
	void Move(sf::Vector2f _vec, float _dt);
	void UpdateIdle(float _posPlayerY, float _dt);
	void Fire(float _dt, std::vector<Projectile>& _projectile, Audio& _audio,std::vector<Player*>&);
	void UpdateTimer(float _dt);
	void UpdateHit(float _dt);
	void CreateAnimFx();

	Mob* mob;
	BirdSprite sprite;
	BirdState lastState;
	BirdState state;

	int targetPlayer;

	bool initFire;
	bool drawFx;

	float timerAttack;
	float timerHit;
};

#endif // !BIRD_HPP