#include "stdafx.h"
#include"GameOver.h"
#include "Player.hpp"
#include "Camera.h"
#include "Easings.h"
#include "Entity.h"
#include "Mob.hpp"
#include "Boar.hpp"
#include "EntityManager.h"

GameOver::GameOver(GameData& _gameData)
{
	InitComponents(_gameData);
}

GameOver::~GameOver()
{

}

void GameOver::InitComponents(GameData& _gameData)
{
	InitBackground();
	InitButtons(_gameData);
	_gameData.audio->AddMusic("Assets/Musics/LoseMusic.ogg", "LOSE");
	_gameData.audio->AddMusic("Assets/Musics/WinMusic.ogg", "WIN");
	timerCredits = 0.f;
	fade = new Fade(2.f, false, true);
}

void GameOver::InitButtons(GameData& _gameData)
{
	buttonVect.push_back(new Button("Assets/Images/Credits/MenuButton.png", Screen::Pos(WORKING_WIDTH - 150, WORKING_HEIGHT - 150), 3, Screen::Scale(1, 1), false));

	buttonSelected = MAIN_MENU_GAME_OVER;
	buttonVect[MAIN_MENU_GAME_OVER]->SetIsSelected(true, _gameData);
}

void GameOver::InitBackground(void)
{
	backgroundLoose = Utils::LoadSprite("Assets/Images/BackGround/LoseBackground.png", TOP_LEFT);
	backgroundWin = Utils::LoadSprite("Assets/Images/Credits/Credits.png", TOP_LEFT);
}

void GameOver::FadeManagement(GameData& _gameData)
{
	fade->Update(_gameData.dt);
	if (fade->GetTimer() >= fade->GetTimerMax())
	{
		_gameData.ChangeState(MENU);
		_gameData.audio->StopAllMusics();
		_gameData.swordSpriteMenu.setPosition(_gameData.startPosMenuSwordSprite);
		buttonVect[MAIN_MENU_GAME_OVER]->SetIsPressed(false, _gameData);
		buttonVect[MAIN_MENU_GAME_OVER]->isPressedFinished = false;
		timerCredits = 0;
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
	}
}

void GameOver::UpdateButtonPressedEffect(GameData& _gameData)
{
	if (buttonVect[MAIN_MENU_GAME_OVER]->isPressedFinished)
	{
		fade->SetIsFadingIn(true);
	}
}

void GameOver::Update(GameData& _gameData)
{
	if (_gameData.reloadGameOver)
	{
		Reload(_gameData);
		_gameData.reloadGameOver = false;
	}	
	
	_gameData.audio->FadeOutMusic("INGAME", _gameData.dt);
	FadeManagement(_gameData);
	_gameData.audio->FadeOutMusic("CHILL", _gameData.dt);
	UpdateButtonsSelection(_gameData);
	UpdateButtonPressedEffect(_gameData);
	UpdateAllButtonsAnim(_gameData.dt);

	for (int i = 0; i < _gameData.tabMob.size(); i++)
	{
		if (_gameData.tabMob[i]->GetType() == TypeMob::Boss && _gameData.tabMob[i]->GetState() == MobState::Death && timerCredits < 1.f)
		{
			posCredits = Easings::Lerp({ 0.f, 0.f }, { 0.f, -CREDITS_BG_HEIGHT + WORKING_HEIGHT }, Easings::OutSine(timerCredits));
			timerCredits += _gameData.dt / 20.f;
			backgroundWin.setPosition(posCredits);
		}
	}
}

void GameOver::Display(GameData& _gameData)
{
	if (_gameData.tabMob.empty())
	{
		_gameData.audio->FadeInMusic("WIN", _gameData.dt);
		_gameData.window.draw(backgroundWin);
	}
	for (int i = 0; i < _gameData.tabMob.size(); i++)
	{
		if (_gameData.tabMob[i]->GetType() == TypeMob::Boss && _gameData.tabMob[i]->GetState() == MobState::Death)
		{
			_gameData.audio->FadeInMusic("WIN", _gameData.dt);
			_gameData.window.draw(backgroundWin);
		}
	}

	int death = 0;
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.tabPlayers[i]->GetLife() <= 0)
		{
			death++;
		}
	}

	if (_gameData.tabPlayers.size() == death)
	{
		_gameData.audio->FadeInMusic("LOSE", _gameData.dt);
		_gameData.window.draw(backgroundLoose);
	}

	DisplayButtons(_gameData.window);

	_gameData.window.draw(textWin);
	fade->Display(_gameData.window);
}

void GameOver::Display3D(GameData& _gameData)
{
	if (_gameData.tabMob.empty())
	{
		_gameData.audio->FadeInMusic("WIN", _gameData.dt);

		Utils::BlitSprite(backgroundWin, sf::Vector2f{ backgroundWin.getPosition().x - (TROAD_GAP * (-1 * backgroundWin.getPosition().y + WORKING_HEIGHT) / 100.f),backgroundWin.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(backgroundWin, sf::Vector2f{ backgroundWin.getPosition().x + (TROAD_GAP * (-1 * backgroundWin.getPosition().y + WORKING_HEIGHT) / 100.f),backgroundWin.getPosition().y }, 0, _gameData.renderTextureBlue);
	}
	for (int i = 0; i < _gameData.tabMob.size(); i++)
	{
		if (_gameData.tabMob[i]->GetType() == TypeMob::Boss && _gameData.tabMob[i]->GetState() == MobState::Death)
		{
			_gameData.audio->FadeInMusic("WIN", _gameData.dt);

			Utils::BlitSprite(backgroundWin, sf::Vector2f{ backgroundWin.getPosition().x - (TROAD_GAP * (-1 * backgroundWin.getPosition().y + WORKING_HEIGHT) / 100.f),backgroundWin.getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(backgroundWin, sf::Vector2f{ backgroundWin.getPosition().x + (TROAD_GAP * (-1 * backgroundWin.getPosition().y + WORKING_HEIGHT) / 100.f),backgroundWin.getPosition().y }, 0, _gameData.renderTextureBlue);
		}
	}

	int death = 0;
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.tabPlayers[i]->GetLife() <= 0)
		{
			death++;
		}
	}

	if (_gameData.tabPlayers.size() == death)
	{
		_gameData.audio->FadeInMusic("LOSE", _gameData.dt);

		Utils::BlitSprite(backgroundLoose, sf::Vector2f{ backgroundLoose.getPosition().x - (TROAD_GAP * (-1 * backgroundLoose.getPosition().y + WORKING_HEIGHT) / 100.f),backgroundLoose.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(backgroundLoose, sf::Vector2f{ backgroundLoose.getPosition().x + (TROAD_GAP * (-1 * backgroundLoose.getPosition().y + WORKING_HEIGHT) / 100.f),backgroundLoose.getPosition().y }, 0, _gameData.renderTextureBlue);
	}

	DisplayButtons3D(_gameData.renderTextureRed,_gameData.renderTextureBlue);

	sf::Vector2f posWin = textWin.getPosition();

	textWin.setPosition(posWin.x + (TROAD_GAP * (-1 * posWin.y + WORKING_HEIGHT) / 100.f), posWin.y);
	_gameData.renderTextureRed.draw(textWin);

	textWin.setPosition(posWin.x - (TROAD_GAP * (-1 * posWin.y + WORKING_HEIGHT) / 100.f), posWin.y);
	_gameData.renderTextureBlue.draw(textWin);

	textWin.setPosition(posWin);

	fade->Display(_gameData.renderTextureRed,_gameData.renderTextureBlue);
}

void GameOver::KeyPressed(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::KeyPressed && (_gameData.event.key.code == sf::Keyboard::Escape || _gameData.event.key.code == sf::Keyboard::Enter))
	{
		buttonVect[MAIN_MENU_GAME_OVER]->SetIsPressed(true, _gameData);
	}
}

void GameOver::JoystickPressed(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::JoystickButtonPressed)
	{
		if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CIRCLE) || sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CROSS))
			{
				buttonVect[MAIN_MENU_GAME_OVER]->SetIsPressed(true, _gameData);
			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::B) || sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::A))
			{
				buttonVect[MAIN_MENU_GAME_OVER]->SetIsPressed(true, _gameData);
			}
		}


	}
}

void GameOver::Reload(GameData& _gameData)
{
	delete fade;
	fade = nullptr;

	fade = new Fade(2.f, false, true);

	buttonSelected = MAIN_MENU_GAME_OVER;
	buttonVect[MAIN_MENU_GAME_OVER]->SetIsSelected(false, _gameData);
	buttonVect[MAIN_MENU_GAME_OVER]->SetIsPressed(false, _gameData);
}