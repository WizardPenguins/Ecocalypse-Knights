#include "stdafx.h"
#include "Arena.h"
#include "GameStates.h"
#include "CharacterSelection.h"
#include "EntityManager.h"
#include "EntityManagerPVP.hpp"
#include "TroaDimmenssionSahMaire.h"
#include "Scarecrow.hpp"

Arena::Arena(GameData& _gameData)
{
	InitComponents(_gameData);
}

void Arena::InitComponents(GameData& _gameData)
{
	crownHud = Utils::LoadSprite("Assets/Images/HUD/CrownHud.png", CENTER);

	winSpriteLeft = Utils::LoadSprite("Assets/Images/HUD/LeftTeamFightVictory.png", TOP_LEFT, 6, 6);
	winSpriteRight = Utils::LoadSprite("Assets/Images/HUD/RightTeamFightVictory.png", TOP_LEFT, 6, 6);
	winSpriteMiddle = Utils::LoadSprite("Assets/Images/HUD/MiddleTeamFightVictory.png", CENTER, 6, 5);
	animeLeft.CreateLeftToRight(winSpriteLeft, 6, 6, winSpriteLeft.getTexture()->getSize());
	animeRight.CreateLeftToRight(winSpriteRight, 6, 6, winSpriteRight.getTexture()->getSize());
	animeMidle.CreateLeftToRight(winSpriteMiddle, 6, 5, winSpriteMiddle.getTexture()->getSize());

	conffeti = Utils::LoadSprite("Assets/Images/HUD/Confetti.png", TOP_LEFT, 9, 3);
	animeConffeti.CreateLeftToRight(conffeti, 9, 3, conffeti.getTexture()->getSize());

	fightStart = Utils::LoadSprite("Assets/Images/HUD/Fight.png", CENTER, 10, 2);
	animeFightStart.CreateLeftToRight(fightStart, 10, 2, fightStart.getTexture()->getSize());

	tutoSprite = Utils::LoadSprite("Assets/Images/HUD/Controls.png", CENTER);
	_gameData.reloadArena = true;
}

void Arena::InitClasses(GameData& _gameData)
{

}

void Arena::FadeManagement(GameData& _gameData)
{
	fade->Update(_gameData.dt);
	if (fade->GetTimer() >= fade->GetTimerMax())
	{
		if (leftArena)
		{
			_gameData.ChangeState(MENU);
			_gameData.audio->StopAllMusics();
			_gameData.swordSpriteMenu.setPosition(_gameData.startPosMenuSwordSprite);
			_gameData.reloadArena = true;

			lineAnime = 0;

			_gameData.camera->GetParalax().Reload(_gameData);
			_gameData.reloadCameraMenu = true;
			_gameData.reloadCharacterSelection = true;
			_gameData.reloadMenu = true;
		}
	}
}

void Arena::Reload(GameData& _gameData)
{
	_gameData.hud->Update(_gameData);
	if (fade == nullptr)
	{
		delete fade;
		fade = nullptr;
	}
	fade = new Fade(2.f, false, true);

	leftArena = false;
	lineAnime = 0;
	showTuto = true;
	displayFight = true;
	posWinCompteur = { WORKING_WIDTH / 2, WORKING_HEIGHT / 2 };
	_gameData.sortVector.clear();
	_gameData.tabMob.clear();
	_gameData.camera->Reload(_gameData);
	_gameData.projectile.clear();
	_gameData.entityManPVP->Reload();
	_gameData.tabPlayers.clear();
	_gameData.tabBuilding.clear();
	_gameData.camera->GetParalax().Reload(_gameData);
	compteurWinPlayer.clear();
	compteurWinTeam.clear();
	textWinCompteur.clear();
	displayBestPlayer.clear();
	teamFlag.clear();
	_gameData.actualWave = WAVE1;
	_gameData.levelState = LEVEL1;
	timerWin = TIMER_WIN;
	uptadeWinCondition = true;
	alpha = 0;
	diplayWinSprite = false;
	displayTeamScore = false;
	timerEasing = 0.f;
	timerSin = 0.f;
	animeLeft.currentFrame = 0;
	animeRight.currentFrame = 0;
	animeMidle.currentFrame = 0;
	animeFightStart.currentFrame = 0;
	lineAnimeFight = 0;

	int compteurTeam1 = 0;
	int compteurTeam2 = 0;
	if (_gameData.teamfight)
	{
		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{
			(bool)(_gameData.playerTeam[i] == 0) ? compteurTeam1++ : compteurTeam2++;
		}
	}

	unsigned int joystickCount = sf::Joystick::isConnected(0) + sf::Joystick::isConnected(1) + sf::Joystick::isConnected(2) + sf::Joystick::isConnected(3);
	for (int i = 0; i < joystickCount; i++)
	{
		Player* player = new Player(_gameData, i);
		_gameData.tabPlayers.push_back(std::move(player));
		if (i == joystickCount - 1)
		{
			player->Init();
		}

		_gameData.tabPlayers[i]->SetAgility(7);
		_gameData.tabPlayers[i]->SetWisdom(7);
		_gameData.tabPlayers[i]->SetStatsValue();

		_gameData.tabPlayers[i]->SetPlayerSelected(_gameData.playerSelected[i]);
		if (_gameData.teamfight)
		{
			compteurWinTeam.push_back(0);
			(bool)(_gameData.playerTeam[i] == 0) ? teamFlag.push_back(Utils::LoadSprite("Assets/Images/HUD/Team1Flag.png", CENTER)) : teamFlag.push_back(Utils::LoadSprite("Assets/Images/HUD/Team2Flag.png", CENTER));
			_gameData.tabPlayers[i]->SetPlayerTeam(_gameData.playerTeam[i]);
			_gameData.tabPlayers[i]->SetLife(_gameData.tabPlayers[i]->GetMaxLife());
			_gameData.tabPlayers[i]->SetMana(_gameData.tabPlayers[i]->GetMaxMana());

			if (_gameData.playerTeam[i] == 0)
			{
				_gameData.tabPlayers[i]->SetPos({ 385.f,720.f + compteurTeam1 * 80 });
				compteurTeam1--;
			}
			else
			{
				_gameData.tabPlayers[i]->SetPos({ 1790.f,750.f + compteurTeam2 * 80 });
				compteurTeam2--;
				_gameData.tabPlayers[i]->SetLastDir(LEFT);
			}

		}
		else
		{
			CrownOnPlayer.push_back(Utils::LoadSprite("Assets/Images/HUD/CrownOnPlayer.png", CENTER));
			switch (i)
			{
			case 0:
				_gameData.tabPlayers[i]->SetPos({ 385.f,720.f });
				break;
			case 1:
				_gameData.tabPlayers[i]->SetPos({ 1790.f,750.f });
				_gameData.tabPlayers[i]->SetLastDir(LEFT);
				break;
			case 2:
				_gameData.tabPlayers[i]->SetPos({ 955.f,430.f });
				break;
			case 3:
				_gameData.tabPlayers[i]->SetPos({ 935.f,990.f });
				break;
			}

			compteurWinPlayer.push_back(0);
			displayBestPlayer.push_back(false);
			sf::Text* temp = new sf::Text();
			Utils::CreateText(*temp, *_gameData.GameFont, "x" + std::to_string(compteurWinPlayer[i]), 30, sf::Color::Black, true);
			temp->setPosition({ _gameData.hud->GetPosIcon(i).x, 250 });
			textWinCompteur.push_back(temp);
		}

	}

	_gameData.hud->Update(_gameData);
}

void Arena::ReloadInterFight(GameData& _gameData)
{
	timerWin = TIMER_WIN;
	uptadeWinCondition = true;
	alpha = 0;
	diplayWinSprite = false;
	displayTeamScore = false;
	timerEasing = 0.f;
	displayFight = true;
	lineAnimeFight = 0;

	animeFightStart.currentFrame = 0;
	animeMidle.currentFrame = 0;

	_gameData.projectile.clear();

	for (int i = 0; i < _gameData.sortVector.size(); i++)
	{
		if (_gameData.sortVector[i]->sort == SortVector::Projectile || _gameData.sortVector[i]->sort == SortVector::Ice)
		{
			_gameData.sortVector.erase(_gameData.sortVector.begin() + i);
		}
	}

	int compteurTeam1 = 0;
	int compteurTeam2 = 0;

	if (_gameData.teamfight)
	{
		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{
			(bool)(_gameData.playerTeam[i] == 0) ? compteurTeam1++ : compteurTeam2++;
		}
	}

	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		_gameData.tabPlayers[i]->SetPlayerState(IDLE);
		_gameData.tabPlayers[i]->DesactivateHitbox();
		_gameData.tabPlayers[i]->SetInAir(false);
		_gameData.tabPlayers[i]->SetLife(_gameData.tabPlayers[i]->GetMaxLife());
		_gameData.tabPlayers[i]->SetMana(_gameData.tabPlayers[i]->GetMaxMana());
		_gameData.tabPlayers[i]->ResetShieldLife();

		if (_gameData.teamfight)
		{
			if (_gameData.playerTeam[i] == 0)
			{
				_gameData.tabPlayers[i]->SetPos({ 385.f,720.f + compteurTeam1 * 80 - 80 });
				compteurTeam1--;
				_gameData.tabPlayers[i]->SetLastDir(RIGHT);
			}
			else
			{
				_gameData.tabPlayers[i]->SetPos({ 1790.f,750.f + compteurTeam2 * 80 - 80 });
				compteurTeam2--;
				_gameData.tabPlayers[i]->SetLastDir(LEFT);
			}
		}
		else
		{
			switch (i)
			{
			case 0:
				_gameData.tabPlayers[i]->SetPos({ 385.f,720.f });
				break;
			case 1:
				_gameData.tabPlayers[i]->SetPos({ 1790.f,750.f });
				break;
			case 2:
				_gameData.tabPlayers[i]->SetPos({ 955.f,430.f });
				break;
			case 3:
				_gameData.tabPlayers[i]->SetPos({ 935.f,990.f });
				break;
			}
		}

	}
}

void Arena::UpdatePlayers(GameData& _gameData)
{
	if (diplayWinSprite)
	{
		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{
			_gameData.tabPlayers[i]->Update(_gameData, _gameData.dt / 3.5f, false);
		}
	}
	else
	{
		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{
			if (displayFight)
			{
				_gameData.tabPlayers[i]->Update(_gameData, _gameData.dt, true);
			}
			else
			{
				_gameData.tabPlayers[i]->Update(_gameData, _gameData.dt, false);
			}
		}
	}
}

bool Arena::ArrowPointCollision(sf::Image& _image, std::vector<Projectile>& _tabProjectile, int _id)
{
	float arrowX;
	float arrowY;

	if (_tabProjectile[_id].direction == 1)
	{
		arrowX = (float)(_tabProjectile[_id].pos.x);
	}
	else if (_tabProjectile[_id].direction == -1)
	{
		arrowX = (float)(_tabProjectile[_id].pos.x + _tabProjectile[_id].hitBox->getSize().x);
	}
	else
	{
		return false;
	}

	arrowY = (float)(_tabProjectile[_id].posAutor.y + 5.f);

	return (_image.getPixel((unsigned int)arrowX, (unsigned int)arrowY) == sf::Color::Red);
}

void Arena::Music(GameData& _gameData)
{
	//_gameData.audio->PlayMusic("INGAME_ARENA", true);
}

void Arena::ArenaHandler(GameData& _gameData)
{
	for (int i = (int)(_gameData.projectile.size() - 1); i >= 0; i--)
	{
		_gameData.projectile.at(i).Update(_gameData.dt);

		if (ArrowPointCollision(_gameData.tempImage1, _gameData.projectile, i))
		{
			_gameData.projectile.erase(_gameData.projectile.begin() + i);
		}
	}
	_gameData.entityManPVP->UpdatePVP(_gameData.tabPlayers, _gameData.projectile, _gameData.sortVector, _gameData.tabBuilding, _gameData.teamfight, *_gameData.GameFont, _gameData.heartHit, *_gameData.audio, _gameData.camera->GetView(), _gameData.dt, _gameData.friendShip);

	if (displayFight && !showTuto)
	{
		bool temp = true;
		animeFightStart.UpdateLeftToRight(fightStart, _gameData.dt, 10, lineAnimeFight, temp, false);


		if (animeFightStart.currentFrame == animeFightStart.maxFrame && lineAnimeFight == 1)
		{
			displayFight = false;
		}

		if (animeFightStart.currentFrame == animeFightStart.maxFrame)
		{
			animeFightStart.currentFrame = 0;
			lineAnimeFight = 1;
		}
	}

	UpdatePlayers(_gameData);

	_gameData.camera->UpdateArena();
	_gameData.hud->Update(_gameData);

	Music(_gameData);
}

void Arena::UpdateWinCondition(GameData& _gameData)
{
	if (!_gameData.teamfight && uptadeWinCondition)
	{
		int death = 0;
		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{
			if (_gameData.tabPlayers[i]->GetLife() <= 0)
			{
				death++;
			}
		}

		if (death == _gameData.tabPlayers.size() - 1)
		{
			for (int i = 0; i < _gameData.tabPlayers.size(); i++)
			{
				if (_gameData.tabPlayers[i]->GetLife() > 0)
				{
					uptadeWinCondition = false;
					compteurWinPlayer[i]++;
					textWinCompteur[i]->setString("x" + std::to_string(compteurWinPlayer[i]));
					winSprite = Utils::LoadSprite("Assets/Images/HUD/ArenaWin" + std::to_string(_gameData.tabPlayers[i]->GetPlayerSelected() + 1) + ".png", CENTER, 1, 1);
					winSprite.setColor(sf::Color(255, 255, 255, alpha));
					diplayWinSprite = true;
				}
			}
		}
	}
	else if (_gameData.teamfight && uptadeWinCondition)
	{
		int nbPlayerTeam1 = 0;
		int nbPlayerTeam2 = 0;

		for (int i = 0; i < _gameData.playerTeam.size(); i++)
		{
			(bool)(_gameData.playerTeam[i] == 0) ? nbPlayerTeam1++ : nbPlayerTeam2++;
		}

		bool deathTeam1 = false;
		bool deathTeam2 = false;

		int nbPlayerAliveTeam1 = nbPlayerTeam1;
		int nbPlayerAliveTeam2 = nbPlayerTeam2;

		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{

			if (_gameData.tabPlayers[i]->GetLife() <= 0)
			{
				(bool)(_gameData.playerTeam[i] == 0) ? nbPlayerAliveTeam1-- : nbPlayerAliveTeam2--;
			}
		}

		if (nbPlayerAliveTeam1 == 0)
		{
			deathTeam1 = true;
		}

		if (nbPlayerAliveTeam2 == 0)
		{
			deathTeam2 = true;
		}

		if (deathTeam1)
		{
			uptadeWinCondition = false;
			compteurWinTeam[1]++;
			winSprite = Utils::LoadSprite("Assets/Images/HUD/ArenaTeam2win.png", CENTER, 1, 1);
			winSprite.setColor(sf::Color(255, 255, 255, alpha));
			animeRight.currentFrame = 0;
			diplayWinSprite = true;
			displayTeamScore = false;
		}

		if (deathTeam2)
		{
			uptadeWinCondition = false;
			compteurWinTeam[0]++;
			winSprite = Utils::LoadSprite("Assets/Images/HUD/ArenaTeam1win.png", CENTER, 1, 1);
			winSprite.setColor(sf::Color(255, 255, 255, alpha));
			animeLeft.currentFrame = 0;
			diplayWinSprite = true;
			displayTeamScore = false;
		}
	}
}

void Arena::UpdateWinFade(GameData& _gameData)
{
	if (diplayWinSprite)
	{
		timerWin -= _gameData.dt;
		if (displayTeamScore)
		{

			bool temp = true;
			bool temp2 = false;
			(bool)(compteurWinTeam[0] > 0) ? animeLeft.UpdateLeftToRight(winSpriteLeft, _gameData.dt, 7, compteurWinTeam[0], temp, false) : animeLeft.UpdateLeftToRight(winSpriteLeft, _gameData.dt, 7, compteurWinTeam[0], temp2, false);
			(bool)(compteurWinTeam[1] > 0) ? animeRight.UpdateLeftToRight(winSpriteRight, _gameData.dt, 7, compteurWinTeam[1], temp, false) : animeRight.UpdateLeftToRight(winSpriteRight, _gameData.dt, 7, compteurWinTeam[1], temp2, false);

			if (compteurWinTeam[0] != 5 && compteurWinTeam[1] != 5)
			{
				(bool)(compteurWinTeam[0] == compteurWinTeam[1]) ? animeMidle.UpdateLeftToRight(winSpriteMiddle, _gameData.dt, 8, 0, temp2, false) : ((bool)(compteurWinTeam[0] > compteurWinTeam[1]) ? animeMidle.UpdateLeftToRight(winSpriteMiddle, _gameData.dt, 8, 2, temp, true) : animeMidle.UpdateLeftToRight(winSpriteMiddle, _gameData.dt, 8, 1, temp, true));
			}
			else if (compteurWinTeam[0] == 5 || compteurWinTeam[1] == 5)
			{
				(bool)(compteurWinTeam[0] == 5) ? animeMidle.UpdateLeftToRight(winSpriteMiddle, _gameData.dt, 5, 4, temp, false) : animeMidle.UpdateLeftToRight(winSpriteMiddle, _gameData.dt, 5, 3, temp, false);
			}
		}

		if (timerWin >= TIMER_WIN - 0.5f)
		{
			sf::Vector2f temp;
			temp = Easings::Lerp({ 0,0 }, { 255,0 }, Easings::OutCirc(timerEasing));
			timerEasing += _gameData.dt * 2;
			alpha = temp.x;
			winSprite.setColor(sf::Color(255, 255, 255, alpha));
		}
		else if (timerWin > (float)(TIMER_WIN - 0.5f) - _gameData.dt && timerWin < (float)(TIMER_WIN - 0.5f))
		{
			alpha = 255;
			winSprite.setColor(sf::Color(255, 255, 255, alpha));
			timerEasing = 0.f;
		}
		else if (timerWin >= 2 && timerWin <= TIMER_WIN - 1.5f)
		{
			sf::Vector2f temp;
			temp = Easings::Lerp({ 255,0 }, { 0,0 }, Easings::InCirc(timerEasing));
			timerEasing += _gameData.dt * 2;
			alpha = temp.x;
			winSprite.setColor(sf::Color(255, 255, 255, alpha));
		}
		else if (timerWin > (float)(TIMER_WIN - 2) - _gameData.dt && timerWin < (float)(TIMER_WIN - 2))
		{
			if (_gameData.teamfight)
			{
				displayTeamScore = true;
			}
			else
			{
				timerWin = 0.f;
			}
		}
		else if (timerWin < 0.f)
		{
			timerEasing = 0.f;

			if (!_gameData.teamfight)
			{
				switch (_gameData.choasWin)
				{
				case ChaosWin::FT5:
					for (int i = 0; i < _gameData.tabPlayers.size(); i++)
					{
						if (compteurWinPlayer[i] == 5)
						{
							leftArena = true;
						}
					}
					if (leftArena)
					{
						bool temp = true;
						animeConffeti.UpdateLeftToRight(conffeti, _gameData.dt, 10, lineAnime, temp, false);

						if (animeConffeti.currentFrame == animeConffeti.maxFrame && lineAnime == 2)
						{
							fade->SetIsFadingIn(true);
						}

						if (animeConffeti.currentFrame == animeConffeti.maxFrame)
						{
							animeConffeti.currentFrame = 0;
							lineAnime++;
						}
					}
					break;
				case ChaosWin::FT10:
					for (int i = 0; i < _gameData.tabPlayers.size(); i++)
					{
						if (compteurWinPlayer[i] == 10)
						{
							leftArena = true;
						}
					}
					if (leftArena)
					{
						bool temp = true;
						animeConffeti.UpdateLeftToRight(conffeti, _gameData.dt, 10, lineAnime, temp, false);

						if (animeConffeti.currentFrame == animeConffeti.maxFrame && lineAnime == 2)
						{
							fade->SetIsFadingIn(true);
						}

						if (animeConffeti.currentFrame == animeConffeti.maxFrame)
						{
							animeConffeti.currentFrame = 0;
							lineAnime++;
						}
					}
					break;
				}
			}
			else if (_gameData.teamfight)
			{
				for (int i = 0; i < 2; i++)
				{
					if (compteurWinTeam[i] == 5)
					{
						leftArena = true;
					}
				}
				if (leftArena)
				{
					bool temp = true;
					animeConffeti.UpdateLeftToRight(conffeti, _gameData.dt, 10, lineAnime, temp, false);

					if (animeConffeti.currentFrame == animeConffeti.maxFrame && lineAnime == 2)
					{
						fade->SetIsFadingIn(true);
					}

					if (animeConffeti.currentFrame == animeConffeti.maxFrame)
					{
						animeConffeti.currentFrame = 0;
						lineAnime++;
					}
				}
			}
			if (!leftArena)
			{
				ReloadInterFight(_gameData);
			}
		}
	}
}

void Arena::UpdateFloatingHUD(GameData& _gameData)
{
	timerSin += _gameData.dt;
	if (_gameData.teamfight)
	{
		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{
			if (_gameData.playerTeam[i] == 0)
			{
				if (_gameData.tabPlayers[i]->GetInAir())
				{
					teamFlag[i].setPosition({ _gameData.tabPlayers[i]->GetPos().x, _gameData.tabPlayers[i]->GetPosInAir().y - 230 + 10 * sinf(timerSin * 6) });
				}
				else
				{
					teamFlag[i].setPosition({ _gameData.tabPlayers[i]->GetPos().x, _gameData.tabPlayers[i]->GetPos().y - 230 + 10 * sinf(timerSin * 6) });
				}
			}
			else if (_gameData.playerTeam[i] == 1)
			{
				if (_gameData.tabPlayers[i]->GetInAir())
				{
					teamFlag[i].setPosition({ _gameData.tabPlayers[i]->GetPos().x, _gameData.tabPlayers[i]->GetPosInAir().y - 230 + 10 * sinf(timerSin * 6) });
				}
				else
				{
					teamFlag[i].setPosition({ _gameData.tabPlayers[i]->GetPos().x, _gameData.tabPlayers[i]->GetPos().y - 230 + 10 * sinf(timerSin * 6) });
				}
			}
		}
	}
	else
	{
		int hightWin = 0;
		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{
			displayBestPlayer[i] = false;
			if (compteurWinPlayer[i] >= hightWin && compteurWinPlayer[i] != 0)
			{
				hightWin = compteurWinPlayer[i];
			}
		}

		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{
			if (compteurWinPlayer[i] == hightWin && hightWin != 0)
			{
				displayBestPlayer[i] = true;
			}

			if (displayBestPlayer[i])
			{
				if (_gameData.tabPlayers[i]->GetInAir())
				{
					CrownOnPlayer[i].setPosition({ _gameData.tabPlayers[i]->GetPos().x, _gameData.tabPlayers[i]->GetPosInAir().y - 200 + 10 * sinf(timerSin * 6) });
				}
				else
				{
					CrownOnPlayer[i].setPosition({ _gameData.tabPlayers[i]->GetPos().x, _gameData.tabPlayers[i]->GetPos().y - 200 + 10 * sinf(timerSin * 6) });
				}

				(bool)(_gameData.tabPlayers[i]->GetLastDir() == LEFT) ? CrownOnPlayer[i].setScale(-1, 1) : CrownOnPlayer[i].setScale(1, 1);
			}
		}
	}
}

void Arena::SkipTuto(GameData& _gameData)
{
	if (showTuto)
	{
		if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CIRCLE))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CROSS))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::L1))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::L2))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::LEFT_STICK_PUSH))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::R1))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::R2))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::RIGHT_STICK_PUSH))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::SQUARE))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::TRIANGLE))
			{
				showTuto = false;
			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::A))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::B))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::LB))
			{
				showTuto = false;
			}
			
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::LEFT_STICK_PUSH))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::RB))
			{
				showTuto = false;
			}
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::RIGHT_STICK_PUSH))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::SELECT))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::X))
			{
				showTuto = false;
			}

			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::Y))
			{
				showTuto = false;
			}
		}
	}
}

void Arena::Update(GameData& _gameData)
{
	if (_gameData.reloadArena)
	{
		Reload(_gameData);
		_gameData.reloadArena = false;
	}

	
	ArenaHandler(_gameData);
	SkipTuto(_gameData);
	UpdateFloatingHUD(_gameData);
	UpdateWinCondition(_gameData);
	UpdateWinFade(_gameData);
	FadeManagement(_gameData);
}

void Arena::DisplayPlayers(GameData& _gameData)
{
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		_gameData.tabPlayers[i]->Display(_gameData.window, *_gameData.audio, _gameData.camera, _gameData.levelState);
	}
}

void Arena::ShadowDisplay(GameData& _gameData)
{
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		_gameData.shadow.setScale({ 1.5f,1.5f });
		if (_gameData.tabPlayers[i]->GetLastDir() == LEFT)
		{
			Utils::BlitSprite(_gameData.shadow, { _gameData.tabPlayers[i]->GetPos().x + 10, _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rt);
		}
		else
		{
			Utils::BlitSprite(_gameData.shadow, { _gameData.tabPlayers[i]->GetPos().x - 10, _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rt);
		}

	}
}

void Arena::ProjectileShadowDisplay(GameData& _gameData)
{
	for (int i = 0; i < _gameData.projectile.size(); i++)
	{
		if (_gameData.projectile.at(i).projetileType == TypeProjectile::ARROW)
		{
			_gameData.shadow.setScale({ 1.f,0.7f });
			if (_gameData.projectile.at(i).direction == -1)
			{
				Utils::BlitSprite(_gameData.shadow, { _gameData.projectile.at(i).pos.x + 50 , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rt);
			}
			else
			{
				Utils::BlitSprite(_gameData.shadow, { _gameData.projectile.at(i).pos.x - 50 , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rt);
			}
		}
		else
		{
			_gameData.shadow.setScale({ 0.7f,0.7f });
			if (_gameData.projectile.at(i).direction == -1)
			{
				Utils::BlitSprite(_gameData.shadow, { _gameData.projectile.at(i).pos.x + 30 , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rt);
			}
			else
			{
				Utils::BlitSprite(_gameData.shadow, { _gameData.projectile.at(i).pos.x - 30 , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rt);
			}
		}

	}
}

void Arena::EntitiesDisplay(GameData& _gameData)
{
	for (int i = 0; i < _gameData.sortVector.size(); i++)
	{
		if (_gameData.sortVector.at(i)->sort == SortVector::Player)
		{
			_gameData.tabPlayers.at(_gameData.sortVector.at(i)->i)->Display(_gameData.window, *_gameData.audio, _gameData.camera, _gameData.levelState);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Ice)
		{
			_gameData.tabPlayers.at(_gameData.sortVector.at(i)->i)->DisplayIceSpell(_gameData.window);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Building)
		{
			sf::Vector2f pos = _gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite.getPosition();
			if (!_gameData.is3DOn)
			{
				if (_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->isAnim)
				{
					bool temp = true;

					_gameData.camera->GetParalax().animSort[_gameData.sortVector.at(i)->i]->UpdateLeftToRight(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, _gameData.dt, 5, 0, temp, true);
					Utils::BlitSprite(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, pos, _gameData.window);
				}
				else
				{
					Utils::BlitSprite(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, pos, _gameData.window);

				}
			}
			else
			{
				if (_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->isAnim)
				{
					if (_gameData.rightEye)
					{
						bool temp = true;
						Utils::BlitSprite(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, { pos.x + (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f),pos.y }, _gameData.window);
					}
					else
					{
						bool temp = true;
						Utils::BlitSprite(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, { pos.x - (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f),pos.y }, _gameData.window);
					}
				}
				else
				{
					if (_gameData.rightEye)
					{
						Utils::BlitSprite(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, { pos.x + (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f),pos.y }, _gameData.window);
					}
					else
					{
						Utils::BlitSprite(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, { pos.x - (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f),pos.y }, _gameData.window);
					}
				}
			}
		}
		else
		{
			_gameData.projectile.at(_gameData.sortVector.at(i)->i).Display(_gameData.window);
		}
	}
}

void Arena::ShadowDisplay3D(GameData& _gameData)
{
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.tabPlayers[i]->GetState() != EAT_BY_BOSS)
		{
			_gameData.shadow.setScale({ 1.5f,1.5f });
			if (_gameData.tabPlayers[i]->GetLastDir() == LEFT)
			{
				Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x + 10) + (TROAD_GAP * (-1 * _gameData.tabPlayers[i]->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtRed);
				Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x + 10) - (TROAD_GAP * (-1 * _gameData.tabPlayers[i]->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtBlue);
			}
			else
			{
				Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x - 10) + (TROAD_GAP * (-1 * _gameData.tabPlayers[i]->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtRed);
				Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x - 10) - (TROAD_GAP * (-1 * _gameData.tabPlayers[i]->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtBlue);
			}
		}
	}
}

void Arena::ProjectileShadowDisplay3D(GameData& _gameData)
{
	for (int i = 0; i < _gameData.projectile.size(); i++)
	{
		if (_gameData.projectile.at(i).projetileType == TypeProjectile::ARROW)
		{
			_gameData.shadow.setScale({ 1.f,0.7f });
			if (_gameData.projectile.at(i).direction == -1)
			{
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x + 50) + (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f) , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtRed);
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x + 50) - (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f) , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtBlue);
			}
			else
			{
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x - 50) + (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f) , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtRed);
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x - 50) - (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f) , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtBlue);
			}
		}
		else
		{
			_gameData.shadow.setScale({ 0.7f,0.7f });
			if (_gameData.projectile.at(i).direction == -1)
			{
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x + 30) + (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f), _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtRed);
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x + 30) - (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f) , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtBlue);
			}
			else
			{
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x - 30) + (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f), _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtRed);
				Utils::BlitSprite(_gameData.shadow, { (_gameData.projectile.at(i).pos.x - 30) - (TROAD_GAP * (-1 * _gameData.projectile.at(i).posAutor.y + WORKING_HEIGHT) / 100.f) , _gameData.projectile.at(i).posAutor.y }, 0, *_gameData.rtBlue);
			}
		}

	}
}

void Arena::DisplayPoisonSpell(std::vector<Player*>& _player, sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	for (int i = 0; i < _player.size(); i++)
	{
		if (_player[i]->GetPlayerSelected() == 1)
		{
			_player[i]->DisplayViperSpell(_red, _blue);
		}
	}
}

void Arena::EntitiesDisplay3D(GameData& _gameData)
{
	for (int i = 0; i < _gameData.sortVector.size(); i++)
	{
		if (_gameData.sortVector.at(i)->sort == SortVector::Player)
		{
			_gameData.tabPlayers[_gameData.sortVector.at(i)->i]->Display(*_gameData.audio, _gameData.renderTextureRed, _gameData.renderTextureBlue, _gameData.camera, _gameData.levelState);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Ice)
		{
			_gameData.tabPlayers.at(_gameData.sortVector.at(i)->i)->DisplayIceSpell(_gameData.renderTextureRed, _gameData.renderTextureBlue);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Mob)
		{
			_gameData.entityMan->DrawLifeBar(_gameData.tabMob.at(_gameData.sortVector.at(i)->i), _gameData.hudBarMob.lifeBar, _gameData.hudBarMob.pixelLife, _gameData.showMobHpBar, _gameData.renderTextureRed, _gameData.renderTextureBlue);
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->Display(_gameData.renderTextureRed, _gameData.renderTextureBlue);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Building)
		{
			sf::Vector2f pos = _gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite.getPosition();
			if (_gameData.gameState != PAUSE && _gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->isAnim)
			{
				bool temp = true;
				_gameData.camera->GetParalax().animSort[_gameData.sortVector.at(i)->i]->UpdateLeftToRight(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, _gameData.dt, 5, 0, temp, true);
			}
			Utils::BlitSprite(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, { pos.x + (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f),pos.y }, _gameData.renderTextureRed);
			Utils::BlitSprite(_gameData.tabBuilding.at(_gameData.sortVector.at(i)->i)->sprite, { pos.x - (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f),pos.y }, _gameData.renderTextureBlue);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Food)
		{
			Utils::BlitSprite(_gameData.drop.at(_gameData.sortVector.at(i)->i)->sprite, { _gameData.drop.at(_gameData.sortVector.at(i)->i)->pos.x + (TROAD_GAP * (-1 * _gameData.drop.at(_gameData.sortVector.at(i)->i)->pos.y + WORKING_HEIGHT) / 100.f), _gameData.drop.at(_gameData.sortVector.at(i)->i)->pos.y }, _gameData.renderTextureRed);
			Utils::BlitSprite(_gameData.drop.at(_gameData.sortVector.at(i)->i)->sprite, { _gameData.drop.at(_gameData.sortVector.at(i)->i)->pos.x - (TROAD_GAP * (-1 * _gameData.drop.at(_gameData.sortVector.at(i)->i)->pos.y + WORKING_HEIGHT) / 100.f), _gameData.drop.at(_gameData.sortVector.at(i)->i)->pos.y }, _gameData.renderTextureBlue);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Scarecrow)
		{
			_gameData.scarecrow.at(_gameData.sortVector.at(i)->i)->Display(_gameData.renderTextureRed, _gameData.renderTextureBlue, _gameData.dt);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Crystal)
		{
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->DisplayCrystal(_gameData.renderTextureRed, _gameData.renderTextureBlue, &_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->GetCrystal()[_gameData.sortVector.at(i)->j]);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Plante)
		{
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->DisplayPlante(_gameData.renderTextureRed, _gameData.renderTextureBlue, _gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->GetVectorPlant()[_gameData.sortVector.at(i)->j]);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::Bramble)
		{
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetWitch()->DisplayBramble(_gameData.renderTextureRed, _gameData.renderTextureBlue);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::SlashH || _gameData.sortVector.at(i)->sort == SortVector::SlashV)
		{
			_gameData.tabMob.at(_gameData.sortVector.at(i)->i)->GetBoar()->DisplaySlash(_gameData.renderTextureRed, _gameData.renderTextureBlue);
		}
		else if (_gameData.sortVector.at(i)->sort == SortVector::SleepingNut)
		{
			sf::Vector2f pos = _gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->sprite.getPosition();
			if (_gameData.gameState != PAUSE && _gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->isAnim)
			{
				bool temp = true;
				_gameData.camera->GetParalax().animSort[1]->UpdateLeftToRight(_gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->sprite, _gameData.dt, 5, 0, temp, true);
			}

			Utils::BlitSprite(_gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->sprite, sf::Vector2f { pos.x - (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f), pos.y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(_gameData.sleepingNut.at(_gameData.sortVector.at(i)->i)->sprite, sf::Vector2f { pos.x + (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f), pos.y }, 0, _gameData.renderTextureBlue);
		}
		else
		{
			_gameData.projectile.at(_gameData.sortVector.at(i)->i).Display(_gameData.renderTextureRed, _gameData.renderTextureBlue);
		}
	}

}

void Arena::ArenaDisplay3D(GameData& _gameData)
{
	if (_gameData.reloadArena)
	{
		Reload(_gameData);
		_gameData.reloadArena = false;
	}

	_gameData.renderTextureRed.setView(_gameData.camera->GetView());
	_gameData.renderTextureBlue.setView(_gameData.camera->GetView());
	_gameData.camera->Display3D(_gameData);

	_gameData.rtRed->clear(sf::Color::Transparent);
	_gameData.rtBlue->clear(sf::Color::Transparent);

	//shadow
	ShadowDisplay3D(_gameData);

	//projectile
	ProjectileShadowDisplay3D(_gameData);

	_gameData.rtRed->display();
	_gameData.rtBlue->display();

	_gameData.renderTextureRed.draw(_gameData.rtRedShadow);
	_gameData.renderTextureBlue.draw(_gameData.rtBlueShadow);

	//Viper Spell
	DisplayPoisonSpell(_gameData.tabPlayers, _gameData.renderTextureRed, _gameData.renderTextureBlue);

	//display all Entity with sorting
	EntitiesDisplay3D(_gameData);

	_gameData.camera->DisplayFirstPlan(_gameData.renderTextureRed, _gameData.renderTextureBlue, _gameData.levelState,_gameData.tabPlayers[0]->GetIsDrawnBehindTree());

	_gameData.window.setView(_gameData.window.getDefaultView());
	_gameData.renderTextureRed.setView(_gameData.renderTextureRed.getDefaultView());
	_gameData.renderTextureBlue.setView(_gameData.renderTextureBlue.getDefaultView());

	if (!showTuto)
	{
		_gameData.hud->Display(_gameData.renderTextureRed, _gameData.renderTextureBlue, _gameData.tabPlayers, _gameData.arenaIsChose);
	}
}

void Arena::ArenaDisplay(GameData& _gameData)
{
	if (_gameData.reloadArena)
	{
		Reload(_gameData);
		_gameData.reloadArena = false;
	}

	_gameData.window.setView(_gameData.camera->GetView());
	_gameData.camera->Display(_gameData);

	//shadow
	ShadowDisplay(_gameData);

	//projectile
	ProjectileShadowDisplay(_gameData);

	_gameData.window.draw(_gameData.rtShadow);

	//Viper Spell
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.tabPlayers[i]->GetPlayerSelected() == 1)
		{
			_gameData.tabPlayers[i]->DisplayViperSpell(_gameData.window);
		}
	}

	//display all Entity with sorting
	EntitiesDisplay(_gameData);

	_gameData.camera->DisplayFirstPlan(_gameData);
	_gameData.window.setView(_gameData.window.getDefaultView());
	if (!showTuto)
	{
		_gameData.hud->Display(_gameData);
	}
	
}

void Arena::Display(GameData& _gameData)
{
	ArenaDisplay(_gameData);

	_gameData.entityManPVP->DisplayDamageVector(_gameData.window);

	if (_gameData.teamfight && !diplayWinSprite)
	{
		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{
			_gameData.window.draw(teamFlag[i]);
		}
	}
	else if (!_gameData.teamfight && !showTuto)
	{

		for (int i = 0; i < textWinCompteur.size(); i++)
		{
			_gameData.window.draw(*textWinCompteur[i]);
			Utils::BlitSprite(crownHud, { textWinCompteur[i]->getPosition().x - 70,textWinCompteur[i]->getPosition().y }, 0, _gameData.window);
			if (displayBestPlayer[i] && _gameData.tabPlayers[i]->GetState() != DEAD)
			{
				_gameData.window.draw(CrownOnPlayer[i]);
			}
		}
	}

	if (!diplayWinSprite)
	{
		_gameData.hud->DisplayMigrant(_gameData);
	}

	if (displayFight && !showTuto)
	{
		if (lineAnimeFight == 0)
		{
			Utils::BlitSprite(fightStart, { (float)(WORKING_WIDTH / 2.f) + 36,(float)(WORKING_HEIGHT / 2.f) }, 0, _gameData.window);
		}
		else if (lineAnimeFight == 1)
		{
			Utils::BlitSprite(fightStart, { (float)(WORKING_WIDTH / 2.f) + 40,(float)(WORKING_HEIGHT / 2.f) - 1 }, 0, _gameData.window);
		}
	}

	if (diplayWinSprite)
	{
		if (!displayTeamScore)
		{
			Utils::BlitSprite(winSprite, { WORKING_WIDTH / 2, WORKING_HEIGHT / 2 }, 0, _gameData.window);
		}

		if (_gameData.teamfight && displayTeamScore)
		{
			Utils::BlitSprite(winSpriteLeft, { posWinCompteur.x - winSpriteLeft.getTexture()->getSize().x / 6,posWinCompteur.y - (winSpriteLeft.getTexture()->getSize().y / 6) / 2 }, 0, _gameData.window);
			Utils::BlitSprite(winSpriteRight, { posWinCompteur.x ,posWinCompteur.y - (winSpriteRight.getTexture()->getSize().y / 6) / 2 + 5 }, 0, _gameData.window);
			Utils::BlitSprite(winSpriteMiddle, posWinCompteur, 0, _gameData.window);
		}

		if (leftArena)
		{
			conffeti.setScale({ 1,1 });
			Utils::BlitSprite(conffeti, { 0,(float)(WORKING_HEIGHT - conffeti.getTexture()->getSize().y / 3) }, 0, _gameData.window);
			conffeti.setScale({ -1,1 });
			Utils::BlitSprite(conffeti, { (float)WORKING_WIDTH,(float)(WORKING_HEIGHT - conffeti.getTexture()->getSize().y / 3) }, 0, _gameData.window);
		}
	}

	if (showTuto)
	{
		Utils::BlitSprite(tutoSprite, { (float)WORKING_WIDTH / 2,(float)(WORKING_HEIGHT / 2) }, 0, _gameData.window);
	}

	if (leftArena)
	{
		fade->Display(_gameData.window);
	}
}

void Arena::Display3D(GameData& _gameData)
{
	ArenaDisplay3D(_gameData);

	_gameData.entityManPVP->DisplayDamageVector(_gameData.renderTextureRed, _gameData.renderTextureBlue);

	if (_gameData.teamfight && !diplayWinSprite)
	{
		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{
			Utils::BlitSprite(teamFlag[i], sf::Vector2f{ teamFlag[i].getPosition().x + (TROAD_GAP * (-1 * teamFlag[i].getPosition().y + WORKING_HEIGHT) / 100.f),teamFlag[i].getPosition().y }, _gameData.renderTextureRed);
			Utils::BlitSprite(teamFlag[i], sf::Vector2f{ teamFlag[i].getPosition().x - (TROAD_GAP * (-1 * teamFlag[i].getPosition().y + WORKING_HEIGHT) / 100.f),teamFlag[i].getPosition().y }, _gameData.renderTextureBlue);
		}
	}
	else if (!_gameData.teamfight && !showTuto)
	{
		for (int i = 0; i < textWinCompteur.size(); i++)
		{
			sf::Vector2f posTextWin = textWinCompteur[i]->getPosition();

			textWinCompteur[i]->setPosition(sf::Vector2f{ posTextWin.x + (TROAD_GAP * (-1 * posTextWin.y + WORKING_HEIGHT) / 100.f),posTextWin.y });
			_gameData.renderTextureRed.draw(*textWinCompteur[i]);

			textWinCompteur[i]->setPosition(sf::Vector2f{ posTextWin.x - (TROAD_GAP * (-1 * posTextWin.y + WORKING_HEIGHT) / 100.f),posTextWin.y });
			_gameData.renderTextureBlue.draw(*textWinCompteur[i]);

			textWinCompteur[i]->setPosition(posTextWin);

			Utils::BlitSprite(crownHud, sf::Vector2f{ (textWinCompteur[i]->getPosition().x - 70) - (TROAD_GAP * (-1 * textWinCompteur[i]->getPosition().y + WORKING_HEIGHT) / 100.f),textWinCompteur[i]->getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(crownHud, sf::Vector2f{ (textWinCompteur[i]->getPosition().x - 70) + (TROAD_GAP * (-1 * textWinCompteur[i]->getPosition().y + WORKING_HEIGHT) / 100.f),textWinCompteur[i]->getPosition().y }, 0, _gameData.renderTextureBlue);

			if (displayBestPlayer[i] && _gameData.tabPlayers[i]->GetState() != DEAD)
			{
				Utils::BlitSprite(CrownOnPlayer[i], sf::Vector2f{ CrownOnPlayer[i].getPosition().x - (TROAD_GAP * (-1 * CrownOnPlayer[i].getPosition().y + WORKING_HEIGHT) / 100.f),CrownOnPlayer[i].getPosition().y }, 0, _gameData.renderTextureRed);
				Utils::BlitSprite(CrownOnPlayer[i], sf::Vector2f{ CrownOnPlayer[i].getPosition().x + (TROAD_GAP * (-1 * CrownOnPlayer[i].getPosition().y + WORKING_HEIGHT) / 100.f),CrownOnPlayer[i].getPosition().y }, 0, _gameData.renderTextureBlue);
			}
		}
	}

	if (!diplayWinSprite)
	{
		_gameData.hud->DisplayMigrant3D(_gameData);
	}

	if (displayFight && !showTuto)
	{
		if (lineAnimeFight == 0)
		{
			Utils::BlitSprite(fightStart, sf::Vector2f{ (WORKING_WIDTH / 2) + (TROAD_GAP * (-1 * (WORKING_HEIGHT / 2) + WORKING_HEIGHT) / 100.f),(WORKING_HEIGHT / 2) }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(fightStart, sf::Vector2f{ (WORKING_WIDTH / 2) - (TROAD_GAP * (-1 * (WORKING_HEIGHT / 2) + WORKING_HEIGHT) / 100.f),(WORKING_HEIGHT / 2) }, 0, _gameData.renderTextureBlue);
		}
		else if (lineAnimeFight == 1)
		{
			Utils::BlitSprite(fightStart, sf::Vector2f{ ((WORKING_WIDTH / 2) + 4) + (TROAD_GAP * (-1 * ((WORKING_HEIGHT / 2) - 1) + WORKING_HEIGHT) / 100.f),((WORKING_HEIGHT / 2) + 1) }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(fightStart, sf::Vector2f{ ((WORKING_WIDTH / 2) + 4) - (TROAD_GAP * (-1 * ((WORKING_HEIGHT / 2) - 1) + WORKING_HEIGHT) / 100.f),((WORKING_HEIGHT / 2) + 1) }, 0, _gameData.renderTextureBlue);
		}
	}

	if (diplayWinSprite)
	{
		if (!displayTeamScore)
		{
			Utils::BlitSprite(winSprite, sf::Vector2f{ WORKING_WIDTH / 2 + (TROAD_GAP * (-1 * WORKING_HEIGHT / 2 + WORKING_HEIGHT) / 100.f),WORKING_HEIGHT / 2 }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(winSprite, sf::Vector2f{ WORKING_WIDTH / 2 - (TROAD_GAP * (-1 * WORKING_HEIGHT / 2 + WORKING_HEIGHT) / 100.f),WORKING_HEIGHT / 2 }, 0, _gameData.renderTextureBlue);
		}

		if (_gameData.teamfight && displayTeamScore)
		{
			Utils::BlitSprite(winSpriteLeft, sf::Vector2f{ (posWinCompteur.x - winSpriteLeft.getTexture()->getSize().x / 6) + (TROAD_GAP * (-1 * (posWinCompteur.y - (winSpriteLeft.getTexture()->getSize().y / 6) / 2) + WORKING_HEIGHT) / 100.f),(posWinCompteur.y - (winSpriteLeft.getTexture()->getSize().y / 6) / 2) }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(winSpriteLeft, sf::Vector2f{ (posWinCompteur.x - winSpriteLeft.getTexture()->getSize().x / 6) - (TROAD_GAP * (-1 * (posWinCompteur.y - (winSpriteLeft.getTexture()->getSize().y / 6) / 2) + WORKING_HEIGHT) / 100.f),(posWinCompteur.y - (winSpriteLeft.getTexture()->getSize().y / 6) / 2) }, 0, _gameData.renderTextureBlue);

			Utils::BlitSprite(winSpriteRight, sf::Vector2f{ posWinCompteur.x + (TROAD_GAP * (-1 * (posWinCompteur.y - (winSpriteRight.getTexture()->getSize().y / 6) / 2 + 5) + WORKING_HEIGHT) / 100.f),(posWinCompteur.y - (winSpriteRight.getTexture()->getSize().y / 6) / 2 + 5) }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(winSpriteRight, sf::Vector2f{ posWinCompteur.x - (TROAD_GAP * (-1 * (posWinCompteur.y - (winSpriteRight.getTexture()->getSize().y / 6) / 2 + 5) + WORKING_HEIGHT) / 100.f),(posWinCompteur.y - (winSpriteRight.getTexture()->getSize().y / 6) / 2 + 5) }, 0, _gameData.renderTextureBlue);

			Utils::BlitSprite(winSpriteMiddle, sf::Vector2f{ posWinCompteur.x + (TROAD_GAP * (-1 * posWinCompteur.y + WORKING_HEIGHT) / 100.f),posWinCompteur.y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(winSpriteMiddle, sf::Vector2f{ posWinCompteur.x - (TROAD_GAP * (-1 * posWinCompteur.y + WORKING_HEIGHT) / 100.f),posWinCompteur.y }, 0, _gameData.renderTextureBlue);
		}

		if (leftArena)
		{
			conffeti.setScale({ 1,1 });
			Utils::BlitSprite(conffeti, sf::Vector2f{ 0.f + (TROAD_GAP * (-1 * (WORKING_HEIGHT - conffeti.getTexture()->getSize().y / 3) + WORKING_HEIGHT) / 100.f),(float)(WORKING_HEIGHT - conffeti.getTexture()->getSize().y / 3) }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(conffeti, sf::Vector2f{ 0.f - (TROAD_GAP * (-1 * (WORKING_HEIGHT - conffeti.getTexture()->getSize().y / 3) + WORKING_HEIGHT) / 100.f),(float)(WORKING_HEIGHT - conffeti.getTexture()->getSize().y / 3) }, 0, _gameData.renderTextureBlue);

			conffeti.setScale({ -1,1 });
			Utils::BlitSprite(conffeti, sf::Vector2f{ WORKING_WIDTH + (TROAD_GAP * (-1 * (WORKING_HEIGHT - conffeti.getTexture()->getSize().y / 3) + WORKING_HEIGHT) / 100.f),(float)(WORKING_HEIGHT - conffeti.getTexture()->getSize().y / 3) }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(conffeti, sf::Vector2f{ WORKING_WIDTH - (TROAD_GAP * (-1 * (WORKING_HEIGHT - conffeti.getTexture()->getSize().y / 3) + WORKING_HEIGHT) / 100.f),(float)(WORKING_HEIGHT - conffeti.getTexture()->getSize().y / 3) }, 0, _gameData.renderTextureBlue);
		}
	}

	if (leftArena)
	{
		fade->Display(_gameData.renderTextureRed, _gameData.renderTextureBlue);
	}
}

void Arena::JoystickPressed(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::JoystickButtonPressed || _gameData.event.type == sf::Event::JoystickMoved)
	{
		if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::START))
			{
				_gameData.ChangeState(PAUSE);
				_gameData.swordSpriteMenu.setPosition(_gameData.startPosPauseSwordSprite);
			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::START))
			{
				_gameData.ChangeState(PAUSE);
				_gameData.swordSpriteMenu.setPosition(_gameData.startPosPauseSwordSprite);
			}
		}
	}
}

const bool& Arena::GetFadeOut(void) const
{
	return fade->GetIsFadingOut();
}

void Arena::SetAlpha(const float _alpha)
{
	fade->SetAlpha(_alpha);
	fade->SetColor(sf::Color(0, 0, 0, (sf::Uint8)_alpha));
}

void Arena::SetFadeOut(const bool _fadeOut)
{
	fade->SetIsFadingOut(_fadeOut);
}