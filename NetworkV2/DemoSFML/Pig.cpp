#include "stdafx.h"
#include "Pig.hpp"
#include "Player.hpp"

Pig::Pig(Mob* _mob, PigSprite* _sprite)
{
	//init mob variable
	this->mob = _mob;
	this->mob->speed = { 100.f,75.f };
	this->mob->inAir = false;
	this->mob->state = MobState::Alive;
	this->mob->timerKnockBack = TimerKnockBack;
	this->mob->timerInAir = 0.f;

	//init hazel variable
	this->state = PigState::Target;
	this->targetPlayer = 0;
	this->drawFx = false;
	this->drawFx2 = false;
	this->timerAttack = TimerAttack;
	this->posDeath = { 0.f,0.f };
	this->posAttack = { 0.f,0.f };
	this->ligneFx = 0;

	//anim
	this->sprite = *_sprite;
	this->sprite.anim = new Animation;
	this->sprite.animFx = new Animation;
	this->sprite.animFxHit = new Animation;
	this->sprite.spriteFx.sprite.setScale(0.1f, 0.1f);
	InitNewAnim();
	CreateAnimFxHit();

	//normal attack
	this->mob->hitBoxAttack->setSize(sf::Vector2f{ 40.f, 190.f });
	this->mob->hitBoxAttack->setOrigin(sf::Vector2f{ this->mob->hitBoxAttack->getSize().x / 2,this->mob->hitBoxAttack->getSize().y });
	DestroyAttack();

}

void Pig::Update(std::vector<Player*>& _tabPlayer, float _dt, Wave _wave, Audio& _audio)
{
	sf::Vector2f posPlayer = _tabPlayer[this->targetPlayer]->GetPos();

	switch (this->state)
	{
	case PigState::Target:
		UpdateTargetPlayer(_tabPlayer);
		Move(posPlayer, _dt);
		UpdateTimer(_dt);
		ChangeState(posPlayer);
		break;

	case PigState::Attack:
		Attack(_dt);
		break;

	case PigState::Death:
		UpdateDeath(_dt);
		break;

	case PigState::InFall:
		UpdateInFall(_dt);
		break;

	case PigState::InUp:
		UpdateInUp(_dt);
		break;

	case PigState::Knockback:
		UpdateKnockback(_dt);
		break;

	case PigState::Knockdown:
		UpdateKnockdown(_dt);
		break;

	default:
		break;
	}

	UpdateDirection(posPlayer);
	if (this->state != PigState::Attack)
	{
		DestroyAttack();
	}
	UpdateAnim(_dt);
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

void Pig::Display(sf::RenderWindow& _window)
{
	DisplayEntity(_window);
	DisplayFx(_window);
	DisplayStatus(_window);
}

void Pig::Display(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	DisplayFx(_red, _blue);
	DisplayEntity(_red, _blue);
	DisplayStatus(_red, _blue);
}

void Pig::DisplayFx(sf::RenderWindow& _window)
{
	if (this->drawFx)
	{
		if (this->mob->scaleSprite.x > 0)
		{
			this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->pos.x + 400.f,this->mob->pos.y + 25.f });
		}
		else
		{
			this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->pos.x - 400.f,this->mob->pos.y + 25.f });
		}

		this->sprite.spriteFx.sprite.setScale(this->mob->scaleSprite.x * 0.6f, 0.6f);
		_window.draw(this->sprite.spriteFx.sprite);
	}

	if (this->drawFx2)
	{
		this->mob->inAir ? this->sprite.spriteFxHit.sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x, this->mob->posInAir.y  }) : this->sprite.spriteFxHit.sprite.setPosition(sf::Vector2f{ this->mob->pos.x, this->mob->pos.y });
		_window.draw(this->sprite.spriteFxHit.sprite);
	}
}

void Pig::DisplayEntity(sf::RenderWindow& _window)
{
	this->sprite.spriteAnim[(int)this->state].sprite.setScale(this->mob->scaleSprite);
	(bool)(this->mob->inAir) ? this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->posInAir) : this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos + this->posDeath);
	_window.draw(this->sprite.spriteAnim[(int)this->state].sprite);
}

void Pig::DisplayStatus(sf::RenderWindow& _window)
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

void Pig::DisplayFx(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	if (this->drawFx)
	{
		if (this->mob->scaleSprite.x > 0)
		{
			this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->pos.x + 400.f,this->mob->pos.y + 25.f });
		}
		else
		{
			this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->pos.x - 400.f,this->mob->pos.y + 25.f });
		}

		this->sprite.spriteFx.sprite.setScale(this->mob->scaleSprite.x * 0.6f, 0.6f);
		Utils::BlitSprite(this->sprite.spriteFx.sprite,sf::Vector2f{this->sprite.spriteFx.sprite.getPosition().x - (TROAD_GAP * (-1 * this->sprite.spriteFx.sprite.getPosition().y + WORKING_HEIGHT) / 100.f), this->sprite.spriteFx.sprite.getPosition().y},0, _red);
		Utils::BlitSprite(this->sprite.spriteFx.sprite,sf::Vector2f{this->sprite.spriteFx.sprite.getPosition().x + (TROAD_GAP * (-1 * this->sprite.spriteFx.sprite.getPosition().y + WORKING_HEIGHT) / 100.f), this->sprite.spriteFx.sprite.getPosition().y},0, _blue);
	}
}

void Pig::DisplayEntity(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	this->sprite.spriteAnim[(int)this->state].sprite.setScale(this->mob->scaleSprite);

	this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos.x + (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f), this->mob->pos.y);
	_red.draw(this->sprite.spriteAnim[(int)this->state].sprite);

	this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos.x - (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f), this->mob->pos.y);
	_blue.draw(this->sprite.spriteAnim[(int)this->state].sprite);
}

void Pig::DisplayStatus(sf::RenderTexture& _red, sf::RenderTexture& _blue)
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

void Pig::InitNewAnim()
{
	sf::IntRect rect = sf::IntRect(0, 0, (int)(this->sprite.spriteAnim[(int)this->state].animDim.x / this->sprite.spriteAnim[(int)this->state].nbAnim.x), (int)(this->sprite.spriteAnim[(int)this->state].nbAnim.y));
	this->sprite.anim->CreateLeftToRight(this->sprite.spriteAnim[(int)this->state].sprite, (unsigned char)(this->sprite.spriteAnim[(int)this->state].nbAnim.x), (unsigned char)(this->sprite.spriteAnim[(int)this->state].nbAnim.y), this->sprite.spriteAnim[(int)this->state].animDim);
	this->sprite.spriteAnim[(int)this->state].sprite.setTextureRect(rect);
	this->sprite.anim->currentFrame = 0;

	this->mob->hitbox->setSize(sf::Vector2f{ this->sprite.spriteAnim[(int)this->state].animDim.x / this->sprite.spriteAnim[(int)this->state].nbAnim.x, this->sprite.spriteAnim[(int)this->state].animDim.y / this->sprite.spriteAnim[(int)this->state].nbAnim.y });
	this->mob->hitbox->setOrigin(this->mob->hitbox->getSize().x / 2, this->mob->hitbox->getSize().y);
}

void Pig::UpdateAnim(float _dt)
{
	if (this->mob->status != this->mob->lastStatus)
	{
		this->mob->InitNewAnimStatus();
	}

	if (this->drawFx)
	{
		if (this->sprite.animFx->currentFrame == this->sprite.animFx->maxFrame && this->ligneFx)
		{
			this->drawFx = false;
			this->sprite.animFx->currentFrame = 0;
			this->ligneFx = 0;
		}
		else if (this->sprite.animFx->currentFrame == this->sprite.animFx->maxFrame && !this->ligneFx)
		{
			this->sprite.animFx->currentFrame = 0;
			this->ligneFx++;
		}

		bool temp = true;
		this->sprite.animFx->UpdateLeftToRight(this->sprite.spriteFx.sprite, _dt, this->sprite.spriteFx.speed, this->ligneFx, temp, this->sprite.spriteFx.loop);
	}

	if (this->drawFx2)
	{
		if (this->sprite.animFxHit->currentFrame == this->sprite.animFxHit->maxFrame)
		{
			this->drawFx2 = false;
			this->sprite.animFxHit->currentFrame = 0;
		}

		bool temp = true;
		this->sprite.animFxHit->UpdateLeftToRight(this->sprite.spriteFxHit.sprite, _dt, this->sprite.spriteFxHit.speed, 0, temp, this->sprite.spriteFxHit.loop);
	}

	bool temp = true;
	this->sprite.anim->UpdateLeftToRight(this->sprite.spriteAnim[(int)this->state].sprite, _dt, this->sprite.spriteAnim[(int)this->state].speed, 0, temp, this->sprite.spriteAnim[(int)this->state].loop);

	if (this->mob->status != Status::None)
	{
		bool temp = true;
		this->mob->statusSprite.anim->UpdateLeftToRight(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite, _dt, this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].speed, 0, temp, this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].loop);
	}
}

void Pig::DestroyAttack()
{
	this->mob->hitBoxAttack->setPosition(sf::Vector2f{ -10'00,-10'000 });
}

void Pig::Death(Audio& _audio)
{
	if (!this->mob->inAir && this->mob->life <= 0 && this->state != PigState::Death)
	{
		_audio.PlaySoundNotRepeated("PIG_DEATH", 100.f);
		this->state = PigState::Death;
		this->mob->state = MobState::Death;
		this->mob->scaleSprite = { 1,1 };
		this->posDeath.y += 20.f;
		this->drawFx = false;
		InitNewAnim();
	}
}

void Pig::UpdateDirection(sf::Vector2f _pos)
{
	if (this->state != PigState::Attack && this->state != PigState::Death)
	{
		this->mob->scaleSprite = sf::Vector2f{ (float)(this->mob->Trigo(_pos)),1.f };
	}
}

void Pig::UpdateTargetPlayer(std::vector<Player*>& _player)
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
}

void Pig::Move(sf::Vector2f _posPlayer, float _dt)
{
	sf::Vector2f goalPos = sf::Vector2f{ _posPlayer.x + GoalPos * this->mob->scaleSprite.x,_posPlayer.y };
	float vecY = goalPos.y - this->mob->pos.y;
	sf::Vector2f normVec = Utils::NormalizeVector(sf::Vector2f{ goalPos - this->mob->pos });

	if (std::abs(vecY) >= DistMinMove)
	{
		this->mob->pos.x += normVec.x * this->mob->speed.x * _dt;
		this->mob->pos.y += normVec.y * this->mob->speed.y * _dt;
	}
}

void Pig::MoveAttack(sf::Vector2f _posPlayer, float _dt)
{
	sf::Vector2f goalPos = sf::Vector2f{ _posPlayer.x - FinishAttackBehindPlayer * this->mob->scaleSprite.x,this->mob->pos.y };
	sf::Vector2f normVec = Utils::NormalizeVector(sf::Vector2f{ goalPos - this->mob->pos });

	this->mob->pos.x += normVec.x * this->mob->speed.x * _dt;
	this->mob->pos.y += normVec.y * this->mob->speed.y * _dt;
}

void Pig::UpdateTimer(float _dt)
{
	if (this->timerAttack > 0)
	{
		this->timerAttack -= _dt;
	}
}

void Pig::ChangeState(sf::Vector2f _pos)
{
	if (this->timerAttack <= 0 && CanAttack(_pos))
	{
		this->state = PigState::Attack;
		this->timerAttack = TimerAttack;
		this->mob->speed = sf::Vector2f{ 800.f, 500.f };
		this->posAttack = _pos;
		this->drawFx = true;
		CreateAnimFx();
	}
}

bool Pig::CanAttack(sf::Vector2f _pos)
{
	if (std::abs(_pos.x - this->mob->pos.x) <= DistMinAttack &&
		this->mob->pos.y <= _pos.y + DistAxeY && this->mob->pos.y >= _pos.y - DistAxeY)
	{
		return true;
	}

	return false;
}

void Pig::UpdateDeath(float _dt)
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
	else
	{
		this->posDeath.x += DeathPosMoveX;
		this->mob->decalage.x += DeathPosMoveX;
	}
}

void Pig::Attack(float _dt)
{
	MoveAttack(this->posAttack, _dt);
	this->mob->hitBoxAttack->setPosition(sf::Vector2f{ this->mob->pos.x - (80.f * this->mob->scaleSprite.x),this->mob->pos.y });
	EndAttack(this->posAttack);
}

void Pig::EndAttack(sf::Vector2f _posPlayer)
{
	if (this->mob->scaleSprite.x > 0)
	{
		if (this->mob->pos.x <= _posPlayer.x - FinishAttackBehindPlayer * this->mob->scaleSprite.x)
		{
			this->state = PigState::Target;
			DestroyAttack();
			this->mob->speed = { 100.f,75.f };
		}
	}
	else
	{
		if (this->mob->pos.x >= _posPlayer.x - FinishAttackBehindPlayer * this->mob->scaleSprite.x)
		{
			this->state = PigState::Target;
			DestroyAttack();
			this->mob->speed = { 100.f,75.f };
		}
	}
}

void Pig::SetState(PigState _state)
{
	this->state = _state;
	this->mob->speed = { 100.f,75.f };
}

sf::Vector2f Pig::GetDimSprite()
{
	sf::Vector2u textureSize = this->sprite.spriteAnim[(int)this->state].sprite.getTexture()->getSize();
	return sf::Vector2f(textureSize.x / this->sprite.spriteAnim[(int)this->state].nbAnim.x, textureSize.y / this->sprite.spriteAnim[(int)this->state].nbAnim.y);
}

void Pig::CreateAnimFx()
{
	sf::IntRect rect = sf::IntRect(0, 0, (int)(this->sprite.spriteFx.animDim.x / this->sprite.spriteFx.nbAnim.x), (int)(this->sprite.spriteFx.nbAnim.y));
	this->sprite.animFx->CreateLeftToRight(this->sprite.spriteFx.sprite, (unsigned char)(this->sprite.spriteFx.nbAnim.x), (unsigned char)(this->sprite.spriteFx.nbAnim.y), this->sprite.spriteFx.animDim);
	this->sprite.spriteFx.sprite.setTextureRect(rect);
	this->sprite.animFx->currentFrame = 0;
}

void Pig::CreateAnimFxHit()
{
	sf::IntRect rect = sf::IntRect(0, 0, (int)(this->sprite.spriteFxHit.animDim.x / this->sprite.spriteFxHit.nbAnim.x), (int)(this->sprite.spriteFxHit.nbAnim.y));
	this->sprite.animFxHit->CreateLeftToRight(this->sprite.spriteFxHit.sprite, (unsigned char)(this->sprite.spriteFxHit.nbAnim.x), (unsigned char)(this->sprite.spriteFxHit.nbAnim.y), this->sprite.spriteFxHit.animDim);
	this->sprite.spriteFxHit.sprite.setTextureRect(rect);
	this->sprite.animFxHit->currentFrame = 0;
}

void Pig::SetDrawFx(bool _bool)
{
	this->drawFx2 = _bool;
}

void Pig::UpdateInUp(float _dt)
{
	if (this->mob->timerInAir < 1)
	{
		this->mob->timerInAir += _dt;
		this->mob->posInAir = Easings::Lerp(this->mob->pos, { this->mob->pos.x,this->mob->pos.y - DistanceJumpY }, Easings::OutCirc(this->mob->timerInAir));
	}
	else
	{
		this->mob->timerInAir = 0;
		this->mob->posYFall = this->mob->posInAir.y;
		this->state = PigState::InFall;
		InitNewAnim();
	}
}

void Pig::UpdateInFall(float _dt)
{
	if (this->mob->timerInAir < 1 && this->mob->posInAir.y < this->mob->pos.y)
	{
		this->mob->timerInAir += _dt + (_dt * this->mob->touchInAir);
		this->mob->posInAir = Easings::Lerp({ this->mob->posInAir.x, this->mob->posYFall }, { this->mob->posInAir.x,this->mob->pos.y }, Easings::InCirc(this->mob->timerInAir));
	}
	else
	{
		this->mob->timerInAir = 0.f;
		this->mob->pos = { this->mob->posInAir.x,this->mob->pos.y };

		this->state = PigState::Target;
		InitNewAnim();

		this->mob->inAir = false;
		this->mob->touchInAir = false;
	}
}

void Pig::UpdateKnockback(float _dt)
{
	this->mob->timerKnockBack -= _dt;
	UpdatePosKnockback();
	if (this->mob->timerKnockBack <= 0)
	{
		this->state = PigState::Target;
		this->mob->state = MobState::Alive;
		this->mob->timerKnockBack = TimerKnockBack;
		if (this->mob->inAir)
		{
			this->state = PigState::InFall;
			this->mob->state = MobState::Hit;
			this->mob->timerInAir = 0;
			this->mob->posYFall = this->mob->posInAir.y;
		}
		InitNewAnim();
	}
}

void Pig::UpdateKnockdown(float _dt)
{
	this->mob->timerKnockDown -= _dt;
	if (this->mob->timerKnockDown <= 0)
	{
		this->state = PigState::Target;
		this->mob->state = MobState::Alive;
		InitNewAnim();
	}
}

void Pig::UpdatePosKnockback()
{
	this->mob->pos.x += KnockbackForce * this->mob->direction;
}