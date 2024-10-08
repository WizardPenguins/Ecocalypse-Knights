#include "stdafx.h"
#include"Settings.h"
#include "Camera.h"

Settings::Settings(GameData& _gameData)
{
	displayMode = _gameData.displayMode;
	InitComponents(_gameData);
}

Settings::~Settings()
{
}

void Settings::InitComponents(GameData& _gameData)
{
	InitVolumeRect(_gameData);
	InitTexts();
	InitSprites();

	hpBarType = HpBarTypes::OFF;

	for (int i = 0; i < (int)TypeSelected::NB_TYPE; i++)
	{
		typeSelected.push_back(false);
		if (i == (int)TypeSelected::VOLUME)
		{
			typeSelected[i] = true;
		}
	}

	buttonVect[(int)SettingsButton::MINUS2]->SetIsSelected(true, _gameData);
	buttonVect[(int)SettingsButton::MINUS3]->SetIsSelected(true, _gameData);
	buttonVect[(int)SettingsButton::PLUS4]->SetIsSelected(true, _gameData);
	buttonVect[(int)SettingsButton::MINUS5]->SetIsSelected(true, _gameData);
	buttonVect[(int)SettingsButton::PLUS6]->SetIsSelected(true, _gameData);
}

void Settings::InitSprites(void)
{
	backgroundSprite = Utils::LoadSprite("Assets/Images/background/TitleScreen.png", TOP_LEFT);

	sf::Vector2f vPos = volumeBar.getPosition();
	sf::Vector2f vSize = (sf::Vector2f)volumeBar.getTexture()->getSize();

	sf::Vector2f tPos = transparentSprite.getPosition();
	sf::Vector2f tSize = (sf::Vector2f)transparentSprite.getTexture()->getSize();

	displayModeSelected.loadFromFile("Assets/Images/Settings/DisplayModeSelected.png");
	displayModeUnSelected.loadFromFile("Assets/Images/Settings/DisplayMode.png");
	displayModeName = Utils::LoadSprite("Assets/Images/Settings/DisplayMode.png", CENTER);

	fullscreenType.loadFromFile("Assets/Images/Settings/Fullscreen.png");
	borderlessType.loadFromFile("Assets/Images/Settings/Borderless.png");
	windowedType.loadFromFile("Assets/Images/Settings/Windowed.png");

	switch (displayMode)
	{
	case sf::Style::Default:
		displayType = Utils::LoadSprite("Assets/Images/Settings/Windowed.png", CENTER);
		displayTypes = DisplayTypes::WINDOWED;
		break;
	case sf::Style::None:
		displayType = Utils::LoadSprite("Assets/Images/Settings/Borderless.png", CENTER);
		displayTypes = DisplayTypes::BORDERLESS;
		break;
	case sf::Style::Fullscreen:
		displayType = Utils::LoadSprite("Assets/Images/Settings/Fullscreen.png", CENTER);
		displayTypes = DisplayTypes::FULLSCREEN;
		break;
	}

	displayType.setPosition(tPos.x, vPos.y + 120);
	displayModeName.setPosition(Screen::Pos(tPos.x, (vPos.y + 34) + 120));


	optionOn.loadFromFile("Assets/Images/Settings/On.png");
	optionOff.loadFromFile("Assets/Images/Settings/Off.png");
	hpBarMode = Utils::LoadSprite("Assets/Images/Settings/Off.png", CENTER);
	hpBarSelected.loadFromFile("Assets/Images/Settings/HpBarSelected.png");
	hpBarUnSelected.loadFromFile("Assets/Images/Settings/HpBar.png");
	hpBarName = Utils::LoadSprite("Assets/Images/Settings/HpBar.png", CENTER);;
	hpBarMode.setPosition(tPos.x, vPos.y + 240);
	hpBarName.setPosition(Screen::Pos(tPos.x, (vPos.y + 34) + 240));
	hpBarPictureOff.loadFromFile("Assets/Images/Settings/HealthBarPictureOff.png");
	hpBarPictureOn.loadFromFile("Assets/Images/Settings/HealthBarPictureOn.png");
	hpBarPicture = Utils::LoadSprite("Assets/Images/Settings/HealthBarPictureOff.png", CENTER);
	hpBarPicture.setPosition(hpBarMode.getPosition());

	friendShipMode = Utils::LoadSprite("Assets/Images/Settings/On.png", CENTER);
	friendShipSelected.loadFromFile("Assets/Images/Settings/PvPSelected.png");
	friendShipUnSelected.loadFromFile("Assets/Images/Settings/PvP.png");
	friendShipName = Utils::LoadSprite("Assets/Images/Settings/PvP.png", CENTER);;
	friendShipMode.setPosition(tPos.x, vPos.y + 360);
	friendShipName.setPosition(Screen::Pos(tPos.x, (vPos.y + 34) + 360));
	friendshipPictureOff.loadFromFile("Assets/Images/Settings/FriendShipPictureOff.png");
	friendshipPictureOn.loadFromFile("Assets/Images/Settings/FriendShipPictureOn.png");
	friendshipPicture = Utils::LoadSprite("Assets/Images/Settings/FriendShipPictureOn.png", CENTER);
	friendshipPicture.setPosition(friendShipMode.getPosition());

	thirdDimentionMode = Utils::LoadSprite("Assets/Images/Settings/Off.png", CENTER);
	thirdDimentionSelected.loadFromFile("Assets/Images/Settings/3DSelected.png");
	thirdDimentionUnSelected.loadFromFile("Assets/Images/Settings/3D.png");
	thirdDimentionName = Utils::LoadSprite("Assets/Images/Settings/3D.png", CENTER);;
	thirdDimentionMode.setPosition(tPos.x, vPos.y + 480);
	thirdDimentionName.setPosition(Screen::Pos(tPos.x, (vPos.y + 34) + 480));
	d3PictureOff.loadFromFile("Assets/Images/Settings/3DPictureOff.png");
	d3PictureOn.loadFromFile("Assets/Images/Settings/3DPictureOn.png");
	d3Picture = Utils::LoadSprite("Assets/Images/Settings/3DPictureOff.png", CENTER);
	d3Picture.setPosition(thirdDimentionMode.getPosition());

	tutoMode = Utils::LoadSprite("Assets/Images/Settings/On.png", CENTER);
	tutoSelected.loadFromFile("Assets/Images/Settings/TutoSelected.png");
	tutoUnSelected.loadFromFile("Assets/Images/Settings/Tuto.png");
	tutoName = Utils::LoadSprite("Assets/Images/Settings/Tuto.png", CENTER);;
	tutoMode.setPosition(tPos.x, vPos.y + 600);
	tutoName.setPosition(Screen::Pos(tPos.x, (vPos.y + 34) + 600));
	tutoPictureOff.loadFromFile("Assets/Images/Settings/TutoPictureOff.png");
	tutoPictureOn.loadFromFile("Assets/Images/Settings/TutoPictureOn.png");
	tutoPicture = Utils::LoadSprite("Assets/Images/Settings/TutoPictureOn.png", CENTER);
	tutoPicture.setPosition(tutoName.getPosition());

	controls = Utils::LoadSprite("Assets/Images/Hud/ControlsP.png", CENTER);

	sf::Vector2f oSize = (sf::Vector2f)hpBarMode.getTexture()->getSize();

	controls.setPosition(tPos);

	buttonVect.push_back(new Button("Assets/Images/Settings/MinusButton.png", Screen::Pos(vPos.x - vSize.x / 2 - 14, vPos.y), 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/PlusButton.png", Screen::Pos(vPos.x + vSize.x / 2 + 15, vPos.y), 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/MinusButton.png", Screen::Pos(vPos.x - oSize.x / 2 - 14, vPos.y + 120), 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/PlusButton.png", Screen::Pos(vPos.x + oSize.x / 2 + 15, vPos.y + 120), 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/MinusButton.png", Screen::Pos(vPos.x - oSize.x / 2 - 14, vPos.y + 240), 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/PlusButton.png", Screen::Pos(vPos.x + oSize.x / 2 + 15, vPos.y + 240), 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/MinusButton.png", Screen::Pos(vPos.x - oSize.x / 2 - 14, vPos.y + 360), 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/PlusButton.png", Screen::Pos(vPos.x + oSize.x / 2 + 15, vPos.y + 360), 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/MinusButton.png", Screen::Pos(vPos.x - oSize.x / 2 - 14, vPos.y + 480), 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/PlusButton.png", Screen::Pos(vPos.x + oSize.x / 2 + 15, vPos.y + 480), 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/MinusButton.png", Screen::Pos(vPos.x - oSize.x / 2 - 14, vPos.y + 600), 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/PlusButton.png", Screen::Pos(vPos.x + oSize.x / 2 + 15, vPos.y + 600), 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Hud/BackButton.png", { tPos.x + 300, tPos.y + tSize.y / 2 }, 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/ResetButton.png", { tPos.x - 300, tPos.y + tSize.y / 2 - 75 }, 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/ApplyButton.png", { tPos.x + 300, tPos.y + tSize.y / 2 - 75 }, 3, Screen::Scale(1, 1), false));
	buttonVect.push_back(new Button("Assets/Images/Settings/ControlsButton.png", { tPos.x - 300, tPos.y + tSize.y / 2 }, 3, Screen::Scale(1, 1), false));
}

void Settings::InitTexts(void)
{
	textFont.loadFromFile("Assets/Fonts/RulesFont.otf");
	Utils::CreateText(volumeText, textFont, "50", 30, sf::Color::Black, true);
	volumeText.setPosition(volumeBar.getPosition().x, volumeBar.getPosition().y - 10);
}

void Settings::InitVolumeRect(GameData& _gameData)
{
	volumeBar = Utils::LoadSprite("Assets/Images/Settings/VolumeBar.png", CENTER);
	volumeBarSelected.loadFromFile("Assets/Images/Settings/VolumeSelected.png");
	volumeBarUnSelected.loadFromFile("Assets/Images/Settings/Volume.png");
	volumeBarName = Utils::LoadSprite("Assets/Images/Settings/VolumeSelected.png", CENTER);

	transparentSprite = Utils::LoadSprite("Assets/Images/Settings/Background.png", CENTER);
	transparentSprite.setScale(1, 1.25f);
	transparentSprite.setPosition(WORKING_WIDTH / 2, WORKING_HEIGHT / 2+ GAP);

	sf::Vector2f tPos = transparentSprite.getPosition();
	sf::Vector2f tSize = (sf::Vector2f)transparentSprite.getTexture()->getSize();

	volumeBar.setPosition(Screen::Pos(tPos.x, tPos.y - tSize.y / 2 - 25));

	sf::Vector2f vPos = volumeBar.getPosition();
	sf::Vector2f vSize = (sf::Vector2f)volumeBar.getTexture()->getSize();

	volumeBarName.setPosition(Screen::Pos(tPos.x, vPos.y + 34.f));

	volumeRect = Utils::LoadSprite("Assets/Images/Settings/FrontVolumeBar.png", TOP_LEFT);
	volumeRect.setPosition(Screen::Pos(vPos.x - vSize.x / 2 + 3, vPos.y - vSize.y / 2 + 3));
	volumeRect.setScale(Screen::Pos(459.f, 1.f));
	originalRectSize = Screen::Pos(459.f, 1.f);
	volumeRect.setScale(sf::Vector2f(_gameData.audio->GetSoundMultiplier() * originalRectSize.x, originalRectSize.y));
}

void Settings::PlusSound(GameData& _gameData)
{
	_gameData.audio->ModifySoundMultiplier(0.01f);
	if (_gameData.audio->GetSoundMultiplier() >= 1)
	{
		_gameData.audio->SetSoundMultiplier(1);
	}
	Utils::SetCenteredString(volumeText, Utils::ToString((int)(_gameData.audio->GetSoundMultiplier() * 100)));
	volumeRect.setScale(sf::Vector2f(_gameData.audio->GetSoundMultiplier() * originalRectSize.x, volumeRect.getScale().y));
	buttonVect[(int)SettingsButton::MINUS]->SetIsSelected(false, _gameData);

	if (volumeText.getString() == "100")
	{
		buttonVect[(int)SettingsButton::PLUS]->SetIsSelected(true, _gameData);
		buttonVect[(int)SettingsButton::PLUS]->isPressedFinished = true;
	}
	else
	{
		buttonVect[(int)SettingsButton::PLUS]->SetIsPressed(true, _gameData);
	}
	timerVolumeBar = 0.1f;
}

void Settings::MinusSound(GameData& _gameData)
{
	_gameData.audio->ModifySoundMultiplier(-0.01f);
	if (_gameData.audio->GetSoundMultiplier() <= 0)
	{
		_gameData.audio->SetSoundMultiplier(0);
	}
	Utils::SetCenteredString(volumeText, Utils::ToString((int)(_gameData.audio->GetSoundMultiplier() * 100)));
	volumeRect.setScale(sf::Vector2f(_gameData.audio->GetSoundMultiplier() * originalRectSize.x, volumeRect.getScale().y));
	buttonVect[(int)SettingsButton::PLUS]->SetIsSelected(false, _gameData);

	if (volumeText.getString() == "0")
	{
		buttonVect[(int)SettingsButton::MINUS]->SetIsSelected(true, _gameData);
		buttonVect[(int)SettingsButton::MINUS]->isPressedFinished = true;
	}
	else
	{
		buttonVect[(int)SettingsButton::MINUS]->SetIsPressed(true, _gameData);
	}
	timerVolumeBar = 0.1f;
}

void Settings::UpdateVolume(GameData& _gameData)
{
	if (currentTypeselected == TypeSelected::VOLUME)
	{
		timerVolumeBar -= _gameData.dt;
		if (timerVolumeBar <= 0)
		{
			if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
			{
				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::R1))
				{
					PlusSound(_gameData);
				}
				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::L1))
				{
					MinusSound(_gameData);
				}
			}
			else
			{
				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::RB))
				{
					PlusSound(_gameData);
				}
				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::LB))
				{
					MinusSound(_gameData);
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::LEFT_JOYSTICK_X) > 15 || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::DIR_CROSS_X) > 15)
			{
				PlusSound(_gameData);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::LEFT_JOYSTICK_X) < -15 || sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::DIR_CROSS_X) < -15)
			{
				MinusSound(_gameData);
			}
		}
	}
}

void Settings::ReCreateWindow(sf::RenderWindow& _window, int& _currentDisplayMode, int _displayMode)
{
	if (_currentDisplayMode != _displayMode)
	{
		_currentDisplayMode = _displayMode;
		_window.close();
		_window.create(sf::VideoMode::getDesktopMode(), "Ecocalypse Knights", _currentDisplayMode);
		_window.setFramerateLimit(144);
		_window.setMouseCursorVisible(false);

		sf::Image icon;
		icon.loadFromFile("Assets/Images/Hud/Red_Head.png");
		_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	}
}

void Settings::UpdateButtonPressedEffect(GameData& _gameData)
{
	timerButton -= TIMER_WAIT_BUTTON;
	if (timerButton <= 0.f)
	{
		if (showControls)
		{
			if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
			{
				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CIRCLE))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CROSS))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::L1))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::L2))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::LEFT_STICK_PUSH))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::R1))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::R2))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::RIGHT_STICK_PUSH))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::SQUARE))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::TRIANGLE))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}
			}
			else
			{
				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::A))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::B))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::LB))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::LEFT_STICK_PUSH))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::RB))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}
				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::RIGHT_STICK_PUSH))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::SELECT))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::X))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}

				if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::Y))
				{
					showControls = false;
					timerButton = TIMER_WAIT_BUTTON;
				}
			}
		}

		if (buttonVect[(int)SettingsButton::APPLY]->isPressedFinished)
		{
			switch (displayTypes)
			{
			case DisplayTypes::WINDOWED:
				ReCreateWindow(_gameData.window, _gameData.displayMode, sf::Style::Default);
				break;
			case DisplayTypes::BORDERLESS:
				ReCreateWindow(_gameData.window, _gameData.displayMode, sf::Style::None);
				break;
			case DisplayTypes::FULLSCREEN:
				ReCreateWindow(_gameData.window, _gameData.displayMode, sf::Style::Fullscreen);
				break;
			}

			_gameData.showMobHpBar = (hpBarType == HpBarTypes::ON);
			_gameData.friendShip = (friendShipType == FriendShipTypes::ON);
			_gameData.is3DOn = (thirdDimentionType == ThirdDimentionTypes::ON);
			_gameData.TutoActive = (tutoType == TutoTypes::ON);

			buttonVect[(int)SettingsButton::APPLY]->SetIsPressed(false, _gameData);
			buttonVect[(int)SettingsButton::APPLY]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::APPLY]->isPressedFinished = false;
		}

		else if (buttonVect[(int)SettingsButton::RESET]->isPressedFinished)
		{

			if (_gameData.displayMode != sf::Style::Default)
			{
				ReCreateWindow(_gameData.window, _gameData.displayMode, sf::Style::Default);
				displayType.setTexture(windowedType);
			}

			_gameData.showMobHpBar = false;
			hpBarMode.setTexture(optionOff);
			_gameData.friendShip = true;
			friendShipMode.setTexture(optionOn);
			_gameData.is3DOn = false;
			thirdDimentionMode.setTexture(optionOff);
			_gameData.TutoActive = true;
			tutoMode.setTexture(optionOn);

			buttonVect[(int)SettingsButton::RESET]->SetIsPressed(false, _gameData);
			buttonVect[(int)SettingsButton::RESET]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::RESET]->isPressedFinished = false;
			buttonVect[(int)SettingsButton::PLUS3]->SetIsSelected(false, _gameData);
			buttonVect[(int)SettingsButton::PLUS2]->SetIsSelected(false, _gameData);
			buttonVect[(int)SettingsButton::PLUS4]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::PLUS5]->SetIsSelected(false, _gameData);
			buttonVect[(int)SettingsButton::PLUS6]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::MINUS3]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::MINUS2]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::MINUS4]->SetIsSelected(false, _gameData);
			buttonVect[(int)SettingsButton::MINUS5]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::MINUS6]->SetIsSelected(false, _gameData);
		}

		else if (buttonVect[(int)SettingsButton::QUIT]->isPressedFinished)
		{
			if (_gameData.lastState == MENU)
			{
				_gameData.ChangeState(MENU);
			}
			else if (_gameData.lastState == PAUSE)
			{
				_gameData.camera->SetLastCenter();
				_gameData.camera->SetLastZoom();

				(bool)(_gameData.arenaIsChose) ? _gameData.lastState = ARENA : _gameData.lastState = GAME;
				_gameData.gameState = PAUSE;
			}

			(bool)(_gameData.showMobHpBar) ? hpBarMode.setTexture(optionOn) : hpBarMode.setTexture(optionOff);
			(bool)(_gameData.showMobHpBar) ? hpBarPicture.setTexture(hpBarPictureOn) : hpBarPicture.setTexture(hpBarPictureOff);


			(bool)(_gameData.friendShip) ? friendShipMode.setTexture(optionOn) : friendShipMode.setTexture(optionOff);
			(bool)(_gameData.friendShip) ? friendshipPicture.setTexture(friendshipPictureOn) : friendshipPicture.setTexture(friendshipPictureOff);

			(bool)(_gameData.is3DOn) ? thirdDimentionMode.setTexture(optionOn) : thirdDimentionMode.setTexture(optionOff);
			(bool)(_gameData.is3DOn) ? d3Picture.setTexture(d3PictureOn) : d3Picture.setTexture(d3PictureOn);

			(bool)(_gameData.TutoActive) ? tutoMode.setTexture(optionOn) : tutoMode.setTexture(optionOff);
			(bool)(_gameData.TutoActive) ? tutoPicture.setTexture(tutoPictureOn) : tutoPicture.setTexture(tutoPictureOff);

			buttonVect[(int)SettingsButton::QUIT]->SetIsPressed(false, _gameData);
			buttonVect[(int)SettingsButton::QUIT]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::QUIT]->isPressedFinished = false;
		}

		else if (buttonVect[(int)SettingsButton::CONTROLS]->isPressedFinished)
		{
			showControls = true;
			buttonVect[(int)SettingsButton::CONTROLS]->SetIsPressed(false, _gameData);
			buttonVect[(int)SettingsButton::CONTROLS]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::CONTROLS]->isPressedFinished = false;
			timerButton = TIMER_WAIT_BUTTON;
		}

		else if (buttonVect[(int)SettingsButton::MINUS]->isPressedFinished)
		{
			buttonVect[(int)SettingsButton::MINUS]->isPressedFinished = false;
		}
		else if (buttonVect[(int)SettingsButton::MINUS2]->isPressedFinished)
		{
			buttonVect[(int)SettingsButton::MINUS2]->isPressedFinished = false;
		}
		else if (buttonVect[(int)SettingsButton::MINUS3]->isPressedFinished)
		{
			buttonVect[(int)SettingsButton::MINUS3]->isPressedFinished = false;
		}
		else if (buttonVect[(int)SettingsButton::MINUS4]->isPressedFinished)
		{
			buttonVect[(int)SettingsButton::MINUS4]->isPressedFinished = false;
		}
		else if (buttonVect[(int)SettingsButton::MINUS5]->isPressedFinished)
		{
			buttonVect[(int)SettingsButton::MINUS5]->isPressedFinished = false;
		}
		else if (buttonVect[(int)SettingsButton::MINUS6]->isPressedFinished)
		{
			buttonVect[(int)SettingsButton::MINUS6]->isPressedFinished = false;
		}

		else if (buttonVect[(int)SettingsButton::PLUS]->isPressedFinished)
		{
			buttonVect[(int)SettingsButton::PLUS]->isPressedFinished = false;
		}
		else if (buttonVect[(int)SettingsButton::PLUS2]->isPressedFinished)
		{
			buttonVect[(int)SettingsButton::PLUS2]->isPressedFinished = false;
		}
		else if (buttonVect[(int)SettingsButton::PLUS3]->isPressedFinished)
		{
			buttonVect[(int)SettingsButton::PLUS3]->isPressedFinished = false;
		}
		else if (buttonVect[(int)SettingsButton::PLUS4]->isPressedFinished)
		{
			buttonVect[(int)SettingsButton::PLUS4]->isPressedFinished = false;
		}
		else if (buttonVect[(int)SettingsButton::PLUS5]->isPressedFinished)
		{
			buttonVect[(int)SettingsButton::PLUS5]->isPressedFinished = false;
		}
		else if (buttonVect[(int)SettingsButton::PLUS6]->isPressedFinished)
		{
			buttonVect[(int)SettingsButton::PLUS6]->isPressedFinished = false;
		}
	}
}

void Settings::UpdateAnim(GameData& _gameData)
{
	bool temp = true;
	_gameData.animFlame->UpdateLeftToRight(_gameData.flame, _gameData.dt, 9, 0, temp, true);
}

void Settings::Update(GameData& _gameData)
{
	
	UpdateVolume(_gameData);
	MovingInSettings(_gameData, true);
	UpdateButtonPressedEffect(_gameData);
	UpdateAllButtonsAnim(_gameData.dt);
	UpdateAnim(_gameData);
}

void Settings::Display(GameData& _gameData)
{
	_gameData.window.setView(_gameData.camera->GetView());
	_gameData.window.draw(backgroundSprite);
	_gameData.window.draw(_gameData.flame);
	_gameData.window.draw(transparentSprite);

	if (!showControls)
	{
		_gameData.window.draw(volumeBar);
		_gameData.window.draw(volumeRect);
		_gameData.window.draw(volumeBarName);
		_gameData.window.draw(volumeText);

		_gameData.window.draw(displayType);
		_gameData.window.draw(displayModeName);

		_gameData.window.draw(hpBarMode);
		_gameData.window.draw(hpBarName);

		_gameData.window.draw(friendShipMode);
		_gameData.window.draw(friendShipName);

		_gameData.window.draw(thirdDimentionMode);
		_gameData.window.draw(thirdDimentionName);

		_gameData.window.draw(tutoMode);
		_gameData.window.draw(tutoName);


		if (typeSelected[(int)TypeSelected::HP_BAR])
		{
			_gameData.window.draw(hpBarPicture);
		}
		else if (typeSelected[(int)TypeSelected::FRIENDLY_FIRE])
		{
			_gameData.window.draw(friendshipPicture);
		}
		else if (typeSelected[(int)TypeSelected::THIRD_DIMENTION])
		{
			_gameData.window.draw(d3Picture);
		}
		else if (typeSelected[(int)TypeSelected::TUTO])
		{
			_gameData.window.draw(tutoPicture);
		}

		DisplayButtons(_gameData.window);
	}
	else if (showControls)
	{
		_gameData.window.draw(controls);
	}
}

void Settings::Display3D(GameData& _gameData)
{
	_gameData.renderTextureRed.setView(_gameData.camera->GetView());
	_gameData.renderTextureBlue.setView(_gameData.camera->GetView());

	Utils::BlitSprite(backgroundSprite, sf::Vector2f{ backgroundSprite.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),backgroundSprite.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(backgroundSprite, sf::Vector2f{ backgroundSprite.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),backgroundSprite.getPosition().y }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(_gameData.flame, sf::Vector2f{ _gameData.flame.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),_gameData.flame.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(_gameData.flame, sf::Vector2f{ _gameData.flame.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),_gameData.flame.getPosition().y }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(transparentSprite, sf::Vector2f{ transparentSprite.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),transparentSprite.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(transparentSprite, sf::Vector2f{ transparentSprite.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),transparentSprite.getPosition().y }, 0, _gameData.renderTextureBlue);

	if (!showControls)
	{
		Utils::BlitSprite(volumeBar, sf::Vector2f{ volumeBar.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),volumeBar.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(volumeBar, sf::Vector2f{ volumeBar.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),volumeBar.getPosition().y }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(volumeRect, sf::Vector2f{ volumeRect.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),volumeRect.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(volumeRect, sf::Vector2f{ volumeRect.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),volumeRect.getPosition().y }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(volumeBarName, sf::Vector2f{ volumeBarName.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),volumeBarName.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(volumeBarName, sf::Vector2f{ volumeBarName.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),volumeBarName.getPosition().y }, 0, _gameData.renderTextureBlue);

		sf::Vector2f posTexte = volumeText.getPosition();

		volumeText.setPosition(sf::Vector2f{ posTexte.x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),posTexte.y });
		_gameData.renderTextureRed.draw(volumeText);

		volumeText.setPosition(sf::Vector2f{ posTexte.x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),posTexte.y });
		_gameData.renderTextureBlue.draw(volumeText);

		volumeText.setPosition(posTexte);

		Utils::BlitSprite(displayType, sf::Vector2f{ displayType.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),displayType.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(displayType, sf::Vector2f{ displayType.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),displayType.getPosition().y }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(displayModeName, sf::Vector2f{ displayModeName.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),displayModeName.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(displayModeName, sf::Vector2f{ displayModeName.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),displayModeName.getPosition().y }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(hpBarMode, sf::Vector2f{ hpBarMode.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),hpBarMode.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(hpBarMode, sf::Vector2f{ hpBarMode.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),hpBarMode.getPosition().y }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(hpBarName, sf::Vector2f{ hpBarName.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),hpBarName.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(hpBarName, sf::Vector2f{ hpBarName.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),hpBarName.getPosition().y }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(friendShipMode, sf::Vector2f{ friendShipMode.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),friendShipMode.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(friendShipMode, sf::Vector2f{ friendShipMode.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),friendShipMode.getPosition().y }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(friendShipName, sf::Vector2f{ friendShipName.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),friendShipName.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(friendShipName, sf::Vector2f{ friendShipName.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),friendShipName.getPosition().y }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(thirdDimentionMode, sf::Vector2f{ thirdDimentionMode.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),thirdDimentionMode.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(thirdDimentionMode, sf::Vector2f{ thirdDimentionMode.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),thirdDimentionMode.getPosition().y }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(thirdDimentionName, sf::Vector2f{ thirdDimentionName.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),thirdDimentionName.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(thirdDimentionName, sf::Vector2f{ thirdDimentionName.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),thirdDimentionName.getPosition().y }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(tutoMode, sf::Vector2f{ tutoMode.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),tutoMode.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(tutoMode, sf::Vector2f{ tutoMode.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),tutoMode.getPosition().y }, 0, _gameData.renderTextureBlue);

		Utils::BlitSprite(tutoName, sf::Vector2f{ tutoName.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),tutoName.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(tutoName, sf::Vector2f{ tutoName.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),tutoName.getPosition().y }, 0, _gameData.renderTextureBlue);

		if (typeSelected[(int)TypeSelected::HP_BAR])
		{
			Utils::BlitSprite(hpBarPicture, sf::Vector2f{ hpBarPicture.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),hpBarPicture.getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(hpBarPicture, sf::Vector2f{ hpBarPicture.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),hpBarPicture.getPosition().y }, 0, _gameData.renderTextureBlue);
		}
		else if (typeSelected[(int)TypeSelected::FRIENDLY_FIRE])
		{
			Utils::BlitSprite(friendshipPicture, sf::Vector2f{ friendshipPicture.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),friendshipPicture.getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(friendshipPicture, sf::Vector2f{ friendshipPicture.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),friendshipPicture.getPosition().y }, 0, _gameData.renderTextureBlue);
		}
		else if (typeSelected[(int)TypeSelected::THIRD_DIMENTION])
		{
			Utils::BlitSprite(d3Picture, sf::Vector2f{ d3Picture.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),d3Picture.getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(d3Picture, sf::Vector2f{ d3Picture.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),d3Picture.getPosition().y }, 0, _gameData.renderTextureBlue);
		}
		else if (typeSelected[(int)TypeSelected::TUTO])
		{
			Utils::BlitSprite(tutoPicture, sf::Vector2f{ tutoPicture.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),tutoPicture.getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(tutoPicture, sf::Vector2f{ tutoPicture.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),tutoPicture.getPosition().y }, 0, _gameData.renderTextureBlue);
		}

		DisplayButtons3D(_gameData.renderTextureRed, _gameData.renderTextureBlue);
	}
	else if (showControls)
	{
		Utils::BlitSprite(controls, sf::Vector2f{ controls.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),controls.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(controls, sf::Vector2f{ controls.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),controls.getPosition().y }, 0, _gameData.renderTextureBlue);
	}
}

void Settings::ChangeButtonSelected(GameData& _gameData, TypeSelected _currentType, TypeSelected _newType, bool _currentIsAButton, bool _newIsAButton, SettingsButton _newButtonType, sf::Sprite& _currentSpriteSelected, sf::Sprite& _newSpriteSelected, sf::Texture& _textureActived, sf::Texture& _textureDesactived)
{
	typeSelected[(int)_newType] = true;
	currentTypeselected = _newType;
	if (_currentIsAButton)
	{
		buttonVect[buttonSelected]->SetIsSelected(false, _gameData);
	}
	else
	{
		_currentSpriteSelected.setTexture(_textureDesactived);
	}
	if (_newIsAButton)
	{
		buttonVect[(int)_newButtonType]->SetIsSelected(true, _gameData);
		buttonSelected = (int)_newButtonType;
	}
	else
	{
		_newSpriteSelected.setTexture(_textureActived);
	}
}

void Settings::MovingUp(GameData& _gameData)
{
	if (!typeSelected[(int)TypeSelected::VOLUME])
	{
		typeSelected[(int)currentTypeselected] = false;

		switch (currentTypeselected)
		{
		case TypeSelected::DISPLAY_MODE:
			ChangeButtonSelected(_gameData, TypeSelected::DISPLAY_MODE, TypeSelected::VOLUME, false, false, (SettingsButton)NULL, displayModeName, volumeBarName, volumeBarSelected, displayModeUnSelected);
			break;
		case TypeSelected::HP_BAR:
			ChangeButtonSelected(_gameData, TypeSelected::HP_BAR, TypeSelected::DISPLAY_MODE, false, false, (SettingsButton)NULL, hpBarName, displayModeName, displayModeSelected, hpBarUnSelected);
			break;
		case TypeSelected::FRIENDLY_FIRE:
			ChangeButtonSelected(_gameData, TypeSelected::FRIENDLY_FIRE, TypeSelected::HP_BAR, false, false, (SettingsButton)NULL, friendShipName, hpBarName, hpBarSelected, friendShipUnSelected);
			break;
		case TypeSelected::THIRD_DIMENTION:
			ChangeButtonSelected(_gameData, TypeSelected::THIRD_DIMENTION, TypeSelected::FRIENDLY_FIRE, false, false, (SettingsButton)NULL, thirdDimentionName, friendShipName, friendShipSelected, thirdDimentionUnSelected);
			break;
		case TypeSelected::TUTO:
			ChangeButtonSelected(_gameData, TypeSelected::TUTO, TypeSelected::THIRD_DIMENTION, false, false, (SettingsButton)NULL, tutoName, thirdDimentionName, thirdDimentionSelected, tutoUnSelected);
			break;
		case TypeSelected::RESET:
			ChangeButtonSelected(_gameData, TypeSelected::RESET, TypeSelected::TUTO, true, false, (SettingsButton)NULL, tutoName, tutoName, tutoSelected, tutoUnSelected);
			break;
		case TypeSelected::APPLY:
			ChangeButtonSelected(_gameData, TypeSelected::APPLY, TypeSelected::TUTO, true, false, (SettingsButton)NULL, tutoName, tutoName, tutoSelected, tutoUnSelected);
			break;
		case TypeSelected::QUIT:
			ChangeButtonSelected(_gameData, TypeSelected::QUIT, TypeSelected::APPLY, true, true, SettingsButton::APPLY, hpBarName, hpBarName, hpBarSelected, hpBarUnSelected);
			break;
		case TypeSelected::CONTROLS:
			ChangeButtonSelected(_gameData, TypeSelected::CONTROLS, TypeSelected::RESET, true, true, SettingsButton::RESET, hpBarName, hpBarName, hpBarSelected, hpBarUnSelected);
			break;
		}
	}
}
void Settings::MovingDown(GameData& _gameData)
{
	if (!typeSelected[(int)TypeSelected::QUIT])
	{
		typeSelected[(int)currentTypeselected] = false;

		switch (currentTypeselected)
		{
		case TypeSelected::VOLUME:
			ChangeButtonSelected(_gameData, TypeSelected::VOLUME, TypeSelected::DISPLAY_MODE, false, false, (SettingsButton)NULL, volumeBarName, displayModeName, displayModeSelected, volumeBarUnSelected);
			break;
		case TypeSelected::DISPLAY_MODE:
			ChangeButtonSelected(_gameData, TypeSelected::DISPLAY_MODE, TypeSelected::HP_BAR, false, false, (SettingsButton)NULL, displayModeName, hpBarName, hpBarSelected, displayModeUnSelected);
			break;
		case TypeSelected::HP_BAR:
			ChangeButtonSelected(_gameData, TypeSelected::HP_BAR, TypeSelected::FRIENDLY_FIRE, false, false, (SettingsButton)NULL, hpBarName, friendShipName, friendShipSelected, hpBarUnSelected);
			break;
		case TypeSelected::FRIENDLY_FIRE:
			ChangeButtonSelected(_gameData, TypeSelected::FRIENDLY_FIRE, TypeSelected::THIRD_DIMENTION, false, false, (SettingsButton)NULL, friendShipName, thirdDimentionName, thirdDimentionSelected, friendShipUnSelected);
			break;
		case TypeSelected::THIRD_DIMENTION:
			ChangeButtonSelected(_gameData, TypeSelected::THIRD_DIMENTION, TypeSelected::TUTO, false, false, SettingsButton::RESET, thirdDimentionName, tutoName, tutoSelected, thirdDimentionUnSelected);
			break;
		case TypeSelected::TUTO:
			if (lastPosIsLeft)
			{
				ChangeButtonSelected(_gameData, TypeSelected::TUTO, TypeSelected::RESET, false, true, SettingsButton::RESET, tutoName, tutoName, tutoSelected, tutoUnSelected);
			}
			else
			{
				ChangeButtonSelected(_gameData, TypeSelected::TUTO, TypeSelected::APPLY, false, true, SettingsButton::APPLY, tutoName, tutoName, tutoSelected, tutoUnSelected);
			}
			break;
		case TypeSelected::RESET:
			ChangeButtonSelected(_gameData, TypeSelected::RESET, TypeSelected::CONTROLS, true, true, SettingsButton::CONTROLS, hpBarName, hpBarName, hpBarSelected, hpBarUnSelected);
			break;
		case TypeSelected::APPLY:
			ChangeButtonSelected(_gameData, TypeSelected::APPLY, TypeSelected::QUIT, true, true, SettingsButton::QUIT, hpBarName, hpBarName, hpBarSelected, hpBarUnSelected);
			break;
		}
	}
}
void Settings::MovingRight(GameData& _gameData)
{
	switch (currentTypeselected)
	{
	case TypeSelected::DISPLAY_MODE:
		if (displayTypes != DisplayTypes::FULLSCREEN)
		{
			if (displayTypes == DisplayTypes::WINDOWED)
			{
				displayTypes = DisplayTypes::BORDERLESS;
				displayType.setTexture(borderlessType);
				buttonVect[(int)SettingsButton::PLUS2]->SetIsPressed(true, _gameData);
			}
			else if (displayTypes == DisplayTypes::BORDERLESS)
			{
				displayTypes = DisplayTypes::FULLSCREEN;
				displayType.setTexture(fullscreenType);
				buttonVect[(int)SettingsButton::PLUS2]->SetIsSelected(true, _gameData);
				buttonVect[(int)SettingsButton::PLUS2]->isPressedFinished = true;
			}
			buttonVect[(int)SettingsButton::MINUS2]->SetIsSelected(false, _gameData);
		}
		break;
	case TypeSelected::HP_BAR:
		if (hpBarType == HpBarTypes::OFF)
		{
			hpBarType = HpBarTypes::ON;
			hpBarMode.setTexture(optionOn);
			hpBarPicture.setTexture(hpBarPictureOn);
			buttonVect[(int)SettingsButton::PLUS3]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::PLUS3]->isPressedFinished = true;
			buttonVect[(int)SettingsButton::MINUS3]->SetIsSelected(false, _gameData);
		}
		break;
	case TypeSelected::FRIENDLY_FIRE:
		if (friendShipType == FriendShipTypes::OFF)
		{
			friendShipType = FriendShipTypes::ON;
			friendShipMode.setTexture(optionOn);
			friendshipPicture.setTexture(friendshipPictureOn);
			buttonVect[(int)SettingsButton::PLUS4]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::PLUS4]->isPressedFinished = true;
			buttonVect[(int)SettingsButton::MINUS4]->SetIsSelected(false, _gameData);
		}
		break;
	case TypeSelected::THIRD_DIMENTION:
		if (thirdDimentionType == ThirdDimentionTypes::OFF)
		{
			thirdDimentionType = ThirdDimentionTypes::ON;
			thirdDimentionMode.setTexture(optionOn);
			d3Picture.setTexture(d3PictureOn);
			buttonVect[(int)SettingsButton::PLUS5]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::PLUS5]->isPressedFinished = true;
			buttonVect[(int)SettingsButton::MINUS5]->SetIsSelected(false, _gameData);
		}
		break;
	case TypeSelected::TUTO:
		if (tutoType == TutoTypes::OFF)
		{
			tutoType = TutoTypes::ON;
			tutoMode.setTexture(optionOn);
			tutoPicture.setTexture(tutoPictureOn);
			buttonVect[(int)SettingsButton::PLUS6]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::PLUS6]->isPressedFinished = true;
			buttonVect[(int)SettingsButton::MINUS6]->SetIsSelected(false, _gameData);
		}
		break;
	case TypeSelected::RESET:
		lastPosIsLeft = false;
		typeSelected[(int)TypeSelected::RESET] = false;
		ChangeButtonSelected(_gameData, TypeSelected::RESET, TypeSelected::APPLY, true, true, SettingsButton::APPLY, hpBarName, hpBarName, hpBarSelected, hpBarUnSelected);
		break;
	case TypeSelected::CONTROLS:
		lastPosIsLeft = false;
		typeSelected[(int)TypeSelected::CONTROLS] = false;
		ChangeButtonSelected(_gameData, TypeSelected::CONTROLS, TypeSelected::QUIT, true, true, SettingsButton::QUIT, hpBarName, hpBarName, hpBarSelected, hpBarUnSelected);
		break;
	}
}
void Settings::MovingLeft(GameData& _gameData)
{
	switch (currentTypeselected)
	{
	case TypeSelected::DISPLAY_MODE:
		if (displayTypes != DisplayTypes::WINDOWED)
		{
			if (displayTypes == DisplayTypes::FULLSCREEN)
			{
				displayTypes = DisplayTypes::BORDERLESS;
				displayType.setTexture(borderlessType);
				buttonVect[(int)SettingsButton::MINUS2]->SetIsPressed(true, _gameData);
			}

			else if (displayTypes == DisplayTypes::BORDERLESS)
			{
				displayTypes = DisplayTypes::WINDOWED;
				displayType.setTexture(windowedType);
				buttonVect[(int)SettingsButton::MINUS2]->SetIsSelected(true, _gameData);
				buttonVect[(int)SettingsButton::MINUS2]->isPressedFinished = true;
			}

			buttonVect[(int)SettingsButton::PLUS2]->SetIsSelected(false, _gameData);
		}
		break;
	case TypeSelected::HP_BAR:
		if (hpBarType == HpBarTypes::ON)
		{
			hpBarType = HpBarTypes::OFF;
			hpBarMode.setTexture(optionOff);
			hpBarPicture.setTexture(hpBarPictureOff);
			buttonVect[(int)SettingsButton::MINUS3]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::MINUS3]->isPressedFinished = true;
			buttonVect[(int)SettingsButton::PLUS3]->SetIsSelected(false, _gameData);
		}
		break;
	case TypeSelected::FRIENDLY_FIRE:
		if (friendShipType == FriendShipTypes::ON)
		{
			friendShipType = FriendShipTypes::OFF;
			friendShipMode.setTexture(optionOff);
			friendshipPicture.setTexture(friendshipPictureOff);
			buttonVect[(int)SettingsButton::MINUS4]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::MINUS4]->isPressedFinished = true;
			buttonVect[(int)SettingsButton::PLUS4]->SetIsSelected(false, _gameData);
		}
		break;
	case TypeSelected::THIRD_DIMENTION:
		if (thirdDimentionType == ThirdDimentionTypes::ON)
		{
			thirdDimentionType = ThirdDimentionTypes::OFF;
			thirdDimentionMode.setTexture(optionOff);
			d3Picture.setTexture(d3PictureOff);
			buttonVect[(int)SettingsButton::MINUS5]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::MINUS5]->isPressedFinished = true;
			buttonVect[(int)SettingsButton::PLUS5]->SetIsSelected(false, _gameData);
		}
		break;
	case TypeSelected::TUTO:
		if (tutoType == TutoTypes::ON)
		{
			tutoType = TutoTypes::OFF;
			tutoMode.setTexture(optionOff);
			tutoPicture.setTexture(tutoPictureOff);
			buttonVect[(int)SettingsButton::MINUS6]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::MINUS6]->isPressedFinished = true;
			buttonVect[(int)SettingsButton::PLUS6]->SetIsSelected(false, _gameData);
		}
		break;
	case TypeSelected::APPLY:
		lastPosIsLeft = true;
		typeSelected[(int)TypeSelected::APPLY] = false;
		ChangeButtonSelected(_gameData, TypeSelected::APPLY, TypeSelected::RESET, true, true, SettingsButton::RESET, hpBarName, hpBarName, hpBarSelected, hpBarUnSelected);
		break;
	case TypeSelected::QUIT:
		lastPosIsLeft = true;
		typeSelected[(int)TypeSelected::QUIT] = false;
		ChangeButtonSelected(_gameData, TypeSelected::QUIT, TypeSelected::CONTROLS, true, true, SettingsButton::CONTROLS, hpBarName, hpBarName, hpBarSelected, hpBarUnSelected);
		break;
	}

}

void Settings::Right1ButtonController(GameData& _gameData)
{
	if (typeSelected[(int)TypeSelected::VOLUME])
	{
		PlusSound(_gameData);
	}
	else if (displayTypes != DisplayTypes::FULLSCREEN && typeSelected[(int)TypeSelected::DISPLAY_MODE])
	{
		if (displayTypes == DisplayTypes::WINDOWED)
		{
			displayTypes = DisplayTypes::BORDERLESS;
			displayType.setTexture(borderlessType);
			buttonVect[(int)SettingsButton::PLUS2]->SetIsPressed(true, _gameData);
			buttonVect[(int)SettingsButton::PLUS2]->isPressedFinished = true;
		}

		else if (displayTypes == DisplayTypes::BORDERLESS)
		{
			displayTypes = DisplayTypes::FULLSCREEN;
			displayType.setTexture(fullscreenType);
			buttonVect[(int)SettingsButton::PLUS2]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::PLUS2]->isPressedFinished = true;
		}
		buttonVect[(int)SettingsButton::MINUS2]->SetIsSelected(false, _gameData);
	}
	else if (hpBarType == HpBarTypes::OFF && typeSelected[(int)TypeSelected::HP_BAR])
	{
		hpBarType = HpBarTypes::ON;
		hpBarMode.setTexture(optionOn);
		hpBarPicture.setTexture(hpBarPictureOn);
		buttonVect[(int)SettingsButton::PLUS3]->SetIsSelected(true, _gameData);
		buttonVect[(int)SettingsButton::PLUS3]->isPressedFinished = true;
		buttonVect[(int)SettingsButton::MINUS3]->SetIsSelected(false, _gameData);
	}
	else if (friendShipType == FriendShipTypes::OFF && typeSelected[(int)TypeSelected::FRIENDLY_FIRE])
	{
		friendShipType = FriendShipTypes::ON;
		friendShipMode.setTexture(optionOn);
		friendshipPicture.setTexture(friendshipPictureOn);
		buttonVect[(int)SettingsButton::PLUS4]->SetIsSelected(true, _gameData);
		buttonVect[(int)SettingsButton::PLUS4]->isPressedFinished = true;
		buttonVect[(int)SettingsButton::MINUS4]->SetIsSelected(false, _gameData);
	}
	else if (thirdDimentionType == ThirdDimentionTypes::OFF && typeSelected[(int)TypeSelected::THIRD_DIMENTION])
	{
		thirdDimentionType = ThirdDimentionTypes::ON;
		thirdDimentionMode.setTexture(optionOn);
		d3Picture.setTexture(d3PictureOn);
		buttonVect[(int)SettingsButton::PLUS5]->SetIsSelected(true, _gameData);
		buttonVect[(int)SettingsButton::PLUS5]->isPressedFinished = true;
		buttonVect[(int)SettingsButton::MINUS5]->SetIsSelected(false, _gameData);
	}
	else if (tutoType == TutoTypes::OFF && typeSelected[(int)TypeSelected::TUTO])
	{
		tutoType = TutoTypes::ON;
		tutoMode.setTexture(optionOn);
		tutoPicture.setTexture(tutoPictureOn);
		buttonVect[(int)SettingsButton::PLUS6]->SetIsSelected(true, _gameData);
		buttonVect[(int)SettingsButton::PLUS6]->isPressedFinished = true;
		buttonVect[(int)SettingsButton::MINUS6]->SetIsSelected(false, _gameData);
	}
}
void Settings::Left1ButtonController(GameData& _gameData)
{
	if (typeSelected[(int)TypeSelected::VOLUME])
	{
		MinusSound(_gameData);
	}
	else if (displayTypes != DisplayTypes::WINDOWED && typeSelected[(int)TypeSelected::DISPLAY_MODE])
	{
		if (displayTypes == DisplayTypes::FULLSCREEN)
		{
			displayTypes = DisplayTypes::BORDERLESS;
			displayType.setTexture(borderlessType);
			buttonVect[(int)SettingsButton::MINUS2]->SetIsPressed(true, _gameData);
		}
		else if (displayTypes == DisplayTypes::BORDERLESS)
		{
			displayTypes = DisplayTypes::WINDOWED;
			displayType.setTexture(windowedType);
			buttonVect[(int)SettingsButton::MINUS2]->SetIsSelected(true, _gameData);
			buttonVect[(int)SettingsButton::MINUS2]->isPressedFinished;
		}
		buttonVect[(int)SettingsButton::PLUS2]->SetIsSelected(false, _gameData);
	}
	else if (hpBarType == HpBarTypes::ON && typeSelected[(int)TypeSelected::HP_BAR])
	{
		hpBarType = HpBarTypes::OFF;
		hpBarMode.setTexture(optionOff);
		hpBarPicture.setTexture(hpBarPictureOff);
		buttonVect[(int)SettingsButton::MINUS3]->SetIsSelected(true, _gameData);
		buttonVect[(int)SettingsButton::MINUS3]->isPressedFinished;
		buttonVect[(int)SettingsButton::PLUS3]->SetIsSelected(false, _gameData);
	}
	else if (friendShipType == FriendShipTypes::ON && typeSelected[(int)TypeSelected::FRIENDLY_FIRE])
	{
		friendShipType = FriendShipTypes::OFF;
		friendShipMode.setTexture(optionOff);
		friendshipPicture.setTexture(friendshipPictureOff);
		buttonVect[(int)SettingsButton::MINUS4]->SetIsSelected(true, _gameData);
		buttonVect[(int)SettingsButton::MINUS4]->isPressedFinished = true;
		buttonVect[(int)SettingsButton::PLUS4]->SetIsSelected(false, _gameData);
	}
	else if (thirdDimentionType == ThirdDimentionTypes::ON && typeSelected[(int)TypeSelected::THIRD_DIMENTION])
	{
		thirdDimentionType = ThirdDimentionTypes::OFF;
		thirdDimentionMode.setTexture(optionOff);
		d3Picture.setTexture(d3PictureOff);
		buttonVect[(int)SettingsButton::MINUS5]->SetIsSelected(true, _gameData);
		buttonVect[(int)SettingsButton::MINUS5]->isPressedFinished = true;
		buttonVect[(int)SettingsButton::PLUS5]->SetIsSelected(false, _gameData);
	}
	else if (tutoType == TutoTypes::ON && typeSelected[(int)TypeSelected::TUTO])
	{
		tutoType = TutoTypes::OFF;
		tutoMode.setTexture(optionOff);
		tutoPicture.setTexture(tutoPictureOff);
		buttonVect[(int)SettingsButton::MINUS6]->SetIsSelected(true, _gameData);
		buttonVect[(int)SettingsButton::MINUS6]->isPressedFinished = true;
		buttonVect[(int)SettingsButton::PLUS6]->SetIsSelected(false, _gameData);
	}
}

void Settings::MovingInSettings(GameData& _gameData, bool _withController)
{
	if (_withController)
	{
		float yPos1 = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::LEFT_JOYSTICK_Y);
		float xPos1 = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::LEFT_JOYSTICK_X);

		float yPos2 = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::DIR_CROSS_Y);
		float xPos2 = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::DIR_CROSS_X);

		joystickTimer += _gameData.dt;

		if (joystickTimer >= 0.2f)
		{

			if (yPos1 > 15 || yPos2 < -15)
			{
				joystickTimer = 0;
				MovingDown(_gameData);
			}
			else if (yPos1 < -15 || yPos2 > 15)
			{
				joystickTimer = 0;
				MovingUp(_gameData);
			}
			else if ((xPos1 > 15 || xPos2 > 15) && yPos1 < 30 && yPos1 > -30)
			{
				joystickTimer = 0;
				MovingRight(_gameData);
			}
			else if ((xPos1 < -15 || xPos2 < -15) && yPos1 < 30 && yPos1 > -30)
			{
				joystickTimer = 0;
				MovingLeft(_gameData);
			}
		}
	}
	else
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			MovingUp(_gameData);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			MovingDown(_gameData);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			MovingRight(_gameData);
		}

		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			MovingLeft(_gameData);
		}
	}
}

void Settings::KeyPressed(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::KeyPressed)
	{
		if (_gameData.event.key.code == sf::Keyboard::Escape)
		{
			buttonVect[(int)SettingsButton::QUIT]->SetIsPressed(true, _gameData);
		}
		if (_gameData.event.key.code == sf::Keyboard::Enter)
		{
			SetButtonSelectedPressed(_gameData);
		}

		MovingInSettings(_gameData, false);
	}
}

void Settings::JoystickXBOXController(GameData& _gameData)
{
	if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
	{
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CIRCLE) && !showControls)
		{
			buttonVect[(int)SettingsButton::QUIT]->SetIsPressed(true, _gameData);
		}
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CROSS) && (buttonSelected == (int)SettingsButton::QUIT || buttonSelected == (int)SettingsButton::APPLY || buttonSelected == (int)SettingsButton::RESET) || buttonSelected == (int)SettingsButton::CONTROLS)
		{
			if (buttonVect[(int)SettingsButton::RESET]->GetIsSelected() || buttonVect[(int)SettingsButton::APPLY]->GetIsSelected() || buttonVect[(int)SettingsButton::QUIT]->GetIsSelected() || buttonVect[(int)SettingsButton::CONTROLS]->GetIsSelected())
			{
				SetButtonSelectedPressed(_gameData);
				_gameData.audio->PlaySoundNotRepeated("BUTTON_WOOD_CLICK_EFFECT");
			}
		}
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::R1))
		{
			Right1ButtonController(_gameData);
		}
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::L1))
		{
			Left1ButtonController(_gameData);
		}
	}
	else
	{
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::B) && !showControls)
		{
			buttonVect[(int)SettingsButton::QUIT]->SetIsPressed(true, _gameData);
		}
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::A) && (buttonSelected == (int)SettingsButton::QUIT || buttonSelected == (int)SettingsButton::APPLY || buttonSelected == (int)SettingsButton::RESET) || buttonSelected == (int)SettingsButton::CONTROLS)
		{
			if (buttonVect[(int)SettingsButton::RESET]->GetIsSelected() || buttonVect[(int)SettingsButton::APPLY]->GetIsSelected() || buttonVect[(int)SettingsButton::QUIT]->GetIsSelected() || buttonVect[(int)SettingsButton::CONTROLS]->GetIsSelected())
			{
				SetButtonSelectedPressed(_gameData);
				_gameData.audio->PlaySoundNotRepeated("BUTTON_WOOD_CLICK_EFFECT");
			}
		}
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::RB))
		{
			Right1ButtonController(_gameData);
		}
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::LB))
		{
			Left1ButtonController(_gameData);
		}
	}
}

void Settings::JoystickPressed(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::JoystickButtonPressed)
	{
		JoystickXBOXController(_gameData);
	}
}