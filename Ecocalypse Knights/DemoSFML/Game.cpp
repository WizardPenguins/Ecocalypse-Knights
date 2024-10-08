#include "stdafx.h"
#include "Game.h"
#include "GameStates.h"
#include "CharacterSelection.h"
#include "Network.h"
#include "EntityManager.h"
#include "PopUp.h"
#include "Upgrade.h"
#include "TroaDimmenssionSahMaire.h"
#include "EntityManagerPVP.hpp"
#include "Scarecrow.hpp"
#include "Boar.hpp"
#include "FlyingNut.h"
#include "Upgrade.h"
#include "Particle.h"
#include "Witch.hpp"

Game::Game(GameData& _gameData)
{
	InitComponents(_gameData);
	InitClasses(_gameData);
}

Game::~Game()
{
	delete fade;
	fade = nullptr;
}

void Game::InitComponents(GameData& _gameData)
{
	_gameData.ChangeLevelState(LEVEL1);
	//_gameData.camera->GetView().setCenter(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f);
	_gameData.rtShadow.setColor(sf::Color(255, 255, 255, 110));
	_gameData.rtShadowSrinks.setColor(sf::Color(255, 255, 255, 110));
	_gameData.rtRedShadow.setColor(sf::Color(255, 255, 255, 110));
	_gameData.rtBlueShadow.setColor(sf::Color(255, 255, 255, 110));

	nextLevel = Utils::LoadSprite("Assets/Images/Hud/Arrow.png", CENTER);
	currentPos.x = (float)(WORKING_WIDTH - nextLevel.getTexture()->getSize().x / 2 - 20);
	currentPos.y = WORKING_HEIGHT / 2.f;

	endPosEasing = currentPos;
	startPosEasing = { currentPos.x - 30, currentPos.y };
	timerEasing = 0.f;
	movingFront = false;

	//scale something bigger than screen width and height to the screen size
	_gameData.audio->AddMusic("Assets/Musics/GameMusic2.ogg", "INGAME");
	_gameData.audio->AddMusic("Assets/Musics/ChillMusic.ogg", "CHILL");
	_gameData.audio->AddMusic("Assets/Musics/ChillMusic.ogg", "BOSS");
	fade = new Fade(2.f, true, false);
}

void Game::InitClasses(GameData& _gameData)
{
	mapManager.Init(_gameData);

	_gameData.rt = new sf::RenderTexture();
	_gameData.rt->create((unsigned int)(Screen::GetInstance().GetWidth() * 8.f), (unsigned int)(Screen::GetInstance().GetHeight() * 5.f));

	_gameData.rtSrinks = new sf::RenderTexture();
	_gameData.rtSrinks->create((unsigned int)(Screen::GetInstance().GetWidth() * 8.f), (unsigned int)(Screen::GetInstance().GetHeight() * 5.f));

	_gameData.rtRed = new sf::RenderTexture();
	_gameData.rtRed->create((unsigned int)(Screen::GetInstance().GetWidth() * 8.f), (unsigned int)(Screen::GetInstance().GetHeight() * 5.f));

	_gameData.rtBlue = new sf::RenderTexture();
	_gameData.rtBlue->create((unsigned int)(Screen::GetInstance().GetWidth() * 8.f), (unsigned int)(Screen::GetInstance().GetHeight() * 5.f));

	_gameData.rtShadow.setTexture(_gameData.rt->getTexture());
	_gameData.rtShadowSrinks.setTexture(_gameData.rtSrinks->getTexture());
	_gameData.rtRedShadow.setTexture(_gameData.rtRed->getTexture());
	_gameData.rtBlueShadow.setTexture(_gameData.rtBlue->getTexture());
	_gameData.shadow = Utils::LoadSprite("Assets/Images/Entities/Shadow.png", CENTER);

	_gameData.flyingNut = new FlyingNut();

	for (int i = 0; i < NB_PANEL_TUTO; i++)
	{
		sf::Sprite temp = Utils::LoadSprite("Assets/Images/Hud/PannelTuto" + std::to_string(i + 1) + ".png", CENTER);
		PopUp* dialogue = new PopUp(i, temp);
		_gameData.popUpTutoVector.push_back(dialogue);
	}

	for (int i = 0; i < NB_PANEL_AOE; i++)
	{
		sf::Sprite temp = Utils::LoadSprite("Assets/Images/Hud/PannelAOE" + std::to_string(i + 1) + ".png", CENTER);
		PopUp* dialogue = new PopUp(i, temp);
		_gameData.popUpAOEVector.push_back(dialogue);
	}

	for (int i = 0; i < NB_PANEL_POTION; i++)
	{
		sf::Sprite temp = Utils::LoadSprite("Assets/Images/Hud/PannelPotion" + std::to_string(i + 1) + ".png", CENTER);
		PopUp* dialogue = new PopUp(i, temp);
		_gameData.popUpPotionVector.push_back(dialogue);
	}
}

////////////UPDATE////////////////////////////////////////////////////////
void Game::Update(GameData& _gameData)
{
	if (_gameData.reloadGame)
	{
		Reload(_gameData);
		_gameData.reloadGame = false;
	}

	if (_gameData.levelState == LEVEL1)
	{
		Level1Handler(_gameData);
		_gameData.flyingNut->Update(_gameData);

		if (_gameData.tabMob.size() == 0)
		{
			UpdateHandFade(_gameData.dt);
		}
	}
	else if (_gameData.levelState == LEVEL2)
	{
		Level2Handler(_gameData);

		if (_gameData.tabMob.size() == 0)
		{
			UpdateHandFade(_gameData.dt);
		}
	}
	else if (_gameData.levelState == LEVEL3)
	{
		Level3Handler(_gameData);

		if (_gameData.tabMob.size() == 0)
		{
			UpdateHandFade(_gameData.dt);
		}
	}
	else if (_gameData.levelState == LEVELSHOP)
	{
		_gameData.sortShop = true;
		if (_gameData.friendShip)
		{
			_gameData.reloadShop = true;
		}
		_gameData.ChangeState(SHOP);
	}
	else if (_gameData.levelState == MAP)
	{
		MapHandler(_gameData);

		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{
			_gameData.tabPlayers[i]->SetLife(_gameData.tabPlayers[i]->GetMaxLife());
			_gameData.tabPlayers[i]->SetMana(_gameData.tabPlayers[i]->GetMaxMana());
		}
	}
}

void Game::Level1Handler(GameData& _gameData)
{
	if (_gameData.network->replacePlayer)
	{
		ReloadBetweenLevel(_gameData);
		_gameData.network->replacePlayer = false;
	}
	UpdateSelectedPlayer(_gameData.tabPlayers, _gameData.playerSelected);

	_gameData.camera->Update(_gameData);
	_gameData.hud->Update(_gameData);
	UpdatePopUp(_gameData.currentPopUpTuTo, _gameData.popUpTutoVector, _gameData.TutoActive, _gameData.dt);

	//update Entity Manger
	_gameData.entityMan->EntityColliding(_gameData.tabMob, _gameData.tabPlayers, _gameData.projectile, _gameData.drop, _gameData.sortVector, _gameData.dt, _gameData.camera->GetView(), *_gameData.audio, _gameData.tabBuilding, *_gameData.GameFont, _gameData.actualWave, _gameData.scarecrow, _gameData.heartHit);

	//update Entity
	UpdateProjectile(_gameData.projectile, _gameData.tempImage1, _gameData.dt, _gameData.levelState);
	UpdatePlayers(_gameData);
	UpdateMob(_gameData.tabMob, _gameData);
	UpdateFriendShip(_gameData.entityManPVP, _gameData.tabPlayers, _gameData.projectile, _gameData.sortVector, _gameData.tabBuilding, _gameData.friendShip);

	_gameData.levelManager->Update(_gameData, _gameData.levelState);

	//network
	UpdateMultiplayer(_gameData);

	//update Sorting
	_gameData.entityMan->UpdateVector(_gameData, _gameData.dt, _gameData.tabPlayers, _gameData.tabMob, _gameData.scarecrow, _gameData.projectile, _gameData.tabBuilding, _gameData.drop, _gameData.sortVector, *_gameData.GameFont, _gameData.heartHit, _gameData.sleepingNut);

	UpdateDeath(_gameData);

	Music(_gameData);
}

void Game::Level2Handler(GameData& _gameData)
{
	UpdateSelectedPlayer(_gameData.tabPlayers, _gameData.playerSelected);

	_gameData.camera->Update(_gameData);
	_gameData.hud->Update(_gameData);

	bool playerUnlockAOE = false;
	bool playerGetAPotion = false;

	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.tabPlayers[i]->GetWisdom() >= 5)
		{
			playerUnlockAOE = true;
		}

		if (_gameData.tabPlayers[i]->GetHasAPotion())
		{
			playerGetAPotion = true;
		}
	}

	if (!playerUnlockAOE && playerGetAPotion)
	{
		UpdatePopUp(_gameData.currentPopUpPotion, _gameData.popUpPotionVector, _gameData.TutoActive, _gameData.dt);
	}
	else if (!playerGetAPotion && playerUnlockAOE)
	{
		UpdatePopUp(_gameData.currentPopUpAOE, _gameData.popUpAOEVector, _gameData.TutoActive, _gameData.dt);
	}
	else if (playerGetAPotion && playerUnlockAOE)
	{
		UpdatePopUp(_gameData.currentPopUpPotion, _gameData.popUpPotionVector, _gameData.TutoActive, _gameData.dt);
		if (_gameData.currentPopUpPotion == _gameData.popUpPotionVector.size())
		{
			UpdatePopUp(_gameData.currentPopUpAOE, _gameData.popUpAOEVector, _gameData.TutoActive, _gameData.dt);
		}
	}

	//update Entity Manger
	_gameData.entityMan->EntityColliding(_gameData.tabMob, _gameData.tabPlayers, _gameData.projectile, _gameData.drop, _gameData.sortVector, _gameData.dt, _gameData.camera->GetView(), *_gameData.audio, _gameData.tabBuilding, *_gameData.GameFont, _gameData.actualWave, _gameData.scarecrow, _gameData.heartHit);

	//update Entity
	UpdateProjectile(_gameData.projectile, _gameData.tempImage2, _gameData.dt, _gameData.levelState);
	UpdatePlayers(_gameData);
	UpdateMob(_gameData.tabMob, _gameData);
	UpdateFriendShip(_gameData.entityManPVP, _gameData.tabPlayers, _gameData.projectile, _gameData.sortVector, _gameData.tabBuilding, _gameData.friendShip);

	_gameData.levelManager->Update(_gameData, _gameData.levelState);

	//network
	UpdateMultiplayer(_gameData);

	//update Sorting
	_gameData.entityMan->UpdateVector(_gameData, _gameData.dt, _gameData.tabPlayers, _gameData.tabMob, _gameData.scarecrow, _gameData.projectile, _gameData.tabBuilding, _gameData.drop, _gameData.sortVector, *_gameData.GameFont, _gameData.heartHit, _gameData.sleepingNut);

	UpdateDeath(_gameData);

	Music(_gameData);
}

void Game::Level3Handler(GameData& _gameData)
{
	UpdateSelectedPlayer(_gameData.tabPlayers, _gameData.playerSelected);

	_gameData.camera->Update(_gameData);
	_gameData.hud->Update(_gameData);

	bool playerUnlockAOE = false;
	bool playerGetAPotion = false;

	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.tabPlayers[i]->GetWisdom() >= 5)
		{
			playerUnlockAOE = true;
		}

		if (_gameData.tabPlayers[i]->GetHasAPotion())
		{
			playerGetAPotion = true;
		}
	}

	if (!playerUnlockAOE && playerGetAPotion)
	{
		UpdatePopUp(_gameData.currentPopUpPotion, _gameData.popUpPotionVector, _gameData.TutoActive, _gameData.dt);
	}
	else if (!playerGetAPotion && playerUnlockAOE)
	{
		UpdatePopUp(_gameData.currentPopUpAOE, _gameData.popUpAOEVector, _gameData.TutoActive, _gameData.dt);
	}
	else if (playerGetAPotion && playerUnlockAOE)
	{
		UpdatePopUp(_gameData.currentPopUpPotion, _gameData.popUpPotionVector, _gameData.TutoActive, _gameData.dt);
		if (_gameData.currentPopUpPotion == _gameData.popUpPotionVector.size())
		{
			UpdatePopUp(_gameData.currentPopUpAOE, _gameData.popUpAOEVector, _gameData.TutoActive, _gameData.dt);
		}
	}

	//update Entity Manger
	_gameData.entityMan->EntityColliding(_gameData.tabMob, _gameData.tabPlayers, _gameData.projectile, _gameData.drop, _gameData.sortVector, _gameData.dt, _gameData.camera->GetView(), *_gameData.audio, _gameData.tabBuilding, *_gameData.GameFont, _gameData.actualWave, _gameData.scarecrow, _gameData.heartHit);

	//update Entity
	UpdateProjectile(_gameData.projectile, _gameData.tempImage3, _gameData.dt, _gameData.levelState);
	UpdatePlayers(_gameData);
	UpdateMob(_gameData.tabMob, _gameData);
	UpdateFriendShip(_gameData.entityManPVP, _gameData.tabPlayers, _gameData.projectile, _gameData.sortVector, _gameData.tabBuilding, _gameData.friendShip);

	_gameData.levelManager->Update(_gameData, _gameData.levelState);

	//network
	UpdateMultiplayer(_gameData);

	//update Sorting
	_gameData.entityMan->UpdateVector(_gameData, _gameData.dt, _gameData.tabPlayers, _gameData.tabMob, _gameData.scarecrow, _gameData.projectile, _gameData.tabBuilding, _gameData.drop, _gameData.sortVector, *_gameData.GameFont, _gameData.heartHit, _gameData.sleepingNut);
	timerSpawnLeaf += _gameData.dt;
	if (timerSpawnLeaf > 0.1f)
	{
		Particle::AddParticle(_gameData);
		timerSpawnLeaf = 0.f;
	}
	Particle::Update(_gameData);

	UpdateDeath(_gameData);

	Music(_gameData);
	if (canFade)
	{
		FadeManagement(_gameData);
	}
}

void Game::UpdateSelectedPlayer(std::vector<Player*>& _player, std::vector<int> _playerSelected)
{
	for (int i = 0; i < _player.size(); i++)
	{
		_player[i]->SetPlayerSelected(_playerSelected[i]);
	}
}

void Game::UpdateProjectile(std::vector<Projectile>& _proj, sf::Image& _image, float _dt, LevelState& _level)
{
	for (int i = (int)(_proj.size() - 1); i >= 0; i--)
	{
		_proj[i].Update(_dt);

		if (ArrowPointCollision(_image, _proj, i, _level))
		{
			_proj.erase(_proj.begin() + i);
		}
	}
}

void Game::UpdateFriendShip(EntityManagerPVP* _entity, std::vector<Player*>& _player, std::vector<Projectile>& _projectile, std::vector<Sort*>& _sort, std::vector<Building*>& _build, bool _activate)
{
	if (!_activate)
	{
		_entity->UpdatePVP(_player, _projectile, _sort, _build);
	}
}

void Game::UpdateMultiplayer(GameData& _gameData)
{
	if (_gameData.multiplayer)
	{
		_gameData.network->IngameStateUpdate(_gameData, &_gameData.levelState);
	}
}

void Game::UpdateMob(std::vector<Mob*>& _mob, GameData& _gameData)
{
	for (int i = 0; i < _mob.size(); i++)
	{
		_mob[i]->Update(_gameData);
	}
}

void Game::UpdatePopUp(int& _currentPopUp, std::vector<PopUp*>& _popUp, bool& _active, float& _dt)
{
	if (_currentPopUp < _popUp.size() && _active)
	{
		_popUp[_currentPopUp]->Update(_dt, _currentPopUp);
	}
}

void Game::UpdateDeath(GameData& _gameData)
{
	int death = 0;
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.tabPlayers[i]->GetLife() <= 0)
		{
			death++;
		}
	}
	if (_gameData.levelState != LEVEL3)
	{
		if (death == _gameData.tabPlayers.size() || _gameData.actualWave >= WAVE5)
		{
			timerDeath += _gameData.dt;
			if (timerDeath >= 2.f)
			{
				_gameData.reloadGameOver = true;
				FadeManagement(_gameData);
			}
		}
		else
		{
			timerDeath = 0.f;
		}
	}
	else
	{
		for (int i = 0; i < _gameData.tabMob.size(); i++)
		{
			if (_gameData.tabMob[i]->GetType() == TypeMob::Boss && _gameData.tabMob[i]->GetState() == MobState::Death)
			{
				if (!_gameData.playWitchDeath)
				{
					//witch death
					_gameData.witchDeath.openFromFile("Assets/Images/Entities/Mobs/Witch/Witch_Death.mp4");
					_gameData.audio->PlaySoundNotRepeated("WitchDeath", 100);
					_gameData.witchDeath.play();
					_gameData.witchDeath.fit(0, 0, WORKING_WIDTH, WORKING_HEIGHT);
					_gameData.playWitchDeath = true;
					_gameData.tabMob[i]->GetWitch()->GetVectorPlant().erase(_gameData.tabMob[i]->GetWitch()->GetVectorPlant().begin(), _gameData.tabMob[i]->GetWitch()->GetVectorPlant().end());

					for (int j = 0; j < _gameData.tabMob.size(); j++)
					{
						if (_gameData.tabMob[j]->GetType() != TypeMob::Boss)
						{
							_gameData.tabMob.erase(_gameData.tabMob.begin() + j);
							j--;
						}
					}

					if (_gameData.tabMob[i]->GetWitch()->GetHasSpawnCrystal())
					{
						for (int j = 0; j < 2; j++)
						{
							_gameData.tabMob[i]->GetWitch()->GetCrystal()[j].life = 0;
						}
					}
				}

				_gameData.witchDeath.update();

				if (_gameData.witchDeath.getPlayingOffset().asSeconds() >= _gameData.witchDeath.getDuration().asSeconds() - 0.65f)
				{
					_gameData.reloadGameOver = true;
					canFade = true;
					FadeManagement(_gameData);
				}
			}
			else if (death == _gameData.tabPlayers.size())
			{
				timerDeath += _gameData.dt;
				if (timerDeath >= 2.f)
				{
					_gameData.reloadGameOver = true;
					FadeManagement(_gameData);
				}
			}
			else
			{
				timerDeath = 0.f;
			}
		}
	}
}

void Game::UpdatePlayers(GameData& _gameData)
{
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		_gameData.tabPlayers[i]->Update(_gameData, _gameData.dt, false);
	}
}

void Game::UpdateHandFade(float _dt)
{
	if (movingFront)
	{
		if (timerEasing < 1)
		{
			if (timerEasing < 0.4f)
			{
				currentPos = Easings::Lerp(startPosEasing, endPosEasing, Easings::OutElastic(timerEasing));
				timerEasing += _dt / 4;
			}
			else
			{
				timerEasing += _dt * 5;
			}
		}
		else
		{
			timerEasing = 0.f;
			movingFront = false;
		}
	}
	else if (!movingFront)
	{
		if (timerEasing < 1)
		{
			if (timerEasing > 0.6f)
			{
				currentPos = Easings::Lerp(endPosEasing, startPosEasing, Easings::InElastic(timerEasing));
				timerEasing += _dt / 4;
			}
			else
			{
				timerEasing += _dt * 5;
			}
		}
		else
		{
			timerEasing = 0.f;
			movingFront = true;
		}
	}
}

bool Game::ArrowPointCollision(sf::Image& _image, std::vector<Projectile>& _tabProjectile, int _id, LevelState& _level)
{
	float arrowX;
	float arrowY;


	if (_tabProjectile[_id].direction == 1)
	{
		arrowX = (float)(_tabProjectile[_id].pos.x);
	}
	else if (_tabProjectile[_id].direction == -1)
	{
		arrowX = (float)(_tabProjectile[_id].pos.x + _tabProjectile[_id].hitBox->getSize().x);
	}
	else
	{
		return false;
	}

	arrowY = (float)(_tabProjectile[_id].posAutor.y + 5.f);

	return _image.getPixel((unsigned int)arrowX, (unsigned int)arrowY) == sf::Color::Red;
}
//////////////////////////////////////////////////////////////////////////



////////////DISPLAY//////////////////////////////////////////////////////
void Game::Display(GameData& _gameData)
{
	if (_gameData.levelState == LEVEL1)
	{
		Level1Display(_gameData);
	}
	else if (_gameData.levelState == LEVEL2)
	{
		Level2Display(_gameData);
	}
	else if (_gameData.levelState == LEVEL3)
	{
		Level3Display(_gameData);
	}
	else if (_gameData.levelState == MAP)
	{
		mapManager.Display(_gameData, level2IsDone);
	}
}

void Game::Display3D(GameData& _gameData)
{
	if (_gameData.levelState != MAP)
	{
		LevelDisplay3D(_gameData);
	}
	else
	{
		mapManager.Display3D(_gameData, level2IsDone);
	}
}

void Game::Level1Display(GameData& _gameData)
{
	_gameData.window.setView(_gameData.camera->GetView());
	_gameData.camera->Display(_gameData);
	_gameData.flyingNut->Display(_gameData);

	//shadow
	ShadowDisplay(_gameData);

	//projectile
	ProjectileShadowDisplay(_gameData);

	_gameData.window.draw(_gameData.rtShadow);

	//Viper Spell
	DisplayPoisonSpell(_gameData.tabPlayers, _gameData.window);

	//circle invoc witch
	DisplayInvocCircle(_gameData.tabMob, _gameData.window);
	_gameData.levelManager->Display(_gameData.window, _gameData.dt);

	_gameData.hud->DisplayMigrant(_gameData);
	//display all Entity with sorting
	EntitiesDisplay(_gameData);
	_gameData.entityMan->DisplayDamageVector(_gameData.window);
	_gameData.entityMan->DisplayGoldVector(_gameData.window);

	_gameData.camera->DisplayFirstPlan(_gameData);

	_gameData.window.setView(_gameData.window.getDefaultView());

	//boss bar
	DisplayBossBar(_gameData.tabMob, _gameData.entityMan, _gameData.hudBarBoss, _gameData.actualWave, _gameData.window, *_gameData.GameFont);

	_gameData.hud->Display(_gameData);

	DisplayPopUp(_gameData.currentPopUpTuTo, _gameData.popUpTutoVector, _gameData.TutoActive, _gameData.window);

	DisplayHand(_gameData.tabMob, _gameData.window, _gameData);

	fade->Display(_gameData.window);
}

void Game::LevelDisplay3D(GameData& _gameData)
{
	_gameData.renderTextureRed.setView(_gameData.camera->GetView());
	_gameData.renderTextureBlue.setView(_gameData.camera->GetView());
	_gameData.camera->Display3D(_gameData);
	_gameData.flyingNut->Display3D(_gameData);

	_gameData.rtRed->clear(sf::Color::Transparent);
	_gameData.rtBlue->clear(sf::Color::Transparent);

	//shadow
	ShadowDisplay3D(_gameData);

	//projectile
	ProjectileShadowDisplay3D(_gameData);

	_gameData.rtRed->display();
	_gameData.rtBlue->display();

	_gameData.renderTextureRed.draw(_gameData.rtRedShadow);
	_gameData.renderTextureBlue.draw(_gameData.rtBlueShadow);

	//Viper Spell
	DisplayPoisonSpell(_gameData.tabPlayers, _gameData.renderTextureRed, _gameData.renderTextureBlue);

	//circle invoc witch
	DisplayInvocCircle(_gameData.tabMob, _gameData.renderTextureRed, _gameData.renderTextureBlue);

	_gameData.levelManager->Display(_gameData.renderTextureRed, _gameData.renderTextureBlue, _gameData.dt);

	_gameData.hud->DisplayMigrant3D(_gameData);

	//display all Entity with sorting
	EntitiesDisplay3D(_gameData);

	_gameData.entityMan->DisplayDamageVector(_gameData.renderTextureRed, _gameData.renderTextureBlue);
	_gameData.entityMan->DisplayGoldVector(_gameData.renderTextureRed, _gameData.renderTextureBlue);
	if (_gameData.levelState == LEVEL3)
	{
		Particle::Display3D(_gameData);
	}
	_gameData.camera->DisplayFirstPlan(_gameData.renderTextureRed, _gameData.renderTextureBlue, _gameData.levelState, _gameData.tabPlayers[0]->GetIsDrawnBehindTree());

	_gameData.window.setView(_gameData.window.getDefaultView());
	_gameData.renderTextureRed.setView(_gameData.renderTextureRed.getDefaultView());
	_gameData.renderTextureBlue.setView(_gameData.renderTextureBlue.getDefaultView());

	//boss bar
	DisplayBossBar(_gameData.tabMob, _gameData.entityMan, _gameData.hudBarBoss, _gameData.actualWave, _gameData.renderTextureRed, _gameData.renderTextureBlue, *_gameData.GameFont);

	_gameData.hud->Display(_gameData.renderTextureRed, _gameData.renderTextureBlue, _gameData.tabPlayers, _gameData.arenaIsChose);

	if (_gameData.levelState == LEVEL1)
	{
		DisplayPopUp3D(_gameData.currentPopUpTuTo, _gameData.popUpTutoVector, _gameData.TutoActive, _gameData.renderTextureRed, _gameData.renderTextureBlue);
	}
	else if (_gameData.levelState == LEVEL2 || _gameData.levelState == LEVEL3)
	{
		DisplayPopUp3D(_gameData.currentPopUpAOE, _gameData.popUpTutoVector, _gameData.TutoActive, _gameData.renderTextureRed, _gameData.renderTextureBlue);
		DisplayPopUp3D(_gameData.currentPopUpPotion, _gameData.popUpTutoVector, _gameData.TutoActive, _gameData.renderTextureRed, _gameData.renderTextureBlue);
	}

	DisplayHand(_gameData.tabMob, _gameData.renderTextureRed, _gameData.renderTextureBlue, _gameData);

	fade->Display(_gameData.window);
}

void Game::Level2Display(GameData& _gameData)
{
	_gameData.window.setView(_gameData.camera->GetView());
	_gameData.camera->Display(_gameData);

	//shadow
	ShadowDisplay(_gameData);

	//projectile
	ProjectileShadowDisplay(_gameData);

	_gameData.window.draw(_gameData.rtShadow);

	//Viper Spell
	DisplayPoisonSpell(_gameData.tabPlayers, _gameData.window);

	//circle invoc witch
	DisplayInvocCircle(_gameData.tabMob, _gameData.window);
	_gameData.levelManager->Display(_gameData.window, _gameData.dt);

	_gameData.hud->DisplayMigrant(_gameData);
	//display all Entity with sorting
	EntitiesDisplay(_gameData);
	_gameData.entityMan->DisplayDamageVector(_gameData.window);
	_gameData.entityMan->DisplayGoldVector(_gameData.window);

	_gameData.camera->DisplayFirstPlan(_gameData);

	_gameData.window.setView(_gameData.window.getDefaultView());

	//boss bar
	DisplayBossBar(_gameData.tabMob, _gameData.entityMan, _gameData.hudBarBoss, _gameData.actualWave, _gameData.window, *_gameData.GameFont);

	_gameData.hud->Display(_gameData);

	if (_gameData.levelState == LEVEL2 || _gameData.levelState == LEVEL3)
	{
		DisplayPopUp(_gameData.currentPopUpPotion, _gameData.popUpPotionVector, _gameData.TutoActive, _gameData.window);
		DisplayPopUp(_gameData.currentPopUpAOE, _gameData.popUpAOEVector, _gameData.TutoActive, _gameData.window);
	}

	DisplayHand(_gameData.tabMob, _gameData.window, _gameData);

	fade->Display(_gameData.window);
}

void Game::Level3Display(GameData& _gameData)
{
	if (!_gameData.playWitchDeath)
	{
		_gameData.window.setView(_gameData.camera->GetView());
		_gameData.camera->Display(_gameData);

		//shadow

		ShadowDisplay(_gameData);

		//projectile
		ProjectileShadowDisplay(_gameData);

		_gameData.window.draw(_gameData.rtShadow);

		//Viper Spell
		DisplayPoisonSpell(_gameData.tabPlayers, _gameData.window);

		//circle invoc witch
		DisplayInvocCircle(_gameData.tabMob, _gameData.window);
		_gameData.levelManager->Display(_gameData.window, _gameData.dt);

		_gameData.hud->DisplayMigrant(_gameData);
		//display all Entity with sorting
		EntitiesDisplay(_gameData);
		_gameData.entityMan->DisplayDamageVector(_gameData.window);
		_gameData.entityMan->DisplayGoldVector(_gameData.window);
		Particle::Display(_gameData);
		_gameData.camera->DisplayFirstPlan(_gameData);

		//boss bar
		DisplayBossBar(_gameData.tabMob, _gameData.entityMan, _gameData.hudBarBoss, _gameData.actualWave, _gameData.window, *_gameData.GameFont);

		_gameData.window.setView(_gameData.window.getDefaultView());

		//boss bar
		DisplayBossBar(_gameData.tabMob, _gameData.entityMan, _gameData.hudBarBoss, _gameData.actualWave, _gameData.window, *_gameData.GameFont);

		_gameData.hud->Display(_gameData);

		DisplayHand(_gameData.tabMob, _gameData.window, _gameData);

		fade->Display(_gameData.window);
	}
	else if (_gameData.playWitchDeath)
	{
		_gameData.window.setView(_gameData.window.getDefaultView());
		_gameData.window.draw(_gameData.witchDeath);
		fade->Display(_gameData.window);
	}
}

void Game::DisplayAllShadowOfWitch(Mob* _witch, sf::Sprite _shadow, sf::RenderTexture* _rt)
{
	if (_witch->GetWitch()->GetHasSpawnCrystal())
	{
		for (int i = 0; i < NbCrystal; i++)
		{

			if (_witch->GetWitch()->GetCrystal()[i].life > 0)
			{

				Utils::BlitSprite(_shadow, { _witch->GetWitch()->GetCrystal()[i].posOnGround }, 0, *_rt);
			}
		}
	}
	for (int i = 0; i < _witch->GetWitch()->GetVectorPlant().size(); i++)
	{
		Utils::BlitSprite(_shadow, sf::Vector2f{ _witch->GetWitch()->GetVectorPlant()[i]->pos.x + 10.f, _witch->GetWitch()->GetVectorPlant()[i]->pos.y - 50.f }, 0, *_rt);
	}

	_shadow.setScale(sf::Vector2f{ 1.75f, 1.75f });
	Utils::BlitSprite(_shadow, { _witch->GetPos() }, 0, *_rt);
}

void Game::DisplayAllShadowOfWitch3D(Mob* _witch, sf::Sprite _shadow, sf::RenderTexture* _rtRed, sf::RenderTexture* _rtBlue)
{
	if (_witch->GetWitch()->GetHasSpawnCrystal())
	{
		for (int i = 0; i < NbCrystal; i++)
		{
			if (_witch->GetWitch()->GetCrystal()[i].life > 0)
			{
				Utils::BlitSprite(_shadow, sf::Vector2f{ _witch->GetWitch()->GetCrystal()[i].posOnGround.x + (TROAD_GAP * (-1 * _witch->GetWitch()->GetCrystal()[i].posOnGround.y + WORKING_HEIGHT) / 100.f), _witch->GetWitch()->GetCrystal()[i].posOnGround.y }, 0, *_rtRed);
				Utils::BlitSprite(_shadow, sf::Vector2f{ _witch->GetWitch()->GetCrystal()[i].posOnGround.x - (TROAD_GAP * (-1 * _witch->GetWitch()->GetCrystal()[i].posOnGround.y + WORKING_HEIGHT) / 100.f), _witch->GetWitch()->GetCrystal()[i].posOnGround.y }, 0, *_rtBlue);
			}
		}
	}
	for (int i = 0; i < _witch->GetWitch()->GetVectorPlant().size(); i++)
	{
		Utils::BlitSprite(_shadow, sf::Vector2f{ (_witch->GetWitch()->GetVectorPlant()[i]->pos.x + 10.f) + (TROAD_GAP * (-1 * (_witch->GetWitch()->GetVectorPlant()[i]->pos.y - 50.f) + WORKING_HEIGHT) / 100.f), (_witch->GetWitch()->GetVectorPlant()[i]->pos.y - 50.f) }, 0, *_rtRed);
		Utils::BlitSprite(_shadow, sf::Vector2f{ (_witch->GetWitch()->GetVectorPlant()[i]->pos.x + 10.f) - (TROAD_GAP * (-1 * (_witch->GetWitch()->GetVectorPlant()[i]->pos.y - 50.f) + WORKING_HEIGHT) / 100.f), (_witch->GetWitch()->GetVectorPlant()[i]->pos.y - 50.f) }, 0, *_rtBlue);
	}

	_shadow.setScale(sf::Vector2f{ 1.75f, 1.75f });
	Utils::BlitSprite(_shadow, { _witch->GetPos().x + (TROAD_GAP * (-1 * _witch->GetPos().y + WORKING_HEIGHT) / 100.f),_witch->GetPos().y }, 0, *_rtRed);
	Utils::BlitSprite(_shadow, { _witch->GetPos().x - (TROAD_GAP * (-1 * _witch->GetPos().y + WORKING_HEIGHT) / 100.f),_witch->GetPos().y }, 0, *_rtBlue);
}

void Game::DisplayPoisonSpell(std::vector<Player*>& _player, sf::RenderWindow& _window)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetPlayerSelected() == 1)
		{
			_player[i]->DisplayViperSpell(_window);
		}
	}
}

void Game::DisplayPoisonSpell(std::vector<Player*>& _player, sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetPlayerSelected() == 1)
		{
			_player[i]->DisplayViperSpell(_red, _blue);
		}
	}
}

void Game::DisplayInvocCircle(std::vector<Mob*>& _mob, sf::RenderWindow& _window)
{
	for (int i = 0; i < _mob.size(); i++)
	{
		if (_mob[i]->GetRace() == RaceMob::Witch)
		{
			_mob[i]->GetWitch()->DrawInvocation(_window);
		}
	}
}

void Game::DisplayInvocCircle(std::vector<Mob*>& _mob, sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	for (int i = 0; i < _mob.size(); i++)
	{
		if (_mob[i]->GetRace() == RaceMob::Witch)
		{
			_mob[i]->GetWitch()->DrawInvocation(_red, _blue);
		}
	}
}

void Game::DisplayBossBar(std::vector<Mob*>& _mob, EntityManager* _entiy, HudBarBoss& _spriteBoss, Wave& _wave, sf::RenderWindow& _window, sf::Font _font)
{
	for (int i = 0; i < _mob.size(); i++)
	{
		if (_mob[i]->GetRace() == RaceMob::Boar)
		{
			_entiy->DrawBossBar("boar", _spriteBoss, *_mob[i], _wave, _window, 0, _font, "The Hamuraï");
		}
		else if (_mob[i]->GetRace() == RaceMob::Witch)
		{
			_entiy->DrawBossBar("Witch", _spriteBoss, *_mob[i], _wave, _window, 60.f, _font, "The Winoaky");
		}
	}
}

void Game::DisplayBossBar(std::vector<Mob*>& _mob, EntityManager* _entiy, HudBarBoss& _spriteBoss, Wave& _wave, sf::RenderTexture& _red, sf::RenderTexture& _blue, sf::Font _font)
{
	for (int i = 0; i < _mob.size(); i++)
	{
		if (_mob[i]->GetRace() == RaceMob::Boar)
		{
			_entiy->DrawBossBar("boar", _spriteBoss, *_mob[i], _wave, _red, _blue, 0, _font, "The Hamuraï");
		}
		else if (_mob[i]->GetRace() == RaceMob::Witch)
		{
			_entiy->DrawBossBar("Witch", _spriteBoss, *_mob[i], _wave, _red, _blue, 60.f, _font, "The Winoaky");
		}
	}
}

void Game::DisplayPopUp(int _currentPopUp, std::vector<PopUp*>& _popUp, bool _active, sf::RenderWindow& _window)
{
	if (_currentPopUp < _popUp.size() && _active)
	{
		_popUp[_currentPopUp]->Draw(_window);
	}
}

void Game::DisplayPopUp3D(int _currentPopUp, std::vector<PopUp*>& _popUp, bool _active, sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	if (_currentPopUp < _popUp.size() && _active)
	{
		_popUp[_currentPopUp]->Draw(_red, _blue);
	}
}

void Game::DisplayHand(std::vector<Mob*>& _mob, sf::RenderWindow& _window, GameData& _gameData)
{
	if (!_mob.size() && _gameData.actualWave != WAVE6)//empty
	{
		Utils::BlitSprite(nextLevel, { (float)(currentPos.x), (float)(currentPos.y) }, _window);
	}
	else
	{
		for (int i = 0; i < _mob.size(); i++)
		{
			if (_mob[i]->GetType() == TypeMob::Boss && _mob[i]->GetState() == MobState::Death && _gameData.actualWave != WAVE6)
			{
				Utils::BlitSprite(nextLevel, { (float)(currentPos.x), (float)(currentPos.y) }, _window);
			}
		}
	}
}

void Game::DisplayHand(std::vector<Mob*>& _mob, sf::RenderTexture& _red, sf::RenderTexture& _blue, GameData& _gameData)
{
	if (!_mob.size() && _gameData.actualWave != WAVE6)//empty
	{
		Utils::BlitSprite(nextLevel, sf::Vector2f{ (float)currentPos.x + (TROAD_GAP * (-1 * currentPos.y + WORKING_HEIGHT) / 100.f), (float)(currentPos.y) }, 0, _red);
		Utils::BlitSprite(nextLevel, sf::Vector2f{ (float)currentPos.x - (TROAD_GAP * (-1 * currentPos.y + WORKING_HEIGHT) / 100.f), (float)(currentPos.y) }, 0, _blue);
	}
	else
	{
		for (int i = 0; i < _mob.size(); i++)
		{
			if (_mob[i]->GetType() == TypeMob::Boss && _mob[i]->GetState() == MobState::Death && _gameData.actualWave != WAVE6)
			{
				Utils::BlitSprite(nextLevel, sf::Vector2f{ (float)currentPos.x + (TROAD_GAP * (-1 * currentPos.y + WORKING_HEIGHT) / 100.f), (float)(currentPos.y) }, 0, _red);
				Utils::BlitSprite(nextLevel, sf::Vector2f{ (float)currentPos.x - (TROAD_GAP * (-1 * currentPos.y + WORKING_HEIGHT) / 100.f), (float)(currentPos.y) }, 0, _blue);
			}
		}
	}
}

//display entities with sorting on y
void Game::EntitiesDisplay(GameData& _gameData)
{
	for (int i = 0; i < _gameData.sortVector.size(); i++)
	{
		if (_gameData.sortVector.at(i)->sort == SortVector::Player)
		{
			if (!_gameData.tabPlayers.at(_gameData.sortVector.at(i)->i)->playerIsShrinks)
			{
				_gameData.tabPlayers.at(_gameData.sortVector.at(i)->i)->Display(_gameData.window, *_gameData.audio, _gameData.camera, _gameData.levelState);
			}
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Ice)
		{
			_gameData.tabPlayers.at(_gameData.sortVector.at(i)->i)->DisplayIceSpell(_gameData.window);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Mob)
		{
			_gameData.entityMan->DrawLifeBar(_gameData.tabMob.at(_gameData.sortVector.at(i)->i), _gameData.hudBarMob.lifeBar, _gameData.hudBarMob.pixelLife, _gameData.showMobHpBar, _gameData.window);
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->Display(_gameData.window);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Building)
		{
			sf::Vector2f pos = _gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite.getPosition();
			if (_gameData.gameState != PAUSE && _gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->isAnim)
			{
				bool temp = true;
				_gameData.camera->GetParalax().animSort[_gameData.sortVector.at(i)->i]->UpdateLeftToRight(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, _gameData.dt, 5, 0, temp, true);
			}
			Utils::BlitSprite(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, pos, _gameData.window);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::SleepingNut)
		{
			sf::Vector2f pos = _gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->sprite.getPosition();
			if (_gameData.gameState != PAUSE && _gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->isAnim)
			{
				bool temp = true;
				_gameData.camera->GetParalax().animSort[1]->UpdateLeftToRight(_gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->sprite, _gameData.dt, 5, 0, temp, true);
			}
			Utils::BlitSprite(_gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->sprite, pos, _gameData.window);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Food)
		{
			Utils::BlitSprite(_gameData.drop.at(_gameData.sortVector.at(i)->i)->sprite, _gameData.drop.at(_gameData.sortVector.at(i)->i)->pos, _gameData.window);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Scarecrow)
		{
			_gameData.scarecrow.at(_gameData.sortVector.at(i)->i)->Display(_gameData.window, _gameData.dt);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Crystal)
		{
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->DisplayCrystal(_gameData.window, &_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->GetCrystal()[_gameData.sortVector.at(i)->j]);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Plante)
		{
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->DisplayPlante(_gameData.window, _gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->GetVectorPlant()[_gameData.sortVector.at(i)->j]);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Bramble)
		{
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->DisplayBramble(_gameData.window);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::SlashH || _gameData.sortVector.at(i)->sort == SortVector::SlashV)
		{
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetBoar()->DisplaySlash(_gameData.window);
		}
		else
		{
			_gameData.projectile.at(_gameData.sortVector.at(i)->i).Display(_gameData.window);
		}
	}
}

void Game::EntitiesDisplay3D(GameData& _gameData)
{
	for (int i = 0; i < _gameData.sortVector.size(); i++)
	{
		if (_gameData.sortVector.at(i)->sort == SortVector::Player)
		{
			_gameData.tabPlayers[_gameData.sortVector.at(i)->i]->Display(*_gameData.audio, _gameData.renderTextureRed, _gameData.renderTextureBlue, _gameData.camera, _gameData.levelState);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Ice)
		{
			_gameData.tabPlayers.at(_gameData.sortVector.at(i)->i)->DisplayIceSpell(_gameData.renderTextureRed, _gameData.renderTextureBlue);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Mob)
		{
			_gameData.entityMan->DrawLifeBar(_gameData.tabMob.at(_gameData.sortVector.at(i)->i), _gameData.hudBarMob.lifeBar, _gameData.hudBarMob.pixelLife, _gameData.showMobHpBar, _gameData.renderTextureRed, _gameData.renderTextureBlue);
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->Display(_gameData.renderTextureRed, _gameData.renderTextureBlue);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Building)
		{
			sf::Vector2f pos = _gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite.getPosition();
			if (_gameData.gameState != PAUSE && _gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->isAnim)
			{
				bool temp = true;
				_gameData.camera->GetParalax().animSort[_gameData.sortVector.at(i)->i]->UpdateLeftToRight(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, _gameData.dt, 5, 0, temp, true);
			}
			Utils::BlitSprite(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, { pos.x + (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f),pos.y }, _gameData.renderTextureRed);
			Utils::BlitSprite(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, { pos.x - (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f),pos.y }, _gameData.renderTextureBlue);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Food)
		{
			Utils::BlitSprite(_gameData.drop.at(_gameData.sortVector.at(i)->i)->sprite, { _gameData.drop.at(_gameData.sortVector.at(i)->i)->pos.x + (TROAD_GAP * (-1 * _gameData.drop.at(_gameData.sortVector.at(i)->i)->pos.y + WORKING_HEIGHT) / 100.f), _gameData.drop.at(_gameData.sortVector.at(i)->i)->pos.y }, _gameData.renderTextureRed);
			Utils::BlitSprite(_gameData.drop.at(_gameData.sortVector.at(i)->i)->sprite, { _gameData.drop.at(_gameData.sortVector.at(i)->i)->pos.x - (TROAD_GAP * (-1 * _gameData.drop.at(_gameData.sortVector.at(i)->i)->pos.y + WORKING_HEIGHT) / 100.f), _gameData.drop.at(_gameData.sortVector.at(i)->i)->pos.y }, _gameData.renderTextureBlue);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Scarecrow)
		{
			_gameData.scarecrow.at(_gameData.sortVector.at(i)->i)->Display(_gameData.renderTextureRed, _gameData.renderTextureBlue, _gameData.dt);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Crystal)
		{
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->DisplayCrystal(_gameData.renderTextureRed, _gameData.renderTextureBlue, &_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->GetCrystal()[_gameData.sortVector.at(i)->j]);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Plante)
		{
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->DisplayPlante(_gameData.renderTextureRed, _gameData.renderTextureBlue, _gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->GetVectorPlant()[_gameData.sortVector.at(i)->j]);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Bramble)
		{
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->DisplayBramble(_gameData.renderTextureRed, _gameData.renderTextureBlue);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::SlashH || _gameData.sortVector.at(i)->sort == SortVector::SlashV)
		{
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetBoar()->DisplaySlash(_gameData.renderTextureRed, _gameData.renderTextureBlue);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::SleepingNut)
		{
			sf::Vector2f pos = _gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->sprite.getPosition();
			if (_gameData.gameState != PAUSE && _gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->isAnim)
			{
				bool temp = true;
				_gameData.camera->GetParalax().animSort[1]->UpdateLeftToRight(_gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->sprite, _gameData.dt, 5, 0, temp, true);
			}

			Utils::BlitSprite(_gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->sprite, sf::Vector2f { pos.x - (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f), pos.y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(_gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->sprite, sf::Vector2f { pos.x + (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f), pos.y }, 0, _gameData.renderTextureBlue);
		}
		else
		{
			_gameData.projectile.at(_gameData.sortVector.at(i)->i).Display(_gameData.renderTextureRed, _gameData.renderTextureBlue);
		}
	}

}

void Game::ShadowDisplay(GameData& _gameData)
{
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.tabPlayers[i]->GetState() != EAT_BY_BOSS && !_gameData.tabPlayers[i]->GetFreeFall())
		{
			sf::Vector2f baseScale = { 1.5f ,1.5f };

			if (!_gameData.tabPlayers[i]->playerIsShrinks)
			{
				float echelleMinimale = (baseScale.y / 1.6f);
				float echelleMaximale = baseScale.y;

				float echelle;

				if (_gameData.tabPlayers[i]->GetInAir())
				{
					float hauteurSol = _gameData.tabPlayers[i]->GetPos().y;
					float hauteurSaut = _gameData.tabPlayers[i]->GetPos().y - JUMPDISTANCE;
					float hauteurOmbre = _gameData.tabPlayers[i]->GetPos().y;
					float hauteurActuelle = _gameData.tabPlayers[i]->GetPosInAir().y;
					float facteurEchelle = (echelleMaximale - echelleMinimale) / (hauteurSol - hauteurSaut);
					echelle = echelleMaximale - facteurEchelle * (hauteurOmbre - hauteurActuelle);
				}
				else
				{
					echelle = echelleMaximale;
				}

				if (_gameData.tabPlayers[i]->GetState() == PARRY)
				{
					_gameData.shadow.setScale({ 1.8f,1.5f });
				}
				else
				{
					_gameData.shadow.setScale({ echelle,echelle });
				}
				((bool)(_gameData.tabPlayers[i]->GetLastDir() == LEFT) ?
					Utils::BlitSprite(_gameData.shadow, { _gameData.tabPlayers[i]->GetPos().x + 10, _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rt) :
					Utils::BlitSprite(_gameData.shadow, { _gameData.tabPlayers[i]->GetPos().x - 10, _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rt));
			}
		}
	}

	for (int i = 0; i < _gameData.tabMob.size(); i++)
	{
		if (_gameData.tabMob[i]->GetRace() == RaceMob::Witch)
		{
			DisplayAllShadowOfWitch(_gameData.tabMob[i], _gameData.shadow, _gameData.rt);
		}

		if (_gameData.tabMob[i]->GetState() != MobState::Dispawn && _gameData.tabMob[i]->GetRace() != RaceMob::Witch)
		{
			sf::Vector2f dimSprite;
			sf::Vector2f decalage{ 0, 0 };
			if (_gameData.tabMob[i]->GetRace() == RaceMob::Boar)
			{
				dimSprite = _gameData.tabMob[i]->GetBoar()->GetDimSprite();

				if (_gameData.tabMob[i]->GetBoar()->GetSlashVertAlive())
				{
					_gameData.shadow.setScale(sf::Vector2f{ 1.25f, 0.75f });
					Utils::BlitSprite(_gameData.shadow, { _gameData.tabMob[i]->GetBoar()->GetSlash()->getPosition() }, 0, *_gameData.rt);
				}
			}
			else
			{
				dimSprite = _gameData.tabMob[i]->GetDimSprite();
			}
			_gameData.shadow.setScale(sf::Vector2f{ (float)(dimSprite.x / 100.f), (float)(dimSprite.y / 100.f) });
			Utils::BlitSprite(_gameData.shadow, { _gameData.tabMob[i]->GetPos() + _gameData.tabMob[i]->GetDecalage() }, 0, *_gameData.rt);
		}
	}

	for (int i = 0; i < _gameData.scarecrow.size(); i++)
	{
		_gameData.shadow.setScale(sf::Vector2f{ 1.f, 1.f });
		Utils::BlitSprite(_gameData.shadow, _gameData.scarecrow.at(i)->GetPos(), 0, *_gameData.rt);
	}
}

void Game::ShadowDisplay3D(GameData& _gameData)
{
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.tabPlayers[i]->GetState() != EAT_BY_BOSS && !_gameData.tabPlayers[i]->GetFreeFall())
		{
			sf::Vector2f baseScale = { 1.5f ,1.5f };

			if (!_gameData.tabPlayers[i]->playerIsShrinks)
			{
				float echelleMinimale = (baseScale.y / 1.6f);
				float echelleMaximale = baseScale.y;

				float echelle;

				if (_gameData.tabPlayers[i]->GetInAir())
				{
					float hauteurSol = _gameData.tabPlayers[i]->GetPos().y;
					float hauteurSaut = _gameData.tabPlayers[i]->GetPos().y - JUMPDISTANCE;
					float hauteurOmbre = _gameData.tabPlayers[i]->GetPos().y;
					float hauteurActuelle = _gameData.tabPlayers[i]->GetPosInAir().y;
					float facteurEchelle = (echelleMaximale - echelleMinimale) / (hauteurSol - hauteurSaut);
					echelle = echelleMaximale - facteurEchelle * (hauteurOmbre - hauteurActuelle);
				}
				else
				{
					echelle = echelleMaximale;
				}

				if (_gameData.tabPlayers[i]->GetState() == PARRY)
				{
					_gameData.shadow.setScale({ 1.8f,1.5f });
				}
				else
				{
					_gameData.shadow.setScale({ echelle,echelle });
				}

				if (_gameData.tabPlayers[i]->GetLastDir() == LEFT)
				{
					Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x + 10) + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtRed);
					Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x + 10) - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtBlue);
				}
				else
				{
					Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x - 10) + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtRed);
					Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x - 10) - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtBlue);
				}
			}
		}
	}

	for (int i = 0; i < _gameData.tabMob.size(); i++)
	{
		if (_gameData.tabMob[i]->GetRace() == RaceMob::Witch)
		{
			DisplayAllShadowOfWitch3D(_gameData.tabMob[i], _gameData.shadow, _gameData.rtRed, _gameData.rtBlue);
		}

		if (_gameData.tabMob[i]->GetState() != MobState::Dispawn && _gameData.tabMob[i]->GetRace() != RaceMob::Witch)
		{
			sf::Vector2f dimSprite;
			sf::Vector2f decalage{ 0, 0 };
			if (_gameData.tabMob[i]->GetRace() == RaceMob::Boar)
			{
				dimSprite = _gameData.tabMob[i]->GetBoar()->GetDimSprite();

				if (_gameData.tabMob[i]->GetBoar()->GetSlashVertAlive())
				{
					_gameData.shadow.setScale(sf::Vector2f{ 1.25f, 0.75f });
					Utils::BlitSprite(_gameData.shadow, sf::Vector2f{ _gameData.tabMob[i]->GetBoar()->GetSlash()->getPosition().x + (TROAD_GAP * (-1 * _gameData.tabMob[i]->GetBoar()->GetSlash()->getPosition().y + WORKING_HEIGHT) / 100.f), _gameData.tabMob[i]->GetBoar()->GetSlash()->getPosition().y }, 0, *_gameData.rtRed);
					Utils::BlitSprite(_gameData.shadow, sf::Vector2f{ _gameData.tabMob[i]->GetBoar()->GetSlash()->getPosition().x + (TROAD_GAP * (-1 * _gameData.tabMob[i]->GetBoar()->GetSlash()->getPosition().y + WORKING_HEIGHT) / 100.f), _gameData.tabMob[i]->GetBoar()->GetSlash()->getPosition().y }, 0, *_gameData.rtBlue);
				}
			}
			else
			{
				dimSprite = _gameData.tabMob[i]->GetDimSprite();
			}
			_gameData.shadow.setScale(sf::Vector2f{ (float)(dimSprite.x / 100.f), (float)(dimSprite.y / 100.f) });
			Utils::BlitSprite(_gameData.shadow, sf::Vector2f{ (_gameData.tabMob[i]->GetPos().x + _gameData.tabMob[i]->GetDecalage().x) + (TROAD_GAP * (-1 * (_gameData.tabMob[i]->GetPos().y + _gameData.tabMob[i]->GetDecalage().y) + WORKING_HEIGHT) / 100.f), (_gameData.tabMob[i]->GetPos().y + _gameData.tabMob[i]->GetDecalage().y) }, 0, *_gameData.rtRed);
			Utils::BlitSprite(_gameData.shadow, sf::Vector2f{ (_gameData.tabMob[i]->GetPos().x + _gameData.tabMob[i]->GetDecalage().x) - (TROAD_GAP * (-1 * (_gameData.tabMob[i]->GetPos().y + _gameData.tabMob[i]->GetDecalage().y) + WORKING_HEIGHT) / 100.f), (_gameData.tabMob[i]->GetPos().y + _gameData.tabMob[i]->GetDecalage().y) }, 0, *_gameData.rtBlue);
		}
	}

	for (int i = 0; i < _gameData.scarecrow.size(); i++)
	{
		_gameData.shadow.setScale(sf::Vector2f{ 1.f, 1.f });
		Utils::BlitSprite(_gameData.shadow, sf::Vector2f{ _gameData.scarecrow.at(i)->GetPos().x + (TROAD_GAP * (-1 * _gameData.scarecrow.at(i)->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.scarecrow.at(i)->GetPos().y }, 0, *_gameData.rtRed);
		Utils::BlitSprite(_gameData.shadow, sf::Vector2f{ _gameData.scarecrow.at(i)->GetPos().x - (TROAD_GAP * (-1 * _gameData.scarecrow.at(i)->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.scarecrow.at(i)->GetPos().y }, 0, *_gameData.rtBlue);
	}
}

void Game::ProjectileShadowDisplay(GameData& _gameData)
{
	for (int i = 0; i < _gameData.projectile.size(); i++)
	{
		if (_gameData.projectile.at(i).projetileType == TypeProjectile::ARROW)
		{
			_gameData.shadow.setScale({ 1.f,0.7f });
			if (_gameData.projectile.at(i).direction == -1)
			{
				Utils::BlitSprite(_gameData.shadow, { _gameData.projectile.at(i).pos.x + 50 , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rt);
			}
			else
			{
				Utils::BlitSprite(_gameData.shadow, { _gameData.projectile.at(i).pos.x - 50 , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rt);
			}
		}
		else if (_gameData.projectile.at(i).autor != AutorProjectile::MOB)
		{
			_gameData.shadow.setScale({ 0.7f,0.7f });
			if (_gameData.projectile.at(i).direction == -1)
			{
				Utils::BlitSprite(_gameData.shadow, { _gameData.projectile.at(i).pos.x + 30 , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rt);
			}
			else
			{
				Utils::BlitSprite(_gameData.shadow, { _gameData.projectile.at(i).pos.x - 30 , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rt);
			}
		}

	}
}

void Game::ProjectileShadowDisplay3D(GameData& _gameData)
{
	for (int i = 0; i < _gameData.projectile.size(); i++)
	{
		if (_gameData.projectile.at(i).projetileType == TypeProjectile::ARROW)
		{
			_gameData.shadow.setScale({ 1.f,0.7f });
			if (_gameData.projectile.at(i).direction == -1)
			{
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x + 50) + (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f) , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtRed);
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x + 50) - (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f) , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtBlue);
			}
			else
			{
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x - 50) + (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f) , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtRed);
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x - 50) - (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f) , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtBlue);
			}
		}
		else if (_gameData.projectile.at(i).autor != AutorProjectile::MOB)
		{
			_gameData.shadow.setScale({ 0.7f,0.7f });
			if (_gameData.projectile.at(i).direction == -1)
			{
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x + 30) + (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f), _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtRed);
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x + 30) - (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f) , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtBlue);
			}
			else
			{
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x - 30) + (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f), _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtRed);
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x - 30) - (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f) , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtBlue);
			}
		}

	}
}
//////////////////////////////////////////////////////////////////////////

void Game::ReloadBetweenLevel(GameData& _gameData)
{
	delete fade;
	fade = nullptr;

	fade = new Fade(2.f, true, false);
	_gameData.drop.clear();
	_gameData.sortVector.clear();
	_gameData.camera->Reload(_gameData);
	_gameData.entityMan->Reload();
	_gameData.reloadFlyingNut = true;
	_gameData.inCinematic = false;
	currentPos.x = (float)(WORKING_WIDTH - nextLevel.getTexture()->getSize().x / 2 - 20);
	currentPos.y = WORKING_HEIGHT / 2.f;

	endPosEasing = currentPos;
	startPosEasing = { currentPos.x - 30, currentPos.y };
	timerEasing = 0.f;
	movingFront = false;
	mapManager.Reload(_gameData);

	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.multiplayer)
		{
			int id = 0;
			if (_gameData.network->state == NetworkState::CLIENT)
			{
				id = _gameData.network->client->id;
			}
			switch (id)
			{
			case 0:
				_gameData.tabPlayers[id]->SetPos({ 500.f, 900.f });
				break;
			case 1:
				_gameData.tabPlayers[id]->SetPos({ 500.f, 800.f });
				break;
			case 2:
				_gameData.tabPlayers[id]->SetPos({ 400.f, 900.f });
				break;
			case 3:
				_gameData.tabPlayers[id]->SetPos({ 400.f, 800.f });
				break;
			}

			if (_gameData.levelState == LEVEL3)
			{
				_gameData.tabPlayers[id]->SetPos({ _gameData.tabPlayers[id]->GetPos().x,  _gameData.tabPlayers[id]->GetPos().y + LEVEL3_GAP });
			}

			_gameData.tabPlayers[id]->SetInAir(false);
			_gameData.tabPlayers[id]->SetPosInAir(_gameData.tabPlayers[id]->GetPos());
			_gameData.tabPlayers[id]->SetLife(_gameData.tabPlayers[id]->GetMaxLife());
			_gameData.tabPlayers[id]->SetPlayerState(IDLE);
			_gameData.tabPlayers[id]->ResetPotionEffectBetweenLevel();
		}
		else
		{
			switch (i)
			{
			case 0:
				_gameData.tabPlayers[i]->SetPos({ 500.f, 900.f });
				break;
			case 1:
				_gameData.tabPlayers[i]->SetPos({ 500.f, 800.f });
				break;
			case 2:
				_gameData.tabPlayers[i]->SetPos({ 400.f, 900.f });
				break;
			case 3:
				_gameData.tabPlayers[i]->SetPos({ 400.f, 800.f });
				break;
			}

			if (_gameData.levelState == LEVEL3)
			{
				_gameData.tabPlayers[i]->SetPos({ _gameData.tabPlayers[i]->GetPos().x,  _gameData.tabPlayers[i]->GetPos().y + LEVEL3_GAP });
			}

			_gameData.tabPlayers[i]->SetInAir(false);
			_gameData.tabPlayers[i]->SetPosInAir(_gameData.tabPlayers[i]->GetPos());
			_gameData.tabPlayers[i]->SetLife(_gameData.tabPlayers[i]->GetMaxLife());
			_gameData.tabPlayers[i]->SetPlayerState(IDLE);
			_gameData.tabPlayers[i]->ResetPotionEffectBetweenLevel();
		}

		_gameData.reloadLevelManager = true;
		_gameData.playWitchDeath = false;
		canFade = false;
	}
}

void Game::FadeManagement(GameData& _gameData)
{
	fade->Update(_gameData.dt);

	//if (!_gameData.multiplayer || _gameData.network->state == NetworkState::HOST)
	{
		if (fade->GetTimer() >= fade->GetTimerMax())
		{
			if (_gameData.actualWave >= WAVE5)
			{
				if (_gameData.levelState == LEVEL2)
				{
					level2IsDone = true;
				}
				else if (_gameData.levelState == LEVEL1)
				{
					level1IsDone = true;
				}
				if (_gameData.levelState != LEVEL3)
				{
					ReloadBetweenLevel(_gameData);
					_gameData.actualWave = WAVE1;
					_gameData.ChangeLevelState(MAP);
					_gameData.camera->GetView().setCenter({ 929.f, 1384.f });
					_gameData.ChangeState(UPGRADE);
					_gameData.camera->SetDefaultCenter();
					_gameData.camera->SetDefaultZoom();
					_gameData.reloadShop = true;
				}
				else if (_gameData.levelState == LEVEL3)
				{
					_gameData.reloadGameOver = true;
					_gameData.reloadShop = true;
					_gameData.ChangeState(GAMEOVER);
					_gameData.camera->SetDefaultCenter();
					_gameData.camera->SetDefaultZoom();
				}
			}
			else
			{
				_gameData.reloadGameOver = true;
				_gameData.reloadShop = true;
				_gameData.ChangeState(GAMEOVER);
				_gameData.camera->SetDefaultCenter();
				_gameData.camera->SetDefaultZoom();
			}
		}
	}
}

void Game::Reload(GameData& _gameData)
{
	delete fade;
	fade = nullptr;

	fade = new Fade(2.f, true, false);
	_gameData.ChangeLevelState(LEVEL1);

	_gameData.drop.clear();
	_gameData.sortVector.clear();
	_gameData.projectile.clear();
	_gameData.tabMob.clear();
	_gameData.camera->Reload(_gameData);
	_gameData.entityMan->Reload();
	_gameData.tabBuilding.clear();
	_gameData.camera->GetParalax().Reload(_gameData);
	_gameData.inCinematic = false;
	currentPos.x = (float)(WORKING_WIDTH - nextLevel.getTexture()->getSize().x / 2 - 20);
	currentPos.y = WORKING_HEIGHT / 2.f;

	endPosEasing = currentPos;
	startPosEasing = { currentPos.x - 30, currentPos.y };
	timerEasing = 0.f;
	movingFront = false;
	_gameData.tabPlayers.clear();
	_gameData.currentPopUpTuTo = 0;
	_gameData.currentPopUpPotion = 0;
	_gameData.currentPopUpAOE = 0;

	int joystickCount = sf::Joystick::isConnected(0) + sf::Joystick::isConnected(1) + sf::Joystick::isConnected(2) + sf::Joystick::isConnected(3);
	for (int i = 0; i < joystickCount; i++)
	{
		Player* player = new Player(_gameData, i);
		_gameData.tabPlayers.push_back(player);
		if (i == joystickCount - 1)
		{
			player->Init();
		}
	}

	_gameData.levelManager->Reload(_gameData);
	mapManager.Reload(_gameData);
	level1IsDone = false;
	level2IsDone = false;
}

void Game::MapHandler(GameData& _gameData)
{
	mapManager.Update(_gameData, level1IsDone, level2IsDone);

	if (_gameData.timerButton <= 0.f)
	{
		if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CROSS))
			{
				_gameData.levelState = (LevelState)mapManager.SetLevel(_gameData);
				_gameData.camera->Reload(_gameData);
				_gameData.camera->GetParalax().Reload(_gameData);

				if (_gameData.levelState != MAP)
				{
					_gameData.levelManager->Init(_gameData.levelState, _gameData);
				}

				for (int i = 0; i < _gameData.tabPlayers.size(); i++)
				{
					if (_gameData.multiplayer)
					{
						int id = 0;
						if (_gameData.network->state == NetworkState::CLIENT)
						{
							id = _gameData.network->client->id;
						}
						switch (id)
						{
						case 0:
							_gameData.tabPlayers[id]->SetPos({ 500.f, 900.f });
							break;
						case 1:
							_gameData.tabPlayers[id]->SetPos({ 500.f, 800.f });
							break;
						case 2:
							_gameData.tabPlayers[id]->SetPos({ 400.f, 900.f });
							break;
						case 3:
							_gameData.tabPlayers[id]->SetPos({ 400.f, 800.f });
							break;
						}
					}
					else
					{
						switch (i)
						{
						case 0:
							_gameData.tabPlayers[i]->SetPos({ 500.f, 900.f });
							break;
						case 1:
							_gameData.tabPlayers[i]->SetPos({ 500.f, 800.f });
							break;
						case 2:
							_gameData.tabPlayers[i]->SetPos({ 400.f, 900.f });
							break;
						case 3:
							_gameData.tabPlayers[i]->SetPos({ 400.f, 800.f });
							break;
						}
					}

					if (_gameData.levelState == LEVEL3)
					{
						_gameData.tabPlayers[i]->SetPos({ _gameData.tabPlayers[i]->GetPos().x,  _gameData.tabPlayers[i]->GetPos().y + LEVEL3_GAP });
					}
				}

			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::A))
			{
				_gameData.levelState = (LevelState)mapManager.SetLevel(_gameData);
				_gameData.camera->Reload(_gameData);
				_gameData.camera->GetParalax().Reload(_gameData);
				if (_gameData.levelState != MAP)
				{
					_gameData.levelManager->Init(_gameData.levelState, _gameData);
				}

				for (int i = 0; i < _gameData.tabPlayers.size(); i++)
				{
					if (_gameData.multiplayer)
					{
						int id = 0;
						if (_gameData.network->state == NetworkState::CLIENT)
						{
							id = _gameData.network->client->id;
						}
						switch (id)
						{
						case 0:
							_gameData.tabPlayers[id]->SetPos({ 500.f, 900.f });
							break;
						case 1:
							_gameData.tabPlayers[id]->SetPos({ 500.f, 800.f });
							break;
						case 2:
							_gameData.tabPlayers[id]->SetPos({ 400.f, 900.f });
							break;
						case 3:
							_gameData.tabPlayers[id]->SetPos({ 400.f, 800.f });
							break;
						}
					}
					else
					{
						switch (i)
						{
						case 0:
							_gameData.tabPlayers[i]->SetPos({ 500.f, 900.f });
							break;
						case 1:
							_gameData.tabPlayers[i]->SetPos({ 500.f, 800.f });
							break;
						case 2:
							_gameData.tabPlayers[i]->SetPos({ 400.f, 900.f });
							break;
						case 3:
							_gameData.tabPlayers[i]->SetPos({ 400.f, 800.f });
							break;
						}
					}

					if (_gameData.levelState == LEVEL3)
					{
						_gameData.tabPlayers[i]->SetPos({ _gameData.tabPlayers[i]->GetPos().x,  _gameData.tabPlayers[i]->GetPos().y + LEVEL3_GAP });
					}
				}

			}
		}
	}
	if (_gameData.multiplayer)
	{
		_gameData.network->IngameStateUpdate(_gameData, &_gameData.levelState);
		_gameData.network->SynchronizeMob(_gameData, _gameData.levelState);
		_gameData.camera->GetParalax().Reload(_gameData);
	}
}

void Game::JoystickPressed(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::JoystickButtonPressed || _gameData.event.type == sf::Event::JoystickMoved)
	{
		if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::START))
			{
				_gameData.ChangeState(PAUSE);
				_gameData.swordSpriteMenu.setPosition(_gameData.startPosPauseSwordSprite);
			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::START))
			{
				_gameData.ChangeState(PAUSE);
				_gameData.swordSpriteMenu.setPosition(_gameData.startPosPauseSwordSprite);
			}
		}
	}
}

void Game::KeyPressed(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::KeyPressed && _gameData.event.key.code == sf::Keyboard::Escape)
	{
		_gameData.ChangeState(PAUSE);
		_gameData.swordSpriteMenu.setPosition(_gameData.startPosPauseSwordSprite);
	}
}

void Game::LostFocus(GameData& _gameData)
{

}

void Game::Init(GameData& _gameData)
{
	mapManager.InitPlayerAnim(_gameData);
}

const bool& Game::GetFadeOut(void) const
{
	return fade->GetIsFadingOut();
}

void Game::SetAlpha(const float _alpha)
{
	fade->SetAlpha(_alpha);
	fade->SetColor(sf::Color(0, 0, 0, (sf::Uint8)_alpha));
}

void Game::SetFadeOut(const bool _fadeOut)
{
	fade->SetIsFadingOut(_fadeOut);
}

void Game::Music(GameData& _gameData)
{
	if (_gameData.audio->GetSoundMultiplier() != 0)
	{
		if (_gameData.audio->GetMusicStatus("MENU") == sf::Sound::Status::Playing)
		{
			float volume = _gameData.audio->GetMusicVolume("MENU") - _gameData.dt * 10;
			if (volume < 0)
			{
				volume = 0;
				_gameData.audio->StopMusic("MENU");
			}
			_gameData.audio->SetMusicVolume("MENU", volume / _gameData.audio->GetSoundMultiplier());
		}
		if (!_gameData.tabMob.empty())
		{

			if (_gameData.audio->GetMusicStatus("INGAME") == sf::Sound::Status::Stopped)
			{

				_gameData.audio->PlayMusic("INGAME", true);
			}
			else
			{

				float volume = _gameData.audio->GetMusicVolume("INGAME") + _gameData.dt * 10;

				if (volume > 100)
				{

					volume = 100;
				}
				_gameData.audio->SetMusicVolume("INGAME", volume / _gameData.audio->GetSoundMultiplier());
			}
			if (_gameData.audio->GetMusicStatus("CHILL") == sf::Sound::Status::Playing)
			{
				float volume = _gameData.audio->GetMusicVolume("CHILL") - _gameData.dt * 10;

				if (volume < 0)
				{
					volume = 0;
					_gameData.audio->StopMusic("CHILL");
				}
				_gameData.audio->SetMusicVolume("CHILL", volume / _gameData.audio->GetSoundMultiplier());
			}
		}
		else
		{

			if (_gameData.audio->GetMusicStatus("INGAME") == sf::Sound::Status::Playing)
			{
				float volume = _gameData.audio->GetMusicVolume("INGAME");
				volume -= _gameData.dt * 10;
				if (volume < 0)
				{
					volume = 0;
					_gameData.audio->StopMusic("INGAME");
				}
				_gameData.audio->SetMusicVolume("INGAME", volume / _gameData.audio->GetSoundMultiplier());
			}
			if (_gameData.audio->GetMusicStatus("CHILL") != sf::Sound::Status::Playing)
			{

				float volume = _gameData.audio->GetMusicVolume("CHILL");
				volume = 0;
				_gameData.audio->PlayMusic("CHILL", true, volume);
			}
			else
			{
				if (_gameData.audio->GetMusicVolume("CHILL") < 100)
				{
					float volume = _gameData.audio->GetMusicVolume("CHILL") + _gameData.dt * 10;
					if (volume > 100)
					{
						volume = 100;
					}
					_gameData.audio->SetMusicVolume("CHILL", volume / _gameData.audio->GetSoundMultiplier());
				}
			}
		}
	}
}