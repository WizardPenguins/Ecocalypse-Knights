#ifndef HUD_H
#define HUD_H

#include"Common.h"
#include"Player.hpp"
#include "Animation.h"
#include "Camera.h"
#include "stdafx.h"

#define MAX_XP 100
#define NB_SPRITE_HEALTH 5
#define NB_SPRITE_MANA 2
#define NB_SPRITES_POTIONS 5

enum SpritesHealth
{
	EMPTYHEART,
	QUARTERHEART,
	HALFHEART,
	THREEQUARTERHEART,
	FULLHEART
};

enum SpritesMana
{
	USEDMANA,
	MANA
};

enum SpritesPotions
{
	EMPTY_POTION,
	HEALTH_POTION,
	STRENGTH_BUFF_POTION,
	SPEED_BUFF_POTION,
	MAGIC_BUFF_POTION	
};

class HUD
{
public:
	HUD() = default;
	~HUD() = default;

	void InitHud(std::vector<Player*>& _tabPlayers, std::vector<int>& _playerSelected, sf::RenderWindow& _window);
	void InitComponents(sf::RenderWindow& _window, std::vector<Player*>& _tabPlayers);
	void Reload(GameData& _gameData);
	void Update(GameData& _gameData);
	
	void UpdateAnim(float _dt, std::vector<Player*>& _tabPlayers);
	void Display(GameData& _gameData);
	void Display(sf::RenderTexture& _red, sf::RenderTexture& _blue, std::vector<Player*>& _tabPlayer, bool _arena);
	void DisplayMigrant(GameData& _gameData);
	void DisplayMigrant3D(GameData& _gameData);
	void InitPortraits(std::vector<Player*>& _tabPlayers, std::vector<int>& _playerSelected, sf::RenderWindow& _window);
	
	sf::Vector2f GetPosIcon(int _id);

	std::vector <sf::Sprite> spriteIconsPlayers;
	sf::Font hudFont;

private:
	void LoadHealthSprites();
	void LoadManaSprites();
	void LoadPotionsSprites();
	void LoadXpSprites(std::vector<Player*>& _tabPlayers);
	void InitSprites(std::vector<Player*>& _tabPlayers);
	void InitHudRevive();
	void InitHudManaRecovery();
	void UpdateMaxLifePlayer(std::vector<Player*>& _tabPlayers, int _i);
	void UpdateMaxManaPlayer(std::vector<Player*>& _tabPlayers, int _i);
	void UpdatePos(GameData& _gameData);
	void UpdatePosStoryMode(std::vector<Player*>& _tabPlayers, sf::RenderWindow& _window);
	void UpdatePosArenaTeamfightMode(std::vector<Player*>& _tabPlayers, sf::RenderWindow& _window , std::vector<int>& _playerTeam);
	
	std::vector <sf::Sprite> spriteFramePlayers;
	std::vector <sf::Sprite> spriteLvlPlayers;
	std::vector<std::vector <sf::Sprite>> spriteHealthPlayers;
	std::vector<std::vector <sf::Sprite>> spriteManaPlayers;
	std::vector < std::vector <sf::Sprite>> spriteXpBarPlayers;
	std::vector <sf::Sprite> spriteXpBarBackgroundPlayers;
	std::vector <sf::Sprite> spriteXpBarOverPlayers;
	std::vector <sf::Sprite> spritePlayersCanBeRevived;
	std::vector <sf::Sprite> spritePlayersManaRecovery;
	std::vector <sf::Sprite> spritePlayersInventory;
	sf::Sprite spriteRevive;
	sf::Sprite spriteManaRecovery;
	sf::Sprite tempXpPixel;
	sf::Sprite tempXpBarBackground;
	sf::Sprite tempXpBarOver;
	Animation animButtonRevive;
	std::vector < Animation> animManaRecovery;
	std::vector <sf::Text> lvlText;
	std::vector <sf::Text> nameText;
	std::vector <sf::Sprite> allHealthSprites;
	std::vector <sf::Sprite> allManaSprites;
	std::vector <sf::Sprite> allPotionsSprites;
	std::vector<int> playerInWhatTeam;

	int nbPlayersDead = 0;
	std::vector <bool> playAnimManaRecovery;
};

#endif