#include "stdafx.h"
#include "EntityManagerPVP.hpp"
#include "Audio.h"
#include "Easings.h"

#define DistAxeY 45.f
bool SortFunctPVP(Sort* _y1, Sort* _y2);

void EntityManagerPVP::UpdatePVP(std::vector<Player*>& _player, std::vector<Projectile>& _projectile, std::vector<Sort*>& _sort, std::vector<Building*>& _build)
{
	//for story
	ProjectileCollision(_player, _projectile);
	AOEMAgicCollision(_player);
	AttackCollision(_player);
}

void EntityManagerPVP::UpdatePVP(std::vector<Player*>& _player, std::vector<Projectile>& _projectile, std::vector<Sort*>& _sort, std::vector<Building*>& _build, bool& _inTeamFight, sf::Font& _font, sf::Sprite& _heartSprite, Audio& _audio, sf::View& _view, float _dt, bool& _friendShip)
{
	//for arena
	ProjectileCollision(_player, _projectile, _inTeamFight, _friendShip);
	AOEMAgicCollision(_player, _inTeamFight, _friendShip);
	AttackCollision(_player, _inTeamFight, _friendShip);

	FillVectorDamage(_player, _font, _heartSprite);
	ErasedArrow(_projectile, _view);
	UpdateDamageVector(_dt);
	PlayerDeath(_player);
	RevivePlayer(_player, _audio, _inTeamFight);
	SortVector(_sort, _player, _build, _projectile);
}


void EntityManagerPVP::Reload(void)
{
	damageHud.clear();
}

//story
void EntityManagerPVP::ProjectileCollision(std::vector<Player*>& _player, std::vector<Projectile>& _projectile)
{
	for (int i = 0; i < _projectile.size(); i++)
	{
		if (_projectile[i].autor == AutorProjectile::PLAYER)
		{
			for (int j = 0; j < _player.size(); j++)
			{
				if (_projectile[i].playerId != j && _player[j]->GetState() != PlayerStates::DEAD && _player[j]->GetState() != PlayerStates::REVIVE && _player[j]->GetState() != PlayerStates::HIT && _player[j]->GetState() != PlayerStates::DASH)
				{
					if (Utils::AABB_AABB_Collision(_projectile[i].hitBox, _player[j]->GetHitBox()) &&
						_projectile[i].posAutor.y <= _player[j]->GetPos().y + DistAxeY && _projectile[i].posAutor.y >= _player[j]->GetPos().y - DistAxeY)
					{
						//magic
						if (_projectile[i].projetileType != TypeProjectile::ARROW)
						{
							_player[j]->SetStatus((Status)(_projectile[i].projetileType));
						}
						//arrow
						else
						{
							if (!_player[j]->GetParryState()) //don't parry
							{
								_player[j]->AddLife(2);
								(bool)(_player[j]->GetInAir()) ? _player[j]->SetPlayerState(KNOCKDOWN) : _player[j]->SetPlayerState(HIT);
							}
							//parry but don't block attack
							else if (_player[j]->GetParryState() && ((_projectile[i].pos.x > _player[j]->GetPos().x && _player[j]->GetLastDir() != RIGHT) || (_projectile[i].pos.x < _player[j]->GetPos().x && _player[j]->GetLastDir() != LEFT)))
							{
								_player[j]->AddLife(2);
								(bool)(_player[j]->GetInAir()) ? _player[j]->SetPlayerState(KNOCKDOWN) : _player[j]->SetPlayerState(HIT);
							}
							else //parry
							{
								_player[j]->SetShieldIsHit(true);
							}
						}

						_projectile.erase(_projectile.begin() + i);
						j = (int)_player.size();
					}
				}
			}
		}
	}
}

void EntityManagerPVP::AOEMAgicCollision(std::vector<Player*>& _player)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetState() != PlayerStates::DEAD && _player[i]->GetState() != PlayerStates::REVIVE && _player[i]->GetState() != PlayerStates::DASH)
		{
			for (int j = 0; j < _player.size(); j++)
			{
				if (i != j && _player[j]->GetState() != PlayerStates::DEAD && _player[j]->GetState() != PlayerStates::REVIVE && _player[j]->GetState() != PlayerStates::DASH && _player[j]->GetStatus() != (Status)((int)(_player[i]->GetAOEMagic().typeAOE) + 1))
				{
					if (Utils::AABB_AABB_Collision(_player[i]->GetAOEMagic().hitbox, _player[j]->GetHitBox()) && _player[i]->GetAOEMagic().isActive)
					{
						if (_player[i]->GetAOEMagic().typeAOE != TypeAOE::POISON &&
							(_player[i]->GetAOEMagic().hitbox->getPosition().y + _player[i]->GetSpriteAOE().getTexture()->getSize().y / 8.f) <= _player[j]->GetPos().y + DistAxeY && (_player[i]->GetAOEMagic().hitbox->getPosition().y + _player[i]->GetSpriteAOE().getTexture()->getSize().y / 8.f) >= _player[j]->GetPos().y - DistAxeY)
						{
							_player[j]->SetStatus((Status)((int)(_player[i]->GetAOEMagic().typeAOE) + 1));
							if (_player[j]->GetInAir())
							{
								(bool)(_player[i]->GetPos().x < _player[j]->GetPos().x) ? _player[j]->SetLastDir(LEFT) : _player[j]->SetLastDir(RIGHT);
							}

							(bool)(_player[j]->GetInAir()) ? _player[j]->SetPlayerState(KNOCKDOWN) : _player[j]->SetPlayerState(HIT);
							_player[j]->AddLife(4);

						}
						else if (_player[i]->GetPos().y <= _player[j]->GetPos().y + DistAxeY && _player[i]->GetPos().y >= _player[j]->GetPos().y - DistAxeY)
						{
							_player[j]->SetStatus((Status)((int)(_player[i]->GetAOEMagic().typeAOE) + 1));
							_player[j]->AddLife(4);
						}
					}
				}
			}
		}
	}
}

void EntityManagerPVP::AttackCollision(std::vector<Player*>& _player)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetState() != PlayerStates::DEAD && _player[i]->GetState() != PlayerStates::REVIVE && _player[i]->GetState() != PlayerStates::DASH)
		{
			for (int j = 0; j < _player.size(); j++)
			{
				if (i != j && _player[j]->GetState() != PlayerStates::DEAD && _player[j]->GetState() != PlayerStates::REVIVE && _player[j]->GetState() != PlayerStates::DASH && _player[j]->GetState() != PlayerStates::HIT &&
					_player[i]->GetPos().y >= _player[j]->GetPos().y - DistAxeY && _player[i]->GetPos().y <= _player[j]->GetPos().y + DistAxeY)
				{
					if ((_player[j]->GetState() == PlayerStates::KNOCKDOWN && _player[j]->GetCanKnockDownHit()) || _player[j]->GetState() != PlayerStates::KNOCKDOWN)
					{
						//light attack
						if (Utils::AABB_AABB_Collision(_player[i]->GetLightAttack(), _player[j]->GetHitBox()) && _player[i]->GetLightAttackActived())
						{
							if (_player[j]->GetInAir())
							{
								_player[j]->AddLife(2);
								(bool)(_player[i]->GetPos().x < _player[j]->GetPos().x) ? _player[j]->SetLastDir(LEFT) : _player[j]->SetLastDir(RIGHT);
								_player[j]->SetPlayerState(KNOCKDOWN);
								_player[i]->SetCanActiveHitbox(false);
							}
							else
							{
								//don't parry
								if (!_player[j]->GetParryState())
								{
									_player[j]->AddLife(2);

									if (_player[i]->GetComboNumber() == 3)
									{
										(bool)(_player[i]->GetPos().x < _player[j]->GetPos().x) ? _player[j]->SetLastDir(LEFT) : _player[j]->SetLastDir(RIGHT);
									}

									(bool)(_player[i]->GetComboNumber() == 3) ? _player[j]->SetPlayerState(KNOCKDOWN) : _player[j]->SetPlayerState(HIT);
									_player[i]->SetCanActiveHitbox(false);

								}
								//parry but don't block attack
								else if (_player[j]->GetParryState() && ((_player[j]->GetPos().x >= _player[i]->GetPos().x && _player[j]->GetLastDir() != LEFT) || (_player[j]->GetPos().x <= _player[i]->GetPos().x && _player[j]->GetLastDir() != RIGHT)))
								{
									_player[j]->AddLife(2);

									if (_player[i]->GetComboNumber() == 3)
									{
										(bool)(_player[i]->GetPos().x < _player[j]->GetPos().x) ? _player[j]->SetLastDir(LEFT) : _player[j]->SetLastDir(RIGHT);
									}

									(bool)(_player[i]->GetComboNumber() == 3) ? _player[j]->SetPlayerState(KNOCKDOWN) : _player[j]->SetPlayerState(HIT);
									_player[i]->SetCanActiveHitbox(false);
								}
								//parry and block attack
								else
								{
									_player[j]->SetShieldIsHit(true);
								}
							}
						}

						//heavy attack
						if (Utils::AABB_AABB_Collision(_player[i]->GetHeavyAttack(), _player[j]->GetHitBox()) && _player[i]->GetHeavyAttackActived())
						{
							_player[j]->AddLife(3);
							if (_player[i]->GetHeavyAttackGoUp())
							{
								if (_player[j]->GetState() != PlayerStates::KNOCKDOWN)
								{
									_player[j]->SetPropultionKnockDown(true);
								}

								(bool)(_player[i]->GetPos().x < _player[j]->GetPos().x) ? _player[j]->SetLastDir(LEFT) : _player[j]->SetLastDir(RIGHT);
								_player[j]->SetPlayerState(KNOCKDOWN);
								_player[i]->SetCanActiveHitbox(false);
							}
							else
							{
								(bool)(_player[j]->GetInAir()) ? _player[j]->SetPVPHeavyAttackDown(true) : _player[j]->SetPVPHeavyAttackDown(false);
								(bool)(_player[j]->GetInAir()) ? _player[j]->SetPlayerState(KNOCKDOWN) : _player[j]->SetPlayerState(HIT);
								_player[i]->SetCanActiveHitbox(false);
							}
						}
					}
				}
			}
		}
	}
}

//arena
void EntityManagerPVP::ProjectileCollision(std::vector<Player*>& _player, std::vector<Projectile>& _projectile, bool& _inTeamFight, bool& _friendShip)
{
	for (int i = 0; i < _projectile.size(); i++)
	{
		if (_projectile[i].autor == AutorProjectile::PLAYER)
		{
			for (int j = 0; j < _player.size(); j++)
			{
				if (!_inTeamFight || (_inTeamFight && _projectile[i].playerId != j && _player[_projectile[i].playerId]->GetTeam() != _player[j]->GetTeam()) || !_friendShip)
				{
					if (_projectile[i].playerId != j && _player[j]->GetState() != PlayerStates::DEAD && _player[j]->GetState() != PlayerStates::REVIVE && _player[j]->GetState() != PlayerStates::HIT && _player[j]->GetState() != PlayerStates::DASH && !_player[j]->GetInAir())
					{
						if (Utils::AABB_AABB_Collision(_projectile[i].hitBox, _player[j]->GetHitBox()) &&
							_projectile[i].posAutor.y <= _player[j]->GetPos().y + DistAxeY && _projectile[i].posAutor.y >= _player[j]->GetPos().y - DistAxeY)
						{
							if (!_player[j]->GetParryState()) //don't parry
							{
								//magic
								if (_projectile[i].projetileType != TypeProjectile::ARROW)
								{
									_player[j]->SetStatus((Status)(_projectile[i].projetileType));
								}
								_player[j]->AddLife(2);	
							    (bool)(_player[i]->GetPos().x < _player[j]->GetPos().x) ? _player[j]->SetLastDir(LEFT) : _player[j]->SetLastDir(RIGHT);
								_player[j]->SetPlayerState(HIT);
							}
							//parry but don't block attack
							else if (_player[j]->GetParryState() && ((_projectile[i].pos.x > _player[j]->GetPos().x && _player[j]->GetLastDir() != RIGHT) || (_projectile[i].pos.x < _player[j]->GetPos().x && _player[j]->GetLastDir() != LEFT)))
							{
								//magic
								if (_projectile[i].projetileType != TypeProjectile::ARROW)
								{
									_player[j]->SetStatus((Status)(_projectile[i].projetileType));
								}
								_player[j]->AddLife(2);

								
								(bool)(_player[i]->GetPos().x < _player[j]->GetPos().x) ? _player[j]->SetLastDir(LEFT) : _player[j]->SetLastDir(RIGHT);
								_player[j]->SetPlayerState(HIT);
							}
							else //parry
							{
								_player[j]->SetShieldIsHit(true);
							}

							_projectile.erase(_projectile.begin() + i);
							j = (int)_player.size();
						}
					}
				}
			}
		}
	}
}

void EntityManagerPVP::AOEMAgicCollision(std::vector<Player*>& _player, bool& _inTeamFight, bool& _friendShip)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetState() != PlayerStates::DEAD && _player[i]->GetState() != PlayerStates::REVIVE && _player[i]->GetState() != PlayerStates::DASH)
		{
			for (int j = 0; j < _player.size(); j++)
			{
				if (!_inTeamFight || (_inTeamFight && i != j && _player[i]->GetTeam() != _player[j]->GetTeam()) || !_friendShip)
				{
					if (i != j && _player[j]->GetState() != PlayerStates::DEAD && _player[j]->GetState() != PlayerStates::REVIVE && _player[j]->GetState() != PlayerStates::DASH && _player[j]->GetStatus() != (Status)((int)(_player[i]->GetAOEMagic().typeAOE) + 1))
					{
						if (Utils::AABB_AABB_Collision(_player[i]->GetAOEMagic().hitbox, _player[j]->GetHitBox()) && _player[i]->GetAOEMagic().isActive)
						{
							if (_player[i]->GetAOEMagic().typeAOE != TypeAOE::POISON &&
								(_player[i]->GetAOEMagic().hitbox->getPosition().y + _player[i]->GetSpriteAOE().getTexture()->getSize().y / 8.f) <= _player[j]->GetPos().y + DistAxeY && (_player[i]->GetAOEMagic().hitbox->getPosition().y + _player[i]->GetSpriteAOE().getTexture()->getSize().y / 8.f) >= _player[j]->GetPos().y - DistAxeY)
							{
								_player[j]->SetStatus((Status)((int)(_player[i]->GetAOEMagic().typeAOE) + 1));

								if (_player[j]->GetInAir())
								{
									(bool)(_player[i]->GetPos().x < _player[j]->GetPos().x) ? _player[j]->SetLastDir(LEFT) : _player[j]->SetLastDir(RIGHT);
								}

								(bool)(_player[j]->GetInAir()) ? _player[j]->SetPlayerState(KNOCKDOWN) : _player[j]->SetPlayerState(HIT);
								_player[j]->AddLife(4);

							}
							else if (_player[i]->GetPos().y <= _player[j]->GetPos().y + DistAxeY && _player[i]->GetPos().y >= _player[j]->GetPos().y - DistAxeY)
							{
								_player[j]->SetStatus((Status)((int)(_player[i]->GetAOEMagic().typeAOE) + 1));
								_player[j]->AddLife(4);
							}
						}
					}
				}
			}
		}
	}
}

void EntityManagerPVP::AttackCollision(std::vector<Player*>& _player, bool& _inTeamFight, bool& _friendShip)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetState() != PlayerStates::DEAD && _player[i]->GetState() != PlayerStates::REVIVE && _player[i]->GetState() != PlayerStates::DASH)
		{
			for (int j = 0; j < _player.size(); j++)
			{
				if (!_inTeamFight || (_inTeamFight && i != j && _player[i]->GetTeam() != _player[j]->GetTeam()) || !_friendShip)
				{
					if (i != j && _player[j]->GetState() != PlayerStates::DEAD && _player[j]->GetState() != PlayerStates::REVIVE && _player[j]->GetState() != PlayerStates::DASH && _player[j]->GetState() != PlayerStates::HIT &&
						_player[i]->GetPos().y >= _player[j]->GetPos().y - DistAxeY && _player[i]->GetPos().y <= _player[j]->GetPos().y + DistAxeY)
					{
						if ((_player[j]->GetState() == PlayerStates::KNOCKDOWN && _player[j]->GetCanKnockDownHit()) || _player[j]->GetState() != PlayerStates::KNOCKDOWN)
						{
							//light attack
							if (Utils::AABB_AABB_Collision(_player[i]->GetLightAttack(), _player[j]->GetHitBox()) && _player[i]->GetLightAttackActived())
							{
								if (_player[j]->GetInAir())
								{
									_player[j]->AddLife(2);
									(bool)(_player[i]->GetPos().x < _player[j]->GetPos().x) ? _player[j]->SetLastDir(LEFT) : _player[j]->SetLastDir(RIGHT);
									_player[j]->SetPlayerState(KNOCKDOWN);
									_player[i]->SetCanActiveHitbox(false);
								}
								else
								{
									//don't parry
									if (!_player[j]->GetParryState())
									{
										_player[j]->AddLife(2);

										if (_player[i]->GetComboNumber() == 3)
										{
											(bool)(_player[i]->GetPos().x < _player[j]->GetPos().x) ? _player[j]->SetLastDir(LEFT) : _player[j]->SetLastDir(RIGHT);
										}

										(bool)(_player[i]->GetComboNumber() == 3) ? _player[j]->SetPlayerState(KNOCKDOWN) : _player[j]->SetPlayerState(HIT);
										_player[i]->SetCanActiveHitbox(false);
									}
									//parry but don't block attack
									else if (_player[j]->GetParryState() && ((_player[j]->GetPos().x >= _player[i]->GetPos().x && _player[j]->GetLastDir() != LEFT) || (_player[j]->GetPos().x <= _player[i]->GetPos().x && _player[j]->GetLastDir() != RIGHT)))
									{
										_player[j]->AddLife(2);

										if (_player[i]->GetComboNumber() == 3)
										{
											(bool)(_player[i]->GetPos().x < _player[j]->GetPos().x) ? _player[j]->SetLastDir(LEFT) : _player[j]->SetLastDir(RIGHT);
										}

										(bool)(_player[i]->GetComboNumber() == 3) ? _player[j]->SetPlayerState(KNOCKDOWN) : _player[j]->SetPlayerState(HIT);
										_player[i]->SetCanActiveHitbox(false);
									}
									//parry and block attack
									else
									{
										_player[j]->SetShieldIsHit(true);
									}
								}
							}

							//heavy attack
							if (Utils::AABB_AABB_Collision(_player[i]->GetHeavyAttack(), _player[j]->GetHitBox()) && _player[i]->GetHeavyAttackActived())
							{
								_player[j]->AddLife(3);
								if (_player[i]->GetHeavyAttackGoUp())
								{
									if (_player[j]->GetState() != PlayerStates::KNOCKDOWN)
									{
										_player[j]->SetPropultionKnockDown(true);
									}
									(bool)(_player[i]->GetPos().x < _player[j]->GetPos().x) ? _player[j]->SetLastDir(LEFT) : _player[j]->SetLastDir(RIGHT);
									_player[j]->SetPlayerState(KNOCKDOWN);
									_player[i]->SetCanActiveHitbox(false);
								}
								else
								{
									(bool)(_player[j]->GetInAir()) ? _player[j]->SetPVPHeavyAttackDown(true) : _player[j]->SetPVPHeavyAttackDown(false);
									(bool)(_player[j]->GetInAir()) ? _player[j]->SetPlayerState(KNOCKDOWN) : _player[j]->SetPlayerState(HIT);
									_player[i]->SetCanActiveHitbox(false);
								}
							}
						}
					}
				}
			}
		}
	}
}

void EntityManagerPVP::ErasedArrow(std::vector<Projectile>& _projectile, sf::View& _view)
{
	for (int i = 0; i < _projectile.size(); i++)
	{
		if (_projectile[i].pos.x <= _view.getCenter().x - _view.getSize().x / 2 || _projectile[i].pos.x >= _view.getCenter().x + _view.getSize().x / 2)
		{
			_projectile.erase(_projectile.begin() + i);
		}
	}
}

void EntityManagerPVP::UpdateDamageVector(float _dt)
{
	for (int i = 0; i < damageHud.size(); i++)
	{
		if (damageHud[i]->timerEasing < 1)
		{
			damageHud[i]->pos = Easings::Lerp(damageHud[i]->startPosEasing, damageHud[i]->endPosEasing, Easings::InOutQuad(damageHud[i]->timerEasing));
			damageHud[i]->timerEasing += _dt / 2;
			damageHud[i]->damage.setPosition(damageHud[i]->pos);
			damageHud[i]->damageBack.setPosition(damageHud[i]->pos.x - 2, damageHud[i]->pos.y + 2);
			if (damageHud[i]->displayHeart)
			{
				damageHud[i]->heartSprite.setPosition({ damageHud[i]->pos.x + 32, damageHud[i]->pos.y });
			}

		}
		else
		{
			damageHud.erase(damageHud.begin() + i);
		}
	}
}

void EntityManagerPVP::FillVectorDamage(std::vector<Player*>& _player, sf::Font& _font, sf::Sprite& _spriteHeart)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetLastLife() != _player[i]->GetLife() && _player[i]->GetLife() < _player[i]->GetLastLife())
		{
			HudDamage2* temp = new HudDamage2();
			Utils::CreateText(temp->damage, _font, "-" + std::to_string(_player[i]->GetLastLife() - _player[i]->GetLife()), 20, sf::Color::Black, true);
			Utils::CreateText(temp->damageBack, _font, "-" + std::to_string(_player[i]->GetLastLife() - _player[i]->GetLife()), 20, sf::Color::White, true);
			_player[i]->SetLastLife();
			temp->heartSprite = _spriteHeart;
			temp->displayHeart = true;

			if (_player[i]->GetInAir())
			{
				temp->pos = { _player[i]->GetPosInAir().x + (rand() % 150 - 75), _player[i]->GetPosInAir().y - 180 };
			}
			else
			{
				temp->pos = { _player[i]->GetPos().x + (rand() % 150 - 75), _player[i]->GetPos().y - 180 };
			}
			temp->damage.setPosition(temp->pos);
			temp->damageBack.setPosition(temp->pos.x - 2, temp->pos.y + 2);
			temp->heartSprite.setPosition({ temp->damage.getPosition().x + 32, temp->damage.getPosition().y });
			temp->startPosEasing = temp->pos;
			temp->endPosEasing = { temp->pos.x,temp->pos.y - 100 };
			temp->timerEasing = 0.f;

			switch (_player[i]->GetStatus())
			{
			case Status::OnFire:
				temp->damage.setFillColor(sf::Color(242, 125, 12));
				break;
			case Status::OnElec:
				temp->damage.setFillColor(sf::Color(232, 200, 67));
				break;
			case Status::OnIce:
				temp->damage.setFillColor(sf::Color(63, 208, 212));
				break;
			case Status::OnPoison:
				temp->damage.setFillColor(sf::Color(0, 102, 20));
				break;
			}

			damageHud.push_back(temp);
		}
		else if (_player[i]->GetLastLife() != _player[i]->GetLife() && _player[i]->GetLife() > _player[i]->GetLastLife())
		{
			_player[i]->SetLastLife();
		}
	}
}

void EntityManagerPVP::DisplayDamageVector(sf::RenderWindow& _window)
{
	for (int i = 0; i < damageHud.size(); i++)
	{
		_window.draw(damageHud[i]->damageBack);
		_window.draw(damageHud[i]->damage);
		if (damageHud[i]->displayHeart)
		{
			_window.draw(damageHud[i]->heartSprite);
		}
	}
}

void EntityManagerPVP::DisplayDamageVector(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	for (int i = 0; i < damageHud.size(); i++)
	{
		sf::Vector2f damageBack = damageHud[i]->damageBack.getPosition();
		sf::Vector2f damage = damageHud[i]->damage.getPosition();

		damageHud[i]->damageBack.setPosition(sf::Vector2f{ damageBack.x + (TROAD_GAP * (-1 * damageBack.y + WORKING_HEIGHT) / 100.f),damageBack.y });
		_red.draw(damageHud[i]->damageBack);

		damageHud[i]->damageBack.setPosition(sf::Vector2f{ damageBack.x - (TROAD_GAP * (-1 * damageBack.y + WORKING_HEIGHT) / 100.f),damageBack.y });
		_blue.draw(damageHud[i]->damageBack);

		damageHud[i]->damageBack.setPosition(sf::Vector2f{ damage.x + (TROAD_GAP * (-1 * damage.y + WORKING_HEIGHT) / 100.f),damage.y });
		_red.draw(damageHud[i]->damage);

		damageHud[i]->damageBack.setPosition(sf::Vector2f{ damage.x - (TROAD_GAP * (-1 * damage.y + WORKING_HEIGHT) / 100.f),damage.y });
		_blue.draw(damageHud[i]->damage);

		damageHud[i]->damageBack.setPosition(damageBack);
		damageHud[i]->damage.setPosition(damage);

		if (damageHud[i]->displayHeart)
		{
			Utils::BlitSprite(damageHud[i]->heartSprite, sf::Vector2f{ damageHud[i]->heartSprite.getPosition().x + (TROAD_GAP * (-1 * damageHud[i]->heartSprite.getPosition().y + WORKING_HEIGHT) / 100.f),damageHud[i]->heartSprite.getPosition().y }, _red);
			Utils::BlitSprite(damageHud[i]->heartSprite, sf::Vector2f{ damageHud[i]->heartSprite.getPosition().x - (TROAD_GAP * (-1 * damageHud[i]->heartSprite.getPosition().y + WORKING_HEIGHT) / 100.f),damageHud[i]->heartSprite.getPosition().y }, _blue);
		}
	}
}

void EntityManagerPVP::PlayerDeath(std::vector<Player*>& _player)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetLife() <= 0 && _player[i]->GetState() != DEAD && _player[i]->GetState() != REVIVE)
		{
			_player[i]->SetPlayerState(DEAD);
		}
	}
}

void EntityManagerPVP::RevivePlayer(std::vector<Player*>& _player, Audio& _audio, bool& _inTeamFight)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (sf::Joystick::isButtonPressed(_player[i]->GetID(), (unsigned int)ControllerButtonXBOX::RIGHT_STICK_PUSH) || sf::Joystick::isButtonPressed(_player[i]->GetID(), (unsigned int)ControllerButtonPS4::RIGHT_STICK_PUSH))
		{
			if (_inTeamFight)
			{
				for (unsigned char j = 0; j < _player.size(); j++)
				{
					if ((_player[j]->state == DEAD && _player[j]->GetCanBeRevive()) && (_player[i]->state == RUN || _player[i]->state == IDLE) && Utils::Distance(_player[j]->GetPos(), _player[i]->GetPos()) <= 150 && i != j && _player[i]->GetPlayerTeam() == _player[j]->GetPlayerTeam())
					{
						_player[i]->SetPlayerState(PRAY);
						_audio.PlaySoundNotRepeated("PLAYER_" + _player[i]->color[_player[i]->GetPlayerSelected()] + "_REZ_SCREAM_" + std::to_string(rand() % 2 + 1), 100.f);
					}
				}
			}
			else
			{
				for (unsigned char j = 0; j < _player.size(); j++)
				{
					if ((_player[j]->state == DEAD && _player[j]->GetCanBeRevive()) && (_player[i]->state == RUN || _player[i]->state == IDLE) && Utils::Distance(_player[j]->GetPos(), _player[i]->GetPos()) <= 150 && i != j)
					{
						_player[i]->SetPlayerState(PRAY);
						_audio.PlaySoundNotRepeated("PLAYER_" + _player[i]->color[_player[i]->GetPlayerSelected()] + "_REZ_SCREAM_" + std::to_string(rand() % 2 + 1), 100.f);
					}
				}
			}
		}

		for (unsigned char j = 0; j < _player.size(); j++)
		{
			if (_inTeamFight)
			{
				if ((_player[j]->state == DEAD && _player[j]->GetCanBeRevive()) && _player[i]->GetHasFinishPray() && _player[i]->GetState() == PRAY && i != j && _player[j]->GetTeam() == _player[i]->GetTeam())
				{
					_player[i]->SetHasFinishPray(false);
					_player[i]->SetPlayerState(IDLE);
					_player[j]->SetCanBeRevive(false);
					_player[j]->SetPlayerState(REVIVE);
				}
			}
			else
			{
				if ((_player[j]->state == DEAD && _player[j]->GetCanBeRevive()) && _player[i]->GetHasFinishPray() && _player[i]->GetState() == PRAY && Utils::Distance(_player[j]->GetPos(), _player[i]->GetPos()) <= 150 && i != j)
				{
					_player[i]->SetHasFinishPray(false);
					_player[i]->SetPlayerState(IDLE);
					_player[j]->SetCanBeRevive(false);
					_player[j]->SetPlayerState(REVIVE);
				}
			}
		}
	}
}

void EntityManagerPVP::SortVector(std::vector<Sort*>& _sort, std::vector<Player*>& _player, std::vector<Building*>& _build, std::vector<Projectile>& _proj)
{
	_sort.erase(_sort.begin(), _sort.end());

	for (int i = 0; i < _proj.size(); i++)
	{
		Sort* sort = new Sort{ _proj[i].posAutor.y + 10.f,i,SortVector::Projectile };
		_sort.push_back(sort);
	}

	for (int i = 0; i < _player.size(); i++)
	{
		Sort* sort = new Sort{ _player[i]->GetPos().y + 5.f,i,SortVector::Player };
		_sort.push_back(sort);

		if (_player[i]->GetIceSpellHere())
		{
			Sort* sort = new Sort{ (_player[i]->GetAOEMagic().hitbox->getPosition().y + _player[i]->GetSpriteAOE().getTexture()->getSize().y / 8.f) - 10.f,i,SortVector::Ice };
			_sort.push_back(sort);
		}
	}

	for (int i = 0; i < _build.size(); i++)
	{
		Sort* sort = new Sort{ _build[i]->posY,i,SortVector::Building };
		_sort.push_back(sort);
	}

	std::sort(_sort.begin(), _sort.end(), SortFunctPVP);
}

//to sort the vector entity
bool SortFunctPVP(Sort* _y1, Sort* _y2)
{
	return _y1->posY < _y2->posY;
}