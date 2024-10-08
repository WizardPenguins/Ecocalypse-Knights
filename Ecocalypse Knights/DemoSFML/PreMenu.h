#ifndef PREMENU_H
#define PREMENU_H

#include "Common.h"
#include "States.h"
#include "Screen.h"
#include "stdafx.h"

class PreMenu : public States
{
public:
	PreMenu();
	~PreMenu();

	void Update(GameData& _gameData) override;
	void Display(GameData& _gameData) override;
	void KeyPressed(GameData& _gameData) override;
	void JoystickPressed(GameData& _gameData) override;
	
private:
	void InitComponents(void);
	void FadeManagement(GameData& _gameData);
	void SkipEffect(GameData& _gameData);

	sf::Sprite sprite;
	sf::Texture texture;
	sf::Texture textureLogo;

	bool isOnLogo;
	float timerSkip = 0.f;
};

#endif


