#include "stdafx.h"
#include "Upgrade.h"
#include "Camera.h"
#include "HUD.h"
#include "Player.hpp"
#include "PlayersSprites.h"

Upgrade::Upgrade(GameData& _gameData)
{
	LoadSprite();

	//player Idle
	this->animPlayerIdle = new Animation;
	for (int i = 0; i < NbMaxPlayer; i++)
	{
		this->idlePlayer[i] = _gameData.playersSprite->playerSprite[i][(int)(AllSprite::IDLE)];
		this->idlePlayer[i].setPosition(sf::Vector2f{ 350.f,495.f });
		this->idlePlayer[i].setScale(sf::Vector2f{ 2.25f,2.25f });
	}

	//setPos des sprites upgrade
	for (int i = 0; i < STATS_NBR; i++)
	{
		stats[i].setPosition(plank[i][0]->getPosition().x + 25, plank[i][0]->getPosition().y + 10);
	}

	InitPoint();

	buttonSelectedUpgrade = (int)ButtonSelected::LIFE;

	InitTexte(*_gameData.GameFont);

	this->timerButton = 0.5f;

	for (int i = 0; i < STATS_NBR; i++)
	{
		buttonVectUpgrade.push_back(noButton[currentColorPlayer][i][0]);
		//hasBeenPushBack = true;
	}
	buttonVectUpgrade.push_back(confirm[1]);
}

Upgrade::~Upgrade()
{
}

void Upgrade::InitUpgrade(GameData& _gameData)
{
	currentColorPlayer = _gameData.tabPlayers[currentPlayer]->GetPlayerSelected();
}

void Upgrade::Reload(GameData& _gameData)
{

}

void Upgrade::JoystickMovingUpgrade(GameData& _gameData)
{
	UpdateButtonsSelectionUpgrade(_gameData);
	DownUpJoysticksButtonsUpgrade(_gameData);
}

void Upgrade::UpdateButtonsSelectionUpgrade(GameData& _gameData)
{
	if (buttonVectUpgrade.size() > 1)
	{
		for (unsigned char i = 0; i < buttonVectUpgrade.size(); i++)
		{
			if (buttonSelectedUpgrade == i)
			{
				buttonVectUpgrade[i]->SetIsSelected(true, _gameData);
			}
			else
			{
				buttonVectUpgrade[i]->SetIsSelected(false, _gameData);
			}
		}
	}
}

void Upgrade::SetButtonSelectedPressed(GameData& _gameData)
{
	buttonVectUpgrade[buttonSelectedUpgrade]->SetIsPressed(true, _gameData);
}

void Upgrade::DownUpJoysticksButtonsUpgrade(GameData& _gameData)
{
	if (!buttonVectUpgrade.empty())
	{
		float yPos1 = sf::Joystick::getAxisPosition(currentPlayer, sf::Joystick::Axis::LEFT_JOYSTICK_Y);
		float yPos2 = sf::Joystick::getAxisPosition(currentPlayer, sf::Joystick::Axis::DIR_CROSS_Y);

		float xPos1 = sf::Joystick::getAxisPosition(currentPlayer, sf::Joystick::Axis::LEFT_JOYSTICK_X);
		float xPos2 = sf::Joystick::getAxisPosition(currentPlayer, sf::Joystick::Axis::DIR_CROSS_X);

		joystickTimer += _gameData.dt;

		if (joystickTimer >= 0.2f)
		{
			joystickTimer = 0;
			if (yPos1 > 15 || yPos2 < -15)
			{
				if (buttonSelectedUpgrade != (buttonVectUpgrade.size() - 1))
				{
					++buttonSelectedUpgrade;
				}
				else
				{
					buttonSelectedUpgrade = 0;
				}

				buttonVectUpgrade[buttonSelectedUpgrade]->SetIsSelected(true, _gameData);

			}
			if (yPos1 < -15 || yPos2 > 15)
			{
				if (buttonSelectedUpgrade != 0)
				{
					--buttonSelectedUpgrade;
				}
				else
				{
					buttonSelectedUpgrade = (int)(buttonVectUpgrade.size() - 1);
				}

				buttonVectUpgrade[buttonSelectedUpgrade]->SetIsSelected(true, _gameData);

			}
			//add
			if ((xPos1 > 15 || xPos2 > 15) && _gameData.tabPlayers[currentPlayer]->GetSkillPoints())
			{
				for (int i = 0; i < buttonVectUpgrade.size(); i++)
				{
					if (buttonVectUpgrade[i]->GetIsSelected())
					{
						switch (i)
						{
						case 0:
							if (_gameData.tabPlayers[currentPlayer]->GetVigor() != MaxStat)
							{
								this->filledBLock[this->currentColorPlayer][0][_gameData.tabPlayers[currentPlayer]->GetVigor()].sprite = this->filledBlocks[this->currentColorPlayer];
								this->filledBLock[this->currentColorPlayer][0][_gameData.tabPlayers[currentPlayer]->GetVigor()].colored = true;
								if (this->filledBLock[this->currentColorPlayer][0][_gameData.tabPlayers[currentPlayer]->GetVigor()].crown)
								{
									this->filledBLock[this->currentColorPlayer][0][_gameData.tabPlayers[currentPlayer]->GetVigor()].sprite = this->kingButtons[this->currentColorPlayer];
								}
								_gameData.tabPlayers[currentPlayer]->AddVigor();
							}
							break;
						case 1:
							if (_gameData.tabPlayers[currentPlayer]->GetStrength() != MaxStat)
							{
								this->filledBLock[this->currentColorPlayer][1][_gameData.tabPlayers[currentPlayer]->GetStrength()].sprite = this->filledBlocks[this->currentColorPlayer];
								this->filledBLock[this->currentColorPlayer][1][_gameData.tabPlayers[currentPlayer]->GetStrength()].colored = true;
								if (this->filledBLock[this->currentColorPlayer][1][_gameData.tabPlayers[currentPlayer]->GetStrength()].crown)
								{
									this->filledBLock[this->currentColorPlayer][1][_gameData.tabPlayers[currentPlayer]->GetStrength()].sprite = this->kingButtons[this->currentColorPlayer];
								}
								_gameData.tabPlayers[currentPlayer]->AddStrength();
							}
							break;
						case 2:
							if (_gameData.tabPlayers[currentPlayer]->GetAgility() != MaxStat)
							{
								this->filledBLock[this->currentColorPlayer][2][_gameData.tabPlayers[currentPlayer]->GetAgility()].sprite = this->filledBlocks[this->currentColorPlayer];
								this->filledBLock[this->currentColorPlayer][2][_gameData.tabPlayers[currentPlayer]->GetAgility()].colored = true;
								if (this->filledBLock[this->currentColorPlayer][2][_gameData.tabPlayers[currentPlayer]->GetAgility()].crown)
								{
									this->filledBLock[this->currentColorPlayer][2][_gameData.tabPlayers[currentPlayer]->GetAgility()].sprite = this->kingButtons[this->currentColorPlayer];
								}
								_gameData.tabPlayers[currentPlayer]->AddAgility();
							}
							break;
						case 3:
							if (_gameData.tabPlayers[currentPlayer]->GetWisdom() != MaxStat)
							{
								this->filledBLock[this->currentColorPlayer][3][_gameData.tabPlayers[currentPlayer]->GetWisdom()].sprite = this->filledBlocks[this->currentColorPlayer];
								this->filledBLock[this->currentColorPlayer][3][_gameData.tabPlayers[currentPlayer]->GetWisdom()].colored = true;
								if (this->filledBLock[this->currentColorPlayer][3][_gameData.tabPlayers[currentPlayer]->GetWisdom()].crown)
								{
									this->filledBLock[this->currentColorPlayer][3][_gameData.tabPlayers[currentPlayer]->GetWisdom()].sprite = this->kingButtons[this->currentColorPlayer];
								}
								_gameData.tabPlayers[currentPlayer]->AddWisdom();
							}
							break;
						default:
							break;
						}
					}
				}
			}
			//remove
			else if (xPos1 < -15 || xPos2 < -15)
			{
				for (int i = 0; i < buttonVectUpgrade.size(); i++)
				{
					if (buttonVectUpgrade[i]->GetIsSelected())
					{
						switch (i)
						{
						case 0:
							if (_gameData.tabPlayers[currentPlayer]->GetVigor() != BaseStat && !this->filledBLock[this->currentColorPlayer][0][_gameData.tabPlayers[currentPlayer]->GetVigor() - 1].lock)
							{
								this->filledBLock[this->currentColorPlayer][0][_gameData.tabPlayers[currentPlayer]->GetVigor() - 1].sprite = this->emptyBlock;
								this->filledBLock[this->currentColorPlayer][0][_gameData.tabPlayers[currentPlayer]->GetVigor() - 1].colored = false;
								if (this->filledBLock[this->currentColorPlayer][0][_gameData.tabPlayers[currentPlayer]->GetVigor() - 1].crown)
								{
									this->filledBLock[this->currentColorPlayer][0][_gameData.tabPlayers[currentPlayer]->GetVigor() - 1].sprite = this->emptyKingButtons;
								}
								_gameData.tabPlayers[currentPlayer]->RemoveVigor();
							}
							break;
						case 1:
							if (_gameData.tabPlayers[currentPlayer]->GetStrength() != BaseStat && !this->filledBLock[this->currentColorPlayer][1][_gameData.tabPlayers[currentPlayer]->GetStrength() - 1].lock)
							{
								this->filledBLock[this->currentColorPlayer][1][_gameData.tabPlayers[currentPlayer]->GetStrength() - 1].sprite = this->emptyBlock;
								this->filledBLock[this->currentColorPlayer][1][_gameData.tabPlayers[currentPlayer]->GetStrength() - 1].colored = false;
								if (this->filledBLock[this->currentColorPlayer][1][_gameData.tabPlayers[currentPlayer]->GetStrength() - 1].crown)
								{
									this->filledBLock[this->currentColorPlayer][1][_gameData.tabPlayers[currentPlayer]->GetStrength() - 1].sprite = this->emptyKingButtons;
								}
								_gameData.tabPlayers[currentPlayer]->RemoveStrength();
							}
							break;
						case 2:
							if (_gameData.tabPlayers[currentPlayer]->GetAgility() != BaseStat && !this->filledBLock[this->currentColorPlayer][2][_gameData.tabPlayers[currentPlayer]->GetAgility() - 1].lock)
							{
								this->filledBLock[this->currentColorPlayer][2][_gameData.tabPlayers[currentPlayer]->GetAgility() - 1].sprite = this->emptyBlock;
								this->filledBLock[this->currentColorPlayer][2][_gameData.tabPlayers[currentPlayer]->GetAgility() - 1].colored = false;
								if (this->filledBLock[this->currentColorPlayer][2][_gameData.tabPlayers[currentPlayer]->GetAgility() - 1].crown)
								{
									this->filledBLock[this->currentColorPlayer][2][_gameData.tabPlayers[currentPlayer]->GetAgility() - 1].sprite = this->emptyKingButtons;
								}
								_gameData.tabPlayers[currentPlayer]->RemoveAgility();
							}
							break;
						case 3:
							if (_gameData.tabPlayers[currentPlayer]->GetWisdom() != BaseStat && !this->filledBLock[this->currentColorPlayer][3][_gameData.tabPlayers[currentPlayer]->GetWisdom() - 1].lock)
							{
								this->filledBLock[this->currentColorPlayer][3][_gameData.tabPlayers[currentPlayer]->GetWisdom() - 1].sprite = this->emptyBlock;
								this->filledBLock[this->currentColorPlayer][3][_gameData.tabPlayers[currentPlayer]->GetWisdom() - 1].colored = false;
								if (this->filledBLock[this->currentColorPlayer][3][_gameData.tabPlayers[currentPlayer]->GetWisdom() - 1].crown)
								{
									this->filledBLock[this->currentColorPlayer][3][_gameData.tabPlayers[currentPlayer]->GetWisdom() - 1].sprite = this->emptyKingButtons;
								}
								_gameData.tabPlayers[currentPlayer]->RemoveWisdom();
							}
							break;
						default:
							break;
						}
					}
				}
			}
		}
	}
}

void StatsUpgradeManager(int _idStat, int _idPlayer, GameData& _gameData)
{
	switch (_idStat)
	{
	case (int)Stats::STRENGTH:
		_gameData.tabPlayers[_idPlayer]->AddStrength();
		break;

	case (int)Stats::WISDOM:
		_gameData.tabPlayers[_idPlayer]->AddWisdom();
		break;

	case (int)Stats::AGILITY:
		_gameData.tabPlayers[_idPlayer]->AddAgility();
		break;

	case (int)Stats::VIGOR:
		_gameData.tabPlayers[_idPlayer]->AddVigor();
		break;

	default:
		std::cout << "Pas de Stats choisie ceci est un bug" << std::endl;
		break;
	}
}

void Upgrade::Update(GameData& _gameData)
{
	if (!this->init)
	{
		Init(_gameData);
		this->init = true;

		while (!_gameData.tabPlayers[this->currentPlayer]->GetSkillPoints())
		{
			if (this->currentPlayer == _gameData.tabPlayers.size()-1)
			{
				_gameData.ChangeState(GAME);
				init = false;
				currentPlayer = 0;
				ChangeLock();
				_gameData.tabPlayers[currentPlayer]->SetStatsValue();
				return;
			}
			this->currentPlayer++;
		}
	}

	UpdateMaxStatText(_gameData.tabPlayers[this->currentPlayer]);
	UpdateAnimIdlePlayer(_gameData.dt);
	skillPoints.setString(std::to_string(_gameData.tabPlayers[currentPlayer]->GetSkillPoints()));
	UpdateAllButtonsAnim(_gameData.dt);
	JoystickMovingUpgrade(_gameData);

	if (timerButton < 0.5)
	{
		timerButton -= _gameData.dt;
		if (timerButton <= 0)
		{
			timerButton = 0.5f;
		}
	}

	if (this->buttonSelectedUpgrade == 2 && (this->filledBLock[this->currentColorPlayer][2][4].colored || this->filledBLock[this->currentColorPlayer][2][9].colored))
	{
		this->textUnlockSkill.setString(this->unlcokDash);
		this->textUnlockSkill.setOrigin(this->textUnlockSkill.getLocalBounds().width / 2, this->textUnlockSkill.getLocalBounds().height / 2);
	}
	if (this->buttonSelectedUpgrade == 3 && this->filledBLock[this->currentColorPlayer][3][4].colored)
	{
		this->textUnlockSkill.setString(this->unlockAOEMagic);
		this->textUnlockSkill.setOrigin(this->textUnlockSkill.getLocalBounds().width / 2, this->textUnlockSkill.getLocalBounds().height / 2);
	}

	if (this->buttonSelectedUpgrade <= 3)
	{
		this->textStat.setString(statsList[this->buttonSelectedUpgrade]);
		this->textStat.setOrigin(this->textStat.getLocalBounds().width / 2, this->textStat.getLocalBounds().height / 2);
	}

	_gameData.tabPlayers[currentPlayer]->SetStatsValue();
	if (sf::Joystick::getIdentification(currentPlayer).name == "Wireless Controller")
	{
		if (sf::Joystick::isButtonPressed(currentPlayer, (unsigned int)ControllerButtonPS4::CROSS))
		{
			for (int i = 0; i < buttonVectUpgrade.size(); i++)
			{
				if (buttonVectUpgrade[i]->GetIsSelected())
				{
					if (i == 4)
					{
						if (currentPlayer != _gameData.tabPlayers.size() - 1)
						{
							currentPlayer++;
							InitUpgrade(_gameData);
							ChangeLock();
							MaxStatPlayer(_gameData.tabPlayers[this->currentPlayer]);
							InitText(*_gameData.GameFont);
							_gameData.tabPlayers[currentPlayer]->SetStatsValue();
						}
						else
						{
							_gameData.idPlayerInUpgrade = i;
							_gameData.ChangeState(GAME);
							init = false;
							currentPlayer = 0;
							ChangeLock();
							_gameData.tabPlayers[currentPlayer]->SetStatsValue();
						}
					}
				}
			}
		}
	}
	else
	{
		if (sf::Joystick::isButtonPressed(currentPlayer, (unsigned int)ControllerButtonXBOX::A))
		{
			for (int i = 0; i < buttonVectUpgrade.size(); i++)
			{
				if (buttonVectUpgrade[i]->GetIsSelected())
				{
					if (i == 4)
					{
						if (currentPlayer != _gameData.tabPlayers.size() - 1)
						{
							currentPlayer++;
							InitUpgrade(_gameData);
							ChangeLock();
							MaxStatPlayer(_gameData.tabPlayers[this->currentPlayer]);
							InitText(*_gameData.GameFont);
							_gameData.tabPlayers[currentPlayer]->SetStatsValue();
						}
						else
						{
							_gameData.idPlayerInUpgrade = i;
							_gameData.ChangeState(GAME);
							init = false;
							currentPlayer = 0;
							ChangeLock();
							_gameData.tabPlayers[currentPlayer]->SetStatsValue();
						}
					}
				}
			}
		}
	}
}

void Upgrade::Display(GameData& _gameData)
{
	_gameData.window.draw(this->backGround);

	_gameData.window.draw(this->confirm[0]->sprite);

	for (int i = 0; i < buttonVectUpgrade.size(); i++)
	{
		_gameData.window.draw(this->plank[i][1]->sprite);
		if (buttonVectUpgrade[i]->GetIsSelected())
		{
			if (i <= 3)
			{
				_gameData.window.draw(this->plank[buttonSelectedUpgrade][0]->sprite);
			}
			else
			{
				_gameData.window.draw(this->confirm[2]->sprite);
			}
		}
	}

	for (int i = 0; i < STATS_NBR; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if (buttonSelectedUpgrade == i)
			{
				_gameData.window.draw(this->selectButton[currentColorPlayer][buttonSelectedUpgrade][j]->sprite);
			}
			else
			{
				//_gameData.window.draw(this->noButton[currentColorPlayer][i][j]->sprite);
				//_gameData.window.draw(this->pressButton[currentColorPlayer][i][j]->sprite);
				_gameData.window.draw(this->justButton[currentColorPlayer][i][j]->sprite);
			}
		}

		for (int i = 0; i < STATS_NBR; i++)
		{
			if (buttonVectUpgrade[i]->GetIsSelected())
			{
				//emptyBlock.setScale(1, 1);
				plank[i][1]->sprite.setScale(1, 1);
				confirm[0]->sprite.setScale(0.9, 0.9);
				confirm[1]->sprite.setScale(0.9, 0.9);
				confirm[2]->sprite.setScale(0.9, 0.9);
			}
			else
			{
				//emptyBlock.setScale(0.8, 0.8);
				plank[i][1]->sprite.setScale(0.98, 0.98);
				confirm[2]->sprite.setScale(1, 1);
			}
		}
	}

	//rescale the current one
	for (int j = 0; j < NbMaxPlanks; j++)
	{
		if (j != this->buttonSelectedUpgrade)
		{
			for (int k = 0; k < NbMaxPoint; k++)
			{
				this->filledBLock[this->currentColorPlayer][j][k].sprite.setScale(0.8f, 0.8f);
			}
		}
	}

	//display all point
	for (int j = 0; j < NbMaxPlanks; j++)
	{
		for (int k = 0; k < NbMaxPoint; k++)
		{
			Utils::BlitSprite(this->filledBLock[this->currentColorPlayer][j][k].sprite, this->filledBLock[this->currentPlayer][j][k].pos, 0, _gameData.window);
			this->filledBLock[this->currentColorPlayer][j][k].sprite.setScale(1.f, 1.f);
		}
	}

	_gameData.window.draw(this->colorBackGround[currentColorPlayer]->sprite);
	_gameData.window.draw(this->characterFrame[currentColorPlayer]->sprite);

	_gameData.window.draw(this->pointsPlank->sprite);
	_gameData.window.draw(this->textPlank->sprite);
	_gameData.window.draw(this->mult->sprite);
	_gameData.window.draw(this->filledBlocksPlank[currentColorPlayer]);
	_gameData.window.draw(this->skillPoints);

	//anim player
	_gameData.window.draw(this->idlePlayer[this->currentColorPlayer]);

	//upgrade stat
	if (this->buttonSelectedUpgrade <= 3)
	{
		Utils::BlitSprite(stats[this->buttonSelectedUpgrade],sf::Vector2f{90.f,600.f}, -13, _gameData.window);
		_gameData.window.draw(this->maxBefore);
		_gameData.window.draw(this->maxAfter);
		_gameData.window.draw(this->textStat);
		_gameData.window.draw(this->arrow);

		if ((this->buttonSelectedUpgrade == 2 && (this->filledBLock[this->currentColorPlayer][2][4].colored || this->filledBLock[this->currentColorPlayer][2][9].colored)) ||
			(this->buttonSelectedUpgrade == 3 && this->filledBLock[this->currentColorPlayer][3][4].colored))
		{
			_gameData.window.draw(this->textUnlockSkill);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		_gameData.window.draw(this->stats[i]);
	}
}

void Upgrade::Display3D(GameData& _gameData)
{
	/*Utils::BlitSprite(this->backGround, sf::Vector2f{ this->backGround.getPosition().x - (TROAD_GAP * (-1 * this->backGround.getPosition().y + WORKING_HEIGHT) / 100.f),this->backGround.getPosition().y },0, _gameData.renderTextureRed);
	Utils::BlitSprite(this->backGround, sf::Vector2f{ this->backGround.getPosition().x + (TROAD_GAP * (-1 * this->backGround.getPosition().y + WORKING_HEIGHT) / 100.f),this->backGround.getPosition().y },0, _gameData.renderTextureBlue);

	for (int i = 0; i < STATS_NBR; i++)
	{
		Utils::BlitSprite(this->plank[i][0]->sprite, sf::Vector2f{ this->plank[i][0]->sprite.getPosition().x + (TROAD_GAP * (-1 * this->plank[i][0]->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->plank[i][0]->sprite.getPosition().y },0, _gameData.renderTextureRed);
		Utils::BlitSprite(this->plank[i][0]->sprite, sf::Vector2f{ this->plank[i][0]->sprite.getPosition().x - (TROAD_GAP * (-1 * this->plank[i][0]->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->plank[i][0]->sprite.getPosition().y },0, _gameData.renderTextureBlue);

		for (int j = 0; j < 2; j++)
		{
			Utils::BlitSprite(this->noButton[currentColorPlayer][i][j]->sprite, sf::Vector2f{ this->noButton[currentColorPlayer][i][j]->sprite.getPosition().x + (TROAD_GAP * (-1 * this->noButton[currentColorPlayer][i][j]->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->noButton[currentColorPlayer][i][j]->sprite.getPosition().y },0, _gameData.renderTextureRed);
			Utils::BlitSprite(this->noButton[currentColorPlayer][i][j]->sprite, sf::Vector2f{ this->noButton[currentColorPlayer][i][j]->sprite.getPosition().x - (TROAD_GAP * (-1 * this->noButton[currentColorPlayer][i][j]->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->noButton[currentColorPlayer][i][j]->sprite.getPosition().y },0, _gameData.renderTextureBlue);
		}

		for (int i = 0; i < STATS_NBR; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				emptyBlock.setPosition(plank[0][0]->getPosition().x + 230 + (j * 60), plank[i][0]->getPosition().y + 35);

				Utils::BlitSprite(this->emptyBlock, sf::Vector2f{ this->emptyBlock.getPosition().x + (TROAD_GAP * (-1 * this->emptyBlock.getPosition().y + WORKING_HEIGHT) / 100.f),this->emptyBlock.getPosition().y }, 0, _gameData.renderTextureRed);
				Utils::BlitSprite(this->emptyBlock, sf::Vector2f{ this->emptyBlock.getPosition().x - (TROAD_GAP * (-1 * this->emptyBlock.getPosition().y + WORKING_HEIGHT) / 100.f),this->emptyBlock.getPosition().y }, 0, _gameData.renderTextureBlue);
			}
		}

		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{
			for (int j = 0; j < _gameData.tabPlayers[i]->GetVigor(); j++)
			{
				filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].setPosition(plank[0][0]->getPosition().x + 230 + (j * 60), plank[0][0]->getPosition().y + 35);

				Utils::BlitSprite(this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().x + (TROAD_GAP * (-1 * this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y + WORKING_HEIGHT) / 100.f),this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y }, 0, _gameData.renderTextureRed);
				Utils::BlitSprite(this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().x - (TROAD_GAP * (-1 * this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y + WORKING_HEIGHT) / 100.f),this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y }, 0, _gameData.renderTextureBlue);
			}
			for (int j = 0; j < _gameData.tabPlayers[i]->GetStrength(); j++)
			{
				filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].setPosition(plank[0][0]->getPosition().x + 230 + (j * 60), plank[1][0]->getPosition().y + 35);

				Utils::BlitSprite(this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().x + (TROAD_GAP * (-1 * this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y + WORKING_HEIGHT) / 100.f),this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y }, 0, _gameData.renderTextureRed);
				Utils::BlitSprite(this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().x - (TROAD_GAP * (-1 * this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y + WORKING_HEIGHT) / 100.f),this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y }, 0, _gameData.renderTextureBlue);
			}
			for (int j = 0; j < _gameData.tabPlayers[i]->GetAgility(); j++)
			{
				filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].setPosition(plank[0][0]->getPosition().x + 230 + (j * 60), plank[2][0]->getPosition().y + 35);

				Utils::BlitSprite(this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().x + (TROAD_GAP * (-1 * this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y + WORKING_HEIGHT) / 100.f),this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y }, 0, _gameData.renderTextureRed);
				Utils::BlitSprite(this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().x - (TROAD_GAP * (-1 * this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y + WORKING_HEIGHT) / 100.f),this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y }, 0, _gameData.renderTextureBlue);
			}
			for (int j = 0; j < _gameData.tabPlayers[i]->GetWisdom(); j++)
			{
				filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].setPosition(plank[0][0]->getPosition().x + 230 + (j * 60), plank[3][0]->getPosition().y + 35);

				Utils::BlitSprite(this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().x + (TROAD_GAP * (-1 * this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y + WORKING_HEIGHT) / 100.f),this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y }, 0, _gameData.renderTextureRed);
				Utils::BlitSprite(this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().x - (TROAD_GAP * (-1 * this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y + WORKING_HEIGHT) / 100.f),this->filledBlocks[_gameData.tabPlayers[i]->GetPlayerSelected()].getPosition().y }, 0, _gameData.renderTextureBlue);
			}
		}
	}

	Utils::BlitSprite(this->colorBackGround[currentColorPlayer]->sprite, sf::Vector2f{ this->colorBackGround[currentColorPlayer]->sprite.getPosition().x + (TROAD_GAP * (-1 * this->colorBackGround[currentColorPlayer]->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->colorBackGround[currentColorPlayer]->sprite.getPosition().y },0, _gameData.renderTextureRed);
	Utils::BlitSprite(this->colorBackGround[currentColorPlayer]->sprite, sf::Vector2f{ this->colorBackGround[currentColorPlayer]->sprite.getPosition().x - (TROAD_GAP * (-1 * this->colorBackGround[currentColorPlayer]->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->colorBackGround[currentColorPlayer]->sprite.getPosition().y },0, _gameData.renderTextureBlue);

	Utils::BlitSprite(this->characterFrame[currentColorPlayer]->sprite, sf::Vector2f{ this->characterFrame[currentColorPlayer]->sprite.getPosition().x + (TROAD_GAP * (-1 * this->characterFrame[currentColorPlayer]->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->characterFrame[currentColorPlayer]->sprite.getPosition().y },0, _gameData.renderTextureRed);
	Utils::BlitSprite(this->characterFrame[currentColorPlayer]->sprite, sf::Vector2f{ this->characterFrame[currentColorPlayer]->sprite.getPosition().x - (TROAD_GAP * (-1 * this->characterFrame[currentColorPlayer]->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->characterFrame[currentColorPlayer]->sprite.getPosition().y },0, _gameData.renderTextureBlue);

	Utils::BlitSprite(this->pointsPlank->sprite, sf::Vector2f{ this->pointsPlank->sprite.getPosition().x + (TROAD_GAP * (-1 * this->pointsPlank->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->pointsPlank->sprite.getPosition().y },0, _gameData.renderTextureRed);
	Utils::BlitSprite(this->pointsPlank->sprite, sf::Vector2f{ this->pointsPlank->sprite.getPosition().x - (TROAD_GAP * (-1 * this->pointsPlank->sprite.getPosition().y + WORKING_HEIGHT) / 100.f), this->pointsPlank->sprite.getPosition().y },0, _gameData.renderTextureBlue);

	Utils::BlitSprite(this->textPlank->sprite, sf::Vector2f{ this->textPlank->sprite.getPosition().x + (TROAD_GAP * (-1 * this->textPlank->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->textPlank->sprite.getPosition().y },0, _gameData.renderTextureRed);
	Utils::BlitSprite(this->textPlank->sprite, sf::Vector2f{ this->textPlank->sprite.getPosition().x - (TROAD_GAP * (-1 * this->textPlank->sprite.getPosition().y + WORKING_HEIGHT) / 100.f), this->textPlank->sprite.getPosition().y },0, _gameData.renderTextureBlue);

	Utils::BlitSprite(this->mult->sprite, sf::Vector2f{ this->mult->sprite.getPosition().x + (TROAD_GAP * (-1 * this->mult->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->mult->sprite.getPosition().y },0, _gameData.renderTextureRed);
	Utils::BlitSprite(this->mult->sprite, sf::Vector2f{ this->mult->sprite.getPosition().x - (TROAD_GAP * (-1 * this->mult->sprite.getPosition().y + WORKING_HEIGHT) / 100.f), this->mult->sprite.getPosition().y },0, _gameData.renderTextureBlue);

	for (int i = 0; i < 4; i++)
	{
		Utils::BlitSprite(this->stats[i], sf::Vector2f{ this->stats[i].getPosition().x + (TROAD_GAP * (-1 * this->stats[i].getPosition().y + WORKING_HEIGHT) / 100.f), this->stats[i].getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(this->stats[i], sf::Vector2f{ this->stats[i].getPosition().x - (TROAD_GAP * (-1 * this->stats[i].getPosition().y + WORKING_HEIGHT) / 100.f), this->stats[i].getPosition().y }, 0, _gameData.renderTextureBlue);
	}

	Utils::BlitSprite(this->confirm->sprite, sf::Vector2f{ this->confirm->sprite.getPosition().x + (TROAD_GAP * (-1 * this->confirm->sprite.getPosition().y + WORKING_HEIGHT) / 100.f),this->confirm->sprite.getPosition().y },0, _gameData.renderTextureRed);
	Utils::BlitSprite(this->confirm->sprite, sf::Vector2f{ this->confirm->sprite.getPosition().x - (TROAD_GAP * (-1 * this->confirm->sprite.getPosition().y + WORKING_HEIGHT) / 100.f), this->confirm->sprite.getPosition().y },0, _gameData.renderTextureBlue);*/
}

void Upgrade::JoystickPressed(GameData& _gameData)
{

}

void Upgrade::KeyPressed(GameData& _gameData)
{

}

void Upgrade::LostFocus(GameData& _gameData)
{

}

void Upgrade::SetAlpha(const float _alpha)
{

}

void Upgrade::SetFadeOut(const bool _fadeOut)
{

}

void Upgrade::InitText(sf::Font& _font)
{
	this->maxBefore.setFont(_font);
	this->maxBefore.setCharacterSize(50);
	this->maxBefore.setString(std::to_string(this->maxLife));
	this->maxBefore.setPosition(sf::Vector2f{ 300.f,675.f });

	this->maxAfter.setFont(_font);
	this->maxAfter.setCharacterSize(50);
	this->maxAfter.setString(std::to_string(this->maxLife));
	this->maxAfter.setPosition(sf::Vector2f{ 520.f,675.f });

	this->textStat.setFont(_font);
	this->textStat.setCharacterSize(50);
	this->textStat.setString(statsList[0]);
	this->textStat.setOrigin(this->textStat.getLocalBounds().width/2, this->textStat.getLocalBounds().height / 2);
	this->textStat.setPosition(sf::Vector2f{ 450.f,600.f });	
	
	this->textUnlockSkill.setFont(_font);
	this->textUnlockSkill.setCharacterSize(50);
	this->textUnlockSkill.setString(unlcokDash);
	this->textUnlockSkill.setOrigin(this->textUnlockSkill.getLocalBounds().width/2, this->textUnlockSkill.getLocalBounds().height / 2);
	this->textUnlockSkill.setPosition(sf::Vector2f{ 350.f,790.f });
}

void Upgrade::LoadSprite()
{
	//backgrounds
	backGround = Utils::LoadSprite("Assets/Images/UpgradeMenu/Background.png", TOP_LEFT);
	
	//arrow new stat
	arrow = Utils::LoadSprite("Assets/Images/UpgradeMenu/ShitButton.png", TOP_LEFT);
	arrow.setPosition(sf::Vector2f{400.f, 650.f});

	//colonne de droite
	for (int i = 0; i < STATS_NBR; i++)
	{
		plank[i][0] = new Button("Assets/Images/UpgradeMenu/Planks/Plank.png", Screen::Pos(825, 30 + (200 * i)), 1, Screen::Scale(1, 1), false, false);
		plank[i][1] = new Button("Assets/Images/UpgradeMenu/Planks/Shadow_Plank.png", Screen::Pos(825, 30 + (200 * i)), 1, Screen::Scale(1, 1), false, false);
	}

	//Buttons
	for (int i = 0; i < 4; i++)
	{
		for (int k = 0; k < 4; k++)
		{
			justButton[i][k][0] = new Button("Assets/Images/UpgradeMenu/Buttons/Button/Left/" + color[i] + ".png", Screen::Pos(plank[k][0]->getPosition().x - 150, plank[k][0]->getPosition().y + 25), 1, Screen::Scale(1, 1), false, false);
			justButton[i][k][1] = new Button("Assets/Images/UpgradeMenu/Buttons/Button/Right/" + color[i] + ".png", Screen::Pos(plank[k][0]->getPosition().x + plank[i][0]->sprite.getGlobalBounds().width + 40, plank[k][0]->getPosition().y + 25), 1, Screen::Scale(1, 1), false, false);

			noButton[i][k][0] = new Button("Assets/Images/UpgradeMenu/Buttons/NoButton/Left/" + color[i] + ".png", Screen::Pos(plank[k][0]->getPosition().x - 150, plank[k][0]->getPosition().y + 25), 1, Screen::Scale(1, 1), false, false);
			noButton[i][k][1] = new Button("Assets/Images/UpgradeMenu/Buttons/NoButton/Right/" + color[i] + ".png", Screen::Pos(plank[k][0]->getPosition().x + plank[i][0]->sprite.getGlobalBounds().width + 40, plank[k][0]->getPosition().y + 25), 1, Screen::Scale(1, 1), false, false);

			pressButton[i][k][0] = new Button("Assets/Images/UpgradeMenu/Buttons/PressButton/Left/" + color[i] + ".png", Screen::Pos(noButton[i][k][0]->getPosition().x, noButton[i][k][0]->getPosition().y), 1, Screen::Scale(1, 1), false, false);
			pressButton[i][k][1] = new Button("Assets/Images/UpgradeMenu/Buttons/PressButton/Right/" + color[i] + ".png", Screen::Pos(noButton[i][k][1]->getPosition().x, noButton[i][k][1]->getPosition().y), 1, Screen::Scale(1, 1), false, false);

			selectButton[i][k][0] = new Button("Assets/Images/UpgradeMenu/Buttons/SelectButton/Left/" + color[i] + ".png", Screen::Pos(noButton[i][k][0]->getPosition().x, noButton[i][k][0]->getPosition().y), 1, Screen::Scale(1, 1), false, false);
			selectButton[i][k][1] = new Button("Assets/Images/UpgradeMenu/Buttons/SelectButton/Right/" + color[i] + ".png", Screen::Pos(noButton[i][k][1]->getPosition().x, noButton[i][k][1]->getPosition().y), 1, Screen::Scale(1, 1), false, false);
		}
	}

	stats[0] = Utils::LoadSprite("Assets/Images/UpgradeMenu/Stats/Life.png", TOP_LEFT);
	stats[1] = Utils::LoadSprite("Assets/Images/UpgradeMenu/Stats/Strength.png", TOP_LEFT);
	stats[2] = Utils::LoadSprite("Assets/Images/UpgradeMenu/Stats/Agility.png", TOP_LEFT);
	stats[3] = Utils::LoadSprite("Assets/Images/UpgradeMenu/Stats/Wisdom.png", TOP_LEFT);


	for (int i = 0; i < 4; i++)
	{
		filledBlocks[i] = Utils::LoadSprite("Assets/Images/UpgradeMenu/Blocks/FilledBlock/" + color[i] + ".png", CENTER);
		kingButtons[i] = Utils::LoadSprite("Assets/Images/UpgradeMenu/Blocks/KingButton/" + color[i] + ".png", CENTER);
	}

	emptyBlock = Utils::LoadSprite("Assets/Images/UpgradeMenu/Blocks/EmptyBlock.png", CENTER);
	emptyKingButtons = Utils::LoadSprite("Assets/Images/UpgradeMenu/Blocks/EmptyKingBlock.png", CENTER);

	for (int i = 0; i < 4; i++)
	{
		this->lockedCrown[i] = Utils::LoadSprite("Assets/Images/UpgradeMenu/Blocks/Locked" + color[i] + "KingBlock.png", CENTER);
		this->lockedBlock[i] = Utils::LoadSprite("Assets/Images/UpgradeMenu/Blocks/" + color[i] + "LockedBlock.png", CENTER);
	}

	//colonne de gauche
	for (int i = 0; i < 4; i++)
	{
		characterFrame[i] = new Button("Assets/Images/UpgradeMenu/Planks/CharacterFrame/" + color[i] + ".png", Screen::Pos(80, 35), 1, Screen::Scale(1, 1), false, false);
		colorBackGround[i] = new Button("Assets/Images/UpgradeMenu/Planks/ColorBackGround/" + color[i] + ".png", Screen::Pos(characterFrame[i]->getPosition().x + 50, characterFrame[i]->getPosition().y + 60), 1, Screen::Scale(1, 1), false, false);
	}

	pointsPlank = new Button("Assets/Images/UpgradeMenu/Planks/PointsPlank.png", Screen::Pos(characterFrame[0]->getPosition().x, characterFrame[0]->getPosition().y + 815), 1, Screen::Scale(1, 1), false, false);
	textPlank = new Button("Assets/Images/UpgradeMenu/Planks/TextPlank.png", Screen::Pos(characterFrame[0]->getPosition().x, characterFrame[0]->getPosition().y + 525), 1, Screen::Scale(1, 1), false, false);
	mult = new Button("Assets/Images/UpgradeMenu/Blocks/mult.png", Screen::Pos(pointsPlank->getPosition().x + 120, pointsPlank->getPosition().y + 80), 1, Screen::Scale(1, 1), false, false);

	for (int i = 0; i < 4; i++)
	{
		filledBlocksPlank[i] = Utils::LoadSprite("Assets/Images/UpgradeMenu/Blocks/FilledBlock/" + color[i] + ".png", TOP_LEFT);
		filledBlocksPlank[i].setPosition({ pointsPlank->getPosition().x + 35, pointsPlank->getPosition().y + 25 });
	}

	confirm[0] = new Button("Assets/Images/UpgradeMenu/Buttons/Confirm/Confirm.png", Screen::Pos(plank[3][0]->getPosition().x + 500, plank[3][0]->getPosition().y + 300), 1, Screen::Scale(1, 1), false, true);
	confirm[1] = new Button("Assets/Images/UpgradeMenu/Buttons/Confirm/ConfirmClicked.png", Screen::Pos(plank[3][0]->getPosition().x + 500, plank[3][0]->getPosition().y + 300), 1, Screen::Scale(1, 1), false, true);
	confirm[2] = new Button("Assets/Images/UpgradeMenu/Buttons/Confirm/ConfirmHighlight.png", Screen::Pos(plank[3][0]->getPosition().x + 500, plank[3][0]->getPosition().y + 300), 1, Screen::Scale(1, 1), false, true);
}

void Upgrade::InitTexte(sf::Font& _font)
{
	for (int i = 0; i < STATS_NBR; i++)
	{
		sf::Text* text = new sf::Text;
		text->setFont(_font);
		text->setCharacterSize(25);
		text->setFillColor(sf::Color::White);
		text->setString(statsList[i]);
		text->setPosition({ 100, (float)(100 + i * 200) });
		this->tabText.push_back(text);

		skillPoints.setFont(_font);
		skillPoints.setCharacterSize(100);
		skillPoints.setFillColor(sf::Color::White);
		skillPoints.setPosition({ pointsPlank->getPosition().x + 200, pointsPlank->getPosition().y + 10 });

		sf::Text* textStats = new sf::Text;
		textStats->setFont(_font);
		textStats->setCharacterSize(25);
		textStats->setFillColor(sf::Color::White);
		textStats->setPosition({ 400, (float)(100 + i * 200) });
		this->tabTextStats.push_back(textStats);

		sf::RectangleShape* rect = new sf::RectangleShape;
		rect->setSize({ 200, 50 });
		rect->setFillColor(sf::Color::Magenta);
		sf::Vector2f pos = { 100, (float)(100 + i * 200) };
		rect->setPosition(pos);
		this->tabButton.push_back(rect);
		this->tabPos.push_back(pos);
	}
}

void Upgrade::InitPoint()
{
	//init first point
	for (int i = 0; i < NbMaxPlayer; i++)
	{
		for (int j = 0; j < NbMaxPlanks; j++)
		{
			this->filledBLock[i][j][0].sprite = this->lockedBlock[i];
			this->filledBLock[i][j][0].pos = sf::Vector2f{ this->plank[j][0]->getPosition().x + 240.f ,plank[j][0]->getPosition().y + 90.f };
			this->filledBLock[i][j][0].lock = true;
			this->filledBLock[i][j][0].crown = false;
			this->filledBLock[i][j][0].colored = false;
		}
	}

	//all point whitout crown and the first
	for (int i = 0; i < NbMaxPlayer; i++)
	{
		for (int j = 0; j < NbMaxPlanks; j++)
		{
			for (int k = 1; k < NbMaxPoint - 1; k++)
			{
				this->filledBLock[i][j][k].sprite = this->emptyBlock;
				this->filledBLock[i][j][k].lock = false;
				this->filledBLock[i][j][k].crown = false;
				this->filledBLock[i][j][k].pos = sf::Vector2f{ this->filledBLock[i][j][k - 1].pos.x + 60.f,this->filledBLock[i][j][0].pos.y };
				this->filledBLock[i][j][k].colored = false;
			}
		}
	}

	//point crown
	for (int i = 0; i < NbMaxPlayer; i++)
	{
		//middle crown
		for (int j = 0; j < NbMaxPlanks; j++)
		{
			this->filledBLock[i][j][9].sprite = this->emptyKingButtons;
			this->filledBLock[i][j][9].pos = sf::Vector2f{ this->filledBLock[i][j][8].pos.x + 60.f,this->filledBLock[i][j][8].pos.y - OffsetCrownBlocks };
			this->filledBLock[i][j][9].lock = false;
			this->filledBLock[i][j][9].crown = true;
			this->filledBLock[i][j][9].colored = false;
		}

		//end crown
		for (int j = 0; j < NbMaxPlanks; j++)
		{
			this->filledBLock[i][j][4].sprite = this->emptyKingButtons;
			this->filledBLock[i][j][4].pos = sf::Vector2f{ this->filledBLock[i][j][3].pos.x + 60.f,this->filledBLock[i][j][3].pos.y - OffsetCrownBlocks };
			this->filledBLock[i][j][4].lock = false;
			this->filledBLock[i][j][4].crown = true;
			this->filledBLock[i][j][4].colored = false;
		}
	}
}

void Upgrade::ChangeLock()
{
	for (int i = 0; i < NbMaxPlayer; i++)
	{
		for (int j = 0; j < NbMaxPlanks; j++)
		{
			for (int k = 0; k < NbMaxPoint; k++)
			{
				if (this->filledBLock[i][j][k].colored)
				{
					this->filledBLock[i][j][k].lock = true;
					this->filledBLock[i][j][k].colored = false;
					this->filledBLock[i][j][k].sprite = this->lockedBlock[i];
					if (this->filledBLock[i][j][k].crown)
					{
						this->filledBLock[i][j][k].sprite = this->lockedCrown[i];
					}
				}
			}
		}
	}
}

void Upgrade::CrateAnimPlayerIdle()
{
	sf::IntRect rect = sf::IntRect(0, 0, 132, 167);
	this->animPlayerIdle->CreateLeftToRight(this->idlePlayer[this->currentColorPlayer], 8, 1, sf::Vector2u{ 1056,167 });
	this->idlePlayer[this->currentColorPlayer].setTextureRect(rect);
	this->animPlayerIdle->currentFrame = 0;
}

void Upgrade::UpdateAnimIdlePlayer(float _dt)
{
	bool temp = true;
	this->animPlayerIdle->UpdateLeftToRight(this->idlePlayer[this->currentColorPlayer], _dt, 8, 0, temp, true);
}

void Upgrade::MaxStatPlayer(Player* _player)
{
	this->maxLife = _player->GetMaxLife();
	this->maxMana = _player->GetMana();
	this->maxDamage = _player->GetStrengthDamage();
	this->maxSpeed = _player->GetRealSpeed();
}

void Upgrade::UpdateMaxStatText(Player* _player)
{
	switch (this->buttonSelectedUpgrade)
	{
	case 0:
		this->maxAfter.setString(std::to_string(_player->GetMaxLife()));
		this->maxBefore.setString(std::to_string(this->maxLife));
		break;

	case 1:
		this->maxAfter.setString(std::to_string(_player->GetStrengthDamage()));
		this->maxBefore.setString(std::to_string(this->maxDamage));
		break;

	case 2:
		this->maxAfter.setString(std::to_string(_player->GetRealSpeed()));
		this->maxBefore.setString(std::to_string(this->maxSpeed));
		break;

	case 3:
		this->maxAfter.setString(std::to_string(_player->GetMaxMana()));
		this->maxBefore.setString(std::to_string(this->maxMana));
		break;

	default:
		break;
	}
}

void Upgrade::Init(GameData& _gameData)
{
	InitUpgrade(_gameData);
	CrateAnimPlayerIdle();
	MaxStatPlayer(_gameData.tabPlayers[this->currentPlayer]);
	InitText(*_gameData.GameFont);
	buttonSelectedUpgrade = (int)ButtonSelected::LIFE;
}