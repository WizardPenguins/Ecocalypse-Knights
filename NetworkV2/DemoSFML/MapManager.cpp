#include "stdafx.h"
#include "MapManager.h"
#include "Easings.h"
#include "Camera.h"

void MapManager::Init(GameData& _gameData)
{
	backGroundLevelSelector = Utils::LoadSprite("Assets/Images/BackGround/Map2.png", TOP_LEFT);
	backGroundDuBackGroundLevelSelector = Utils::LoadSprite("Assets/Images/BackGround/WorldMapBack.png", TOP_LEFT);
	//player = Utils::LoadSprite("Assets/Images/Entities/Players/Hit/PlayerHit2.png", CENTER);

	caravan = Utils::LoadSprite("Assets/Images/BackGround/CaravanAnim.png", TOP_LEFT);
	caravanAnim = new Animation;
	sf::IntRect rect = sf::IntRect(0, 0, 992 / 8, 136);
	caravan.setTextureRect(rect);
	caravanAnim->CreateLeftToRight(caravan, 8, 1, { 992,136 });

	locked = Utils::LoadSprite("Assets/Images/BackGround/Locked.png", CENTER);

	level2 = Utils::LoadSprite("Assets/Images/BackGround/Level2Anim.png", TOP_LEFT);
	level2Anim = new Animation;
	sf::IntRect zebi = sf::IntRect(0, 0, 2170 / 10, 207);
	level2.setTextureRect(zebi);
	level2Anim->CreateLeftToRight(level2, 10, 1, { 2170,207 });

	level3 = Utils::LoadSprite("Assets/Images/BackGround/Level3Anim.png", CENTER);
	level3Anim = new Animation;
	sf::IntRect zeb3 = sf::IntRect(0, 0, 2052 / 9, 295);
	level3.setTextureRect(zeb3);
	level3Anim->CreateLeftToRight(level3, 9, 1, { 2052,295 });

	boar = Utils::LoadSprite("Assets/Images/BackGround/Boar_Map.png", TOP_LEFT);
	boarAnim = new Animation;
	sf::IntRect rect2 = sf::IntRect(0, 0, 3020 / 10, 335);
	boar.setTextureRect(rect2);
	boarAnim->CreateLeftToRight(boar, 10, 1, { 3020,335 });
	boar.setScale(0.5f, 0.5f);

	witch = Utils::LoadSprite("Assets/Images/BackGround/Witch_Map.png", CENTER, 10, 1);
	witchAnim = new Animation;
	sf::IntRect rect25456 = sf::IntRect(0, 0, 3630 / 10, 636);
	witch.setTextureRect(rect25456);
	witchAnim->CreateLeftToRight(witch, 10, 1, { 3630,636 });
	witch.setScale(0.5f, 0.5f);

	travelLevel2 = Utils::LoadSprite("Assets/Images/BackGround/TravelLevel2Anim.png", TOP_LEFT);
	travelLevel2Anim = new Animation;
	sf::IntRect rect2556 = sf::IntRect(0, 0, 2752 / 8, 185);
	travelLevel2.setTextureRect(rect2556);
	travelLevel2Anim->CreateLeftToRight(travelLevel2, 8, 1, { 2752,185 });

	travelCaravan = Utils::LoadSprite("Assets/Images/BackGround/TravelCaravanAnim.png", TOP_LEFT);
	travelCaravanAnim = new Animation;
	sf::IntRect rect256 = sf::IntRect(0, 0, 856 / 4, 121);
	travelCaravan.setTextureRect(rect256);
	travelCaravanAnim->CreateLeftToRight(travelCaravan, 4, 1, { 856,121 });

	travelLevel3 = Utils::LoadSprite("Assets/Images/BackGround/TravelLevel3Anim.png", TOP_LEFT);
	travelLevel3Anim = new Animation;
	sf::IntRect rect255 = sf::IntRect(0, 0, 2106 / 9, 170);
	travelLevel3.setTextureRect(rect255);
	travelLevel3Anim->CreateLeftToRight(travelLevel2, 9, 1, { 2106,170 });

	posLevel.push_back({ 680.f,1460.f });
	posLevel.push_back({ 929.f, 1384.f });
	posLevel.push_back({ 1200.f, 1328.f });
	posLevel.push_back({ 1490.f, 1394.f });
	nameLevel.push_back(Utils::LoadSprite("Assets/Images/Menu/MapSelection4.png", CENTER));
	nameLevel.push_back(Utils::LoadSprite("Assets/Images/Menu/MapSelection1.png", CENTER));
	nameLevel.push_back(Utils::LoadSprite("Assets/Images/Menu/MapSelection2.png", CENTER));
	nameLevel.push_back(Utils::LoadSprite("Assets/Images/Menu/MapSelection3.png", CENTER));

	for (int i = 0; i < NB_LEVEL; i++)
	{
		nameLevel[i].setPosition(posLevel[i].x, posLevel[i].y - 300.f);
	}

	playerGoOn = NONEMAP;
	playerIsOn = MAP0;
	timer = 0;
	player.setPosition(posLevel[1]);
	font.loadFromFile("Assets/Fonts/Sparkle.otf");

	for (int i = 0; i < 4; i++)
	{
		playerRun[i] = _gameData.playersSprite->playerSprite[i][(int)AllSprite::RUN];
		playerRunAnim[i] = new Animation;
		sf::IntRect rect = sf::IntRect(0, 0, 1232 / 8, 176);
		playerRun[i].setTextureRect(rect);
		playerRunAnim[i]->CreateLeftToRight(playerRun[i], 8, 1, { 1232,176 });
		playerRun[i].setScale(0.75f, 0.75f);
	}

	for (int i = 0; i < 4; i++)
	{
		playerIdle[i] = _gameData.playersSprite->playerSprite[i][(int)AllSprite::IDLE];
		playerIdleAnim[i] = new Animation;
		sf::IntRect rect = sf::IntRect(0, 0, 1056 / 8, 167);
		playerIdle[i].setTextureRect(rect);
		playerIdleAnim[i]->CreateLeftToRight(playerIdle[i], 8, 1, { 1056,167 });
		playerIdle[i].setScale(0.75f, 0.75f);
	}

	//text.setFont(font);
	//text.setFillColor(sf::Color::White);
	playerDirection = RIGHT;
}

void MapManager::Update(GameData& _gameData, bool _level1IsDone, bool _level2IsDone)
{

	PlayerChooseLevel(_gameData);
	PlayerMove(_gameData);
	UpdateCamera(_gameData);
	PopUpPositionUpdate(_gameData);
	bool temp = true;
	if (!playerIsOn)
	{
		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{

			playerRunAnim[i]->UpdateLeftToRight(playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()], _gameData.dt, 8, 0, temp, true);
			playerIdleAnim[i]->currentFrame = 0;
		}
	}
	else
	{
		for (int i = 0; i < _gameData.tabPlayers.size(); i++)
		{

			playerIdleAnim[i]->UpdateLeftToRight(playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()], _gameData.dt, 8, 0, temp, true);
			playerRunAnim[i]->currentFrame = 0;
		}
	}

	if (travelLevel2Anim->currentFrame != travelLevel2Anim->maxFrame && _level1IsDone)
	{

		travelLevel2Anim->UpdateLeftToRight(travelLevel2, _gameData.dt, 8, 0, temp, false);
	}
	if (travelCaravanAnim->currentFrame != travelCaravanAnim->maxFrame && _level1IsDone)
	{

		travelCaravanAnim->UpdateLeftToRight(travelCaravan, _gameData.dt, 8, 0, temp, false);
	}
	if (level2Anim->currentFrame != level2Anim->maxFrame && travelLevel2Anim->currentFrame == travelLevel2Anim->maxFrame)
	{

		level2Anim->UpdateLeftToRight(level2, _gameData.dt, 8, 0, temp, false);
	}
	if (caravanAnim->currentFrame != caravanAnim->maxFrame && travelCaravanAnim->currentFrame == travelCaravanAnim->maxFrame)
	{

		caravanAnim->UpdateLeftToRight(caravan, _gameData.dt, 8, 0, temp, false);
	}
	if (travelLevel3Anim->currentFrame != travelLevel3Anim->maxFrame && _level2IsDone)
	{

		travelLevel3Anim->UpdateLeftToRight(travelLevel3, _gameData.dt, 8, 0, temp, false);
	}
	if (level3Anim->currentFrame != level3Anim->maxFrame && travelLevel3Anim->currentFrame == travelLevel3Anim->maxFrame)
	{

		level3Anim->UpdateLeftToRight(level3, _gameData.dt, 8, 0, temp, false);
	}

	if (!_level2IsDone)
	{
		boarAnim->UpdateLeftToRight(boar, _gameData.dt, 8, 0, temp, true);
	}
	else
	{
		witchAnim->UpdateLeftToRight(witch, _gameData.dt, 8, 0, temp, true);
	}
}

void MapManager::PlayerChooseLevel(GameData& _gameData)
{
	sf::Vector2f playerPos = player.getPosition();
	for (int i = 0; i < NB_LEVEL; i++)
	{
		if (playerPos == posLevel[i] || playerPos.x >= posLevel[i].x - 1 && playerPos.x <= posLevel[i].x + 1 && playerPos.y >= posLevel[i].y - 1 && playerPos.y <= posLevel[i].y + 1)
		{
			playerPos = posLevel[i];
			playerGoOn = NONEMAP;
			playerIsOn = (PlayerGoOn)(i + 1);
			float xPos1 = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::LEFT_JOYSTICK_X);
			float yPos1 = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::LEFT_JOYSTICK_Y);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && i != NB_LEVEL - 1 || xPos1 > 15 && i != NB_LEVEL - 1)
			{

				timer = 0;
				playerGoOn = (PlayerGoOn)(i + 2);
				playerIsOn = NONEMAP;
				playerDirection = RIGHT;

			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && i != 0 || xPos1 < -15 && i != 0)
			{
				timer = 0;
				playerGoOn = (PlayerGoOn)(i);
				playerIsOn = NONEMAP;
				playerDirection = LEFT;
			}
		}

	}
}

void MapManager::PlayerMove(GameData& _gameData)
{
	if (playerGoOn != NONEMAP)
	{
		sf::Vector2f playerPos = player.getPosition();
		switch (playerGoOn)
		{
		case MAP0:
			timer += _gameData.dt / 16;
			player.setPosition(Easings::Lerp(playerPos, posLevel[0], Easings::OutQuart(timer)));
			break;
		case MAP1:
			player.setPosition(Easings::Lerp(playerPos, posLevel[1], Easings::OutQuart(timer)));
			timer += _gameData.dt / 16;
			break;
		case MAP2:
			player.setPosition(Easings::Lerp(playerPos, posLevel[2], Easings::OutQuart(timer)));
			timer += _gameData.dt / 16;
			break;
		case MAP3:
			player.setPosition(Easings::Lerp(playerPos, posLevel[3], Easings::OutQuart(timer)));
			timer += _gameData.dt / 16;
			break;
		case MAP4:
			player.setPosition(Easings::Lerp(playerPos, posLevel[4], Easings::OutQuart(timer)));
			timer += _gameData.dt / 16;
			break;
		case MAP5:
			player.setPosition(Easings::Lerp(playerPos, posLevel[5], Easings::OutQuart(timer)));
			timer += _gameData.dt / 16;
			break;
		case MAP6:
			player.setPosition(Easings::Lerp(playerPos, posLevel[6], Easings::OutQuart(timer)));
			timer += _gameData.dt / 16;
			break;
		case MAP7:
			player.setPosition(Easings::Lerp(playerPos, posLevel[7], Easings::OutQuart(timer)));
			timer += _gameData.dt / 16;
			break;
		case MAP8:
			player.setPosition(Easings::Lerp(playerPos, posLevel[8], Easings::OutQuart(timer)));
			timer += _gameData.dt / 16;
			break;
		case MAP9:
			player.setPosition(Easings::Lerp(playerPos, posLevel[9], Easings::OutQuart(timer)));
			timer += _gameData.dt / 16;
			break;
		case MAP10:
			player.setPosition(Easings::Lerp(playerPos, posLevel[10], Easings::OutQuart(timer)));
			timer += _gameData.dt / 16;
			break;
		default:
			std::cout << __FUNCTION__ << std::endl;
			break;
		}
	}
}

void MapManager::PopUpPositionUpdate(GameData& _gameData)
{
	sf::Vector2f playerPos = player.getPosition();
	for (int i = 0; i < NB_LEVEL; i++)
	{
		if (playerPos == posLevel[i] || playerPos.x >= posLevel[i].x - 10 && playerPos.x <= posLevel[i].x + 10 && playerPos.y >= posLevel[i].y - 10 && playerPos.y <= posLevel[i].y + 10)
		{
			playerPos = posLevel[i];
			//popUp.setPosition(posLevel[i].x, posLevel[i].y - 300.f);
			//string = " Level " + std::to_string(i + 1);
			//text.setString(string);
			//text.setPosition(posLevel[i].x - 40.f, posLevel[i].y - 330.f);
		}
	}
}

const char MapManager::SetLevel(GameData& _gameData)
{
	sf::Vector2f playerPos = player.getPosition();
	for (int i = 0; i < NB_LEVEL; i++)
	{
		if (playerPos == posLevel[i] || playerPos.x >= posLevel[i].x - 1 && playerPos.x <= posLevel[i].x + 1 && playerPos.y >= posLevel[i].y - 1 && playerPos.y <= posLevel[i].y + 1)
		{
			_gameData.lastPosInMap = posLevel[i];
			return i + 1;
		}
	}
	return playerGoOn;
}

void MapManager::UpdateCamera(GameData& _gameData)
{
	sf::Vector2f playerPos = player.getPosition();
	sf::View& view = _gameData.camera->GetView();
	view.setCenter(playerPos);
	if (playerPos.x < WORKING_WIDTH / 2.f && playerPos.y < WORKING_HEIGHT / 2.f)
	{
		view.setCenter(WORKING_WIDTH / 2, WORKING_HEIGHT / 2);
	}
	else if (playerPos.x > WORKING_WIDTH * 1.5f && playerPos.y > WORKING_HEIGHT * 1.5f)
	{
		view.setCenter(WORKING_WIDTH * 1.5f, WORKING_HEIGHT * 1.5f);
	}
	else if (playerPos.x > WORKING_WIDTH * 1.5f && playerPos.y < WORKING_HEIGHT / 2.f)
	{
		view.setCenter(WORKING_WIDTH * 1.5f, WORKING_HEIGHT / 2.f);
	}
	else if (playerPos.x < WORKING_WIDTH / 2.f)
	{
		view.setCenter(WORKING_WIDTH / 2.f, playerPos.y);
	}
	else if (playerPos.y < WORKING_HEIGHT / 2.f)
	{
		view.setCenter(playerPos.x, WORKING_HEIGHT / 2.f);
	}
	else if (playerPos.x > WORKING_WIDTH * 1.5f)
	{
		view.setCenter(WORKING_WIDTH * 1.5f, playerPos.y);
	}
	else if (playerPos.y > WORKING_HEIGHT * 1.5f)
	{
		view.setCenter(playerPos.x, WORKING_HEIGHT * 1.5f);
	}

}

void MapManager::Display(GameData& _gameData, bool _level2)
{
	_gameData.window.setView(_gameData.window.getDefaultView());
	_gameData.window.draw(backGroundDuBackGroundLevelSelector);
	_gameData.window.setView(_gameData.camera->GetView());

	Utils::BlitSprite(backGroundLevelSelector, { 0,0 }, _gameData.window);

	Utils::BlitSprite(travelLevel2, { 890.f,1300.f }, _gameData.window);

	Utils::BlitSprite(travelCaravan, { 700.f,1360.f }, _gameData.window);
	Utils::BlitSprite(caravan, { 600.f,1400.f }, _gameData.window);

	//display level 3 
	if (_level2)
	{
		Utils::BlitSprite(travelLevel3, { 1240.f,1290.f }, _gameData.window);
		Utils::BlitSprite(level3, { 2440.f,1300.f }, _gameData.window);
		//remplacer par la sorciere
		/*Utils::BlitSprite(flag, { 1540.f,1300.f }, _gameData.window);*/
		if (level3Anim->currentFrame == level3Anim->maxFrame)
		{
			witch.setColor(sf::Color::Black);
			Utils::BlitSprite(witch, { 1520.f,1270.f }, _gameData.window);
		}
	}
	Utils::BlitSprite(level2, { 1090.f,1180.f }, _gameData.window);
	if (!_level2)
	{
		Utils::BlitSprite(locked, { 1540.f,1350.f }, _gameData.window);
		if (level2Anim->currentFrame == level2Anim->maxFrame)
		{
			boar.setColor(sf::Color::Black);
			Utils::BlitSprite(boar, { 1110.f,1190.f }, _gameData.window);
		}
	}

	if (!playerIsOn)
	{
		if (_gameData.tabPlayers.size() == 1)
		{
			if (playerDirection == RIGHT)
			{
				playerRun[_gameData.tabPlayers[0]->GetPlayerSelected()].setScale(0.75f, 0.75f);
			}
			else
			{
				playerRun[_gameData.tabPlayers[0]->GetPlayerSelected()].setScale(-0.75f, 0.75f);
			}
			Utils::BlitSprite(playerRun[_gameData.tabPlayers[0]->GetPlayerSelected()], { player.getPosition().x , player.getPosition().y }, _gameData.window);
		}
		else
		{
			for (int i = _gameData.tabPlayers.size() - 1; i >= 0; i--)
			{
				if (playerDirection == RIGHT)
				{
					playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()].setScale(0.75f, 0.75f);
				}
				else
				{
					playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()].setScale(-0.75f, 0.75f);
				}
				switch (i)
				{
				case 0:
					Utils::BlitSprite(playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()], { player.getPosition().x - 50.f, player.getPosition().y }, _gameData.window);
					break;
				case 1:
					Utils::BlitSprite(playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()], { player.getPosition().x + 50.f, player.getPosition().y }, _gameData.window);
					break;

				case 2:
					Utils::BlitSprite(playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()], { player.getPosition().x, player.getPosition().y - 50.f }, _gameData.window);
					break;
				case 3:
					Utils::BlitSprite(playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()], { player.getPosition().x, player.getPosition().y + 50.f }, _gameData.window);
					break;
				default:
					break;
				}

			}
		}
	}
	else
	{
		if (_gameData.tabPlayers.size() == 1)
		{
			if (playerDirection == RIGHT)
			{
				playerIdle[_gameData.tabPlayers[0]->GetPlayerSelected()].setScale(0.75f, 0.75f);
			}
			else
			{
				playerIdle[_gameData.tabPlayers[0]->GetPlayerSelected()].setScale(-0.75f, 0.75f);
			}
			Utils::BlitSprite(playerIdle[_gameData.tabPlayers[0]->GetPlayerSelected()], { player.getPosition().x , player.getPosition().y }, _gameData.window);
		}
		else
		{
			for (int i = _gameData.tabPlayers.size() - 1; i >= 0; i--)
			{
				if (playerDirection == RIGHT)
				{
					playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()].setScale(0.75f, 0.75f);
				}
				else
				{
					playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()].setScale(-0.75f, 0.75f);
				}
				switch (i)
				{
				case 0:
					Utils::BlitSprite(playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()], { player.getPosition().x - 50.f, player.getPosition().y }, _gameData.window);
					break;
				case 1:
					Utils::BlitSprite(playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()], { player.getPosition().x + 50.f, player.getPosition().y }, _gameData.window);
					break;

				case 2:
					Utils::BlitSprite(playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()], { player.getPosition().x, player.getPosition().y - 50.f }, _gameData.window);
					break;
				case 3:
					Utils::BlitSprite(playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()], { player.getPosition().x, player.getPosition().y + 50.f }, _gameData.window);
					break;
				default:
					break;
				}
			}
		}
	}

	sf::Vector2f playerPos = player.getPosition();

	for (int i = 0; i < NB_LEVEL; i++)
	{
		if (playerPos == posLevel[i] || playerPos.x >= posLevel[i].x - 10 && playerPos.x <= posLevel[i].x + 10 && playerPos.y >= posLevel[i].y - 10 && playerPos.y <= posLevel[i].y + 10)
		{
			//playerIsOn = (PlayerGoOn)(i + 1);
			_gameData.window.draw(nameLevel[i]);
			//_gameData.window.draw(text);
		}
	}
}

void MapManager::Display3D(GameData& _gameData, bool _level2)
{
	_gameData.window.setView(_gameData.window.getDefaultView());
	_gameData.renderTextureRed.setView(_gameData.renderTextureRed.getDefaultView());
	_gameData.renderTextureBlue.setView(_gameData.renderTextureBlue.getDefaultView());

	Utils::BlitSprite(backGroundDuBackGroundLevelSelector, sf::Vector2f{ backGroundDuBackGroundLevelSelector.getPosition().x - (TROAD_GAP * (-1 * backGroundDuBackGroundLevelSelector.getPosition().y + WORKING_HEIGHT) / 100.f),backGroundDuBackGroundLevelSelector.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(backGroundDuBackGroundLevelSelector, sf::Vector2f{ backGroundDuBackGroundLevelSelector.getPosition().x + (TROAD_GAP * (-1 * backGroundDuBackGroundLevelSelector.getPosition().y + WORKING_HEIGHT) / 100.f),backGroundDuBackGroundLevelSelector.getPosition().y }, 0, _gameData.renderTextureBlue);

	_gameData.renderTextureRed.setView(_gameData.camera->GetView());
	_gameData.renderTextureBlue.setView(_gameData.camera->GetView());

	Utils::BlitSprite(backGroundLevelSelector, sf::Vector2f{ 0 - (TROAD_GAP * (-1 * 0 + WORKING_HEIGHT) / 100.f),0 }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(backGroundLevelSelector, sf::Vector2f{ 0 + (TROAD_GAP * (-1 * 0 + WORKING_HEIGHT) / 100.f),0 }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(travelCaravan, sf::Vector2f{ 700.f + (TROAD_GAP * (-1 * 1360.f + WORKING_HEIGHT) / 100.f),1360.f }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(travelCaravan, sf::Vector2f{ 700.f - (TROAD_GAP * (-1 * 1360.f + WORKING_HEIGHT) / 100.f),0 }, 1360.f, _gameData.renderTextureBlue);

	Utils::BlitSprite(caravan, sf::Vector2f{ 600.f + (TROAD_GAP * (-1 * 1400.f + WORKING_HEIGHT) / 100.f),600.f }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(caravan, sf::Vector2f{ 600.f - (TROAD_GAP * (-1 * 1400.f + WORKING_HEIGHT) / 100.f),600.f }, 0, _gameData.renderTextureBlue);

	if (_level2)
	{
		Utils::BlitSprite(travelLevel3, sf::Vector2f{ 1240.f + (TROAD_GAP * (-1 * 1290.f + WORKING_HEIGHT) / 100.f),1290.f }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(travelLevel3, sf::Vector2f{ 1240.f - (TROAD_GAP * (-1 * 1290.f + WORKING_HEIGHT) / 100.f),1290.f }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(level3, sf::Vector2f{ 2440.f + (TROAD_GAP * (-1 * 1300.f + WORKING_HEIGHT) / 100.f),1300.f }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(level3, sf::Vector2f{ 2440.f - (TROAD_GAP * (-1 * 1300.f + WORKING_HEIGHT) / 100.f),1300.f }, 0, _gameData.renderTextureBlue);

		if (level3Anim->currentFrame == level3Anim->maxFrame)
		{
			witch.setColor(sf::Color::Black);

			Utils::BlitSprite(witch, sf::Vector2f{ 1520.f + (TROAD_GAP * (-1 * 1270.f + WORKING_HEIGHT) / 100.f),1270.f }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(witch, sf::Vector2f{ 1520.f - (TROAD_GAP * (-1 * 1270.f + WORKING_HEIGHT) / 100.f),1270.f }, 0, _gameData.renderTextureBlue);
		}
	}

	Utils::BlitSprite(level2, sf::Vector2f{ 1090.f + (TROAD_GAP * (-1 * 1180.f + WORKING_HEIGHT) / 100.f),1180.f }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(level2, sf::Vector2f{ 1090.f - (TROAD_GAP * (-1 * 1180.f + WORKING_HEIGHT) / 100.f),1180.f }, 0, _gameData.renderTextureBlue);

	if (!_level2)
	{
		Utils::BlitSprite(locked, sf::Vector2f{ 1540.f + (TROAD_GAP * (-1 * 1350.f + WORKING_HEIGHT) / 100.f),1350.f }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(locked, sf::Vector2f{ 1540.f - (TROAD_GAP * (-1 * 1350.f + WORKING_HEIGHT) / 100.f),1350.f }, 0, _gameData.renderTextureBlue);

		if (level2Anim->currentFrame == level2Anim->maxFrame)
		{
			boar.setColor(sf::Color::Black);

			Utils::BlitSprite(boar, sf::Vector2f{ 1110.f + (TROAD_GAP * (-1 * 1190.f + WORKING_HEIGHT) / 100.f),1190.f }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(boar, sf::Vector2f{ 1110.f - (TROAD_GAP * (-1 * 1190.f + WORKING_HEIGHT) / 100.f),1190.f }, 0, _gameData.renderTextureBlue);
		}
	}

	if (!playerIsOn)
	{
		if (_gameData.tabPlayers.size() == 1)
		{
			if (playerDirection == RIGHT)
			{
				playerRun[_gameData.tabPlayers[0]->GetPlayerSelected()].setScale(0.75f, 0.75f);
			}
			else
			{
				playerRun[_gameData.tabPlayers[0]->GetPlayerSelected()].setScale(-0.75f, 0.75f);
			}

			Utils::BlitSprite(playerRun[_gameData.tabPlayers[0]->GetPlayerSelected()], sf::Vector2f{ player.getPosition().x + (TROAD_GAP * (-1 * player.getPosition().y + WORKING_HEIGHT) / 100.f),player.getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(playerRun[_gameData.tabPlayers[0]->GetPlayerSelected()], sf::Vector2f{ player.getPosition().x - (TROAD_GAP * (-1 * player.getPosition().y + WORKING_HEIGHT) / 100.f),player.getPosition().y }, 0, _gameData.renderTextureBlue);
		}
		else
		{
			for (int i = _gameData.tabPlayers.size() - 1; i >= 0; i--)
			{
				if (playerDirection == RIGHT)
				{
					playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()].setScale(0.75f, 0.75f);
				}
				else
				{
					playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()].setScale(-0.75f, 0.75f);
				}
				switch (i)
				{
				case 0:
					Utils::BlitSprite(playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ (player.getPosition().x - 50.f) + (TROAD_GAP * (-1 * player.getPosition().y + WORKING_HEIGHT) / 100.f),player.getPosition().y }, 0, _gameData.renderTextureRed);
					Utils::BlitSprite(playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ (player.getPosition().x - 50.f) - (TROAD_GAP * (-1 * player.getPosition().y + WORKING_HEIGHT) / 100.f),player.getPosition().y }, 0, _gameData.renderTextureBlue);
					break;
				case 1:
					Utils::BlitSprite(playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ (player.getPosition().x + 50.f) + (TROAD_GAP * (-1 * player.getPosition().y + WORKING_HEIGHT) / 100.f),player.getPosition().y }, 0, _gameData.renderTextureRed);
					Utils::BlitSprite(playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ (player.getPosition().x + 50.f) - (TROAD_GAP * (-1 * player.getPosition().y + WORKING_HEIGHT) / 100.f),player.getPosition().y }, 0, _gameData.renderTextureBlue);
					break;

				case 2:
					Utils::BlitSprite(playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ player.getPosition().x + (TROAD_GAP * (-1 * (player.getPosition().y - 50.f) + WORKING_HEIGHT) / 100.f),(player.getPosition().y - 50.f) }, 0, _gameData.renderTextureRed);
					Utils::BlitSprite(playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ player.getPosition().x - (TROAD_GAP * (-1 * (player.getPosition().y - 50.f) + WORKING_HEIGHT) / 100.f),(player.getPosition().y - 50.f) }, 0, _gameData.renderTextureBlue);
					break;
				case 3:
					Utils::BlitSprite(playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ player.getPosition().x + (TROAD_GAP * (-1 * (player.getPosition().y + 50.f) + WORKING_HEIGHT) / 100.f),(player.getPosition().y + 50.f) }, 0, _gameData.renderTextureRed);
					Utils::BlitSprite(playerRun[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ player.getPosition().x - (TROAD_GAP * (-1 * (player.getPosition().y + 50.f) + WORKING_HEIGHT) / 100.f),(player.getPosition().y + 50.f) }, 0, _gameData.renderTextureBlue);
					break;
				default:
					break;
				}

			}
		}
	}
	else
	{
		if (_gameData.tabPlayers.size() == 1)
		{
			if (playerDirection == RIGHT)
			{
				playerIdle[_gameData.tabPlayers[0]->GetPlayerSelected()].setScale(0.75f, 0.75f);
			}
			else
			{
				playerIdle[_gameData.tabPlayers[0]->GetPlayerSelected()].setScale(-0.75f, 0.75f);
			}
			Utils::BlitSprite(playerIdle[_gameData.tabPlayers[0]->GetPlayerSelected()], sf::Vector2f{ player.getPosition().x + (TROAD_GAP * (-1 * player.getPosition().y + WORKING_HEIGHT) / 100.f),player.getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(playerIdle[_gameData.tabPlayers[0]->GetPlayerSelected()], sf::Vector2f{ player.getPosition().x - (TROAD_GAP * (-1 * player.getPosition().y + WORKING_HEIGHT) / 100.f),player.getPosition().y }, 0, _gameData.renderTextureBlue);
		}
		else
		{
			for (int i = _gameData.tabPlayers.size() - 1; i >= 0; i--)
			{
				if (playerDirection == RIGHT)
				{
					playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()].setScale(0.75f, 0.75f);
				}
				else
				{
					playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()].setScale(-0.75f, 0.75f);
				}
				switch (i)
				{
				case 0:
					Utils::BlitSprite(playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ (player.getPosition().x - 50.f) + (TROAD_GAP * (-1 * player.getPosition().y + WORKING_HEIGHT) / 100.f),player.getPosition().y }, 0, _gameData.renderTextureRed);
					Utils::BlitSprite(playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ (player.getPosition().x - 50.f) - (TROAD_GAP * (-1 * player.getPosition().y + WORKING_HEIGHT) / 100.f),player.getPosition().y }, 0, _gameData.renderTextureBlue);
					break;
				case 1:
					Utils::BlitSprite(playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ (player.getPosition().x + 50.f) + (TROAD_GAP * (-1 * player.getPosition().y + WORKING_HEIGHT) / 100.f),player.getPosition().y }, 0, _gameData.renderTextureRed);
					Utils::BlitSprite(playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ (player.getPosition().x + 50.f) - (TROAD_GAP * (-1 * player.getPosition().y + WORKING_HEIGHT) / 100.f),player.getPosition().y }, 0, _gameData.renderTextureBlue);
					break;

				case 2:
					Utils::BlitSprite(playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ player.getPosition().x + (TROAD_GAP * (-1 * (player.getPosition().y - 50.f) + WORKING_HEIGHT) / 100.f),(player.getPosition().y - 50.f) }, 0, _gameData.renderTextureRed);
					Utils::BlitSprite(playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ player.getPosition().x - (TROAD_GAP * (-1 * (player.getPosition().y - 50.f) + WORKING_HEIGHT) / 100.f),(player.getPosition().y - 50.f) }, 0, _gameData.renderTextureBlue);
					break;
				case 3:
					Utils::BlitSprite(playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ player.getPosition().x + (TROAD_GAP * (-1 * (player.getPosition().y + 50.f) + WORKING_HEIGHT) / 100.f),(player.getPosition().y + 50.f) }, 0, _gameData.renderTextureRed);
					Utils::BlitSprite(playerIdle[_gameData.tabPlayers[i]->GetPlayerSelected()], sf::Vector2f{ player.getPosition().x - (TROAD_GAP * (-1 * (player.getPosition().y + 50.f) + WORKING_HEIGHT) / 100.f),(player.getPosition().y + 50.f) }, 0, _gameData.renderTextureBlue);
					break;
				default:
					break;
				}
			}
		}
	}

	sf::Vector2f playerPos = player.getPosition();
	for (int i = 0; i < NB_LEVEL; i++)
	{
		if (playerPos == posLevel[i] || playerPos.x >= posLevel[i].x - 10 && playerPos.x <= posLevel[i].x + 10 && playerPos.y >= posLevel[i].y - 10 && playerPos.y <= posLevel[i].y + 10)
		{
			Utils::BlitSprite(nameLevel[i], sf::Vector2f{ nameLevel[i].getPosition().x + (TROAD_GAP * (-1 * nameLevel[i].getPosition().y + WORKING_HEIGHT) / 100.f),nameLevel[i].getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(nameLevel[i], sf::Vector2f{ nameLevel[i].getPosition().x - (TROAD_GAP * (-1 * nameLevel[i].getPosition().y + WORKING_HEIGHT) / 100.f),nameLevel[i].getPosition().y }, 0, _gameData.renderTextureBlue);
		}
	}
}

void MapManager::Reload(GameData& _gameData)
{
	playerGoOn = NONEMAP;
	playerIsOn = MAP0;
	timer = 0;
	player.setPosition({ _gameData.lastPosInMap });

}
