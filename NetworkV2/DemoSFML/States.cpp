#include "stdafx.h"
#include "States.h"

States::~States()
{
	while (!buttonVect.empty())
	{
		delete buttonVect.back();
		buttonVect.pop_back();
	}
}

void States::UpdateAllButtonsAnim(const float& _dt)
{
	for (unsigned char i = 0; i < buttonVect.size(); i++)
	{
		buttonVect[i]->UpdateAnimButton(_dt);
	}
}

void States::SetButtonSelectedPressed(GameData& _gameData)
{
	buttonVect[buttonSelected]->SetIsPressed(true, _gameData);
}

void States::UpdateButtonsSelection(GameData& _gameData)
{
	if (buttonVect.size() > 1)
	{
		for (unsigned char i = 0; i < buttonVect.size(); i++)
		{
			if (buttonSelected == i)
			{
				buttonVect[i]->SetIsSelected(true, _gameData);
			}
			else
			{
				buttonVect[i]->SetIsSelected(false, _gameData);
			}
		}
	}
}

void States::DisplayButtonsMulti(sf::RenderWindow& _window)
{
	for (int i = 0; i < buttonVectMulti.size(); i++)
	{
		_window.draw(buttonVectMulti[i]->sprite);
	}
}

void States::DisplayButtons(sf::RenderWindow& _window)
{
	for (unsigned char i = 0; i < buttonVect.size(); i++)
	{
		_window.draw(buttonVect[i]->sprite);
	}
}

void States::DisplayButtons3D(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	for (unsigned char i = 0; i < buttonVect.size(); i++)
	{
		Utils::BlitSprite(buttonVect[i]->sprite, sf::Vector2f{ buttonVect[i]->sprite.getPosition().x + (TROAD_GAP * (-1 * buttonVect[i]->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),buttonVect[i]->sprite.getPosition().y }, 0, _red);
		Utils::BlitSprite(buttonVect[i]->sprite, sf::Vector2f{ buttonVect[i]->sprite.getPosition().x - (TROAD_GAP * (-1 * buttonVect[i]->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),buttonVect[i]->sprite.getPosition().y }, 0, _blue);
	}
}

void States::DownUpKeysButtons(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::KeyPressed)
	{
		switch (_gameData.event.key.code)
		{

		case sf::Keyboard::Down:

			if (buttonSelected != (buttonVect.size() - 1))
			{
				++buttonSelected;
			}
			else
			{
				buttonSelected = 0;
			}

			buttonVect[buttonSelected]->SetIsSelected(true, _gameData);

			break;

		case sf::Keyboard::Up:

			if (buttonSelected != 0)
			{
				--buttonSelected;
			}
			else
			{
				buttonSelected = (int)(buttonVect.size() - 1);
			}

			buttonVect[buttonSelected]->SetIsSelected(true, _gameData);

			break;

		default:
			break;
		}
	}
}

void States::DownUpKeysButtonsMulti(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::KeyPressed)
	{
		switch (_gameData.event.key.code)
		{

		case sf::Keyboard::Down:

			if (buttonSelected != (buttonVectMulti.size() - 1))
			{
				++buttonSelected;
			}
			else
			{
				buttonSelected = 0;
			}

			buttonVectMulti[buttonSelected]->SetIsSelected(true, _gameData);

			break;

		case sf::Keyboard::Up:

			if (buttonSelected != 0)
			{
				--buttonSelected;
			}
			else
			{
				buttonSelected = (int)(buttonVectMulti.size() - 1);
			}

			buttonVectMulti[buttonSelected]->SetIsSelected(true, _gameData);

			break;

		default:
			break;
		}
	}
}

void States::DownUpJoysticksButtons(GameData& _gameData)
{
	if (!buttonVect.empty())
	{
		float yPos1 = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::LEFT_JOYSTICK_Y);
		float yPos2 = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::DIR_CROSS_Y);

		joystickTimer += _gameData.dt;

		if (joystickTimer >= 0.2f)
		{
			joystickTimer = 0;
			if (yPos1 > 15 || yPos2 < -15)
			{
				if (buttonSelected != (buttonVect.size() - 1))
				{
					++buttonSelected;
				}
				else
				{
					buttonSelected = 0;
				}

				buttonVect[buttonSelected]->SetIsSelected(true, _gameData);

			}
			if (yPos1 < -15 || yPos2 > 15)
			{
				if (buttonSelected != 0)
				{
					--buttonSelected;
				}
				else
				{
					buttonSelected = (int)(buttonVect.size() - 1);
				}

				buttonVect[buttonSelected]->SetIsSelected(true, _gameData);

			}
		}
	}
}

void States::LeftRightJoysticksButtons(GameData& _gameData)
{
	if (!buttonVect.empty())
	{
		float xPos1 = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::LEFT_JOYSTICK_X);

		joystickTimer += _gameData.dt;

		if (joystickTimer >= 0.1f)
		{

			joystickTimer = 0;
			if (xPos1 > 15)
			{
				if (buttonSelected != (buttonVect.size() - 1))
				{
					++buttonSelected;
				}
				else
				{
					buttonSelected = 0;
				}

				buttonVect[buttonSelected]->SetIsSelected(true, _gameData);

			}
			if (xPos1 < -15)
			{
				if (buttonSelected != 0)
				{
					--buttonSelected;
				}
				else
				{
					buttonSelected = (int)(buttonVect.size() - 1);
				}

				buttonVect[buttonSelected]->SetIsSelected(true, _gameData);

			}

		}
	}
}