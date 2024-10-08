#ifndef UPGRADE_H
#define UPGRADE_H

#define NbMaxPlayer 4
#define NbMaxPlanks 4
#define NbMaxPoint 10
#define OffsetCrownBlocks 10.f
#define MaxStat 10
#define BaseStat 1 

#include "stdafx.h"
#include "Common.h"
#include "States.h"
#include "Animation.h"

enum class ButtonSelected
{
	LIFE,
	STRENGTH,
	AGILITY,
	WISDOM,
	CONFIRM
};

struct FilledBlocks
{
	sf::Sprite sprite;
	sf::Vector2f pos;
	bool lock;
	bool crown;
	bool colored;
};

class Upgrade : public States
{
public:
	Upgrade(GameData& _gameData);
	~Upgrade();

	void InitUpgrade(GameData& _gameData);
	void Reload(GameData& _gameData);
	void JoystickMovingUpgrade(GameData& _gameData);
	void UpdateButtonsSelectionUpgrade(GameData& _gameData);
	void SetButtonSelectedPressed(GameData& _gameData);
	void DownUpJoysticksButtonsUpgrade(GameData& _gameData);
	void Update(GameData& _gameData) override;
	void Display(GameData& _gameData) override;
	void Display3D(GameData& _gameData) override;
	void JoystickPressed(GameData& _gameData) override;
	void KeyPressed(GameData& _gameData) override;
	void LostFocus(GameData& _gameData) override;
	void SetAlpha(const float _alpha);
	void SetFadeOut(const bool _fadeOut);
	void Init(GameData& _gameData);
private:
	void InitText(sf::Font& _font);
	void LoadSprite();
	void InitTexte(sf::Font& _font);
	void InitPoint();
	void ChangeLock();
	void CrateAnimPlayerIdle();
	void UpdateAnimIdlePlayer(float _dt);
	void MaxStatPlayer(Player* _player);
	void UpdateMaxStatText(Player* _player);

	int maxLife;
	int maxMana;
	int maxDamage;
	int maxSpeed;

	sf::Text maxBefore;
	sf::Text maxAfter;
	sf::Text textStat;
	sf::Text textUnlockSkill;

	Animation* animPlayerIdle;

	std::string color[4] = { "Red", "Green", "Blue", "Yellow" };
	std::string statsList[4] = { "Life", "Strength", "Agility", "Wisdom" };
	std::string unlockAOEMagic = "Unlock heavy magic";
	std::string unlcokDash = "+1 Dash";

	sf::Text skillPoints;

	int currentPlayer = 0;

	int currentColorPlayer = 0;
	int buttonSelectedUpgrade;
	std::vector<Button*> buttonVectUpgrade;
	bool hasBeenPushBack = false;
	sf::Sprite backGround;
	sf::Sprite filledBlocksPlank[4];
	sf::Sprite filledBlocks[4];
	sf::Sprite emptyBlock;
	sf::Sprite kingButtons[4];
	sf::Sprite emptyKingButtons;
	sf::Sprite lockedBlock[4];
	sf::Sprite lockedCrown[4];
	sf::Sprite idlePlayer[4];
	sf::Sprite arrow;

	Button* mult;
	Button* noButton[4][4][2];
	Button* justButton[4][4][2];
	Button* pressButton[4][4][2];
	Button* selectButton[4][4][2];
	Button* confirm[3];

	Button* characterFrame[4];
	Button* colorBackGround[4];
	Button* plank[4][2];
	Button* pointsPlank;
	Button* textPlank;

	sf::Sprite stats[4];

	std::vector<sf::Text*> tabText;
	std::vector<sf::Text*> tabTextStats;
	std::vector<sf::RectangleShape*> tabButton;
	std::vector<sf::Vector2f> tabPos;
	float timerButton;

	bool init = false;
	FilledBlocks filledBLock[NbMaxPlayer][NbMaxPlanks][NbMaxPoint];
};

#endif // !UPGRADE_H
