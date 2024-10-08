#include "stdafx.h"
#include "Screen.h"

void Screen::Init(sf::RenderWindow& _window)
{
	GetInstance().width = (float)_window.getSize().x;
	GetInstance().height = (float)_window.getSize().y;
}

sf::Vector2f Screen::Scale(const sf::Vector2f& _scale)
{
	return sf::Vector2f
	(
		(float)(GetInstance().width / WORKING_WIDTH) * _scale.x,
		(float)(GetInstance().height / WORKING_HEIGHT) * _scale.y
	);
}

sf::Vector2f Screen::Scale(const float _scaleX, const float _scaleY)
{
	return sf::Vector2f
	(
		(float)(GetInstance().width / WORKING_WIDTH) * _scaleX,
		(float)(GetInstance().height / WORKING_HEIGHT) * _scaleY
	);
}

sf::Vector2f Screen::Pos(const sf::Vector2f& _pos)
{
	return sf::Vector2f
	(
		(_pos.x / WORKING_WIDTH) * GetInstance().width,
		(_pos.y / WORKING_HEIGHT) * GetInstance().height
	);
}

sf::Vector2f Screen::Pos(const float _x, const float _y)
{
	return sf::Vector2f
	(
		(_x / WORKING_WIDTH) * GetInstance().width,
		(_y / WORKING_HEIGHT) * GetInstance().height
	);
}

float Screen::PosX(const float _x)
{
	return (_x / WORKING_WIDTH) * GetInstance().width;
}

float Screen::PosY(const float _y)
{
	return (_y / WORKING_HEIGHT) * GetInstance().height;
}

const float& Screen::GetWidth(void)
{
	return GetInstance().width;
}

const float& Screen::GetHeight(void)
{
	return GetInstance().height;
}