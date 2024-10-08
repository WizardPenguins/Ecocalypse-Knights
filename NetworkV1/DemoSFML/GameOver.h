#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "stdafx.h"

#include"States.h"
#include"Animation.h"
#include"Audio.h"

#define CREDITS_BG_HEIGHT 4671

enum ButtonTypeGameOver
{
	MAIN_MENU_GAME_OVER
};

class GameOver : public States
{
public:
	GameOver(GameData& _gameData);
	~GameOver();

	void UpdateButtonPressedEffect(GameData& _gameData);
	void Update(GameData& _gameData) override;
	void Display(GameData& _gameData) override;
	void Display3D(GameData& _gameData) override ;
	void KeyPressed(GameData& _gameData) override;
	void JoystickPressed(GameData& _gameData)override;
	void Reload(GameData& _gameData);

private:
	void InitComponents(GameData& _gameData);
	void InitButtons(GameData& _gameData);
	void InitBackground(void);
	void FadeManagement(GameData& _gameData);

	sf::Sprite backgroundWin;
	sf::Sprite backgroundLoose;

	std::string musicKey;
	sf::Text textWin;

	float timerCredits = 0.f;
	sf::Vector2f posCredits;
	Fade* fade = nullptr;
};
#endif // !GAMEOVER_H
