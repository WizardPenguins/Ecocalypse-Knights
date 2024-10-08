#ifndef GAMESTATE_H
#define GAMESTATE_H

#define SpeedFade 150
#define NbFunct 16

#include "stdafx.h"
#include "LoadingScreen.h"
#include"Screen.h"
#include"PreMenu.h"
#include"Pause.h"
#include"GameOver.h"
#include"Settings.h"
#include "Game.h"
#include "Menu.h"
#include "CharacterSelection.h"
#include "Arena.h"
#include "EntityManagerPVP.hpp"

class GameState
{
public:
	GameState(GameData& _gameData);
	virtual ~GameState();

	void Init(GameData& _gameData);
	void Reload(void);
	void ReloadNewGame(void);
	void Update(GameData& _gameData);
	void PollEvent(GameData& _gameData);
	void Display(GameData& _gameData);
	void Run(GameData& _gameData);
	void UpdateMultiThread(GameData& _gameData, LoadingScreen& gt);
	void SetLastState(GameData& _gameData);
	std::vector<States*>states;
private:
	void RunSplashScreen(GameData& _gameData);
	void RunLoadingScreen(GameData& _gameData, LoadingScreen& gt);
	void UpdateSplashScreen(float _dt);
	void DisplaySplashScreen(sf::RenderWindow& _window);
	void InitSplashScreen();
	void JoysticPressed();

	bool endOfLogo = false;
	bool onCrea = true;
	bool onWizard = false;
	bool fadeIn = true;
	bool fadeOut = false;

	float timerButton = 0.f;

	sf::Sprite creaLogo;
	sf::Sprite wizardLogo;

 	int currentFunct = 0;
};


#endif // ! GAMESTATE_H
