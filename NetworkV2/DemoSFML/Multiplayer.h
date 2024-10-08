#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include "stdafx.h"
#include "States.h"
#include "LevelManager.h"

class Multiplayer : public States
{
public:
	Multiplayer(GameData& _gameData);
	~Multiplayer();

	void Reload();
	void Update(GameData& _gameData) override;
	void Display(GameData& _gameData) override;
	void Display3D(GameData& _gameData) override;
	void KeyPressed(GameData& _gameData) override;
	void MenuStateUpdate(GameData& _gameData);
	void InGameStateUpdate(GameData& _gameData);
	void MenuStateDisplay(GameData& _gameData);

	void SetAlpha(const float _alpha);

private:

	void InitComponents(GameData& _gameData);
	void FadeManagement(GameData& _gameData);

	std::string musicKey;
	int buttonPressSound;
	int buttonSelectSound;

	bool reload;
};
#endif