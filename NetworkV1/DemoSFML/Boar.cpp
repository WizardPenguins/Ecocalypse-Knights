#include "Boar.hpp"
#include "stdafx.h"
#include "Player.hpp"

Boar::Boar(Mob* _mob, BoarSprite* _sprite)
{
	//init mob variable
	this->mob = _mob;
	this->mob->speed = { 150.f,125.f };
	this->mob->state = MobState::Alive;
	this->mob->nbPhase = 2;

	//init hazel variable
	this->state = BoarState::Idle;
	this->targetPlayer = 0;
	this->nbAttack = 0;
	this->secondPhase = false;
	this->doZenitsu = false;
	this->slashVertiAlive = false;
	this->slashHorizonAlive = false;
	this->leak = false;
	this->isLeaking = false;

	//timer
	this->timerAttack = TimerAttack;
	this->timerSlashH = TimerHorizontalSlash;
	this->timerSlashV = TimerVerticalSlash;
	this->timerZenitsu = TimerZenitsu;
	this->timerKnockDown = TimerKnockdown;
	this->timerSlashZenitsu = TimerAfterZenitsu;
	this->timerShield = TimerShield;

	//anim
	this->sprite = *_sprite;
	this->sprite.anim = new Animation;
	this->sprite.animSlash = new Animation;
	this->sprite.slashHitBox = new sf::RectangleShape;
	InitNewAnim();

	//normal attack
	this->mob->hitBoxAttack->setSize(sf::Vector2f{ 120.f,160.f });
	this->mob->hitBoxAttack->setOrigin(sf::Vector2f{ this->mob->hitBoxAttack->getSize().x / 2 + 5.f,150.f });
	DestroyAttack();

	//shadow slah horizontale
	this->shadowH = Utils::LoadSprite("Assets/Images/Entities/Mobs/Boar/Shadow_Slash.png", CENTER_FOOT);
	this->shadowH.setColor(sf::Color{ 0,0,0,100 });
}

void Boar::Update(std::vector<Player*>& _tabPlayer, float _dt, Wave _wave, Audio& _audio)
{
	zenitsuTimerSound += _dt;
	attack1TimerSound += _dt;
	attack2TimerSound += _dt;
	breathlessTimerSound += _dt;
	groinkTimerSound += _dt;

	if (groinkTimerSound > 2.f && this->state != BoarState::Death)
	{
		_audio.PlaySoundNotRepeated("BOAR_GROINK_" + std::to_string(rand() % 8 + 1), 100.f);
		groinkTimerSound = 0.f;
	}

	sf::Vector2f posPlayer = _tabPlayer[this->targetPlayer]->GetPos();

	switch (this->state)
	{
	case BoarState::Idle:
		UpdateIdle(_tabPlayer);
		UpdateIdleSecondPhase(_tabPlayer, _dt, _audio);
		break;

	case BoarState::Target:
		if (!this->isLeaking)
		{
			UpdateTargetFirstPhase(_tabPlayer, _dt, _audio);
			UpdateTargetSecondPhase(_tabPlayer, _wave, _dt);
			UpdateTimer(_dt);
			ChangeState(_tabPlayer, _audio);
		}
		else
		{
			RunAway(_dt,_wave);
		}
		break;

	case BoarState::Back:
		UpdateBack(_wave, _dt, _audio);
		break;

	case BoarState::AttackNormal:
		UpdateNormalAttack(posPlayer, _audio);
		break;

	case BoarState::AttackH:
		UpdateSlashAttackHorizon(posPlayer);
		break;

	case BoarState::AttackV:
		UpdateSlashAttackVerti(posPlayer);
		break;

	case BoarState::AttackZenitsu:
		UpdateZenitsuAttack(posPlayer, _dt, _audio);
		break;

	case BoarState::KnockDown:
		UpdateTimerKnockDown(_dt, _audio);
		break;

	case BoarState::Shield:
		UpdateShield(_tabPlayer,_dt,_audio);
		break;

	case BoarState::Death:
		UpdateDeath(_dt);
		break;

	default:
		break;
	}

	ChangePhase();
	UpdateSlash(_wave, _dt);
	UpdateDirection(posPlayer);
	if (this->state != BoarState::AttackNormal)
	{
		DestroyAttack();
	}
	Death(_audio);
	PlayerTargetIsDead(_tabPlayer);
	UpdateAnim(_dt);

	this->lastState = this->state;
}

void Boar::Display(sf::RenderWindow& _window)
{
	DisplayEntity(_window);
	DisplayStatus(_window);
}

void Boar::Display(sf::RenderTarget& _red, sf::RenderTarget& _blue)
{
	DisplayEntity(_red, _blue);
	DisplayStatus(_red, _blue);
}

void Boar::DisplaySlash(sf::RenderWindow& _window)
{
	if (this->slashHorizonAlive)
	{
		this->shadowH.setScale({ this->sprite.dir,1.f });
		this->sprite.spriteSlash[0].sprite.setScale({ -this->sprite.dir,1.f });

		(bool)(this->sprite.dir > 0) ? this->shadowH.setPosition(sf::Vector2f{ this->sprite.slashHitBox->getPosition().x + 25.f,this->sprite.slashHitBox->getPosition().y + 35.f }) : this->shadowH.setPosition(sf::Vector2f{ this->sprite.slashHitBox->getPosition().x - 25.f,this->sprite.slashHitBox->getPosition().y + 35.f });
		_window.draw(this->shadowH);

		this->sprite.spriteSlash[0].sprite.setPosition(this->sprite.slashHitBox->getPosition().x + 161, this->sprite.slashHitBox->getPosition().y - 270);
		_window.draw(this->sprite.spriteSlash[0].sprite);
	}

	if (this->slashVertiAlive)
	{
		this->sprite.spriteSlash[1].sprite.setScale({ -this->sprite.dir,1.f });
		this->sprite.spriteSlash[1].sprite.setPosition({ this->sprite.slashHitBox->getPosition() });
		_window.draw(this->sprite.spriteSlash[1].sprite);
	}
}

void Boar::DisplaySlash(sf::RenderTarget& _red, sf::RenderTarget& _blue)
{
	if (this->slashHorizonAlive)
	{
		this->shadowH.setScale({ this->sprite.dir,1.f });
		this->sprite.spriteSlash[0].sprite.setScale({ -this->sprite.dir,1.f });

		(bool)(this->sprite.dir > 0) ? this->shadowH.setPosition(sf::Vector2f{ this->sprite.slashHitBox->getPosition().x + (TROAD_GAP * (-1 * this->sprite.slashHitBox->getPosition().y + WORKING_HEIGHT) / 100.f) + 25.f,this->sprite.slashHitBox->getPosition().y + 35.f }) : this->shadowH.setPosition(sf::Vector2f{ this->sprite.slashHitBox->getPosition().x + (TROAD_GAP * (-1 * this->sprite.slashHitBox->getPosition().y + WORKING_HEIGHT) / 100.f) - 25.f,this->sprite.slashHitBox->getPosition().y + 35.f });
		this->sprite.spriteSlash[0].sprite.setPosition(this->sprite.slashHitBox->getPosition().x + 161, this->sprite.slashHitBox->getPosition().y - 270);

		_red.draw(this->shadowH);
		Utils::BlitSprite(this->sprite.spriteSlash[0].sprite, sf::Vector2f{ this->sprite.spriteSlash[0].sprite.getPosition().x - (TROAD_GAP * (-1 * this->sprite.spriteSlash[0].sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->sprite.spriteSlash[0].sprite.getPosition().y }, 0,_red);

		(bool)(this->sprite.dir > 0) ? this->shadowH.setPosition(sf::Vector2f{ this->sprite.slashHitBox->getPosition().x - (TROAD_GAP * (-1 * this->sprite.slashHitBox->getPosition().y + WORKING_HEIGHT) / 100.f) + 25.f,this->sprite.slashHitBox->getPosition().y + 35.f }) : this->shadowH.setPosition(sf::Vector2f{ this->sprite.slashHitBox->getPosition().x + (TROAD_GAP * (-1 * this->sprite.slashHitBox->getPosition().y + WORKING_HEIGHT) / 100.f) - 25.f,this->sprite.slashHitBox->getPosition().y + 35.f });

		_blue.draw(this->shadowH);
		Utils::BlitSprite(this->sprite.spriteSlash[0].sprite, sf::Vector2f{ this->sprite.spriteSlash[0].sprite.getPosition().x + (TROAD_GAP * (-1 * this->sprite.spriteSlash[0].sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->sprite.spriteSlash[0].sprite.getPosition().y }, 0, _blue);
	}

	if (this->slashVertiAlive)
	{
		this->sprite.spriteSlash[1].sprite.setScale({ -this->sprite.dir,1.f });

		this->sprite.spriteSlash[1].sprite.setPosition({ this->sprite.slashHitBox->getPosition().x + (TROAD_GAP * (-1 * this->sprite.slashHitBox->getPosition().y + WORKING_HEIGHT) / 100.f), this->sprite.slashHitBox->getPosition().y });
		_red.draw(this->sprite.spriteSlash[1].sprite);

		this->sprite.spriteSlash[1].sprite.setPosition({ this->sprite.slashHitBox->getPosition().x - (TROAD_GAP * (-1 * this->sprite.slashHitBox->getPosition().y + WORKING_HEIGHT) / 100.f), this->sprite.slashHitBox->getPosition().y });
		_blue.draw(this->sprite.spriteSlash[1].sprite);
	}
}

void Boar::DisplayEntity(sf::RenderWindow& _window)
{
	this->sprite.spriteAnim[(int)this->state].sprite.setScale(this->mob->scaleSprite);
	(bool)(this->mob->inAir) ? this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->posInAir) : this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos);
	_window.draw(this->sprite.spriteAnim[(int)this->state].sprite);
}

void Boar::DisplayStatus(sf::RenderWindow& _window)
{
	if (this->mob->status != Status::None && this->state != BoarState::Death && this->mob->state != MobState::Dispawn)
	{
		this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setColor(sf::Color(255, 255, 255, 230));
		int temp; sf::Vector2f scale;
		if (this->mob->status == Status::OnFire)
		{
			temp = 10;
			scale = { 1.f,1.f };
		}
		else if (this->mob->status == Status::OnElec)
		{
			temp = 120;
			scale = { 1.f,1.f };
		}
		else if (this->mob->status == Status::OnIce)
		{
			temp = 120;
			scale = { 1.f,1.f };
		}
		else
		{
			temp = 60;
			scale = { 1.f,1.f };
		}

		this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setScale(scale);
		this->mob->inAir ? this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x,this->mob->posInAir.y - this->mob->GetDimSprite().y + temp }) : this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->pos.x,this->mob->pos.y - this->mob->GetDimSprite().y + temp });
		_window.draw(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite);
	}
}

void Boar::DisplayEntity(sf::RenderTarget& _red, sf::RenderTarget& _blue)
{
	this->sprite.spriteAnim[(int)this->state].sprite.setScale(this->mob->scaleSprite);

	this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos.x + (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f), this->mob->pos.y);
	_red.draw(this->sprite.spriteAnim[(int)this->state].sprite);

	this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos.x - (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f), this->mob->pos.y);
	_blue.draw(this->sprite.spriteAnim[(int)this->state].sprite);
}

void Boar::DisplayStatus(sf::RenderTarget& _red, sf::RenderTarget& _blue)
{
	if (this->mob->status != Status::None && this->state != BoarState::Death && this->mob->state != MobState::Dispawn)
	{
		this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setColor(sf::Color(255, 255, 255, 230));
		int temp; sf::Vector2f scale;
		if (this->mob->status == Status::OnFire)
		{
			temp = 10;
			scale = { 1.f,1.f };
		}
		else if (this->mob->status == Status::OnElec)
		{
			temp = 120;
			scale = { 1.f,1.f };
		}
		else if (this->mob->status == Status::OnIce)
		{
			temp = 120;
			scale = { 1.f,1.f };
		}
		else
		{
			temp = 60;
			scale = { 1.f,1.f };
		}

		this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setScale(scale);
		this->mob->inAir ? this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x + (TROAD_GAP * (-1 * this->mob->posInAir.y + WORKING_HEIGHT) / 100.f),this->mob->posInAir.y - this->mob->GetDimSprite().y + temp }) : this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->pos.x + (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f),this->mob->pos.y - this->mob->GetDimSprite().y + temp });
		_red.draw(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite);

		this->mob->inAir ? this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x - (TROAD_GAP * (-1 * this->mob->posInAir.y + WORKING_HEIGHT) / 100.f),this->mob->posInAir.y - this->mob->GetDimSprite().y + temp }) : this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->pos.x - (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f),this->mob->pos.y - this->mob->GetDimSprite().y + temp });
		_blue.draw(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite);
	}
}

void Boar::DestroyAttack()
{
	this->mob->hitBoxAttack->setPosition(sf::Vector2f{ -10'00,-10'000 });
}

void Boar::UpdateDirection(sf::Vector2f _pos)
{
	if (this->state != BoarState::KnockDown && this->state != BoarState::AttackNormal && this->state != BoarState::AttackH && this->state != BoarState::AttackV && this->state != BoarState::Death && !this->isLeaking)
	{
		this->mob->scaleSprite = sf::Vector2f{ (float)(this->mob->Trigo(_pos)),1.f };
	}
}

void Boar::UpdateTargetPlayer(std::vector<Player*>& _player)
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
}

void Boar::Death(Audio& _audio)
{
	if (this->mob->life <= 0 && (this->state == BoarState::Idle || this->state == BoarState::Target))
	{
		this->state = BoarState::Death;
		this->mob->scaleSprite = { 1,1 };
		this->mob->state = MobState::Death;
		_audio.PlaySoundNotRepeated("BOAR_DEATH", 100.f);
	}
}

void Boar::UpdateIdleSecondPhase(std::vector<Player*>& _player, float _dt, Audio& _audio)
{
	if (this->secondPhase)
	{
		ReturnToTarget(_player[this->targetPlayer]->GetPos().y);
		UpdateTimer(_dt);
		ChangeState(_player, _audio);
	}
}

void Boar::ReturnToTarget(float _posY)
{
	float vecPosPlayer = std::abs(_posY - this->mob->pos.y);
	if (std::abs(vecPosPlayer) > DistMinMove)
	{
		this->state = BoarState::Target;
	}
}

void Boar::PlayerTargetIsDead(std::vector<Player*>& _player)
{
	if (_player[this->targetPlayer]->GetState() == DEAD )
	{
		this->targetPlayer = (int)((this->targetPlayer + 1) % _player.size());
	}
}

void Boar::MoveBack(sf::Vector2f _pos, float _dt)
{
	sf::Vector2f normVec = Utils::NormalizeVector(sf::Vector2f{ _pos - this->mob->pos });

	this->mob->pos.x += normVec.x * this->mob->speed.x * _dt;
	this->mob->pos.y += normVec.y * this->mob->speed.y * _dt;
}

void Boar::MoveSecondPhase(sf::Vector2f _pos, float _dt)
{
	sf::Vector2f normVec = Utils::NormalizeVector(sf::Vector2f{ _pos - this->mob->pos });

	this->mob->pos.x += normVec.x * this->mob->speed.x * _dt;
	this->mob->pos.y += normVec.y * this->mob->speed.y * _dt;
}

void Boar::RunAway(float _dt,Wave _wave)
{
	sf::Vector2f goalPos = { (float)((int)(_wave + 2) * WORKING_WIDTH), 900.f };

	MoveBack(goalPos,_dt);

	if (this->mob->pos.x >= (float)((int)(_wave + 1) * WORKING_WIDTH) + 100.f)
	{
		this->mob->state = MobState::Dispawn;
	}
}

void Boar::UpdateTimer(float _dt)
{
	if (this->timerAttack > 0)
	{
		this->timerAttack -= _dt;
	}

	if (this->secondPhase)
	{
		if (this->timerZenitsu > 0)
		{
			this->timerZenitsu -= _dt;
		}

		if (this->timerSlashV > 0)
		{
			this->timerSlashV -= _dt;
		}

		if (this->timerSlashH > 0)
		{
			this->timerSlashH -= _dt;
		}
	}
}

void Boar::ChangeState(std::vector<Player*>& _player, Audio& _audio)
{
	if (this->timerAttack <= 0 && CanAttack(_player[this->targetPlayer]->GetPos()))
	{
		this->state = BoarState::AttackNormal;
		this->timerAttack = TimerAttack;
	}

	if (this->timerSlashH <= 0 && !this->slashVertiAlive)
	{
		ChooseAPlayer(_player);
		this->state = BoarState::AttackH;
		this->timerSlashH = TimerHorizontalSlash;
		InitNewSlash(this->sprite.spriteSlash[0], this->sprite.animSlash, this->sprite.slashHitBox);
		_audio.PlaySoundNotRepeated("BOAR_ATTACK1", 100.f);
	}

	if (this->timerSlashV <= 0 && !this->slashHorizonAlive)
	{
		ChooseAPlayer(_player);
		this->state = BoarState::AttackV;
		this->timerSlashV = TimerVerticalSlash;
		InitNewSlash(this->sprite.spriteSlash[1], this->sprite.animSlash, this->sprite.slashHitBox);
		_audio.PlaySoundNotRepeated("BOAR_ATTACK2", 100.f);
	}

	if (this->timerZenitsu <= 0)
	{
		ChooseAPlayer(_player);
		this->state = BoarState::AttackZenitsu;
		this->timerZenitsu = TimerZenitsu;
	}

	if (this->nbAttack >= MaxAttackShield && !this->secondPhase)
	{
		this->nbAttack = 0;
		this->state = BoarState::Shield;
	}
}

bool Boar::CanAttack(sf::Vector2f _pos)
{
	if (std::abs(_pos.x - this->mob->pos.x) <= DistMinAttack &&
		this->mob->pos.y <= _pos.y + DistAxeY && this->mob->pos.y >= _pos.y - DistAxeY)
	{
		return true;
	}

	return false;
}

void Boar::UpdateTargetFirstPhase(std::vector<Player*>& _player, float _dt, Audio& _audio)
{
	if (!this->secondPhase)
	{
		UpdateTargetPlayer(_player);
		Move(_player[this->targetPlayer]->GetPos(), _dt, _audio);
	}
}

void Boar::UpdateTargetSecondPhase(std::vector<Player*>& _player, Wave _wave, float _dt)
{
	if (this->secondPhase)
	{
		sf::Vector2f goalPos = { (float)(WORKING_WIDTH + (int)(_wave)*WORKING_WIDTH) - GetDimSprite().x, _player[this->targetPlayer]->GetPos().y };//just to only get y axis
		float vecY = goalPos.y - this->mob->pos.y;

		if (std::abs(vecY) <= DistMinMove)
		{
			this->state = BoarState::Idle;
		}

		MoveSecondPhase(goalPos, _dt);
	}
}

void Boar::ChooseAPlayer(std::vector<Player*>& _player)
{
	this->targetPlayer = rand() % _player.size();
	PlayerTargetIsDead(_player);
}

void Boar::Move(sf::Vector2f _posPlayer, float _dt, Audio& _audio)
{
	if (timerSound > 0.5f)
	{
		//random pitch between -0.1f and 0.1f
		float randomPitch = (rand() / (float)RAND_MAX) / 5 - 0.1f;
		_audio.SetPitch("BOAR_STEP", 1 + randomPitch);
		_audio.PlaySoundRepeated("BOAR_STEP", 100.f);
		timerSound = 0.f;
	}

	sf::Vector2f goalPos = sf::Vector2f{ _posPlayer.x + GoalPos * this->mob->scaleSprite.x,_posPlayer.y };
	sf::Vector2f normVec = Utils::NormalizeVector(sf::Vector2f{ goalPos - this->mob->pos });

	this->mob->pos.x += normVec.x * this->mob->speed.x * _dt;
	this->mob->pos.y += normVec.y * this->mob->speed.y * _dt;
}

void Boar::UpdateNormalAttack(sf::Vector2f _player, Audio& _audio)
{
	if (this->sprite.anim->currentFrame >= 3 && this->sprite.anim->currentFrame <= 8) //frame when attack
	{
		_audio.PlaySoundNotRepeated("BOAR_SCREAM_ATTACK1", 50.f);
		float vec = (this->mob->Trigo(_player) * (this->sprite.spriteAnim[(int)this->state].sprite.getTexture()->getSize().x / this->sprite.spriteAnim[(int)this->state].nbAnim.x / 2 - 100.f));
		this->mob->hitBoxAttack->setPosition(sf::Vector2f{ this->mob->pos.x - vec,this->mob->pos.y - 40.f });
	}
	else if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame) //end frame of animation
	{
		this->state = BoarState::Target;
		this->mob->hitBoxAttack->setPosition(sf::Vector2f{ -10'000.f,-10'000.f });
	}
}

void Boar::UpdateZenitsuAttack(sf::Vector2f _player, float _dt, Audio& _audio)
{
	hasZenistsuSoundStarted = true;
	if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame) //end frame of animation
	{
		(bool)(this->timerSlashZenitsu == TimerAfterZenitsu) ? this->doZenitsu = true : this->doZenitsu = false;

		this->timerSlashZenitsu -= _dt;
		if (this->timerSlashZenitsu <= 0)
		{
			this->timerKnockDown = TimerKnockdown;
			this->timerSlashZenitsu = TimerAfterZenitsu;
			this->state = BoarState::KnockDown;
			this->hasZenistsuSoundStarted = false;
		}
	}
	else if (this->sprite.anim->currentFrame >= 13 && this->sprite.anim->currentFrame <= 14) //frame when attack
	{
		this->mob->pos = sf::Vector2f{ _player.x + 200.f,_player.y };
	}
	else if (zenitsuTimerSound > 3.f)
	{
		_audio.PlaySoundNotRepeated("BOAR_ZENITSU", 100.f);
		_audio.PlaySoundRepeated("BOAR_ZENITSU_SCREAM", 100.f);

		zenitsuTimerSound = 0.f;
	}
}

void Boar::UpdateTimerKnockDown(float _dt, Audio& _audio)
{
	if (breathlessTimerSound > 3)
	{
		_audio.PlaySoundNotRepeated("BOAR_BREATHLESS", 100.f);
	}

	if (!this->knockDownAnim)
	{
		this->sprite.anim->currentFrame = 13;
		this->knockDownAnim = true;
	}

	this->timerKnockDown -= _dt;
	if (this->timerKnockDown <= 0 && this->sprite.anim->currentFrame == this->sprite.anim->maxFrame)
	{
		breathlessTimerSound = 0;
		this->state = BoarState::Target;
		this->knockDownAnim = false;
	}
}

void Boar::UpdateSlashAttackVerti(sf::Vector2f _player)
{
	if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame) //end frame of animation
	{
		this->state = BoarState::Target;
	}
	else if (this->sprite.anim->currentFrame == 14) //frame when attack slash verti
	{
		this->sprite.slashHitBox->setPosition(this->mob->pos);
		this->sprite.dir = this->mob->scaleSprite.x;
		this->slashVertiAlive = true;
	}
}

void Boar::UpdateSlashAttackHorizon(sf::Vector2f _player)
{
	if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame) //end frame of animation
	{
		this->state = BoarState::Target;
	}
	else if (this->sprite.anim->currentFrame == 16) //frame when attack slash horizon
	{
		this->sprite.slashHitBox->setPosition(sf::Vector2f{ this->mob->pos });
		this->sprite.dir = this->mob->scaleSprite.x;
		this->slashHorizonAlive = true;
	}
}

void Boar::UpdateSlash(Wave _wave, float _dt)
{
	sf::Vector2f speed{ -800.f,0.f };
	if (this->slashHorizonAlive)
	{
		speed.x *= this->sprite.dir;
		this->sprite.slashHitBox->setPosition(this->sprite.slashHitBox->getPosition() + speed * _dt);

		if (this->sprite.slashHitBox->getPosition().x > 1920 + (int)(_wave) * 1920 || this->sprite.slashHitBox->getPosition().x < 0 + (int)(_wave) * 1920)
		{
			this->slashHorizonAlive = false;
		}

		bool temp = true;
		this->sprite.animSlash->UpdateLeftToRight(this->sprite.spriteSlash[0].sprite, _dt, this->sprite.spriteSlash[0].speed, 0, temp, this->sprite.spriteSlash[0].loop);
	}

	if (this->slashVertiAlive)
	{
		speed.x *= this->sprite.dir;
		this->sprite.slashHitBox->setPosition(this->sprite.slashHitBox->getPosition() + speed * _dt);

		if (this->sprite.slashHitBox->getPosition().x > 1920 + (int)(_wave) * 1920 || this->sprite.slashHitBox->getPosition().x < 0 + (int)(_wave) * 1920)
		{
			this->slashVertiAlive = false;
		}

		bool temp = true;
		this->sprite.animSlash->UpdateLeftToRight(this->sprite.spriteSlash[1].sprite, _dt, this->sprite.spriteSlash[1].speed, 0, temp, this->sprite.spriteSlash[1].loop);
	}
}

void Boar::UpdateIdle(std::vector<Player*>& _players)
{
	if (!this->secondPhase)
	{
		ChooseAPlayer(_players);
		this->mob->maxLife = this->mob->life;
		this->state = BoarState::Target;
	}
}

void Boar::ChangePhase()
{
	if (this->mob->life <= this->mob->maxLife / 2 && !this->secondPhase)
	{
		if (!this->leak)
		{
			this->secondPhase = true;
			this->mob->speed = { 500,150 };
			this->state = BoarState::Back;
		}
		else
		{
			this->state = BoarState::Target;
			this->isLeaking = true;
			this->mob->speed = { 800.f, 300.f };
			this->mob->scaleSprite = { -1,1 };
		}
	}
}

void Boar::InitNewAnim()
{
	if (this->sprite.spriteAnim->nbAnim.x != 0)
	{
		sf::IntRect rect = sf::IntRect(0, 0, (int)(this->sprite.spriteAnim[(int)this->state].animDim.x / this->sprite.spriteAnim[(int)this->state].nbAnim.x), (int)(this->sprite.spriteAnim[(int)this->state].nbAnim.y));
		this->sprite.anim->CreateLeftToRight(this->sprite.spriteAnim[(int)this->state].sprite, (const unsigned char)(this->sprite.spriteAnim[(int)this->state].nbAnim.x), (const unsigned char)(this->sprite.spriteAnim[(int)this->state].nbAnim.y), this->sprite.spriteAnim[(int)this->state].animDim);
		this->sprite.spriteAnim[(int)this->state].sprite.setTextureRect(rect);
		this->sprite.anim->currentFrame = 0;

		this->mob->hitbox->setSize(sf::Vector2f{ this->sprite.spriteAnim[(int)this->state].animDim.x / this->sprite.spriteAnim[(int)this->state].nbAnim.x , this->sprite.spriteAnim[(int)this->state].animDim.y / this->sprite.spriteAnim[(int)this->state].nbAnim.y });
		this->mob->hitbox->setOrigin(this->mob->hitbox->getSize().x / 2, this->mob->hitbox->getSize().y);
	}
}

void Boar::UpdateAnim(float _dt)
{
	if (this->state != this->lastState)
	{
		InitNewAnim();
	}

	if (this->mob->status != this->mob->lastStatus)
	{
		this->mob->InitNewAnimStatus();
	}

	bool temp = true;
	this->sprite.anim->UpdateLeftToRight(this->sprite.spriteAnim[(int)this->state].sprite, _dt, this->sprite.spriteAnim[(int)this->state].speed, this->dirAnim, temp, this->sprite.spriteAnim[(int)this->state].loop);

	if (this->mob->status != Status::None)
	{
		bool temp = true;
		this->mob->statusSprite.anim->UpdateLeftToRight(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite, _dt, this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].speed, 0, temp, this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].loop);
	}
}

void Boar::UpdateDeath(float _dt)
{
	if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame && this->dirAnim)
	{
		this->mob->finishAnim = true;
	}
	else if (this->sprite.anim->currentFrame == 19)
	{
		this->dirAnim = 1;
		this->sprite.anim->currentFrame = 0;
	}
	else if (this->dirAnim && this->sprite.anim->currentFrame != this->sprite.anim->maxFrame)
	{
		this->mob->decalage.x -= 0.75f;
	}
}

void Boar::InitNewSlash(SpriteAnim& _sprite, Animation* _anim, sf::RectangleShape* _slash)
{
	sf::IntRect rect = sf::IntRect(0, 0, (int)(_sprite.animDim.x / _sprite.nbAnim.x), (int)_sprite.nbAnim.y);
	_anim->CreateLeftToRight(_sprite.sprite, (const unsigned char)_sprite.nbAnim.x, (const unsigned char)_sprite.nbAnim.y, _sprite.animDim);
	_sprite.sprite.setTextureRect(rect);

	_slash->setSize(sf::Vector2f{ (_sprite.animDim.x / _sprite.nbAnim.x) , (_sprite.animDim.y / _sprite.nbAnim.y) });
	_slash->setOrigin(_slash->getSize().x / 2, _slash->getSize().y);
}

void Boar::UpdateBack(Wave _wave, float _dt, Audio& _audio)
{
	sf::Vector2f goalPos = sf::Vector2f{ (float)(WORKING_WIDTH + (int)(_wave)*WORKING_WIDTH) - GetDimSprite().x, this->mob->pos.y };
	sf::Vector2f vec = sf::Vector2f{ goalPos - this->mob->pos };

	MoveBack(goalPos, _dt);

	if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame && vec.x <= DistMinMove)
	{
		this->state = BoarState::Target;
	}
}

void Boar::UpdateShield(std::vector<Player*>& _player, float _dt, Audio& _audio)
{
	this->timerShield -= _dt;
	if (this->timerShield <= 0)
	{
		this->timerShield = TimerShield;
		this->state = BoarState::Target;
	}
}

bool Boar::GetSlashVertAlive()
{
	return this->slashVertiAlive;
}

bool Boar::GetSlashHorizonAlive()
{
	return this->slashHorizonAlive;
}

bool Boar::GetDoZenitsu()
{
	return this->doZenitsu;
}

sf::RectangleShape* Boar::GetSlash()
{
	return this->sprite.slashHitBox;
}

sf::Vector2f Boar::GetSlashInfo()
{
	return { (float)(this->sprite.animSlash->currentFrame) , 0.f };
}

sf::Vector2f Boar::GetDimSprite()
{
	if (this->state == BoarState::Death)
	{
		return sf::Vector2f(this->sprite.spriteAnim[(int)this->state].sprite.getTexture()->getSize().x / this->sprite.spriteAnim[(int)this->state].nbAnim.x / 3, this->sprite.spriteAnim[(int)this->state].sprite.getTexture()->getSize().y / this->sprite.spriteAnim[(int)this->state].nbAnim.y / 3);
	}
	return sf::Vector2f(this->sprite.spriteAnim[(int)this->state].sprite.getTexture()->getSize().x / this->sprite.spriteAnim[(int)this->state].nbAnim.x, this->sprite.spriteAnim[(int)this->state].sprite.getTexture()->getSize().y / this->sprite.spriteAnim[(int)this->state].nbAnim.y);
}

int Boar::GetTarget()
{
	return targetPlayer;
}

void Boar::SetTarget(int _targetID)
{
	this->targetPlayer = _targetID;
}

void Boar::SetLeak(bool _leak)
{
	this->leak = _leak;
}

sf::Vector2f Boar::GetPosSlashH()
{
	return this->sprite.spriteSlash[0].sprite.getPosition();
}

void Boar::AddNbAttack()
{
	this->nbAttack++;
}

bool Boar::OnLeft()
{
	return (bool)(this->mob->scaleSprite.x == 1);
}

BoarState Boar::GetState()
{
	return this->state;
}