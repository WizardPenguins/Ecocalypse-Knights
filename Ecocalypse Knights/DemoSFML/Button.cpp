#include "stdafx.h"
#include "Button.h"

Button::~Button()
{
	delete texture;
	texture = nullptr;
}

Button::Button(std::string _file, sf::Vector2f _pos, unsigned char _nbFramesX, sf::Vector2f _scale, bool _canBreak, bool _isCenter)
{
	if (_isCenter)
	{
		sprite = Utils::LoadSprite(_file, CENTER, _nbFramesX);
	}
	else
	{
		sprite = Utils::LoadSprite(_file, TOP_LEFT, _nbFramesX);
	}
	animation.CreateLeftToRight(sprite, _nbFramesX, 1, sprite.getTexture()->getSize());

	sf::Vector2u size = sprite.getTexture()->getSize();
	size.x = (unsigned int)(((float)size.x / (float)_nbFramesX) * _scale.x);
	size.y = (unsigned int)(size.y * _scale.y);

	sprite.setScale(_scale);
	posButton = { _pos.x,_pos.y };
	sprite.setPosition(posButton);

	canBreak = _canBreak;

	isSelected = false;
	isPressed = false;
}

void Button::UpdateAnimButton(const float& _dt)
{
	if (isSelected && !isPressed)
	{
		animation.currentFrame = 1;
	}
	else if (isPressed && !isPressedFinished)
	{
		isSelected = false;
		if (!canBreak)
		{
			animation.currentFrame = 2;
			animation.timer += _dt;
			if (animation.timer >= animation.resetAnimTime)
			{
				animation.timer = 0.f;
				animation.currentFrame = 0;
				isPressed = false;
				isPressedFinished = true;
			}
		}
		else
		{
			animation.timer += _dt;
			if (animation.timer >= 0.035f && animation.currentFrame < 6)
			{
				animation.currentFrame++;
				animation.timer = 0.f;
			}
			if (animation.currentFrame == 6 && animation.timer >0.15f)
			{
				animation.timer = 0.f;
				isPressedFinished = true;
			}
		}
	}
	else if (isPressed && !isPressedFinished)
	{
		animation.currentFrame = 6;
	}
	else
	{
		animation.currentFrame = 0;
	}

	animation.rect = sf::IntRect(animation.currentFrame * animation.frameSize.x, 0, animation.frameSize.x, animation.frameSize.y);
	sprite.setTextureRect(animation.rect);
}

const bool& Button::GetIsPressed(void) const
{
	return isPressed;
}

const bool& Button::GetIsSelected(void) const
{
	return isSelected;
}

void Button::SetTexture(std::string _file, sf::Vector2f _pos, unsigned char _nbFramesX, sf::Vector2f _scale, bool _canBreak, bool _isCenter)
{
	if (_isCenter)
	{
		sprite = Utils::LoadSprite(_file, CENTER, _nbFramesX);
	}
	else
	{
		sprite = Utils::LoadSprite(_file, TOP_LEFT, _nbFramesX);
	}
	animation.CreateLeftToRight(sprite, _nbFramesX, 1, sprite.getTexture()->getSize());

	sf::Vector2u size = sprite.getTexture()->getSize();
	size.x = (unsigned int)(((float)size.x / (float)_nbFramesX) * _scale.x);
	size.y = (unsigned int)(size.y * _scale.y);

	sprite.setScale(_scale);
	posButton = { _pos.x,_pos.y };
	sprite.setPosition(posButton);

	canBreak = _canBreak;

	isSelected = true;
	isPressed = false;
}

void Button::SetIsPressed(const bool _bool, GameData& _gameData)
{
	//if (!isPressed && _bool == true)
	//{
	//	_gameData.audio->PlaySoundNotRepeated("BUTTON_PRESSED", 50.f);
	//}
	isPressed = _bool;
}

void Button::SetIsSelected(const bool _bool, GameData& _gameData)
{
	//if (!isSelected && _bool == true && _gameData.gameState != PREMENU)
	//{
	//	_gameData.audio->PlaySoundNotRepeated("BUTTON_SELECTED", 10.f);
	//}
	isSelected = _bool;
}

sf::Vector2f Button::getPosition()
{
	return posButton;
}
