#include "stdafx.h"
#include "PopUp.h"

PopUp::PopUp(int _number)
{
	panel = Utils::LoadSprite("Assets/Images/Hud/Pannel" + std::to_string(_number + 1) + ".png", CENTER);
	panel.setPosition({ WORKING_WIDTH / 2, WORKING_HEIGHT + (float)(panel.getTexture()->getSize().y * 2)});

	startPosBackGround = panel.getPosition();
	endPosBackGround = { panel.getPosition().x, WORKING_HEIGHT - (float)(panel.getTexture()->getSize().y) };
}

void PopUp::Update(float _dt, int& _currentPopUp)
{
	if (!textIsSpawning && !textAsSpawning)
	{
		if (_currentPopUp == 0)
		{
			timerBeforeSpawning -= _dt;
			if (timerBeforeSpawning <= 0.f)
			{
				textIsSpawning = true;
			}
		}
		else
		{
			textIsSpawning = true;
		}
	}

	if (textIsSpawning && !textAsSpawning)
	{
		panel.setPosition(Easings::Lerp(startPosBackGround, endPosBackGround, timerSpawning));
		timerSpawning += _dt;
		

		if (timerSpawning >=1.f)
		{
			textAsSpawning = true;
			timerSpawning = 0.f;
		}
	}

	if (textIsSpawning && textAsSpawning)
	{
		timerWaitOnScreen -= _dt;
		if (timerWaitOnScreen <= 0)
		{
			textIsSpawning = false;
			textIsDespawning = true;
			timerWaitOnScreen = TIMER_WAIT;
		}
	}

	if (textIsDespawning && !textAsDespawning)
	{
		panel.setPosition(Easings::Lerp(endPosBackGround, startPosBackGround, timerDespawning));
		timerDespawning += _dt;

		if (timerDespawning >= 1.f)
		{
			textAsDespawning = true;
			timerDespawning = 0.f;
			_currentPopUp++;
		}
	}
}

void PopUp::Draw(sf::RenderWindow& _window)
{
	if (textIsSpawning || textIsDespawning)
	{
		_window.draw(panel);
	}
}

void PopUp::Draw(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	if (textIsSpawning || textIsDespawning)
	{
		Utils::BlitSprite(panel,sf::Vector2f{panel.getPosition().x + (TROAD_GAP * (-1 * panel.getPosition().y + WORKING_HEIGHT)/ 100.f),panel.getPosition().y},0,_red);
		Utils::BlitSprite(panel,sf::Vector2f{panel.getPosition().x - (TROAD_GAP * (-1 * panel.getPosition().y + WORKING_HEIGHT)/ 100.f),panel.getPosition().y},0,_blue);
	}
}