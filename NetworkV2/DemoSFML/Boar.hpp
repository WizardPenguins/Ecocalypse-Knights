#ifndef BOAR_HPP
#define BOAR_HPP

#pragma region Parameters
#define TimerAttack 1.5f
#define TimerZenitsu 16.25f
#define TimerVerticalSlash 5.5f
#define TimerHorizontalSlash 5.f
#define TimerKnockdown 1.5f //after ultimate
#define TimerAfterZenitsu 0.75f //after boss ultimate to stay in the slash
#define TimerShield 3.f
#define DistMinAttack 225.f
#define DistAxeY 30.f
#define GoalPos 150.f
#define DistMinMove 5.f
#define MaxAttackShield 10
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

class Boar
{
public:
	Boar(Mob* _mob,BoarSprite* _sprite);
	~Boar() = default;

	void Update(std::vector<Player*>& _tabPlayers, float _dt, Wave _wave, Audio& _audio);
	void Display(sf::RenderWindow& _window);
	void Display(sf::RenderTarget& _red, sf::RenderTarget& _blue);
	void DisplaySlash(sf::RenderWindow& _window);
	void DisplaySlash(sf::RenderTarget& _red, sf::RenderTarget& _blue);

	bool GetSlashVertAlive();
	bool GetSlashHorizonAlive();
	bool GetDoZenitsu();
	BoarState GetState();
	sf::RectangleShape* GetSlash();
	sf::Vector2f GetSlashInfo();
	sf::Vector2f GetDimSprite(); // get the actual size of the sprite
	int GetTarget();
	void SetTarget(int _targetID);
	void SetLeak(bool _leak);
	sf::Vector2f GetPosSlashH();
	void AddNbAttack();
	bool OnLeft();

private:
	void DisplayEntity(sf::RenderWindow& _window);
	void DisplayStatus(sf::RenderWindow& _window);

	void DisplayEntity(sf::RenderTarget& _red, sf::RenderTarget& _blue);
	void DisplayStatus(sf::RenderTarget& _red, sf::RenderTarget& _blue);

	void DestroyAttack();
	void UpdateDirection(sf::Vector2f _pos);
	void UpdateTargetPlayer(std::vector<Player*>& _player);
	void Move(sf::Vector2f _posPlayer, float _dt, Audio& _audio);
	void UpdateTimer(float _dt);
	void ChangeState(std::vector<Player*>& _player, Audio& _audio);
	bool CanAttack(sf::Vector2f _pos);
	void UpdateTargetFirstPhase(std::vector<Player*>& _player, float _dt, Audio& _audio);
	void UpdateTargetSecondPhase(std::vector<Player*>& _player, Wave _wave, float _dt);
	void Death(Audio& _audio);
	void UpdateIdleSecondPhase(std::vector<Player*>& _player, float _dt, Audio& _audio);
	void ReturnToTarget(float _posY);
	void PlayerTargetIsDead(std::vector<Player*>& _player);
	void MoveBack(sf::Vector2f _pos, float _dt);
	void MoveSecondPhase(sf::Vector2f _pos, float _dt);
	void RunAway(float _dt, Wave _wave);

	void ChooseAPlayer(std::vector<Player*>& _player);
	void UpdateNormalAttack(sf::Vector2f _player, Audio& _audio);
	void UpdateZenitsuAttack(sf::Vector2f _player, float _dt, Audio& _audio);
	void UpdateTimerKnockDown(float _dt, Audio& _audio);
	void UpdateSlashAttackVerti(sf::Vector2f _player);
	void UpdateSlashAttackHorizon(sf::Vector2f _player);
	void UpdateSlash(Wave _wave, float _dt);
	void UpdateIdle(std::vector<Player*>& _player);
	void ChangePhase();
	void InitNewAnim();
	void UpdateAnim(float _dt);
	void UpdateDeath(float _dt);
	void InitNewSlash(SpriteAnim& _sprite, Animation* _anim, sf::RectangleShape* _slash);
	void UpdateBack(Wave _wave, float _dt, Audio& _audio);
	void UpdateShield(std::vector<Player*>& _player, float _dt, Audio& _audio);

	float timerAttack;//normal Attack
	float timerZenitsu;//ultimate Attack
	float timerSlashV;//slash verticval
	float timerSlashH;//slash horizontal
	float timerKnockDown;//after zenitsu slash
	float timerSlashZenitsu;//to stay on last frame
	float timerShield;

	float timerSound;
	float zenitsuTimerSound = 0.f;
	float attack1TimerSound = 0.f;
	float attack2TimerSound = 0.f;
	float breathlessTimerSound = 0.f;
	float groinkTimerSound = 0.f;

	int targetPlayer;
	int nbAttack;
	int dirAnim;

	bool isDead = false;
	bool secondPhase;
	bool doZenitsu;
	bool hasZenistsuSoundStarted = false;
	bool slashVertiAlive;
	bool slashHorizonAlive;
	bool knockDownAnim = false;
	bool leak;
	bool isLeaking;

	sf::Sprite shadowH;

	BoarState state;
	BoarState lastState;

	BoarSprite sprite;

	Mob* mob;
};

#endif // !BOAR_HPP