#ifndef FADE_H
#define FADE_H

#include"Common.h"
#include"Screen.h"
#include"Audio.h"
#include "stdafx.h"

class Fade
{
public:

	Fade();
	Fade(const float _timerMax, const bool _isFadingIn, const bool _isFadingOut);
	Fade(const float _timerMax, const float _maxVolume, const bool _isFadingIn, const bool _isFadingOut);
	~Fade();

	void Update(const float& _dt);
	//void Update(GameData& _gameData, const std::string& _musicKey);

	void Display(sf::RenderTexture& _red, sf::RenderTexture& _blue);

	void Display(sf::RenderWindow& _window);

	void SetIsFadingIn(const bool _bool);
	void SetIsFadingOut(const bool _bool);
	void SetAlpha(const float _alpha);
	void SetColor(const sf::Color _color);
	void ResetTimer(void);

	const float& GetTimer(void) const;
	float& GetTimerMax(void);
	const float& GetTimerMin(void) const;
	const bool& GetIsFadingIn(void) const;
	const bool& GetIsFadingOut(void) const;

	void SetTimer(float& _timer);

private:

	bool isFadingIn = false;
	bool isFadingOut = false;

	float timerMin = 0.f;
	float timerMax = 2.f;
	float timer = 0.f;

	float alphaMin = 0.f;
	float alphaMax = 0.f;
	float alpha = 0.f;

	float maxVolume = 0.f;

	sf::Color color;
	sf::RectangleShape rect;
};

#endif // !FADE_H



