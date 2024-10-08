#ifndef POPUP_H
#define POPUP_H

#include "Common.h"
#include "Easings.h"
#include "stdafx.h"

constexpr float TIMER_INTER_SPAWN = 2.f;
constexpr float TIMER_WAIT = 5.f;
constexpr unsigned short int NB_PANEL_TUTO = 9;
constexpr unsigned short int NB_PANEL_AOE = 2;
constexpr unsigned short int NB_PANEL_POTION = 1;

class PopUp
{
public:
	PopUp(int _number, sf::Sprite& _sprite);

	void Update(float _dt, int& _currentPopUp);
	void Draw(sf::RenderWindow& _window);
	void Draw(sf::RenderTexture& _red, sf::RenderTexture& _blue);

private:
	sf::Sprite panel;

	sf::Vector2f startPosBackGround;
	sf::Vector2f endPosBackGround;

	bool textIsSpawning = false;
	bool textAsSpawning = false;
	bool textIsDespawning = false;
	bool textAsDespawning = false;

	float timerSpawning = 0.f;
	float timerDespawning = 0.f;
	float timerWaitOnScreen = TIMER_WAIT;
	float timerBeforeSpawning = TIMER_INTER_SPAWN;
};

#endif // !POPUP_H