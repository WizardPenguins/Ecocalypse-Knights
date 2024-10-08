#ifndef MOB_HPP
#define MOB_HPP

#include "Entity.h"
#include "Animation.h"
#include "stdafx.h"
#include "MobSprite.hpp"

class Mob : public Entity
{
	friend class Player;
	friend class Boar;
	friend class HazelNut;
	friend class Bird;
	friend class Mushroom;
	friend class Witch;
	friend class Pig;
public:
	//Constructor/Destructor
	Mob(RaceMob _newType, Wave _wave, Audio& _audio, MushroomSprite* _spriteMushroom , PigSprite* _spritePig, StatusSprite* _status, BoarSprite* _spriteBoar, WitchSprite* _spriteWitch, HazelSprite* _hazelSprite, BirdSprite* _birdSprite, sf::Vector2f _pos);
	Mob(Audio& _audio, HazelSprite* _hazelSprite, StatusSprite* _statusSprite, sf::Vector2f _pos);
	~Mob() = default;

	//Set
	void SetState(MobState _state);
	void SetTimerKnockBack(float _timer);
	void SetTimerKnockDown(float _timer);
	void SetTimerHit(float _timer);
	void SetTimerStatus(float _timer);
	void SetTimerAttack(float _timer);
	void SetStatus(Status _status);
	void SetLastStatus(Status _status);
	void SetInAir(bool _bool);
	void SetTouchInAir(bool _bool);
	void SetMove(bool _bool);
	void SetDamage(int _damage);
	void SetDirection(int _dir);
	void SetInitKnockBack(bool _bool);
	void SetPos(sf::Vector2f _pos);
	void SetTargetPos(sf::Vector2f _targetPos);

	void SetTargetState();
	void SetKnockbackState();
	void SetKnockdownState();
	void SetInUpState();
	void SetInFallState();
	void SetDamageMagic(int _damage);

	//Get
	Boar* GetBoar();
	Witch* GetWitch();
	HazelNut* GetHazel();

	sf::Vector2f GetPosInAir();
	sf::Vector2f GetLastPos();
	sf::Vector2f GetSpeed();
	MagicUse GetMagicUse();
	sf::RectangleShape* GetHitBoxAttack();
	MobState GetState();
	RaceMob GetRace();
	TypeMob GetType();
	bool GetInAir();
	sf::Vector2f GetDimSprite(); //get the actual dimension of the sprite and not the spritsheet
	Status GetStatus();
	sf::Vector2f GetDecalage();
	int GetNbPhase();
	bool GetAnimFinish();
	int GetTargetPlayer();
	void SetTargetPlayer(int _newTarget);

	//Functions
	void InitNewAnimStatus();
	int  Trigo(sf::Vector2f _pos);
	void DropLife();
	void UpdateStatus(float _dt);
	void UpdateTimerHit(float _dt);
	void InitDropLife(std::vector<Drop*>* _drop);
	void OutOfScreen(Wave _wave);

	//Functions mother
	void Update(GameData& _gameData) override;
	void Display(sf::RenderWindow& _window);
	void Display(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void UpdateHitBox() override;

protected:
	sf::Vector2f scaleSprite;
	sf::Vector2f lastPos;
	sf::Vector2f decalage;

	MobState state = MobState::Wait;
	MobState lastState = MobState::Wait;
	Status status = Status::None;
	Status lastStatus = Status::None;
	TypeMob type;
	Buff typeBuff;
	RaceMob race;
	MagicUse magicUse;

	//MobSprite mobSprite;
	StatusSprite statusSprite;

	sf::RectangleShape* hitBoxAttack;

	int lifeDropValue;
	int direction;
	int tickDamage;
	int nbPhase;
	int rndSpeedAnim;
	int damageMagic;
	float posYFall;

	bool haveDropLife;
	bool inAir;
	bool touchInAir;
	bool move;
	bool initKnockBack;
	bool finishAnim;
	bool showMob;//debug

	//timer
	float timerDeath;
	float timerAttack;
	float timerKnockBack;
	float timerKnockDown;
	float timerInAir;
	float timerStatus;
	float timerHit;

	//mob
	Boar* boar;
	HazelNut* nut;
	Bird* bird;
	Mushroom* mush;
	Witch* witch;
	Pig* pig;
};

#endif // !MOB_HPP