#include "stdafx.h"
#include "Camera.h"
#include "Player.hpp"
#include "Easings.h"
#include "Math.hpp"
#include "Entity.h"
#include "Mob.hpp"
#include "Boar.hpp"

Camera::Camera(GameData& _gameData)
{
	view.setSize(WORKING_WIDTH, WORKING_HEIGHT);
	view.setCenter(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f + GAP);
	ratio = WORKING_WIDTH;
	paralax.Init(_gameData);
	lastCenter = view.getCenter();

	//speed camera = speed player
	speed = { 450.f ,225.f };
	goTo = view.getCenter();
	moveMenuToPlayerSelection = false;
	timer = 0.f;
	lastRatio = ratio;
}

void Camera::Reload(GameData& _gameData)
{
	view.setSize(WORKING_WIDTH, WORKING_HEIGHT);
	(bool)(_gameData.levelState == LEVEL3) ? view.setCenter(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f + LEVEL3_GAP) : view.setCenter(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f);
	ratio = WORKING_WIDTH;
	lastCenter = view.getCenter();
	goTo = view.getCenter();
	moveMenuToPlayerSelection = false;
	timer = 0.f;
	lastRatio = ratio;
	bossDeath = true;
}
void Camera::ReloadMenu(GameData& _gameData)
{
	view.setSize(WORKING_WIDTH, WORKING_HEIGHT);
	view.setCenter(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f + GAP);
	ratio = WORKING_WIDTH;
	lastCenter = view.getCenter();
	goTo = view.getCenter();
	moveMenuToPlayerSelection = false;
	timer = 0.f;
	lastRatio = ratio;
	bossDeath = true;
}

void Camera::Move(GameData& _gameData)
{
	sf::Vector2f center;
	sf::Vector2f size = view.getSize();
	Wave actualWave = _gameData.actualWave;
	LevelState levelState = _gameData.levelState;
	if (_gameData.inCinematic)
	{
		center = { 7654.f, 1561.f };
		speed = { 450.f ,500.f };
		ratio = 1920.f;
	}
	else
	{
		speed = { 450.f ,225.f };
		if (_gameData.tabPlayers.size() == 1)
		{
			if (bossDeath)
			{
				ratio = 1500.f;
			}
			center = _gameData.tabPlayers[0]->GetPos();
			lastCenterX = view.getCenter().x;

			for (int i = 0; i < _gameData.tabMob.size(); i++)
			{


				if (_gameData.tabMob[i]->GetType() == TypeMob::Boss && _gameData.tabMob[i]->GetState() == MobState::Death && !bossDeath && _gameData.tabMob[i]->GetAnimFinish())
				{

					//center = _gameData.tabPlayers[0]->GetPos();
					//goTo = center;
					//lastCenter = center;
					bossDeath = true;
				}
				else if (_gameData.tabMob[i]->GetType() == TypeMob::Boss && _gameData.tabMob[i]->GetState() != MobState::Death)
				{
					ratio = WORKING_WIDTH;
					center = { size.x / 2.f + WORKING_WIDTH * actualWave , WORKING_HEIGHT - size.y / 2.f };
					bossDeath = false;
				}
			}
		}
		else
		{
			sf::Vector2f player1Pos = _gameData.tabPlayers[0]->GetPos();
			sf::Vector2f player2Pos = _gameData.tabPlayers[1]->GetPos();

			for (int i = 0; i < _gameData.tabPlayers.size(); i++)
			{
				sf::Vector2f actualPlayer = _gameData.tabPlayers[i]->GetPos();
				if (player1Pos.x > actualPlayer.x)
				{
					player1Pos = actualPlayer;
				}
				if (player2Pos.x < actualPlayer.x)
				{
					player2Pos = actualPlayer;
				}
			}

			center = Easings::Lerp(player2Pos, player1Pos, 0.5f);
			center.x += 200.f;
			sf::Vector2f vectorPlayer = Vector::CreateVector(player1Pos, player2Pos);

			float vectorLenght = Vector::GetVectorLength(vectorPlayer);

			lastCenterX = view.getCenter().x;

			if (bossDeath)
			{
				ratio = abs(((vectorLenght * WORKING_WIDTH / 2.f) / 100.f)) / 6.f;
				if (ratio < 1300.f)
				{
					ratio = 1300.f;
				}
				if (ratio > WORKING_WIDTH)
				{
					ratio = WORKING_WIDTH;
				}
				int nbPlayer = _gameData.tabPlayers.size();
				int nbPlayerDead = 0;
				for (int i = 0; i < _gameData.tabPlayers.size(); i++)
				{
					if (_gameData.tabPlayers[i]->GetLife() <= 0)
					{
						nbPlayerDead++;
					}
				}
				if (nbPlayerDead == nbPlayer - 1)
				{
					for (int i = 0; i < _gameData.tabPlayers.size(); i++)
					{
						if (_gameData.tabPlayers[i]->GetLife() != 0)
						{
							center = _gameData.tabPlayers[i]->GetPos();
							ratio = 1500.f;
						}
					}
				}

			}

			for (int i = 0; i < _gameData.tabMob.size(); i++)
			{

				if (_gameData.tabMob[i]->GetType() == TypeMob::Boss && _gameData.tabMob[i]->GetState() == MobState::Death && !bossDeath && _gameData.tabMob[i]->GetAnimFinish())
				{

					//center = _gameData.tabPlayers[0]->GetPos();
					//goTo = center;
					//lastCenter = center;
					bossDeath = true;
				}
				else if (_gameData.tabMob[i]->GetType() == TypeMob::Boss && _gameData.tabMob[i]->GetState() != MobState::Death)
				{
					ratio = WORKING_WIDTH;
					lastCenter = { size.x / 2.f + WORKING_WIDTH * actualWave , WORKING_HEIGHT - size.y / 2.f };
					bossDeath = false;
				}
			}
		}
		//collision
		if (!_gameData.inTravel)
		{
			if (center.x - size.x / 2.f < WORKING_WIDTH * actualWave)
			{
				center.x = size.x / 2.f + WORKING_WIDTH * actualWave;
			}
			else if (center.x + size.x / 2.f > WORKING_WIDTH + WORKING_WIDTH * actualWave)
			{
				center.x = WORKING_WIDTH - size.x / 2.f + WORKING_WIDTH * actualWave;
			}
			if (center.y - size.y / 2.f < 0)
			{
				center.y = size.y / 2.f;
			}
			else if (center.y + size.y / 2.f > WORKING_HEIGHT)
			{
				center.y = WORKING_HEIGHT - size.y / 2.f;
			}
		}
		else
		{
			sf::Vector2f playerLeft = _gameData.tabPlayers[0]->GetPos();
			float playerLeftSize = _gameData.tabPlayers[0]->GetHitBox()->getSize().x;
			if (_gameData.tabPlayers.size() > 1)
			{
				if (playerLeft.x > _gameData.tabPlayers[1]->GetPos().x)
				{
					playerLeft = _gameData.tabPlayers[1]->GetPos();
				}

				if (_gameData.tabPlayers[0]->GetLife() <= 0)
				{
					playerLeft = _gameData.tabPlayers[1]->GetPos();
				}
				else if (_gameData.tabPlayers[1]->GetLife() <= 0)
				{
					playerLeft = _gameData.tabPlayers[0]->GetPos();
				}
			}
			//if (center.x - size.x / 2.f < playerLeft.x + playerLeftSize / 2.f - size.x / 2.f)
			//{
			//	center.x = playerLeft.x + playerLeftSize / 2.f - size.x / 2.f;
			//}
			if (center.x - size.x / 2.f < WORKING_WIDTH * actualWave)
			{
				center.x = size.x / 2.f + WORKING_WIDTH * actualWave;
			}
			else if (center.x + size.x / 2.f > WORKING_WIDTH + WORKING_WIDTH * (actualWave + 1))
			{
				center.x = WORKING_WIDTH - size.x / 2.f + WORKING_WIDTH * (actualWave + 1);
			}
			/*if (center.y - size.y / 2.f < 0)
			{
				center.y = size.y / 2.f;
			}
			else*/
			if (levelState != LEVEL3)
			{
				if (center.y + size.y / 2.f > WORKING_HEIGHT)
				{
					center.y = WORKING_HEIGHT - size.y / 2.f;
				}
			}
			else
			{
				if (center.y + size.y / 2.f > 4500.f)
				{
					center.y = 4500.f - size.y / 2.f;
				}
			}

			if (playerLeft.x > WORKING_WIDTH * (actualWave + 1))
			{
				if (center.x > WORKING_WIDTH * (actualWave + 1))
				{

					_gameData.inTravel = false;
					_gameData.mobRespawn = true;
					_gameData.actualWave = (Wave)(actualWave + 1);
				}
			}
		}
	}
	if (levelState == LEVEL3 && center.y < 1760.f && !_gameData.tabPlayers[0]->GetFreeFall())
	{
		center.y = 1400.f;
	}
	goTo = center;

	if (goTo != lastCenter)
	{
		sf::Vector2f  direction = Vector::NormalizeVector(goTo - lastCenter);

		lastCenter.x += direction.x * speed.x * _gameData.dt;
		lastCenter.y += direction.y * speed.y * _gameData.dt;
		if (lastCenter.x + 15.f > goTo.x && goTo.x > lastCenter.x - 15.f)
		{
			lastCenter.x = goTo.x;
		}
		if (lastCenter.y + 15.f > goTo.y && goTo.y > lastCenter.y - 15.f)
		{
			lastCenter.y = goTo.y;
		}
	}
	if (_gameData.levelState)
		goToRatio = ratio;

	if (goToRatio != lastRatio)
	{
		if (lastRatio > goToRatio)
		{
			lastRatio -= _gameData.dt * 150.f;
		}
		else
		{
			lastRatio += _gameData.dt * 150.f;
		}
		if (lastRatio + 5.f > goToRatio && goToRatio > lastRatio - 5.f)
		{
			lastRatio = goToRatio;
		}
	}

	view.setCenter(lastCenter);
	view.setSize(lastRatio, lastRatio * 0.5625f);
	if (_gameData.levelState == LEVEL3)
	{
		view.setSize(1920, 1080);
	}
}

void Camera::TpPlayerLevel3(Player* _player, bool& _playerIsShrinks)
{
	if (_player->GetPos().x >= 2005.f && _player->GetPos().x <= 2105.f)
	{
		_player->SetPos({ _player->GetPos().x + 200.f, 4075.f });

		if (_player->GetInAir())
		{
			_player->SetInAir(false);
			_player->SetPosInAir(_player->GetPos());
		}
	}
	if (_player->GetPos().x >= 3373.f && _player->GetPos().x <= 3400.f && !_player->GetInAir())
	{
		_player->SetPos({ _player->GetPos().x, 4300.f });

		if (_player->GetInAir())
		{
			_player->SetInAir(false);
			_player->SetPosInAir(_player->GetPos());
		}
	}
	if (_player->GetPos().x >= 2005.f && _player->GetPos().x <= 3400.f)
	{
		(bool)(_player->GetLastDir() == LEFT) ? _player->GetSprite().setScale({ -SHRINKS,SHRINKS }) : _player->GetSprite().setScale({ SHRINKS,SHRINKS });
		_playerIsShrinks = true;
	}
	else
	{
		_playerIsShrinks = false;
	}
}

void Camera::Update(GameData& _gameData)
{
	if (_gameData.reloadCamera)
	{
		Reload(_gameData);
		_gameData.reloadCamera = false;
		paralax.Reload(_gameData);
	}
	if (_gameData.reloadCameraMenu)
	{
		ReloadMenu(_gameData);
		_gameData.reloadCameraMenu = false;
		paralax.Reload(_gameData);
	}

	Move(_gameData);
	paralax.ChangeAlphaFirstPlan(_gameData);
	float actualX = view.getCenter().x;

	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.tabPlayers[i]->GetPos().x < view.getCenter().x - view.getSize().x / 2.f)
		{

			_gameData.tabPlayers[i]->SetPos({ view.getCenter().x - view.getSize().x / 2.f + 1.f,_gameData.tabPlayers[i]->GetPos().y });
		}
	}

	if (!_gameData.inCinematic)
	{
		if (lastCenterX != actualX)
		{
			float dist = 0;
			if (lastCenterX < actualX)
			{
				dist = abs(actualX - lastCenterX);
				float speed = (dist / _gameData.dt) / 30.f;
				paralax.Update(_gameData, 1, speed);

			}
			else
			{
				dist = abs(lastCenterX - actualX);
				float speed = (dist / _gameData.dt) / 30.f;
				paralax.Update(_gameData, -1, speed);

			}
		}
	}
	paralax.UpdateAnim(_gameData);
}

void Camera::UpdatePause(GameData& _gameData)
{
	if (_gameData.reloadCamera)
	{
		Reload(_gameData);
		_gameData.reloadCamera = false;
		paralax.Reload(_gameData);
	}
	if (_gameData.reloadCameraMenu)
	{
		ReloadMenu(_gameData);
		_gameData.reloadCameraMenu = false;
		paralax.Reload(_gameData);
	}
	Move(_gameData);
	paralax.ChangeAlphaFirstPlan(_gameData);

	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.tabPlayers[i]->GetPos().x < view.getCenter().x - view.getSize().x / 2.f)
		{

			_gameData.tabPlayers[i]->SetPos({ view.getCenter().x - view.getSize().x / 2.f + 1.f,_gameData.tabPlayers[i]->GetPos().y });
		}
	}
}

void Camera::UpdateMenu(GameData& _gameData)
{
	if (_gameData.reloadCamera)
	{
		Reload(_gameData);
		_gameData.reloadCamera = false;
		paralax.Reload(_gameData);
	}
	if (_gameData.reloadCameraMenu)
	{
		ReloadMenu(_gameData);
		_gameData.reloadCameraMenu = false;
		paralax.Reload(_gameData);
	}
	if (moveMenuToPlayerSelection)
	{
		if (_gameData.gameState == MENU)
		{
			lastCenter = Easings::Lerp(lastCenter, { WORKING_WIDTH / 2.f,1500.f }, Easings::OutExpo(timer));

			timer += _gameData.dt / 512.f;
			if (timer > 1.f)
			{
				timer = 0.f;
				moveMenuToPlayerSelection = false;
			}

			view.setCenter(lastCenter);
			view.setSize(ratio, ratio * 0.5625f);
		}
		else if (_gameData.gameState == CHARACTER_SELECTION)
		{

			lastCenter = Easings::Lerp(lastCenter, { WORKING_WIDTH / 2.f,2115.f }, Easings::OutSine(timer));

			timer += _gameData.dt / 512.f;
			if (timer > 1.f)
			{
				timer = 0.f;
				moveMenuToPlayerSelection = false;
			}

			view.setCenter(lastCenter);
			view.setSize(ratio, ratio * 0.5625f);

		}
	}
}

void Camera::UpdateArena(void)
{
	view.setSize(WORKING_WIDTH, WORKING_HEIGHT);
	view.setCenter(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f);
}

void Camera::Display(GameData& _gameData)
{
	paralax.Display(_gameData);
}

void Camera::Display3D(GameData& _gameData)
{
	paralax.Display(_gameData);
}

void Camera::DisplayFirstPlan(GameData& _gameData)
{
	paralax.DisplayFirstPlan(_gameData);
}

void Camera::DisplayFirstPlan(sf::RenderTexture& _red, sf::RenderTexture& _blue, LevelState _level)
{
	paralax.DisplayFirstPlan(_red, _blue, _level);
}

sf::View& Camera::GetView()
{
	return view;
}

const float Camera::GetRatioX()
{
	return ratio;
}
const float Camera::GetRatioY()
{
	return ratio * 0.5625f;
}

void Camera::SetRatio(float _ratio)
{
	ratio = _ratio;
}

void Camera::SetGoTo(sf::Vector2f _goTo)
{
	goTo = _goTo;

}

void Camera::SetMoveMenuToPlayerSelection(bool _isMoving)
{
	moveMenuToPlayerSelection = _isMoving;
}
const bool Camera::GetMoveMenuToPlayerSelection(void)
{
	return moveMenuToPlayerSelection;
}
void Camera::SetDefaultCenter(void)
{
	lastCenterForRealWallah = view.getCenter();
	view.setCenter(WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f);
}

void Camera::SetDefaultZoom(void)
{
	lastZoomForRealWallah = view.getSize();
	view.setSize(WORKING_WIDTH, WORKING_HEIGHT);
}

void Camera::SetLastCenter(void)
{
	view.setCenter(lastCenterForRealWallah);
}

void Camera::SetLastZoom(void)
{
	view.setCenter(lastZoomForRealWallah);
}

void Camera::SetCenter(sf::Vector2f _center)
{
	view.setCenter(_center);
	lastCenter = _center;
	goTo = _center;
}

Paralax& Camera::GetParalax()
{
	return paralax;
}

void Camera::CameraForFilterBlue()
{
	view.setCenter(view.getCenter().x + 20, view.getCenter().y);
}

void Camera::ResetPosCameraAfterFilterBlue()
{
	view.setCenter(view.getCenter().x - 20, view.getCenter().y);
}



