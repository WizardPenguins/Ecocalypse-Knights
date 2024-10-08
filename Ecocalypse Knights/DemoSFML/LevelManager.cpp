#include "stdafx.h"
#include "LevelManager.h"
#include "Mob.hpp"
#include "EntityManager.h"
#include "Scarecrow.hpp"
#include "Network.h"

LevelManager::LevelManager()
{
	this->nbMob = 0;
	this->scarecrow = false;
}

void LevelManager::Init(LevelState& _levelState, GameData& _gameData)
{
	InitMob(_gameData, _levelState, _gameData.actualWave);
}

void LevelManager::Update(GameData& _gameData, LevelState& _levelState)
{
	if (_gameData.reloadLevelManager)
	{
		Reload(_gameData);
		_gameData.reloadLevelManager = false;
	}
	if (_gameData.tabMob.size() == 0 && !_gameData.mobRespawn)
	{
		_gameData.inTravel = true;
	}
	else if (_gameData.tabMob.size() == 0 && _gameData.mobRespawn)
	{
		if (!_gameData.multiplayer)
		{
			CreateMob(_gameData);
		}
		else if (_gameData.network->state == NetworkState::HOST)
		{
			CreateMob(_gameData);
		}
	}
	for (int i = 0; i < _gameData.tabMob.size(); i++)
	{
		if (_gameData.tabMob[i]->GetType() == TypeMob::Boss && _gameData.tabMob[i]->GetState() == MobState::Death && !_gameData.mobRespawn)
		{
			_gameData.inTravel = true;
		}
		else if (_gameData.tabMob[i]->GetType() == TypeMob::Boss && _gameData.tabMob[i]->GetState() == MobState::Death && _gameData.mobRespawn)
		{
			_gameData.tabMob.clear();
			InitMob(_gameData, _levelState, _gameData.actualWave);
			_gameData.entityMan->waveLaunch = false;
			_gameData.inTravel = false;
			_gameData.mobRespawn = false;
		}
	}
}

void LevelManager::Display(sf::RenderWindow& _window, float _dt)
{
	//for (int i = 0; i < this->vectorCircle.size(); i++)
	//{
	//	UpdateCircleInvoc(_dt, this->vectorCircle[i]);
	//	_window.draw(this->vectorCircle[i].circleInvoc);
	//
	//	if (this->vectorCircle[i].dispawn)
	//	{
	//		this->vectorCircle.erase(this->vectorCircle.begin() + i);
	//		i--;
	//	}
	//}
}

void LevelManager::Display(sf::RenderTexture& _red, sf::RenderTexture& _blue, float _dt)
{
	//for (int i = 0; i < this->vectorCircle.size(); i++)
	//{
	//	UpdateCircleInvoc(_dt, this->vectorCircle[i]);
	//
	//	Utils::BlitSprite(this->vectorCircle[i].circleInvoc,sf::Vector2f{this->vectorCircle[i].circleInvoc.getPosition().x + (TROAD_GAP * (-1 * this->vectorCircle[i].circleInvoc.getPosition().y + WORKING_HEIGHT)/ 100.f),this->vectorCircle[i].circleInvoc.getPosition().y},0,_red);
	//	Utils::BlitSprite(this->vectorCircle[i].circleInvoc,sf::Vector2f{this->vectorCircle[i].circleInvoc.getPosition().x - (TROAD_GAP * (-1 * this->vectorCircle[i].circleInvoc.getPosition().y + WORKING_HEIGHT)/ 100.f),this->vectorCircle[i].circleInvoc.getPosition().y},0,_blue);
	//
	//	if (this->vectorCircle[i].dispawn)
	//	{
	//		this->vectorCircle.erase(this->vectorCircle.begin() + i);
	//		i--;
	//	}
	//}
}

void LevelManager::Reload(GameData& _gameData)
{
	_gameData.inTravel = false;
	_gameData.mobRespawn = true;
	_gameData.tabMob.clear();
	_gameData.entityMan->waveLaunch = false;
	this->nbMob = 0;
	this->scarecrow = false;
	this->wave6Count = 0;
}

void LevelManager::InitMobStat(std::fstream& _file, RaceMob _typeMob, GameData& _gameData)
{
	int nbMob; int gold; int xp; int life;
	_file >> nbMob >> gold >> xp >> life;

	int nbPlayer = _gameData.tabPlayers.size();
	if (_typeMob == RaceMob::Witch || _typeMob == RaceMob::Boar)
	{
		life *= nbPlayer;
	}
	else
	{
		nbMob *= nbPlayer;
	}

	for (int i = 0; i < nbMob; i++)
	{
		sf::Vector2f pos = sf::Vector2f{ (rand() % 590) + 1300.f + (WORKING_WIDTH * _gameData.actualWave), (rand() % 360) + 700.f };

		if (_typeMob == RaceMob::Witch || _typeMob == RaceMob::Boar)
		{
			pos = sf::Vector2f{ (rand() % 590) + 1300.f + (WORKING_WIDTH * _gameData.actualWave), (rand() % 360) + 700.f };
		}
		else
		{
			if (_gameData.levelState == LEVEL3 && (_gameData.actualWave == WAVE1 || _gameData.actualWave == WAVE3 || _gameData.actualWave == WAVE4))
			{
				pos.y += LEVEL3_GAP;
			}
			else if (_gameData.levelState == LEVEL3 && _gameData.actualWave == WAVE6 && !_gameData.tabPlayers[0]->GetFreeFall())
			{
				pos.y = 1525;
			}
			else if (_gameData.levelState == LEVEL3 && _gameData.actualWave == WAVE6 && _gameData.tabPlayers[0]->GetFreeFall())
			{
				pos.y += 2300;
			}
		}

		Mob* temp = new Mob{ _typeMob,_gameData.actualWave, *_gameData.audio, _gameData.mushroomSprite ,_gameData.pigSprite,_gameData.statusSprite,_gameData.boarSprite,_gameData.witchSprite,_gameData.hazelSprite,_gameData.birdSprite,pos};
		temp->SetGold(gold);
		temp->SetXp(xp);
		temp->SetLife(life);
		temp->SetMaxLife(life);
		temp->SetLastLife();

		if (_typeMob == RaceMob::Boar && this->level == "Level1")
		{
			temp->GetBoar()->SetLeak(true);
		}
		_gameData.tabMob.push_back(std::move(temp));
	}
}

void LevelManager::InitMobStat(std::fstream& _file, GameData& _gameData)
{
	int posX; int posY; int trash;
	_file >> posX >> posY >> trash >> trash;

	Scarecrow* temp = new Scarecrow{ *_gameData.scarecrowSprite, *_gameData.statusSprite,sf::Vector2f{(float)posX,(float)posY} };
	_gameData.scarecrow.push_back(std::move(temp));
}

void LevelManager::InitMob(GameData& _gameData, LevelState _level, Wave _wave)
{
	std::string	currentLevel = "Level" + std::to_string(_level - 1);
	std::string	currentWave = "Wave" + std::to_string(_wave + 1);
	if (_level == LEVEL3 && _wave == WAVE6)
	{
		currentWave = "Wave" + std::to_string(_wave + 1 + wave6Count);
		wave6Count++;
	}

	_gameData.tabMob.erase(_gameData.tabMob.begin(), _gameData.tabMob.end());

	std::fstream infile("Assets/MobStats/Mob.txt", std::ios::in);
	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string wave = ""; std::string type = "";
			infile >> level >> wave >> type;
			if (level == currentLevel && wave == currentWave)
			{
				if (type == "Bird")
				{
					InitMobStat(infile, RaceMob::Bird, _gameData);
				}
				else if (type == "HazelNut")
				{
					InitMobStat(infile, RaceMob::HazelNut, _gameData);
				}
				else if (type == "Boar")
				{
					InitMobStat(infile, RaceMob::Boar, _gameData);
				}
				else if (type == "Mushroom")
				{
					InitMobStat(infile, RaceMob::Mushroom, _gameData);
				}
				else if (type == "Scarecrow")
				{
					InitMobStat(infile, _gameData);
				}
				else if (type == "Witch")
				{
					InitMobStat(infile, RaceMob::Witch, _gameData);
				}
				else if (type == "Pig")
				{
					InitMobStat(infile, RaceMob::Pig, _gameData);
				}

			}
			else
			{
				int trash;
				infile >> trash >> trash >> trash >> trash;
			}
		}
	}
	infile.close();
}

void LevelManager::CreateCircle(GameData& _gameData)
{
	JeCompteCombienIlYADeMobAFaireSpawn(_gameData.levelState, _gameData.actualWave);
	this->nbMob *= _gameData.tabPlayers.size();
}

void LevelManager::CreateMobMulti(GameData& _gameData)
{
	InitMob(_gameData, _gameData.levelState, _gameData.actualWave);
	this->nbMob = 0;
	this->scarecrow = false;
}

void LevelManager::CreateMob(GameData& _gameData)
{
	if (!this->nbMob)
	{
		JeCompteCombienIlYADeMobAFaireSpawn(_gameData.levelState,_gameData.actualWave);
		if (!this->nbMob)
		{
			_gameData.inTravel = true;
		}
		this->nbMob *= _gameData.tabPlayers.size();
		InitMob(_gameData, _gameData.levelState, _gameData.actualWave);
		_gameData.inTravel = false;
		_gameData.mobRespawn = false;
		this->nbMob = 0;
		this->scarecrow = false;
	}
}

void LevelManager::UpdateCircleInvoc(float _dt, InvocCircle& _invoc)
{
	bool temp = true;
	_invoc.anim->UpdateLeftToRight(_invoc.circleInvoc, _dt, 5, 0, temp, true);

	if (_invoc.anim->currentFrame == _invoc.anim->maxFrame)
	{
		_invoc.dispawn = true;
	}
}

void LevelManager::InitCircle(sf::Vector2f _pos)
{
	InvocCircle* tempCircleInvoc = new InvocCircle;

	tempCircleInvoc->circleInvoc = this->circleInvoc;
	tempCircleInvoc->circleInvoc.setPosition(sf::Vector2f{ _pos.x,_pos.y + 50.f });
	tempCircleInvoc->circleInvoc.setScale(sf::Vector2f{0.75f,0.75f});
	tempCircleInvoc->pos = _pos;
	tempCircleInvoc->anim = new Animation;
	sf::IntRect rect = sf::IntRect(0, 0, (int)(2610 / 10), (int)(358));
	tempCircleInvoc->anim->CreateLeftToRight(tempCircleInvoc->circleInvoc, 10, 1, { 2610,358 });
	tempCircleInvoc->circleInvoc.setTextureRect(rect);
	tempCircleInvoc->anim->currentFrame = 0;
	tempCircleInvoc->dispawn = false;

	this->vectorCircle.push_back(*tempCircleInvoc);
}

void LevelManager::JeCompteCombienIlYADeMobAFaireSpawn(LevelState _level, Wave _wave)
{
	std::string	currentLevel = "Level" + std::to_string(_level - 1);
	std::string	currentWave = "Wave" + std::to_string(_wave + 1);
	if (_level == LEVEL3 && _wave == WAVE6)
	{
		currentWave = "Wave" + std::to_string(_wave + 1 + wave6Count);
	}

	std::fstream infile("Assets/MobStats/Mob.txt", std::ios::in);
	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string level = ""; std::string wave = ""; std::string mob = ""; int trash;
			infile >> level >> wave >> mob >> trash;
			if (level == currentLevel && wave == currentWave)
			{
				if (mob == "Scarecrow" || mob == "Boar" || mob == "Witch")
				{
					this->scarecrow = true;
				}
				else
				{
					this->nbMob += trash;
				}
			}
			infile >> trash >> trash >> trash;
		}
	}

	infile.close();
}