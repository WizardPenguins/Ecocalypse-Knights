#include "stdafx.h"
#include "Fade.h"

Fade::Fade()
{
}

Fade::Fade(const float _timerMax, const bool _isFadingIn, const bool _isFadingOut)
{
	isFadingIn = _isFadingIn;
	isFadingOut = _isFadingOut;

	timerMin = 0.f;
	timerMax = _timerMax;

	alphaMin = 0.f;
	alphaMax = 255.f;

	maxVolume = 0.f;

	if (_isFadingIn)
	{
		timer = timerMin;
		alpha = alphaMin;
	}
	else
	{
		timer = timerMax;
		alpha = alphaMax;
	}


	color = sf::Color(0, 0, 0, (sf::Uint8)alpha);
	Utils::CreateRectangleShape(rect, sf::Vector2f(0.f, 0.f), Screen::Pos(WORKING_WIDTH,WORKING_HEIGHT*2), color, false);
}

Fade::Fade(const float _timerMax, const float _maxVolume, const bool _isFadingIn, const bool _isFadingOut)
{
	isFadingIn = _isFadingIn;
	isFadingOut = _isFadingOut;

	timerMin = 0.f;
	timerMax = _timerMax;

	alphaMin = 0.f;
	alphaMax = 255.f;

	maxVolume = _maxVolume;

	if (_isFadingIn)
	{
		timer = timerMin;
		alpha = alphaMin;
	}
	else
	{
		timer = timerMax;
		alpha = alphaMax;
	}

	color = sf::Color(0, 0, 0, (sf::Uint8)alpha);
	Utils::CreateRectangleShape(rect, sf::Vector2f(0.f, 0.f), Screen::Pos(WORKING_WIDTH, WORKING_HEIGHT*3), color, false);
}

Fade::~Fade()
{
}

void Fade::Update(const float& _dt)
{
	if (isFadingOut)
	{
		timer = timer - _dt;
		alpha = fmaxf((timer / timerMax) * alphaMax, alphaMin);
		color = sf::Color(0, 0, 0, (sf::Uint8)alpha);
		//std::cout << alpha << std::endl;
		rect.setFillColor(color);

		if (timer <= timerMin)
		{
			isFadingOut = false;
		}
	}
	else if (isFadingIn)
	{
		timer = timer + _dt;
		alpha = fminf((timer / timerMax) * alphaMax, alphaMax);

		color = sf::Color(0, 0, 0, (sf::Uint8)alpha);
		rect.setFillColor(color);

		if (timer >= timerMax)
		{
			isFadingIn = false;
		}
	}
}

//void Fade::Update(GameData& _gameData, const std::string& _musicKey)
//{
//	if (isFadingOut)
//	{
//		//std::cout << "out" << std::endl;
//		timer = timer - _gameData.dt;
//		alpha = fmaxf((timer / timerMax) * alphaMax, alphaMin);
//		color = sf::Color(0, 0, 0, (sf::Uint8)alpha);
//		rect.setFillColor(color);
//
//		//_gameData.audio->SetMusicVolume(_musicKey, fmaxf(((timerMax - timer) / timerMax) * (maxVolume * _gameData.audio->GetSoundMultiplier()), 0.f));
//
//		if (timer <= timerMin)
//		{
//			isFadingOut = false;
//			
//		}
//	}
//	else if (isFadingIn)
//	{
//		//std::cout << "in" << std::endl;
//		timer = timer + _gameData.dt;
//		alpha = fminf((timer / timerMax) * alphaMax, alphaMax);
//		//std::cout << alpha << std::endl;
//		color = sf::Color(0, 0, 0, (sf::Uint8)alpha);
//		rect.setFillColor(color);
//
//		//_gameData.audio->SetMusicVolume(_musicKey, fminf(((timerMax - timer) / timerMax) * (maxVolume * _gameData.audio->GetSoundMultiplier()), maxVolume * _gameData.audio->GetSoundMultiplier()));
//
//		if (timer >= timerMax)
//		{
//			isFadingIn = false;
//			_gameData.audio->StopMusic(_musicKey);
//		}
//	}
//
//}

void Fade::Display(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	_red.draw(rect);
	_blue.draw(rect);
}

void Fade::Display(sf::RenderWindow& _window)
{
	_window.draw(rect);
}

void Fade::SetIsFadingIn(const bool _bool)
{
	isFadingIn = _bool;
	isFadingOut = !_bool;
}

void Fade::SetIsFadingOut(const bool _bool)
{
	isFadingOut = _bool;
	isFadingIn = !_bool;
}

void Fade::SetAlpha(const float _alpha)
{
	if (_alpha >= alphaMin && _alpha <= alphaMax)
	{
		alpha = _alpha;
	}
}

void Fade::SetColor(const sf::Color _color)
{
	color = _color;
	rect.setFillColor(color);
}

void Fade::ResetTimer(void)
{
	timer = timerMin;
}

const float& Fade::GetTimer(void) const
{
	return timer;
}

float& Fade::GetTimerMax(void) 
{
	return timerMax;
}

const float& Fade::GetTimerMin(void) const
{
	return timerMin;
}

const bool& Fade::GetIsFadingIn(void) const
{
	return isFadingIn;
}

const bool& Fade::GetIsFadingOut(void) const
{
	return isFadingOut;
}

void Fade::SetTimer(float& _timer)
{
	timer = _timer;
}