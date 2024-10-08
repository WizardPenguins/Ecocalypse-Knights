#include "stdafx.h"
#include "Witch.hpp"
#include "Player.hpp"

Witch::Witch(Mob* _mob, WitchSprite* _sprite, Wave _wave)
{
	//variable mob
	this->mob = _mob;
	this->mob->pos = { (float)(1400.f + _wave * 1980.f),800.f + LEVEL3_GAP };
	this->mob->speed = { 100.f,100.f };
	this->mob->nbPhase = 3;
	this->mob->inAir = true;
	this->mob->posInAir = sf::Vector2f{ this->mob->pos.x,this->mob->pos.y - 20.f };

	//anim & sprite
	this->sprite = *_sprite;
	this->sprite.anim = new Animation;
	this->sprite.animFx = new Animation;
	this->sprite.animAttack = new Animation;
	this->sprite.animInvoc = new Animation;
	this->sprite.animScream = new Animation;
	this->sprite.rectFx = new sf::RectangleShape;
	this->hitBoxBramble = new sf::RectangleShape;
	InitNewAnim();

	//variable Witch
	this->state = WitchState::Idle;
	this->timerFloating = 0.f;
	this->floating = -0.25f;
	this->nbInvoc = 0;
	this->line = 0;
	this->playerChoose = 0;

	//timer Attack
	this->timerScream = TimerScream;
	this->timerPlante = TimerPlante;
	this->timerBramble = TimerBramble;
	this->timerInvoc = TimerInvoc;
	this->timerBeforeAttack = TimerBeforeAttack1;

	//bool to know what attack is actual play
	this->initFx = false;
	this->hasInvocCircle = false;
	this->hasBramble = false;
	this->hasSpawnCrystal = false;
	this->playerBecomeFuzzy = false;
	this->hasScream = false;
	this->hasSpawnPlant = false;
}

void Witch::Update(float _dt, std::vector<Mob*>& _mob, Wave _wave, Audio& _audio, std::vector<Player*>& _player)
{
	this->timerBeforeAttack -= _dt;
	if (this->timerBeforeAttack <= 0)
	{
		switch (this->state)
		{
		case WitchState::Idle:
			UpdateChoosePlayer(_player.size(), _player[this->playerChoose]);
			UpdateIdleMove(_dt);
			UpdateTimerAttack(_dt);
			UpdateChangeState();
			ChangePhase();
			break;

		case WitchState::Invoc:
			UpdateInvoc(_dt, _mob, _wave, _audio);
			break;

		case WitchState::Bramble:
			UpdateBrambleSpawn(_dt, _wave, _player[this->playerChoose]->GetPos().y, _audio);
			this->playerChoose = (this->playerChoose + 1) % _player.size();
			break;

		case WitchState::Invincible:
			AddCrystal(_audio);
			break;

		case WitchState::Plante:
			AddPlanteOnGround(_wave, _audio);
			break;

		case WitchState::Scream:
			UpdateScream(_dt, _audio);
			break;

		case WitchState::Death:
			UpdateDeath(_audio);
			break;

		case WitchState::Normal:
			UpdateIdleMove(_dt);
			break;

		default:
			break;
		}
	}
	UpdateInvicible(_dt, _audio);
	UpdatePlante(_dt, _audio);
	UpdateCrystalHit(_dt, _audio);
	UpdateAnim(_dt);
	Death(_audio);
}

void Witch::Display(sf::RenderWindow& _window)
{
	DisplayFx(_window);
	DisplayEntity(_window);
	DrawShield(_window);
	DisplayStatus(_window);
}

void Witch::Display(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	DisplayFx(_red, _blue);
	DisplayEntity(_red, _blue);
	DrawShield(_red, _blue);
	DisplayStatus(_red, _blue);
}

//invoc hazelNut
void Witch::DrawInvocation(sf::RenderWindow& _window)
{
	if (this->hasInvocCircle)
	{
		_window.draw(this->sprite.spriteCircleInvoc.sprite);
	}
}

void Witch::DrawInvocation(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	if (this->hasInvocCircle)
	{
		Utils::BlitSprite(this->sprite.spriteCircleInvoc.sprite, sf::Vector2f{ this->sprite.spriteCircleInvoc.sprite.getPosition().x + (TROAD_GAP * (-1 * this->sprite.spriteCircleInvoc.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->sprite.spriteCircleInvoc.sprite.getPosition().y }, 0, _red);
		Utils::BlitSprite(this->sprite.spriteCircleInvoc.sprite, sf::Vector2f{ this->sprite.spriteCircleInvoc.sprite.getPosition().x - (TROAD_GAP * (-1 * this->sprite.spriteCircleInvoc.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->sprite.spriteCircleInvoc.sprite.getPosition().y }, 0, _blue);
	}
}

void Witch::DisplayEntity(sf::RenderWindow& _window)
{
	this->sprite.spriteAnim[(int)this->state].sprite.setScale(this->mob->scaleSprite);
	(bool)(this->mob->inAir) ? this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->posInAir) : this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos);
	_window.draw(this->sprite.spriteAnim[(int)this->state].sprite);
}

void Witch::DisplayStatus(sf::RenderWindow& _window)
{
	if (this->mob->status != Status::None && this->state != WitchState::Death)
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
		this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->pos.x,this->mob->pos.y - GetDimSprite().y + temp });
		_window.draw(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite);
	}
}

void Witch::DisplayFx(sf::RenderWindow& _window)
{
	//draw fx invoc
	if (this->hasInvocCircle)
	{
		_window.draw(this->sprite.spriteFxCircleInvoc.sprite);
	}

	//draw fx Scream
	if (this->hasScream)
	{
		_window.draw(this->sprite.spriteScream.sprite);
	}
}

void Witch::DisplayFx(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	//draw fx invoc
	if (this->hasInvocCircle)
	{
		Utils::BlitSprite(this->sprite.spriteFxCircleInvoc.sprite, sf::Vector2f{ this->sprite.spriteFxCircleInvoc.sprite.getPosition().x + (TROAD_GAP * (-1 * this->sprite.spriteFxCircleInvoc.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->sprite.spriteFxCircleInvoc.sprite.getPosition().y }, 0, _red);
		Utils::BlitSprite(this->sprite.spriteFxCircleInvoc.sprite, sf::Vector2f{ this->sprite.spriteFxCircleInvoc.sprite.getPosition().x - (TROAD_GAP * (-1 * this->sprite.spriteFxCircleInvoc.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->sprite.spriteFxCircleInvoc.sprite.getPosition().y }, 0, _blue);
	}

	//draw fx Scream
	if (this->hasScream)
	{
		Utils::BlitSprite(this->sprite.spriteScream.sprite, sf::Vector2f{ this->sprite.spriteScream.sprite.getPosition().x + (TROAD_GAP * (-1 * this->sprite.spriteScream.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->sprite.spriteScream.sprite.getPosition().y }, 0, _red);
		Utils::BlitSprite(this->sprite.spriteScream.sprite, sf::Vector2f{ this->sprite.spriteScream.sprite.getPosition().x - (TROAD_GAP * (-1 * this->sprite.spriteScream.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->sprite.spriteScream.sprite.getPosition().y }, 0, _blue);
	}
}

void Witch::DisplayEntity(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	this->sprite.spriteAnim[(int)this->state].sprite.setScale(this->mob->scaleSprite);

	this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos.x + (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f), this->mob->pos.y);
	_red.draw(this->sprite.spriteAnim[(int)this->state].sprite);

	this->sprite.spriteAnim[(int)this->state].sprite.setPosition(this->mob->pos.x - (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f), this->mob->pos.y);
	_blue.draw(this->sprite.spriteAnim[(int)this->state].sprite);
}

void Witch::DisplayStatus(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	if (this->mob->status != Status::None && this->state != WitchState::Death)
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

		this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->pos.x + (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f),this->mob->pos.y - GetDimSprite().y + temp });
		_red.draw(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite);

		this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite.setPosition(sf::Vector2f{ this->mob->pos.x - (TROAD_GAP * (-1 * this->mob->pos.y + WORKING_HEIGHT) / 100.f),this->mob->pos.y - GetDimSprite().y + temp });
		_blue.draw(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite);
	}
}

void Witch::DisplayCrystal(sf::RenderWindow& _window, Crystal* _crystal)
{
	if (_crystal->life > 0)
	{
		_crystal->sprite.sprite.setPosition(_crystal->pos);
		_crystal->hitBox->setPosition(sf::Vector2f{ _crystal->pos.x - 5.f ,_crystal->pos.y - 80.f });
		_window.draw(_crystal->sprite.sprite);
	}

	if (_crystal->hit)
	{
		_window.draw(_crystal->spriteHit.sprite);
	}
}

void Witch::DisplayCrystal(sf::RenderTexture& _red, sf::RenderTexture& _blue, Crystal* _crystal)
{
	if (_crystal->life > 0)
	{
		_crystal->sprite.sprite.setPosition(_crystal->pos);
		_crystal->hitBox->setPosition(sf::Vector2f{ (_crystal->pos.x - 5.f) + (TROAD_GAP * (-1 * (_crystal->pos.y - 80.f) + WORKING_HEIGHT) / 100.f),_crystal->pos.y - 80.f });
		_red.draw(_crystal->sprite.sprite);

		_crystal->hitBox->setPosition(sf::Vector2f{ (_crystal->pos.x - 5.f) - (TROAD_GAP * (-1 * (_crystal->pos.y - 80.f) + WORKING_HEIGHT) / 100.f),_crystal->pos.y - 80.f });
		_blue.draw(_crystal->sprite.sprite);
	}

	if (_crystal->hit)
	{
		Utils::BlitSprite(_crystal->spriteHit.sprite, sf::Vector2f{ _crystal->spriteHit.sprite.getPosition().x + (TROAD_GAP * (-1 * _crystal->spriteHit.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),_crystal->spriteHit.sprite.getPosition().y }, 0, _red);
		Utils::BlitSprite(_crystal->spriteHit.sprite, sf::Vector2f{ _crystal->spriteHit.sprite.getPosition().x - (TROAD_GAP * (-1 * _crystal->spriteHit.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),_crystal->spriteHit.sprite.getPosition().y }, 0, _blue);
	}
}

void Witch::DisplayPlante(sf::RenderWindow& _window, Plante* _plante)
{
	if (_plante->canDelete)
	{
		_window.draw(_plante->spriteDeath.sprite);
	}
	else
	{
		_window.draw(_plante->sprite.sprite);
	}
}

void Witch::DisplayPlante(sf::RenderTexture& _red, sf::RenderTexture& _blue, Plante* _plante)
{
	if (_plante->canDelete)
	{
		Utils::BlitSprite(_plante->spriteDeath.sprite, sf::Vector2f{ _plante->spriteDeath.sprite.getPosition().x + (TROAD_GAP * (-1 * _plante->spriteDeath.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),_plante->spriteDeath.sprite.getPosition().y }, 0, _red);
		Utils::BlitSprite(_plante->spriteDeath.sprite, sf::Vector2f{ _plante->spriteDeath.sprite.getPosition().x + (TROAD_GAP * (-1 * _plante->spriteDeath.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),_plante->spriteDeath.sprite.getPosition().y }, 0, _blue);

	}
	else
	{
		Utils::BlitSprite(_plante->sprite.sprite, sf::Vector2f{ _plante->sprite.sprite.getPosition().x + (TROAD_GAP * (-1 * _plante->sprite.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),_plante->sprite.sprite.getPosition().y }, 0, _red);
		Utils::BlitSprite(_plante->sprite.sprite, sf::Vector2f{ _plante->sprite.sprite.getPosition().x + (TROAD_GAP * (-1 * _plante->sprite.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),_plante->sprite.sprite.getPosition().y }, 0, _blue);
	}
}

//draw shield of boss
void Witch::DrawShield(sf::RenderWindow& _window)
{
	if (this->hasSpawnCrystal)
	{
		this->sprite.spriteShield.sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x ,this->mob->posInAir.y + 230.f });
		_window.draw(this->sprite.spriteShield.sprite);
	}
}

void Witch::DrawShield(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	if (this->hasSpawnCrystal)
	{
		this->sprite.spriteShield.sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x ,this->mob->posInAir.y + 230.f });
		Utils::BlitSprite(this->sprite.spriteShield.sprite, sf::Vector2f{ this->sprite.spriteShield.sprite.getPosition().x + (TROAD_GAP * (-1 * this->sprite.spriteShield.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->sprite.spriteShield.sprite.getPosition().y }, 0, _red);
		Utils::BlitSprite(this->sprite.spriteShield.sprite, sf::Vector2f{ this->sprite.spriteShield.sprite.getPosition().x - (TROAD_GAP * (-1 * this->sprite.spriteShield.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->sprite.spriteShield.sprite.getPosition().y }, 0, _blue);
	}
}

void Witch::DisplayBramble(sf::RenderWindow& _window)
{
	//invoc bramble
	if (this->hasBramble)
	{
		for (int i = 0; i < this->vecBramble.size(); i++)
		{
			_window.draw(this->vecBramble[i]->sprite.sprite);
		}

		//hitbox bramble
		this->hitBoxBramble->setPosition(sf::Vector2f{ this->vecBramble[0]->pos.x + 100.f,this->vecBramble[0]->pos.y });
	}
}

void Witch::DisplayBramble(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	//invoc bramble
	if (this->hasBramble)
	{
		for (int i = 0; i < this->vecBramble.size(); i++)
		{
			Utils::BlitSprite(this->vecBramble[i]->sprite.sprite, sf::Vector2f{ this->vecBramble[i]->sprite.sprite.getPosition().x + (TROAD_GAP * (-1 * this->vecBramble[i]->sprite.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->vecBramble[i]->sprite.sprite.getPosition().y }, 0, _red);
			Utils::BlitSprite(this->vecBramble[i]->sprite.sprite, sf::Vector2f{ this->vecBramble[i]->sprite.sprite.getPosition().x - (TROAD_GAP * (-1 * this->vecBramble[i]->sprite.sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->vecBramble[i]->sprite.sprite.getPosition().y }, 0, _blue);
		}

		//hitbox bramble
		this->hitBoxBramble->setPosition(sf::Vector2f{ this->vecBramble[0]->pos.x + 100.f,this->vecBramble[0]->pos.y });
	}
}

void Witch::ChangePhase()
{
	if (this->mob->life <= (this->mob->maxLife / 3) * 2 && !this->secondPhase)
	{
		this->secondPhase = true;
		this->state = WitchState::Invincible;
	}

	if (this->mob->life <= (this->mob->maxLife / 3) && !this->thirdPhase)
	{
		this->thirdPhase = true;
		this->state = WitchState::Invincible;
	}
}

void Witch::UpdateIdleMove(float _dt)
{
	this->timerFloating += _dt;
	if (this->timerFloating > 1.5f)
	{
		this->timerFloating = 0.f;
		this->floating *= -1;
		this->mob->finishAnim = true;
	}

	this->mob->posInAir.y += this->floating;
}

void Witch::UpdateDeath(Audio& _audio)
{
	//if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame)
	{
		this->state = WitchState::Normal;
	}
}

void Witch::UpdateChangeState()
{
	if (this->timerBramble <= 0)
	{
		this->state = WitchState::Bramble;
		this->timerBramble = TimerBramble;
	}

	if (this->secondPhase)
	{
		if (this->timerInvoc <= 0)
		{
			this->state = WitchState::Invoc;
			this->timerInvoc = TimerInvoc;
		}

		if (this->timerPlante <= 0)
		{
			this->state = WitchState::Plante;
			this->timerPlante = TimerPlante;
		}
	}

	if (this->thirdPhase)
	{
		if (this->timerScream <= 0)
		{
			this->state = WitchState::Scream;
			this->timerScream = TimerScream;
		}
	}
}

void Witch::UpdateInvoc(float _dt, std::vector<Mob*>& _mob, Wave _wave, Audio& _audio)
{
	if (!this->initFx)
	{
		_audio.PlaySoundNotRepeated("WITCH_SUMMON_SCREAM_1", 100.f);
		_audio.PlaySoundNotRepeated("WITCH_SUMMON_SOUND_1", 100.f);
		InitAnimAttack(this->sprite.spriteCircleInvoc, *this->sprite.rectFx);
		InitFxCircleInvoc();
		this->hasInvocCircle = true;
		this->initFx = true;
		this->sprite.spriteCircleInvoc.sprite.setPosition(sf::Vector2f{ this->mob->pos.x - 400.f,this->mob->pos.y });
		this->sprite.spriteFxCircleInvoc.sprite.setPosition(this->sprite.spriteCircleInvoc.sprite.getPosition());
	}
	SpawnHazelNut(_mob, _wave, _audio);
	UpdateAnimFx(_dt, this->sprite.spriteCircleInvoc, this->sprite.animAttack, 0);
	UpdateAnimFx(_dt, this->sprite.spriteFxCircleInvoc, this->sprite.animInvoc, 0);
}

void Witch::UpdateChoosePlayer(int _nbPlayer, Player* _player)
{
	if (_player->GetLife() <= 0) // if the player choose is dead
	{
		this->playerChoose++;
		this->playerChoose = this->playerChoose % _nbPlayer;
	}

	if (rand() % 10 == 0) // rand to change player
	{
		this->playerChoose++;
		this->playerChoose = this->playerChoose % _nbPlayer;
	}
}

void Witch::UpdateTimerAttack(float _dt)
{
	if (this->timerBramble > 0)
	{
		this->timerBramble -= _dt;
	}

	if (this->timerInvoc > 0)
	{
		this->timerInvoc -= _dt;
	}

	if (this->timerPlante > 0)
	{
		this->timerPlante -= _dt;
	}

	if (this->timerScream > 0)
	{
		this->timerScream -= _dt;
	}
}

void Witch::InitAnimAttack(SpriteAnim& _sprite, sf::RectangleShape& _shape)
{
	sf::IntRect rect = sf::IntRect(0, 0, (int)(_sprite.animDim.x / _sprite.nbAnim.x), (int)(_sprite.nbAnim.y));
	this->sprite.animAttack->CreateLeftToRight(_sprite.sprite, (const unsigned char)(_sprite.nbAnim.x), (const unsigned char)(_sprite.nbAnim.y), _sprite.animDim);
	_sprite.sprite.setTextureRect(rect);
	this->sprite.animAttack->currentFrame = 0;

	_shape.setSize(sf::Vector2f{ _sprite.animDim.x / _sprite.nbAnim.x , _sprite.animDim.y / _sprite.nbAnim.y });
	_shape.setOrigin(_shape.getSize().x / 2, _shape.getSize().y);
}

void Witch::UpdateAnimFx(float _dt, SpriteAnim& _sprite, Animation* _anim, int _line)
{
	bool temp = true;
	_anim->UpdateLeftToRight(_sprite.sprite, _dt, _sprite.speed, _line, temp, _sprite.loop);
}

void Witch::SpawnHazelNut(std::vector<Mob*>& _mob, Wave _wave, Audio& _audio)
{
	if (this->sprite.animAttack->currentFrame == 10 && this->nbInvoc < 3)//to spawn only 3 hazelNut
	{
		this->nbInvoc++;
		Mob* temp = new Mob{ _audio,this->sprite.hazelnutCorrupted,&this->mob->statusSprite,sf::Vector2f{this->sprite.spriteCircleInvoc.sprite.getPosition().x,this->sprite.spriteCircleInvoc.sprite.getPosition().y - 50.f} };
		_mob.push_back(temp);
	}
	else if (this->sprite.animAttack->currentFrame == this->sprite.animAttack->maxFrame)
	{
		this->hasInvocCircle = false;
		this->initFx = false;
		this->state = WitchState::Idle;
		this->nbInvoc = 0;
	}
}

void Witch::DestroyHitBoxAttack()
{
	this->mob->hitBoxAttack->setPosition(10'000, 10'000);
}

void Witch::UpdateAnim(float _dt)
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
	this->sprite.anim->UpdateLeftToRight(this->sprite.spriteAnim[(int)this->state].sprite, _dt, this->sprite.spriteAnim[(int)this->state].speed, 0, temp, this->sprite.spriteAnim[(int)this->state].loop);

	if (this->mob->status != Status::None)
	{
		bool temp = true;
		this->mob->statusSprite.anim->UpdateLeftToRight(this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].sprite, _dt, this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].speed, 0, temp, this->mob->statusSprite.spriteAnim[(int)(this->mob->status) - 1].loop);
	}
}

void Witch::InitShield()
{
	sf::IntRect rect = sf::IntRect{ 0, 0, (int)(this->sprite.spriteShield.animDim.x / this->sprite.spriteShield.nbAnim.x),(int)(this->sprite.spriteShield.nbAnim.y / this->sprite.spriteShield.nbAnim.y) };
	this->sprite.animFx->CreateLeftToRight(this->sprite.spriteShield.sprite, this->sprite.spriteShield.nbAnim.x, this->sprite.spriteShield.nbAnim.y, this->sprite.spriteShield.animDim);
	this->sprite.spriteShield.sprite.setTextureRect(rect);
	this->sprite.animFx->currentFrame = 0;
}

void Witch::UpdateScream(float _dt, Audio& _audio)
{
	//init
	if (!this->initFx)
	{
		InitScreamFx();
		this->sprite.spriteScream.sprite.setPosition(sf::Vector2f{ this->mob->posInAir.x - 20.f,this->mob->posInAir.y - 50.f }); // fx scream
		this->hasScream = true;
		this->initFx = true;
		_audio.PlaySoundNotRepeated("WITCH_ULTIMATE_SCREAM_1", 100.f);
	}

	//to launch status on player
	if (this->sprite.anim->currentFrame == 7)
	{
		this->playerBecomeFuzzy = true;
	}

	//reload and change state 
	if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame)
	{
		this->state = WitchState::Idle;
		this->initFx = false;
		this->hasScream = false;
		this->sprite.line = 0;
	}

	//to change line of the spritesheet fx scream
	if (this->sprite.animScream->currentFrame == 7)
	{
		this->sprite.line++;
	}

	//updtae fx scream
	UpdateAnimFx(_dt, this->sprite.spriteScream, this->sprite.animScream, this->sprite.line);
}

void Witch::InitScreamFx()
{
	sf::IntRect rect = sf::IntRect{ 0, 0, (int)(this->sprite.spriteScream.animDim.x / this->sprite.spriteScream.nbAnim.x),(int)(this->sprite.spriteScream.nbAnim.y) };
	this->sprite.animScream->CreateLeftToRight(this->sprite.spriteScream.sprite, this->sprite.spriteScream.nbAnim.x, this->sprite.spriteScream.nbAnim.y, this->sprite.spriteScream.animDim);
	this->sprite.spriteScream.sprite.setTextureRect(rect);
	this->sprite.animScream->currentFrame = 0;
}

void Witch::UpdateAnimShield(float _dt)
{
	if (this->line == 0 && this->sprite.animFx->currentFrame == 9)
	{
		this->line++;
		this->sprite.animFx->currentFrame = 0;
	}
	else if (this->line == 2 && this->sprite.animFx->currentFrame >= 6 && this->hasSpawnCrystal)
	{
		this->hasSpawnCrystal = false;
		this->line = 0;
	}
	UpdateAnimFx(_dt, this->sprite.spriteShield, this->sprite.animFx, this->line);
}

void Witch::UpdateAnimCrystal(float _dt)
{
	for (int i = 0; i < NbCrystal; i++)
	{
		UpdateAnimFx(_dt, this->crystal[i].sprite, this->crystal[i].anim, 0);
	}
}

void Witch::InitFxCircleInvoc()
{
	sf::IntRect rect = sf::IntRect(0, 0, (int)(this->sprite.spriteFxCircleInvoc.animDim.x / this->sprite.spriteFxCircleInvoc.nbAnim.x), (int)(this->sprite.spriteFxCircleInvoc.nbAnim.y));
	this->sprite.animInvoc->CreateLeftToRight(this->sprite.spriteFxCircleInvoc.sprite, (const unsigned char)(this->sprite.spriteFxCircleInvoc.nbAnim.x), (const unsigned char)(this->sprite.spriteFxCircleInvoc.nbAnim.y), this->sprite.spriteFxCircleInvoc.animDim);
	this->sprite.spriteFxCircleInvoc.sprite.setTextureRect(rect);
	this->sprite.animInvoc->currentFrame = 0;
}

void Witch::ChangePhaseCrystal()
{
	for (int i = 0; i < NbCrystal; i++)
	{
		if (this->crystal[i].life <= (int)(LifeCrystal / 3))
		{
			this->crystal[i].state = CrystalState::End;
			this->crystal[i].sprite = this->sprite.spriteCrystalInvoc[(int)this->crystal[i].state];
		}
		else if (this->crystal[i].life <= (int)(LifeCrystal / 3 * 2))
		{
			this->crystal[i].state = CrystalState::Middle;
			this->crystal[i].sprite = this->sprite.spriteCrystalInvoc[(int)this->crystal[i].state];
		}
	}
}

void Witch::CrystalHit(float _dt)
{
	for (int i = 0; i < NbCrystal; i++)
	{
		//end of animation
		if (this->crystal[i].animHitCrystal->currentFrame == this->crystal[i].animHitCrystal->maxFrame)
		{
			this->crystal[i].hit = false;
			this->crystal[i].animHitCrystal->currentFrame = 0;
		}

		//update animation
		if (this->crystal[i].hit)
		{
			UpdateAnimFx(_dt, this->crystal[i].spriteHit, this->crystal[i].animHitCrystal, 0);
		}
	}
}

void Witch::UpdatePosCrystal(float _dt)
{
	for (int i = 0; i < NbCrystal; i++)
	{
		this->crystal[i].pos.y -= this->crystal[i].floating;
		this->crystal[i].timerFloating += _dt;
		if (this->crystal[i].timerFloating > 2.f)
		{
			this->crystal[i].floating *= -1;
			this->crystal[i].timerFloating = 0;
		}
	}
}

void Witch::UpdateHealthCrystal(Audio& _audio)
{
	//to know how many crystal remaining
	int nbCrystalAlive = NbCrystal;
	for (int i = 0; i < NbCrystal; i++)
	{
		if (this->crystal[i].life <= 0)
		{
			nbCrystalAlive--;
			if (!this->crystal[i].soundDeath)
			{
				this->crystal[i].soundDeath = true;
				_audio.PlaySoundNotRepeated("WITCH_CRISTAL_DESTROY_SOUND_1");
			}
		}
	}

	//to launch anim explosion shield
	if (!nbCrystalAlive && this->line != 2)
	{
		this->line++;
		this->sprite.animFx->currentFrame = 0;
	}
}

void Witch::CreateNewPlant(Wave _wave)
{
	//create plante
	Plante* temp = new Plante;
	temp->anim = new Animation;
	temp->pos = sf::Vector2f{ (float)(((rand() % 1400) + 10026)),(float)((rand() % 450) + 4000.f) };
	temp->sprite = this->sprite.spritePlante;
	temp->spriteDeath = this->sprite.spritePlanteDeath;
	temp->sprite.sprite.setPosition(temp->pos);
	temp->timerOnGround = 0.f;
	temp->line = 0;

	temp->goOut = false;
	temp->isReady = false;
	temp->playerEat = false;
	temp->spitOut = false;
	temp->toLong = false;
	temp->canDelete = false;
	temp->eatSomeone = false;

	//init hitBox plante
	temp->hitBox = new sf::RectangleShape;
	temp->hitBox->setSize(sf::Vector2f{ 80.f,100.f });
	temp->hitBox->setOrigin(sf::Vector2f{ temp->hitBox->getSize().x / 2,temp->hitBox->getSize().y });
	temp->hitBox->setPosition(sf::Vector2f{ temp->pos.x + 5.f ,temp->pos.y - 50.f });

	//init anim plante
	sf::IntRect rect = sf::IntRect(0, 0, (int)(temp->sprite.animDim.x / temp->sprite.nbAnim.x), (int)(temp->sprite.nbAnim.y));
	temp->anim->CreateLeftToRight(temp->sprite.sprite, (const unsigned char)(temp->sprite.nbAnim.x), (const unsigned char)(temp->sprite.nbAnim.y), temp->sprite.animDim);
	temp->sprite.sprite.setTextureRect(rect);
	temp->anim->currentFrame = 0;

	//add it to vector
	this->vecPlante.push_back(temp);
}

void Witch::InitAnimCrystal(SpriteAnim& _sprite, Animation* _anim)
{
	sf::IntRect rect = sf::IntRect{ 0, 0, (int)(_sprite.animDim.x / _sprite.nbAnim.x),(int)(_sprite.nbAnim.y) };
	_anim->CreateLeftToRight(_sprite.sprite, _sprite.nbAnim.x, _sprite.nbAnim.y, _sprite.animDim);
	_sprite.sprite.setTextureRect(rect);
	_anim->currentFrame = 0;
}

void Witch::UpdateHaveSomeone(float _dt, Plante* _plante, Audio& _audio)
{
	if (_plante->timerOnGround <= 3.f) // when the plant is eating the player
	{
		_plante->timerOnGround += _dt;

		//if the plant eat a player loop the anim
		if (_plante->anim->currentFrame >= 13)
		{
			_plante->anim->currentFrame = 10;
		}
		else
		{
			UpdateAnimPlante(_plante, _dt);
		}
	}
	else // if the player is gonna spit out wait the good frame
	{
		UpdateAnimPlante(_plante, _dt);

		if (_plante->anim->currentFrame == 7 && _plante->line)
		{
			_plante->spitOut = true;
		}
		//(bool)(rand() % 2 == 1) ? _audio.PlaySoundNotRepeated("WITCH_PLANT_SPIT_SOUND_1") : _audio.PlaySoundNotRepeated("WITCH_PLANT_BURP_SOUND_1");
		_audio.PlaySoundNotRepeated("WITCH_PLANT_BURP_SOUND_1");
	}
}

void Witch::UpdateSpawn(float _dt, Plante* _plante)
{
	if (_plante->anim->currentFrame >= 3)
	{
		_plante->isReady = true;
	}
	else //if the plant wasn't on the frame when wait to eat a player
	{
		UpdateAnimPlante(_plante, _dt);
	}
}

void Witch::UpdateWaitPlante(float _dt, Plante* _plante)
{
	//update anim if too long on ground
	_plante->timerOnGround += _dt;

	if (_plante->timerOnGround >= 10.f)
	{
		_plante->toLong = true;
	}
}

void Witch::InitCystal()
{
	for (int i = 0; i < NbCrystal; i++)
	{
		//init variable of crystal
		this->crystal[i].pos = { this->mob->pos.x - 400.f,this->mob->pos.y - 200 };
		if (i)
		{
			this->crystal[i].pos = { this->mob->pos.x - 400.f,this->mob->pos.y + 100 };
		}
		this->crystal[i].posOnGround = sf::Vector2f{ this->crystal[i].pos.x, this->crystal[i].pos.y - 30.f }; //only for the shadow
		this->crystal[i].anim = new Animation;
		this->crystal[i].animHitCrystal = new Animation;
		this->crystal[i].life = LifeCrystal;
		this->crystal[i].state = CrystalState::Full;
		this->crystal[i].timerFloating = 0.f;
		this->crystal[i].sprite = this->sprite.spriteCrystalInvoc[(int)this->crystal[i].state];
		this->crystal[i].spriteHit = this->sprite.spriteCrystalHit;
		this->crystal[i].hit = false;
		this->crystal[i].soundDeath = false;

		//init anim
		InitAnimCrystal(this->crystal[i].sprite, this->crystal[i].anim);
		InitAnimCrystal(this->crystal[i].spriteHit, this->crystal[i].animHitCrystal);

		//init hitbox
		this->crystal[i].hitBox = new sf::RectangleShape;
		this->crystal[i].hitBox->setSize(sf::Vector2f{ 150.f,200.f });
		this->crystal[i].hitBox->setOrigin(sf::Vector2f{ this->crystal[i].hitBox->getSize().x / 2,this->crystal[i].hitBox->getSize().y });
	}
}

void Witch::AddCrystal(Audio& _audio)
{
	//if witch already invicible
	if (!this->hasSpawnCrystal)
	{
		InitCystal();
		InitShield();
		this->hasSpawnCrystal = true;
		this->state = WitchState::Idle;
	}
	else
	{
		this->state = WitchState::Idle;
	}
}

void Witch::UpdateInvicible(float _dt, Audio& _audio)
{
	if (this->hasSpawnCrystal)
	{
		UpdatePosCrystal(_dt);
		UpdateHealthCrystal(_audio);
		ChangePhaseCrystal();
		UpdateAnimCrystal(_dt);
		CrystalHit(_dt);
		UpdateAnimShield(_dt);
	}
}

void Witch::UpdateAnimBramble(float _dt)
{
	(bool)(this->vecBramble.at(0)->dispawn) ? UpdateAnimFx(_dt, this->vecBramble[0]->sprite, this->vecBramble[0]->anim, 0) : UpdateAnimFx(_dt, this->vecBramble.at(this->vecBramble.size() - 1)->sprite, this->vecBramble.at(this->vecBramble.size() - 1)->anim, 0);
}

void Witch::DispawnBramble()
{
	if (this->vecBramble[0]->dispawn && this->vecBramble[0]->anim->currentFrame == this->vecBramble[0]->anim->maxFrame)
	{
		this->vecBramble.erase(this->vecBramble.begin(), this->vecBramble.begin() + 1);
		if (this->vecBramble.size() == 1)//vector empty
		{
			this->vecBramble.erase(this->vecBramble.begin(), this->vecBramble.begin() + 1);
			this->hitBoxBramble->setSize(sf::Vector2f{ 0,0 });
			this->state = WitchState::Idle;
			this->initFx = false;
			this->hasBramble = false;
		}
		else
		{
			this->vecBramble[0]->dispawn = true;
		}
	}
}

void Witch::AddBramble(float _playerY)
{
	Bramble* temp;
	if (!this->vecBramble.size())
	{
		temp = new Bramble{ this->sprite.spriteBrambleInvoc,nullptr,sf::Vector2f{this->mob->pos.x + 100.f,_playerY},true,false };
	}
	else
	{
		temp = new Bramble{ this->sprite.spriteBrambleInvoc,nullptr,sf::Vector2f{this->vecBramble.at(this->vecBramble.size() - 1)->pos.x - 180.f,this->vecBramble.at(this->vecBramble.size() - 1)->pos.y},true,false };
	}
	temp->anim = new Animation;

	sf::IntRect rect = sf::IntRect{ 0, 0, (int)(temp->sprite.animDim.x / temp->sprite.nbAnim.x),(int)(temp->sprite.nbAnim.y) };
	temp->anim->CreateLeftToRight(temp->sprite.sprite, temp->sprite.nbAnim.x, temp->sprite.nbAnim.y, temp->sprite.animDim);
	temp->sprite.sprite.setTextureRect(rect);
	temp->anim->currentFrame = 0;

	temp->sprite.sprite.setPosition(temp->pos);
	this->vecBramble.push_back(temp);
}

void Witch::UpdatePosBramble(Wave _wave, float _playerY)
{
	if (this->vecBramble.at(this->vecBramble.size() - 1)->anim->currentFrame == 6 && !this->vecBramble[0]->dispawn)
	{
		AddBramble(_playerY);
	}

	if (this->vecBramble.at(this->vecBramble.size() - 1)->pos.x < 0 + (1980.f * _wave) && !this->vecBramble[0]->dispawn)
	{
		this->vecBramble[0]->dispawn = true;
	}
}

void Witch::AddPlanteOnGround(Wave _wave, Audio& _audio)
{
	//when witch in anim create a new plant
	if (this->sprite.anim->currentFrame == 6 && this->nbInvoc < 3)
	{
		CreateNewPlant(_wave);
		this->nbInvoc++;
		_audio.PlaySoundNotRepeated("WITCH_SUMMON_PLANT_SOUND_1");
	}

	//if anim of witch is end
	if (this->sprite.anim->currentFrame == this->sprite.anim->maxFrame)
	{
		this->state = WitchState::Idle;
		this->nbInvoc = 0;
	}
}

void Witch::UpdatePlante(float _dt, Audio& _audio)
{
	for (int i = 0; i < this->vecPlante.size(); i++)
	{
		if (this->vecPlante[i]->canDelete) // delete the plante
		{
			bool temp = true;
			this->vecPlante[i]->animDeath->UpdateLeftToRight(this->vecPlante[i]->spriteDeath.sprite, _dt, this->vecPlante[i]->spriteDeath.speed, 0, temp, this->vecPlante[i]->spriteDeath.loop);

			if (this->vecPlante[i]->animDeath->currentFrame == this->vecPlante[i]->animDeath->maxFrame)
			{
				this->vecPlante.erase(this->vecPlante.begin() + i);
				return;
			}
		}

		if (!this->vecPlante[i]->isReady) // to growth the plante on ground
		{
			UpdateSpawn(_dt, this->vecPlante[i]);
		}
		else if (this->vecPlante[i]->isReady && !this->vecPlante[i]->eatSomeone && !this->vecPlante[i]->toLong && !this->vecPlante[i]->goOut) // wait state
		{
			UpdateWaitPlante(_dt, this->vecPlante[i]);
		}
		else if (this->vecPlante[i]->isReady && this->vecPlante[i]->toLong && !this->vecPlante[i]->canDelete) // too long on ground nobody came
		{
			UpdatePlanteTooLong(_dt, this->vecPlante[i]);
		}
		else if (this->vecPlante[i]->isReady && this->vecPlante[i]->goOut && !this->vecPlante[i]->eatSomeone) //somebody activate the plante
		{
			UpdateActivate(_dt, this->vecPlante[i]);
		}
		else if (this->vecPlante[i]->isReady && this->vecPlante[i]->eatSomeone && !this->vecPlante[i]->spitOut) // eat someone
		{
			UpdateHaveSomeone(_dt, this->vecPlante[i], _audio);
		}
		else if (this->vecPlante[i]->isReady && this->vecPlante[i]->eatSomeone && this->vecPlante[i]->spitOut && !this->vecPlante[i]->canDelete) // spit out the player
		{
			UpdateAnimPlante(this->vecPlante[i], _dt);
		}
	}

	//ternaire to know if there are some plant in the vec
	(bool)(this->vecPlante.size() > 0) ? this->hasSpawnPlant = true : this->hasSpawnPlant = false;
}

void Witch::UpdateActivate(float _dt, Plante* _plante)
{
	UpdateAnimPlante(_plante, _dt);
}

void Witch::UpdatePlanteTooLong(float _dt, Plante* _plante)
{
	//to delete it
	if (_plante->anim->currentFrame == _plante->anim->maxFrame && _plante->line)
	{
		_plante->canDelete = true;

		_plante->animDeath = new Animation;
		sf::IntRect rect = sf::IntRect(0, 0, (int)(_plante->spriteDeath.animDim.x / _plante->spriteDeath.nbAnim.x), (int)(_plante->spriteDeath.nbAnim.y));
		_plante->animDeath->CreateLeftToRight(_plante->spriteDeath.sprite, (const unsigned char)(_plante->spriteDeath.nbAnim.x), (const unsigned char)(_plante->spriteDeath.nbAnim.y), _plante->spriteDeath.animDim);
		_plante->spriteDeath.sprite.setTextureRect(rect);
		_plante->animDeath->currentFrame = 0;
	}

	UpdateAnimPlante(_plante, _dt);
}

bool Witch::GetHasScream()
{
	return this->hasScream;
}

//create and update anim when a crystal his hit
void Witch::UpdateCrystalHit(float _dt, Audio& _audio)
{
	timerSoundAmbianceCristal += _dt;
	for (int i = 0; i < NbCrystal; i++)
	{
		//init crystal Hit
		if (this->crystal[i].life > 0 && this->crystal[i].hit && !this->crystal[i].initHit)
		{
			sf::IntRect rect = sf::IntRect(0, 0, (int)(this->crystal[i].spriteHit.animDim.x / this->crystal[i].spriteHit.nbAnim.x), (int)(this->crystal[i].spriteHit.nbAnim.y));
			this->crystal[i].animHitCrystal->CreateLeftToRight(this->crystal[i].spriteHit.sprite, (const unsigned char)(this->crystal[i].spriteHit.nbAnim.x), (const unsigned char)(this->crystal[i].spriteHit.nbAnim.y), this->crystal[i].spriteHit.animDim);
			this->crystal[i].spriteHit.sprite.setTextureRect(rect);
			this->crystal[i].animHitCrystal->currentFrame = 0;

			//init pos sprite hit
			this->crystal[i].spriteHit.sprite.setPosition(sf::Vector2f{ this->crystal[i].pos.x,this->crystal[i].pos.y - 10.f });

			//to don't init in continue
			this->crystal[i].initHit = true;
		}

		//update crystal hit
		if (this->crystal[i].life > 0 && this->crystal[i].hit && this->crystal[i].initHit)
		{
			if (this->crystal[i].animHitCrystal->currentFrame == this->crystal[i].animHitCrystal->maxFrame)
			{
				this->crystal[i].hit = false;
				this->crystal[i].initHit = false;
			}

			bool temp = true;
			this->crystal[i].animHitCrystal->UpdateLeftToRight(this->crystal[i].spriteHit.sprite, _dt, this->crystal[i].spriteHit.speed, 0, temp, this->crystal[i].spriteHit.loop);
		}
	}

	if (this->hasSpawnCrystal && timerSoundAmbianceCristal > 1.f)
	{
		timerSoundAmbianceCristal = 0.f;
	}
}

void Witch::UpdateAnimPlante(Plante* _plante, float _dt)
{
	//to delete it
	if (_plante->anim->currentFrame == _plante->anim->maxFrame && _plante->line)
	{
		_plante->canDelete = true;

		_plante->animDeath = new Animation;
		sf::IntRect rect = sf::IntRect(0, 0, (int)(_plante->spriteDeath.animDim.x / _plante->spriteDeath.nbAnim.x), (int)(_plante->spriteDeath.nbAnim.y));
		_plante->animDeath->CreateLeftToRight(_plante->spriteDeath.sprite, (const unsigned char)(_plante->spriteDeath.nbAnim.x), (const unsigned char)(_plante->spriteDeath.nbAnim.y), _plante->spriteDeath.animDim);
		_plante->spriteDeath.sprite.setTextureRect(rect);
		_plante->animDeath->currentFrame = 0;
		_plante->spriteDeath.sprite.setPosition(_plante->pos);
	}

	//to change line in the spritesheeet
	if (_plante->anim->currentFrame == _plante->anim->maxFrame && !_plante->line)
	{
		_plante->line++;
		_plante->anim->currentFrame = 0;
	}

	bool temp = true;
	_plante->anim->UpdateLeftToRight(_plante->sprite.sprite, _dt, _plante->sprite.speed, _plante->line, temp, _plante->sprite.loop);
}

void Witch::UpdateHitBoxBramble()
{
	if (this->vecBramble[this->vecBramble.size() - 1]->anim->currentFrame == 4 || this->vecBramble[0]->anim->currentFrame == 8)
	{
		this->hitBoxBramble->setSize(sf::Vector2f{ (float)(200.f * this->vecBramble.size()),150.f });
		this->hitBoxBramble->setOrigin(sf::Vector2f{ this->hitBoxBramble->getSize().x ,this->hitBoxBramble->getSize().y });
	}
}

void Witch::SetPlayerBecomeFuzzy(bool _bool)
{
	this->playerBecomeFuzzy = _bool;
}

void Witch::UpdateBrambleSpawn(float _dt, Wave _wave, float _playerY, Audio& _audio)
{
	if (!this->initFx)
	{
		_audio.PlaySoundNotRepeated("WITCH_BRAMBLE_SCREAM_1", 100.f);
		_audio.PlaySoundNotRepeated("WITCH_SPELL_BRAMBLE_SOUND_1", 100.f);
		AddBramble(_playerY);
		this->hasBramble = true;
		this->initFx = true;
	}

	UpdatePosBramble(_wave, _playerY);
	UpdateAnimBramble(_dt);
	UpdateHitBoxBramble();
	DispawnBramble();
}

void Witch::InitNewAnim()
{
	if (this->sprite.spriteAnim[(int)this->state].nbAnim.x > 0)
	{
		sf::IntRect rect = sf::IntRect(0, 0, (int)(this->sprite.spriteAnim[(int)(this->state)].animDim.x / this->sprite.spriteAnim[(int)(this->state)].nbAnim.x), (int)(this->sprite.spriteAnim[(int)(this->state)].nbAnim.y));
		this->sprite.anim->CreateLeftToRight(this->sprite.spriteAnim[(int)(this->state)].sprite, (const unsigned char)(this->sprite.spriteAnim[(int)this->state].nbAnim.x), (const unsigned char)(this->sprite.spriteAnim[(int)(this->state)].nbAnim.y), this->sprite.spriteAnim[(int)(this->state)].animDim);
		this->sprite.spriteAnim[(int)this->state].sprite.setTextureRect(rect);
		this->sprite.anim->currentFrame = 0;

		this->mob->hitbox->setSize(sf::Vector2f{ this->sprite.spriteAnim[(int)this->state].animDim.x / this->sprite.spriteAnim[(int)this->state].nbAnim.x , this->sprite.spriteAnim[(int)(this->state)].animDim.y / this->sprite.spriteAnim[(int)(this->state)].nbAnim.y });
		this->mob->hitbox->setOrigin(this->mob->hitbox->getSize().x / 2, this->mob->hitbox->getSize().y);
	}
}

void Witch::Death(Audio& _audio)
{
	if (this->mob->life <= 0 && this->state != WitchState::Death && this->state == WitchState::Idle)
	{
		this->mob->scaleSprite = { 1,1 };
		this->mob->life = 0;
		this->state = WitchState::Death;
		this->mob->state = MobState::Death;
	}

	if (this->mob->life <= 0)
	{
		this->mob->life = 0;
	}
}

WitchState Witch::GetState()
{
	return this->state;
}

void Witch::SetState(WitchState _state)
{
	this->state = _state;
}

WitchState Witch::GetLastState()
{
	return this->lastState;
}

void Witch::SetLastState(WitchState _state)
{
	this->lastState = _state;
}

sf::Vector2f Witch::GetDimSprite()
{
	if (this->state != WitchState::Death)
	{
		return sf::Vector2f(this->sprite.spriteAnim[(int)this->state].sprite.getTexture()->getSize().x / this->sprite.spriteAnim[(int)this->state].nbAnim.x, this->sprite.spriteAnim[(int)this->state].sprite.getTexture()->getSize().y / this->sprite.spriteAnim[(int)this->state].nbAnim.y);
	}
}

Crystal* Witch::GetCrystal()
{
	return this->crystal;
}

bool Witch::GetHasSpawnCrystal()
{
	return this->hasSpawnCrystal;
}

bool Witch::GetHasBramble()
{
	return this->hasBramble;
}

sf::RectangleShape* Witch::GetHitBoxBramble()
{
	return this->hitBoxBramble;
}

std::vector<Plante*>& Witch::GetVectorPlant()
{
	return this->vecPlante;
}

int Witch::GetTargetPlayer()
{
	return this->playerChoose;
}

void Witch::SetTargetPlayer(int _newTarget)
{
	this->playerChoose = _newTarget;
}

bool Witch::GetPlayerBecomeFuzzy()
{
	return this->playerBecomeFuzzy;
}

bool Witch::GetHasSpawnPlant()
{
	return this->hasSpawnPlant;
}