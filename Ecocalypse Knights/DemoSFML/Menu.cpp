#include "stdafx.h"
#include "Menu.h"
#include "Camera.h"
#include "Network.h"

Menu::Menu(GameData& _gameData)
{
	InitPhase1(_gameData);
	fade = new Fade(2.f, false, true);
}

Menu::~Menu()
{
	delete fade;
	fade = nullptr;
}

void Menu::InitSwordVector(void)
{
	swordsPos.clear();

	for (int i = 0; i < buttonVect.size(); i++)
	{
		if (i % 2 == 0)
		{
			swordsPos.push_back({ buttonVect[i]->posButton.x - 200 + WORKING_WIDTH , buttonVect[i]->posButton.y });
		}
		else
		{
			swordsPos.push_back({ buttonVect[i]->posButton.x - 200 - WORKING_WIDTH, buttonVect[i]->posButton.y });
		}

		endPosEasing.push_back(swordsPos[i]);
		startPosEasing.push_back({ swordsPos[i].x - 30, swordsPos[i].y });
	}
}

void Menu::InitPhase1(GameData& _gameData)
{
	//// Background
	textureBackground.loadFromFile("Assets/Images/BackGround/TitleScreen.png");
	spriteBackground.setTexture(textureBackground);

	//// Title
	textureGameName.loadFromFile("Assets/Images/Menu/Title.png");
	spriteGameName.setTexture(textureGameName);
	spriteGameName.setOrigin((float)textureGameName.getSize().x / 2.f, (float)textureGameName.getSize().y / 2.f);
	spriteGameName.setScale(Screen::Scale(1.f, 1.f));
	spriteGameName.setPosition(Screen::Pos({ 960.f, 250.f + GAP }));

	//// Buttons
	buttonVect.push_back(new Button("Assets/Images/Menu/Story.png", { (WORKING_WIDTH / 2.f) - WORKING_WIDTH, 550.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/Arena.png", { (WORKING_WIDTH / 2.f) + WORKING_WIDTH, 650.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/Settings.png", { (WORKING_WIDTH / 2.f) - WORKING_WIDTH, 750.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/Quit.png", { (WORKING_WIDTH / 2.f) + WORKING_WIDTH, 850.f + GAP }, 7, { 1, 1 }, true));
	timerEasingButton = 0.f;

	//// Sword
	InitSwordVector();

	lastPos = (int)ButtonTypePhase1::STORY;
	nextPos = (int)ButtonTypePhase1::STORY;
	timerEasing = 0.f;
	isSwifting = false;
	timerEasing2 = 0.f;
	movingFront = false;
	_gameData.swordSpriteMenu = Utils::LoadSprite("Assets/Images/Menu/Cursor.png", CENTER);
	_gameData.startPosMenuSwordSprite = swordsPos[0];
	_gameData.swordSpriteMenu.setPosition(_gameData.startPosMenuSwordSprite.x, _gameData.startPosMenuSwordSprite.y + GAP);

	buttonSelected = 0;

	musicKey = "MENU";
	_gameData.audio->AddMusic("Assets/Musics/MenuMusic.ogg", musicKey);
}

void Menu::InitPhase2(sf::Sprite& _sprite)
{
	buttonSelected = 0;
	buttonVect.clear();

	//// Buttons
	buttonVect.push_back(new Button("Assets/Images/Menu/Local.png", { (WORKING_WIDTH / 2.f) - WORKING_WIDTH, 550.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/NetWork.png", { (WORKING_WIDTH / 2.f + 10) + WORKING_WIDTH, 650.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/BackBreak.png", { (WORKING_WIDTH / 2.f) - WORKING_WIDTH, 750.f + GAP }, 7, { 1, 1 }, true));
	timerEasingButton = 0.f;

	//// Sword
	InitSwordVector();
}

void Menu::InitPhase3(void)
{
	buttonSelected = 0;
	buttonVect.clear();

	//// Buttons
	buttonVect.push_back(new Button("Assets/Images/Menu/FreeForAll.png", { (WORKING_WIDTH / 2.f) - WORKING_WIDTH, 550.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/Teamfight.png", { (WORKING_WIDTH / 2.f) + WORKING_WIDTH, 650.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/BackBreak.png", { (WORKING_WIDTH / 2.f) - WORKING_WIDTH, 750.f + GAP }, 7, { 1, 1 }, true));
	timerEasingButton = 0.f;

	//// Sword
	InitSwordVector();
}

void Menu::GoToPhase1(GameData& _gameData)
{
	buttonSelected = 0;

	_gameData.arenaIsChose = false;
	_gameData.teamfight = false;

	buttonVect.clear();

	//// Buttons
	buttonVect.push_back(new Button("Assets/Images/Menu/Story.png", { (WORKING_WIDTH / 2.f) - WORKING_WIDTH, 550.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/Arena.png", { (WORKING_WIDTH / 2.f) + WORKING_WIDTH, 650.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/Settings.png", { (WORKING_WIDTH / 2.f) - WORKING_WIDTH, 750.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/Quit.png", { (WORKING_WIDTH / 2.f) + WORKING_WIDTH, 850.f + GAP }, 7, { 1, 1 }, true));
	timerEasingButton = 0.f;

	//// Sword
	InitSwordVector();
	currentPhase = PhaseType::SWIFTING1_FRONT;
	nextPhase = PhaseType::PHASE1;

}

void Menu::Reload(GameData& _gameData)
{
	buttonSelected = 0;

	delete fade;
	fade = nullptr;

	fade = new Fade(2.f, false, true);

	_gameData.arenaIsChose = false;
	_gameData.teamfight = false;

	buttonVect.clear();

	//// Buttons
	buttonVect.push_back(new Button("Assets/Images/Menu/Story.png", { (WORKING_WIDTH / 2.f) - WORKING_WIDTH, 550.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/Arena.png", { (WORKING_WIDTH / 2.f) + WORKING_WIDTH, 650.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/Settings.png", { (WORKING_WIDTH / 2.f) - WORKING_WIDTH, 750.f + GAP }, 7, { 1, 1 }, true));
	buttonVect.push_back(new Button("Assets/Images/Menu/Quit.png", { (WORKING_WIDTH / 2.f) + WORKING_WIDTH, 850.f + GAP }, 7, { 1, 1 }, true));
	timerEasingButton = 0.f;

	//// Sword
	InitSwordVector();
	currentPhase = PhaseType::SWIFTING1_FRONT;
	nextPhase = PhaseType::PHASE1;
	lastPos = 0;
	nextPos = 0;

	for (int i = 0; i < buttonVect.size(); i++)
	{
		buttonVect[i]->sprite.setColor(sf::Color{ 255,255,255, 255 });
	}
	spriteGameName.setColor(sf::Color{ 255,255,255, 255});
	_gameData.swordSpriteMenu.setColor(sf::Color{ 255,255,255, 255 });
}

void UpdateCamera(GameData& _gameData)
{
	_gameData.camera->UpdateMenu(_gameData);
}

void Menu::UpdateSwordPos(float _dt, sf::Sprite& _swordSprite)
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

void Menu::UpdateAnim(GameData& _gameData)
{
	bool temp = true;
	_gameData.animFlame->UpdateLeftToRight(_gameData.flame, _gameData.dt, 9, 0, temp, true);
}

void Menu::UpdateAlpha(GameData& _gameData)
{
	if (_gameData.camera->GetMoveMenuToPlayerSelection() && buttonVect[0]->sprite.getColor().a > 0)
	{

		int alpha = buttonVect[0]->sprite.getColor().a;
		if (alpha <= 0)
		{
			alpha = 0;
		}
		else
		{
			alpha--;
		}

		for (int i = 0; i < buttonVect.size(); i++)
		{
			buttonVect[i]->sprite.setColor(sf::Color{ 255,255,255, (sf::Uint8)alpha });
		}
		spriteGameName.setColor(sf::Color{ 255,255,255, (sf::Uint8)alpha });
		_gameData.swordSpriteMenu.setColor(sf::Color{ 255,255,255, (sf::Uint8)alpha });
	} 
}

void Menu::Update(GameData& _gameData)
{
	if (_gameData.reloadMenu)
	{
		_gameData.reloadMenu = false;
		Reload(_gameData);
	}

	FadeManagement(_gameData);
	_gameData.audio->FadeOutMusic("LoadingScreen", _gameData.dt);
	_gameData.audio->PlayMusic(musicKey, true);
	if (_gameData.lastState == PAUSE || _gameData.lastState == SETTINGS || _gameData.lastState == CHARACTER_SELECTION)
	{
		SetAlpha(0.f);
	}

	if ((currentPhase == PhaseType::PHASE1 || currentPhase == PhaseType::PHASE2 || currentPhase == PhaseType::PHASE3) && !_gameData.camera->moveMenuToPlayerSelection)
	{
		UpdateButtonsSelection(_gameData);
		UpdateButtonPressedEffect(_gameData);
		UpdateAllButtonsAnim(_gameData.dt);
		DownUpJoysticksButtons(_gameData);
	}
	else
	{
		UpdateThierrySwift(_gameData);
	}

	UpdateCamera(_gameData);
	UpdateAlpha(_gameData);
	if (!_gameData.camera->moveMenuToPlayerSelection)
	{
		UpdateSwordPos(_gameData.dt, _gameData.swordSpriteMenu);
		if (swordHasSwoosh)
		{
			buttonVect[0]->SetIsPressed(false, _gameData);
		}
		swordHasSwoosh = false;
	}

	if (_gameData.camera->GetView().getCenter().y > 1499.f)
	{
		_gameData.audio->StopMusic("LoadingScreen");
		_gameData.ChangeState(CHARACTER_SELECTION);
		_gameData.camera->moveMenuToPlayerSelection = false;
		_gameData.network->isRevealed = true;

		for (int i = 0; i < buttonVect.size(); i++)
		{
			buttonVect[i]->sprite.setColor(sf::Color{ 255,255,255, 255 });
		}
		spriteGameName.setColor(sf::Color{ 255,255,255, 255 });
		_gameData.swordSpriteMenu.setColor(sf::Color{ 255,255,255, 255 });
	}
	UpdateAnim(_gameData);
}

void Menu::UpdateButtonPressedEffectPhase1(GameData& _gameData)
{
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

	if (buttonVect[(int)ButtonTypePhase1::STORY]->isPressedFinished)
	{
		currentPhase = PhaseType::SWIFTING1_BACK;
		nextPhase = PhaseType::PHASE2;
		buttonVect[(int)ButtonTypePhase1::STORY]->isPressedFinished = false;
		timerEasing = 0.f;
		timerEasingButton = 0.f;
	}
	else if (buttonVect[(int)ButtonTypePhase1::ARENA]->isPressedFinished)
	{
		currentPhase = PhaseType::SWIFTING1_BACK;
		nextPhase = PhaseType::PHASE3;
		buttonVect[(int)ButtonTypePhase1::ARENA]->isPressedFinished = false;
		timerEasing = 0.f;
		timerEasingButton = 0.f;
	}
	else if (buttonVect[(int)ButtonTypePhase1::SETTINGS]->isPressedFinished)
	{
		_gameData.ChangeState(SETTINGS);
		_gameData.swordSpriteMenu.setPosition(swordsPos[lastPos]);
		buttonVect[(int)ButtonTypePhase1::SETTINGS]->SetIsPressed(false, _gameData);
		buttonVect[(int)ButtonTypePhase1::SETTINGS]->isPressedFinished = false;
		SetAlpha(255.f);
		timerEasing = 0.f;
		timerEasingButton = 0.f;
	}
	else if (buttonVect[(int)ButtonTypePhase1::QUIT]->isPressedFinished)
	{
		_gameData.window.close();
		buttonVect[(int)ButtonTypePhase1::QUIT]->isPressedFinished = false;
		timerEasing = 0.f;
		timerEasingButton = 0.f;
	}
}

void Menu::UpdateButtonPressedEffectPhase2(GameData& _gameData)
{
	if (buttonVect[buttonSelected]->GetIsPressed() && !swordHasSwoosh)
	{
		if (timerEasing < 1.f)
		{
			if (buttonSelected != (int)ButtonTypePhase2::NETWORK)
			{
				_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR", 100.f);
			}
			else
			{
				_gameData.audio->PlaySoundNotRepeated("SWORD_CURSOR_HINHIN", 100.f);
			}

			swordCurrentPos = Easings::Lerp(swordsPos[lastPos], { swordsPos[lastPos].x + 320 ,swordsPos[lastPos].y }, timerEasing);
			timerEasing += _gameData.dt * 10;
			_gameData.swordSpriteMenu.setPosition(swordCurrentPos);
		}
	}

	if (buttonVect[(int)ButtonTypePhase2::LOCAL]->isPressedFinished)
	{
		_gameData.camera->SetMoveMenuToPlayerSelection(true);
		swordHasSwoosh = true;
		buttonVect[(int)ButtonTypePhase1::STORY]->isPressedFinished = false;
		timerEasing = 0.f;
		timerEasingButton = 0.f;
	}
	else if (buttonVect[(int)ButtonTypePhase2::NETWORK]->isPressedFinished)
	{
		/*_gameData.ChangeState(NETWORK_CONNECTION);*/
		buttonVect[(int)ButtonTypePhase2::NETWORK]->isPressedFinished = false;
		timerEasing = 0.f;
		timerEasingButton = 0.f;
	}
	else if (buttonVect[(int)ButtonTypePhase2::BACK]->isPressedFinished)
	{
		currentPhase = PhaseType::SWIFTING2_BACK;
		nextPhase = PhaseType::PHASE1;
		buttonVect[(int)ButtonTypePhase2::BACK]->isPressedFinished = false;
		timerEasing = 0.f;
		timerEasingButton = 0.f;
	}
}

void Menu::UpdateButtonPressedEffectPhase3(GameData& _gameData)
{
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

	if (buttonVect[(int)ButtonTypePhase3::FREE_FOR_ALL]->isPressedFinished)
	{
		_gameData.camera->SetMoveMenuToPlayerSelection(true);
		swordHasSwoosh = true;
		_gameData.arenaIsChose = true;
		_gameData.teamfight = false;
		buttonVect[(int)ButtonTypePhase3::FREE_FOR_ALL]->isPressedFinished = false;
		timerEasing = 0.f;
		timerEasingButton = 0.f;
	}
	else if (buttonVect[(int)ButtonTypePhase3::TEAM_FIGHT]->isPressedFinished)
	{
		_gameData.camera->SetMoveMenuToPlayerSelection(true);
		swordHasSwoosh = true;
		_gameData.arenaIsChose = true;
		_gameData.teamfight = true;
		buttonVect[(int)ButtonTypePhase3::TEAM_FIGHT]->isPressedFinished = false;
		timerEasing = 0.f;
		timerEasingButton = 0.f;
	}
	else if (buttonVect[(int)ButtonTypePhase3::BACK]->isPressedFinished)
	{
		currentPhase = PhaseType::SWIFTING3_BACK;
		nextPhase = PhaseType::PHASE1;
		buttonVect[(int)ButtonTypePhase3::BACK]->isPressedFinished = false;
		timerEasing = 0.f;
		timerEasingButton = 0.f;
	}
}

void Menu::UpdateButtonPressedEffect(GameData& _gameData)
{
	switch (currentPhase)
	{
	case PhaseType::PHASE1:
		UpdateButtonPressedEffectPhase1(_gameData);
		break;
	case PhaseType::PHASE2:
		UpdateButtonPressedEffectPhase2(_gameData);
		break;
	case PhaseType::PHASE3:
		UpdateButtonPressedEffectPhase3(_gameData);
		break;
	}
}

void Menu::UpdateButtonSwiftIn(GameData& _gameData)
{
	if (timerEasingButton <= 1)
	{
		for (int i = 0; i < buttonVect.size(); i++)
		{
			if (i % 2 == 0)
			{
				buttonVect[i]->posButton = Easings::Lerp({ ((WORKING_WIDTH / 2.f) - WORKING_WIDTH), buttonVect[i]->posButton.y }, { (WORKING_WIDTH / 2.f), buttonVect[i]->posButton.y }, Easings::OutBack(timerEasingButton));
			}
			else
			{
				buttonVect[i]->posButton = Easings::Lerp({ ((WORKING_WIDTH / 2.f) + WORKING_WIDTH), buttonVect[i]->posButton.y }, { (WORKING_WIDTH / 2.f), buttonVect[i]->posButton.y }, Easings::OutBack(timerEasingButton));
			}
			buttonVect[i]->sprite.setPosition(buttonVect[i]->posButton);
		}
		timerEasingButton += _gameData.dt;

	}
	else
	{
		timerEasingButton = 0.f;

		if (nextPhase == PhaseType::PHASE1 && currentPhase != PhaseType::SWIFTING1_FRONT)
		{
			Reload(_gameData);
		}
		else if (nextPhase == PhaseType::PHASE2 && currentPhase != PhaseType::SWIFTING2_FRONT)
		{
			InitPhase2(_gameData.commingSoon);
		}
		else if (nextPhase == PhaseType::PHASE3 && currentPhase != PhaseType::SWIFTING3_FRONT)
		{
			InitPhase3();
		}
		currentPhase = nextPhase;
	}
}

void Menu::UpdateButtonSwiftOut(GameData& _gameData)
{
	if (timerEasingButton <= 1)
	{
		for (int i = 0; i < buttonVect.size(); i++)
		{
			if (i % 2 == 0)
			{
				buttonVect[i]->posButton = Easings::Lerp({ (WORKING_WIDTH / 2.f), buttonVect[i]->posButton.y }, { ((WORKING_WIDTH / 2.f) - WORKING_WIDTH), buttonVect[i]->posButton.y }, Easings::InBack(timerEasingButton));
			}
			else
			{
				buttonVect[i]->posButton = Easings::Lerp({ (WORKING_WIDTH / 2.f), buttonVect[i]->posButton.y }, { ((WORKING_WIDTH / 2.f) + WORKING_WIDTH), buttonVect[i]->posButton.y }, Easings::InBack(timerEasingButton));
			}
			buttonVect[i]->sprite.setPosition(buttonVect[i]->posButton);
		}
		timerEasingButton += _gameData.dt * 1.5f;
	}
	else
	{
		timerEasingButton = 0.f;

		if (nextPhase == PhaseType::PHASE1)
		{
			GoToPhase1(_gameData);
			currentPhase = PhaseType::SWIFTING1_FRONT;
		}
		else if (nextPhase == PhaseType::PHASE2)
		{
			InitPhase2(_gameData.commingSoon);
			currentPhase = PhaseType::SWIFTING2_FRONT;
		}
		else if (nextPhase == PhaseType::PHASE3)
		{
			InitPhase3();
			currentPhase = PhaseType::SWIFTING3_FRONT;
		}

	}
}

void Menu::UpdateThierrySwift(GameData& _gameData)
{
	switch (currentPhase)
	{
	case PhaseType::SWIFTING1_BACK:
		UpdateButtonSwiftOut(_gameData);
		break;
	case PhaseType::SWIFTING2_BACK:
		UpdateButtonSwiftOut(_gameData);
		break;
	case PhaseType::SWIFTING3_BACK:
		UpdateButtonSwiftOut(_gameData);
		break;
	case PhaseType::SWIFTING1_FRONT:
		UpdateButtonSwiftIn(_gameData);
		break;
	case PhaseType::SWIFTING2_FRONT:
		UpdateButtonSwiftIn(_gameData);
		break;
	case PhaseType::SWIFTING3_FRONT:
		UpdateButtonSwiftIn(_gameData);
		break;
	}
}

void Menu::FadeManagement(GameData& _gameData)
{
	fade->Update(_gameData.dt);
}

void Menu::Display(GameData& _gameData)
{
	if (_gameData.reloadMenu)
	{
		_gameData.reloadMenu = false;
		Reload(_gameData);
	}
	_gameData.window.setView(_gameData.camera->GetView());
	_gameData.window.draw(spriteBackground);
	_gameData.window.draw(_gameData.flame);
	if (!_gameData.multiplayer)
	{
		DisplayButtons(_gameData.window);
		_gameData.window.draw(spriteGameName);
		if (currentPhase == PhaseType::PHASE1 || currentPhase == PhaseType::PHASE2 || currentPhase == PhaseType::PHASE3)
		{
			_gameData.window.draw(_gameData.swordSpriteMenu);
		}
	}
	fade->Display(_gameData.window);
}

void Menu::Display3D(GameData& _gameData)
{
	if (_gameData.reloadMenu)
	{
		_gameData.reloadMenu = false;
		Reload(_gameData);
	}

	_gameData.window.setView(_gameData.camera->GetView());
	_gameData.renderTextureRed.setView(_gameData.window.getView());
	_gameData.renderTextureBlue.setView(_gameData.window.getView());

	Utils::BlitSprite(spriteBackground, sf::Vector2f{ spriteBackground.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),spriteBackground.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(spriteBackground, sf::Vector2f{ spriteBackground.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),spriteBackground.getPosition().y }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(_gameData.flame, sf::Vector2f{ _gameData.flame.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),_gameData.flame.getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(_gameData.flame, sf::Vector2f{ _gameData.flame.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),_gameData.flame.getPosition().y }, 0, _gameData.renderTextureBlue);

	if (!_gameData.multiplayer)
	{
		DisplayButtons3D(_gameData.renderTextureRed, _gameData.renderTextureBlue);

		Utils::BlitSprite(spriteGameName, sf::Vector2f{ spriteGameName.getPosition().x + (TROAD_GAP * (-1 * 700.f + WORKING_HEIGHT) / 100.f),spriteGameName.getPosition().y }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(spriteGameName, sf::Vector2f{ spriteGameName.getPosition().x - (TROAD_GAP * (-1 * 700.f + WORKING_HEIGHT) / 100.f),spriteGameName.getPosition().y }, 0, _gameData.renderTextureBlue);

		if (currentPhase == PhaseType::PHASE1 || currentPhase == PhaseType::PHASE2 || currentPhase == PhaseType::PHASE3)
		{
			Utils::BlitSprite(_gameData.swordSpriteMenu, sf::Vector2f{ _gameData.swordSpriteMenu.getPosition().x + (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),_gameData.swordSpriteMenu.getPosition().y }, 0, _gameData.renderTextureRed);
			Utils::BlitSprite(_gameData.swordSpriteMenu, sf::Vector2f{ _gameData.swordSpriteMenu.getPosition().x - (TROAD_GAP * (-1 * 800.f + WORKING_HEIGHT) / 100.f),_gameData.swordSpriteMenu.getPosition().y }, 0, _gameData.renderTextureBlue);
		}
	}

	fade->Display(_gameData.renderTextureRed, _gameData.renderTextureBlue);
}

void Menu::KeyPressed(GameData& _gameData)
{
	DownUpKeysButtons(_gameData);

	if (_gameData.event.type == sf::Event::KeyPressed && _gameData.event.key.code == sf::Keyboard::Enter && !fade->GetIsFadingIn() && !fade->GetIsFadingOut() && (currentPhase == PhaseType::PHASE1 || currentPhase == PhaseType::PHASE2 || currentPhase == PhaseType::PHASE3))
	{
		SetButtonSelectedPressed(_gameData);
	}
}

void Menu::JoystickHandler(GameData& _gameData)
{
	if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
	{
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CROSS) && (currentPhase == PhaseType::PHASE1 || currentPhase == PhaseType::PHASE2 || currentPhase == PhaseType::PHASE3))
		{
			SetButtonSelectedPressed(_gameData);
		}
	}
	else
	{
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::A) && (currentPhase == PhaseType::PHASE1 || currentPhase == PhaseType::PHASE2 || currentPhase == PhaseType::PHASE3))
		{
			SetButtonSelectedPressed(_gameData);
		}
	}
}

void Menu::JoystickPressed(GameData& _gameData)
{
	if (_gameData.event.type == sf::Event::JoystickButtonPressed && !fade->GetIsFadingIn() && !fade->GetIsFadingOut())
	{
		JoystickHandler(_gameData);
	}
}

void Menu::SetAlpha(const float _alpha)
{
	fade->SetAlpha(_alpha);
	fade->SetColor(sf::Color(0, 0, 0, (sf::Uint8)_alpha));
}