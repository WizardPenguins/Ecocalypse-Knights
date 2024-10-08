#include "stdafx.h"
#include "Scarecrow.hpp"

Scarecrow::Scarecrow(ScarecrowSprite& _mobSprite, StatusSprite& _statusSprite, sf::Vector2f _pos)
{
	this->sprite = _mobSprite;
	this->sprite.anim = new Animation;

	this->statusSprite = _statusSprite;
	this->statusSprite.anim = new Animation;

	this->state = StateScarecrow::Idle;
	this->lastState = StateScarecrow::Idle;

	this->status = Status::None;

	this->life = LifeScarecrow;
	this->pos = _pos;
	this->damageMagic = 0;

	this->hitbox = new sf::RectangleShape;
	InitNewAnim();
}

void Scarecrow::UpdateAnim(float _dt)
{
	if (this->state != this->lastState)
	{
		InitNewAnim();
	}

	if (this->status != Status::None && !this->initStatus)
	{
		InitNewAnimStatus();
	}

	bool temp = true;
	this->sprite.anim->UpdateLeftToRight(this->sprite.spriteAnim[(int)this->state].sprite, _dt, this->sprite.spriteAnim[(int)this->state].speed, this->ligne, temp, this->sprite.spriteAnim[(int)this->state].loop);

	if (this->status != Status::None)
	{
		bool temp = true;
		this->statusSprite.anim->UpdateLeftToRight(this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite, _dt, this->statusSprite.spriteAnim[(int)(this->status) - 1].speed, 0, temp, this->statusSprite.spriteAnim[(int)(this->status) - 1].loop);
	}
}

void Scarecrow::InitNewAnim()
{
	if (this->sprite.spriteAnim[(int)this->state].nbAnim.x != 0)
	{
		sf::IntRect rect = sf::IntRect(0, 0, (int)(this->sprite.spriteAnim[(int)this->state].animDim.x / this->sprite.spriteAnim[(int)this->state].nbAnim.x), this->sprite.spriteAnim[(int)this->state].animDim.y / (int)(this->sprite.spriteAnim[(int)this->state].nbAnim.y));
		this->sprite.anim->CreateLeftToRight(this->sprite.spriteAnim[(int)this->state].sprite, (const unsigned char)(this->sprite.spriteAnim[(int)this->state].nbAnim.x), (const unsigned char)(this->sprite.spriteAnim[(int)this->state].nbAnim.y), this->sprite.spriteAnim[(int)this->state].animDim);
		this->sprite.spriteAnim[(int)this->state].sprite.setTextureRect(rect);
		this->sprite.anim->currentFrame = 0;

		this->hitbox->setSize(sf::Vector2f{ this->sprite.spriteAnim[(int)this->state].animDim.x / this->sprite.spriteAnim[(int)this->state].nbAnim.x, this->sprite.spriteAnim[(int)this->state].animDim.y / this->sprite.spriteAnim[(int)this->state].nbAnim.y });
		this->hitbox->setOrigin(this->hitbox->getSize().x / 2, this->hitbox->getSize().y);
		this->lastState = this->state;
	}
}

void Scarecrow::InitNewAnimStatus()
{
	sf::IntRect rect = sf::IntRect(0, 0, (int)(this->statusSprite.spriteAnim[(int)(this->status) - 1].animDim.x / this->statusSprite.spriteAnim[(int)(this->status) - 1].nbAnim.x), (int)(this->statusSprite.spriteAnim[(int)(this->status) - 1].nbAnim.y));
	this->statusSprite.anim->CreateLeftToRight(this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite, (const unsigned char)(this->statusSprite.spriteAnim[(int)(this->status) - 1].nbAnim.x), (const unsigned char)(this->statusSprite.spriteAnim[(int)(this->status) - 1].nbAnim.y), this->statusSprite.spriteAnim[(int)(this->status) - 1].animDim);
	this->statusSprite.anim->currentFrame = 0;
	this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite.setTextureRect(rect);
	this->initStatus = true;
}

void Scarecrow::UpdateHitBox()
{
	this->hitbox->setPosition(this->pos);
	this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->pos);
}

void Scarecrow::UpdateHit()
{
	if (this->state == StateScarecrow::Hit && this->sprite.anim->currentFrame == this->sprite.anim->maxFrame)
	{
		this->state = StateScarecrow::Idle;
	}
	else if (this->state == StateScarecrow::Hit)
	{
		this->sprite.spriteAnim[(int)this->state].sprite.setPosition(sf::Vector2f{ this->pos.x - 25.f,this->pos.y });
	}
}

void Scarecrow::UpdateDeath()
{
	if (this->life <= 0 && this->state != StateScarecrow::Death)
	{
		this->state = StateScarecrow::Death;
		InitNewAnim();
	}
}

void Scarecrow::DeathAnim()
{
	if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame && !this->ligne)
	{
		this->sprite.anim->currentFrame = 0;
		this->ligne = 1;
	}
	else if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame && this->ligne)
	{
		this->canDelete = true;
	}
}

void Scarecrow::Display(sf::RenderWindow& _window, float _dt)
{
	if (this->state != StateScarecrow::Death)
	{
		UpdateAnim(_dt);
		UpdateHitBox();
		UpdateHit();
		UpdateStatus(_dt);
		Utils::BlitSprite(this->sprite.spriteAnim[(int)this->state].sprite, sf::Vector2f{ this->sprite.spriteAnim[(int)this->state].sprite.getPosition().x, this->sprite.spriteAnim[(int)this->state].sprite.getPosition().y }, 0, _window);
		DisplayStatus(_window);
		UpdateDeath();
	}
	else
	{
		UpdateAnim(_dt);
		UpdateHitBox();
		DeathAnim();
		Utils::BlitSprite(this->sprite.spriteAnim[(int)this->state].sprite, sf::Vector2f{ this->sprite.spriteAnim[(int)this->state].sprite.getPosition().x, this->sprite.spriteAnim[(int)this->state].sprite.getPosition().y }, 0, _window);
	}
}

void Scarecrow::Display(sf::RenderTexture& _red, sf::RenderTexture& _blue, float _dt)
{
	UpdateAnim(_dt);
	UpdateHitBox();
	UpdateHit();
	UpdateStatus(_dt);
	Utils::BlitSprite(this->sprite.spriteAnim[(int)this->state].sprite, sf::Vector2f{ (float)this->sprite.spriteAnim[(int)this->state].sprite.getPosition().x + (TROAD_GAP * (-1 * this->sprite.spriteAnim[(int)this->state].sprite.getPosition().y + WORKING_HEIGHT) / 100.f),(float)this->sprite.spriteAnim[(int)this->state].sprite.getPosition().y }, 0, _red);
	Utils::BlitSprite(this->sprite.spriteAnim[(int)this->state].sprite, sf::Vector2f{ (float)this->sprite.spriteAnim[(int)this->state].sprite.getPosition().x - (TROAD_GAP * (-1 * this->sprite.spriteAnim[(int)this->state].sprite.getPosition().y + WORKING_HEIGHT) / 100.f),(float)this->sprite.spriteAnim[(int)this->state].sprite.getPosition().y }, 0, _blue);
	DisplayStatus(_red, _blue);
	this->life = 999'999;
}

sf::Vector2f Scarecrow::GetPos()
{
	return this->pos;
}

void Scarecrow::SetTimerStatus(float _timer)
{
	this->timerStatus = _timer;
}

Status Scarecrow::GetStatus()
{
	return this->status;
}

StateScarecrow Scarecrow::GetState()
{
	return this->state;
}

bool Scarecrow::GetInitStatus()
{
	return this->initStatus;
}

void Scarecrow::SetStatus(Status _status)
{
	this->status = _status;
}

void Scarecrow::SetDamageMagic(int _damage)
{
	this->damageMagic = _damage;
}

void Scarecrow::SetState(StateScarecrow _state)
{
	this->state = _state;
}

bool Scarecrow::GetCanDelete()
{
	return this->canDelete;
}

void Scarecrow::UpdateStatus(float _dt)
{
	if (this->status != Status::None)
	{
		(bool)(this->timerStatus == 1.2f) ? this->timerStatus = this->timerStatus : this->timerStatus = this->timerStatus;

		this->timerStatus -= _dt;
		if (this->timerStatus <= 0.f)
		{
			this->tickDamage = 0;
			this->status = Status::None;
			this->initStatus = false;
		}
		else if (!(this->tickDamage % 35) && this->status != Status::OnIce && this->status != Status::OnElec)
		{
			this->life -= damageMagic;
		}
		this->tickDamage++;
	}
}

void Scarecrow::DisplayStatus(sf::RenderWindow& _window)
{
	if (this->status != Status::None)
	{
		int temp; sf::Vector2f scale;
		if (this->status == Status::OnFire)
		{
			temp = 190;
			scale = { 0.75f,0.75f };
		}
		else if (this->status == Status::OnElec)
		{
			temp = 170;
			scale = { 1.f,1.f };
		}
		else if (this->status == Status::OnIce)
		{
			temp = 170;
			scale = { 0.75f,0.75f };
		}
		else//poison
		{
			temp = 190;
			scale = { 0.75f,0.75f };
		}
		this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite.setScale(scale);

		this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite.setPosition(sf::Vector2f{ this->pos.x,this->pos.y - temp });
		_window.draw(this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite);
	}
}

void Scarecrow::DisplayStatus(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	if (this->status != Status::None)
	{
		int temp; sf::Vector2f scale;
		if (this->status == Status::OnFire)
		{
			temp = 190;
			scale = { 0.75f,0.75f };
		}
		else if (this->status == Status::OnElec)
		{
			temp = 170;
			scale = { 1.f,1.f };
		}
		else if (this->status == Status::OnIce)
		{
			temp = 170;
			scale = { 0.75f,0.75f };
		}
		else//poison
		{
			temp = 190;
			scale = { 0.75f,0.75f };
		}
		this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite.setScale(scale);

		this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite.setPosition(sf::Vector2f{ this->pos.x + 625,this->pos.y - temp });


		Utils::BlitSprite(this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite, sf::Vector2f{ (float)this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite.getPosition().x + (TROAD_GAP * (-1 * this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite.getPosition().y + WORKING_HEIGHT)),(float)this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite.getPosition().y }, 0, _red);
		Utils::BlitSprite(this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite, sf::Vector2f{ (float)this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite.getPosition().x - (TROAD_GAP * (-1 * this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite.getPosition().y + WORKING_HEIGHT)),(float)this->statusSprite.spriteAnim[(int)(this->status) - 1].sprite.getPosition().y }, 0, _blue);

	}
}