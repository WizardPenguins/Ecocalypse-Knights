#include "stdafx.h"
#include "TroaDimmenssionSahMaire.h"
#include "Utils.h"
//#include "Player.hpp"
#include "EntityManager.h"


TroaDimmenssionSahMaire::TroaDimmenssionSahMaire()
{
	//blue.loadFromFile("Assets/Shader/grisbleu.frag", sf::Shader::Fragment);
	//red.loadFromFile("Assets/Shader/grisrouge.frag", sf::Shader::Fragment);

	leftEye = new sf::Texture;
	rightEye = new sf::Texture;
}

void TroaDimmenssionSahMaire::TakeSreenRedFilter(GameData& _gameData)
{
#pragma warning(suppress : 4996)
		leftEye->loadFromImage(_gameData.window.capture());
}

void TroaDimmenssionSahMaire::TakeSreenBlueFilter(GameData& _gameData)
{
#pragma warning(suppress : 4996)
		rightEye->loadFromImage(_gameData.window.capture());
}

void TroaDimmenssionSahMaire::DisplayFilter(GameData& _gameData)
{
	sf::Sprite tempSprite;
	tempSprite.setTexture(*leftEye);

	sf::Sprite tempSprite2;
	tempSprite2.setTexture(*rightEye);
	
	tempSprite.setColor({ (sf::Uint8)(tempSprite.getColor().r * 0.299f),(sf::Uint8)(tempSprite.getColor().g * 0.587f),(sf::Uint8)(tempSprite.getColor().b * 0.114f )});
	tempSprite.setColor({ (sf::Uint8)(tempSprite.getColor().r * 255),(sf::Uint8)(tempSprite.getColor().g * 0),(sf::Uint8)(tempSprite.getColor().b * 0),255 });
	_gameData.window.draw(tempSprite,sf::BlendAdd);


	tempSprite2.setColor({ (sf::Uint8)(tempSprite2.getColor().r * 0.299f),(sf::Uint8)(tempSprite2.getColor().g * 0.587f),(sf::Uint8)(tempSprite2.getColor().b * 0.114f) });
	tempSprite2.setColor({ (sf::Uint8)(tempSprite2.getColor().r * 0),(sf::Uint8)(tempSprite2.getColor().g * 255),(sf::Uint8)(tempSprite2.getColor().b * 255),255 });
	_gameData.window.draw(tempSprite2, sf::BlendAdd);
	////Utils::BlitSprite(*tempSprite, {0, 0 }, _gameData.window, red,0, sf::BlendAdd);
	//Utils::BlitSprite(*tempSprite2, { 0, 0 }, _gameData.window, blue,0, sf::BlendAdd);


}

