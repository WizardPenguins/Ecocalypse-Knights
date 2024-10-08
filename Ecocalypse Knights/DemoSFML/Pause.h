#ifndef PAUSE_H
#define PAUSE_H
#include "stdafx.h"


#include"States.h"
#include "Animation.h"
#include "Audio.h"
#include "Easings.h"

enum ButtonTypePause
{
	CONTINUE,
	SETTINGS_PAUSE,
	MAIN_MENU,
	QUIT_PAUSE
};

enum class ButtonTypePauseMulti
{
	CONTINUE,
	MAIN_MENU,
	QUIT
};

class Pause : public States
{
public:
	Pause(GameData& _gameData);
	~Pause();

	virtual void Reload(GameData& _gameData);
	void UpdatePos(GameData& _gameData) override;
	void UpdateButtonPressedEffect(GameData& _gameData) override;
	void Update(GameData& _gameData) override;
	void Display(GameData& _gameData) override;
	void Display3D(GameData& _gameData) override ;
	void KeyPressed(GameData& _gameData) override;
	void JoystickPressed(GameData& _gameData)override;
	void JoystickPressedMulti(GameData& _gameData);
	void UpdateButtonPressedEffectMulti(GameData& _gameData);


private:
	void InitComponents(GameData& _gameData);

	void UpdateSwordPos(float _dt, sf::Sprite& _swordSprite);
	void UpdateSwordPosMulti(float _dt, sf::Sprite& _swordSprite);

	int lastPos = CONTINUE;
	int nextPos = CONTINUE;
	bool isSwifting = false;
	float timerEasing;
	sf::Vector2f swordCurrentPos;
	std::vector<sf::Vector2f> swordsPos;
	std::vector<sf::Vector2f> swordsPosMulti;

	std::vector<sf::Vector2f> startPosEasing;
	std::vector<sf::Vector2f> startPosEasingMulti;
	std::vector<sf::Vector2f> endPosEasing;
	std::vector<sf::Vector2f> endPosEasingMulti;
	float timerEasing2 = 0.f;
	bool movingFront;
};
#endif // !PAUSE_H


