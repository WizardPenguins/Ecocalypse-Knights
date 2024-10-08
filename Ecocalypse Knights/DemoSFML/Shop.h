#ifndef SHOP_H

#define SHOP_H
#include "stdafx.h"
#include"Common.h"
#include "States.h"
#include "Player.hpp"
 
#define SHOP_BUTTONS_NUMBER 5

#define POTION_PRICE 10
#define SPEED_BUFF_PRICE 20
#define STRENGTH_BUFF_PRICE 30
enum class ShopButtonType
{
	POTION,
	SPEED_BUFF,
	STRENGTH_BUFF,
	MAGIC_BUFF,
	QUIT
};

struct PlayerSort
{
	int id;
	int gold;
};

class Shop : public States
{
public:
	Shop();
	void Reload(GameData& _gameData);
	void ReloadWhenEnterInTheShop(GameData& _gameData);
	void Update(GameData& _gameData) override;
	void UpdateButtonPressedEffect(GameData& _gameData) override;
	void UpdateButtonsSelection(GameData& _gameData);
	void UpdateAnims(const float& _dt);
	void UpdateAllButtonsAnim(const float& _dt);
	void DisplayButtons(sf::RenderWindow& _window);
	void DisplayButtons(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void Display(GameData& _gameData) override;
	void Display3D(GameData& _gameData) override;
	void KeyPressed(GameData& _gameData)override;
	void SetButtonSelectedPressed(GameData& _gameData);
	void JoystickPressed(GameData& _gameData);
	void JoysticksButtonsMovements(GameData& _gameData);
	void JoystickHandler(GameData& _gameData);

private:

	sf::Sprite spriteBackground;
	sf::Sprite spriteForeground;
	sf::Sprite spriteGold;
	sf::Sprite spriteGoldBack;
	sf::Sprite spriteNutman;
	sf::Sprite spriteSign;
	sf::Sprite spriteCloset;
	sf::Sprite spriteCauldron;
	sf::Sprite spritePotionsCost;
	sf::Sprite spriteCurrentPlayer;
	std::vector<sf::Sprite> spritePlayersHead;
	Animation* animNutman;
	Animation* animCauldron;
	Animation* animCloset;
	Animation* animSign;
	float timerAnimNutman = 3.f;
	float timerAnimCloset = 5.f;
	float timerAnimWaitingCloset = 1.f;
	float timerAnimCauldron = 5.f;
	bool animClosetIsPlayingToRight = false;
	bool animClosetIsPlayingToLeft = false;
	bool animSignIsPlayingToRight = false;
	bool animSignIsPlayingToLeft = false;
	bool animCauldronIsPlaying = false;
	std::vector<Button*> buttonVect;
	std::vector<bool> itemIsAvailable;
	int buttonSelected = 0;
	int lastButtonSelected = 0;
	int playerTurn = 0;
	float timerJoystickPressed = 0.f;
	bool wantToQuit = false;
	
	std::vector<PlayerSort> turnOrder;
	sf::Font shopFont;
	std::vector<sf::Text> descriptionItems;
	sf::Text currentDescription;
	sf::Text goldText;
	sf::Text potionPriceText;
	int ligne = 0;
};

#endif