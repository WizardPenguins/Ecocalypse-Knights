#include "stdafx.h"
#include "CharacterSelection.h"
#include "Animation.h"
#include "Audio.h"
#include "Menu.h"
#include "Easings.h"
#include "Camera.h"
#include "HUD.h"
#include "Network.h"

CharacterSelection::CharacterSelection(GameData& _gameData)
{
	InitComponents(_gameData);
}

CharacterSelection::~CharacterSelection()
{
}

void CharacterSelection::LoadSprites()
{
	spriteBackground = Utils::LoadSprite("Assets/Images/BackGround/TitleScreen.png", TOP_LEFT);
	spriteMask = Utils::LoadSprite("Assets/Images/CharacterSelection/TitleScreenMask.png", TOP_LEFT);
	manetteEncule = Utils::LoadSprite("Assets/Images/CharacterSelection/NoController.png", TOP_LEFT);
	shadowFountain = Utils::LoadSprite("Assets/Images/Background/ShadowFountain.png", TOP_LEFT);

	
	//Load all the sprites for skins of the players
	allPlayerSkinsSprites.resize(NB_SPRITES_SKINS);
	allPlayerSkinsSprites[(int)SpritesPlayersSkin::RED] = Utils::LoadSprite("Assets/Images/CharacterSelection/FireChara.png", TOP_LEFT);
	allPlayerSkinsSprites[(int)SpritesPlayersSkin::GREEN] = Utils::LoadSprite("Assets/Images/CharacterSelection/PoisonChara.png", TOP_LEFT);
	allPlayerSkinsSprites[(int)SpritesPlayersSkin::BLUE] = Utils::LoadSprite("Assets/Images/CharacterSelection/IceChara.png", TOP_LEFT);
	allPlayerSkinsSprites[(int)SpritesPlayersSkin::YELLOW] = Utils::LoadSprite("Assets/Images/CharacterSelection/LightningChara.png", TOP_LEFT);

	//Load all the sprites for the frame of the players
	allPlayerFrameSprites.resize(NB_SPRITES_SKINS);
	allPlayerFrameSprites[(int)SpritesPlayersSkin::RED] = Utils::LoadSprite("Assets/Images/CharacterSelection/FireCharacter.png", TOP_LEFT);
	allPlayerFrameSprites[(int)SpritesPlayersSkin::GREEN] = Utils::LoadSprite("Assets/Images/CharacterSelection/PoisonCharacter.png", TOP_LEFT);
	allPlayerFrameSprites[(int)SpritesPlayersSkin::BLUE] = Utils::LoadSprite("Assets/Images/CharacterSelection/IceCharacter.png", TOP_LEFT);
	allPlayerFrameSprites[(int)SpritesPlayersSkin::YELLOW] = Utils::LoadSprite("Assets/Images/CharacterSelection/LightningCharacter.png", TOP_LEFT);

	//Load all the sprites for the lock of the players
	allPlayerLockSkins.resize(4);
	allPlayerLockSkins[0] = Utils::LoadSprite("Assets/Images/CharacterSelection/AButton.png", TOP_LEFT);
	allPlayerLockSkins[1] = Utils::LoadSprite("Assets/Images/CharacterSelection/AButtonValidate.png", TOP_LEFT);
	allPlayerLockSkins[2] = Utils::LoadSprite("Assets/Images/CharacterSelection/CrossButton.png", TOP_LEFT);
	allPlayerLockSkins[3] = Utils::LoadSprite("Assets/Images/CharacterSelection/CrossButtonValidate.png", TOP_LEFT);

	//Load all the sprites for the team choice of the players
	allPlayerTeamChoiceSprites.resize(2);
	allPlayerTeamChoiceSprites[0] = Utils::LoadSprite("Assets/Images/CharacterSelection/Team1.png", TOP_LEFT);
	allPlayerTeamChoiceSprites[1] = Utils::LoadSprite("Assets/Images/CharacterSelection/Team2.png", TOP_LEFT);

	//Load all the sprites for the choice of the number of rounds
	allPlayerRoundChoiceSprites.resize(3);
	allPlayerRoundChoiceSprites[0] = Utils::LoadSprite("Assets/Images/CharacterSelection/Ft5.png", TOP_LEFT);
	allPlayerRoundChoiceSprites[1] = Utils::LoadSprite("Assets/Images/CharacterSelection/Ft10.png", TOP_LEFT);
	allPlayerRoundChoiceSprites[2] = Utils::LoadSprite("Assets/Images/CharacterSelection/Infinite.png", TOP_LEFT);
	//Load the background of the frame
	playersBackgroundFrame = Utils::LoadSprite("Assets/Images/CharacterSelection/BackgroundCharacter.png", TOP_LEFT);

	//Fountain
	fountain = Utils::LoadSprite("Assets/Images/Background/Anim/Fountain.png", TOP_LEFT);

	animFountain = new Animation;
	sf::IntRect rect = sf::IntRect(0, 0, 612 / 3, 337);
	fountain.setTextureRect(rect);
	animFountain->CreateLeftToRight(fountain, 3, 1, { 612,337 });

	fountain.setPosition(955, 1980);
	shadowFountain.setPosition(955, 2270);
}

void CharacterSelection::InitSprites()
{
	playerSkins.resize(4);
	for (int i = 0; i < playerSkins.size(); i++)
	{
		playerSkins[i].isReady = false;
		playerSkins[i].teamSelected = false;
		playerSkins[i].teamChoice = 0;
		playerSkins[i].slidingUp = false;
		playerSkins[i].slidingDown = false;
		playerSkins[i].tabSpriteNumberPlayer.resize(4);
		allPlayerSkinsSprites[i].setPosition({ (float)((WORKING_WIDTH / 5 * i) + allPlayerSkinsSprites[i].getGlobalBounds().width / 1.15f), (float)(WORKING_HEIGHT / 2 - allPlayerSkinsSprites[i].getGlobalBounds().height / 2) });
		allPlayerFrameSprites[i].setPosition({ (float)((WORKING_WIDTH / 5 * i) + allPlayerFrameSprites[i].getGlobalBounds().width / 1.15f), 430.f + HEIGHT_GAP });

		playerSkins[i].spriteSkinPlayer = allPlayerSkinsSprites[(int)SpritesPlayersSkin::RED];
		playerSkins[i].spriteFramePlayer = allPlayerFrameSprites[(int)SpritesPlayersSkin::RED];
		playerSkins[i].spriteFramePlayer.setPosition({ (float)((WORKING_WIDTH / 5 * i) + playerSkins[i].spriteFramePlayer.getGlobalBounds().width / 1.15f), MIDDLE_HEIGHT + HEIGHT_GAP });
		if (sf::Joystick::isConnected(i) && sf::Joystick::getIdentification(GetFirstController(i)).name == "Wireless Controller")
		{
			playerSkins[i].spriteLockSkin = allPlayerLockSkins[2];
			playerSkins[i].spriteLockTeam = allPlayerLockSkins[2];
		}
		else
		{
			playerSkins[i].spriteLockSkin = allPlayerLockSkins[0];
			playerSkins[i].spriteLockTeam = allPlayerLockSkins[0];
		}
		playerSkins[i].spriteLockSkin.setPosition({ playerSkins[i].spriteFramePlayer.getPosition().x + playerSkins[i].spriteFramePlayer.getGlobalBounds().width - playerSkins[i].spriteLockSkin.getGlobalBounds().width / 2, playerSkins[i].spriteFramePlayer.getPosition().y - playerSkins[i].spriteLockSkin.getGlobalBounds().height / 2 });
		//load background sprite of the frame
		playerSkins[i].spriteBackgroundFrame = playersBackgroundFrame;
		playerSkins[i].spriteBackgroundFrame.setPosition(playerSkins[i].spriteFramePlayer.getPosition().x + 5.f, playerSkins[i].spriteFramePlayer.getPosition().y);
		playerSkins[i].posSpriteSkinPlayer = { (float)((WORKING_WIDTH / 5 * i) + playerSkins[i].spriteFramePlayer.getGlobalBounds().width / 1.15f), MIDDLE_HEIGHT + HEIGHT_GAP };
		playerSkins[i].nextSkinSelected = playerSkins[i].spriteSkinPlayer;
		playerSkins[i].posNextSkinSelected = playerSkins[i].posSpriteSkinPlayer;
		playerSkins[i].spriteTeamChoice = allPlayerTeamChoiceSprites[0];
		playerSkins[i].spriteTeamChoice.setPosition({ playerSkins[i].spriteFramePlayer.getPosition().x, playerSkins[i].spriteFramePlayer.getPosition().y + playerSkins[i].spriteFramePlayer.getGlobalBounds().height + playerSkins[i].spriteTeamChoice.getGlobalBounds().height / 2 });
		playerSkins[i].timerChoiceTeam = 0.f;
	}
	choiceRound = 0;
	spriteRoundNumberChoice = allPlayerRoundChoiceSprites[0];
	roundNumberIsSelected = false;
	if (sf::Joystick::isConnected(0) && sf::Joystick::getIdentification(0).name == "Wireless Controller")
	{
		spriteLockRoundNumber = allPlayerLockSkins[2];
	}
	else
	{
		spriteLockRoundNumber = allPlayerLockSkins[0];
	}
}

void CharacterSelection::InitNumber()
{
	for (int i = 0; i < playerSkins.size(); i++)
	{
		for (int j = 0; j < playerSkins[i].tabSpriteNumberPlayer.size(); j++)
		{
			playerSkins[i].tabSpriteNumberPlayer[j].setPosition({ (float)(playerSkins[i].spriteFramePlayer.getPosition().x + playerSkins[i].spriteFramePlayer.getGlobalBounds().width / 2 - playerSkins[i].tabSpriteNumberPlayer[j].getGlobalBounds().width / 4 - 5.f), (float)(playerSkins[i].spriteFramePlayer.getPosition().y + 61.f - playerSkins[i].tabSpriteNumberPlayer[j].getGlobalBounds().height / 2) });
		}
		playerSkins[i].spriteNumberPlayerCurrent = playerSkins[i].tabSpriteNumberPlayer[(int)SpritesPlayersSkin::RED];
	}
}

void CharacterSelection::InitComponents(GameData& _gameData)
{
	LoadSprites();

	InitSprites();

	LoadPlayerSkinSpriteNumber();

	InitNumber();
}

//Load all the sprites for the number of the players
void CharacterSelection::LoadPlayerSkinSpriteNumber()
{
	for (int i = 0; i < playerSkins.size(); i++)
	{
		playerSkins[i].tabSpriteNumberPlayer[(int)SpritesPlayersSkin::RED] = Utils::LoadSprite("Assets/Images/CharacterSelection/" + std::to_string(i + 1) + "P.png", TOP_LEFT);
		playerSkins[i].tabSpriteNumberPlayer[(int)SpritesPlayersSkin::GREEN] = Utils::LoadSprite("Assets/Images/CharacterSelection/" + std::to_string(i + 1) + "P.png", TOP_LEFT);
		playerSkins[i].tabSpriteNumberPlayer[(int)SpritesPlayersSkin::BLUE] = Utils::LoadSprite("Assets/Images/CharacterSelection/" + std::to_string(i + 1) + "P.png", TOP_LEFT);
		playerSkins[i].tabSpriteNumberPlayer[(int)SpritesPlayersSkin::YELLOW] = Utils::LoadSprite("Assets/Images/CharacterSelection/" + std::to_string(i + 1) + "P.png", TOP_LEFT);
	}
}

void CharacterSelection::Reload(GameData& _gameData)
{
	//Reload variables
	_gameData.compteurPlayer = 0;
	nbPlayerReady = 0;
	nbPlayerTeamSelected = 0;
	waitTime = 0.f;
	_gameData.playerSelected.clear();
	_gameData.playerTeam.clear();

	InitSprites();

	InitNumber();

	ReloadAlphaSprite(_gameData.tabPlayers);
}

void CharacterSelection::ReloadAlphaSprite(std::vector<Player*>& _tabPlayers)
{
	sf::Color tempColor = { 255, 255, 255, 255 };
	for (int i = 0; i < _tabPlayers.size(); i++)
	{		
		playerSkins[i].spriteSkinPlayer.setColor(tempColor);
		playerSkins[i].nextSkinSelected.setColor(tempColor);
		playerSkins[i].spriteBackgroundFrame.setColor(tempColor);
		playerSkins[i].spriteFramePlayer.setColor(tempColor);
		playerSkins[i].spriteNumberPlayerCurrent.setColor(tempColor);
		playerSkins[i].spriteLockSkin.setColor(tempColor);
		playerSkins[i].spriteTeamChoice.setColor(tempColor);
		playerSkins[i].spriteLockTeam.setColor(tempColor);
		spriteMask.setColor(tempColor);
	}
	spriteRoundNumberChoice.setColor(tempColor);
	spriteLockRoundNumber.setColor(tempColor);
}
void UpdateCameraBITE(GameData& _gameData)
{
	_gameData.camera->UpdateMenu(_gameData);
}

void CharacterSelection::StartGame(GameData& _gameData)
{
	if (!_gameData.arenaIsChose)
	{
		StartStoryMode(_gameData);
	}
	else
	{
		if (!_gameData.teamfight)
		{
			StartArenaMode(_gameData);
		}
		else
		{
			StartArenaModeTeamfight(_gameData);
		}
	}
}

//Start the game in story mode
void CharacterSelection::StartStoryMode(GameData& _gameData)
{
	if (nbPlayerReady == _gameData.tabPlayers.size() && _gameData.tabPlayers.size() > 0)
	{
		//use waitTime to wait for 1 second before going to the game
		if (waitTime < 0.5f)
		{
			waitTime += _gameData.dt;
		}
		else
		{

			//give the good skin to the player
			for (int i = 0; i < _gameData.tabPlayers.size(); i++)
			{
				_gameData.playerSelected.push_back(playerSkins[i].currentSkin);
			}

			_gameData.hud = new HUD;
			_gameData.hud->InitHud(_gameData.tabPlayers, _gameData.playerSelected, _gameData.window);
			_gameData.camera->SetMoveMenuToPlayerSelection(true);

			nbPlayerReady = 0;
			waitTime = 0.f;
		}
	}

	if (_gameData.camera->GetView().getCenter().y >= 2114.f)
	{
		if (_gameData.multiplayer && _gameData.network->state == NetworkState::HOSTWAIT)
		{
			_gameData.ChangeState(GAME);
			_gameData.network->state = NetworkState::HOST;
			_gameData.camera->lastCenter = { WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f };
			_gameData.camera->moveMenuToPlayerSelection = false;
			_gameData.network->replacePlayer = true;
		}
		else if (!_gameData.multiplayer)
		{
			_gameData.ChangeState(GAME);
			_gameData.camera->lastCenter = { WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f };
			_gameData.camera->moveMenuToPlayerSelection = false;
		}
	}
}

//Start the game in arena mode with the teamfight mode
void CharacterSelection::StartArenaModeTeamfight(GameData& _gameData)
{
	//detecter le nombre de joueur par team
	int nbPlayerTeam0 = 0;
	int nbPlayerTeam1 = 0;
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (playerSkins[i].teamChoice == 0 && playerSkins[i].teamSelected)
		{
			nbPlayerTeam0++;
		}
		else if (playerSkins[i].teamChoice == 1 && playerSkins[i].teamSelected)
		{
			nbPlayerTeam1++;
		}
	}

	if (nbPlayerTeamSelected == _gameData.tabPlayers.size() && _gameData.tabPlayers.size() > 1 && nbPlayerTeam0 != 0 && nbPlayerTeam1 != 0)
	{
		//use waitTime to wait for 1 second before going to the game
		if (waitTime < 0.5f)
		{
			waitTime += _gameData.dt;
		}
		else
		{
			//give the good skin to the player
			for (int i = 0; i < _gameData.tabPlayers.size(); i++)
			{
				_gameData.playerSelected.push_back(playerSkins[i].currentSkin);
				_gameData.playerTeam.push_back(playerSkins[i].teamChoice);
			}

			_gameData.hud = new HUD;
			_gameData.hud->InitHud(_gameData.tabPlayers, _gameData.playerSelected, _gameData.window);
			_gameData.camera->SetMoveMenuToPlayerSelection(true);

			nbPlayerReady = 0;
			nbPlayerTeamSelected = 0;
			waitTime = 0.f;
		}
	}

	if (_gameData.camera->GetView().getCenter().y >= 2114.f || _gameData.gameState == ARENA)
	{
		_gameData.ChangeState(ARENA);
		_gameData.camera->lastCenter = { WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f };
		_gameData.camera->moveMenuToPlayerSelection = false;
	}
}

//Start the game in arena mode with the free for all mode
void CharacterSelection::StartArenaMode(GameData& _gameData)
{
	if (nbPlayerReady == _gameData.tabPlayers.size() && _gameData.tabPlayers.size() > 1 && roundNumberIsSelected)
	{
		//use waitTime to wait for 1 second before going to the game
		if (waitTime < 0.5f)
		{
			waitTime += _gameData.dt;
		}
		else
		{
			//give the good skin to the player
			for (int i = 0; i < _gameData.tabPlayers.size(); i++)
			{
				_gameData.playerSelected.push_back(playerSkins[i].currentSkin);
			}

			_gameData.choasWin = (ChaosWin)choiceRound;
			
			_gameData.hud = new HUD;
			_gameData.hud->InitHud(_gameData.tabPlayers, _gameData.playerSelected, _gameData.window);
			_gameData.camera->SetMoveMenuToPlayerSelection(true);

			nbPlayerReady = 0;
			waitTime = 0.f;
		}
	}

	if (_gameData.camera->GetView().getCenter().y >= 2114.f || _gameData.gameState == ARENA)
	{
		_gameData.ChangeState(ARENA);
		_gameData.camera->lastCenter = { WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f };
		_gameData.camera->moveMenuToPlayerSelection = false;
	}
}

void CharacterSelection::Update(GameData& _gameData)
{
	if (_gameData.reloadCharacterSelection)
	{
		Reload(_gameData);
		_gameData.reloadCharacterSelection = false;
	}

	_gameData.audio->PlayMusic("MENU", true, 25);
	if (!_gameData.multiplayer && !_gameData.camera->moveMenuToPlayerSelection)
	{
		UpdateNbPlayers(_gameData);
	}

	UpdatePos(_gameData);
	UpdateCameraBITE(_gameData);
	UpdateAnim(_gameData);
	StartGame(_gameData);

	if (_gameData.multiplayer)
	{
		UpdateMulti(_gameData);
	}
}

//Create and erase players in function of the number of joysticks connected to the computer
void CharacterSelection::UpdateNbPlayers(GameData& _gameData)
{
	//get the number of joysticks connected to the computer
	joystickCount = sf::Joystick::isConnected(0) + sf::Joystick::isConnected(1) + sf::Joystick::isConnected(2) + sf::Joystick::isConnected(3);

	if (joystickCount > _gameData.tabPlayers.size() && _gameData.tabPlayers.size() < NB_MAX_PLAYER)
	{
		for (int i = 0; i < joystickCount - _gameData.tabPlayers.size(); i++)
		{
			Player* player = new Player(_gameData, _gameData.compteurPlayer);
			_gameData.compteurPlayer++;
			_gameData.tabPlayers.push_back(player);
			if (i == joystickCount - 1)
			{
				player->Init();
			}
		}
	}

	if (_gameData.tabPlayers.size() < NB_MAX_PLAYER && _gameData.tabPlayers.size() > joystickCount)
	{
		for (int i = 0; i < _gameData.tabPlayers.size() - joystickCount; i++)
		{
			_gameData.tabPlayers.erase(_gameData.tabPlayers.begin() + i);
		}
	}
}

void CharacterSelection::UpdateMulti(GameData& _gameData)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(_gameData.window);
	_gameData.network->WaitStateUpdate(mousePos, _gameData, &this->playerSkins);
	nbPlayerReady = 0;
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (this->playerSkins[i].isReady)
		{
			nbPlayerReady++;
			if (sf::Joystick::getIdentification(GetFirstController(0)).name == "Wireless Controller")
			{
				playerSkins[i].spriteLockSkin = allPlayerLockSkins[3];
			}
			else
			{
				playerSkins[i].spriteLockSkin = allPlayerLockSkins[1];
			}
		}
		else
		{
			if (sf::Joystick::getIdentification(GetFirstController(0)).name == "Wireless Controller")
			{
				playerSkins[i].spriteLockSkin = allPlayerLockSkins[2];
			}
			else
			{
				playerSkins[i].spriteLockSkin = allPlayerLockSkins[0];
			}
		}
		playerSkins[i].spriteLockSkin.setPosition({ playerSkins[i].spriteFramePlayer.getPosition().x + playerSkins[i].spriteFramePlayer.getGlobalBounds().width - playerSkins[i].spriteLockSkin.getGlobalBounds().width / 2, playerSkins[i].spriteFramePlayer.getPosition().y - playerSkins[i].spriteLockSkin.getGlobalBounds().height / 2 });
		if (!playerSkins[i].slidingUp && !playerSkins[i].slidingDown)
		{
			playerSkins[i].spriteSkinPlayer = playerSkins[i].nextSkinSelected;
			playerSkins[i].posNextSkinSelected = { playerSkins[i].posSpriteSkinPlayer.x, MIDDLE_HEIGHT + HEIGHT_GAP };
		}
	}
}

void CharacterSelection::UpdateAnim(GameData& _gameData)
{
	bool temp = true;
	animFountain->UpdateLeftToRight(fountain, _gameData.dt, 9, 0, temp, true);
}

int CharacterSelection::GetFirstController(int _i)
{
	for (int j = 0; j < 4; j++)
	{
		if (sf::Joystick::isConnected(j + _i))
		{
			return _i + j;
		}
	}
	return 0;
}

bool CharacterSelection::IsSkinAlreadySelected(int _i)
{
	for (int j = 0; j < playerSkins.size(); j++)
	{
		if (playerSkins[j].currentSkin == _i && playerSkins[j].isReady)
		{
			return true;
		}
	}
	return false;
}

void CharacterSelection::JoystickMovedMulti(float yPos1, int i)
{
	if (yPos1 < -15 || sf::Joystick::isAxisPressed(GetFirstController(0), sf::Joystick::Axis::DIR_CROSS_Y, sf::Joystick::PLUS)) //JOYSTICK VERS LE HAUT
	{
		playerSkins[i].posSpriteSkinPlayer = { playerSkins[i].posSpriteSkinPlayer.x, MIDDLE_HEIGHT + HEIGHT_GAP };
		if (playerSkins[i].currentSkin == NB_SPRITES_SKINS - 1)
		{
			playerSkins[i].currentSkin = 0;
			playerSkins[i].slidingUp = true;
		}
		else
		{
			playerSkins[i].currentSkin++;
			playerSkins[i].slidingUp = true;
		}
	}
	if (yPos1 > 15 || sf::Joystick::isAxisPressed(GetFirstController(0), sf::Joystick::Axis::DIR_CROSS_Y, sf::Joystick::MINUS)) //JOYSTICK VERS LE BAS 
	{
		playerSkins[i].posSpriteSkinPlayer = { playerSkins[i].posSpriteSkinPlayer.x, MIDDLE_HEIGHT + HEIGHT_GAP };
		if (playerSkins[i].currentSkin == 0)
		{
			playerSkins[i].currentSkin = NB_SPRITES_SKINS - 1;
			playerSkins[i].slidingDown = true;
		}
		else
		{
			playerSkins[i].currentSkin--;
			playerSkins[i].slidingDown = true;
		}
	}
}

void CharacterSelection::UpdateJoystickMove(int _i, float _dt, bool _arena, bool _teamfight)
{
	playerSkins[_i].spriteLockTeam.setPosition(playerSkins[_i].spriteTeamChoice.getPosition().x + playerSkins[_i].spriteTeamChoice.getGlobalBounds().width - playerSkins[_i].spriteLockSkin.getGlobalBounds().width, playerSkins[_i].spriteTeamChoice.getPosition().y - playerSkins[_i].spriteLockSkin.getGlobalBounds().height / 2);
	spriteLockRoundNumber.setPosition(spriteRoundNumberChoice.getPosition().x + spriteRoundNumberChoice.getGlobalBounds().width - spriteLockRoundNumber.getGlobalBounds().width, spriteRoundNumberChoice.getPosition().y - spriteLockRoundNumber.getGlobalBounds().height / 2);
	
	if (!playerSkins[_i].slidingUp && !playerSkins[_i].slidingDown && !playerSkins[_i].isReady)
	{
		playerSkins[_i].spriteSkinPlayer = playerSkins[_i].nextSkinSelected;
		playerSkins[_i].posSpriteSkinPlayer = { playerSkins[_i].posNextSkinSelected.x, MIDDLE_HEIGHT + HEIGHT_GAP };

		float yPos1 = sf::Joystick::getAxisPosition(GetFirstController(_i), sf::Joystick::Axis::LEFT_JOYSTICK_Y);
		if (yPos1 < -15 || sf::Joystick::isAxisPressed(GetFirstController(_i), sf::Joystick::Axis::DIR_CROSS_Y, sf::Joystick::PLUS)) //JOYSTICK ou CROIX VERS LE HAUT
		{
			if (playerSkins[_i].currentSkin == NB_SPRITES_SKINS - 1)
			{
				playerSkins[_i].currentSkin = 0;
				playerSkins[_i].slidingUp = true;
			}
			else
			{
				playerSkins[_i].currentSkin++;
				playerSkins[_i].slidingUp = true;
			}
		}
		if (yPos1 > 15 || sf::Joystick::isAxisPressed(GetFirstController(_i), sf::Joystick::Axis::DIR_CROSS_Y, sf::Joystick::MINUS)) //JOYSTICK ou CROIX VERS LE BAS 
		{
			if (playerSkins[_i].currentSkin == 0)
			{
				playerSkins[_i].currentSkin = NB_SPRITES_SKINS - 1;
				playerSkins[_i].slidingDown = true;
			}
			else
			{
				playerSkins[_i].currentSkin--;
				playerSkins[_i].slidingDown = true;
			}
		}
	}
	else if (!playerSkins[_i].slidingUp && !playerSkins[_i].slidingDown && playerSkins[_i].isReady && _arena && _teamfight && !playerSkins[_i].teamSelected)
	{
		float xPos1 = sf::Joystick::getAxisPosition(GetFirstController(_i), sf::Joystick::Axis::LEFT_JOYSTICK_X);
		if (xPos1 < -15 || sf::Joystick::isAxisPressed(GetFirstController(_i), sf::Joystick::Axis::DIR_CROSS_X, sf::Joystick::PLUS)) //JOYSTICK ou CROIX VERS LA GAUCHE
		{
			if (playerSkins[_i].timerChoiceTeam > 0)
			{
				playerSkins[_i].timerChoiceTeam -= _dt;
			}
			else
			{
				if (playerSkins[_i].teamChoice == 0)
				{
					playerSkins[_i].teamChoice = 1;
				}
				else
				{
					playerSkins[_i].teamChoice--;
				}
				playerSkins[_i].timerChoiceTeam = 0.2f;
			}
		}
		if (xPos1 > 15 || sf::Joystick::isAxisPressed(GetFirstController(_i), sf::Joystick::Axis::DIR_CROSS_X, sf::Joystick::MINUS)) //JOYSTICK ou CROIX VERS LA DROITE
		{
			if (playerSkins[_i].timerChoiceTeam > 0)
			{
				playerSkins[_i].timerChoiceTeam -= _dt;
			}
			else
			{
				if (playerSkins[_i].teamChoice == 1)
				{
					playerSkins[_i].teamChoice = 0;
				}
				else
				{
					playerSkins[_i].teamChoice++;
				}
				playerSkins[_i].timerChoiceTeam = 0.2f;
			}
		}
		playerSkins[_i].spriteTeamChoice = allPlayerTeamChoiceSprites[playerSkins[_i].teamChoice];
		playerSkins[_i].spriteTeamChoice.setPosition({ playerSkins[_i].spriteFramePlayer.getPosition().x, playerSkins[_i].spriteFramePlayer.getPosition().y + playerSkins[_i].spriteFramePlayer.getGlobalBounds().height + playerSkins[_i].spriteTeamChoice.getGlobalBounds().height / 2 });
	}
	else if (!playerSkins[_i].slidingUp && !playerSkins[_i].slidingDown && playerSkins[_i].isReady && _arena && !_teamfight && !roundNumberIsSelected)
	{
		if (_i == 0)
		{
			float xPos1 = sf::Joystick::getAxisPosition(GetFirstController(0), sf::Joystick::Axis::LEFT_JOYSTICK_X);
			if (xPos1 < -15 || sf::Joystick::isAxisPressed(GetFirstController(0), sf::Joystick::Axis::DIR_CROSS_X, sf::Joystick::PLUS)) //JOYSTICK ou CROIX VERS LA GAUCHE
			{
				if (timerChoiceNumberRounds > 0)
				{
					timerChoiceNumberRounds -= _dt;
				}
				else
				{
					if (choiceRound == 0)
					{
						choiceRound = 2;
					}
					else
					{
						choiceRound--;
					}
					timerChoiceNumberRounds = 0.2f;
				}
			}
			if (xPos1 > 15 || sf::Joystick::isAxisPressed(GetFirstController(0), sf::Joystick::Axis::DIR_CROSS_X, sf::Joystick::MINUS)) //JOYSTICK ou CROIX VERS LA DROITE
			{
				if (timerChoiceNumberRounds > 0)
				{
					timerChoiceNumberRounds -= _dt;
				}
				else
				{
					if (choiceRound == 2)
					{
						choiceRound = 0;
					}
					else
					{
						choiceRound++;
					}
					timerChoiceNumberRounds = 0.2f;
				}
			}
			spriteRoundNumberChoice = allPlayerRoundChoiceSprites[choiceRound];
			spriteRoundNumberChoice.setPosition({ WORKING_WIDTH / 2 - spriteRoundNumberChoice.getGlobalBounds().width/2, playerSkins[_i].spriteFramePlayer.getPosition().y + playerSkins[_i].spriteFramePlayer.getGlobalBounds().height + spriteRoundNumberChoice.getGlobalBounds().height / 2});
		}
	}
}

void CharacterSelection::UpdateEasing(int i, float _dt, bool _moveMenuToPlayerSelection, Audio& _audio)
{
	//init pos and color
	if (!playerSkins[i].timerSpriteSkinPlayer && !_moveMenuToPlayerSelection)
	{
		playerSkins[i].nextSkinSelected = allPlayerSkinsSprites[playerSkins[i].currentSkin];
		playerSkins[i].posNextSkinSelected = { playerSkins[i].posSpriteSkinPlayer.x, playerSkins[i].posSpriteSkinPlayer.y - playerSkins[i].spriteSkinPlayer.getGlobalBounds().height };
		playerSkins[i].spriteFramePlayer = allPlayerFrameSprites[playerSkins[i].currentSkin];
		playerSkins[i].spriteFramePlayer.setPosition({ (float)((WORKING_WIDTH / 5 * i) + playerSkins[i].spriteFramePlayer.getGlobalBounds().width / 1.15f), 430.f + HEIGHT_GAP });
		playerSkins[i].spriteNumberPlayerCurrent = playerSkins[i].tabSpriteNumberPlayer[playerSkins[i].currentSkin];
		playerSkins[i].spriteNumberPlayerCurrent.setPosition({ (float)(playerSkins[i].spriteFramePlayer.getPosition().x + playerSkins[i].spriteFramePlayer.getGlobalBounds().width / 2 - playerSkins[i].spriteNumberPlayerCurrent.getGlobalBounds().width / 4 - 5.f), (float)(playerSkins[i].spriteFramePlayer.getPosition().y + 61.f - playerSkins[i].spriteNumberPlayerCurrent.getGlobalBounds().height / 2) });
	}

	if (playerSkins[i].slidingUp) // stick up
	{
		//move the next skin and the current skin to make the effect of a sliding
		playerSkins[i].timerSpriteSkinPlayer += _dt * 1.5f;

	

		if (playerSkins[i].timerSpriteSkinPlayer < 1)
		{
			if (transitionSoundCanBePlayed)
			{
				_audio.PlaySoundNotRepeated("BUTTON_TRANSITION_CHARACTER_SELECTION");
				transitionSoundCanBePlayed = false;
			}
			playerSkins[i].posSpriteSkinPlayer = Easings::Lerp({ (float)((WORKING_WIDTH / 5 * i) + playerSkins[i].spriteFramePlayer.getGlobalBounds().width / 1.15f), MIDDLE_HEIGHT + HEIGHT_GAP }, { (float)((WORKING_WIDTH / 5 * i) + playerSkins[i].spriteFramePlayer.getGlobalBounds().width / 1.15f + 20.f), TOP_HEIGHT + HEIGHT_GAP }, playerSkins[i].timerSpriteSkinPlayer);
			playerSkins[i].posNextSkinSelected = Easings::Lerp({ (float)((WORKING_WIDTH / 5 * i) + playerSkins[i].spriteFramePlayer.getGlobalBounds().width / 1.15f - 20.f), BOTTOM_HEIGHT + HEIGHT_GAP }, { (float)((WORKING_WIDTH / 5 * i) + playerSkins[i].spriteFramePlayer.getGlobalBounds().width / 1.15f), MIDDLE_HEIGHT + HEIGHT_GAP }, playerSkins[i].timerSpriteSkinPlayer);
		}
		else //end of easing
		{
			playerSkins[i].slidingUp = false;
			playerSkins[i].timerSpriteSkinPlayer = 0;
			transitionSoundCanBePlayed = true;
		}
	}
	else if (playerSkins[i].slidingDown) //stick down
	{
		playerSkins[i].timerSpriteSkinPlayer += _dt * 1.5f;

		if (playerSkins[i].timerSpriteSkinPlayer < 1)
		{
			if (transitionSoundCanBePlayed)
			{
				_audio.PlaySoundNotRepeated("BUTTON_TRANSITION_CHARACTER_SELECTION");
				transitionSoundCanBePlayed = false;
			}
			playerSkins[i].posSpriteSkinPlayer = Easings::Lerp({ (float)((WORKING_WIDTH / 5 * i) + playerSkins[i].spriteFramePlayer.getGlobalBounds().width / 1.15f), MIDDLE_HEIGHT + HEIGHT_GAP }, { (float)((WORKING_WIDTH / 5 * i) + playerSkins[i].spriteFramePlayer.getGlobalBounds().width / 1.15f - 20.f), BOTTOM_HEIGHT + HEIGHT_GAP }, playerSkins[i].timerSpriteSkinPlayer);
			playerSkins[i].posNextSkinSelected = Easings::Lerp({ (float)((WORKING_WIDTH / 5 * i) + playerSkins[i].spriteFramePlayer.getGlobalBounds().width / 1.15f + 20.f), TOP_HEIGHT + HEIGHT_GAP }, { (float)((WORKING_WIDTH / 5 * i) + playerSkins[i].spriteFramePlayer.getGlobalBounds().width / 1.15f), MIDDLE_HEIGHT + HEIGHT_GAP }, playerSkins[i].timerSpriteSkinPlayer);
		}
		else //end of easing
		{
			playerSkins[i].slidingDown = false;
			playerSkins[i].timerSpriteSkinPlayer = 0;
			transitionSoundCanBePlayed = true;
		}
	}
}

void CharacterSelection::UpdateJoystickMoveNetwork(int _i, Network& _network)
{
	int i = 0;
	//si pas le server alors on récupère l'id du client pour changer son perso
	if (_network.state != NetworkState::HOSTWAIT)
	{
		i = _network.client->id;
	}

	if (!playerSkins[i].slidingUp && !playerSkins[i].slidingDown && !playerSkins[i].isReady)
	{
		playerSkins[i].spriteSkinPlayer = playerSkins[i].nextSkinSelected;
		playerSkins[i].posSpriteSkinPlayer = { playerSkins[i].posSpriteSkinPlayer.x, MIDDLE_HEIGHT + HEIGHT_GAP };
		float yPos1 = sf::Joystick::getAxisPosition(GetFirstController(0), sf::Joystick::Axis::LEFT_JOYSTICK_Y);
		JoystickMovedMulti(yPos1, i);
	}
}

void CharacterSelection::UpdatePos(GameData& _gameData)
{
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (sf::Joystick::isConnected(i) && !playerSkins[i].isReady)
		{
			if (sf::Joystick::getIdentification(GetFirstController(i)).name == "Wireless Controller")
			{
				playerSkins[i].spriteLockSkin = allPlayerLockSkins[2];
				playerSkins[i].spriteLockTeam = allPlayerLockSkins[2];
			}
			else
			{
				playerSkins[i].spriteLockSkin = allPlayerLockSkins[0];
				playerSkins[i].spriteLockTeam = allPlayerLockSkins[0];
			}
		}
		if(!roundNumberIsSelected)
		{ 
			if (sf::Joystick::getIdentification(GetFirstController(0)).name == "Wireless Controller")
			{
				spriteLockRoundNumber = allPlayerLockSkins[2];
			}
			else
			{
				spriteLockRoundNumber = allPlayerLockSkins[0];
			}
		}
		
		playerSkins[i].spriteLockSkin.setPosition({ playerSkins[i].spriteFramePlayer.getPosition().x + playerSkins[i].spriteFramePlayer.getGlobalBounds().width - playerSkins[i].spriteLockSkin.getGlobalBounds().width / 2, playerSkins[i].spriteFramePlayer.getPosition().y - playerSkins[i].spriteLockSkin.getGlobalBounds().height / 2 });

		if (!_gameData.multiplayer)
		{
			UpdateJoystickMove(i, _gameData.dt, _gameData.arenaIsChose, _gameData.teamfight);
		}
		else
		{
			UpdateJoystickMoveNetwork(i, *_gameData.network);
		}

		//easing
		UpdateEasing(i, _gameData.dt, _gameData.camera->moveMenuToPlayerSelection, *_gameData.audio);
	}
	
}

//Display fade when go to game
void CharacterSelection::FadeGoToGame(bool _moveMenuToPlayerSelection, int _i, float _dt, bool _teamfight, bool _arena, bool _multiplayer)
{
	if (_moveMenuToPlayerSelection && playerSkins[_i].spriteLockSkin.getColor().a && playerSkins[_i].spriteSkinPlayer.getColor().a > 0)
	{
		int alpha = playerSkins[_i].spriteSkinPlayer.getColor().a;
		if (alpha - _dt * 100 <= 0)
		{
			alpha = 0;
		}
		else
		{
			alpha -= _dt * 100;
		}
		playerSkins[_i].spriteSkinPlayer.setColor(sf::Color{ 255,255,255, (sf::Uint8)alpha });//(sf::Uint8)(playerSkins[_i].spriteSkinPlayer.getColor().a - _dt * 100) });
		playerSkins[_i].nextSkinSelected.setColor(sf::Color{ 255,255,255, (sf::Uint8)alpha });// (sf::Uint8)(playerSkins[_i].spriteSkinPlayer.getColor().a - _dt * 100) });
		playerSkins[_i].spriteBackgroundFrame.setColor(sf::Color{ 255,255,255,(sf::Uint8)alpha });// (sf::Uint8)(playerSkins[_i].spriteBackgroundFrame.getColor().a - _dt * 100) });
		playerSkins[_i].spriteFramePlayer.setColor(sf::Color{ 255,255,255,(sf::Uint8)alpha });// (sf::Uint8)(playerSkins[_i].spriteFramePlayer.getColor().a - _dt * 100) });
		playerSkins[_i].spriteNumberPlayerCurrent.setColor(sf::Color{ 255,255,255,(sf::Uint8)alpha });// (sf::Uint8)(playerSkins[_i].spriteNumberPlayerCurrent.getColor().a - _dt * 100) });
		if (_multiplayer)
		{
			playerSkins[_i].spriteLockSkin.setColor(sf::Color{ 255, 255, 255, 0 });
		}
		else
		{
			playerSkins[_i].spriteLockSkin.setColor(sf::Color{ 255,255,255,(sf::Uint8)alpha });// (sf::Uint8)(playerSkins[_i].spriteLockSkin.getColor().a - _dt * 100) });
		}
		if (_teamfight && _arena)
		{
			playerSkins[_i].spriteTeamChoice.setColor(sf::Color{ 255,255,255,(sf::Uint8)alpha });// (sf::Uint8)(playerSkins[_i].spriteTeamChoice.getColor().a - _dt * 100) });
			playerSkins[_i].spriteLockTeam.setColor(sf::Color{ 255,255,255,(sf::Uint8)alpha });// (sf::Uint8)(playerSkins[_i].spriteLockTeam.getColor().a - _dt * 100) });
		}
		else if (!_teamfight && _arena)
		{
			spriteRoundNumberChoice.setColor(sf::Color{ 255,255,255,(sf::Uint8)alpha });// (sf::Uint8)(spriteRoundNumberChoice.getColor().a - _dt * 100) });
			spriteLockRoundNumber.setColor(sf::Color{ 255,255,255,(sf::Uint8)alpha });// (sf::Uint8)(spriteLockRound.getColor().a - _dt * 100) });
		}
		spriteMask.setColor(sf::Color{ 255,255,255, 0 });
	}

}

//Display the sprite to insult if you don't have controller connected
void CharacterSelection::DisplayWarningController(GameData& _gameData)
{
	if (!sf::Joystick::isConnected(0) && !sf::Joystick::isConnected(1) && !sf::Joystick::isConnected(2) && !sf::Joystick::isConnected(3))
	{
		Utils::BlitSprite(manetteEncule, { (float)((WORKING_WIDTH - 503) / 2), (float)(WORKING_HEIGHT / 2 + HEIGHT_GAP - manetteEncule.getGlobalBounds().height / 2) }, _gameData.window);
	}
}

//Display the sprite to insult if you don't have controller connected
void CharacterSelection::DisplayWarningController(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	if (!sf::Joystick::isConnected(0) && !sf::Joystick::isConnected(1) && !sf::Joystick::isConnected(2) && !sf::Joystick::isConnected(3))
	{		
		Utils::BlitSprite(manetteEncule, sf::Vector2f{ (float)((WORKING_WIDTH - 503) / 2) + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),(float)(WORKING_HEIGHT / 2 + HEIGHT_GAP - manetteEncule.getGlobalBounds().height / 2) }, 0, _red);
		Utils::BlitSprite(manetteEncule, sf::Vector2f{ (float)((WORKING_WIDTH - 503) / 2) - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),(float)(WORKING_HEIGHT / 2 + HEIGHT_GAP - manetteEncule.getGlobalBounds().height / 2) }, 0, _blue);
	}
}

void CharacterSelection::Display(GameData& _gameData)
{
	_gameData.window.setView(_gameData.camera->GetView());
	_gameData.window.draw(spriteBackground);

	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		FadeGoToGame(_gameData.camera->moveMenuToPlayerSelection, i, _gameData.dt, _gameData.teamfight, _gameData.arenaIsChose, _gameData.multiplayer);
		//display background of the frame
		Utils::BlitSprite(playerSkins[i].spriteBackgroundFrame, playerSkins[i].spriteBackgroundFrame.getPosition(), _gameData.window);
		//display skins
		if (!_gameData.camera->moveMenuToPlayerSelection)
		{
			Utils::BlitSprite(playerSkins[i].nextSkinSelected, playerSkins[i].posNextSkinSelected, _gameData.window);
		}
		Utils::BlitSprite(playerSkins[i].spriteSkinPlayer, playerSkins[i].posSpriteSkinPlayer, _gameData.window);
	}

	_gameData.window.draw(spriteMask);

	//Display encadré des skins
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		_gameData.window.draw(playerSkins[i].spriteFramePlayer);
		_gameData.window.draw(playerSkins[i].spriteNumberPlayerCurrent);
		_gameData.window.draw(playerSkins[i].spriteLockSkin);
		if (_gameData.teamfight)
		{
			_gameData.window.draw(playerSkins[i].spriteTeamChoice);
			_gameData.window.draw(playerSkins[i].spriteLockTeam);
		}
		if(!_gameData.teamfight && _gameData.arenaIsChose)
		{
			_gameData.window.draw(spriteRoundNumberChoice);
			_gameData.window.draw(spriteLockRoundNumber);
		}
	}

	DisplayWarningController(_gameData);
	_gameData.window.draw(shadowFountain);
	_gameData.window.draw(fountain);

	//Display IP
	if (_gameData.multiplayer)
	{
		if (_gameData.network->isRevealed)
		{
			_gameData.network->tabText[0]->setString(sf::IpAddress::getLocalAddress().toString());
			_gameData.network->tabText[0]->setPosition({ (float)(WORKING_WIDTH / 2 - 100),(float)(220 + WORKING_HEIGHT / 1.3f) });
			_gameData.network->tabText[0]->setCharacterSize(50);
			_gameData.network->tabText[0]->setFillColor(sf::Color::Red);
			_gameData.window.draw(*_gameData.network->tabText[0]);
		}
		else
		{
			_gameData.window.draw(*_gameData.network->tabText[3]);
		}
	}
}

void CharacterSelection::Display3D(GameData& _gameData)
{
	_gameData.window.setView(_gameData.camera->GetView());

	_gameData.renderTextureRed.setView(_gameData.camera->GetView());
	_gameData.renderTextureBlue.setView(_gameData.camera->GetView());

	Utils::BlitSprite(spriteBackground, sf::Vector2f{ spriteBackground.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),spriteBackground.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(spriteBackground, sf::Vector2f{ spriteBackground.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),spriteBackground.getPosition().y }, 0, _gameData.renderTextureBlue);

	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		FadeGoToGame(_gameData.camera->moveMenuToPlayerSelection, i, _gameData.dt, _gameData.teamfight, _gameData.arenaIsChose, _gameData.multiplayer);

		//display background of the frame
		Utils::BlitSprite(playerSkins[i].spriteBackgroundFrame, sf::Vector2f{ playerSkins[i].spriteBackgroundFrame.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),playerSkins[i].spriteBackgroundFrame.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(playerSkins[i].spriteBackgroundFrame, sf::Vector2f{ playerSkins[i].spriteBackgroundFrame.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),playerSkins[i].spriteBackgroundFrame.getPosition().y }, 0, _gameData.renderTextureBlue);

		//display skins
		if (!_gameData.camera->moveMenuToPlayerSelection)
		{
			Utils::BlitSprite(playerSkins[i].nextSkinSelected, sf::Vector2f{ playerSkins[i].posNextSkinSelected.x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),playerSkins[i].posNextSkinSelected.y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(playerSkins[i].nextSkinSelected, sf::Vector2f{ playerSkins[i].posNextSkinSelected.x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),playerSkins[i].posNextSkinSelected.y }, 0, _gameData.renderTextureBlue);
		}

		Utils::BlitSprite(playerSkins[i].spriteSkinPlayer, sf::Vector2f{ playerSkins[i].posSpriteSkinPlayer.x + (TROAD_GAP * (-1 * 700.f + WORKING_HEIGHT) / 100.f),playerSkins[i].posSpriteSkinPlayer.y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(playerSkins[i].spriteSkinPlayer, sf::Vector2f{ playerSkins[i].posSpriteSkinPlayer.x - (TROAD_GAP * (-1 * 700.f + WORKING_HEIGHT) / 100.f),playerSkins[i].posSpriteSkinPlayer.y }, 0, _gameData.renderTextureBlue);
	}

	Utils::BlitSprite(spriteMask, sf::Vector2f{ spriteMask.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),spriteMask.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(spriteMask, sf::Vector2f{ spriteMask.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),spriteMask.getPosition().y }, 0, _gameData.renderTextureBlue);

	//Display encadré des skins
	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		Utils::BlitSprite(playerSkins[i].spriteFramePlayer, sf::Vector2f{ playerSkins[i].spriteFramePlayer.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),playerSkins[i].spriteFramePlayer.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(playerSkins[i].spriteFramePlayer, sf::Vector2f{ playerSkins[i].spriteFramePlayer.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),playerSkins[i].spriteFramePlayer.getPosition().y }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(playerSkins[i].spriteNumberPlayerCurrent, sf::Vector2f{ playerSkins[i].spriteNumberPlayerCurrent.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),playerSkins[i].spriteNumberPlayerCurrent.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(playerSkins[i].spriteNumberPlayerCurrent, sf::Vector2f{ playerSkins[i].spriteNumberPlayerCurrent.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),playerSkins[i].spriteNumberPlayerCurrent.getPosition().y }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(playerSkins[i].spriteLockSkin, sf::Vector2f{ playerSkins[i].spriteLockSkin.getPosition().x + (TROAD_GAP * (-1 * 750.f + WORKING_HEIGHT) / 100.f),playerSkins[i].spriteLockSkin.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(playerSkins[i].spriteLockSkin, sf::Vector2f{ playerSkins[i].spriteLockSkin.getPosition().x - (TROAD_GAP * (-1 * 750.f + WORKING_HEIGHT) / 100.f),playerSkins[i].spriteLockSkin.getPosition().y }, 0, _gameData.renderTextureBlue);

		if (_gameData.teamfight)
		{
			Utils::BlitSprite(playerSkins[i].spriteTeamChoice, sf::Vector2f{ playerSkins[i].spriteTeamChoice.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),playerSkins[i].spriteTeamChoice.getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(playerSkins[i].spriteTeamChoice, sf::Vector2f{ playerSkins[i].spriteTeamChoice.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),playerSkins[i].spriteTeamChoice.getPosition().y }, 0, _gameData.renderTextureBlue);

			Utils::BlitSprite(playerSkins[i].spriteLockTeam, sf::Vector2f{ playerSkins[i].spriteLockTeam.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),playerSkins[i].spriteLockTeam.getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(playerSkins[i].spriteLockTeam, sf::Vector2f{ playerSkins[i].spriteLockTeam.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),playerSkins[i].spriteLockTeam.getPosition().y }, 0, _gameData.renderTextureBlue);
		}

		if (!_gameData.teamfight && _gameData.arenaIsChose)
		{
			Utils::BlitSprite(spriteRoundNumberChoice, sf::Vector2f{ spriteRoundNumberChoice.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),spriteRoundNumberChoice.getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(spriteRoundNumberChoice, sf::Vector2f{ spriteRoundNumberChoice.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),spriteRoundNumberChoice.getPosition().y }, 0, _gameData.renderTextureBlue);

			Utils::BlitSprite(spriteLockRoundNumber, sf::Vector2f{ spriteLockRoundNumber.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),spriteLockRoundNumber.getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(spriteLockRoundNumber, sf::Vector2f{ spriteLockRoundNumber.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),spriteLockRoundNumber.getPosition().y }, 0, _gameData.renderTextureBlue);
		}
	}

	DisplayWarningController(_gameData.renderTextureRed, _gameData.renderTextureBlue);

	Utils::BlitSprite(shadowFountain, sf::Vector2f{ shadowFountain.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),shadowFountain.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(shadowFountain, sf::Vector2f{ shadowFountain.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),shadowFountain.getPosition().y }, 0, _gameData.renderTextureBlue);
	
	Utils::BlitSprite(fountain, sf::Vector2f{ fountain.getPosition().x + (TROAD_GAP * (-1 * fountain.getPosition().y + WORKING_HEIGHT) / 100.f),fountain.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(fountain, sf::Vector2f{ fountain.getPosition().x - (TROAD_GAP * (-1 * fountain.getPosition().y + WORKING_HEIGHT) / 100.f),fountain.getPosition().y }, 0, _gameData.renderTextureBlue);
}

void CharacterSelection::JoystickPressedMulti(Network& _network, Audio& _audio, Camera& _camera)
{
	int i = 0;
	if (_network.state != NetworkState::HOSTWAIT)
	{
		i = _network.client->id;
	}
	if (sf::Joystick::getIdentification(GetFirstController(0)).name == "Wireless Controller")
	{
		if (sf::Joystick::isButtonPressed(GetFirstController(0), (unsigned int)ControllerButtonPS4::CROSS))
		{
			if (!playerSkins[i].isReady && !playerSkins[i].slidingUp && !playerSkins[i].slidingDown)// && sf::Joystick::getAxisPosition(GetFirstController(0), sf::Joystick::Axis::LEFT_JOYSTICK_Y) >= -15.f && sf::Joystick::getAxisPosition(GetFirstController(0), sf::Joystick::Axis::LEFT_JOYSTICK_Y) <= 15.f && !IsSkinAlreadySelected(playerSkins[i].currentSkin))
			{
				_audio.PlaySoundNotRepeated("BUTTON_CLICK_EFFECT", 100.f);
				playerSkins[i].isReady = true;
				playerSkins[i].spriteLockSkin = allPlayerLockSkins[3];
				playerSkins[i].spriteSkinPlayer = playerSkins[i].nextSkinSelected;
				playerSkins[i].posNextSkinSelected = { playerSkins[i].posSpriteSkinPlayer.x,  MIDDLE_HEIGHT + HEIGHT_GAP };
				playerSkins[i].posSpriteSkinPlayer = { playerSkins[i].posNextSkinSelected.x, MIDDLE_HEIGHT + HEIGHT_GAP };

				nbPlayerReady++;
			}
			else if (!playerSkins[i].isReady && !playerSkins[i].slidingUp && !playerSkins[i].slidingDown)// && sf::Joystick::getAxisPosition(GetFirstController(0), sf::Joystick::Axis::LEFT_JOYSTICK_Y) >= -15.f && sf::Joystick::getAxisPosition(GetFirstController(0), sf::Joystick::Axis::LEFT_JOYSTICK_Y) <= 15.f && IsSkinAlreadySelected(playerSkins[i].currentSkin))
			{
				_audio.PlaySoundNotRepeated("SWORD_CURSOR_HINHIN", 100.f);
			}
		}

		if (sf::Joystick::isButtonPressed(GetFirstController(0), (unsigned int)ControllerButtonPS4::CIRCLE))
		{
			if (playerSkins[i].isReady && _camera.moveMenuToPlayerSelection == false)
			{
				_audio.PlaySoundNotRepeated("BUTTON_CLICK_EFFECT", 100.f);

				playerSkins[i].isReady = false;
				nbPlayerReady--;
			}
		}
	}
	else
	{
		if (sf::Joystick::isButtonPressed(GetFirstController(0), (unsigned int)ControllerButtonXBOX::A))
		{
			if (!playerSkins[i].isReady && !playerSkins[i].slidingUp && !playerSkins[i].slidingDown)// && sf::Joystick::getAxisPosition(GetFirstController(0), sf::Joystick::Axis::LEFT_JOYSTICK_Y) >= -15.f && sf::Joystick::getAxisPosition(GetFirstController(0), sf::Joystick::Axis::LEFT_JOYSTICK_Y) <= 15.f && !IsSkinAlreadySelected(playerSkins[i].currentSkin))
			{
				_audio.PlaySoundNotRepeated("BUTTON_CLICK_EFFECT", 100.f);

				playerSkins[i].isReady = true;
				playerSkins[i].spriteLockSkin = allPlayerLockSkins[1];
				playerSkins[i].spriteSkinPlayer = playerSkins[i].nextSkinSelected;
				playerSkins[i].posNextSkinSelected = { playerSkins[i].posSpriteSkinPlayer.x,  MIDDLE_HEIGHT + HEIGHT_GAP };
				playerSkins[i].posSpriteSkinPlayer = { playerSkins[i].posNextSkinSelected.x, MIDDLE_HEIGHT + HEIGHT_GAP };
				nbPlayerReady++;
			}
			else if (!playerSkins[i].isReady && !playerSkins[i].slidingUp && !playerSkins[i].slidingDown)// && sf::Joystick::getAxisPosition(GetFirstController(0), sf::Joystick::Axis::LEFT_JOYSTICK_Y) >= -15.f && sf::Joystick::getAxisPosition(GetFirstController(0), sf::Joystick::Axis::LEFT_JOYSTICK_Y) <= 15.f && IsSkinAlreadySelected(playerSkins[i].currentSkin))
			{
				_audio.PlaySoundNotRepeated("SWORD_CURSOR_HINHIN", 100.f);
			}
		}

		if (sf::Joystick::isButtonPressed(GetFirstController(0), (unsigned int)ControllerButtonXBOX::B))
		{
			if (playerSkins[i].isReady && _camera.moveMenuToPlayerSelection == false)
			{
				_audio.PlaySoundNotRepeated("BUTTON_CLICK_EFFECT", 100.f);

				playerSkins[i].isReady = false;
				nbPlayerReady--;
			}
		}
	}
}

void CharacterSelection::JoystickPressedLocal(int _nbPlayers, Audio& _audio, Camera& _camera, float _dt, bool _arena, bool _teamfight)
{
	for (int i = 0; i < _nbPlayers; i++)
	{
		if (sf::Joystick::getIdentification(GetFirstController(i)).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(GetFirstController(i), (unsigned int)ControllerButtonPS4::CROSS))
			{
				if (!playerSkins[i].isReady && !playerSkins[i].slidingUp && !playerSkins[i].slidingDown && sf::Joystick::getAxisPosition(GetFirstController(i), sf::Joystick::Axis::LEFT_JOYSTICK_Y) >= -15.f &&
					sf::Joystick::getAxisPosition(GetFirstController(i), sf::Joystick::Axis::LEFT_JOYSTICK_Y) <= 15.f && !IsSkinAlreadySelected(playerSkins[i].currentSkin))
				{
					_audio.PlaySoundNotRepeated("BUTTON_CLICK_EFFECT", 100.f);
					playerSkins[i].isReady = true;
					playerSkins[i].spriteLockSkin = allPlayerLockSkins[3];
					playerSkins[i].spriteSkinPlayer = playerSkins[i].nextSkinSelected;
					playerSkins[i].posNextSkinSelected = { playerSkins[i].posSpriteSkinPlayer.x,  MIDDLE_HEIGHT + HEIGHT_GAP };
					playerSkins[i].posSpriteSkinPlayer = { playerSkins[i].posNextSkinSelected.x, MIDDLE_HEIGHT + HEIGHT_GAP };

					nbPlayerReady++;
				}
				else if (i == 0 && playerSkins[0].isReady && !roundNumberIsSelected && _arena && !_teamfight)
				{
					roundNumberIsSelected = true;
					spriteLockRoundNumber = allPlayerLockSkins[3];
				}
				else if (playerSkins[i].isReady && !playerSkins[i].teamSelected && _arena && _teamfight)
				{
					playerSkins[i].teamSelected = true;
					playerSkins[i].spriteLockTeam = allPlayerLockSkins[3];
					nbPlayerTeamSelected++;
				}
				else if (!playerSkins[i].isReady && !playerSkins[i].slidingUp && !playerSkins[i].slidingDown && sf::Joystick::getAxisPosition(GetFirstController(i), sf::Joystick::Axis::LEFT_JOYSTICK_Y) >= -15.f &&
					sf::Joystick::getAxisPosition(GetFirstController(i), sf::Joystick::Axis::LEFT_JOYSTICK_Y) <= 15.f && IsSkinAlreadySelected(playerSkins[i].currentSkin))
				{
					_audio.PlaySoundNotRepeated("SWORD_CURSOR_HINHIN", 100.f);
				}
			}

			if (sf::Joystick::isButtonPressed(GetFirstController(i), (unsigned int)ControllerButtonPS4::CIRCLE))
			{
				if (playerSkins[i].isReady && _camera.moveMenuToPlayerSelection == false && !playerSkins[i].teamSelected && !roundNumberIsSelected)
				{
					_audio.PlaySoundNotRepeated("BUTTON_CLICK_EFFECT", 100.f);
					playerSkins[i].isReady = false;
					playerSkins[i].spriteLockSkin = allPlayerLockSkins[2];
					nbPlayerReady--;
				}
				else if (i == 0 && playerSkins[0].isReady && !_camera.moveMenuToPlayerSelection && roundNumberIsSelected && _arena && !_teamfight)
				{
					roundNumberIsSelected = false;
					spriteLockRoundNumber = allPlayerLockSkins[2];
				}
				else if (playerSkins[i].isReady && !_camera.moveMenuToPlayerSelection && playerSkins[i].teamSelected && _arena && _teamfight)
				{
					playerSkins[i].teamSelected = false;
					playerSkins[i].spriteLockTeam = allPlayerLockSkins[2];
					nbPlayerTeamSelected--;
				}
			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(GetFirstController(i), (unsigned int)ControllerButtonXBOX::A))
			{
				if (!playerSkins[i].isReady && !playerSkins[i].slidingUp && !playerSkins[i].slidingDown && sf::Joystick::getAxisPosition(GetFirstController(i), sf::Joystick::Axis::LEFT_JOYSTICK_Y) >= -15.f &&
					sf::Joystick::getAxisPosition(GetFirstController(i), sf::Joystick::Axis::LEFT_JOYSTICK_Y) <= 15.f && !IsSkinAlreadySelected(playerSkins[i].currentSkin))
				{
					_audio.PlaySoundNotRepeated("BUTTON_CLICK_EFFECT", 100.f);

					playerSkins[i].isReady = true;
					playerSkins[i].spriteLockSkin = allPlayerLockSkins[1];
					playerSkins[i].spriteSkinPlayer = playerSkins[i].nextSkinSelected;
					playerSkins[i].posNextSkinSelected = { playerSkins[i].posSpriteSkinPlayer.x,  MIDDLE_HEIGHT + HEIGHT_GAP };
					playerSkins[i].posSpriteSkinPlayer = { playerSkins[i].posNextSkinSelected.x, MIDDLE_HEIGHT + HEIGHT_GAP };

					nbPlayerReady++;

				}
				else if (i == 0 && playerSkins[0].isReady && !roundNumberIsSelected && _arena && !_teamfight)
				{
					roundNumberIsSelected = true;
					spriteLockRoundNumber = allPlayerLockSkins[1];
				}
				else if (playerSkins[i].isReady && !playerSkins[i].teamSelected && _arena && _teamfight)
				{
					playerSkins[i].teamSelected = true;
					playerSkins[i].spriteLockTeam = allPlayerLockSkins[1];
					nbPlayerTeamSelected++;
				}
				else if (!playerSkins[i].isReady && !playerSkins[i].slidingUp && !playerSkins[i].slidingDown && sf::Joystick::getAxisPosition(GetFirstController(i), sf::Joystick::Axis::LEFT_JOYSTICK_Y) >= -15.f &&
					sf::Joystick::getAxisPosition(GetFirstController(i), sf::Joystick::Axis::LEFT_JOYSTICK_Y) <= 15.f && IsSkinAlreadySelected(playerSkins[i].currentSkin))
				{
					_audio.PlaySoundNotRepeated("SWORD_CURSOR_HINHIN", 100.f);
				}

			}

			if (sf::Joystick::isButtonPressed(GetFirstController(i), (unsigned int)ControllerButtonXBOX::B))
			{
				if (playerSkins[i].isReady && _camera.moveMenuToPlayerSelection == false && !playerSkins[i].teamSelected && !roundNumberIsSelected)
				{
					_audio.PlaySoundNotRepeated("BUTTON_CLICK_EFFECT", 100.f);

					playerSkins[i].isReady = false;
					playerSkins[i].spriteLockSkin = allPlayerLockSkins[0];
					nbPlayerReady--;
				}
				else if (i == 0 && playerSkins[i].isReady && !_camera.moveMenuToPlayerSelection && roundNumberIsSelected && _arena && !_teamfight)
				{
					roundNumberIsSelected = false;
					spriteLockRoundNumber = allPlayerLockSkins[2];
				}
				else if (playerSkins[i].isReady && !_camera.moveMenuToPlayerSelection && playerSkins[i].teamSelected && _arena && _teamfight)
				{
					playerSkins[i].teamSelected = false;
					playerSkins[i].spriteLockTeam = allPlayerLockSkins[0];
					nbPlayerTeamSelected--;
				}				
			}
		}
	}
}
void CharacterSelection::JoystickPressed(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::JoystickButtonPressed)
	{
		if (!_gameData.multiplayer)
		{
			JoystickPressedLocal((int)_gameData.tabPlayers.size(), *_gameData.audio, *_gameData.camera, _gameData.dt, _gameData.arenaIsChose, _gameData.teamfight);
		}
		else
		{
			JoystickPressedMulti(*_gameData.network, *_gameData.audio, *_gameData.camera);
		}
	}
}