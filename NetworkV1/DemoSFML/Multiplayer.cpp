#include "stdafx.h"
#include "Multiplayer.h"
#include "Network.h"
#include "Easings.h"

void Multiplayer::InitSwordVector(void)
{
	swordsPos.clear();

	for (int i = 0; i < buttonVect.size(); i++)
	{
		swordsPos.push_back({ buttonVect[i]->posButton.x - 180, buttonVect[i]->posButton.y });
		endPosEasing.push_back(swordsPos[i]);
		startPosEasing.push_back({ swordsPos[i].x - 30, swordsPos[i].y });
	}
}

void Multiplayer::InitWaitButton(GameData& _gameData)
{
	buttonVect.clear();
	//buttonVect.push_back(new Button("Assets/Images/Menu/IP.png", { (WORKING_WIDTH / 2.f), 550.f + GAP }, 1));
	buttonVect.push_back(new Button("Assets/Images/Menu/Apply.png", { (2 * WORKING_WIDTH / 3.f), 750.f + GAP }, 3));
	buttonVect.push_back(new Button("Assets/Images/Menu/Back.png", { (WORKING_WIDTH / 3.f), 750.f + GAP }, 3));
	timerEasing = 0.f;
	timerEasing2 = 0.f;
	timerEasingButton = 0.f;

	lastPos = (int)ButtonTypeMultiWait::CONFIRM;
	nextPos = (int)ButtonTypeMultiWait::CONFIRM;
	movingFront = false;
	buttonSelected = 0;

}

Multiplayer::Multiplayer(GameData& _gameData)
{
	InitComponents(_gameData);
	fade = new Fade(2.f, 25.f, false, true);
}

Multiplayer::~Multiplayer()
{
	delete fade;
	fade = nullptr;
}

void Multiplayer::InitComponents(GameData& _gameData)
{
	buttonVect.push_back(new Button("Assets/Images/Menu/Host.png", { (WORKING_WIDTH / 2.f), 550.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/Client.png", { (WORKING_WIDTH / 2.f), 650.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/BackBreak.png", { (WORKING_WIDTH / 2.f), 750.f + GAP }, 7, { 1, 1 }, true));
	timerEasingButton = 0.f;
	timerEasing = 0.f;
	timerEasing2 = 0.f;

	InitSwordVector();

	lastPos = (int)ButtonTypeMulti::HOST;
	nextPos = (int)ButtonTypeMulti::HOST;
	movingFront = false;
	_gameData.startPosMenuSwordSprite = swordsPos[0];
	_gameData.swordSpriteMenu.setPosition(_gameData.startPosMenuSwordSprite.x, _gameData.startPosMenuSwordSprite.y + GAP);
	buttonSelected = 0;

	reload = false;
	_gameData.network = new Network;
}

void Multiplayer::Reload()
{
	delete fade;
	fade = nullptr;

	fade = new Fade(2.f, 25.f, true, false);
}

void Multiplayer::Update(GameData& _gameData)
{
	_gameData.network->Update(_gameData.dt, &_gameData.window, _gameData);

	UpdateButtonsSelection(_gameData);
	UpdateButtonPressedEffect(_gameData);
	UpdateAllButtonsAnim(_gameData.dt);
	DownUpJoysticksButtons(_gameData);
	UpdateSwordPos(_gameData.dt, _gameData.swordSpriteMenu);
	if (buttonVect[buttonSelected]->GetIsPressed() && !swordHasSwoosh)
	{
		if (timerEasing < 1.f)
		{
			_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR", 100.f);
			swordCurrentPos = Easings::Lerp(swordsPos[lastPos], { swordsPos[lastPos].x + 320 ,swordsPos[lastPos].y }, timerEasing);
			timerEasing += _gameData.dt * 10;
			_gameData.swordSpriteMenu.setPosition(swordCurrentPos);
		}
	}
	bool temp = true;
	_gameData.animFlame->UpdateLeftToRight(_gameData.flame, _gameData.dt, 9, 0, temp, true);
}

void Multiplayer::UpdateSwordPos(float _dt, sf::Sprite& _swordSprite)
{
	nextPos = buttonSelected;
	_swordSprite.setPosition(swordCurrentPos);
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
}

void Multiplayer::Display(GameData& _gameData)
{
	_gameData.network->Display(&_gameData.window, _gameData.camera, &_gameData.flame);
	DisplayButtons(_gameData.window);
	if (_gameData.network->state == NetworkState::PENDING)
	{
		_gameData.window.draw(_gameData.swordSpriteMenu);
	}
}

void Multiplayer::Display3D(GameData& _gameData)
{
}

void Multiplayer::KeyPressed(GameData& _gameData)
{
	if (_gameData.network->state == NetworkState::CLIENTWAIT)
	{
		_gameData.network->SetEnteredCharServer(&_gameData.event);
		RightLeftKeysButtons(_gameData);
	}
	else
	{
		DownUpKeysButtons(_gameData);
	}

	if (_gameData.event.type == sf::Event::KeyPressed && _gameData.event.key.code == sf::Keyboard::Enter)
	{
		SetButtonSelectedPressed(_gameData);
		this->swordHasSwoosh = false;
	}
}

void Multiplayer::JoystickHandler(GameData& _gameData)
{
	if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
	{
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CROSS))
		{
			SetButtonSelectedPressed(_gameData);
			this->swordHasSwoosh = false;
		}
	}
	else
	{
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::A))
		{
			SetButtonSelectedPressed(_gameData);
			this->swordHasSwoosh = false;
		}
	}
}

void Multiplayer::JoystickPressed(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::JoystickButtonPressed)
	{
		JoystickHandler(_gameData);
	}
}

void Multiplayer::UpdateButtonPressedEffect(GameData& _gameData)
{
	if (_gameData.network->state == NetworkState::PENDING)
	{
		if (buttonVect[0]->isPressedFinished)
		{
			_gameData.tabPlayers.erase(_gameData.tabPlayers.begin(), _gameData.tabPlayers.end()); //erase the current nb player security

			_gameData.network->server = new Server{ PORT }; //create the server

			Player* temp = new Player(_gameData, 0); //init tabPlayer
			temp->Init();
			_gameData.tabPlayers.push_back(temp);
			_gameData.ChangeState(CHARACTER_SELECTION);
			_gameData.camera->SetCenter({ _gameData.camera->GetView().getCenter().x, 1499.f });
			_gameData.network->state = NetworkState::HOSTWAIT;
			_gameData.network->timerButton += _gameData.dt; //timer anti-spam
			_gameData.network->serverString = sf::IpAddress::getLocalAddress().toString(); // get address to show him
			*_gameData.network->tabStr[0] = sf::String(_gameData.network->serverString);
			_gameData.network->tabText[0]->setString(*_gameData.network->tabStr[0]);
			_gameData.multiplayer = true;
			_gameData.network->isRevealed = true;
		}
		else if (buttonVect[1]->isPressedFinished)
		{
			_gameData.tabPlayers.erase(_gameData.tabPlayers.begin(), _gameData.tabPlayers.end()); //erase the current nb player security

			_gameData.network->state = NetworkState::CLIENTWAIT; // state client wait to connect on a server
			_gameData.network->timerButton += _gameData.dt; //timer anti-spam
			InitWaitButton(_gameData);
		}
		else if (buttonVect[2]->isPressedFinished)
		{
			_gameData.ChangeState(MENU);
		}
	}
	else if (_gameData.network->state == NetworkState::CLIENTWAIT)
	{
		if (buttonVect[0]->isPressedFinished)
		{
			_gameData.network->state = NetworkState::PENDING;
			_gameData.network->timerButton += _gameData.dt;
		}
		else if (buttonVect[1]->isPressedFinished)
		{
			_gameData.network->client = new Client{ PORT , _gameData.network->serverString, _gameData.network->nameString, _gameData }; //init the client
			Player* temp = new Player(_gameData, _gameData.network->client->id);
			temp->Init();

			int i = 0;
			while (_gameData.tabPlayers.size() < _gameData.network->client->nbPlayer - 1) //add player to have the same as the server send just before -1 because the client is add at the end
			{
				Player* temp2 = new Player(_gameData, i);
				temp2->Init();
				_gameData.tabPlayers.push_back(temp2);
				i++;
			}
			_gameData.tabPlayers.push_back(temp);

			//change state
			_gameData.ChangeState(CHARACTER_SELECTION);
			_gameData.multiplayer = true;
		}
	}
}

void Multiplayer::MenuStateUpdate(GameData& _gameData)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(_gameData.window);
	_gameData.network->PendingStateUpdate(mousePos, _gameData.dt, _gameData);
	_gameData.network->WaitStateUpdate(mousePos, _gameData);
}

void Multiplayer::InGameStateUpdate(GameData& _gameData)
{
	_gameData.network->IngameStateUpdate(_gameData);
}

void Multiplayer::MenuStateDisplay(GameData& _gameData)
{
	if (_gameData.network->state == NetworkState::PENDING)
	{
		_gameData.network->PendingDisplay(&_gameData.window, _gameData.camera, &_gameData.flame);
	}
	else if (_gameData.network->state == NetworkState::HOSTWAIT)
	{
		_gameData.network->HostWaitDisplay(&_gameData.window);
	}
	else if (_gameData.network->state == NetworkState::CLIENTWAIT)
	{
		_gameData.network->ClientWaitDisplay(&_gameData.window, _gameData.camera, &_gameData.flame);
	}
}

void Multiplayer::SetAlpha(const float _alpha)
{
	fade->SetAlpha(_alpha);
	fade->SetColor(sf::Color(0, 0, 0, (sf::Uint8)_alpha));
}

void Multiplayer::FadeManagement(GameData& _gameData)
{
	fade->Update(_gameData.dt);

	if (fade->GetTimer() >= fade->GetTimerMax() && fade->GetIsFadingIn())
	{
		_gameData.ChangeState(CHARACTER_SELECTION);
	}
}