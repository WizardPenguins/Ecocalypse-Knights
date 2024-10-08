#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include "stdafx.h"
#include "States.h"
#include "LevelManager.h"

enum class ButtonTypeMulti
{
	HOST,
	CLIENT,
};

enum class ButtonTypeMultiWait
{
	BACK,
	CONFIRM,
};

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
	void InitWaitButton(GameData& _gameData);
	void InitSwordVector(void);
	void JoystickHandler(GameData& _gameData);
	void JoystickPressed(GameData& _gameData) override;
	void UpdateButtonPressedEffect(GameData& _gameData);
	void UpdateSwordPos(float _dt, sf::Sprite& _swordSprite);

	void SetAlpha(const float _alpha);

	int lastPos;
	int nextPos;

	float timerEasing;
	float timerEasing2 = 0.f;
	float timerEasingButton;
	bool movingFront;
	bool swordHasSwoosh;
	bool isSwifting = false;

	sf::Vector2f swordCurrentPos;
	std::vector<sf::Vector2f> swordsPos;
	std::vector<sf::Vector2f> swordsPosMulti;

	std::vector<sf::Vector2f> startPosEasing;
	std::vector<sf::Vector2f> startPosEasingMulti;
	std::vector<sf::Vector2f> endPosEasing;
	std::vector<sf::Vector2f> endPosEasingMulti;

private:

	void InitComponents(GameData& _gameData);
	void FadeManagement(GameData& _gameData);

	std::string musicKey;
	int buttonPressSound;
	int buttonSelectSound;

	bool reload;
};
#endif