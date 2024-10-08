#include "stdafx.h"
#include "HUD.h"
#include "Player.hpp"
#include "CharacterSelection.h"

void HUD::InitHud(std::vector<Player*>& _tabPlayers, std::vector<int>& _playerSelected, sf::RenderWindow& _window)
{
	InitSprites(_tabPlayers);
	InitComponents(_window, _tabPlayers);
	InitPortraits(_tabPlayers, _playerSelected, _window);
}

void HUD::InitPortraits(std::vector<Player*>& _tabPlayers, std::vector<int>& _playerSelected, sf::RenderWindow& _window)
{
	for (int i = 0; i < _tabPlayers.size(); i++)
	{
		switch ((SpritesPlayersSkin)_playerSelected[i])
		{
		case SpritesPlayersSkin::RED:
			spriteFramePlayers[i] = Utils::LoadSprite("Assets/Images/Hud/FirePortrait.png", CENTER);
			spriteIconsPlayers[i] = Utils::LoadSprite("Assets/Images/Hud/Red_Head.png", CENTER);
			break;
		case SpritesPlayersSkin::GREEN:
			spriteFramePlayers[i] = Utils::LoadSprite("Assets/Images/Hud/PoisonPortrait.png", CENTER);
			spriteIconsPlayers[i] = Utils::LoadSprite("Assets/Images/Hud/Green_Head.png", CENTER);
			break;
		case SpritesPlayersSkin::BLUE:
			spriteFramePlayers[i] = Utils::LoadSprite("Assets/Images/Hud/IcePortrait.png", CENTER);
			spriteIconsPlayers[i] = Utils::LoadSprite("Assets/Images/Hud/Blue_Head.png", CENTER);
			break;
		case SpritesPlayersSkin::YELLOW:
			spriteFramePlayers[i] = Utils::LoadSprite("Assets/Images/Hud/ThunderPortrait.png", CENTER);
			spriteIconsPlayers[i] = Utils::LoadSprite("Assets/Images/Hud/Yellow_Head.png", CENTER);
			break;
		default:
			std::cout << "Pas de playerSelected oops" << std::endl;
		}

		spriteFramePlayers[i].setPosition((_window.mapPixelToCoords({ (int)((_window.getSize().x / 13) * (i + 1) * 2.f + i * spriteFramePlayers[i].getGlobalBounds().width), (int)(spriteFramePlayers[i].getGlobalBounds().height / 2) })));
	}
}

// Health icon
void HUD::LoadHealthSprites()
{
	allHealthSprites.resize(NB_SPRITE_HEALTH);
	allHealthSprites[EMPTYHEART] = Utils::LoadSprite("Assets/Images/Hud/EmptyHeart.png", CENTER);
	allHealthSprites[QUARTERHEART] = Utils::LoadSprite("Assets/Images/Hud/QuarterHeart.png", CENTER);
	allHealthSprites[HALFHEART] = Utils::LoadSprite("Assets/Images/Hud/HalfHeart.png", CENTER);
	allHealthSprites[THREEQUARTERHEART] = Utils::LoadSprite("Assets/Images/Hud/ThreeQuarterHeart.png", CENTER);
	allHealthSprites[FULLHEART] = Utils::LoadSprite("Assets/Images/Hud/FullHeart.png", CENTER);
}

// Mana icon
void HUD::LoadManaSprites()
{
	allManaSprites.resize(NB_SPRITE_MANA);
	allManaSprites[USEDMANA] = Utils::LoadSprite("Assets/Images/Hud/ManaCristalUsed.png", CENTER);
	allManaSprites[USEDMANA].setColor(sf::Color(255, 255, 255, 125));
	allManaSprites[MANA] = Utils::LoadSprite("Assets/Images/Hud/ManaCristal.png", CENTER);

}

// Potions icon
void HUD::LoadPotionsSprites()
{
	allPotionsSprites.resize(NB_SPRITES_POTIONS);
	allPotionsSprites[EMPTY_POTION] = Utils::LoadSprite("Assets/Images/Hud/EmptyPotion.png", CENTER);
	allPotionsSprites[HEALTH_POTION] = Utils::LoadSprite("Assets/Images/Hud/IconHealthPotion.png", CENTER);
	allPotionsSprites[STRENGTH_BUFF_POTION] = Utils::LoadSprite("Assets/Images/Hud/IconStrengthBuffPotion.png", CENTER);
	allPotionsSprites[SPEED_BUFF_POTION] = Utils::LoadSprite("Assets/Images/Hud/IconSpeedBuffPotion.png", CENTER);
	allPotionsSprites[MAGIC_BUFF_POTION] = Utils::LoadSprite("Assets/Images/Hud/IconMagicBuffPotion.png", CENTER);
}

void HUD::LoadXpSprites(std::vector<Player*>& _tabPlayers)
{
	//Xp bar
	tempXpPixel = Utils::LoadSprite("Assets/Images/Hud/XpPixel.png", CENTER);
	tempXpBarBackground = Utils::LoadSprite("Assets/Images/Hud/XpFond.png", CENTER);
	tempXpBarOver = Utils::LoadSprite("Assets/Images/Hud/XpOver.png", CENTER);
}

void HUD::InitSprites(std::vector<Player*>& _tabPlayers)
{
	hudFont.loadFromFile("Assets/Fonts/RulesFont.otf");

	LoadHealthSprites();
	LoadManaSprites();
	LoadXpSprites(_tabPlayers);
	LoadPotionsSprites();
	
	spriteRevive = Utils::LoadSprite("Assets/Images/Hud/R.png", CENTER);
	spriteManaRecovery = Utils::LoadSprite("Assets/Images/Hud/ManaRecovery.png", CENTER, 3, 1);
}

void HUD::InitComponents(sf::RenderWindow& _window, std::vector<Player*>& _tabPlayers)
{
	// Player icon
	spriteIconsPlayers.resize(_tabPlayers.size());
	spriteFramePlayers.resize(spriteIconsPlayers.size());
	spritePlayersCanBeRevived.resize(spriteIconsPlayers.size());
	spritePlayersManaRecovery.resize(spriteIconsPlayers.size());
	playAnimManaRecovery.resize(spriteIconsPlayers.size());
	animManaRecovery.resize(spriteIconsPlayers.size());
	spritePlayersInventory.resize(spriteIconsPlayers.size());
	
	for (int i = 0; i < spriteIconsPlayers.size(); i++)
	{
		spriteIconsPlayers[i].setPosition((_window.mapPixelToCoords({ (int)((_window.getSize().x / 13) * (i + 1) * 2.f + i * spriteIconsPlayers[i].getGlobalBounds().width),(int)_window.getSize().y / 8 })));
	}

	spriteHealthPlayers.resize(spriteIconsPlayers.size());
	for (int i = 0; i < spriteIconsPlayers.size(); i++)
	{
		spriteHealthPlayers[i].resize(ceil((float)_tabPlayers[i]->GetMaxLife() / 4));
		for (int j = 0; j < spriteHealthPlayers[i].size(); j++)
		{
			spriteHealthPlayers[i][j] = allHealthSprites[FULLHEART];
		}
	}

	spriteManaPlayers.resize(spriteIconsPlayers.size());
	for (int i = 0; i < spriteManaPlayers.size(); i++)
	{
		spriteManaPlayers[i].resize(_tabPlayers[i]->GetMaxMana());
		for (int j = 0; j < spriteManaPlayers[i].size(); j++)
		{
			spriteManaPlayers[i][j] = allManaSprites[MANA];
		}
	}

	spriteXpBarPlayers.resize(spriteIconsPlayers.size());
	for (int i = 0; i < spriteXpBarPlayers.size(); i++)
	{
		spriteXpBarPlayers[i].resize(_tabPlayers[i]->GetMaxXp());
		for (int j = 0; j < spriteXpBarPlayers[i].size(); j++)
		{
			spriteXpBarPlayers[i][j] = tempXpPixel;
		}
		spriteXpBarBackgroundPlayers.resize(spriteIconsPlayers.size());
		spriteXpBarBackgroundPlayers[i] = tempXpBarBackground;
		spriteXpBarOverPlayers.resize(spriteIconsPlayers.size());
		spriteXpBarOverPlayers[i] = tempXpBarOver;
	}

	// Player LVL
	lvlText.resize(spriteIconsPlayers.size());
	for (int i = 0; i < spriteIconsPlayers.size(); i++)
	{
		lvlText[i].setString((std::to_string(_tabPlayers[i]->GetXp())));
		lvlText[i].setFont(hudFont);
	}

	// Player inventory
	for (int i = 0; i < spriteIconsPlayers.size(); i++)
	{
		if (_tabPlayers[i]->GetHasPotion() == true)
		{
			spritePlayersInventory[i] = allPotionsSprites[HEALTH_POTION];
		}
		else if (_tabPlayers[i]->GetHasStrengthBuff() == true)
		{
			spritePlayersInventory[i] = allPotionsSprites[STRENGTH_BUFF_POTION];
		}
		else if (_tabPlayers[i]->GetHasSpeedBuff() == true)
		{
			spritePlayersInventory[i] = allPotionsSprites[SPEED_BUFF_POTION];
		}
		else if (_tabPlayers[i]->GetHasMagicBuff() == true)
		{
			spritePlayersInventory[i] = allPotionsSprites[MAGIC_BUFF_POTION];
		}
		else
		{
			spritePlayersInventory[i] = allPotionsSprites[EMPTY_POTION];
		}
		spritePlayersInventory[i].setRotation(-30);
	}
	// Revive button
	for (int i = 0; i < spritePlayersCanBeRevived.size(); i++)
	{
		spritePlayersCanBeRevived[i] = spriteRevive;
	}
	InitHudRevive();

	// Mana recovery
	for (int i = 0; i < spriteIconsPlayers.size(); i++)
	{
		spritePlayersManaRecovery[i] = spriteManaRecovery;
		playAnimManaRecovery[i] = false;
	}
	InitHudManaRecovery();
}

// Init the revive button
void HUD::InitHudRevive()
{
	for (int i = 0; i < spritePlayersCanBeRevived.size(); i++)
	{
		animButtonRevive.CreateLeftToRight(spritePlayersCanBeRevived[i], 2, 1, spritePlayersCanBeRevived[i].getTexture()->getSize());

		sf::IntRect rect = sf::IntRect(0, 0, spritePlayersCanBeRevived[i].getTexture()->getSize().x / 2, spritePlayersCanBeRevived[i].getTexture()->getSize().y);
		spritePlayersCanBeRevived[i].setTextureRect(rect);
	}
}

// Init the mana recovery
void HUD::InitHudManaRecovery()
{
	for (int i = 0; i < spriteIconsPlayers.size(); i++)
	{
		animManaRecovery[i].CreateLeftToRight(spritePlayersManaRecovery[i], 3, 1, spritePlayersManaRecovery[i].getTexture()->getSize());
		animManaRecovery[i].currentFrame = 0;
		sf::IntRect rect = sf::IntRect(0, 0, spritePlayersManaRecovery[i].getTexture()->getSize().x / 3, spritePlayersManaRecovery[i].getTexture()->getSize().y);
		spritePlayersManaRecovery[i].setTextureRect(rect);
	}
}

void HUD::Reload(GameData& _gameData)
{
	InitComponents(_gameData.window, _gameData.tabPlayers);
}

void HUD::Update(GameData& _gameData)
{
	UpdatePos(_gameData);
	UpdateAnim(_gameData.dt, _gameData.tabPlayers);
}
void HUD::UpdateAnim(float _dt, std::vector<Player*>& _tabPlayers)
{
	bool temp = true;
	for (int i = 0; i < spritePlayersCanBeRevived.size(); i++)
	{
		animButtonRevive.UpdateLeftToRight(spritePlayersCanBeRevived[i], _dt, 1.75, 0, temp, true);
	}

	for (int i = 0; i < spriteIconsPlayers.size(); i++)
	{
		if (_tabPlayers[i]->GetIsManaRecoveringFinished())
		{
			playAnimManaRecovery[i] = true;
		}

		if (animManaRecovery[i].currentFrame == animManaRecovery[i].maxFrame)
		{
			playAnimManaRecovery[i] = false;
			animManaRecovery[i].currentFrame = 0;
		}

		if (playAnimManaRecovery[i])
		{
			animManaRecovery[i].UpdateLeftToRight(spritePlayersManaRecovery[i], _dt, 9, 0, temp, false);
		}
	}

}

void HUD::UpdatePosStoryMode(std::vector<Player*>& _tabPlayers, sf::RenderWindow& _window)
{
	for (int i = 0; i < spriteIconsPlayers.size(); i++)
	{
		//Update frame Players pos
		spriteFramePlayers[i].setPosition((_window.mapPixelToCoords({ (int)((_window.getSize().x / 19) * (i + 1) * 2 + i * spriteFramePlayers[i].getGlobalBounds().width),(int)(_window.getSize().y / 8) })));
		//Update icon Players pos
		spriteIconsPlayers[i].setPosition((_window.mapPixelToCoords({ (int)((_window.getSize().x / 19) * (i + 1) * 2 + i * spriteFramePlayers[i].getGlobalBounds().width),(int)(_window.getSize().y / 8/*spriteFramePlayers[i].getGlobalBounds().height / 2*/) })));

		//Update name Players pos
		//nameText[i].setPosition({ (float)(spriteIconsPlayers[i].getPosition().x - nameText[i].getGlobalBounds().width / 2.f), (float)(spriteIconsPlayers[i].getPosition().y - spriteIconsPlayers[i].getGlobalBounds().height / 2.f - nameText[i].getGlobalBounds().height * 2.f) });

		//Update health Players pos
		for (int j = 0; j < spriteHealthPlayers[i].size(); j++)
		{
			if (j <= _tabPlayers[i]->GetLife() / 4.f - 1)
			{
				spriteHealthPlayers[i][j] = allHealthSprites[FULLHEART];
			}
			else if (j <= _tabPlayers[i]->GetLife() / 4.f - 0.75f)
			{
				spriteHealthPlayers[i][j] = allHealthSprites[THREEQUARTERHEART];
			}
			else if (j <= _tabPlayers[i]->GetLife() / 4.f - 0.5f)
			{
				spriteHealthPlayers[i][j] = allHealthSprites[HALFHEART];
			}
			else if (j <= _tabPlayers[i]->GetLife() / 4.f - 0.25f)
			{
				spriteHealthPlayers[i][j] = allHealthSprites[QUARTERHEART];
			}
			else
			{
				spriteHealthPlayers[i][j] = allHealthSprites[EMPTYHEART];
			}

			if (j == 0)
			{
				spriteHealthPlayers[i][j].setPosition({ (float)(spriteFramePlayers[i].getPosition().x + spriteFramePlayers[i].getGlobalBounds().width / 2.f + spriteHealthPlayers[i][j].getGlobalBounds().width) - 50, (float)(spriteFramePlayers[i].getPosition().y - spriteFramePlayers[i].getGlobalBounds().height / 2.f + spriteHealthPlayers[i][j].getGlobalBounds().height / 2.f) + 75 });
			}
			else if (j <= 2)
			{
				spriteHealthPlayers[i][j].setPosition({ (float)(spriteHealthPlayers[i][j - 1].getPosition().x + spriteHealthPlayers[i][j].getGlobalBounds().width * 1.5f), (float)(spriteHealthPlayers[i][j - 1].getPosition().y) });
			}
			else
			{
				spriteHealthPlayers[i][j].setPosition({ (float)(spriteHealthPlayers[i][j - 3].getPosition().x), (float)(spriteHealthPlayers[i][j - 3].getPosition().y + spriteHealthPlayers[i][j].getGlobalBounds().height) });
			}
		}

		//Update mana Players pos
		for (int j = 0; j < spriteManaPlayers[i].size(); j++)
		{
			if (j < _tabPlayers[i]->GetMana())
			{
				spriteManaPlayers[i][j] = allManaSprites[MANA];
			}
			else
			{
				spriteManaPlayers[i][j] = allManaSprites[USEDMANA];
			}


			if (j == 0)
			{
				spriteManaPlayers[i][j].setPosition({ (float)(spriteHealthPlayers[i][j].getPosition().x), (float)(spriteHealthPlayers[i][spriteHealthPlayers[i].size() - 1].getPosition().y + spriteHealthPlayers[i][j].getGlobalBounds().height + spriteManaPlayers[i][j].getGlobalBounds().height / 4.f) });
			}
			else if (j <= 2)
			{
				spriteManaPlayers[i][j].setPosition({ (float)(spriteManaPlayers[i][j - 1].getPosition().x + spriteHealthPlayers[i][0].getGlobalBounds().width * 1.5f), (float)(spriteManaPlayers[i][j - 1].getPosition().y) });
			}
			else
			{
				spriteManaPlayers[i][j].setPosition({ (float)(spriteManaPlayers[i][j - 3].getPosition().x), (float)(spriteManaPlayers[i][j - 3].getPosition().y + spriteHealthPlayers[i][0].getGlobalBounds().height) });
			}
		}

		//Update xp bar Players pos
		for (int j = 0; j < spriteXpBarPlayers[i].size(); j++)
		{
			//spriteXpBarPlayers[i][j].setPosition({ (float)(spriteIconsPlayers[i].getPosition().x + spriteIconsPlayers[i].getGlobalBounds().width / 2.f + spriteHealthPlayers[i][0].getGlobalBounds().width + j), (float)(spriteManaPlayers[i][0].getPosition().y + spriteHealthPlayers[i][0].getGlobalBounds().height * 1.5f )});
			spriteXpBarPlayers[i][j].setPosition({ (float)(spriteManaPlayers[i][0].getPosition().x - spriteManaPlayers[i][0].getGlobalBounds().width / 2 + j), (float)(spriteManaPlayers[i][spriteManaPlayers[i].size()-1].getPosition().y + spriteManaPlayers[i][0].getGlobalBounds().height) });
		}
		spriteXpBarBackgroundPlayers[i].setPosition({ (float)(spriteXpBarPlayers[i][0].getPosition().x + spriteXpBarBackgroundPlayers[i].getGlobalBounds().width / 4 - spriteXpBarPlayers[i][0].getGlobalBounds().width), (float)(spriteXpBarPlayers[i][0].getPosition().y) });
		spriteXpBarOverPlayers[i].setPosition({ (float)(spriteXpBarBackgroundPlayers[i].getPosition().x), (float)(spriteXpBarBackgroundPlayers[i].getPosition().y) });

		//Update lvl Players text and Players text pos-
		lvlText[i].setString((std::to_string(_tabPlayers[i]->GetLevel())));
		lvlText[i].setPosition({ (float)(spriteXpBarOverPlayers[i].getPosition().x - spriteXpBarOverPlayers[i].getGlobalBounds().width / 2.4f), (float)(spriteXpBarOverPlayers[i].getPosition().y - spriteXpBarOverPlayers[i].getGlobalBounds().height / 2) });

		//Update spritePlayersCanBeRevived pos
		spritePlayersCanBeRevived[i].setPosition({ (float)(_tabPlayers[i]->GetPos().x), (float)(_tabPlayers[i]->GetPos().y - spritePlayersCanBeRevived[i].getGlobalBounds().height - 50) });

		//Update spritePlayersManaRecovery pos		
		if (playAnimManaRecovery[i])
		{
			if (_tabPlayers[i]->GetMana() > 0)
			{
				spritePlayersManaRecovery[i].setPosition({ (float)(spriteManaPlayers[i][_tabPlayers[i]->GetMana() - 1].getPosition().x), (float)(spriteManaPlayers[i][_tabPlayers[i]->GetMana() - 1].getPosition().y) });
			}
			else
			{
				spritePlayersManaRecovery[i].setPosition({ (float)(spriteManaPlayers[i][0].getPosition().x), (float)(spriteManaPlayers[i][0].getPosition().y) });
			}
		}
		
		//Update spritePlayersInventory pos
		if (_tabPlayers[i]->GetHasPotion() == true)
		{
			spritePlayersInventory[i] = allPotionsSprites[HEALTH_POTION];
		}
		else if (_tabPlayers[i]->GetHasStrengthBuff() == true)
		{
			spritePlayersInventory[i] = allPotionsSprites[STRENGTH_BUFF_POTION];
		}
		else if (_tabPlayers[i]->GetHasSpeedBuff() == true)
		{
			spritePlayersInventory[i] = allPotionsSprites[SPEED_BUFF_POTION];
		}
		else if (_tabPlayers[i]->GetHasMagicBuff() == true)
		{
			spritePlayersInventory[i] = allPotionsSprites[MAGIC_BUFF_POTION];
		}
		else
		{
			spritePlayersInventory[i] = allPotionsSprites[EMPTY_POTION];
		}
		spritePlayersInventory[i].setPosition({ (float)(spriteIconsPlayers[i].getPosition().x - spriteIconsPlayers[i].getGlobalBounds().width/2 - spritePlayersInventory[i].getGlobalBounds().width/2), (float)(spriteIconsPlayers[i].getPosition().y + 30) });
		spritePlayersInventory[i].setRotation(-30);
	}
}

void HUD::UpdatePosArenaTeamfightMode(std::vector<Player*>& _tabPlayers, sf::RenderWindow& _window, std::vector<int>& _playerTeam)
{
	for (int i = 0; i < spriteIconsPlayers.size(); i++)
	{
		(bool)(_playerTeam[i] == 0) ? playerInWhatTeam.push_back(0) : playerInWhatTeam.push_back(1);
	}

	int compteurTeam1 = 0;
	int compteurTeam2 = 0;
	for (int i = 0; i < spriteIconsPlayers.size(); i++)
	{
		if (playerInWhatTeam[i] == 0)
		{
			//Update frame Players pos
			spriteFramePlayers[i].setPosition((_window.mapPixelToCoords({ 130 + (compteurTeam1) * 350,(int)(spriteFramePlayers[i].getGlobalBounds().height / 2) })));
			//Update icon Players pos
			spriteIconsPlayers[i].setPosition((_window.mapPixelToCoords({ 130 + (compteurTeam1) * 350,(int)(spriteFramePlayers[i].getGlobalBounds().height / 2) })));

			compteurTeam1++;
		}
		else if (playerInWhatTeam[i] == 1)
		{
			//Update frame Players pos
			spriteFramePlayers[i].setPosition((_window.mapPixelToCoords({ 1650 - (compteurTeam2) * 350,(int)(spriteFramePlayers[i].getGlobalBounds().height / 2) })));
			//Update icon Players pos
			spriteIconsPlayers[i].setPosition((_window.mapPixelToCoords({ 1650 - (compteurTeam2) * 350,(int)(spriteFramePlayers[i].getGlobalBounds().height / 2) })));
			spriteIconsPlayers[i].setScale({ -1,1 });

			compteurTeam2++;
		}


		//Update health Players pos
		for (int j = 0; j < spriteHealthPlayers[i].size(); j++)
		{
			if (j <= _tabPlayers[i]->GetLife() / 4.f - 1)
			{
				spriteHealthPlayers[i][j] = allHealthSprites[FULLHEART];
			}
			else if (j <= _tabPlayers[i]->GetLife() / 4.f - 0.75f)
			{
				spriteHealthPlayers[i][j] = allHealthSprites[THREEQUARTERHEART];
			}
			else if (j <= _tabPlayers[i]->GetLife() / 4.f - 0.5f)
			{
				spriteHealthPlayers[i][j] = allHealthSprites[HALFHEART];
			}
			else if (j <= _tabPlayers[i]->GetLife() / 4.f - 0.25f)
			{
				spriteHealthPlayers[i][j] = allHealthSprites[QUARTERHEART];
			}
			else
			{
				spriteHealthPlayers[i][j] = allHealthSprites[EMPTYHEART];
			}

			if (j == 0)
			{
				spriteHealthPlayers[i][j].setPosition({ (float)(spriteFramePlayers[i].getPosition().x + spriteFramePlayers[i].getGlobalBounds().width / 2.f + spriteHealthPlayers[i][j].getGlobalBounds().width) - 50, (float)(spriteFramePlayers[i].getPosition().y - spriteFramePlayers[i].getGlobalBounds().height / 2.f + spriteHealthPlayers[i][j].getGlobalBounds().height / 2.f) + 75 });
			}
			else if (j <= 2)
			{
				spriteHealthPlayers[i][j].setPosition({ (float)(spriteHealthPlayers[i][j - 1].getPosition().x + spriteHealthPlayers[i][j].getGlobalBounds().width * 1.5f), (float)(spriteHealthPlayers[i][j - 1].getPosition().y) });
			}
			else
			{
				spriteHealthPlayers[i][j].setPosition({ (float)(spriteHealthPlayers[i][j - 3].getPosition().x), (float)(spriteHealthPlayers[i][j - 3].getPosition().y + spriteHealthPlayers[i][j].getGlobalBounds().height) });
			}
		}

		//Update mana Players pos
		for (int j = 0; j < spriteManaPlayers[i].size(); j++)
		{
			if (j < _tabPlayers[i]->GetMana())
			{
				spriteManaPlayers[i][j] = allManaSprites[MANA];
			}
			else
			{
				spriteManaPlayers[i][j] = allManaSprites[USEDMANA];
			}


			if (j == 0)
			{
				spriteManaPlayers[i][j].setPosition({ (float)(spriteHealthPlayers[i][j].getPosition().x), (float)(spriteHealthPlayers[i][spriteHealthPlayers[i].size() - 1].getPosition().y + spriteHealthPlayers[i][j].getGlobalBounds().height + spriteManaPlayers[i][j].getGlobalBounds().height / 4.f) });
			}
			else if (j <= 2)
			{
				spriteManaPlayers[i][j].setPosition({ (float)(spriteManaPlayers[i][j - 1].getPosition().x + spriteHealthPlayers[i][0].getGlobalBounds().width * 1.5f), (float)(spriteManaPlayers[i][j - 1].getPosition().y) });
			}
			else
			{
				spriteManaPlayers[i][j].setPosition({ (float)(spriteManaPlayers[i][j - 3].getPosition().x), (float)(spriteManaPlayers[i][j - 3].getPosition().y + spriteHealthPlayers[i][0].getGlobalBounds().height) });
			}
		}

		//Update spritePlayersCanBeRevived pos
		spritePlayersCanBeRevived[i].setPosition({ (float)(_tabPlayers[i]->GetPos().x), (float)(_tabPlayers[i]->GetPos().y - spritePlayersCanBeRevived[i].getGlobalBounds().height - 50) });

		//Update spritePlayersManaRecovery pos		
		if (playAnimManaRecovery[i])
		{
			if (_tabPlayers[i]->GetMana() > 0)
			{
				spritePlayersManaRecovery[i].setPosition({ (float)(spriteManaPlayers[i][_tabPlayers[i]->GetMana() - 1].getPosition().x), (float)(spriteManaPlayers[i][_tabPlayers[i]->GetMana() - 1].getPosition().y) });
			}
			else
			{
				spritePlayersManaRecovery[i].setPosition({ (float)(spriteManaPlayers[i][0].getPosition().x), (float)(spriteManaPlayers[i][0].getPosition().y) });
			}
		}
	}
}
void HUD::UpdateMaxLifePlayer(std::vector<Player*>& _tabPlayers, int _i)
{
	if (_tabPlayers[_i]->GetMaxLife() / 4 != spriteHealthPlayers[_i].size())
	{
		spriteHealthPlayers[_i].resize(ceil((float)_tabPlayers[_i]->GetMaxLife() / 4));
		for (int j = 0; j < spriteHealthPlayers[_i].size(); j++)
		{
			if (j <= _tabPlayers[_i]->GetLife() / 4.f - 1)
			{
				spriteHealthPlayers[_i][j] = allHealthSprites[FULLHEART];
			}
			else if (j <= _tabPlayers[_i]->GetLife() / 4.f - 0.75f)
			{
				spriteHealthPlayers[_i][j] = allHealthSprites[THREEQUARTERHEART];
			}
			else if (j <= _tabPlayers[_i]->GetLife() / 4.f - 0.5f)
			{
				spriteHealthPlayers[_i][j] = allHealthSprites[HALFHEART];
			}
			else if (j <= _tabPlayers[_i]->GetLife() / 4.f - 0.25f)
			{
				spriteHealthPlayers[_i][j] = allHealthSprites[QUARTERHEART];
			}
			else
			{
				spriteHealthPlayers[_i][j] = allHealthSprites[EMPTYHEART];
			}

			if (j == 0)
			{
				spriteHealthPlayers[_i][j].setPosition({ (float)(spriteFramePlayers[_i].getPosition().x + spriteFramePlayers[_i].getGlobalBounds().width / 2.f + spriteHealthPlayers[_i][j].getGlobalBounds().width) - 50, (float)(spriteFramePlayers[_i].getPosition().y - spriteFramePlayers[_i].getGlobalBounds().height / 2.f + spriteHealthPlayers[_i][j].getGlobalBounds().height / 2.f) + 75 });
			}
			else if (j <= 2)
			{
				spriteHealthPlayers[_i][j].setPosition({ (float)(spriteHealthPlayers[_i][j - 1].getPosition().x + spriteHealthPlayers[_i][j].getGlobalBounds().width * 1.5f), (float)(spriteHealthPlayers[_i][j - 1].getPosition().y) });
			}
			else
			{
				spriteHealthPlayers[_i][j].setPosition({ (float)(spriteHealthPlayers[_i][j - 3].getPosition().x), (float)(spriteHealthPlayers[_i][j - 3].getPosition().y + spriteHealthPlayers[_i][j].getGlobalBounds().height) });
			}
		}
	}
}

void HUD::UpdateMaxManaPlayer(std::vector<Player*>& _tabPlayers, int _i)
{
	if (_tabPlayers[_i]->GetMaxMana() != spriteManaPlayers[_i].size())
	{
		spriteManaPlayers[_i].resize(_tabPlayers[_i]->GetMaxMana());
		for (int j = 0; j < spriteManaPlayers[_i].size(); j++)
		{
			if (j < _tabPlayers[_i]->GetMana())
			{
				spriteManaPlayers[_i][j] = allManaSprites[MANA];
			}
			else
			{
				spriteManaPlayers[_i][j] = allManaSprites[USEDMANA];
			}


			if (j == 0)
			{
				spriteManaPlayers[_i][j].setPosition({ (float)(spriteHealthPlayers[_i][j].getPosition().x), (float)(spriteHealthPlayers[_i][spriteHealthPlayers[_i].size() - 1].getPosition().y + spriteHealthPlayers[_i][j].getGlobalBounds().height + spriteManaPlayers[_i][j].getGlobalBounds().height / 4.f) });
			}
			else if (j <= 2)
			{
				spriteManaPlayers[_i][j].setPosition({ (float)(spriteManaPlayers[_i][j - 1].getPosition().x + spriteHealthPlayers[_i][0].getGlobalBounds().width * 1.5f), (float)(spriteManaPlayers[_i][j - 1].getPosition().y) });
			}
			else
			{
				spriteManaPlayers[_i][j].setPosition({ (float)(spriteManaPlayers[_i][j - 3].getPosition().x), (float)(spriteManaPlayers[_i][j - 3].getPosition().y + spriteHealthPlayers[_i][0].getGlobalBounds().height) });
			}
		}
	}
}
void HUD::UpdatePos(GameData& _gameData)
{
	for (int i = 0; i < spriteIconsPlayers.size(); i++)
	{
		UpdateMaxLifePlayer(_gameData.tabPlayers, i);
		UpdateMaxManaPlayer(_gameData.tabPlayers, i);
	}

	if (!_gameData.teamfight)
	{
		UpdatePosStoryMode(_gameData.tabPlayers, _gameData.window);
	}
	else
	{
		UpdatePosArenaTeamfightMode(_gameData.tabPlayers, _gameData.window, _gameData.playerTeam);
	}
}

void HUD::Display(GameData& _gameData)
{
	for (int i = 0; i < spriteIconsPlayers.size(); i++)
	{
		Utils::BlitSprite(spriteIconsPlayers[i], spriteIconsPlayers[i].getPosition(), _gameData.window, 0);
		Utils::BlitSprite(spriteFramePlayers[i], spriteFramePlayers[i].getPosition(), _gameData.window, 0);

		for (int j = 0; j < spriteHealthPlayers[i].size(); j++)
		{
			Utils::BlitSprite(spriteHealthPlayers[i][j], spriteHealthPlayers[i][j].getPosition(), _gameData.window, 0);
		}
		
		for (int j = 0; j < spriteManaPlayers[i].size(); j++)
		{
			Utils::BlitSprite(spriteManaPlayers[i][j], spriteManaPlayers[i][j].getPosition(), _gameData.window, 0);
		}

		//display mana recovery if player is not full mana
		if (playAnimManaRecovery[i])
		{
			Utils::BlitSprite(spritePlayersManaRecovery[i], spritePlayersManaRecovery[i].getPosition(), _gameData.window, 0);
		}
		
		if (!_gameData.arenaIsChose)
		{
		    Utils::BlitSprite(spriteXpBarBackgroundPlayers[i], spriteXpBarBackgroundPlayers[i].getPosition(), _gameData.window, 0);
		    int xpBarSize = _gameData.tabPlayers[i]->GetXp() * 100 / _gameData.tabPlayers[i]->GetMaxXp();
		    for (int j = 0; j < xpBarSize; j++)
		    {
		        Utils::BlitSprite(spriteXpBarPlayers[i][j], spriteXpBarPlayers[i][j].getPosition(), _gameData.window, 0);
		    }
		    Utils::BlitSprite(spriteXpBarOverPlayers[i], spriteXpBarOverPlayers[i].getPosition(), _gameData.window);
		    _gameData.window.draw(lvlText[i]);
			_gameData.window.draw(spritePlayersInventory[i]);
		}
	}
}

void HUD::Display(sf::RenderTexture& _red, sf::RenderTexture& _blue,std::vector<Player*>& _tabPlayer,bool _arena)
{
	for (int i = 0; i < spriteIconsPlayers.size(); i++)
	{
		Utils::BlitSprite(spriteIconsPlayers[i], sf::Vector2f{ spriteIconsPlayers[i].getPosition().x + (TROAD_GAP * (-1 * spriteIconsPlayers[i].getPosition().y + WORKING_HEIGHT) / 100.f),spriteIconsPlayers[i].getPosition().y }, _red, 0);
		Utils::BlitSprite(spriteIconsPlayers[i], sf::Vector2f{ spriteIconsPlayers[i].getPosition().x - (TROAD_GAP * (-1 * spriteIconsPlayers[i].getPosition().y + WORKING_HEIGHT) / 100.f),spriteIconsPlayers[i].getPosition().y }, _blue, 0);

		Utils::BlitSprite(spriteFramePlayers[i], sf::Vector2f{ spriteFramePlayers[i].getPosition().x + (TROAD_GAP * (-1 * spriteFramePlayers[i].getPosition().y + WORKING_HEIGHT) / 100.f),spriteFramePlayers[i].getPosition().y }, _red, 0);
		Utils::BlitSprite(spriteFramePlayers[i], sf::Vector2f{ spriteFramePlayers[i].getPosition().x - (TROAD_GAP * (-1 * spriteFramePlayers[i].getPosition().y + WORKING_HEIGHT) / 100.f),spriteFramePlayers[i].getPosition().y }, _blue, 0);

		for (int j = 0; j < spriteHealthPlayers[i].size(); j++)
		{
			Utils::BlitSprite(spriteHealthPlayers[i][j], sf::Vector2f{ spriteHealthPlayers[i][j].getPosition().x + (TROAD_GAP * (-1 * spriteHealthPlayers[i][j].getPosition().y + WORKING_HEIGHT)/ 100.f),spriteHealthPlayers[i][j].getPosition().y }, _red, 0);
			Utils::BlitSprite(spriteHealthPlayers[i][j], sf::Vector2f{ spriteHealthPlayers[i][j].getPosition().x - (TROAD_GAP * (-1 * spriteHealthPlayers[i][j].getPosition().y + WORKING_HEIGHT)/ 100.f),spriteHealthPlayers[i][j].getPosition().y }, _blue, 0);
		}

		for (int j = 0; j < _tabPlayer[i]->GetMaxMana(); j++)
		{
			Utils::BlitSprite(spriteManaPlayers[i][j], sf::Vector2f{ spriteManaPlayers[i][j].getPosition().x + (TROAD_GAP * (-1 *  spriteManaPlayers[i][j].getPosition().y + WORKING_HEIGHT)/ 100.f),spriteManaPlayers[i][j].getPosition().y }, _red, 0);
			Utils::BlitSprite(spriteManaPlayers[i][j], sf::Vector2f{ spriteManaPlayers[i][j].getPosition().x - (TROAD_GAP * (-1 *  spriteManaPlayers[i][j].getPosition().y + WORKING_HEIGHT)/ 100.f),spriteManaPlayers[i][j].getPosition().y }, _blue, 0);
		}

		//display mana recovery if player is not full mana
		if (playAnimManaRecovery[i])
		{
			Utils::BlitSprite(spritePlayersManaRecovery[i], sf::Vector2f{ spritePlayersManaRecovery[i].getPosition().x + (TROAD_GAP * (-1 * spritePlayersManaRecovery[i].getPosition().y + WORKING_HEIGHT)/ 100.f),spritePlayersManaRecovery[i].getPosition().y }, _red, 0);
			Utils::BlitSprite(spritePlayersManaRecovery[i], sf::Vector2f{ spritePlayersManaRecovery[i].getPosition().x - (TROAD_GAP * (-1 * spritePlayersManaRecovery[i].getPosition().y + WORKING_HEIGHT)/ 100.f),spritePlayersManaRecovery[i].getPosition().y }, _blue, 0);
		}

		if (!_arena)
		{
			Utils::BlitSprite(spriteXpBarBackgroundPlayers[i], sf::Vector2f{ spriteXpBarBackgroundPlayers[i].getPosition().x + (TROAD_GAP * (-1 * spriteXpBarBackgroundPlayers[i].getPosition().y + WORKING_HEIGHT) / 100.f),spriteXpBarBackgroundPlayers[i].getPosition().y }, _red, 0);
			Utils::BlitSprite(spriteXpBarBackgroundPlayers[i], sf::Vector2f{ spriteXpBarBackgroundPlayers[i].getPosition().x - (TROAD_GAP * (-1 * spriteXpBarBackgroundPlayers[i].getPosition().y + WORKING_HEIGHT) / 100.f),spriteXpBarBackgroundPlayers[i].getPosition().y }, _blue, 0);

			int xpBarSize = _tabPlayer[i]->GetXp() * 100 / _tabPlayer[i]->GetMaxXp();
			for (int j = 0; j < xpBarSize; j++)
			{
				Utils::BlitSprite(spriteXpBarPlayers[i][j], sf::Vector2f{ spriteXpBarPlayers[i][j].getPosition().x + (TROAD_GAP * (-1 * spriteXpBarPlayers[i][j].getPosition().y + WORKING_HEIGHT) / 100.f),spriteXpBarPlayers[i][j].getPosition().y }, _red, 0);
				Utils::BlitSprite(spriteXpBarPlayers[i][j], sf::Vector2f{ spriteXpBarPlayers[i][j].getPosition().x - (TROAD_GAP * (-1 * spriteXpBarPlayers[i][j].getPosition().y + WORKING_HEIGHT) / 100.f),spriteXpBarPlayers[i][j].getPosition().y }, _blue, 0);
			}
			Utils::BlitSprite(spriteXpBarOverPlayers[i], sf::Vector2f{ spriteXpBarOverPlayers[i].getPosition().x + (TROAD_GAP * (-1 * spriteXpBarOverPlayers[i].getPosition().y + WORKING_HEIGHT) / 100.f),spriteXpBarOverPlayers[i].getPosition().y }, _red, 0);
			Utils::BlitSprite(spriteXpBarOverPlayers[i], sf::Vector2f{ spriteXpBarOverPlayers[i].getPosition().x - (TROAD_GAP * (-1 * spriteXpBarOverPlayers[i].getPosition().y + WORKING_HEIGHT) / 100.f),spriteXpBarOverPlayers[i].getPosition().y }, _blue, 0);
			
			sf::Vector2f posText = lvlText[i].getPosition();
			
			lvlText[i].setPosition(posText.x + (TROAD_GAP * (-1 * posText.y + WORKING_HEIGHT) / 100.f), posText.y);
			_red.draw(lvlText[i]);

			lvlText[i].setPosition(posText.x - (TROAD_GAP * (-1 * posText.y + WORKING_HEIGHT) / 100.f), posText.y);
			_blue.draw(lvlText[i]);
			
			lvlText[i].setPosition(posText);

			Utils::BlitSprite(spritePlayersInventory[i], sf::Vector2f{ spritePlayersInventory[i].getPosition().x + (TROAD_GAP * (-1 * spritePlayersInventory[i].getPosition().y + WORKING_HEIGHT) / 100.f),spritePlayersInventory[i].getPosition().y }, _red, 0);
			Utils::BlitSprite(spritePlayersInventory[i], sf::Vector2f{ spritePlayersInventory[i].getPosition().x - (TROAD_GAP * (-1 * spritePlayersInventory[i].getPosition().y + WORKING_HEIGHT) / 100.f),spritePlayersInventory[i].getPosition().y }, _blue, 0);
		}
	}
}

void HUD::DisplayMigrant(GameData& _gameData)
{
	nbPlayersDead = 0;
	for (int i = 0; i < spritePlayersCanBeRevived.size(); i++)
	{
		if (_gameData.tabPlayers[i]->GetState() == DEAD && nbPlayersDead < _gameData.tabPlayers.size() - 1)
		{
			nbPlayersDead++;
			Utils::BlitSprite(spritePlayersCanBeRevived[i], spritePlayersCanBeRevived[i].getPosition(), _gameData.window);
		}
	}
}

void HUD::DisplayMigrant3D(GameData& _gameData)
{
	nbPlayersDead = 0;
	for (int i = 0; i < spritePlayersCanBeRevived.size(); i++)
	{
		if (_gameData.tabPlayers[i]->GetState() == DEAD && nbPlayersDead < _gameData.tabPlayers.size() - 1)
		{
			nbPlayersDead++;
			Utils::BlitSprite(spritePlayersCanBeRevived[i], sf::Vector2f{ spritePlayersCanBeRevived[i].getPosition().x + (TROAD_GAP * (-1 * spritePlayersCanBeRevived[i].getPosition().y + WORKING_HEIGHT) / 100.f),spritePlayersCanBeRevived[i].getPosition().y }, _gameData.renderTextureRed);
			Utils::BlitSprite(spritePlayersCanBeRevived[i], sf::Vector2f{ spritePlayersCanBeRevived[i].getPosition().x - (TROAD_GAP * (-1 * spritePlayersCanBeRevived[i].getPosition().y + WORKING_HEIGHT) / 100.f),spritePlayersCanBeRevived[i].getPosition().y }, _gameData.renderTextureBlue);
		}
	}
}

sf::Vector2f HUD::GetPosIcon(int _id)
{
	return lvlText[_id].getPosition();
}