#include "stdafx.h"
#include "Pause.h"
#include "Camera.h"
#include "Network.h"
#include "EntityManager.h"


Pause::Pause(GameData& _gameData)
{
	InitComponents(_gameData);
}

Pause::~Pause()
{

}

void Pause::InitComponents(GameData& _gameData)
{
	buttonVect.push_back(new Button("Assets/Images/Pause/ResumeButton.png", Screen::Pos(WORKING_WIDTH / 2.f, 420.f), 7, Screen::Scale(1.f, 1.f), true));
	buttonVect.push_back(new Button("Assets/Images/Menu/Settings.png", Screen::Pos(WORKING_WIDTH / 2.f, 500.f), 7, Screen::Scale(1.f, 1.f), true));
	buttonVect.push_back(new Button("Assets/Images/Pause/MainMenuButton.png", Screen::Pos(WORKING_WIDTH / 2.f, 580.f), 7, Screen::Scale(1.f, 1.f), true));
	buttonVect.push_back(new Button("Assets/Images/Menu/Quit.png", Screen::Pos(WORKING_WIDTH / 2.f, 660.f), 7, Screen::Scale(1.f, 1.f), true));

	buttonSelected = CONTINUE;
	buttonVect[CONTINUE]->SetIsSelected(true, _gameData);


	for (int i = 0; i < buttonVect.size(); i++)
	{
		swordsPos.push_back({ buttonVect[i]->posButton.x - 200, buttonVect[i]->posButton.y });
		endPosEasing.push_back(swordsPos[i]);
		startPosEasing.push_back({ swordsPos[i].x - 30, swordsPos[i].y });
	}
	lastPos = CONTINUE;
	nextPos = CONTINUE;
	timerEasing = 0.f;
	isSwifting = false;
	timerEasing2 = 0.f;
	movingFront = false;

	_gameData.startPosPauseSwordSprite = swordsPos[CONTINUE];
	_gameData.swordSpriteMenu.setPosition(_gameData.startPosPauseSwordSprite);

	//Init Network

	buttonVectMulti.push_back(new Button("Assets/Images/Pause/ResumeButton.png", Screen::Pos(WORKING_WIDTH / 2.f, 420.f), 7, Screen::Scale(1.f, 1.f), true));
	buttonVectMulti.push_back(new Button("Assets/Images/Pause/MainMenuButton.png", Screen::Pos(WORKING_WIDTH / 2.f, 500.f), 7, Screen::Scale(1.f, 1.f), true));
	buttonVectMulti.push_back(new Button("Assets/Images/Menu/Quit.png", Screen::Pos(WORKING_WIDTH / 2.f, 580.f), 7, Screen::Scale(1.f, 1.f), true));

	for (int i = 0; i < buttonVectMulti.size(); i++)
	{
		swordsPosMulti.push_back({ buttonVectMulti[i]->posButton.x - 200, buttonVectMulti[i]->posButton.y });
		endPosEasingMulti.push_back(swordsPosMulti[i]);
		startPosEasingMulti.push_back({ swordsPosMulti[i].x - 30, swordsPosMulti[i].y });
	}

	_gameData.startPosPauseSwordSpriteMulti = swordsPosMulti[CONTINUE];
	//_gameData.swordSpriteMenu.setPosition(_gameData.startPosPauseSwordSprite);
}

void Pause::Reload(GameData& _gameData)
{
	buttonSelected = CONTINUE;
	buttonVect[CONTINUE]->SetIsSelected(true, _gameData);
	buttonVect[SETTINGS_PAUSE]->SetIsSelected(false, _gameData);
	buttonVect[QUIT_PAUSE]->SetIsSelected(false, _gameData);
	buttonVect[MAIN_MENU]->SetIsSelected(false, _gameData);

	//Reload Network

	buttonVectMulti[(int)ButtonTypePauseMulti::CONTINUE]->SetIsSelected(true, _gameData);
	buttonVectMulti[(int)ButtonTypePauseMulti::MAIN_MENU]->SetIsSelected(false, _gameData);
	buttonVectMulti[(int)ButtonTypePauseMulti::QUIT]->SetIsSelected(false, _gameData);
}

void Pause::UpdatePos(GameData& _gameData)
{
	for (int i = 0; i < buttonVect.size(); i++)
	{
		buttonVect[i]->sprite.setPosition((_gameData.window.mapPixelToCoords((sf::Vector2i)buttonVect[i]->posButton)));
	}

	for (int i = 0; i < buttonVectMulti.size(); i++)
	{
		buttonVectMulti[i]->sprite.setPosition(_gameData.window.mapPixelToCoords((sf::Vector2i)buttonVectMulti[i]->posButton));
	}
}

void Pause::UpdateButtonPressedEffectMulti(GameData& _gameData)
{
	if (buttonVectMulti[buttonSelected]->GetIsPressed() && buttonSelected != 1)
	{
		if (timerEasing < 1.f)
		{
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR", 100.f);

			swordCurrentPos = Easings::Lerp(swordsPosMulti[lastPos], { swordsPosMulti[lastPos].x + 320 ,swordsPosMulti[lastPos].y }, timerEasing);
			timerEasing += _gameData.dt * 10;
			_gameData.swordSpriteMenu.setPosition(swordCurrentPos);
		}
	}

	if (buttonVectMulti[(int)ButtonTypePauseMulti::CONTINUE]->isPressedFinished)
	{
		_gameData.lastState = _gameData.gameState;
		(bool)(_gameData.arenaIsChose) ? _gameData.ChangeState(ARENA) : _gameData.ChangeState(GAME);
		_gameData.swordSpriteMenu.setPosition(swordsPosMulti[lastPos]);
		timerEasing = 0.f;
		buttonVectMulti[(int)ButtonTypePauseMulti::CONTINUE]->SetIsPressed(false, _gameData);
		buttonVectMulti[(int)ButtonTypePauseMulti::CONTINUE]->isPressedFinished = false;
	}
	else if (buttonVectMulti[(int)ButtonTypePauseMulti::MAIN_MENU]->isPressedFinished)
	{
		_gameData.ChangeState(MENU);
		_gameData.playerTeam.clear();
		_gameData.camera->SetDefaultCenter();
		_gameData.camera->SetDefaultZoom();

		_gameData.swordSpriteMenu.setPosition(_gameData.startPosMenuSwordSprite);
		timerEasing = 0.f;

		buttonSelected = (int)ButtonTypePauseMulti::CONTINUE;

		_gameData.audio->StopAllMusics();
		buttonVectMulti[(int)ButtonTypePauseMulti::MAIN_MENU]->SetIsPressed(false, _gameData);
		buttonVectMulti[(int)ButtonTypePauseMulti::MAIN_MENU]->isPressedFinished = false;
		_gameData.reloadGame = true;
		_gameData.camera->GetParalax().Reload(_gameData);
		_gameData.reloadCameraMenu = true;
		_gameData.reloadCharacterSelection = true;
		_gameData.reloadMenu = true;
		_gameData.reloadFlyingNut = true;
		_gameData.actualWave = WAVE1;
		_gameData.drop.clear();
		_gameData.sortVector.clear();
		_gameData.camera->Reload(_gameData);
		_gameData.entityMan->Reload();
		_gameData.inCinematic = false;
		_gameData.reloadLevelManager = true;
		_gameData.playWitchDeath = false;

		_gameData.reloadCamera = true;
		_gameData.reloadShop = true;
		_gameData.reloadGameOver = true;
		_gameData.reloadMapManager = true;
		_gameData.reloadArena = true;
	}
	else if (buttonVectMulti[(int)ButtonTypePauseMulti::QUIT]->isPressedFinished)
	{
		_gameData.window.close();
		buttonVectMulti[(int)ButtonTypePauseMulti::QUIT]->SetIsPressed(false, _gameData);
		buttonVectMulti[(int)ButtonTypePauseMulti::QUIT]->isPressedFinished = false;
	}
}

void Pause::UpdateButtonPressedEffect(GameData& _gameData)
{
	if (buttonVect[buttonSelected]->GetIsPressed())
	{
		if (timerEasing < 1.f)
		{
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR", 100.f);

			swordCurrentPos = Easings::Lerp(swordsPos[lastPos], { swordsPos[lastPos].x + 320 ,swordsPos[lastPos].y }, timerEasing);
			timerEasing += _gameData.dt * 10;
			_gameData.swordSpriteMenu.setPosition(swordCurrentPos);
		}
	}

	if (buttonVect[CONTINUE]->isPressedFinished)
	{
		_gameData.lastState = _gameData.gameState;
		(bool)(_gameData.arenaIsChose) ? _gameData.ChangeState(ARENA) : _gameData.ChangeState(GAME);
		_gameData.swordSpriteMenu.setPosition(swordsPos[lastPos]);
		timerEasing = 0.f;
		buttonVect[CONTINUE]->SetIsPressed(false, _gameData);
		buttonVect[CONTINUE]->isPressedFinished = false;
	}
	else if (buttonVect[SETTINGS_PAUSE]->isPressedFinished)
	{
		_gameData.ChangeState(SETTINGS);

		//_gameData.camera->SetDefaultCenter();
		_gameData.camera->GetView().setCenter(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f + GAP);
		_gameData.camera->SetDefaultZoom();

		_gameData.swordSpriteMenu.setPosition(swordsPos[lastPos]);
		timerEasing = 0.f;
		buttonVect[SETTINGS_PAUSE]->SetIsPressed(false, _gameData);
		buttonVect[SETTINGS_PAUSE]->isPressedFinished = false;
	}
	else if (buttonVect[MAIN_MENU]->isPressedFinished)
	{
		_gameData.ChangeState(MENU);
		_gameData.playerTeam.clear();
		_gameData.camera->SetDefaultCenter();
		_gameData.camera->SetDefaultZoom();

		_gameData.swordSpriteMenu.setPosition(_gameData.startPosMenuSwordSprite);
		timerEasing = 0.f;

		buttonSelected = CONTINUE;

		_gameData.audio->StopAllMusics();
		buttonVect[MAIN_MENU]->SetIsPressed(false, _gameData);
		buttonVect[MAIN_MENU]->isPressedFinished = false;
		_gameData.reloadGame = true;
		_gameData.reloadFlyingNut = true;
		_gameData.reloadCameraMenu = true;
		_gameData.reloadCharacterSelection = true;
		_gameData.reloadMenu = true;
		_gameData.reloadArena = true;
	}
	else if (buttonVect[QUIT_PAUSE]->isPressedFinished)
	{
		_gameData.window.close();
		buttonVect[QUIT_PAUSE]->SetIsPressed(false, _gameData);
		buttonVect[QUIT_PAUSE]->isPressedFinished = false;
	}
}

void Pause::UpdateSwordPosMulti(float _dt, sf::Sprite& _swordSprite)
{
	nextPos = buttonSelected;
	if (nextPos != lastPos || isSwifting)
	{

		if (timerEasing < 1.f)
		{
			isSwifting = true;
			timerEasing += _dt * 15;

			if (timerEasing > 1.f)
			{
				timerEasing = 1.f;
			}

			swordCurrentPos = Easings::Lerp(swordsPosMulti[lastPos], swordsPosMulti[nextPos], timerEasing);

		}
		else
		{
			isSwifting = false;
			timerEasing = 0.f;
			lastPos = nextPos;
		}
	}
	else if (!buttonVectMulti[buttonSelected]->GetIsPressed())
	{
		if (movingFront)
		{
			if (timerEasing2 < 1)
			{

				if (timerEasing2 < 0.4f)
				{
					swordCurrentPos = Easings::Lerp(startPosEasingMulti[lastPos], endPosEasingMulti[lastPos], Easings::OutElastic(timerEasing2));
					timerEasing2 += _dt / 4;
				}
				else
				{
					timerEasing2 += _dt * 5;
				}

			}
			else
			{
				timerEasing2 = 0.f;
				movingFront = false;
			}
		}
		else if (!movingFront)
		{
			if (timerEasing2 < 1)
			{
				if (timerEasing2 > 0.6f)
				{
					swordCurrentPos = Easings::Lerp(endPosEasingMulti[lastPos], startPosEasingMulti[lastPos], Easings::InElastic(timerEasing2));
					timerEasing2 += _dt / 4;
				}
				else
				{
					timerEasing2 += _dt * 5;
				}
			}
			else
			{
				timerEasing2 = 0.f;
				movingFront = true;
			}
		}
	}
	_swordSprite.setPosition(swordCurrentPos);
}

void Pause::UpdateSwordPos(float _dt, sf::Sprite& _swordSprite)
{
	nextPos = buttonSelected;
	if (nextPos != lastPos || isSwifting)
	{

		if (timerEasing < 1.f)
		{
			isSwifting = true;
			timerEasing += _dt * 15;

			if (timerEasing > 1.f)
			{
				timerEasing = 1.f;
			}

			swordCurrentPos = Easings::Lerp(swordsPos[lastPos], swordsPos[nextPos], timerEasing);

		}
		else
		{
			isSwifting = false;
			timerEasing = 0.f;
			lastPos = nextPos;
		}
	}
	else if (!buttonVect[buttonSelected]->GetIsPressed())
	{
		if (movingFront)
		{
			if (timerEasing2 < 1)
			{

				if (timerEasing2 < 0.4f)
				{
					swordCurrentPos = Easings::Lerp(startPosEasing[lastPos], endPosEasing[lastPos], Easings::OutElastic(timerEasing2));
					timerEasing2 += _dt / 4;
				}
				else
				{
					timerEasing2 += _dt * 5;
				}

			}
			else
			{
				timerEasing2 = 0.f;
				movingFront = false;
			}
		}
		else if (!movingFront)
		{
			if (timerEasing2 < 1)
			{
				if (timerEasing2 > 0.6f)
				{
					swordCurrentPos = Easings::Lerp(endPosEasing[lastPos], startPosEasing[lastPos], Easings::InElastic(timerEasing2));
					timerEasing2 += _dt / 4;
				}
				else
				{
					timerEasing2 += _dt * 5;
				}
			}
			else
			{
				timerEasing2 = 0.f;
				movingFront = true;
			}
		}
	}
	_swordSprite.setPosition(swordCurrentPos);
}

void Pause::Update(GameData& _gameData)
{
	if (_gameData.lastState == GAME && _gameData.levelState != MAP)
	{
		_gameData.camera->UpdatePause(_gameData);
	}
	else if (_gameData.lastState == ARENA)
	{
		_gameData.camera->UpdateArena();
	}
	else if (_gameData.levelState == MAP && _gameData.lastState == SETTINGS)
	{
		_gameData.camera->UpdatePause(_gameData);
	}
	UpdatePos(_gameData);
	UpdateButtonsSelection(_gameData);
	DownUpJoysticksButtons(_gameData);
	if (!_gameData.multiplayer)
	{
		UpdateSwordPos(_gameData.dt, _gameData.swordSpriteMenu);
		UpdateButtonPressedEffect(_gameData);
	}
	else
	{
		UpdateSwordPosMulti(_gameData.dt, _gameData.swordSpriteMenu);
		UpdateButtonPressedEffectMulti(_gameData);
	}
	UpdateAllButtonsAnim(_gameData.dt);

	if (_gameData.multiplayer)
	{
		//_gameData.network->IngameStateUpdatePause(_gameData, this->swordCurrentPos);
	}
}

void Pause::Display(GameData& _gameData)
{
	_gameData.window.setView(_gameData.window.getDefaultView());
	if (!_gameData.multiplayer)
	{
		DisplayButtons(_gameData.window);
	}
	else
	{
		DisplayButtonsMulti(_gameData.window);
	}
	if (_gameData.lastState == GAME && _gameData.levelState == MAP)
	{
		Utils::BlitSprite(_gameData.swordSpriteMenu, swordCurrentPos, 0, _gameData.window);
	}
	_gameData.window.draw(_gameData.swordSpriteMenu);
}

void Pause::Display3D(GameData& _gameData)
{
	_gameData.window.setView(_gameData.window.getDefaultView());
	_gameData.renderTextureRed.setView(_gameData.renderTextureRed.getDefaultView());
	_gameData.renderTextureBlue.setView(_gameData.renderTextureBlue.getDefaultView());

	DisplayButtons3D(_gameData.renderTextureRed, _gameData.renderTextureBlue);

	if (_gameData.lastState == GAME && _gameData.levelState == MAP)
	{
		Utils::BlitSprite(_gameData.swordSpriteMenu, sf::Vector2f{ swordCurrentPos.x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),swordCurrentPos.y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(_gameData.swordSpriteMenu, sf::Vector2f{ swordCurrentPos.x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),swordCurrentPos.y }, 0, _gameData.renderTextureBlue);
	}

	Utils::BlitSprite(_gameData.swordSpriteMenu, sf::Vector2f{ _gameData.swordSpriteMenu.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),_gameData.swordSpriteMenu.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(_gameData.swordSpriteMenu, sf::Vector2f{ _gameData.swordSpriteMenu.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),_gameData.swordSpriteMenu.getPosition().y }, 0, _gameData.renderTextureBlue);
}

void Pause::KeyPressed(GameData& _gameData)
{
	if (!_gameData.multiplayer)
	{
		DownUpKeysButtons(_gameData);

		if (_gameData.event.type == sf::Event::KeyPressed && _gameData.event.key.code == sf::Keyboard::Enter)
		{
			switch (buttonSelected)
			{
			case CONTINUE:
				buttonVect[CONTINUE]->SetIsPressed(true, _gameData);
				break;
			case SETTINGS_PAUSE:
				buttonVect[SETTINGS_PAUSE]->SetIsPressed(true, _gameData);
				break;
			case MAIN_MENU:
				buttonVect[MAIN_MENU]->SetIsPressed(true, _gameData);
				break;
			case QUIT_PAUSE:
				buttonVect[QUIT_PAUSE]->SetIsPressed(true, _gameData);
				break;
			}
		}
	}
	else
	{
		DownUpKeysButtonsMulti(_gameData);

		if (_gameData.event.type == sf::Event::KeyPressed && _gameData.event.key.code == sf::Keyboard::Enter)
		{
			/*switch (buttonSelected)
			{
			case (int)ButtonTypePauseMulti::CONTINUE:
				buttonVect[CONTINUE]->SetIsPressed(true, _gameData);
				break;
			case (int)ButtonTypePauseMulti::MAIN_MENU:
				buttonVect[MAIN_MENU]->SetIsPressed(true, _gameData);
				break;
			case (int)ButtonTypePauseMulti::QUIT:
				buttonVect[QUIT_PAUSE]->SetIsPressed(true, _gameData);
				break;
			}*/
			buttonVect[buttonSelected]->SetIsPressed(true, _gameData);
		}
	}
}

void Pause::JoystickPressedMulti(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::JoystickButtonPressed)
	{
		if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CROSS))
			{
				/*switch (buttonSelected)
				{
				case CONTINUE:
					buttonVect[CONTINUE]->SetIsPressed(true, _gameData);
					break;
				case MAIN_MENU:
					buttonVect[MAIN_MENU]->SetIsPressed(true, _gameData);
					break;
				case QUIT_PAUSE:
					buttonVect[QUIT_PAUSE]->SetIsPressed(true, _gameData);
					break;
				}*/
				buttonVectMulti[buttonSelected]->SetIsPressed(true, _gameData);
			}
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CIRCLE))
			{
				buttonSelected = (int)ButtonTypePauseMulti::CONTINUE;
				buttonVectMulti[(int)ButtonTypePauseMulti::CONTINUE]->SetIsPressed(true, _gameData);
			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::A))
			{
				/*switch (buttonSelected)
				{
				case CONTINUE:
					buttonVect[CONTINUE]->SetIsPressed(true, _gameData);
					break;
				case MAIN_MENU:
					buttonVect[MAIN_MENU]->SetIsPressed(true, _gameData);
					break;
				case QUIT_PAUSE:
					buttonVect[QUIT_PAUSE]->SetIsPressed(true, _gameData);
					_gameData.window.close();
					break;
				}*/
				buttonVectMulti[buttonSelected]->SetIsPressed(true, _gameData);
			}
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::B))
			{
				buttonSelected = (int)ButtonTypePauseMulti::CONTINUE;
				buttonVectMulti[(int)ButtonTypePauseMulti::CONTINUE]->SetIsPressed(true, _gameData);
			}
		}
	}
}

void Pause::JoystickPressed(GameData& _gameData)
{
	if (!_gameData.multiplayer)
	{
		if (_gameData.event.type == sf::Event::JoystickButtonPressed)
		{
			if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
			{
				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CROSS))
				{
					switch (buttonSelected)
					{
					case CONTINUE:
						buttonVect[CONTINUE]->SetIsPressed(true, _gameData);
						break;
					case SETTINGS_PAUSE:
						buttonVect[SETTINGS_PAUSE]->SetIsPressed(true, _gameData);
						break;
					case MAIN_MENU:
						buttonVect[MAIN_MENU]->SetIsPressed(true, _gameData);
						break;
					case QUIT_PAUSE:
						buttonVect[QUIT_PAUSE]->SetIsPressed(true, _gameData);
						break;
					}
				}
				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CIRCLE))
				{
					buttonSelected = CONTINUE;
					buttonVect[CONTINUE]->SetIsPressed(true, _gameData);
				}
			}
			else
			{
				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::A))
				{
					switch (buttonSelected)
					{
					case CONTINUE:
						buttonVect[CONTINUE]->SetIsPressed(true, _gameData);
						break;
					case SETTINGS_PAUSE:
						buttonVect[SETTINGS_PAUSE]->SetIsPressed(true, _gameData);
						break;
					case MAIN_MENU:
						buttonVect[MAIN_MENU]->SetIsPressed(true, _gameData);
						break;
					case QUIT_PAUSE:
						buttonVect[QUIT_PAUSE]->SetIsPressed(true, _gameData);
						_gameData.window.close();
						break;
					}
				}
				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::B))
				{
					buttonSelected = CONTINUE;
					buttonVect[CONTINUE]->SetIsPressed(true, _gameData);
				}
			}
		}
	}
	else
	{
		JoystickPressedMulti(_gameData);
	}
}