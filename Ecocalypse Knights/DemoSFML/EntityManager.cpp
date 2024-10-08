#include "stdafx.h"
#include "EntityManager.h"
#include "Audio.h"
#include "CharacterSelection.h"
#include "Scarecrow.hpp"
#include "HazelNut.hpp"

#pragma region Parameters
#define DistCollideMob 75.f
#define TimerKnockBackMob 0.5f
#define TimerStatusMob 1.2f
#define TimerHitMob 0.6f
#define DistAxeY 35.f
#define DimLifeBar 100
#define DimLifeBarBoss 1280
#pragma endregion

sf::Vector2f operator%(sf::Vector2f _vec1, sf::Vector2f _vec2)
{
	return sf::Vector2f{ (float)((int)_vec1.x % (int)_vec2.x),(float)((int)_vec1.y % (int)_vec2.y) };
}

EntityManager::EntityManager()
{
	sf::Image image;
	image.loadFromFile("Assets/Images/Entities/Players/Magic/Player1AOEMagic.png");
	this->tempImage = image;
	this->size = image.getSize();
	this->oneFrameSize = { (float)(size.x / 6), (float)(size.y / 8) };

	image.loadFromFile("Assets/Images/Entities/Mobs/Boar/Boar_SlashH.png");
	this->dimSlash = sf::Vector2f{ image.getSize() };
	this->dimSlashOneFrame = sf::Vector2f{ (float)(this->dimSlash.x / 46),270.f };
	this->imageSlash = image;
}

#pragma region Player
void EntityManager::MobArrowCollision(std::vector<Projectile>& _projectile, std::vector<Mob*>& _mob, std::vector<Player*>& _player, Audio& _audio)
{
	for (int k = 0; k < _projectile.size(); k++)
	{
		if (_projectile[k].autor == AutorProjectile::PLAYER)
		{
			for (int i = 0; i < _mob.size(); i++)
			{
				if (_mob[i]->GetState() != MobState::Death && _mob[i]->GetState() != MobState::Dispawn && TestWitch(_mob[i]))
				{
					if (Utils::AABB_AABB_Collision(_mob[i]->GetHitBox(), _projectile[k].hitBox) && (!_mob[i]->GetInAir() || TestWitch(_mob[i])) &&
						_projectile[k].posAutor.y <= _mob[i]->GetPos().y + DistAxeY && _projectile[k].posAutor.y >= _mob[i]->GetPos().y - DistAxeY)
					{
						switch (_projectile[k].projetileType)
						{
						case TypeProjectile::DISTANCE_MAGIC_ELECTRICAL://elec
							_mob[i]->SetTimerStatus(TimerStatusMob);
							_mob[i]->SetStatus(Status::OnElec);
							_mob[i]->SetKnockdownState();
							_mob[i]->SetTimerKnockDown(3.f);
							_audio.PlaySoundNotRepeated("ELECTRICBALL", 100.f);
							break;

						case TypeProjectile::DISTANCE_MAGIC_FIRE://fire
							_mob[i]->SetTimerStatus(TimerStatusMob);
							_mob[i]->SetStatus(Status::OnFire);
							_audio.PlaySoundNotRepeated("FIREBALL", 100.f);
							break;

						case TypeProjectile::DISTANCE_MAGIC_ICE://ice
							_mob[i]->SetStatus(Status::OnIce);
							_mob[i]->SetKnockdownState();
							_mob[i]->SetTimerKnockDown(3.f);
							_mob[i]->SetTimerStatus(TimerStatusMob);
							_audio.PlaySoundNotRepeated("ICEBALL", 100.f);
							break;

						case TypeProjectile::DISTANCE_MAGIC_POISON://poison
							_mob[i]->SetTimerStatus(TimerStatusMob);
							_mob[i]->SetStatus(Status::OnPoison);
							_audio.PlaySoundNotRepeated("POISONBALL", 100.f);
							break;

						default://arrow
							if (_mob[i]->GetRace() != RaceMob::Boar)
							{
								_mob[i]->SetKnockbackState();
							}
							_audio.PlaySoundNotRepeated("ARROW_LAND", 100.f);
							break;
						}

						if (_projectile[k].projetileType != TypeProjectile::ARROW)
						{
							_mob[i]->SetDamageMagic(_player[_projectile[k].playerId]->GetWisdom());
						}

						_mob[i]->AddLife(_projectile[k].damage);
						_mob[i]->SetLastStatus(Status::None);
						_mob[i]->SetTimerHit(TimerHitMob);
						_mob[i]->SetDirection(-_projectile[k].direction);

						if (_mob[i]->GetRace() == RaceMob::Boar)
						{
							_mob[i]->GetBoar()->AddNbAttack();
						}

						//Death mob
						if (_mob[i]->GetLife() <= 0 && _mob[i]->GetState() != MobState::Death)
						{
							_player[_projectile[k].playerId]->AddXp(_mob[i]->GetXp());
							_player[_projectile[k].playerId]->AddGold(_mob[i]->GetGold());
							_mob[i]->SetState(MobState::Death);
							_mob[i]->SetInFallState();
						}

						_projectile.erase(_projectile.begin() + k);
						i = (int)_mob.size();
					}
				}
			}
		}
	}
}

void EntityManager::AttackOnMobCollision(std::vector<Mob*>& _mob, std::vector<Player*>& _player, Audio& _audio)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetLightAttackActived() || _player[i]->GetHeavyAttackActived())
		{
			for (int j = 0; j < _mob.size(); j++)
			{
				if (_mob[j]->GetState() != MobState::Death && _mob[j]->GetState() != MobState::Dispawn && TestWitch(_mob[j]) && TestBoar(_mob[j], _player[i]))
				{
					//light attack collision
					if (_player[i]->GetLightAttackActived() && Utils::AABB_AABB_Collision(_player[i]->GetLightAttack(), _mob[j]->GetHitBox()) &&
						_player[i]->GetPos().y <= _mob[j]->GetPos().y + DistAxeY && _player[i]->GetPos().y >= _mob[j]->GetPos().y - DistAxeY)
					{
						_mob[j]->SetKnockbackState();
						_mob[j]->AddLife(_player[i]->GetComboNumber() == 3 ? (_player[i]->GetStrengthDamage() + 5.f) : _player[i]->GetStrengthDamage());
						_mob[j]->SetDirection(_mob[j]->Trigo(_player[i]->GetPos()));
#pragma region SoundHit
						if (_mob[j]->GetRace() == RaceMob::HazelNut)
						{
							_audio.PlaySoundNotRepeated("LIGHT_ATTACK_ON_HAZELNUT", 100.f);
							if (rand() % _player.size() < 1)
							{
								_audio.PlaySoundNotRepeated("HAZELNUT_GET_HIT_" + std::to_string(rand() % 3 + 1), 100.f);
							}

						}
						else if (_mob[j]->GetRace() == RaceMob::Mushroom)
						{
							if (rand() % (_player.size() * 3) < 1)
							{
								_audio.PlaySoundNotRepeated("MUSHROOM_GET_HIT", 100.f);
							}
						}
						else if (_mob[j]->GetRace() == RaceMob::Pig)
						{
							if (rand() % (_player.size() * 3) < 1)
							{
								_audio.PlaySoundNotRepeated("PIG_GET_HIT", 100.f);
							}
						}
						else if (_mob[j]->GetRace() == RaceMob::Bird)
						{
							if (rand() % (_player.size() * 3) < 1)
							{
								_audio.PlaySoundNotRepeated("BIRD_GET_HIT", 100.f);
							}

						}
						else if (_mob[j]->GetRace() == RaceMob::Boar)
						{
							if (rand() % (_player.size() * 3) < 1)
							{
								_audio.PlaySoundNotRepeated("BOAR_SUFFERING", 100.f);
							}
						}
						else if (_mob[j]->GetRace() == RaceMob::Witch)
						{
							if (rand() % _player.size() < 1)
							{
								float randomPitch = (rand() / (float)RAND_MAX) / 5 - 0.1f;
								_audio.SetPitch("WITCH_GET_HIT_SCREAM_1", 0.85 + randomPitch);
								_audio.SetPitch("WITCH_GET_HIT_SCREAM_2", 0.85 + randomPitch);
								_audio.SetPitch("WITCH_GET_HIT_SCREAM_3", 0.85 + randomPitch);
								_audio.PlaySoundNotRepeated("WITCH_GET_HIT_SCREAM_" + std::to_string(rand() % 3 + 1), 100.f);
							}
						}
#pragma endregion				
						if (_mob[j]->GetRace() == RaceMob::Boar)
						{
							_mob[j]->GetBoar()->AddNbAttack();
						}

						_player[i]->SetCanActiveHitbox(false);
					}
					else //ligth attack miss
					{
						_audio.PlaySoundNotRepeated("LIGHT_ATTACK_MISS", 100.f);
					}

					//heavy attack collision
					if (_player[i]->GetHeavyAttackActived() && Utils::AABB_AABB_Collision(_player[i]->GetHeavyAttack(), _mob[j]->GetHitBox()) &&
						_player[i]->GetPos().y <= _mob[j]->GetPos().y + DistAxeY && _player[i]->GetPos().y >= _mob[j]->GetPos().y - DistAxeY)
					{
						if (_player[i]->GetHeavyAttackGoUp() && !_mob[j]->GetInAir())//heavy attack on ground
						{
							_mob[j]->AddLife((int)(_player[i]->GetStrengthDamage() + 5.f));
							_mob[j]->SetInUpState();
							if (_mob[j]->GetRace() == RaceMob::Boar)
							{
								_mob[j]->GetBoar()->AddNbAttack();
							}
						}
						else if (!_player[i]->GetHeavyAttackGoUp() && _mob[j]->GetInAir())//heavy attack in air
						{
							_mob[j]->AddLife((int)(_player[i]->GetStrengthDamage() + 5.f));
							_mob[j]->SetInFallState();
							if (_mob[j]->GetRace() == RaceMob::Boar)
							{
								_mob[j]->GetBoar()->AddNbAttack();
							}
						}
						else //heavy attack but not go up and mob on ground
						{
							_mob[j]->AddLife(_player[i]->GetStrengthDamage() + 10.f);
							if (_mob[j]->GetRace() == RaceMob::Boar)
							{
								_mob[j]->GetBoar()->AddNbAttack();
							}
						}

						_player[i]->SetCanActiveHitbox(false);
					}

					_mob[j]->SetDirection(_mob[j]->Trigo(_player[i]->GetPos()));

					//Death mob
					if (_mob[j]->GetLife() <= 0 && _mob[j]->GetState() != MobState::Death)
					{
						_player[i]->AddXp(_mob[j]->GetXp());
						_player[i]->AddGold(_mob[j]->GetGold());
						_mob[j]->SetState(MobState::Death);
						_mob[j]->SetInFallState();
					}
				}
			}
		}
	}
}

//collide FOOD
void EntityManager::PlayerLifeDropCollision(std::vector<Player*>& _player, std::vector<Drop*>& _drop, Audio& _audio)
{
	for (int j = 0; j < _player.size(); j++)
	{
		if (_player[j]->GetState() != DEAD && !_player[j]->GetInAir() && _player[j]->GetState() != REVIVE && _player[j]->GetLife() < _player[j]->GetMaxLife())
		{
			for (int i = 0; i < _drop.size(); i++)
			{
				if (Utils::AABB_AABB_Collision(&_drop[i]->rect, _player[j]->GetHitBox()) && _player[j]->GetPos().y <= _drop[i]->pos.y + 65.f && _player[j]->GetPos().y >= _drop[i]->pos.y - 5.f)
				{
					_audio.PlaySoundNotRepeated("PLAYER_EAT");

					_drop.erase(_drop.begin() + i);
					_player[j]->AddLife(-6);
				}
			}
		}
	}
}

//Dispawn player
void EntityManager::PlayerDead(std::vector<Player*>& _player, Audio& _audio)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetLife() <= 0 && _player[i]->state != DEAD && _player[i]->state != REVIVE)
		{
			_player[i]->SetPlayerState(DEAD);
			_audio.PlaySoundNotRepeated("PLAYER_" + _player[i]->color[_player[i]->GetPlayerSelected()] + "_DEATH_SCREAM_1");
		}
	}
}

//revive a player
void EntityManager::RevivePlayer(std::vector<Player*>& _player, Audio& _audio)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (sf::Joystick::isButtonPressed(_player[i]->GetID(), (unsigned int)ControllerButtonXBOX::RIGHT_STICK_PUSH) || sf::Joystick::isButtonPressed(_player[i]->GetID(), (unsigned int)ControllerButtonPS4::RIGHT_STICK_PUSH))
		{
			for (unsigned char j = 0; j < _player.size(); j++)
			{
				if ((_player[j]->state == DEAD && _player[j]->GetCanBeRevive()) && (_player[i]->state == RUN || _player[i]->state == IDLE) && Utils::Distance(_player[j]->GetPos(), _player[i]->GetPos()) <= 150 && i != j)
				{
					_player[i]->SetPlayerState(PRAY);
					_audio.PlaySoundNotRepeated("PLAYER_" + _player[i]->color[_player[i]->GetPlayerSelected()] + "_REZ_SCREAM_1", 100.f);
				}
			}
		}

		for (unsigned char j = 0; j < _player.size(); j++)
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

//erased arrow out of bound
void EntityManager::ErasedArrow(std::vector<Projectile>& _projectile, sf::View& _view)
{
	for (int i = 0; i < _projectile.size(); i++)
	{
		if (_projectile[i].pos.x <= _view.getCenter().x - _view.getSize().x / 2 || _projectile[i].pos.x >= _view.getCenter().x + _view.getSize().x / 2)
		{
			_projectile.erase(_projectile.begin() + i);
		}
	}
}

/////////////surdef for scarecrow
void EntityManager::MobArrowCollision(std::vector<Projectile>& _projectile, std::vector<Scarecrow*>& _scarecrow, std::vector<Player*>& _player, Audio& _audio)
{
	for (int k = 0; k < _projectile.size(); k++)
	{
		if (_projectile[k].autor == AutorProjectile::PLAYER)
		{
			for (int i = 0; i < _scarecrow.size(); i++)
			{
				if (_scarecrow[i]->GetState() != StateScarecrow::Hit && _scarecrow[i]->GetState() != StateScarecrow::Death && Utils::AABB_AABB_Collision(_scarecrow[i]->GetHitBox(), _projectile[k].hitBox) &&
					_projectile[k].posAutor.y <= _scarecrow[i]->GetPos().y + DistAxeY && _projectile[k].posAutor.y >= _scarecrow[i]->GetPos().y - DistAxeY)
				{
					switch (_projectile[k].projetileType)
					{
					case TypeProjectile::DISTANCE_MAGIC_ELECTRICAL://elec
						_scarecrow[i]->SetTimerStatus(TimerStatusMob);
						_scarecrow[i]->SetStatus(Status::OnElec);
						_audio.PlaySoundNotRepeated("ELECTRICBALL", 100.f);
						break;

					case TypeProjectile::DISTANCE_MAGIC_FIRE://fire
						_scarecrow[i]->SetTimerStatus(TimerStatusMob);
						_scarecrow[i]->SetStatus(Status::OnFire);
						_audio.PlaySoundNotRepeated("FIREBALL", 100.f);
						break;

					case TypeProjectile::DISTANCE_MAGIC_ICE://ice
						_scarecrow[i]->SetTimerStatus(TimerStatusMob);
						_scarecrow[i]->SetStatus(Status::OnIce);
						_audio.PlaySoundNotRepeated("ICEBALL", 100.f);
						break;

					case TypeProjectile::DISTANCE_MAGIC_POISON://poison
						_scarecrow[i]->SetTimerStatus(TimerStatusMob);
						_scarecrow[i]->SetStatus(Status::OnPoison);
						_audio.PlaySoundNotRepeated("POISONBALL", 100.f);
						break;

					default://arrow
						_audio.PlaySoundNotRepeated("ARROW_LAND", 100.f);
						break;
					}

					if (_projectile[k].projetileType != TypeProjectile::ARROW)
					{
						_scarecrow[i]->SetDamageMagic(_player[_projectile[k].playerId]->GetWisdom());
					}

					_scarecrow[i]->AddLife(_projectile[k].damage);
					_scarecrow[i]->SetState(StateScarecrow::Hit);
					_projectile.erase(_projectile.begin() + k);
					i = (int)_scarecrow.size();
				}
			}
		}
	}
}
void EntityManager::AttackOnMobCollision(std::vector<Scarecrow*>& _scarecrow, std::vector<Player*>& _player, Audio& _audio)
{
	for (int i = 0; i < _player.size(); i++)
	{
		for (int j = 0; j < _scarecrow.size(); j++)
		{
			if (_scarecrow[j]->GetState() != StateScarecrow::Hit && _scarecrow[j]->GetState() != StateScarecrow::Death)
			{
				//light attack collision
				if (_player[i]->GetLightAttackActived() && Utils::AABB_AABB_Collision(_player[i]->GetLightAttack(), _scarecrow[j]->GetHitBox()) &&
					_player[i]->GetPos().y <= _scarecrow[j]->GetPos().y + DistAxeY && _player[i]->GetPos().y >= _scarecrow[j]->GetPos().y - DistAxeY)
				{
					_scarecrow[j]->SetState(StateScarecrow::Hit);
					_scarecrow[j]->AddLife(_player[i]->GetComboNumber() == 3 ? (_player[i]->GetStrengthDamage() + 5.f) : _player[i]->GetStrengthDamage());
					_player[i]->SetCanActiveHitbox(false);
					_audio.PlaySoundNotRepeated("LIGHT_ATTACK_ON_HAZELNUT", 100.f);
				}

				//heavy attack collision
				if (_player[i]->GetHeavyAttackActived() && Utils::AABB_AABB_Collision(_player[i]->GetHeavyAttack(), _scarecrow[j]->GetHitBox()) &&
					_player[i]->GetPos().y <= _scarecrow[j]->GetPos().y + DistAxeY && _player[i]->GetPos().y >= _scarecrow[j]->GetPos().y - DistAxeY)
				{
					if (_player[i]->GetHeavyAttackGoUp())//heavy attack on ground
					{
						_scarecrow[j]->SetState(StateScarecrow::Hit);
						_scarecrow[j]->AddLife((int)(_player[i]->GetStrengthDamage() + 5.f));
					}
					else if (!_player[i]->GetHeavyAttackGoUp())//heavy attack in air
					{
						_scarecrow[j]->SetState(StateScarecrow::Hit);
						_scarecrow[j]->AddLife(_player[i]->GetStrengthDamage() + 10.f);
					}
					else //heavy attack but not go up and mob on ground
					{
						_scarecrow[j]->SetState(StateScarecrow::Hit);
						_scarecrow[j]->AddLife(_player[i]->GetStrengthDamage() + 10.f);
					}

					_player[i]->SetCanActiveHitbox(false);
				}
			}
		}
	}
}
void EntityManager::PlayerLongMagicAttack(std::vector<Scarecrow*>& _scarecrow, std::vector<Player*>& _player, Audio& _audio)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetState() != DEAD && _player[i]->GetState() != REVIVE)
		{
			for (int j = 0; j < _scarecrow.size(); j++)
			{
				if (_scarecrow[j]->GetState() != StateScarecrow::Hit && _scarecrow[j]->GetState() != StateScarecrow::Death && Utils::AABB_AABB_Collision(_player[i]->GetAOEMagic().hitbox, _scarecrow[j]->GetHitBox()) && _player[i]->GetAOEMagic().isActive)
				{
					sf::Vector2f mobPos;
					sf::Vector2f spritePos;
					sf::Vector2f oneFrameSize;
					sf::Vector2f viperSpellInfo;
					sf::Vector2f diffPosMobPlayer;
					sf::Vector2f testValue;

					if (_player[i]->GetAOEMagic().typeAOE == TypeAOE::POISON)
					{
						spritePos = _player[i]->GetAOEMagic().hitbox->getPosition();
						viperSpellInfo = _player[i]->GetViperSpellInfo();
					}

					switch (_player[i]->GetAOEMagic().typeAOE)
					{
					case TypeAOE::FIRE:
						if (!_scarecrow[j]->GetInitStatus() &&
							_player[i]->GetPos().y <= _scarecrow[j]->GetPos().y + DistAxeY && _player[i]->GetPos().y >= _scarecrow[j]->GetPos().y - DistAxeY)
						{
							_scarecrow[j]->SetStatus(Status::OnFire);
							_scarecrow[j]->SetState(StateScarecrow::Hit);
							_scarecrow[j]->SetTimerStatus(TimerStatusMob);
						}
						break;

					case TypeAOE::POISON:
						if (_scarecrow[j]->GetStatus() != Status::OnPoison)
						{
							mobPos = _scarecrow[j]->GetPos();
							diffPosMobPlayer = { mobPos - spritePos };
							testValue = { this->oneFrameSize.x * viperSpellInfo.x + diffPosMobPlayer.x, this->oneFrameSize.y * viperSpellInfo.y + diffPosMobPlayer.y };

							if (diffPosMobPlayer.x >= 0 && diffPosMobPlayer.y >= 0)
							{
								if ((testValue.x > this->oneFrameSize.x * viperSpellInfo.x) && (testValue.x < this->oneFrameSize.x * viperSpellInfo.x + this->oneFrameSize.x))
								{
									if ((testValue.y > this->oneFrameSize.y * viperSpellInfo.y) && (testValue.y < this->oneFrameSize.y * viperSpellInfo.y + this->oneFrameSize.y))
									{
										if (this->tempImage.getPixel((unsigned int)testValue.x, (unsigned int)testValue.y).a != 0)
										{
											if (!_scarecrow[j]->GetInitStatus())
											{
												_scarecrow[j]->SetState(StateScarecrow::Hit);
												_scarecrow[j]->SetTimerStatus(TimerStatusMob);
												_scarecrow[j]->SetStatus(Status::OnPoison);
											}
										}
									}
								}
							}
						}
						break;

					case TypeAOE::ICE:
						if (!_scarecrow[j]->GetInitStatus() &&
							(_player[i]->GetAOEMagic().hitbox->getPosition().y + _player[i]->GetSpriteAOE().getTexture()->getSize().y / 8.f) <= _scarecrow[j]->GetPos().y + DistAxeY
							&& (_player[i]->GetAOEMagic().hitbox->getPosition().y + _player[i]->GetSpriteAOE().getTexture()->getSize().y / 8.f) >= _scarecrow[j]->GetPos().y - DistAxeY)
						{
							_scarecrow[j]->SetStatus(Status::OnIce);
							_scarecrow[j]->SetState(StateScarecrow::Hit);
							_scarecrow[j]->SetTimerStatus(TimerStatusMob);
						}
						break;

					case TypeAOE::ELECTROCUTION:
						if (!_scarecrow[j]->GetInitStatus() &&
							_player[i]->GetPos().y <= _scarecrow[j]->GetPos().y + DistAxeY && _player[i]->GetPos().y >= _scarecrow[j]->GetPos().y - DistAxeY)
						{
							_scarecrow[j]->SetStatus(Status::OnElec);
							_scarecrow[j]->SetState(StateScarecrow::Hit);
							_scarecrow[j]->SetTimerStatus(TimerStatusMob);
						}
						break;

					default:
						break;
					}
					_scarecrow[j]->SetDamageMagic(_player[i]->GetWisdom());
				}
			}
		}
	}
}
/////////////surdef for scarecrow

//collision long magic with mob
void EntityManager::PlayerLongMagicAttack(std::vector<Mob*>& _mob, std::vector<Player*>& _player)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetState() != DEAD && _player[i]->GetState() != REVIVE)
		{
			sf::Vector2f mobPos;
			sf::Vector2f spritePos;
			sf::Vector2f viperSpellInfo;
			sf::Vector2f diffPosMobPlayer;
			sf::Vector2f testValue;

			if (_player[i]->GetAOEMagic().typeAOE == TypeAOE::POISON)
			{
				spritePos = _player[i]->GetAOEMagic().hitbox->getPosition();
				viperSpellInfo = _player[i]->GetViperSpellInfo();
			}

			for (int j = 0; j < _mob.size(); j++)
			{
				if (_mob[j]->GetState() != MobState::Death && _mob[j]->GetState() != MobState::Dispawn)
				{
					if (Utils::AABB_AABB_Collision(_player[i]->GetAOEMagic().hitbox, _mob[j]->GetHitBox()) && _player[i]->GetAOEMagic().isActive)
					{
						switch (_player[i]->GetAOEMagic().typeAOE)
						{
						case TypeAOE::FIRE:
							if (_player[i]->GetPos().y <= _mob[j]->GetPos().y + DistAxeY && _player[i]->GetPos().y >= _mob[j]->GetPos().y - DistAxeY)
							{
								_mob[j]->SetKnockbackState();
								_mob[j]->SetTimerStatus(TimerStatusMob);
								_mob[j]->SetStatus(Status::OnFire);
							}
							break;

						case TypeAOE::POISON:
							if (_mob[j]->GetStatus() != Status::OnPoison)
							{
								mobPos = _mob[j]->GetPos();
								diffPosMobPlayer = { mobPos - spritePos };
								testValue = { this->oneFrameSize.x * viperSpellInfo.x + diffPosMobPlayer.x, this->oneFrameSize.y * viperSpellInfo.y + diffPosMobPlayer.y };

								if (diffPosMobPlayer.x >= 0 && diffPosMobPlayer.y >= 0)
								{
									if ((testValue.x > this->oneFrameSize.x * viperSpellInfo.x) && (testValue.x < this->oneFrameSize.x * viperSpellInfo.x + this->oneFrameSize.x))
									{
										if ((testValue.y > this->oneFrameSize.y * viperSpellInfo.y) && (testValue.y < this->oneFrameSize.y * viperSpellInfo.y + this->oneFrameSize.y))
										{
											if (this->tempImage.getPixel((unsigned int)testValue.x, (unsigned int)testValue.y).a != 0)
											{
												_mob[j]->SetTimerStatus(TimerStatusMob);
												_mob[j]->SetStatus(Status::OnPoison);
											}
										}
									}
								}
							}
							break;

						case TypeAOE::ICE:
							if (!_mob[j]->GetInAir() && (_player[i]->GetAOEMagic().hitbox->getPosition().y + _player[i]->GetSpriteAOE().getTexture()->getSize().y / 8.f) <= _mob[j]->GetPos().y + DistAxeY && (_player[i]->GetAOEMagic().hitbox->getPosition().y + _player[i]->GetSpriteAOE().getTexture()->getSize().y / 8.f) >= _mob[j]->GetPos().y - DistAxeY)
							{
								if (_mob[j]->GetRace() != RaceMob::Boar)
								{
									_mob[j]->SetKnockdownState();
									_mob[j]->SetTimerKnockDown(0.75f);
								}

								_mob[j]->SetTimerStatus(0.75f);
								_mob[j]->SetStatus(Status::OnIce);
							}
							break;

						case TypeAOE::ELECTROCUTION:
							if (_player[i]->GetPos().y <= _mob[j]->GetPos().y + DistAxeY && _player[i]->GetPos().y >= _mob[j]->GetPos().y - DistAxeY)
							{
								if (_mob[j]->GetRace() != RaceMob::Boar)
								{
									_mob[j]->SetKnockdownState();
									_mob[j]->SetTimerKnockDown(0.75f);
								}

								_mob[j]->SetTimerStatus(0.75f);
								_mob[j]->SetStatus(Status::OnElec);
							}
							break;

						default:
							break;

							_mob[j]->SetDirection(_mob[i]->Trigo(_player[i]->GetPos()));

							if (_mob[j]->GetRace() == RaceMob::Boar)
							{
								_mob[j]->GetBoar()->AddNbAttack();
							}

							_mob[j]->SetDamageMagic(_player[i]->GetWisdom());

							//Death mob
							if (_mob[j]->GetLife() <= 0 && _mob[j]->GetState() != MobState::Death)
							{
								_player[i]->AddXp(_mob[j]->GetXp());
								_player[i]->AddGold(_mob[j]->GetGold());
								_mob[j]->SetState(MobState::Death);
								_mob[j]->SetInFallState();
							}
							_mob[j]->SetLastStatus(Status::None);
							_mob[j]->SetDirection(_mob[j]->Trigo(_player[i]->GetPos()));
						}
						_mob[i]->SetDamageMagic(_player[i]->GetWisdom());
					}
				}
			}
		}
	}
}

bool EntityManager::PlayerIsReady(Player* _player)
{
	if (_player->GetState() != HIT && _player->GetState() != KNOCKDOWN && _player->GetState() != DEAD && _player->GetState() != REVIVE && _player->GetState() != EAT_BY_BOSS && _player->GetState() != DASH)
	{
		return true;
	}
	return false;
}

void EntityManager::TpPlayerBlockedInRed(std::vector<Player*>& _player)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetTimerInRed() >= 1.f)
		{
			for (int j = 0; j < _player.size(); j++)
			{
				if (_player[j]->GetTimerInRed() == 0.f)
				{
					_player[i]->SetPos(_player[j]->GetPos());
					_player[i]->SetPosInAir(_player[j]->GetPosInAir());
				}
			}
		}
	}
}

void EntityManager::SetFreeFall(std::vector<Player*>& _player)
{
	if (_player[0]->GetState() == FREEFALL && !setPlayerFreeFall)
	{
		for (int i = 0; i < _player.size(); i++)
		{
			_player[i]->SetFreeFall(true);
			setPlayerFreeFall = true;
		}
	}
}

#pragma endregion

#pragma region Mob
void EntityManager::PlayerArrowCollision(std::vector<Projectile>& _projectile, std::vector<Player*>& _player, Audio& _audio)
{
	for (int k = 0; k < _projectile.size(); k++)
	{
		if (_projectile[k].autor == AutorProjectile::MOB)
		{
			for (int i = 0; i < _player.size(); i++)
			{
				if (PlayerIsReady(_player[i]))
				{
					if (Utils::AABB_AABB_Collision(_player[i]->GetHitBox(), _projectile[k].hitBox) &&
						_projectile[k].posAutor.y <= _player[i]->GetPos().y + DistAxeY && _projectile[k].posAutor.y >= _player[i]->GetPos().y - DistAxeY)
					{
						if (!_player[i]->GetInAir())
						{
							if (!_player[i]->GetParryState())
							{
								_player[i]->AddLife(_projectile[k].damage);
								_player[i]->SetPlayerState(HIT);
								if (_projectile[k].projetileType == TypeProjectile::DISTANCE_MAGIC_FIRE)
								{
									_player[i]->SetStatus(Status::OnFire);
								}
							}
							else if (_player[i]->GetParryState() && ((_projectile[k].pos.x > _player[i]->GetPos().x && _player[i]->GetLastDir() != RIGHT) || (_projectile[k].pos.x < _player[i]->GetPos().x && _player[i]->GetLastDir() != LEFT)))
							{
								_player[i]->AddLife(_projectile[k].damage);
								_player[i]->SetPlayerState(HIT);
								if (_projectile[k].projetileType == TypeProjectile::DISTANCE_MAGIC_FIRE)
								{
									_player[i]->SetStatus(Status::OnFire);
								}
							}
							else
							{
								_player[i]->SetShieldIsHit(true);
								_audio.PlaySoundNotRepeated("PLAYER_PARRY", 100.f);
							}
						}
						else
						{
							_player[i]->AddLife(_projectile[k].damage);
							(bool)(_projectile[k].pos.x < _player[i]->GetPos().x) ? _player[i]->SetLastDir(LEFT) : _player[i]->SetLastDir(RIGHT);
							_player[i]->SetPlayerState(KNOCKDOWN);
						}
						_projectile.erase(_projectile.begin() + k);
						i = (int)_player.size();
					}
				}
			}
		}
	}
}

void EntityManager::AttackOnPlayerCollision(std::vector<Mob*>& _mob, std::vector<Player*>& _player, float _dt, Audio& _audio)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (PlayerIsReady(_player[i]))
		{
			for (int j = 0; j < _mob.size(); j++)
			{
				if (_mob[j]->GetState() != MobState::Dispawn && TestHazel(_mob[j]))
				{
					if (Utils::AABB_AABB_Collision(_player[i]->GetHitBox(), _mob[j]->GetHitBoxAttack()) &&
						_mob[j]->GetPos().y >= _player[i]->GetPos().y - DistAxeY && _mob[j]->GetPos().y <= _player[i]->GetPos().y + DistAxeY)
					{
#pragma region Sound
						if (_mob[j]->GetRace() == RaceMob::HazelNut)
						{
							_mob[j]->GetHazel()->SetHitBoxAttackBool(true);
							if (rand() % 2 >= 1)
							{
								_audio.PlaySoundNotRepeated("HAZELNUT_ATTACK_" + std::to_string(rand() % 3 + 1), 100.f);
							}
						}
						else if (_mob[j]->GetRace() == RaceMob::Mushroom)
						{
							if (rand() % 2 >= 1)
							{
								_audio.PlaySoundNotRepeated("MUSHROOM_ATTACK1", 100.f);
							}
						}
						else if (_mob[j]->GetRace() == RaceMob::Pig)
						{
							if (rand() % 2 >= 1)
							{
								_audio.PlaySoundNotRepeated("PIG_ATTACK", 100.f);
							}
						}

#pragma endregion
						//don't parry
						if (!_player[i]->GetParryState())
						{
							_player[i]->AddLife(_mob[j]->GetDamage());
							_player[i]->SetPlayerState(HIT);

							if (!(rand() % 15) || _player[i]->GetInAir())
							{
								(bool)(_mob[j]->GetPos().x < _player[i]->GetPos().x) ? _player[i]->SetLastDir(LEFT) : _player[i]->SetLastDir(RIGHT);
								_player[i]->SetPlayerState(KNOCKDOWN);
							}


							if (_mob[j]->GetMagicUse() != MagicUse::None)
							{
								_player[i]->SetStatus((Status)_mob[j]->GetMagicUse());
							}
						}
						//parry but don't block attack
						else if (_player[i]->GetParryState() && ((_mob[j]->GetPos().x > _player[i]->GetPos().x && _player[i]->GetLastDir() != RIGHT) || (_mob[j]->GetPos().x < _player[i]->GetPos().x && _player[i]->GetLastDir() != LEFT)))
						{
							_player[i]->AddLife(_mob[j]->GetDamage());
							_player[i]->SetPlayerState(HIT);

							if (_mob[j]->GetMagicUse() != MagicUse::None)
							{
								_player[i]->SetStatus((Status)_mob[j]->GetMagicUse());
							}
						}
						else // block attack
						{
							_player[i]->SetShieldIsHit(true);
							if (_player[i]->GetParryTimer() > 0.5f)
							{
								_audio.PlaySoundNotRepeated("PLAYER_PARRY", 100.f);
								_player[i]->SetParryTimer(0.f);
							}
						}
					}
				}
			}
		}
	}
}

//Dispawn mob
void EntityManager::DeleteMob(std::vector<Mob*>& _mob, std::vector<Scarecrow*>& _scarecrow)
{
	for (int i = 0; i < _mob.size(); i++)
	{
		if (_mob[i]->GetState() == MobState::Dispawn)
		{
			_mob.erase(_mob.begin() + i);
			i--;

			if (!_mob.size())
			{
				this->waveLaunch = false;
			}
		}
	}

	for (int i = 0; i < _scarecrow.size(); i++)
	{
		if (_scarecrow[i]->GetCanDelete())
		{
			_scarecrow.erase(_scarecrow.begin() + i);
			i--;
		}
	}
}

//launch wave ia mob
void EntityManager::LaunchAIMob(std::vector<Mob*>& _mob, std::vector<Player*>& _player)
{
	if (!this->waveLaunch)
	{
		for (int i = 0; i < _player.size(); i++)
		{
			for (int j = 0; j < _mob.size(); j++)
			{
				if ((Utils::Distance(_mob[j]->GetPos(), _player[i]->GetPos()) <= 400.f || _mob[j]->GetState() != MobState::Wait) && !this->waveLaunch)
				{
					this->waveLaunch = true;
					j = 0;
				}

				if (this->waveLaunch)
				{
					_mob[j]->SetState(MobState::Alive);
					_mob[j]->SetTargetState();
				}
			}
		}
	}
}

bool EntityManager::TestWitch(Mob* _mob)
{
	if (_mob->GetRace() != RaceMob::Witch)
	{
		return true;
	}
	else
	{
		if (_mob->GetState() == MobState::Death || _mob->GetWitch()->GetState() == WitchState::Normal)
		{
			return false;
		}

		if (_mob->GetWitch()->GetHasSpawnCrystal())
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}

bool EntityManager::TestHazel(Mob* _mob)
{
	if (_mob->GetRace() == RaceMob::HazelNut)
	{
		if (_mob->GetHazel()->GetHitboxAttack())
		{
			return false;
		}
	}
	return true;
}

bool EntityManager::TestBoar(Mob* _mob, Player* _player)
{
	if (_mob->GetRace() == RaceMob::Boar)
	{
		if (_mob->GetBoar()->GetState() == BoarState::Shield)
		{
			//left 
			if (_mob->GetBoar()->OnLeft() && _player->GetPos().x < _mob->GetPos().x)
			{
				return false;
			}
			//right
			else if (!_mob->GetBoar()->OnLeft() && _player->GetPos().x > _mob->GetPos().x)
			{
				return false;
			}
		}
	}
	return true;
}

//physique engine mob collision 
void EntityManager::CollideBetweenMob(std::vector<Mob*>& _mob, int _wave)
{
	for (int i = 0; i < _mob.size(); i++)
	{
		if (_mob[i]->GetState() != MobState::Death && _mob[i]->GetState() != MobState::Dispawn && _mob[i]->GetRace() != RaceMob::Witch)
		{
			sf::Vector2f totalRepulsion = sf::Vector2f(0.f, 0.f);

			for (int j = 0; j < _mob.size(); j++)
			{
				if (i != j && _mob[j]->GetState() != MobState::Death && _mob[j]->GetState() != MobState::Dispawn)
				{
					sf::Vector2f vec = _mob[i]->GetPos() - _mob[j]->GetPos();
					float dist = Utils::GetVectorLength(vec);
					if (dist < DistCollideMob)
					{
						totalRepulsion += Utils::NormalizeVector(vec) * (DistCollideMob - dist);
					}
				}
			}
			_mob[i]->SetPos(_mob[i]->GetPos() + totalRepulsion);

			if (_mob[i]->GetPos().x < 0.f + 1920.f * _wave || _mob[i]->GetPos().x > 1920.f + 1920.f * _wave)
			{
				_mob[i]->SetPos(sf::Vector2f{ _mob[i]->GetPos().x - totalRepulsion.x, _mob[i]->GetPos().y });
			}

			if (_mob[i]->GetPos().y < 700.f || _mob[i]->GetPos().y > 1080.f)
			{
				_mob[i]->SetPos(sf::Vector2f{ _mob[i]->GetPos().x,_mob[i]->GetPos().y - totalRepulsion.y });
			}
		}
	}
}

//special capacity of hamurai
void EntityManager::BoarSlash(Mob& _mob, std::vector<Player*>& _tabPlayer)
{
	if (_mob.GetBoar()->GetSlashHorizonAlive())
	{
		sf::Vector2f slashPos = sf::Vector2f{ _mob.GetBoar()->GetPosSlashH() };
		slashPos.x = slashPos.x - 322;
		sf::Vector2f slashInfo = _mob.GetBoar()->GetSlashInfo();

		for (int i = 0; i < _tabPlayer.size(); i++)
		{
			if (PlayerIsReady(_tabPlayer[i]) && !_tabPlayer[i]->GetInAir())
			{
				sf::Vector2f playerPos = { _tabPlayer[i]->GetPos() };
				if (playerPos.y < slashPos.y - 30.f)
				{
					playerPos.y = slashPos.y + 5;
				}
				else if (playerPos.y > slashPos.y + this->dimSlashOneFrame.y && playerPos.y < slashPos.y + this->dimSlashOneFrame.y + 30.f)
				{
					playerPos.y = slashPos.y + this->dimSlashOneFrame.y - 5.f;
				}

				sf::Vector2f diffSlashPlayerPos = { playerPos - slashPos };
				sf::Vector2f testValue = { this->dimSlashOneFrame.x * slashInfo.x + diffSlashPlayerPos.x, diffSlashPlayerPos.y };

				if (diffSlashPlayerPos.x >= 0 && diffSlashPlayerPos.y >= 0)
				{
					if ((testValue.x > this->dimSlashOneFrame.x * slashInfo.x) && (testValue.x < this->dimSlashOneFrame.x * slashInfo.x + this->dimSlashOneFrame.x))
					{
						if ((testValue.y > 0) && (testValue.y < this->dimSlashOneFrame.y))
						{
							int testValueModuloX = testValue.x - this->dimSlashOneFrame.x * slashInfo.x;
							if (testValueModuloX < 161)
							{
								int dist = 161 - testValueModuloX;
								testValueModuloX += dist * 2;
							}
							else
							{
								int dist = testValueModuloX - 161;
								testValueModuloX -= dist * 2;
							}

							testValueModuloX = testValueModuloX + this->dimSlashOneFrame.x * slashInfo.x;

							if (this->imageSlash.getPixel((unsigned int)testValueModuloX, (unsigned int)testValue.y).a != 0)
							{
								_tabPlayer[i]->AddLife(6);
								(bool)(_tabPlayer[i]->GetInAir()) ? _tabPlayer[i]->SetPlayerState(KNOCKDOWN) : _tabPlayer[i]->SetPlayerState(HIT);
							}
						}
					}
				}
			}
		}
	}

	if (_mob.GetBoar()->GetSlashVertAlive())
	{
		for (int i = 0; i < _tabPlayer.size(); i++)
		{
			if (Utils::AABB_AABB_Collision(_tabPlayer[i]->GetHitBox(), _mob.GetBoar()->GetSlash()) && PlayerIsReady(_tabPlayer[i]) &&
				_tabPlayer[i]->GetPos().y <= _mob.GetBoar()->GetSlash()->getPosition().y + DistAxeY && _tabPlayer[i]->GetPos().y >= _mob.GetBoar()->GetSlash()->getPosition().y - DistAxeY)
			{
				_tabPlayer[i]->AddLife(8);
				(bool)(_tabPlayer[i]->GetInAir()) ? _tabPlayer[i]->SetPlayerState(KNOCKDOWN) : _tabPlayer[i]->SetPlayerState(HIT);
			}
		}
	}

	if (_mob.GetBoar()->GetDoZenitsu())
	{
		for (int i = 0; i < _tabPlayer.size(); i++)
		{
			if (_tabPlayer[i]->GetPos().x <= _mob.GetPos().x && _tabPlayer[i]->GetPos().x >= _mob.GetPos().x - 200.f && PlayerIsReady(_tabPlayer[i]) &&
				_tabPlayer[i]->GetPos().y <= _mob.GetPos().y + DistAxeY && _tabPlayer[i]->GetPos().y >= _mob.GetPos().y - DistAxeY)
			{
				_tabPlayer[i]->AddLife(10);
				(bool)(_tabPlayer[i]->GetInAir()) ? _tabPlayer[i]->SetPlayerState(KNOCKDOWN) : _tabPlayer[i]->SetPlayerState(HIT);
			}
		}
	}
}

//draw life bar on the top of mob
void EntityManager::DrawLifeBar(Mob* _mob, sf::Sprite& _lifeBar, sf::Sprite& _pixelBar, bool _draw, sf::RenderWindow& _window)
{
	if (_draw)
	{
		if (_mob->GetState() != MobState::Death && _mob->GetState() != MobState::Dispawn && _mob->GetType() != TypeMob::Boss)
		{
			sf::Vector2f posMob;
			(bool)(_mob->GetInAir()) ? posMob = _mob->GetPosInAir() : posMob = _mob->GetPos();

			float heightSprite = _mob->GetDimSprite().y;
			int mobMaxLife = _mob->GetMaxLife();
			int lifeMob = _mob->GetLife();
			if (lifeMob <= 0)
			{
				lifeMob = 0;
			}
			float ratio = (float)(lifeMob * DimLifeBar / mobMaxLife);

			sf::Vector2f posLifeBar = sf::Vector2f{ posMob.x,posMob.y - heightSprite - 10.f };
			sf::Vector2f posRedpixel = sf::Vector2f{ posLifeBar.x - 50 ,posLifeBar.y - _pixelBar.getTexture()->getSize().y / 2 };

			_pixelBar.setScale(sf::Vector2f{ ratio,1.f });

			Utils::BlitSprite(_lifeBar, posLifeBar, 0, _window);
			Utils::BlitSprite(_pixelBar, posRedpixel, 0, _window);
		}
	}
}

void EntityManager::DrawLifeBar(Mob* _mob, sf::Sprite& _lifeBar, sf::Sprite& _pixelBar, bool _draw, sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	if (_draw)
	{
		if (_mob->GetState() != MobState::Death && _mob->GetState() != MobState::Dispawn && _mob->GetType() != TypeMob::Boss)
		{
			sf::Vector2f posMob;
			(bool)(_mob->GetInAir()) ? posMob = _mob->GetPosInAir() : posMob = _mob->GetPos();

			float heightSprite = _mob->GetDimSprite().y;
			int mobMaxLife = _mob->GetMaxLife();
			int lifeMob = _mob->GetLife();
			if (lifeMob <= 0)
			{
				lifeMob = 0;
			}
			float ratio = (float)(lifeMob * DimLifeBar / mobMaxLife);

			sf::Vector2f posLifeBar = sf::Vector2f{ posMob.x,posMob.y - heightSprite - 10.f };
			sf::Vector2f posRedpixel = sf::Vector2f{ posLifeBar.x - 50 ,posLifeBar.y - _pixelBar.getTexture()->getSize().y / 2 };

			_pixelBar.setScale(sf::Vector2f{ ratio,1.f });

			Utils::BlitSprite(_lifeBar, { posLifeBar.x + (TROAD_GAP * (-1 * posLifeBar.y + WORKING_HEIGHT) / 100.f), posLifeBar.y }, 0, _red);
			Utils::BlitSprite(_lifeBar, { posLifeBar.x - (TROAD_GAP * (-1 * posLifeBar.y + WORKING_HEIGHT) / 100.f), posLifeBar.y }, 0, _blue);

			Utils::BlitSprite(_pixelBar, { posRedpixel.x + (TROAD_GAP * (-1 * posRedpixel.y + WORKING_HEIGHT) / 100.f), posRedpixel.y }, 0, _red);
			Utils::BlitSprite(_pixelBar, { posRedpixel.x - (TROAD_GAP * (-1 * posRedpixel.y + WORKING_HEIGHT) / 100.f), posRedpixel.y }, 0, _blue);
		}
	}
}

//collision HitBox witch
void EntityManager::UpdateWitchHitBox(Witch* _witch, std::vector<Player*>& _player, std::vector<Projectile>& _projectile, Audio& _audio)
{
	//crystal invicible phase
	if (_witch->GetHasSpawnCrystal())
	{
		for (int i = 0; i < _player.size(); i++)
		{
			for (int j = 0; j < NbCrystal; j++)
			{
				if (_witch->GetCrystal()[j].life > 0)
				{
					if (_player[i]->GetHeavyAttackActived() && Utils::AABB_AABB_Collision(_witch->GetCrystal()[j].hitBox, _player[i]->GetHeavyAttack()) &&
						_player[i]->GetPos().y <= _witch->GetCrystal()[j].posOnGround.y + DistAxeY && _player[i]->GetPos().y >= _witch->GetCrystal()[j].posOnGround.y - DistAxeY)
					{
						_witch->GetCrystal()[j].life -= (int)(_player[i]->GetStrengthDamage() + 5.f);
						_witch->GetCrystal()[j].hit = true;
						_player[i]->SetCanActiveHitbox(false);
						_audio.PlaySoundNotRepeated("WITCH_CRISTAL_SOUND_1");
					}
					else if (_player[i]->GetLightAttackActived() && Utils::AABB_AABB_Collision(_witch->GetCrystal()[j].hitBox, _player[i]->GetLightAttack()) &&
						_player[i]->GetPos().y <= _witch->GetCrystal()[j].posOnGround.y + DistAxeY && _player[i]->GetPos().y >= _witch->GetCrystal()[j].posOnGround.y - DistAxeY)
					{
						_witch->GetCrystal()[j].life -= (int)(_player[i]->GetComboNumber() == 3 ? (_player[i]->GetStrengthDamage() + 5.f) : _player[i]->GetStrengthDamage());
						_witch->GetCrystal()[j].hit = true;
						_player[i]->SetCanActiveHitbox(false);
						_audio.PlaySoundNotRepeated("WITCH_CRISTAL_SOUND_1");
					}
					else
					{
						for (int k = 0; k < _projectile.size(); k++)
						{
							if (Utils::AABB_AABB_Collision(_witch->GetCrystal()[j].hitBox, _projectile[k].hitBox) &&
								_player[i]->GetPos().y <= _witch->GetCrystal()[j].posOnGround.y + DistAxeY && _player[i]->GetPos().y >= _witch->GetCrystal()[j].posOnGround.y - DistAxeY)
							{
								_witch->GetCrystal()[j].life--;
								_witch->GetCrystal()[j].hit = true;
								_audio.PlaySoundNotRepeated("WITCH_CRISTAL_SOUND_1");
							}
						}
					}
				}
			}
		}
	}

	//attack with bramble
	if (_witch->GetHasBramble())
	{
		for (int i = 0; i < _player.size(); i++)
		{
			if (PlayerIsReady(_player[i]) && _player[i]->GetStatus() != Status::OnPoison)
			{
				if (Utils::AABB_AABB_Collision(_witch->GetHitBoxBramble(), _player[i]->GetHitBox()) &&
					_player[i]->GetPos().y <= _witch->GetHitBoxBramble()->getPosition().y + DistAxeY && _player[i]->GetPos().y >= _witch->GetHitBoxBramble()->getPosition().y - DistAxeY)
				{
					_player[i]->AddLife(2);
					(bool)(_player[i]->GetInAir()) ? _player[i]->SetPlayerState(KNOCKDOWN) : _player[i]->SetPlayerState(HIT);
					_player[i]->SetStatus(Status::OnPoison);
				}
			}
		}
	}

	//when witch scream
	if (_witch->GetHasScream() && _witch->GetPlayerBecomeFuzzy())
	{
		for (int i = 0; i < _player.size(); i++)
		{
			_player[i]->SetStatus(Status::OnFuzzy);
		}
		_witch->SetPlayerBecomeFuzzy(false);
	}

	//when plant on ground
	if (_witch->GetHasSpawnPlant())
	{
		for (int i = 0; i < _witch->GetVectorPlant().size(); i++)
		{
			//if player was too close
			if (_witch->GetVectorPlant()[i]->isReady && !_witch->GetVectorPlant()[i]->eatSomeone && !_witch->GetVectorPlant()[i]->toLong && !_witch->GetVectorPlant()[i]->goOut)
			{
				for (int j = 0; j < _player.size(); j++)
				{
					if (_player[j]->GetLife() > 0)
					{
						sf::Vector2f vec = { _player[j]->GetPos() - _witch->GetVectorPlant()[i]->pos };
						float lenght = Utils::GetVectorLength(vec);

						if (lenght < 100.f && _player[j]->GetPos().y <= _witch->GetVectorPlant()[i]->pos.y + DistAxeY &&
							_player[j]->GetPos().y >= _witch->GetVectorPlant()[i]->pos.y - DistAxeY)
						{
							_witch->GetVectorPlant()[i]->goOut = true;
						}
					}
				}
			}
			//to eat the player
			else if (_witch->GetVectorPlant()[i]->isReady && _witch->GetVectorPlant()[i]->goOut && !_witch->GetVectorPlant()[i]->eatSomeone)
			{
				for (int j = 0; j < _player.size(); j++)
				{
					if (_player[j]->GetLife() > 0)
					{
						if (Utils::AABB_AABB_Collision(_player[j]->GetHitBox(), _witch->GetVectorPlant()[i]->hitBox))
						{
							_player[j]->SetPlayerState(EAT_BY_BOSS);
							_player[j]->SetPos(_witch->GetVectorPlant()[i]->pos);
							_witch->GetVectorPlant()[i]->eatSomeone = true;
							_witch->GetVectorPlant()[i]->playerEat = j;
							_witch->GetVectorPlant()[i]->timerOnGround = 0;
							_audio.PlaySoundNotRepeated("WITCH_PLANT_EAT_SOUND_1", 100.f);
						}
					}
				}
			}
			//to eject player whitch was eat
			else if (_witch->GetVectorPlant()[i]->isReady && _witch->GetVectorPlant()[i]->eatSomeone && _witch->GetVectorPlant()[i]->spitOut && _player[_witch->GetVectorPlant()[i]->playerEat]->GetState() != KNOCKDOWN)
			{
				_player[_witch->GetVectorPlant()[i]->playerEat]->SetPropultionKnockDown(true);
				_player[_witch->GetVectorPlant()[i]->playerEat]->SetPlayerState(KNOCKDOWN);
			}
		}
	}
}

//draw boss bar
void EntityManager::DrawBossBar(std::string _race, HudBarBoss& _spriteBoss, Mob& _mob, Wave& _wave, sf::RenderWindow& _window, float _decalage, sf::Font _font, std::string _string)
{
	if (_mob.GetState() != MobState::Death)
	{
		if (!this->bossBarInit)
		{
			_spriteBoss.headBoss = Utils::LoadSprite("Assets/Images/HUD/BossHP/" + _race + ".png", CENTER);
			_spriteBoss.headBossShadow = Utils::LoadSprite("Assets/Images/HUD/BossHP/" + _race + "Shadow.png", CENTER);
			this->bossBarInit = true;
			this->textTaMere.setFont(_font);
			this->textTaMere.setString(_string);
			this->textTaMere.setCharacterSize(50);
			this->textTaMere.setPosition(sf::Vector2f{ (float)(WORKING_WIDTH / 2),1020.f });
			this->textTaMere.setOrigin(this->textTaMere.getGlobalBounds().width / 2, this->textTaMere.getGlobalBounds().height / 2);
		}

		sf::Vector2f posMob;
		(bool)(_mob.GetInAir()) ? posMob = _mob.GetPosInAir() : posMob = _mob.GetPos();

		float heightSprite = _mob.GetDimSprite().y;
		int mobMaxLife = _mob.GetMaxLife();
		int lifeMob = _mob.GetLife();

		if (lifeMob <= 0)
		{
			lifeMob = 0;
		}

		float ratio = (float)(lifeMob * DimLifeBarBoss / mobMaxLife);

		sf::Vector2f posLifeBar = sf::Vector2f{ (float)(WORKING_WIDTH / 2),1020.f };
		float posLifeBarMax = (float)(posLifeBar.x + (_spriteBoss.lifeBar.getTexture()->getSize().x / 2));
		sf::Vector2f posRedpixel = sf::Vector2f{ posLifeBar.x - 640 ,posLifeBar.y - _spriteBoss.lifeBar.getTexture()->getSize().y / 2 + 5.f };

		//blit Life bar
		Utils::BlitSprite(_spriteBoss.lifeBar, posLifeBar, 0, _window);

		//blit pixel life
		_spriteBoss.pixelLifeRed.setScale(sf::Vector2f{ ratio,1.f });
		Utils::BlitSprite(_spriteBoss.pixelLifeRed, posRedpixel, 0, _window);

		//blit name of the boss
		_window.draw(this->textTaMere);

		//blit cursor change phase
		for (int i = 1; i < _mob.GetNbPhase(); i++)
		{
			float cursor = (float)((_spriteBoss.lifeBar.getTexture()->getSize().x / _mob.GetNbPhase()) * i);
			Utils::BlitSprite(_spriteBoss.changePhase, sf::Vector2f{ posLifeBarMax - cursor , 985.f }, 0, _window);
		}

		//blit boss head
		Utils::BlitSprite(_spriteBoss.headBossShadow, sf::Vector2f{ posLifeBar.x - _spriteBoss.lifeBar.getTexture()->getSize().x / 2 - 65.f + _decalage,posLifeBar.y - 65.f }, 0, _window);//left
		Utils::BlitSprite(_spriteBoss.headBoss, sf::Vector2f{ posLifeBar.x - _spriteBoss.lifeBar.getTexture()->getSize().x / 2 - 60.f + _decalage,posLifeBar.y - 60.f }, 0, _window);//left
	}
	else
	{
		this->bossBarInit = false;
		this->waveBossBar = NULL;
	}
}

void EntityManager::DrawBossBar(std::string _race, HudBarBoss& _spriteBoss, Mob& _mob, Wave& _wave, sf::RenderTexture& _red, sf::RenderTexture& _blue, float _decalage, sf::Font _font, std::string _string)
{
	if (_mob.GetState() != MobState::Death)
	{
		if (!this->bossBarInit)
		{
			_spriteBoss.headBoss = Utils::LoadSprite("Assets/Images/HUD/BossHP/" + _race + ".png", CENTER);
			_spriteBoss.headBossShadow = Utils::LoadSprite("Assets/Images/HUD/BossHP/" + _race + "Shadow.png", CENTER);
			this->bossBarInit = true;
			this->waveBossBar = (int)(_wave);
			this->textTaMere.setFont(_font);
			this->textTaMere.setString(_string);
			this->textTaMere.setCharacterSize(50);
			this->textTaMere.setPosition(sf::Vector2f{ (float)(WORKING_WIDTH / 2),1020.f });
			this->textTaMere.setOrigin(this->textTaMere.getGlobalBounds().width / 2, this->textTaMere.getGlobalBounds().height / 2);
		}

		sf::Vector2f posMob;
		(bool)(_mob.GetInAir()) ? posMob = _mob.GetPosInAir() : posMob = _mob.GetPos();

		float heightSprite = _mob.GetDimSprite().y;
		int mobMaxLife = _mob.GetMaxLife();
		int lifeMob = _mob.GetLife();
		float ratio = (float)(lifeMob * DimLifeBarBoss / mobMaxLife);

		if (lifeMob <= 0)
		{
			lifeMob = 0;
		}

		sf::Vector2f posLifeBar = sf::Vector2f{ (float)(WORKING_WIDTH / 2),1020.f };
		float posLifeBarMax = (float)(posLifeBar.x + (_spriteBoss.lifeBar.getTexture()->getSize().x / 2));
		sf::Vector2f posRedpixel = sf::Vector2f{ posLifeBar.x - 640 ,posLifeBar.y - _spriteBoss.lifeBar.getTexture()->getSize().y / 2 + 5.f };

		//blit Life bar
		Utils::BlitSprite(_spriteBoss.lifeBar, sf::Vector2f{ posLifeBar.x + (TROAD_GAP * (-1 * posLifeBar.y + WORKING_HEIGHT) / 100.f),posLifeBar.y }, 0, _red);
		Utils::BlitSprite(_spriteBoss.lifeBar, sf::Vector2f{ posLifeBar.x - (TROAD_GAP * (-1 * posLifeBar.y + WORKING_HEIGHT) / 100.f),posLifeBar.y }, 0, _blue);

		//blit pixel life
		_spriteBoss.pixelLifeRed.setScale(sf::Vector2f{ ratio,1.f });
		Utils::BlitSprite(_spriteBoss.pixelLifeRed, sf::Vector2f{ posRedpixel.x + (TROAD_GAP * (-1 * posRedpixel.y + WORKING_HEIGHT) / 100.f),posRedpixel.y }, 0, _red);
		Utils::BlitSprite(_spriteBoss.pixelLifeRed, sf::Vector2f{ posRedpixel.x - (TROAD_GAP * (-1 * posRedpixel.y + WORKING_HEIGHT) / 100.f),posRedpixel.y }, 0, _blue);

		//blit name of the boss
		sf::Vector2f posTexte = this->textTaMere.getPosition();

		this->textTaMere.setPosition(sf::Vector2f{ posTexte.x + (TROAD_GAP * (-1 * posTexte.y + WORKING_HEIGHT) / 100.f),posTexte.y });
		_red.draw(this->textTaMere);

		this->textTaMere.setPosition(sf::Vector2f{ posTexte.x - (TROAD_GAP * (-1 * posTexte.y + WORKING_HEIGHT) / 100.f),posTexte.y });
		_blue.draw(this->textTaMere);

		this->textTaMere.setPosition(posTexte);

		//blit cursor change phase
		for (int i = 1; i < _mob.GetNbPhase(); i++)
		{
			float cursor = (float)((_spriteBoss.lifeBar.getTexture()->getSize().x / _mob.GetNbPhase()) * i);
			Utils::BlitSprite(_spriteBoss.changePhase, sf::Vector2f{ (posLifeBarMax - cursor) + (TROAD_GAP * (-1 * 985.f + WORKING_HEIGHT) / 100.f), 985.f }, 0, _red);
			Utils::BlitSprite(_spriteBoss.changePhase, sf::Vector2f{ (posLifeBarMax - cursor) - (TROAD_GAP * (-1 * 985.f + WORKING_HEIGHT) / 100.f), 985.f }, 0, _blue);
		}

		//blit boss head
		Utils::BlitSprite(_spriteBoss.headBossShadow, sf::Vector2f{ (posLifeBar.x - _spriteBoss.lifeBar.getTexture()->getSize().x / 2 - 65.f + _decalage) + (TROAD_GAP * (-1 * (posLifeBar.y - 65.f) + WORKING_HEIGHT) / 100.f), (posLifeBar.y - 65.f) }, 0, _red);//left
		Utils::BlitSprite(_spriteBoss.headBossShadow, sf::Vector2f{ (posLifeBar.x - _spriteBoss.lifeBar.getTexture()->getSize().x / 2 - 65.f + _decalage) - (TROAD_GAP * (-1 * (posLifeBar.y - 65.f) + WORKING_HEIGHT) / 100.f), (posLifeBar.y - 65.f) }, 0, _blue);//left

		Utils::BlitSprite(_spriteBoss.headBoss, sf::Vector2f{ (posLifeBar.x - _spriteBoss.lifeBar.getTexture()->getSize().x / 2 - 60.f + _decalage) + (TROAD_GAP * (-1 * (posLifeBar.y - 60.f) + WORKING_HEIGHT) / 100.f),(posLifeBar.y - 60.f) }, 0, _red);//left
		Utils::BlitSprite(_spriteBoss.headBoss, sf::Vector2f{ (posLifeBar.x - _spriteBoss.lifeBar.getTexture()->getSize().x / 2 - 60.f + _decalage) - (TROAD_GAP * (-1 * (posLifeBar.y - 60.f) + WORKING_HEIGHT) / 100.f),(posLifeBar.y - 60.f) }, 0, _blue);//left
	}
	else
	{
		this->bossBarInit = false;
		this->waveBossBar = NULL;
	}
}
#pragma endregion

#pragma region Vector
//to sort the vector entity
bool SortFunct(Sort* _y1, Sort* _y2)
{
	return _y1->posY < _y2->posY;
}

void EntityManager::UpdateDamageVector(float _dt)
{
	for (int i = 0; i < damageHud.size(); i++)
	{
		if (damageHud[i]->timerEasing < 0.4f)
		{
			damageHud[i]->pos = Easings::Lerp(damageHud[i]->startPosEasing, damageHud[i]->endPosEasing, Easings::OutExpo(damageHud[i]->timerEasing));
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

void EntityManager::UpdateGoldVector(float _dt)
{
	for (int i = 0; i < goldHud.size(); i++)
	{
		if (goldHud[i]->timerEasing < 1)
		{
			goldHud[i]->pos = Easings::Lerp(goldHud[i]->startPosEasing, goldHud[i]->endPosEasing, Easings::OutExpo(goldHud[i]->timerEasing));
			goldHud[i]->timerEasing += _dt / 2;
			goldHud[i]->gold.setPosition(goldHud[i]->pos);
			goldHud[i]->goldBack.setPosition(goldHud[i]->pos.x - 2, goldHud[i]->pos.y + 2);

			goldHud[i]->goldSprite.setPosition({ goldHud[i]->pos.x - 35, goldHud[i]->pos.y });
		}
		else
		{
			goldHud.erase(goldHud.begin() + i);
		}
	}
}

//text show when damaged
void EntityManager::DisplayDamageVector(sf::RenderWindow& _window)
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

void EntityManager::DisplayDamageVector(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	for (int i = 0; i < damageHud.size(); i++)
	{
		sf::Vector2f pos1 = damageHud[i]->damageBack.getPosition();
		sf::Vector2f pos2 = damageHud[i]->damage.getPosition();

		damageHud[i]->damageBack.setPosition(pos1.x + (TROAD_GAP * (-1 * pos1.y + WORKING_HEIGHT) / 100.f), pos1.y);
		_red.draw(damageHud[i]->damageBack);

		damageHud[i]->damageBack.setPosition(pos1.x - (TROAD_GAP * (-1 * pos1.y + WORKING_HEIGHT) / 100.f), pos1.y);
		_blue.draw(damageHud[i]->damageBack);

		damageHud[i]->damageBack.setPosition(pos1);

		damageHud[i]->damage.setPosition(pos2.x + (TROAD_GAP * (-1 * pos2.y + WORKING_HEIGHT) / 100.f), pos2.y);
		_red.draw(damageHud[i]->damage);

		damageHud[i]->damage.setPosition(pos2.x - (TROAD_GAP * (-1 * pos2.y + WORKING_HEIGHT) / 100.f), pos2.y);
		_blue.draw(damageHud[i]->damage);

		damageHud[i]->damage.setPosition(pos2);

		if (damageHud[i]->displayHeart)
		{
			sf::Vector2f pos3 = damageHud[i]->heartSprite.getPosition();

			damageHud[i]->heartSprite.setPosition(pos3.x + (TROAD_GAP * (-1 * pos3.y + WORKING_HEIGHT) / 100.f), pos3.y);
			_red.draw(damageHud[i]->heartSprite);

			damageHud[i]->heartSprite.setPosition(pos3.x - (TROAD_GAP * (-1 * pos3.y + WORKING_HEIGHT) / 100.f), pos3.y);
			_blue.draw(damageHud[i]->heartSprite);

			damageHud[i]->heartSprite.setPosition(pos3);
		}
	}
}

//text show when gold gain
void EntityManager::DisplayGoldVector(sf::RenderWindow& _window)
{
	for (int i = 0; i < goldHud.size(); i++)
	{
		_window.draw(goldHud[i]->goldBack);
		_window.draw(goldHud[i]->gold);
		goldHud[i]->goldSprite.setScale(0.3f, 0.3f);
		_window.draw(goldHud[i]->goldSprite);
	}
}

void EntityManager::DisplayGoldVector(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	for (int i = 0; i < goldHud.size(); i++)
	{
		sf::Vector2f pos1 = goldHud[i]->goldBack.getPosition();
		sf::Vector2f pos2 = goldHud[i]->gold.getPosition();

		goldHud[i]->goldBack.setPosition(pos1.x + (TROAD_GAP * (-1 * pos1.y + WORKING_HEIGHT) / 100.f), pos1.y);
		_red.draw(goldHud[i]->goldBack);

		goldHud[i]->goldBack.setPosition(pos1.x - (TROAD_GAP * (-1 * pos1.y + WORKING_HEIGHT) / 100.f), pos1.y);
		_blue.draw(goldHud[i]->goldBack);

		goldHud[i]->goldBack.setPosition(pos1);

		goldHud[i]->gold.setPosition(pos2.x + (TROAD_GAP * (-1 * pos2.y + WORKING_HEIGHT) / 100.f), pos2.y);
		_red.draw(goldHud[i]->gold);

		goldHud[i]->gold.setPosition(pos2.x - (TROAD_GAP * (-1 * pos2.y + WORKING_HEIGHT) / 100.f), pos2.y);
		_blue.draw(goldHud[i]->gold);

		goldHud[i]->goldBack.setPosition(pos2);

		goldHud[i]->goldSprite.setScale(0.3f, 0.3f);
		sf::Vector2f pos3 = goldHud[i]->goldSprite.getPosition();

		goldHud[i]->goldSprite.setPosition(pos3.x + (TROAD_GAP * (-1 * pos3.y + WORKING_HEIGHT) / 100.f), pos3.y);
		_red.draw(goldHud[i]->goldSprite);

		goldHud[i]->goldSprite.setPosition(pos3.x - (TROAD_GAP * (-1 * pos3.y + WORKING_HEIGHT) / 100.f), pos3.y);
		_blue.draw(goldHud[i]->goldSprite);
	}
}

//creat text for HUD damage
void EntityManager::FillVectorDamage(std::vector<Player*>& _player, std::vector<Mob*>& _mob, std::vector<Scarecrow*>& _scarecrow, sf::Font& _font, sf::Sprite& _spriteHeart)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetLastLife() != _player[i]->GetLife() && _player[i]->GetLife() < _player[i]->GetLastLife())
		{
			HudDamage* temp = new HudDamage();
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

	for (int i = 0; i < _mob.size(); i++)
	{
		if (_mob[i]->GetLastLife() != _mob[i]->GetLife() && _mob[i]->GetLife() < _mob[i]->GetLastLife())
		{
			HudDamage* temp = new HudDamage;

			Utils::CreateText(temp->damage, _font, "-" + std::to_string(_mob[i]->GetLastLife() - _mob[i]->GetLife()), 20, sf::Color::Black, true);
			Utils::CreateText(temp->damageBack, _font, "-" + std::to_string(_mob[i]->GetLastLife() - _mob[i]->GetLife()), 20, sf::Color::White, true);
			_mob[i]->SetLastLife();

			if (_mob[i]->GetInAir())
			{
				temp->pos = { _mob[i]->GetPosInAir().x + (rand() % 50 - 25), _mob[i]->GetPosInAir().y - 100 };
			}
			else
			{
				temp->pos = { _mob[i]->GetPos().x + (rand() % 50 - 25), _mob[i]->GetPos().y - 100 };
			}
			temp->damage.setPosition(temp->pos);
			temp->damageBack.setPosition(temp->pos.x - 2, temp->pos.y + 2);
			temp->heartSprite.setPosition({ temp->damage.getPosition().x + 32, temp->damage.getPosition().y });
			temp->startPosEasing = temp->pos;
			temp->endPosEasing = { temp->pos.x,temp->pos.y - 100 };
			temp->timerEasing = 0.f;


			switch (_mob[i]->GetStatus())
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
		else if (_mob[i]->GetLastLife() != _mob[i]->GetLife() && _mob[i]->GetLife() > _mob[i]->GetLastLife())
		{
			_mob[i]->SetLastLife();
		}
	}

	for (int i = 0; i < _scarecrow.size(); i++)
	{
		if (_scarecrow[i]->GetLastLife() != _scarecrow[i]->GetLife() && _scarecrow[i]->GetLife() < _scarecrow[i]->GetLastLife())
		{
			HudDamage* temp = new HudDamage;

			Utils::CreateText(temp->damage, _font, "-" + std::to_string(_scarecrow[i]->GetLastLife() - _scarecrow[i]->GetLife()), 20, sf::Color::Black, true);
			Utils::CreateText(temp->damageBack, _font, "-" + std::to_string(_scarecrow[i]->GetLastLife() - _scarecrow[i]->GetLife()), 20, sf::Color::White, true);
			_scarecrow[i]->SetLastLife();

			temp->pos = { _scarecrow[i]->GetPos().x, _scarecrow[i]->GetPos().y - 200 };
			temp->damage.setPosition(temp->pos);
			temp->damageBack.setPosition(temp->pos.x - 2, temp->pos.y + 2);
			temp->heartSprite.setPosition({ temp->damage.getPosition().x + 32, temp->damage.getPosition().y });
			temp->startPosEasing = temp->pos;
			temp->endPosEasing = { temp->pos.x,temp->pos.y - 100 };
			temp->timerEasing = 0.f;


			switch (_scarecrow[i]->GetStatus())
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
		else if (_scarecrow[i]->GetLastLife() != _scarecrow[i]->GetLife() && _scarecrow[i]->GetLife() > _scarecrow[i]->GetLastLife())
		{
			_scarecrow[i]->SetLastLife();
		}
	}
}

void EntityManager::FillVectorGold(std::vector<Player*>& _player, sf::Font& _font)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetLastGold() != _player[i]->GetGold() && _player[i]->GetGold() > _player[i]->GetLastGold())
		{
			HudGold* temp = new HudGold();
			Utils::CreateText(temp->gold, _font, "+" + std::to_string(_player[i]->GetGold() - _player[i]->GetLastGold()), 20, sf::Color::Black, true);
			Utils::CreateText(temp->goldBack, _font, "+" + std::to_string(_player[i]->GetGold() - _player[i]->GetLastGold()), 20, sf::Color::White, true);
			_player[i]->SetLastLife();
			temp->goldSprite = Utils::LoadSprite("Assets/Images/Shop/Gold.png", CENTER);
			if (_player[i]->GetInAir())
			{
				temp->pos = { _player[i]->GetPosInAir().x + (rand() % 150 - 75), _player[i]->GetPosInAir().y - 180 };
			}
			else
			{
				temp->pos = { _player[i]->GetPos().x + (rand() % 150 - 75), _player[i]->GetPos().y - 180 };
			}
			temp->gold.setPosition(temp->pos);
			temp->goldBack.setPosition(temp->pos.x - 2, temp->pos.y + 2);
			temp->goldSprite.setPosition({ temp->gold.getPosition().x - 50, temp->gold.getPosition().y });
			temp->startPosEasing = temp->pos;
			temp->endPosEasing = { temp->pos.x,temp->pos.y - 100 };
			temp->timerEasing = 0.f;

			goldHud.push_back(temp);
			_player[i]->SetLastGold();
		}
	}
}

//sort entity to display
void EntityManager::FillVectorSort(GameData& _gameData, std::vector<Projectile>& _projectile, std::vector<Player*>& _player, std::vector<Mob*>& _mob, std::vector<Building*>& _building, std::vector<Drop*>& _food, std::vector<Sort*>& _sort, std::vector<Scarecrow*>& _scarecrow, std::vector<Building*>& _sleepingNut)
{
	_sort.erase(_sort.begin(), _sort.end());

	for (int i = 0; i < _projectile.size(); i++)
	{
		Sort* sort = new Sort{ _projectile[i].posAutor.y + 10.f,i,SortVector::Projectile };
		_sort.push_back(sort);
	}

	for (int i = 0; i < _player.size(); i++)
	{
		Sort* sort = new Sort{ _player[i]->GetPos().y + 5.f,i,SortVector::Player };
		_sort.push_back(sort);

		if (_player[i]->GetIceSpellHere() && !_gameData.multiplayer)
		{
			Sort* sort = new Sort{ (_player[i]->GetAOEMagic().hitbox->getPosition().y + _player[i]->GetSpriteAOE().getTexture()->getSize().y / 8.f) - 10.f,i,SortVector::Ice };
			_sort.push_back(sort);
		}
	}

	for (int i = 0; i < _mob.size(); i++)
	{
		if (_mob[i]->GetState() != MobState::Dispawn)
		{
			Sort* sort = new Sort{ _mob[i]->GetPos().y ,i,SortVector::Mob };
			_sort.push_back(sort);

			if (_mob[i]->GetRace() == RaceMob::Witch)
			{
				if (_mob[i]->GetWitch()->GetHasSpawnCrystal())
				{
					for (int j = 0; j < NbCrystal; j++)
					{
						if (_mob[i]->GetWitch()->GetCrystal()[j].life > 0)
						{
							Sort* sort = new Sort{ _mob[i]->GetWitch()->GetCrystal()[j].posOnGround.y ,i,SortVector::Crystal,j };
							_sort.push_back(sort);
						}
					}
				}

				if (_mob[i]->GetWitch()->GetHasSpawnPlant())
				{
					for (int j = 0; j < _mob[i]->GetWitch()->GetVectorPlant().size(); j++)
					{
						Sort* sort = new Sort{ _mob[i]->GetWitch()->GetVectorPlant()[j]->pos.y ,i,SortVector::Plante ,j };
						_sort.push_back(sort);
					}
				}

				if (_mob[i]->GetWitch()->GetHasBramble())
				{
					Sort* sort = new Sort{ _mob[i]->GetWitch()->GetHitBoxBramble()->getPosition().y ,i,SortVector::Bramble };
					_sort.push_back(sort);
				}
			}
			else if (_mob[i]->GetRace() == RaceMob::Boar)
			{
				if (_mob[i]->GetBoar()->GetSlashHorizonAlive())
				{
					Sort* sort = new Sort{ _mob[i]->GetBoar()->GetSlash()->getPosition().y + _mob[i]->GetBoar()->GetSlash()->getSize().y ,i,SortVector::SlashH };
					_sort.push_back(sort);
				}
				else if (_mob[i]->GetBoar()->GetSlashVertAlive())
				{
					Sort* sort = new Sort{ _mob[i]->GetBoar()->GetSlash()->getPosition().y ,i,SortVector::SlashV };
					_sort.push_back(sort);
				}
			}
		}
	}

	for (int i = 0; i < _building.size(); i++)
	{
		Sort* sort = new Sort{ _building[i]->posY,i,SortVector::Building };
		_sort.push_back(sort);
	}

	for (int i = 0; i < _sleepingNut.size(); i++)
	{
		Sort* sort = new Sort{ _sleepingNut[i]->posY,i,SortVector::SleepingNut };
		_sort.push_back(sort);
	}

	for (int i = 0; i < _food.size(); i++)
	{
		Sort* sort = new Sort{ _food[i]->pos.y,i,SortVector::Food };
		_sort.push_back(sort);
	}

	for (int i = 0; i < _scarecrow.size(); i++)
	{
		Sort* sort = new Sort{ _scarecrow[i]->GetPos().y,i,SortVector::Scarecrow };
		_sort.push_back(sort);
	}

	std::sort(_sort.begin(), _sort.end(), SortFunct);
}
#pragma endregion

////Update
void EntityManager::EntityColliding(std::vector<Mob*>& _mob, std::vector<Player*>& _player, std::vector<Projectile>& _projectile, std::vector<Drop*>& _drop, std::vector<Sort*>& _sort, float _dt, sf::View& _view,
	Audio& _audio, std::vector<Building*>& _building, sf::Font& _font, Wave& _wave, std::vector<Scarecrow*>& _scarecrow, sf::Sprite& _spriteHeart)
{
	CollideBetweenMob(_mob, _wave);
	LaunchAIMob(_mob, _player);
	PlayerArrowCollision(_projectile, _player, _audio);

	MobArrowCollision(_projectile, _mob, _player, _audio);
	AttackOnMobCollision(_mob, _player, _audio);
	PlayerLongMagicAttack(_mob, _player);

	AttackOnPlayerCollision(_mob, _player, _dt, _audio);
	PlayerLifeDropCollision(_player, _drop, _audio);
	DeleteMob(_mob, _scarecrow);
	PlayerDead(_player, _audio);
	RevivePlayer(_player, _audio);
	ErasedArrow(_projectile, _view);

	TpPlayerBlockedInRed(_player);
	SetFreeFall(_player);

	//only for boss
	for (int i = 0; i < _mob.size(); i++)
	{
		if (_mob[i]->GetRace() == RaceMob::Boar)
		{
			BoarSlash(*_mob[i], _player);
		}
		else if (_mob[i]->GetRace() == RaceMob::Witch)
		{
			UpdateWitchHitBox(_mob[i]->GetWitch(), _player, _projectile, _audio);
		}
	}

	//only for _scarecrow
	for (int i = 0; i < _scarecrow.size(); i++)
	{
		MobArrowCollision(_projectile, _scarecrow, _player,_audio);
		AttackOnMobCollision(_scarecrow, _player, _audio);
		PlayerLongMagicAttack(_scarecrow, _player, _audio);
	}

}

void EntityManager::UpdateVector(GameData& _gameData, float _dt, std::vector<Player*>& _player, std::vector<Mob*>& _mob, std::vector<Scarecrow*>& _scarecrow, std::vector<Projectile>& _projectile, std::vector<Building*>& _building, std::vector<Drop*>& _food, std::vector<Sort*>& _sort, sf::Font& _font, sf::Sprite& _spriteHeart, std::vector<Building*>& _sleepingNut)
{
	UpdateDamageVector(_dt);
	UpdateGoldVector(_dt);
	FillVectorDamage(_player, _mob, _scarecrow, _font, _spriteHeart);
	FillVectorGold(_player, _font);
	FillVectorSort(_gameData, _projectile, _player, _mob, _building, _food, _sort, _scarecrow, _sleepingNut);
}

void EntityManager::Reload(void)
{
	waveLaunch = false;
	bossBarInit = false;
	setPlayerFreeFall = false;
	damageHud.clear();
	goldHud.clear();
}