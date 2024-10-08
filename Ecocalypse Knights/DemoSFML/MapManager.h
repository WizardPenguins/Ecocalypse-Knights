#ifndef MAPMANAGER_H

#define MAPMANAGER_H
#include "Common.h"
#include "Player.hpp"
#include "stdafx.h"


enum PlayerGoOn
{
	NONEMAP,
	MAP0,
	MAP1,
	MAP2,
	MAP3,
	MAP4,
	MAP5,
	MAP6,
	MAP7,
	MAP8,
	MAP9,
	MAP10,
};

class MapManager
{
public:
	void Init(GameData& _gameData);

	void Update(GameData& _gameData, bool _level1IsDone, bool _level2IsDone);
	void PlayerChooseLevel(GameData& _gameData, bool _level2IsDone);
	void PlayerMove(GameData& _gameData);
	void PopUpPositionUpdate(GameData& _gameData);
	const char SetLevel(GameData& _gameData);
	void UpdateCamera(GameData& _gameData);
	void Display(GameData& _gameData,bool _level2);
	void Display3D(GameData& _gameData, bool _level2);
	void InitPlayerAnim(GameData& _gameData);
	void Reload(GameData& _gameData);
	void ReloadForGameOver(GameData& _gameData);

private:
	sf::Sprite backGroundLevelSelector;
	sf::Sprite backGroundDuBackGroundLevelSelector;
	sf::Sprite player;

	sf::Sprite caravan;
	Animation* caravanAnim;

	sf::Sprite locked;

	sf::Sprite level2;
	Animation* level2Anim;

	sf::Sprite level3;
	Animation* level3Anim;

	sf::Sprite boar;
	Animation* boarAnim;	
	
	sf::Sprite witch;
	Animation* witchAnim;

	sf::Sprite travelLevel2;
	Animation* travelLevel2Anim;

	sf::Sprite travelCaravan;
	Animation* travelCaravanAnim;

	sf::Sprite travelLevel3;
	Animation* travelLevel3Anim;

	//sf::Sprite popUp;
	sf::Font font;
	//sf::Text text;
	//std::string string;
	std::vector <sf::Vector2f> posLevel;
	std::vector<sf::Sprite> nameLevel;
	PlayerGoOn playerGoOn = NONEMAP;
	PlayerGoOn playerIsOn = NONEMAP;
	float timer = 0.f;

	sf::Sprite playerRun[4];
	Animation* playerRunAnim[4];

	sf::Sprite playerIdle[4];
	Animation* playerIdleAnim[4];
	Direction playerDirection;

};

#endif
