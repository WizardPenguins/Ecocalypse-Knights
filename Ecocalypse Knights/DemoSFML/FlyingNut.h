#ifndef FLYINGNUT_HPP
#define FLYINGNUT_HPP

#include "stdafx.h"
#include "Mob.hpp"
#include "Player.hpp"
#include "Animation.h"

class FlyingNut
{
public :
	FlyingNut();
	void Reload(GameData& _gameData);
	void Update(GameData& _gameData);
	void Display(GameData& _gameData);

	void Display3D(GameData& _gameData);

private :
	sf::Sprite spriteNut;
	sf::Vector2f flyingNutPos;

	sf::Vector2f startPosEasing;
	sf::Vector2f startPosEasing2;
	sf::Vector2f endPosEasing;
	sf::Vector2f endPosEasing2;
	sf::Sprite spriteGrass;
	sf::Sprite starSprite;
	Animation grassAnime;
	bool hisLunched = false;
	bool showStar = false;
	float timer;
	float timer2;
	float angleNut = 0.f;
	sf::RectangleShape* hitbox;

};


#endif // !FLYINGNUT_HPP
