#include "stdafx.h"
#include "PreMenu.h"

PreMenu::PreMenu()
{
	InitComponents();
}

PreMenu::~PreMenu()
{
	delete fade;
	fade = nullptr;
}

void PreMenu::InitComponents(void)
{
	texture.loadFromFile("Assets/Images/PreMenu/logoCreajeux.png");
	texture.setSmooth(true);
	textureLogo.loadFromFile("Assets/Images/PreMenu/Logo.png");
	textureLogo.setSmooth(true);

	sprite.setTexture(texture);
	sf::Vector2u size = texture.getSize();
	sprite.setOrigin((float)size.x / 2.f, (float)size.y / 2.f);
	sprite.setScale(Screen::GetWidth() / (texture.getSize().x * 2), Screen::GetWidth() / (texture.getSize().y * 2));
	sprite.setPosition(Screen::GetWidth() / 2.f, Screen::GetHeight() / 2);

	fade = new Fade(8.f, false, true);
	isOnLogo = false;
	timerSkip = 0.f;
}

void PreMenu::FadeManagement(GameData& _gameData)
{
	fade->Update(_gameData.dt*3.f);

	if (fade->GetTimer() <= fade->GetTimerMin())
	{
		fade->SetIsFadingIn(true);
	}

	if (fade->GetTimer() >= fade->GetTimerMax())
	{
		SkipEffect(_gameData);
	}
}

void PreMenu::Update(GameData& _gameData)
{
	FadeManagement(_gameData);
	timerSkip += _gameData.dt;
}

void PreMenu::Display(GameData& _gameData)
{
	_gameData.window.draw(sprite);
	fade->Display(_gameData.window);
}

void PreMenu::SkipEffect(GameData& _gameData)
{
	if (timerSkip >= 0.25f)
	{
		if (!isOnLogo)
		{
			sprite.setTexture(textureLogo, true);
			sf::Vector2u size = textureLogo.getSize();
			sprite.setOrigin((float)size.x / 2.f, (float)size.y / 2.f + 30);
			sprite.setScale(Screen::Scale(0.8f, 0.8f));
			sprite.setPosition(Screen::GetWidth() / 2.f + 40.f, Screen::GetHeight() / 2);
			fade->SetIsFadingOut(true);
			isOnLogo = true;
			timerSkip = 0.f;
		}
		else
		{
			_gameData.ChangeState(MENU);
			timerSkip = 0.f;
		}
	}
}

void PreMenu::KeyPressed(GameData& _gameData)
{
	if (_gameData.event.key.code == sf::Keyboard::Key::Enter && _gameData.event.type == sf::Event::KeyPressed)
	{
		SkipEffect(_gameData);
	}
}

void PreMenu::JoystickPressed(GameData& _gameData)
{
	if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
	{
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CROSS))
		{
			SkipEffect(_gameData);
		}
	}
	else
	{
		if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::A))
		{
			SkipEffect(_gameData);
		}
	}

}