#ifndef ANIMATION_H

#define ANIMATION_H
#include "Common.h"
#include "stdafx.h"

class Button;

class Animation
{
public:

	void CreateLeftToRight(sf::Sprite& _sprite, const unsigned char _totalFrameX, const unsigned char _totalFrameY, const sf::Vector2u _size);
	void CreateUpToDown(sf::Sprite& _sprite, const unsigned char _totalFrameX, const unsigned char _totalFrameY, const sf::Vector2u _size);
	void CreateUpToDownToUp(sf::Sprite& _sprite, const unsigned char _totalFrameX, const unsigned char _totalFrameY, const sf::Vector2u _size);
	void CreateLeftToRightMultipleLines(sf::Sprite& _sprite, const unsigned char _totalFrameX, const unsigned char _totalFrameY, const unsigned char _totalLines, const sf::Vector2u _size);

	void UpdateLeftToRight(sf::Sprite& _sprite, const float& _dt, const float _speed, const unsigned char _direction, bool& _isPlaying, const bool _isLooping);
	void UpdateRightToLeft(sf::Sprite& _sprite, const float& _dt, const float _speed, const unsigned char _direction, bool _isPlaying, const bool _isLooping);
	void UpdateUpToDown(const float& _dt, const float _speed, const unsigned char _location, bool& _isPlaying, const bool _isLooping);
	void UpdateUpToDownToUp(const float& _dt, const float _speed, const unsigned char _location, bool& _isPlaying, const bool _isLooping);
	void UpdateLeftToRightMultipleLines(const float& _dt, const float _speed, bool& _isPlaying, const bool _isLooping);
	void UpdateIsEntitySelected(const bool& _isSelected);

	unsigned char currentFrame = 0;
	unsigned char maxFrame = 0;
	float timer = 0.f;

	sf::Vector2u frameSize;
	sf::Sprite sprite;
	bool upToDownFinished = false;
	sf::IntRect rect;
	unsigned char linePlaying = 0;
	unsigned char maxLine = 0;
	float resetAnimTime = 0.f;
private:
	friend class Button;


};

#endif
