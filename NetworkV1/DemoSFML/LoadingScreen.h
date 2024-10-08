#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H
#include "stdafx.h"

#include "Common.h"
#include "Animation.h"

constexpr float NB_INIT = 16.f;

class LoadingScreen
{
public:
	LoadingScreen(GameData& _gameData, std::vector<States*>& _stateRef) : gameDataRef(_gameData), statesRef(_stateRef)
	{
		InitLoadingScreen();
	}

	void InitLoadingScreen(void);
	void UpdateAnim(sf::RenderWindow& _window);
	void DisplayLoadingScreen();
	void LoadSound(Audio& _audio);
	float compteur = 0;
	bool loadingScreenFinish = false;
	float loadingScreenCompteur;
	sf::RectangleShape backChargingBar;
	sf::RectangleShape frontChargingBar;
	std::string funnyStringList[6];
	sf::Text funnySentence;

	sf::Font GetFont();
private:

	sf::Font font;
	sf::Text text;
	sf::Sprite animeSprite;
	sf::Sprite background;
	Animation anime;

	GameData& gameDataRef;
	std::vector<States*>& statesRef;

	
	float timer = 0;
};

#endif // !LOADINGSCREEN_H