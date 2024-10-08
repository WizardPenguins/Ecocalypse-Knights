#include "stdafx.h"
#include "Multiplayer.h"
#include "Network.h"

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
}

void Multiplayer::Display(GameData& _gameData)
{
	_gameData.network->Display(&_gameData.window, _gameData.camera, &_gameData.flame);
}

void Multiplayer::Display3D(GameData& _gameData)
{
}

void Multiplayer::KeyPressed(GameData& _gameData)
{
	if (_gameData.network->state == NetworkState::CLIENTWAIT)
	{
		if (_gameData.network->wantReplaceServer)
		{
			_gameData.network->SetEnteredCharServer(&_gameData.event);
		}
		else if (_gameData.network->wantReplaceName)
		{
			_gameData.network->SetEnteredCharName(&_gameData.event);
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
		_gameData.network->ClientWaitDisplay(&_gameData.window , _gameData.camera, &_gameData.flame);
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