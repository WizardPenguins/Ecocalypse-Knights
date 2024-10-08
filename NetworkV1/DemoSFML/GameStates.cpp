#include "stdafx.h"
#include "GameStates.h"
#include "Mob.hpp"
#include "Player.hpp"
#include "Multiplayer.h"
#include "EntityManager.h"
#include "LoadingScreen.h"
#include <thread>
#include "Network.h"
#include "Upgrade.h"
#include "TroaDimmenssionSahMaire.h"
#include "Shop.h"

GameState::GameState(GameData& _gameData)
{
	Init(_gameData);
}

GameState::~GameState()
{
	while (!states.empty())
	{
		delete states.back();
		states.pop_back();
	}
}

void GameState::Init(GameData& _gameData)
{
	_gameData.displayMode = sf::Style::Fullscreen;
	_gameData.window.create(sf::VideoMode::getDesktopMode(), "Ecocalypse Knights", _gameData.displayMode);

	sf::Image icon;
	icon.loadFromFile("Assets/Images/Hud/Red_Head.png");
	_gameData.window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	_gameData.window.setFramerateLimit(144);
	_gameData.window.clear(sf::Color::Black);

	_gameData.renderTextureBlue.create(_gameData.window.getSize().x, _gameData.window.getSize().y);
	_gameData.renderTextureRed.create(_gameData.window.getSize().x, _gameData.window.getSize().y);

	//_gameData.window.setMouseCursorVisible(false);
	_gameData.audio = new Audio{};
	_gameData.audio->AddMusic("Assets/Sounds/Global_SFX/LoadingScreen.wav", "LoadingScreen");

	Screen::Init(_gameData.window);

	//splash Screen
	InitSplashScreen();

	// Create graphic thread
	LoadingScreen gt(_gameData, states);

	// Launch thread
	std::thread t(&LoadingScreen::DisplayLoadingScreen, &gt);

	RunSplashScreen(_gameData);

	while (!gt.loadingScreenFinish)
	{
		while (_gameData.window.pollEvent(_gameData.event))
		{
			if (_gameData.event.type == sf::Event::Closed)
			{
				_gameData.window.close();
			}
		}
		_gameData.window.clear(sf::Color(45, 45, 63));

		gt.loadingScreenCompteur = gt.compteur / NB_INIT;

		gt.UpdateAnim(_gameData.window);

		_gameData.window.draw(gt.backChargingBar);
		gt.frontChargingBar.setSize({ (gt.loadingScreenCompteur * gt.backChargingBar.getSize().x), gt.backChargingBar.getSize().y });

		if (gt.compteur == NB_INIT)
		{
			gt.frontChargingBar.setSize(gt.backChargingBar.getSize());
		}

		_gameData.window.draw(gt.frontChargingBar);
		_gameData.window.draw(gt.funnySentence);
		_gameData.window.display();
	}

	// Thread join
	t.join();

	_gameData.dtClock.restart();
	_gameData.dt = 0.f;
}

void GameState::InitGlobalSounds(GameData& _gameData)
{
	//_gameData.audio->AddSound("Assets/Sounds/ButtonPress.ogg", "BUTTON_PRESSED");
	//_gameData.audio->AddSound("Assets/Sounds/ButtonSelect.ogg", "BUTTON_SELECTED");
}

void GameState::RunSplashScreen(GameData& _gameData)
{
	sf::Clock clockTemp;
	float dtTemp;
	while (!this->endOfLogo)
	{
		dtTemp = clockTemp.restart().asSeconds();
		while (_gameData.window.pollEvent(_gameData.event))
		{
			if (_gameData.event.type == sf::Event::Closed)
			{
				_gameData.window.close();
			}
		}

		this->timerButton += dtTemp;
		JoysticPressed();
		UpdateSplashScreen(dtTemp);
		DisplaySplashScreen(_gameData.window);
	}
}

void GameState::UpdateSplashScreen(float _dt)
{
	if (this->onCrea)
	{
		if (this->fadeIn)
		{
			this->creaLogo.setColor(sf::Color(255, 255, 255, this->creaLogo.getColor().a + _dt * SpeedFade));
			if (this->creaLogo.getColor().a >= 245)
			{
				this->fadeOut = true;
				this->fadeIn = false;
			}
		}
		else if (this->fadeOut)
		{
			this->creaLogo.setColor(sf::Color(255, 255, 255, this->creaLogo.getColor().a - _dt * SpeedFade));
			if (this->creaLogo.getColor().a <= 5)
			{
				this->fadeOut = false;
				this->fadeIn = true;
				this->onWizard = true;
				this->onCrea = false;
			}
		}
	}
	else if (this->onWizard)
	{
		if (this->fadeIn)
		{
			this->wizardLogo.setColor(sf::Color(255, 255, 255, this->wizardLogo.getColor().a + _dt * SpeedFade));
			if (this->wizardLogo.getColor().a >= 245)
			{
				this->fadeOut = true;
				this->fadeIn = false;
			}
		}
		else if (this->fadeOut)
		{
			this->wizardLogo.setColor(sf::Color(255, 255, 255, this->wizardLogo.getColor().a - _dt * SpeedFade));
			if (this->wizardLogo.getColor().a <= 5)
			{
				this->fadeOut = false;
				this->fadeIn = true;
				this->onWizard = false;
				this->endOfLogo = true;
			}
		}
	}
}

void GameState::DisplaySplashScreen(sf::RenderWindow& _window)
{
	_window.clear(sf::Color::Black);

	if (this->onCrea)
	{
		_window.draw(this->creaLogo);
	}
	else if (this->onWizard)
	{
		_window.draw(this->wizardLogo);
	}

	_window.display();
}

void GameState::InitSplashScreen()
{
	//for splash Screen
	this->creaLogo = Utils::LoadSprite("Assets/Images/PreMenu/logoCreajeux.png", CENTER);
	this->wizardLogo = Utils::LoadSprite("Assets/Images/PreMenu/Logo.png", CENTER);

	this->creaLogo.setPosition(sf::Vector2f{ WORKING_WIDTH / 2,WORKING_HEIGHT / 2 });
	this->wizardLogo.setPosition(sf::Vector2f{ WORKING_WIDTH / 2,WORKING_HEIGHT / 2 });

	this->creaLogo.setColor(sf::Color(255, 255, 255, 0));
	this->wizardLogo.setColor(sf::Color(255, 255, 255, 0));
}

void GameState::JoysticPressed()
{
	if (sf::Joystick::isConnected(0))
	{
		if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CROSS))
			{
				if (this->onCrea && this->timerButton >= 0.5f)
				{
					this->onCrea = false;
					this->onWizard = true;
					this->fadeOut = false;
					this->fadeIn = true;
					this->timerButton = 0.f;
				}
				else if (this->onWizard && this->timerButton >= 0.5f)
				{
					this->endOfLogo = true;
				}
			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::A))
			{
				if (this->onCrea && this->timerButton >= 0.5f)
				{
					this->onCrea = false;
					this->onWizard = true;
					this->fadeOut = false;
					this->fadeIn = true;
					this->timerButton = 0.f;
				}
				else if (this->onWizard && this->timerButton >= 0.5f)
				{
					this->endOfLogo = true;
				}
			}
		}
	}
}

void GameState::Reload(void)
{

}

void GameState::ReloadNewGame(void)
{
}

void GameState::Update(GameData& _gameData)
{
	_gameData.dt = _gameData.dtClock.restart().asSeconds();
	_gameData.timerButton -= _gameData.dt;
	states[_gameData.gameState]->Update(_gameData);	
}

void GameState::PollEvent(GameData& _gameData)
{
	while (_gameData.window.pollEvent(_gameData.event))
	{
		if (_gameData.event.type == sf::Event::Closed)
		{
			_gameData.window.close();
		}
		if (!_gameData.camera->moveMenuToPlayerSelection && _gameData.timerButton <= 0.f)
		{
			states[_gameData.gameState]->KeyPressed(_gameData);
			states[_gameData.gameState]->MousePressed(_gameData);
			states[_gameData.gameState]->MouseMoved(_gameData);
			states[_gameData.gameState]->MouseReleased(_gameData);
			states[_gameData.gameState]->LostFocus(_gameData);
			states[_gameData.gameState]->JoystickPressed(_gameData);
		}
	}
}

void GameState::Display(GameData& _gameData)
{
	_gameData.window.clear(sf::Color::Black);
	_gameData.renderTextureRed.clear();
	_gameData.renderTextureBlue.clear();
	_gameData.rt->clear(sf::Color::Transparent);

	if (_gameData.gameState == PAUSE && !_gameData.is3DOn)
	{
		states[_gameData.lastState]->Display(_gameData);
	}
	else if (_gameData.gameState == PAUSE && _gameData.is3DOn)
	{
		states[_gameData.lastState]->Display3D(_gameData);
	}

	if (_gameData.is3DOn)
	{
		states[_gameData.gameState]->Display3D(_gameData);

		_gameData.renderTextureRed.display();
		_gameData.renderTextureBlue.display();

		sf::Sprite spriteBlue(_gameData.renderTextureBlue.getTexture());
		sf::Sprite spriteRed(_gameData.renderTextureRed.getTexture());

		spriteRed.setColor({ (sf::Uint8)(spriteRed.getColor().r * 0.299f),(sf::Uint8)(spriteRed.getColor().g * 0.587f),(sf::Uint8)(spriteRed.getColor().b * 0.114f) });
		spriteRed.setColor({ (sf::Uint8)(spriteRed.getColor().r * 255),(sf::Uint8)(spriteRed.getColor().g * 0),(sf::Uint8)(spriteRed.getColor().b * 0),255 });

		spriteBlue.setColor({ (sf::Uint8)(spriteBlue.getColor().r * 0.299f),(sf::Uint8)(spriteBlue.getColor().g * 0.587f),(sf::Uint8)(spriteBlue.getColor().b * 0.114f) });
		spriteBlue.setColor({ (sf::Uint8)(spriteBlue.getColor().r * 0),(sf::Uint8)(spriteBlue.getColor().g * 255),(sf::Uint8)(spriteBlue.getColor().b * 255),255 });

		if (_gameData.gameState == CHARACTER_SELECTION)
		{
			spriteRed.setPosition(0, 0 + HEIGHT_GAP - 1.f );
			spriteBlue.setPosition(0, 0 + HEIGHT_GAP - 1.f);

			if (_gameData.camera->moveMenuToPlayerSelection)
			{
				float ySlide = _gameData.camera->lastCenter.y - (WORKING_HEIGHT / 2);
				spriteRed.setPosition(0, ySlide);
				spriteBlue.setPosition(0, ySlide);
			}
		}
		else if (_gameData.gameState == MENU || _gameData.gameState == SETTINGS)
		{
			spriteRed. setPosition(0, GAP);
			spriteBlue.setPosition(0, GAP);

			if (_gameData.lastState == PAUSE)
			{
				spriteRed.setPosition(0, 0);
				spriteBlue.setPosition(0, 0);
			}

			if (_gameData.camera->moveMenuToPlayerSelection)
			{
				float ySlide = _gameData.camera->lastCenter.y - (WORKING_HEIGHT / 2);
				spriteRed. setPosition(0, ySlide);
				spriteBlue.setPosition(0, ySlide);
			}
		}

		_gameData.window.clear();

		_gameData.window.draw(spriteBlue, sf::BlendAdd);
		_gameData.window.draw(spriteRed, sf::BlendAdd);
	}
	else
	{
		states[_gameData.gameState]->Display(_gameData);
	}

	_gameData.window.display();
	_gameData.rt->display();
}

void GameState::Run(GameData& _gameData)
{
	while (_gameData.window.isOpen())
	{
		PollEvent(_gameData);

		/*_gameData.elapsed = _gameData.fpsClock.restart();
		std::cout << "FPS : " << (1.0f / _gameData.elapsed.asSeconds()) << std::endl;*/

		Update(_gameData);
		_gameData.audio->CheckNumberSound();

		Display(_gameData);
	}
}

void GameState::SetLastState(GameData& _gameData)
{
	_gameData.lastState = _gameData.gameState;
}
