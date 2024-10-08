#include "stdafx.h"
#include "Bird.hpp"
#include "Player.hpp"

Bird::Bird(Mob* _mob, BirdSprite* _birdSprite)
{
	//init variable
	this->mob = _mob;
	this->mob->speed = { 75.f,50.f };
	this->mob->inAir = false;
	this->mob->state = MobState::Alive;
	this->mob->magicUse = MagicUse::Poison;
	this->mob->timerInAir = 0.f;

	//init hazel variable
	this->state = BirdState::Idle;
	this->targetPlayer = 0;
	this->initFire = false;
	this->drawFx = false;
	this->timerAttack = TimerAttack;
	this->timerHit = TimerHit;

	//anim & sprite
	this->sprite = *_birdSprite;
	this->sprite.anim = new Animation;
	this->sprite.animFx = new Animation;
	InitNewAnim();
	CreateAnimFx();
}

void Bird::Update(std::vector<Player*>& _tabPlayer, std::vector<Projectile>& _tabProj, float _dt, Wave _wave, Audio& _audio)
{
	sf::Vector2f posPlayer = _tabPlayer[this->targetPlayer]->GetPos();

	switch (this->state)
	{
	case BirdState::Idle:
		UpdateIdle(posPlayer.y, _dt);
		ChangeState(posPlayer.y);
		UpdateTimer(_dt);
		break;

	case BirdState::Attack:
		Fire(_dt,_tabProj, _audio,_tabPlayer);
		break;

	case BirdState::Death:
		UpdateDeath(_dt);
		break;

	case BirdState::Hit:
		UpdateHit(_dt);
		break;

	default:
		break;
	}

	UpdateAnim(_dt);
	this->lastState = this->state;
	UpdateDirection(posPlayer);
	IsDeath(_audio);
}

void Bird::Display(sf::RenderWindow& _window)
{
	DisplayEntity(_window);
	DisplayFx(_window);
	DisplayStatus(_window);
}

void Bird::Display(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	DisplayEntity(_red, _blue);
	DisplayStatus(_red, _blue);
	DisplayFx(_red, _blue);
}

void Bird::SetState(BirdState _state)
{
	this->state = _state;
}

void Bird::SetDrawFx(bool _bool)
{
	this->drawFx = _bool;
}

void Bird::DisplayFx(sf::RenderWindow& _window)
{
	if (this->drawFx)
	{
		this->mob->inAir ? this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x, this->mob->posInAir.y  }) : this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->pos.x, this->mob->pos.y });
		_window.draw(this->sprite.spriteFx.sprite);
	}
}

void Bird::DisplayEntity(sf::RenderWindow& _window)
{
	this->sprite.spriteAnim[(int)this->state].sprite.setScale(this->mob->scaleSprite);
	(bool)(this->mob->inAir) ? this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->posInAir) : this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos);
	_window.draw(this->sprite.spriteAnim[(int)this->state].sprite);
}

void Bird::DisplayStatus(sf::RenderWindow& _window)
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

void Bird::DisplayFx(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	if (this->drawFx)
	{
		this->mob->inAir ? this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x, this->mob->posInAir.y }) : this->sprite.spriteFx.sprite.setPosition(sf::Vector2f{ this->mob->pos.x, this->mob->pos.y });
		
		Utils::BlitSprite(this->sprite.spriteFx.sprite, sf::Vector2f{ this->sprite.spriteFx.sprite.getPosition().x + (TROAD_GAP * (-1 * this->sprite.spriteFx.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->sprite.spriteFx.sprite.getPosition().y }, 0, _red);
		Utils::BlitSprite(this->sprite.spriteFx.sprite, sf::Vector2f{ this->sprite.spriteFx.sprite.getPosition().x - (TROAD_GAP * (-1 * this->sprite.spriteFx.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->sprite.spriteFx.sprite.getPosition().y }, 0, _blue);
	}
}

void Bird::DisplayEntity(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	this->sprite.spriteAnim[(int)this->state].sprite.setScale(this->mob->scaleSprite);
	this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos.x + (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f), this->mob->pos.y);
	_red.draw(this->sprite.spriteAnim[(int)this->state].sprite);

	this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos.x - (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f), this->mob->pos.y);
	_blue.draw(this->sprite.spriteAnim[(int)this->state].sprite);
}

void Bird::DisplayStatus(sf::RenderTexture& _red, sf::RenderTexture& _blue)
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

void Bird::InitNewAnim()
{
	sf::IntRect rect = sf::IntRect(0, 0, (int)(this->sprite.spriteAnim[(int)this->state].animDim.x / this->sprite.spriteAnim[(int)this->state].nbAnim.x), (int)(this->sprite.spriteAnim[(int)this->state].nbAnim.y));
	this->sprite.anim->CreateLeftToRight(this->sprite.spriteAnim[(int)this->state].sprite, (unsigned char)(this->sprite.spriteAnim[(int)this->state].nbAnim.x), (unsigned char)(this->sprite.spriteAnim[(int)this->state].nbAnim.y), this->sprite.spriteAnim[(int)this->state].animDim);
	this->sprite.spriteAnim[(int)this->state].sprite.setTextureRect(rect);
	this->sprite.anim->currentFrame = 0;
	if (this->lastState == BirdState::Attack)
	{
		this->sprite.anim->currentFrame = 5;
	}

	this->mob->hitbox->setSize(sf::Vector2f{ this->sprite.spriteAnim[(int)this->state].animDim.x / this->sprite.spriteAnim[(int)this->state].nbAnim.x, this->sprite.spriteAnim[(int)this->state].animDim.y / this->sprite.spriteAnim[(int)this->state].nbAnim.y });
	this->mob->hitbox->setOrigin(this->mob->hitbox->getSize().x / 2, this->mob->hitbox->getSize().y);
}

void Bird::UpdateDirection(sf::Vector2f _pos)
{
	if (this->state != BirdState::Attack && this->state != BirdState::Death)
	{
		this->mob->scaleSprite = sf::Vector2f{ (float)(this->mob->Trigo(_pos)),1.f };
	}
}

void Bird::UpdateAnim(float _dt)
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
	this->sprite.anim->UpdateLeftToRight(this->sprite.spriteAnim[(int)this->state].sprite, _dt, this->sprite.spriteAnim[(int)this->state].speed, 0, temp,this->sprite.spriteAnim[(int)this->state].loop);

	if (this->mob->status != Status::None)
	{
		bool temp = true;
		this->mob->statusSprite.anim->UpdateLeftToRight(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite, _dt, this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].speed, 0, temp, this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].loop);
	}
}

void Bird::IsDeath(Audio& _audio)
{
	if (this->mob->life <= 0 && this->state != BirdState::Death)
	{
		_audio.PlaySoundNotRepeated("BIRD_DEATH", 100.f);
		this->mob->state = MobState::Death;
		this->state = BirdState::Death;
		this->mob->pos.y += 100.f;
	}
}

void Bird::UpdateDeath(float _dt)
{
	if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame)
	{
		this->sprite.spriteAnim[(int)this->state].sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)(this->sprite.spriteAnim[(int)this->state].sprite.getColor().a - _dt * SpeedFadeDeath)));
		if (this->sprite.spriteAnim[(int)this->state].sprite.getColor().a <= 0)
		{
			this->mob->state = MobState::Dispawn;
		}
	}
}

void Bird::ChangeState(float _playerY)
{
	if (this->timerAttack <= 0 && CanFire(_playerY))
	{
		this->state = BirdState::Attack;
		this->timerAttack = TimerAttack;
	}
}

bool Bird::CanFire(float _playerY)
{
	if (this->mob->pos.y <= _playerY + DistAxeY && this->mob->pos.y >= _playerY - DistAxeY)
	{
		return true;
	}
	return false;
}

void Bird::Move(sf::Vector2f _vec, float _dt)
{
	sf::Vector2f normVec = Utils::NormalizeVector(sf::Vector2f{ _vec });

	this->mob->pos.x += normVec.x * this->mob->speed.x * _dt;
	this->mob->pos.y += normVec.y * this->mob->speed.y * _dt;
}

void Bird::UpdateIdle(float _posPlayerY, float _dt)
{
	sf::Vector2f goalPos = { (float)( this->mob->pos.x), _posPlayerY };//just to only get y axis
	sf::Vector2f vec = goalPos - this->mob->pos;

	Move(vec,_dt);
}

void Bird::Fire(float _dt, std::vector<Projectile>& _projectile, Audio& _audio, std::vector<Player*>& _tabPlayer)
{
	if (this->sprite.anim->currentFrame == 5)
	{
		this->state = BirdState::Idle;
		this->initFire = false;
		this->sprite.anim->currentFrame = 5;
		this->targetPlayer = (rand() % _tabPlayer.size());
	}
	else if (this->sprite.anim->currentFrame == 3 && !this->initFire)
	{
		//create arrow
		Projectile temp = { this->mob->Trigo(this->mob->scaleSprite),{ this->mob->pos },false };

		_audio.PlaySoundNotRepeated("BIRD_ATTACK", 100.f);

		temp.autor = AutorProjectile::MOB;
		temp.projetileType = TypeProjectile::DISTANCE_MAGIC_FIRE;

		temp.posAutor = this->mob->pos;
		temp.pos = sf::Vector2f{ this->mob->pos.x - (10.f * this->mob->scaleSprite.x),this->mob->pos.y - 70.f };
		temp.hitBox = new sf::RectangleShape(sf::Vector2f{ (float)(85),(float)(48) });
		temp.hitBox->setOrigin(temp.hitBox->getSize().x / 2, temp.hitBox->getSize().y);
		temp.damage = this->mob->damaged;
		temp.asAnime = true;
		temp.sprite = this->sprite.spriteAnimprojectile.sprite;
		temp.speedAnime = this->sprite.spriteAnimprojectile.speed;
		temp.sprite.setScale(-this->mob->scaleSprite.x, 1);
		temp.direction = this->mob->scaleSprite.x;

		temp.projectileAnime = new Animation;
		sf::IntRect rect = sf::IntRect(0, 0, (int)(this->sprite.spriteAnimprojectile.animDim.x / this->sprite.spriteAnimprojectile.nbAnim.x), (int)(this->sprite.spriteAnimprojectile.nbAnim.y));
		this->sprite.spriteAnim[(int)this->state].sprite.setTextureRect(rect);
		temp.projectileAnime->CreateLeftToRight(this->sprite.spriteAnimprojectile.sprite, (unsigned char)(this->sprite.spriteAnimprojectile.nbAnim.x), (unsigned char)(this->sprite.spriteAnimprojectile.nbAnim.y), this->sprite.spriteAnimprojectile.animDim);

		_projectile.push_back(temp);
		this->initFire = true;
	}
}

void Bird::UpdateTimer(float _dt)
{
	if (this->timerAttack > 0)
	{
		this->timerAttack -= _dt;
	}
}

void Bird::UpdateHit(float _dt)
{
	this->timerHit -= _dt;
	if (this->timerHit <= 0)
	{
		this->state = BirdState::Idle;
		this->timerHit = TimerHit;
	}
}

void Bird::CreateAnimFx()
{
	sf::IntRect rect = sf::IntRect(0, 0, (int)(this->sprite.spriteFx.animDim.x / this->sprite.spriteFx.nbAnim.x), (int)(this->sprite.spriteFx.nbAnim.y));
	this->sprite.animFx->CreateLeftToRight(this->sprite.spriteFx.sprite, (unsigned char)(this->sprite.spriteFx.nbAnim.x), (unsigned char)(this->sprite.spriteFx.nbAnim.y), this->sprite.spriteFx.animDim);
	this->sprite.spriteFx.sprite.setTextureRect(rect);
	this->sprite.animFx->currentFrame = 0;
}