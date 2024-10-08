#ifndef BUTTON_H
#define BUTTON_H

#include "Common.h"
#include "Animation.h"
#include "Audio.h"
#include "stdafx.h"

class Button
{
public:
	Button(std::string _file, sf::Vector2f _pos, unsigned char _nbFramesX = 3, sf::Vector2f _scale = sf::Vector2f(1.f, 1.f), bool _canBreak = false, bool _isCentered = true);
	Button();
	virtual ~Button();

	void UpdateAnimButton(const float& _dt);

	const bool& GetIsPressed(void) const;
	const bool& GetIsSelected(void) const;

	void SetIsPressed(const bool _bool, GameData& _gameData);
	void SetIsSelected(const bool _bool, GameData& _gameData);

	void SetTexture(std::string _file, sf::Vector2f _pos, unsigned char _nbFramesX = 3, sf::Vector2f _scale = sf::Vector2f(1.f, 1.f), bool _canBreak = false, bool _isCenter = true);
	
	sf::Vector2f getPosition();

	Animation animation;
	sf::Sprite sprite;
	sf::Vector2f posButton;
	bool isPressedFinished = false;
private:
	bool isSelected = false;
	bool isPressed = false;
	bool canBreak;
	sf::Texture* texture = nullptr;
};

#endif // !BUTTON_H



