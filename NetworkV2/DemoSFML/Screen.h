#ifndef SCREEN_H

#define SCREEN_H 

#include "Common.h"
#include "stdafx.h"

class Screen
{
public:

	static Screen& GetInstance()
	{
		static Screen myInstance;
		return myInstance;
	}

	static void Init(sf::RenderWindow& _window);
	static sf::Vector2f Scale(const sf::Vector2f& _scale);
	static sf::Vector2f Scale(const float _scaleX, const float _scaleY);
	static sf::Vector2f Pos(const sf::Vector2f& _pos);
	static sf::Vector2f Pos(const float _x, const float _y);
	static float PosX(const float _x);
	static float PosY(const float _y);

	static const float& GetWidth(void);
	static const float& GetHeight(void);
private:

	float width = 0.f;
	float height = 0.f;
};
#endif // !SCREEN_H
