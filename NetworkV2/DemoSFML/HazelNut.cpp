#include "stdafx.h"
#include "HazelNut.hpp"
#include "Player.hpp"

HazelNut::HazelNut(Mob* _mob, HazelSprite* _sprite)
{
	//init mob variable
	this->mob = _mob;
	this->mob->speed = { 100.f,75.f };
	this->mob->inAir = false;
	this->mob->state = MobState::Alive;
	this->mob->timerKnockBack = TimerKnockBack;
	this->mob->timerInAir = 0.f;

	//init hazel variable
	this->state = HazelState::Idle;
	this->targetPlayer = 0;
	this->drawFx = false;
	this->hitboxAttack = true;
	this->moveAttack = XAttackMove;
	this->timerAttack = TimerAttack;
	this->posDeath = { 0.f,0.f };

	//anim
	this->sprite = *_sprite;
	this->sprite.anim = new Animation;
	this->sprite.animFx = new Animation;
	InitNewAnim();
	CreateAnimFx();

	//normal attack
	this->mob->hitBoxAttack->setSize(sf::Vector2f{ 40.f, 75.f });
	this->mob->hitBoxAttack->setOrigin(sf::Vector2f{ this->mob->hitBoxAttack->getSize().x / 2,this->mob->hitBoxAttack->getSize().y });
	DestroyAttack();
}

void HazelNut::Update(std::vector<Player*>& _tabPlayer, float _dt, Wave _wave, Audio& _audio)
{
	sf::Vector2f posPlayer = _tabPlayer[this->targetPlayer]->GetPos();

	switch (this->state)
	{
	case HazelState::Idle:
		UpdateTargetPlayer(_tabPlayer);
		break;

	case HazelState::Target:
		UpdateTargetPlayer(_tabPlayer);
		Move(posPlayer, _dt);
		UpdateTimer(_dt);
		ChangeState(posPlayer);
		break;

	case HazelState::Attack:
		Attack();
		break;

	case HazelState::Death:
		UpdateDeath(_dt);
		break;

	case HazelState::InFall:
		UpdateInFall(_dt);
		break;

	case HazelState::InUp:
		UpdateInUp(_dt);
		break;

	case HazelState::Knockback:
		UpdateKnockback(_dt);
		break;

	case HazelState::Knockdown:
		UpdateKnockdown(_dt);
		break;

	default:
		break;
	}

	UpdateDirection(posPlayer);
	if (this->state != HazelState::Attack)
	{
		DestroyAttack();
	}
	UpdateAnim(_dt);
	this->lastState = this->state;
	Death(_audio);

	if (this->mob->showMob)
	{
		std::cout << "Race : " << (int)this->mob->race << std::endl;
		std::cout << "Pos : " << this->mob->pos.x << "  " << this->mob->pos.y << std::endl;
		std::cout << "PosInAir : " << this->mob->posInAir.x << "  " << this->mob->posInAir.y << std::endl;
		std::cout << "InAir : " << this->mob->inAir << std::endl;
		std::cout << "State : " << (int)this->state << std::endl;
		std::cout << "Life : " << this->mob->life << std::endl;
		std::cout << "TargetPlayer : " << this->targetPlayer << std::endl;
		std::cout << "Status : " << (int)this->mob->status << std::endl;
		std::cout << "TimerInAir : " << this->mob->timerInAir << std::endl;
		std::cout << "PosYFall : " << this->mob->posYFall << std::endl;
		std::cout << "--------------------------------------------------------------------------------------------" << std::endl;
	}
}

void HazelNut::Display(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	DisplayEntity(_red, _blue);
	DisplayFx(_red, _blue);
	DisplayStatus(_red, _blue);
}

void HazelNut::Display(sf::RenderWindow& _window)
{
	DisplayEntity(_window);
	DisplayFx(_window);
	DisplayStatus(_window);
}

void HazelNut::DisplayFx(sf::RenderWindow& _window)
{
	if (this->drawFx)
	{
		this->mob->inAir ? this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x +5.f, this->mob->posInAir.y - this->mob->GetDimSprite().y + 45.f}) : this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->pos.x+5.f,this->mob->pos.y - this->mob->GetDimSprite().y + 45.f});
		_window.draw(this->sprite.spriteFx.sprite);
	}
}

void HazelNut::DisplayEntity(sf::RenderWindow& _window)
{
	this->sprite.spriteAnim[(int)this->state].sprite.setScale(this->mob->scaleSprite);
	(bool)(this->mob->inAir) ? this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->posInAir) : this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos + this->posDeath);
	_window.draw(this->sprite.spriteAnim[(int)this->state].sprite);
}

void HazelNut::DisplayStatus(sf::RenderWindow& _window)
{
	if (this->mob->status != Status::None && this->mob->state != MobState::Death && this->mob->state != MobState::Dispawn)
	{
		this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setColor(sf::Color(255, 255, 255, 225));

		int temp; sf::Vector2f scale;
		if (this->mob->status == Status::OnFire)
		{
			temp = 5;
			scale = { 0.5f,0.5f };
		}
		else if (this->mob->status == Status::OnElec)
		{
			temp = 60;
			scale = { 1.f,1.f };
		}
		else if (this->mob->status == Status::OnIce)
		{
			temp = 65;
			scale = { 0.7f,0.7f };
		}
		else//poison
		{
			temp = 5;
			scale = { 0.5f,0.5f };
		}

		this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setScale(scale);
		this->mob->inAir ? this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x,this->mob->posInAir.y - this->mob->GetDimSprite().y + temp }) : this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->pos.x,this->mob->pos.y - this->mob->GetDimSprite().y + temp });
		_window.draw(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite);
	}
}

void HazelNut::DisplayFx(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	if (this->drawFx)
	{
		this->mob->inAir ? this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x + (TROAD_GAP * (-1 * this->mob->posInAir.y + WORKING_HEIGHT) / 100.f) , this->mob->posInAir.y - this->mob->GetDimSprite().y }) : this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->pos.x + (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f),this->mob->pos.y - this->mob->GetDimSprite().y });
		_red.draw(this->sprite.spriteFx.sprite);

		this->mob->inAir ? this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x - (TROAD_GAP * (-1 * this->mob->posInAir.y + WORKING_HEIGHT) / 100.f) , this->mob->posInAir.y - this->mob->GetDimSprite().y }) : this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->pos.x - (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f),this->mob->pos.y - this->mob->GetDimSprite().y });
		_blue.draw(this->sprite.spriteFx.sprite);
	}
}

void HazelNut::DisplayEntity(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	this->sprite.spriteAnim[(int)this->state].sprite.setScale(this->mob->scaleSprite);
	this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos.x + (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f), this->mob->pos.y);
	_red.draw(this->sprite.spriteAnim[(int)this->state].sprite);

	this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos.x - (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f), this->mob->pos.y);
	_blue.draw(this->sprite.spriteAnim[(int)this->state].sprite);
}

void HazelNut::DisplayStatus(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	if (this->mob->status != Status::None && this->mob->state != MobState::Death && this->mob->state != MobState::Dispawn)
	{
		this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setColor(sf::Color(255, 255, 255, 225));

		int temp; sf::Vector2f scale;
		if (this->mob->status == Status::OnFire)
		{
			temp = 5;
			scale = { 0.5f,0.5f };
		}
		else if (this->mob->status == Status::OnElec)
		{
			temp = 60;
			scale = { 1.f,1.f };
		}
		else if (this->mob->status == Status::OnIce)
		{
			temp = 65;
			scale = { 0.7f,0.7f };
		}
		else//poison
		{
			temp = 5;
			scale = { 0.5f,0.5f };
		}

		this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setScale(scale);
		this->mob->inAir ? this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x + (TROAD_GAP * (-1 * this->mob->posInAir.y + WORKING_HEIGHT) / 100.f),this->mob->posInAir.y - this->mob->GetDimSprite().y + temp }) : this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->pos.x + (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f),this->mob->pos.y - this->mob->GetDimSprite().y + temp });
		_red.draw(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite);
		
		this->mob->inAir ? this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x - (TROAD_GAP * (-1 * this->mob->posInAir.y + WORKING_HEIGHT) / 100.f),this->mob->posInAir.y - this->mob->GetDimSprite().y + temp }) : this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->pos.x - (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f),this->mob->pos.y - this->mob->GetDimSprite().y + temp });
		_blue.draw(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite);
	}
}

void HazelNut::UpdateTargetPlayer(std::vector<Player*>& _player)
{
	float distance = Utils::GetVectorLength(sf::Vector2f{ _player[this->targetPlayer]->GetPos() - this->mob->pos });
	for (int i = 0; i < _player.size(); i++)
	{
		float test = Utils::GetVectorLength(sf::Vector2f{ _player[i]->GetPos() - this->mob->pos });
		if (test < distance && _player[i]->GetState() != DEAD)
		{
			distance = test;
			this->targetPlayer = i;
		}
	}

	if (_player[this->targetPlayer]->GetState() == DEAD)
	{
		this->targetPlayer = (this->targetPlayer + 1) % _player.size();
	}

	if (this->hitboxAttack)
	{
		this->hitboxAttack = false;
	}
}

void HazelNut::Move(sf::Vector2f _posPlayer, float _dt)
{
	sf::Vector2f goalPos = sf::Vector2f{ _posPlayer.x + GoalPos * this->mob->scaleSprite.x,_posPlayer.y };
	sf::Vector2f normVec = Utils::NormalizeVector(sf::Vector2f{ goalPos - this->mob->pos });

	this->mob->pos.x += normVec.x * this->mob->speed.x * _dt;
	this->mob->pos.y += normVec.y * this->mob->speed.y * _dt;
}

void HazelNut::Attack()
{
	if (this->sprite.anim->currentFrame >= 3 && this->sprite.anim->currentFrame <= 7)//when up
	{
		this->mob->pos.x -= this->moveAttack;
		this->mob->posInAir.y -= YAttackMove;
		SetHitBoxAttack();
	}
	else if (this->sprite.anim->currentFrame >= 7)//when down
	{
		this->mob->pos.x -= this->moveAttack;
		this->mob->posInAir.y += YAttackMove;
		SetHitBoxAttack();
	}

	//end anim and touch ground
	if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame && this->mob->posInAir.y >= this->mob->pos.y)
	{
		this->mob->inAir = false;
		this->mob->pos = this->mob->posInAir;
		this->state = HazelState::Target;
		DestroyAttack();
	}
}

void HazelNut::Death(Audio& _audio)
{
	if (!this->mob->inAir && this->mob->life <= 0 && this->state != HazelState::Death)
	{
		_audio.PlaySoundNotRepeated("HAZELNUT_DEATH", 100.f);
		this->state = HazelState::Death;
		this->mob->state = MobState::Death;
		this->posDeath.y += 30.f;
	}
}

void HazelNut::DestroyAttack()
{
	this->mob->hitBoxAttack->setPosition(sf::Vector2f{ -10'00,-10'000 });
}

void HazelNut::InitNewAnim()
{
	sf::IntRect rect = sf::IntRect(0, 0, (int)(this->sprite.spriteAnim[(int)this->state].animDim.x / this->sprite.spriteAnim[(int)this->state].nbAnim.x), (int)(this->sprite.spriteAnim[(int)this->state].nbAnim.y));
	this->sprite.anim->CreateLeftToRight(this->sprite.spriteAnim[(int)this->state].sprite, (unsigned char)(this->sprite.spriteAnim[(int)this->state].nbAnim.x), (unsigned char)(this->sprite.spriteAnim[(int)this->state].nbAnim.y), this->sprite.spriteAnim[(int)this->state].animDim);
	this->sprite.spriteAnim[(int)this->state].sprite.setTextureRect(rect);
	this->sprite.anim->currentFrame = 0;

	this->mob->hitbox->setSize(sf::Vector2f{ this->sprite.spriteAnim[(int)this->state].animDim.x / this->sprite.spriteAnim[(int)this->state].nbAnim.x, this->sprite.spriteAnim[(int)this->state].animDim.y / this->sprite.spriteAnim[(int)this->state].nbAnim.y });
	this->mob->hitbox->setOrigin(this->mob->hitbox->getSize().x / 2, this->mob->hitbox->getSize().y);
}

void HazelNut::UpdateAnim(float _dt)
{
	if (this->state != this->lastState)
	{
		InitNewAnim();
	}

	if (this->mob->status != this->mob->lastStatus)
	{
		this->mob->InitNewAnimStatus();
	}

	if (this->drawFx)
	{
		if (this->sprite.animFx->currentFrame == this->sprite.animFx->maxFrame)
		{
			this->drawFx = false;
			this->sprite.animFx->currentFrame = 0;
		}

		bool temp = true;
		this->sprite.animFx->UpdateLeftToRight(this->sprite.spriteFx.sprite, _dt, this->sprite.spriteFx.speed, 0, temp, this->sprite.spriteFx.loop);
	}

	bool temp = true;
	this->sprite.anim->UpdateLeftToRight(this->sprite.spriteAnim[(int)this->state].sprite, _dt, this->sprite.spriteAnim[(int)this->state].speed, 0, temp, this->sprite.spriteAnim[(int)this->state].loop);

	if (this->mob->status != Status::None)
	{
		bool temp = true;
		this->mob->statusSprite.anim->UpdateLeftToRight(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite, _dt, this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].speed, 0, temp, this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].loop);
	}
}

void HazelNut::UpdateDirection(sf::Vector2f _pos)
{
	if (this->state != HazelState::Attack && this->state != HazelState::Death)
	{
		this->mob->scaleSprite = sf::Vector2f{ (float)(this->mob->Trigo(_pos)),1.f };
		this->moveAttack = this->mob->scaleSprite.x * XAttackMove;
	}
}

void HazelNut::SetHitBoxAttack()
{
	this->mob->hitBoxAttack->setPosition(sf::Vector2f{ this->mob->posInAir.x - this->moveAttack * 10.f ,this->mob->posInAir.y - 20.f });
}

void HazelNut::UpdateTimer(float _dt)
{
	if (this->timerAttack > 0)
	{
		this->timerAttack -= _dt;
	}
}

bool HazelNut::CanAttack(sf::Vector2f _pos)
{
	if (std::abs(_pos.x - this->mob->pos.x) <= DistMinAttack &&
		this->mob->pos.y <= _pos.y + DistAxeY && this->mob->pos.y >= _pos.y - DistAxeY)
	{
		return true;
	}

	return false;
}

void HazelNut::ChangeState(sf::Vector2f _pos)
{
	if (this->timerAttack <= 0 && CanAttack(_pos))
	{
		this->state = HazelState::Attack;
		this->mob->inAir = true;
		this->mob->posInAir = this->mob->pos;
		this->timerAttack = TimerAttack;
	}
}

void HazelNut::UpdateDeath(float _dt)
{
	if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame)
	{
		this->sprite.spriteAnim[(int)this->state].sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)(this->sprite.spriteAnim[(int)this->state].sprite.getColor().a - _dt * SpeedFadeDeath)));
		if (this->sprite.spriteAnim[(int)this->state].sprite.getColor().a <= 0)
		{
			this->mob->state = MobState::Dispawn;

			if (rand() % 10 <= 3)
			{
				this->mob->haveDropLife = true;
			}
		}
	}
}

void HazelNut::UpdateKnockback(float _dt)
{
	this->mob->timerKnockBack -= _dt;
	UpdatePosKnockback();
	if (this->mob->timerKnockBack <= 0)
	{
		this->state = HazelState::Target;
		this->mob->state = MobState::Alive;
		this->mob->timerKnockBack = TimerKnockBack;
		if (this->mob->inAir)
		{
			this->state = HazelState::InFall;
			this->mob->state = MobState::Hit;
			this->mob->timerInAir = 0;
			this->mob->posYFall = this->mob->posInAir.y;
		}
	}
}

void HazelNut::UpdateKnockdown(float _dt)
{
	this->mob->timerKnockDown -= _dt;
	if (this->mob->timerKnockDown <= 0)
	{
		this->state = HazelState::Target;
		this->mob->state = MobState::Alive;
	}
}

void HazelNut::UpdatePosKnockback()
{
	this->mob->pos.x += KnockbackForce * this->mob->direction;
}

void HazelNut::UpdateInUp(float _dt)
{
	if (this->mob->timerInAir < 1)
	{
		this->mob->posInAir = Easings::Lerp(this->mob->pos, { this->mob->pos.x,this->mob->pos.y - DistanceJumpY }, Easings::OutCirc(this->mob->timerInAir));
		this->mob->timerInAir += _dt;
	}
	else
	{
		this->mob->timerInAir = 0;
		this->mob->posYFall = this->mob->posInAir.y;
		this->state = HazelState::InFall;
	}
}

void HazelNut::UpdateInFall(float _dt)
{
	if (this->mob->timerInAir < 1 && this->mob->posInAir.y < this->mob->pos.y)
	{
		this->mob->posInAir = Easings::Lerp({ this->mob->posInAir.x, this->mob->posYFall }, { this->mob->posInAir.x,this->mob->pos.y }, Easings::InCirc(this->mob->timerInAir));
		this->mob->timerInAir += _dt + (_dt * this->mob->touchInAir);
	}
	else
	{
		this->mob->timerInAir = 0.f;
		this->mob->pos = { this->mob->posInAir.x,this->mob->pos.y };

		this->state = HazelState::Target;

		this->mob->inAir = false;
		this->mob->touchInAir = false;
	}
}

void HazelNut::CreateAnimFx()
{
	sf::IntRect rect = sf::IntRect(0, 0, (int)(this->sprite.spriteFx.animDim.x / this->sprite.spriteFx.nbAnim.x), (int)(this->sprite.spriteFx.nbAnim.y));
	this->sprite.animFx->CreateLeftToRight(this->sprite.spriteFx.sprite, (unsigned char)(this->sprite.spriteFx.nbAnim.x), (unsigned char)(this->sprite.spriteFx.nbAnim.y), this->sprite.spriteFx.animDim);
	this->sprite.spriteFx.sprite.setTextureRect(rect);
	this->sprite.animFx->currentFrame = 0;
}

sf::Vector2f HazelNut::GetDimSprite()
{
	sf::Vector2u textureSize = this->sprite.spriteAnim[(int)this->state].sprite.getTexture()->getSize();
	return sf::Vector2f(textureSize.x / this->sprite.spriteAnim[(int)this->state].nbAnim.x, textureSize.y / this->sprite.spriteAnim[(int)this->state].nbAnim.y);
}

int HazelNut::GetTargetPlayer()
{
	return this->targetPlayer;
}

void HazelNut::SetTargetplayer(int _newTarget)
{
	this->targetPlayer = _newTarget;
}

bool HazelNut::GetHitboxAttack()
{
	return this->hitboxAttack;
}

void HazelNut::SetHitBoxAttackBool(bool _bool)
{
	this->hitboxAttack = _bool;
}

void HazelNut::SetState(HazelState _state)
{
	this->state = _state;
}

void HazelNut::SetDrawFx(bool _drawFx)
{
	this->drawFx = _drawFx;
}

HazelState HazelNut::GetState()
{
	return this->state;
}