#include "stdafx.h"
#include "Animation.h"

void Animation::CreateLeftToRight(sf::Sprite& _sprite, const unsigned char _totalFrameX, const unsigned char _totalFrameY, const sf::Vector2u _size)
{
	maxFrame = _totalFrameX - 1;
	timer = 0.f;
	sprite = _sprite;
	frameSize.x = _size.x / _totalFrameX;
	frameSize.y = _size.y / _totalFrameY;
	currentFrame = 0;
	resetAnimTime = 0.2f;
	rect = sf::IntRect(0, 0, frameSize.x, frameSize.y);
}

void Animation::CreateUpToDown(sf::Sprite& _sprite, const unsigned char _totalFrameX, const unsigned char _totalFrameY, const sf::Vector2u _size)
{
	maxFrame = _totalFrameY - 1;
	timer = 0.f;
	frameSize.x = _size.x / _totalFrameX;
	frameSize.y = _size.y / _totalFrameY;
	currentFrame = 0;
	rect = sf::IntRect(0, 0, frameSize.x, frameSize.y);
}

void Animation::CreateUpToDownToUp(sf::Sprite& _sprite, const unsigned char _totalFrameX, const unsigned char _totalFrameY, const sf::Vector2u _size)
{
	maxFrame = _totalFrameY - 1;
	timer = 0.f;
	frameSize.x = _size.x / _totalFrameX;
	frameSize.y = _size.y / _totalFrameY;
	currentFrame = 0;
	upToDownFinished = false;
	rect = sf::IntRect(0, 0, frameSize.x, frameSize.y);
}

void Animation::CreateLeftToRightMultipleLines(sf::Sprite& _sprite, const unsigned char _totalFrameX, const unsigned char _totalFrameY, const unsigned char _totalLines, const sf::Vector2u _size)
{
	maxFrame = _totalFrameX - 1;
	timer = 0;
	frameSize.x = _size.x / _totalFrameX;
	frameSize.y = _size.y / _totalFrameY;
	currentFrame = 0;
	linePlaying = 0;
	maxLine = _totalLines - 1;
	rect = sf::IntRect(0, 0, frameSize.x, frameSize.y);
}

void Animation::UpdateLeftToRight(sf::Sprite& _sprite,const float& _dt, const float _speed, const unsigned char _direction, bool& _isPlaying, const bool _isLooping)
{
	if (_isPlaying)
	{
		timer += _dt;
		if (timer > 1 / _speed)
		{
			currentFrame++;
			timer = 0;
		}
		if (currentFrame > maxFrame)
		{
			if (_isLooping)
			{
				currentFrame = 0;
			}
			else
			{
				_isPlaying = false;
				currentFrame = maxFrame;
				timer = 0;
			}
		}
	}

	rect = sf::IntRect(currentFrame * frameSize.x, _direction * frameSize.y + 1, frameSize.x, frameSize.y);
	_sprite.setTextureRect(rect);
}

void Animation::UpdateRightToLeft(sf::Sprite& _sprite, const float& _dt, const float _speed, const unsigned char _direction, bool _isPlaying, const bool _isLooping)
{
	if (_isPlaying)
	{
		timer += _dt;
		if (timer > 1 / _speed)
		{
			currentFrame++;
			timer = 0;
		}
		if (currentFrame > maxFrame)
		{
			if (_isLooping)
			{
				currentFrame = 0;
			}
			else
			{
				_isPlaying = false;
				currentFrame = maxFrame;
				timer = 0;
			}
		}
	}

	rect = sf::IntRect((frameSize.x * maxFrame) - (currentFrame * frameSize.x), _direction * frameSize.y + 1, frameSize.x, frameSize.y);
	_sprite.setTextureRect(rect);
}

void Animation::UpdateUpToDown(const float& _dt, const float _speed, const unsigned char _location, bool& _isPlaying, const bool _isLooping)
{
	if (_isPlaying)
	{
		timer += _dt;
		if (timer > 1 / _speed)
		{
			currentFrame++;
			timer = 0;
		}
		if (currentFrame > maxFrame)
		{
			if (_isLooping)
			{
				currentFrame = 0;
			}
			else
			{
				_isPlaying = false;
				currentFrame = maxFrame;
				timer = 0;
			}
		}
	}
	else
	{
		currentFrame = 1;
	}

	rect = sf::IntRect(_location * frameSize.x, currentFrame * frameSize.y, frameSize.x, frameSize.y);
	sprite.setTextureRect(rect);
}

void Animation::UpdateUpToDownToUp(const float& _dt, const float _speed, const unsigned char _location, bool& _isPlaying, const bool _isLooping)
{
	if (_isPlaying)
	{
		if (!upToDownFinished)
		{
			timer += _dt;
			if (timer > 1 / _speed)
			{
				currentFrame++;
				timer = 0;
			}
			if (currentFrame > maxFrame)
			{
				upToDownFinished = true;
				currentFrame = maxFrame;
			}
		}
		else
		{
			timer += _dt;
			if (timer > 1 / _speed)
			{
				currentFrame--;
				timer = 0;
			}
			if (currentFrame < 0)
			{
				currentFrame = 0;
				if (_isLooping)
				{
					upToDownFinished = false;
				}
			}
		}

	}
	else
	{
		currentFrame = 1;
	}

	rect = sf::IntRect(_location * frameSize.x, currentFrame * frameSize.y, frameSize.x, frameSize.y);
	sprite.setTextureRect(rect);
}

void Animation::UpdateLeftToRightMultipleLines(const float& _dt, const float _speed, bool& _isPlaying, const bool _isLooping)
{
	if (_isPlaying)
	{
		timer += _dt;
		if (timer > 1 / _speed)
		{
			currentFrame++;
			timer = 0;
		}
		if (currentFrame > maxFrame)
		{
			if (_isLooping)
			{
				currentFrame = 0;
				linePlaying = (linePlaying + 1) % maxLine;
			}
			else
			{
				currentFrame = 0;
				if (linePlaying < maxLine)
				{
					linePlaying++;
				}
				else
				{
					_isPlaying = false;
					currentFrame = maxFrame;
					timer = 0;
				}
			}
		}
	}

	rect = sf::IntRect(currentFrame * frameSize.x, linePlaying * frameSize.y, frameSize.x, frameSize.y);
	sprite.setTextureRect(rect);
}

void Animation::UpdateIsEntitySelected(const bool& _isSelected)
{
	if (_isSelected)
	{
		currentFrame = 1;
	}
	else
	{
		currentFrame = 0;
	}

	rect = sf::IntRect(currentFrame * frameSize.x, 0, frameSize.x, frameSize.y);
	sprite.setTextureRect(rect);
}