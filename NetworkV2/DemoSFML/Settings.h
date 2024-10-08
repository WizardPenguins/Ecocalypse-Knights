#ifndef SETTINGS_H
#define SETTINGS_H

#include "stdafx.h"

#include"States.h"

enum class SettingsButton
{
	MINUS,
	PLUS,
	MINUS2,
	PLUS2,
	MINUS3,
	PLUS3,
	MINUS4,
	PLUS4,
	MINUS5,
	PLUS5,
	MINUS6,
	PLUS6,
	QUIT,
	RESET,
	APPLY,
};

enum class TypeSelected
{
	VOLUME,
	DISPLAY_MODE,
	HP_BAR,
	FRIENDLY_FIRE,
	THIRD_DIMENTION,
	TUTO,
	QUIT,
	RESET,
	APPLY,
	NB_TYPE
};

enum class DisplayTypes
{
	WINDOWED,
	BORDERLESS,
	FULLSCREEN
};

enum class HpBarTypes
{
	ON,
	OFF,
};

enum class FriendShipTypes
{
	ON,
	OFF,
};

enum class ThirdDimentionTypes
{
	ON,
	OFF,
};

enum class TutoTypes
{
	ON,
	OFF,
};

class Settings : public States
{
public:
	Settings(GameData& _gameData);
	~Settings();

	void Update(GameData& _gameData) override;
	void Display(GameData& _gameData) override;
	void Display3D(GameData& _gameData) override;
	void KeyPressed(GameData& _gameData) override;
	void JoystickPressed(GameData& _gameData)override;
	void UpdateButtonPressedEffect(GameData& _gameData) override;

	

private:
	void InitComponents(GameData& _gameData);
	void InitSprites(void);
	void InitTexts(void);
	void InitVolumeRect(GameData& _gameData);

	void JoystickXBOXController(GameData& _gameData);
	void UpdateVolume(GameData& _gameData);

	void PlusSound(GameData& _gameData);
	void MinusSound(GameData& _gameData);

	void UpdateAnim(GameData& _gameData);
	void ReCreateWindow(sf::RenderWindow& _window, int& _currentDisplayMode, int _displayModeOption);
	void ChangeButtonSelected(GameData& _gameData, TypeSelected _currentType, TypeSelected _newType, bool _currentIsAButton, bool _newIsAButton, SettingsButton _newButtonType, sf::Sprite& _currentSpriteSelected, sf::Sprite& _newSpriteSelected, sf::Texture& _textureActived, sf::Texture& _textureDesactived);

	void MovingInSettings(GameData& _gameData, bool _withController);
	void MovingUp(GameData& _gameData);
	void MovingDown(GameData& _gameData);
	void MovingRight(GameData& _gameData);
	void MovingLeft(GameData& _gameData);

	void Right1ButtonController(GameData& _gameData);
	void Left1ButtonController(GameData& _gameData);

	sf::Sprite backgroundSprite;
	sf::Sprite transparentSprite;

	sf::Sprite volumeRect;
	sf::Sprite volumeBar;

	sf::Texture volumeBarSelected;
	sf::Texture volumeBarUnSelected;
	sf::Sprite volumeBarName;

	sf::Texture windowedType;
	sf::Texture fullscreenType;
	sf::Texture borderlessType;
	sf::Sprite displayType;

	sf::Texture displayModeSelected;
	sf::Texture displayModeUnSelected;
	sf::Sprite displayModeName;

	sf::Texture optionOn;
	sf::Texture optionOff;
	sf::Sprite hpBarMode;
	sf::Texture hpBarSelected;
	sf::Texture  hpBarUnSelected;
	sf::Sprite  hpBarName;

	sf::Sprite friendShipMode;
	sf::Texture friendShipSelected;
	sf::Texture friendShipUnSelected;
	sf::Sprite  friendShipName;

	sf::Sprite thirdDimentionMode;
	sf::Texture thirdDimentionSelected;
	sf::Texture  thirdDimentionUnSelected;
	sf::Sprite  thirdDimentionName;

	sf::Sprite tutoMode;
	sf::Texture tutoSelected;
	sf::Texture  tutoUnSelected;
	sf::Sprite  tutoName;

	sf::Vector2f originalRectSize;
	bool volumeRectSelected;

	sf::Texture hpBarPictureOn;
	sf::Texture hpBarPictureOff;
	sf::Sprite hpBarPicture;

	sf::Texture friendshipPictureOn;
	sf::Texture friendshipPictureOff;
	sf::Sprite friendshipPicture;

	sf::Texture d3PictureOn;
	sf::Texture d3PictureOff;
	sf::Sprite d3Picture;

	sf::Texture tutoPictureOn;
	sf::Texture tutoPictureOff;
	sf::Sprite tutoPicture;

	TypeSelected currentTypeselected = TypeSelected::VOLUME;
	DisplayTypes displayTypes;
	HpBarTypes hpBarType = HpBarTypes::OFF;
	FriendShipTypes friendShipType = FriendShipTypes::ON;
	ThirdDimentionTypes thirdDimentionType = ThirdDimentionTypes::OFF;
	TutoTypes tutoType = TutoTypes::ON;
	bool lastPosIsLeft = true;
	std::vector<bool>typeSelected;

	int displayMode = sf::Style::Default;

	sf::Font textFont;
	sf::Text volumeText;
	
	float timerVolumeBar = 0.f;

};

#endif


