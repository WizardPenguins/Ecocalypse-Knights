#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "stdafx.h"
#include "Animation.h"
#include "Common.h"
#include "MobSprite.hpp"

struct InvocCircle
{
	Animation* anim;
	sf::Sprite circleInvoc;
	sf::Vector2f pos;
	bool dispawn;
};

class LevelManager
{
public:

	//Constructor/Destructor
	LevelManager(sf::Sprite _circleInvoc);
	~LevelManager() = default;

	//Function
	void Init(LevelState& _levelState, GameData& _gameData);
	void Update(GameData& _gameData, LevelState& _levelState);

	void Display(sf::RenderWindow& _window,float _dt);
	void Display(sf::RenderTexture& _red, sf::RenderTexture& _blue,float _dt);

	void Reload(GameData& _gameData);

	void CreateMob(GameData& _gameData);
	void CreateMobMulti(GameData& _gameData);
	void CreateCircle(GameData& _gameData);
	void InitMob(GameData& _gameData, LevelState _level, Wave _wave);

	std::vector<InvocCircle> vectorCircle;
private:
	void InitMobStat(std::fstream& _file, RaceMob _typeMob, GameData& _gameData);
	void InitMobStat(std::fstream& _file, GameData& _gameData);
	void UpdateCircleInvoc(float _dt, InvocCircle& _invoc);
	void InitCircle(sf::Vector2f _pos);
	void JeCompteCombienIlYADeMobAFaireSpawn(LevelState _level,Wave _wave);

	sf::Sprite circleInvoc;
	int nbMob;
	bool scarecrow;
	std::string level = "";
};

#endif