#include "stdafx.h"

#include "Mob.hpp"
#include "Boar.hpp"
#include "HazelNut.hpp"
#include "Bird.hpp"
#include "Mushroom.hpp"
#include "Witch.hpp"
#include "Pig.hpp"

#include "Player.hpp"
#include "Easings.h"
#include "Camera.h"
#include "Audio.h"

//only for hazelNut when witch spawn a invoc circle
Mob::Mob(Audio& _audio, HazelSprite* _hazelSprite, StatusSprite* _statusSprite, sf::Vector2f _pos)
{
	//init some variable
	this->pos = _pos;
	this->scaleSprite = { 1.f,1.f };
	this->decalage = { 0,0 };
	this->lastPos = this->pos;
	this->tickDamage = 0;
	this->rndSpeedAnim = rand() % 3 + 1;
	this->gold = 0;
	this->xp = 0;
	this->finishAnim = false;
	this->state = MobState::Alive;
	this->lastState = this->state;
	this->life = 100;
	this->maxLife = this->life;
	this->showMob = false;

	this->status = Status::None;
	this->magicUse = MagicUse::None;
	this->race = RaceMob::HazelNut;

	this->haveDropLife = false;
	this->inAir = false;

	//init status sprite & anim
	this->statusSprite = *_statusSprite;
	this->statusSprite.anim = new Animation;

	//timer
	this->timerDeath = 0.f;
	this->timerAttack = 0.f;
	this->timerKnockBack = 0.f;
	this->timerKnockDown = 0.f;
	this->timerInAir = 0.f;

	//init hitbox
	this->hitbox = new sf::RectangleShape;
	this->hitBoxAttack = new sf::RectangleShape;
	this->hitBoxAttack->setPosition(sf::Vector2f{ -10'000,-10'000 }); //just securty

	this->type = TypeMob::CAC;
	this->nut = new HazelNut{ this,_hazelSprite };
	this->nut->SetState(HazelState::Target);
}

Mob::Mob(RaceMob _newType, Wave _wave, Audio& _audio, MushroomSprite* _spriteMushroom ,PigSprite* _spritePig, StatusSprite* _status, BoarSprite* _spriteBoar, WitchSprite* _spriteWitch, HazelSprite* _hazelSprite, BirdSprite* _birdSprite, sf::Vector2f _pos)
{
	//init some variable
	this->pos = _pos;
	this->scaleSprite = { 1.f,1.f };
	this->decalage = { 0,0 };
	this->lastPos = this->pos;
	this->tickDamage = 0;
	this->rndSpeedAnim = rand() % 3 + 1;
	this->gold = 5 + ((rand() % 10) - 5);
	this->finishAnim = false;
	this->state = MobState::Wait;
	this->lastState = this->state;
	this->showMob = false;

	this->status = Status::None;
	this->magicUse = MagicUse::None;
	this->race = _newType;

	this->haveDropLife = false;
	this->inAir = false;

	//init status sprite & anim
	this->statusSprite = *_status;
	this->statusSprite.anim = new Animation;

	//timer
	this->timerDeath = 0.f;
	this->timerAttack = 0.f;
	this->timerKnockBack = 0.f;
	this->timerKnockDown = 0.f;
	this->timerInAir = 0.f;
	this->damageMagic = 0;

	//init hitbox
	this->hitbox = new sf::RectangleShape;
	this->hitBoxAttack = new sf::RectangleShape;
	this->hitBoxAttack->setPosition(sf::Vector2f{ -10'000,-10'000 }); //just securty

	//init distance & mage mob
	switch (this->race)
	{
	case RaceMob::HazelNut:
		this->type = TypeMob::CAC;
		this->nut = new HazelNut{ this ,_hazelSprite };
		break;

	case RaceMob::Bird:
		this->type = TypeMob::Distance;
		this->bird = new Bird{ this,_birdSprite };
		break;

	case RaceMob::Mushroom:
		this->type = TypeMob::CAC;
		this->mush = new Mushroom{ this,_spriteMushroom };
		break;

	case RaceMob::Pig:
		this->type = TypeMob::CAC;
		this->pig = new Pig{ this,_spritePig };
		break;

	case RaceMob::Boar:
		this->type = TypeMob::Boss;
		this->boar = new Boar{ this,_spriteBoar };
		break;

	case RaceMob::Witch:
		this->type = TypeMob::Boss;
		this->witch = new Witch{ this,_spriteWitch,_wave };
		break;

	default:
		break;
	}
}

void Mob::Update(GameData& _gameData)
{
	this->lastPos = this->pos;
	UpdateStatus(_gameData.dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
	{
		this->showMob = true;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		this->showMob = false;
	}

	switch (this->race)
	{
	case RaceMob::HazelNut:
		this->nut->Update(_gameData.tabPlayers, _gameData.dt, _gameData.actualWave, *_gameData.audio);
		break;

	case RaceMob::Bird:
		this->bird->Update(_gameData.tabPlayers, _gameData.projectile, _gameData.dt, _gameData.actualWave, *_gameData.audio);
		break;

	case RaceMob::Mushroom:
		this->mush->Update(_gameData.tabPlayers, _gameData.dt, _gameData.actualWave, *_gameData.audio);
		break;

	case RaceMob::Pig:
		this->pig->Update(_gameData.tabPlayers, _gameData.dt, _gameData.actualWave, *_gameData.audio);
		break;

	case RaceMob::Boar:
		this->boar->Update(_gameData.tabPlayers, _gameData.dt, _gameData.actualWave, *_gameData.audio);
		break;

	case RaceMob::Witch:
		this->witch->SetLastState(this->witch->GetState());
		this->witch->Update(_gameData.dt,_gameData.tabMob,_gameData.actualWave,*_gameData.audio,_gameData.tabPlayers);
		break;

	default:
		break;
	}

	InitDropLife(&_gameData.drop);
	OutOfScreen(_gameData.actualWave);
	UpdateHitBox();
}

void Mob::Display(sf::RenderWindow& _window)
{
	switch (this->race)
	{
	case RaceMob::HazelNut:
		this->nut->Display(_window);
		break;
	case RaceMob::Bird:
		this->bird->Display(_window);
		break;
	case RaceMob::Mushroom:
		this->mush->Display(_window);
		break;
	case RaceMob::Pig:
		this->pig->Display(_window);
		break;
	case RaceMob::Boar:
		this->boar->Display(_window);
		break;
	case RaceMob::Witch:
		this->witch->Display(_window);
		break;
	default:
		break;
	}
}

void Mob::Display(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	switch (this->race)
	{
	case RaceMob::HazelNut:
		this->nut->Display(_red,_blue);
		break;
	case RaceMob::Bird:
		this->bird->Display(_red,_blue);
		break;
	case RaceMob::Mushroom:
		this->mush->Display(_red, _blue);
		break;
	case RaceMob::Pig:
		this->pig->Display(_red, _blue);
		break;
	case RaceMob::Boar:
		this->boar->Display(_red, _blue);
		break;
	case RaceMob::Witch:
		this->witch->Display(_red, _blue);
		break;
	default:
		break;
	}
}

void Mob::SetState(MobState _state)
{
	this->state = _state;
}

void Mob::SetTimerKnockBack(float _timer)
{
	this->timerKnockBack = _timer;
}

void Mob::SetTimerKnockDown(float _timer)
{
	this->timerKnockDown = _timer;
}

void Mob::SetTimerHit(float _timer)
{
	this->timerHit = _timer;
}

void Mob::SetTimerStatus(float _timer)
{
	this->timerStatus = _timer;
}

void Mob::SetTimerAttack(float _timer)
{
	this->timerAttack = _timer;
}

void Mob::SetStatus(Status _status)
{
	this->status = _status;
}

void Mob::SetLastStatus(Status _status)
{
	this->lastStatus = _status;
}

void Mob::SetInAir(bool _bool)
{
	this->inAir = _bool;
}

void Mob::SetTouchInAir(bool _bool)
{
	this->touchInAir = _bool;
}

void Mob::SetMove(bool _bool)
{
	this->move = _bool;
}

void Mob::SetDamage(int _damage)
{
	this->damaged = _damage;
}

void Mob::SetDirection(int _dir)
{
	this->direction = _dir;
}

void Mob::SetInitKnockBack(bool _bool)
{
	this->initKnockBack = _bool;
}

void Mob::SetPos(sf::Vector2f _pos)
{
	this->pos = _pos;
}

void Mob::SetTargetPos(sf::Vector2f _targetPos)
{
}

void Mob::SetTargetState()
{
	switch (this->race)
	{
	case RaceMob::HazelNut:
		this->nut->SetState(HazelState::Target);
		break;

	case RaceMob::Mushroom:
		this->mush->SetState(MushroomState::Target);
		break;

	default:
		break;
	}
}

void Mob::SetKnockbackState()
{
	switch (this->race)
	{
	case RaceMob::HazelNut:
		this->nut->SetState(HazelState::Knockback);
		this->nut->SetDrawFx(true);
		break;

	case RaceMob::Mushroom:
		this->mush->SetState(MushroomState::Knockback);
		this->mush->SetDrawFx(true);
		break;

	case RaceMob::Bird:
		this->bird->SetState(BirdState::Hit);
		this->bird->SetDrawFx(true);
		break;

	case RaceMob::Pig:
		this->pig->SetState(PigState::Knockback);
		this->pig->SetDrawFx(true);
		this->pig->InitNewAnim();
		break;

	default:
		break;
	}
	this->initKnockBack = true;
}

void Mob::SetKnockdownState()
{
	switch (this->race)
	{
	case RaceMob::HazelNut:
		this->nut->SetState(HazelState::Knockdown);
		this->nut->SetDrawFx(true);
		break;

	case RaceMob::Mushroom:
		this->mush->SetState(MushroomState::Knockdown);
		this->mush->SetDrawFx(true);
		break;

	case RaceMob::Bird:
		this->bird->SetState(BirdState::Hit);
		this->bird->SetDrawFx(true);
		break;

	case RaceMob::Pig:
		this->pig->SetState(PigState::Knockdown);
		this->pig->SetDrawFx(true);
		this->pig->InitNewAnim();
		break;

	default:
		break;
	}
}

void Mob::SetInUpState()
{
	if (this->status != Status::OnIce && this->status != Status::OnElec && !this->inAir && this->life > 0)
	{
		switch (this->race)
		{
		case RaceMob::HazelNut:
			this->nut->SetState(HazelState::InUp);
			this->nut->SetDrawFx(true);
			this->inAir = true;
			this->posInAir = this->pos;
			break;

		case RaceMob::Mushroom:
			this->mush->SetState(MushroomState::InUp);
			this->mush->SetDrawFx(true);
			this->inAir = true;
			this->posInAir = this->pos;
			break;

		case RaceMob::Bird:
			this->bird->SetState(BirdState::Hit);
			this->bird->SetDrawFx(true);
			break;

		case RaceMob::Pig:
			this->pig->SetState(PigState::InUp);
			this->pig->SetDrawFx(true);
			this->inAir = true;
			this->pig->InitNewAnim();
			this->posInAir = this->pos;
			break;

		default:
			break;
		}
	}
}

void Mob::SetInFallState()
{
	if (this->status != Status::OnIce && this->status != Status::OnElec && this->inAir && this->life > 0)
	{
		switch (this->race)
		{
		case RaceMob::HazelNut:
			this->nut->SetState(HazelState::InFall);
			this->nut->SetDrawFx(true);
			this->timerInAir = 0.f;
			this->touchInAir = true;
			this->inAir = true;
			this->posYFall = this->posInAir.y;
			break;

		case RaceMob::Mushroom:
			this->mush->SetState(MushroomState::InFall);
			this->mush->SetDrawFx(true);
			this->timerInAir = 0.f;
			this->touchInAir = true;
			this->inAir = true;
			this->posYFall = this->posInAir.y;
			break;

		case RaceMob::Bird:
			this->bird->SetState(BirdState::Hit);
			this->bird->SetDrawFx(true);
			break;

		case RaceMob::Pig:
			this->pig->SetState(PigState::InFall);
			this->pig->SetDrawFx(true);
			this->timerInAir = 0.f;
			this->touchInAir = true;
			this->inAir = true;
			this->posYFall = this->posInAir.y;
			this->pig->InitNewAnim();
			break;

		default:
			break;
		}
	}
}

void Mob::SetDamageMagic(int _damage)
{
	this->damageMagic = _damage;
}

Boar* Mob::GetBoar()
{
	return this->boar;
}

Witch* Mob::GetWitch()
{
	return this->witch;
}

HazelNut* Mob::GetHazel()
{
	return this->nut;
}

bool Mob::GetAnimFinish()
{
	return this->finishAnim;
}

int Mob::GetTargetPlayer()
{
	int target = 0;
	switch (this->race)
	{
	case RaceMob::HazelNut:
		target = this->nut->GetTargetPlayer();
		break;
		
	case RaceMob::Mushroom:
		target = this->mush->GetTargetPlayer();
		break;
		
	case RaceMob::Boar:
		target = this->boar->GetTarget();
		break;
		
	case RaceMob::Witch:
		target = this->witch->GetTargetPlayer();
		break;

	default:
		break;
	};

	return target;
}

void Mob::SetTargetPlayer(int _newTarget)
{
	switch (this->race)
	{
	case RaceMob::HazelNut:
		this->nut->SetTargetplayer(_newTarget);
		break;

	case RaceMob::Mushroom:
		this->mush->SetTargetplayer(_newTarget);
		break;

	case RaceMob::Boar:
		this->boar->SetTarget(_newTarget);
		break;

	case RaceMob::Witch:
		this->witch->SetTargetPlayer(_newTarget);
		break;

	default:
		break;
	};
}

sf::Vector2f Mob::GetPosInAir()
{
	return this->posInAir;
}

sf::Vector2f Mob::GetLastPos()
{
	return this->lastPos;
}

sf::Vector2f Mob::GetSpeed()
{
	return this->speed;
}

MagicUse Mob::GetMagicUse()
{
	return this->magicUse;
}

sf::RectangleShape* Mob::GetHitBoxAttack()
{
	return this->hitBoxAttack;
}

MobState Mob::GetState()
{
	return this->state;
}

RaceMob Mob::GetRace()
{
	return this->race;
}

TypeMob Mob::GetType()
{
	return this->type;
}

bool Mob::GetInAir()
{
	return this->inAir;
}

sf::Vector2f Mob::GetDimSprite()
{
	if (this->race == RaceMob::Boar)
	{
		return this->boar->GetDimSprite();
	}

	if (this->race == RaceMob::Witch)
	{
		return this->witch->GetDimSprite();
	}

	if (this->race == RaceMob::HazelNut)
	{
		return this->nut->GetDimSprite();
	}

	if (this->race == RaceMob::Mushroom)
	{
		return this->mush->GetDimSprite();
	}

	if (this->race == RaceMob::Pig)
	{
		return this->pig->GetDimSprite();
	}

	return sf::Vector2f{ 0,0 };
}

Status Mob::GetStatus()
{
	return this->status;
}

sf::Vector2f Mob::GetDecalage()
{
	return this->decalage;
}

int Mob::GetNbPhase()
{
	return this->nbPhase;
}

int Mob::Trigo(sf::Vector2f _pos)
{
	sf::Vector2f vector = { _pos - this->pos };
	float circle = atan2f(vector.y, vector.x) * (180 / PI);

	if (circle >= 90 || circle <= -90) // gauche
	{
		return 1;
	}
	else // droite
	{
		return -1;
	}
}

void Mob::DropLife()
{
	if (!(rand() % 10))
	{
		this->haveDropLife = true;
	}
}

void Mob::UpdateStatus(float _dt)
{
	if (this->status != Status::None && (!this->inAir || this->witch != nullptr))
	{
		(bool)(this->timerStatus == 1.2f) ? this->timerStatus = this->timerStatus : this->timerStatus = this->timerStatus;
		this->initKnockBack = true;
		this->timerKnockBack = this->timerStatus;
		this->timerHit = 0.5f;
		this->direction = 0;

		this->timerStatus -= _dt;
		if (this->timerStatus <= 0.f)
		{
			this->tickDamage = 0;
			this->status = Status::None;
			this->lastStatus = Status::None;
			this->timerKnockBack = 0.f;
		}
		else if (!(this->tickDamage % 35) && this->status != Status::OnIce && this->status != Status::OnElec)
		{
			this->life -= this->damageMagic;
		}
		this->tickDamage++;
	}
}

void Mob::UpdateTimerHit(float _dt)
{
	if (this->timerHit > 0)
	{
		this->timerHit -= _dt;
	}
}

void Mob::InitDropLife(std::vector<Drop*>* _drop)
{
	if (this->haveDropLife)
	{
		Drop* temp = new Drop{ this->pos };
		temp->rect.setPosition(temp->pos);
		_drop->push_back(temp);
	}
}

void Mob::OutOfScreen(Wave _wave)
{
	if (this->boar == nullptr && this->witch == nullptr)
	{
		if (this->pos.x < 0 + (int)(_wave) * 1920.f)
		{
			this->pos.x = 0 + (int)(_wave) * 1920.f;
		}

		if (this->pos.x > 1900.f + (int)(_wave) * 1900.f)
		{
			this->pos.x = 1900.f + (int)(_wave) * 1900.f;
		}
	}
}

void Mob::UpdateHitBox()
{
	this->inAir ? this->hitbox->setPosition(this->posInAir) : this->hitbox->setPosition(this->pos);
	this->posInAir.x = this->pos.x;
}

void Mob::InitNewAnimStatus()
{
	sf::IntRect rect = sf::IntRect(0, 0, (int)(this->statusSprite.spriteAnim[(int)(this->status) - 1].animDim.x / this->statusSprite.spriteAnim[(int)(this->status) - 1].nbAnim.x), (int)(this->statusSprite.spriteAnim[(int)(this->status) - 1].nbAnim.y));
	this->statusSprite.anim->CreateLeftToRight(this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite, (unsigned char)(this->statusSprite.spriteAnim[(int)(this->status) - 1].nbAnim.x), (unsigned char)(this->statusSprite.spriteAnim[(int)(this->status) - 1].nbAnim.y), this->statusSprite.spriteAnim[(int)(this->status) - 1].animDim);
	this->statusSprite.anim->currentFrame = 0;
	this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite.setTextureRect(rect);
	this->lastStatus = this->status;
}