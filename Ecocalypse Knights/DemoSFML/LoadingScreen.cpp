#include "stdafx.h"
#include "GameStates.h"
#include "Mob.hpp"
#include "Player.hpp"
#include "Multiplayer.h"
#include "EntityManager.h"
#include "EntityManagerPVP.hpp"
#include "LoadingScreen.h"
#include <thread>
#include "Network.h"
#include "Upgrade.h"
#include "TroaDimmenssionSahMaire.h"
#include "Shop.h"
#include "Arena.h"

void LoadingScreen::InitLoadingScreen(void)
{
	loadingScreenCompteur = 0;
	font.loadFromFile("Assets/Fonts/RulesFont.otf");

	Utils::CreateText(text, font, "LOADING", 35, sf::Color::Black, true);
	Utils::CreateRectangleShape(backChargingBar, sf::Vector2f(1920.f / 4.f, 1080.f / 10.f * 9.f), sf::Vector2f(1920 - (2 * (1920.f / 4.f)), 40), sf::Color::White, false);
	Utils::CreateRectangleShape(frontChargingBar, sf::Vector2f(1920.f / 4.f, 1080.f / 10.f * 9.f), sf::Vector2f(0, 40), sf::Color::Green, false);
	text.setPosition(sf::Vector2f(backChargingBar.getPosition().x + backChargingBar.getSize().x / 2, backChargingBar.getPosition().y + 10));
	timer = 0;

	funnyStringList[0] = "You must be nuts to play this game";
	funnyStringList[1] = "I GOT FUUUUUUUEL ! Now, I'm ready to fight ! - Red Knight";
	funnyStringList[2] = "Hope you're fighting with honor, not 4 vs 1 poor hazelnut...";
	funnyStringList[3] = "I wonder how does the Hamuraï taste... Should I bring mustard ?";
	funnyStringList[4] = "I'mma break this Nut - Almond Hazelnegger";
	funnyStringList[5] = "My favourite sauce ? The delicious Hamurai sauce !";

	int tempRand = rand() % 6;
	Utils::CreateText(funnySentence, font, funnyStringList[tempRand], 25, sf::Color::Black, true);
	funnySentence.setPosition(sf::Vector2f(backChargingBar.getPosition().x + backChargingBar.getSize().x / 2, backChargingBar.getPosition().y + 13));

	animeSprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Noisette/Noisette_Target.png", CENTER, 8, 1);
	anime.CreateLeftToRight(animeSprite, 8, 1, { 1096 , 138 });
	sf::IntRect rect({ 0,0 }, { 1096 / 8,138/1 });
	animeSprite.setTextureRect(rect);
	background = Utils::LoadSprite("Assets/Images/BackGround/LoadingScreenBD.png", TOP_LEFT);
}

void LoadingScreen::UpdateAnim(sf::RenderWindow& _window)
{
	_window.draw(background);
	bool temp = true;
	anime.UpdateLeftToRight(animeSprite, 1.f, 0.1f, 0, temp, true);
	animeSprite.setScale({ -1,1 });
	Utils::BlitSprite(animeSprite, { backChargingBar.getPosition().x + frontChargingBar.getSize().x - 20 ,backChargingBar.getPosition().y - 65 }, 0, _window);
}

sf::Font LoadingScreen::GetFont()
{
	return font;
}
