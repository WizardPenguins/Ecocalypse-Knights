#ifndef PARALAX_H
#define PARALAX_H

#include "Common.h"
#include "Player.hpp"
#include "stdafx.h"

#define BACKGROUND_WIDHT 5760
#define ALPHA_MAX 255
#define ALPHA_MIN 200

#define FIRE_RUN_MIN 5110
#define FIRE_RUN_MAX 5400

#define NBR_BAT_1PLAN_LEVEL1 1
#define NBR_BAT_2PLAN_LEVEL1 5
#define NBR_BAT_3PLAN_LEVEL1 1
#define NBR_BAT_4PLAN_LEVEL1 2
#define NBR_BAT_5PLAN_LEVEL1 0

#define NBR_BAT_ANIM_1PLAN_LEVEL1 3
#define NBR_BAT_ANIM_3PLAN_LEVEL1 4
#define NBR_BAT_ANIM_4PLAN_LEVEL1 2
#define NBR_BAT_ANIM_5PLAN_LEVEL1 0

#define NBR_BAT_ANIM_3PLAN_LEVEL2 1

enum FirstPlanFirstLevelBuilding
{
	FirstPlanGrass,
};

//The second plan is special because he is sort in y with other element
enum SecondPlanFirstLevelBuildingSort
{
	Fountain,
	Church,
	YellowHouse,
	RedHouse,
	GreenHouse,
};
enum ThirdPlanFirstLevelBuilding
{
	ThirdPlanVillage,
};
enum FourthPlanFirstLevelBuilding
{
	FourthPlanBlueHouse,
	RedHouse2,
};
enum FifthPlanFirstLevelBuilding
{

};
enum FirstPlanFirstLevelBuildingAnim
{
	FirstPlanDuo,
	FirstPlanIdle,
	FirstPlanTree,
};

enum ThirdPlanFirstLevelBuildingAnim
{
	Picnic,
	OldDesign,
	Flinch,
	LyingNut,
};
enum FourthPlanFirstLevelBuildingAnim
{
	Kick,
	FireRun
};

enum FifthPlanFirstLevelBuildingAnim
{
};

enum  ThirdPlanSecondLevelBuildingAnim
{
	CoruptNut
};

class Paralax
{
public:

	void Init(GameData& _gameData);
	void Reload(GameData& _gameData);
	void UpdateAnim(GameData& _gameData);
	void UpdateFireNut(GameData& _gameData);
	void UpdateParalaxLevel1(GameData& _gameData, int _signed, float _speed);
	void UpdateParalaxLevel2(GameData& _gameData, int _signed, float _speed);
	void UpdateParalaxLevel3(GameData& _gameData, int _signed, float _speed);
	void Update(GameData& _gameData, int _signed, float _speed);
	void ChangeAlphaFirstPlan(GameData& _gameData);
	void DisplayLevel1(GameData& _gameData);
	void DisplayLevel1(sf::RenderTexture& _red, sf::RenderTexture& _blue);	
	void DisplayLevel2(GameData& _gameData);
	void DisplayLevel2(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayLevel3(GameData& _gameData);
	void Display3DLevel3(GameData& _gameData);
	void Display(GameData& _gameData);
	void Display3D(GameData& _gameData);
	void DisplayFirstPlanLevel1(GameData& _gameData);
	void DisplayFirstPlanLevel1(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayFirstPlanLevel2(GameData& _gameData);
	void DisplayFirstPlanLevel2(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayFirstPlanLevel3(GameData& _gameData);
	void DisplayFirstPlanLevel3(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayFirstPlan(GameData& _gameData);
	void DisplayFirstPlan(sf::RenderTexture& _red,sf::RenderTexture& _blue,LevelState _level);
	sf::Image imageMapPathLevel1;
	sf::Image imageMapPathLevel2;
	sf::Image imageMapPathLevel3;
	Animation* animSort[2];
private:

	sf::Sprite backGroundLevel1[10];
	sf::Sprite backGroundLevel2[5];
	sf::Sprite backGroundLevel3[8];

	sf::Sprite spriteMapPathLevel1;
	sf::Sprite spriteMapPathLevel2; 
	sf::Sprite spriteMapPathLevel3;

	sf::Sprite buildingSecondPlanFirstLevel;

	sf::Sprite buildingFirstPlanFirstLevel[NBR_BAT_1PLAN_LEVEL1];
	sf::Sprite buildingThirdPlanFirstLevel[NBR_BAT_3PLAN_LEVEL1];
	sf::Sprite buildingFourthPlanFirstLevel[NBR_BAT_4PLAN_LEVEL1];
	//sf::Sprite buildingFifthPlanFirstLevel[NBR_BAT_5PLAN_LEVEL1];

	sf::Sprite buildingFirstPlanFirstLevelAnim[NBR_BAT_ANIM_1PLAN_LEVEL1];
	Animation* animationFirstPlanFirstLevel[NBR_BAT_ANIM_1PLAN_LEVEL1];
	sf::Sprite buildingThirdPlanFirstLevelAnim[NBR_BAT_ANIM_3PLAN_LEVEL1];
	Animation* animationThirdPlanFirstLevel[NBR_BAT_ANIM_3PLAN_LEVEL1];
	sf::Sprite buildingFourthPlanFirstLevelAnim[NBR_BAT_ANIM_4PLAN_LEVEL1];
	Animation* animationFourthPlanFirstLevel[NBR_BAT_ANIM_4PLAN_LEVEL1];
	//sf::Sprite buildingFifthPlanFirstLevelAnim[NBR_BAT_ANIM_5PLAN_LEVEL1];
	//Animation* animationFifthPlanFirstLevel[NBR_BAT_ANIM_5PLAN_LEVEL1];

	Building buildingSortLevel1[NBR_BAT_2PLAN_LEVEL1];
	Building sleepingNut;

	sf::Sprite buildingThirdPlanSecondLevelAnim[NBR_BAT_ANIM_3PLAN_LEVEL2];
	Animation* animationThirdPlanSecondLevel[NBR_BAT_ANIM_3PLAN_LEVEL2];

	sf::Sprite spriteShadowLevel1;
	sf::Sprite spriteShadowLevel2;

	sf::Sprite maskTreeLevel3;

	float timer;
	float timerFireRun;
	bool goRight;
	sf::Vector2f alpha;

	sf::Sprite bridge;
};

#endif // !PARALAX_H