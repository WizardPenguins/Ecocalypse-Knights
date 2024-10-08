#include "stdafx.h"
#include "Paralax.h"
#include "Easings.h"
#include "Camera.h"
#include "Animation.h"

void Paralax::Init(GameData& _gameData)
{
	backGroundLevel1[0] = Utils::LoadSprite("Assets/Images/Background/WideMap1.png", TOP_LEFT);
	backGroundLevel1[1] = Utils::LoadSprite("Assets/Images/Background/WideMap2.png", TOP_LEFT);
	backGroundLevel1[2] = Utils::LoadSprite("Assets/Images/Background/WideMap3.png", TOP_LEFT);
	backGroundLevel1[3] = Utils::LoadSprite("Assets/Images/Background/WideMap4.png", TOP_LEFT);
	backGroundLevel1[4] = Utils::LoadSprite("Assets/Images/Background/WideMap5.png", TOP_LEFT);
	backGroundLevel1[5] = Utils::LoadSprite("Assets/Images/Background/Tree1.png", TOP_LEFT);
	backGroundLevel1[6] = Utils::LoadSprite("Assets/Images/Background/Tree2.png", TOP_LEFT);
	backGroundLevel1[7] = Utils::LoadSprite("Assets/Images/Background/Tree2.png", TOP_LEFT);
	backGroundLevel1[8] = Utils::LoadSprite("Assets/Images/Background/Tree3.png", TOP_LEFT);
	backGroundLevel1[9] = Utils::LoadSprite("Assets/Images/Background/WideMap1Over.png", TOP_LEFT);

	backGroundLevel2[0] = Utils::LoadSprite("Assets/Images/Background/WideMap1Level2.png", TOP_LEFT);
	backGroundLevel2[1] = Utils::LoadSprite("Assets/Images/Background/WideMap2Level2.png", TOP_LEFT);
	backGroundLevel2[2] = Utils::LoadSprite("Assets/Images/Background/WideMap3Level2.png", TOP_LEFT);
	backGroundLevel2[3] = Utils::LoadSprite("Assets/Images/Background/WideMap4Level2.png", TOP_LEFT);
	backGroundLevel2[4] = Utils::LoadSprite("Assets/Images/Background/WideMap5Level2.png", TOP_LEFT);

	backGroundLevel3[0] = Utils::LoadSprite("Assets/Images/Background/Map3/WideMap1Level3.png", TOP_LEFT);
	backGroundLevel3[1] = Utils::LoadSprite("Assets/Images/Background/Map3/WideMap2Level3.png", TOP_LEFT);
	backGroundLevel3[2] = Utils::LoadSprite("Assets/Images/Background/Map3/WideMap3Level3.png", TOP_LEFT);
	backGroundLevel3[3] = Utils::LoadSprite("Assets/Images/Background/Map3/WideMap4Level3.png", TOP_LEFT);
	backGroundLevel3[4] = Utils::LoadSprite("Assets/Images/Background/Map3/WideMap5Level3.png", TOP_LEFT);
	backGroundLevel3[5] = Utils::LoadSprite("Assets/Images/Background/Map3/WideMap6Level3.png", TOP_LEFT);
	backGroundLevel3[6] = Utils::LoadSprite("Assets/Images/Background/Map3/WideMap7Level3.png", TOP_LEFT);
	backGroundLevel3[7] = Utils::LoadSprite("Assets/Images/Background/Map3/WideMap8Level3.png", TOP_LEFT);

	maskTreeLevel3 = Utils::LoadSprite("Assets/Images/Background/Map3/MaskTree.png", TOP_LEFT);
	maskTreeLevel3.setPosition(7386.f, 0.f);

	spriteMapPathLevel1 = Utils::LoadSprite("Assets/Images/Background/HugeMapPath.png", TOP_LEFT);
	spriteMapPathLevel2 = Utils::LoadSprite("Assets/Images/Background/HugeMapPathLevel2.png", TOP_LEFT);
	spriteMapPathLevel3 = Utils::LoadSprite("Assets/Images/Background/Map3/HugeMapPathLevel3.png", TOP_LEFT);

	spriteShadowLevel1 = Utils::LoadSprite("Assets/Images/Background/Shadow.png", TOP_LEFT);
	spriteShadowLevel2 = Utils::LoadSprite("Assets/Images/Background/ShadowLevel2.png", TOP_LEFT);

	buildingSortLevel1[YellowHouse].sprite = Utils::LoadSprite("Assets/Images/Background/YellowHouse.png", TOP_LEFT);
	buildingSortLevel1[RedHouse].sprite = Utils::LoadSprite("Assets/Images/Background/RedHouse.png", TOP_LEFT);
	buildingSortLevel1[GreenHouse].sprite = Utils::LoadSprite("Assets/Images/Background/GreenHouse.png", TOP_LEFT);
	buildingSortLevel1[Fountain].sprite = Utils::LoadSprite("Assets/Images/Background/Anim/Fountain.png", TOP_LEFT, 3, 1);
	buildingSortLevel1[Fountain].isAnim = true;
	sleepingNut.sprite = Utils::LoadSprite("Assets/Images/Background/Anim/SleepingNut.png", TOP_LEFT);
	sleepingNut.isAnim = true;
	buildingSortLevel1[Church].sprite = Utils::LoadSprite("Assets/Images/Background/Church.png", TOP_LEFT);

	buildingFirstPlanFirstLevel[FirstPlanGrass] = Utils::LoadSprite("Assets/Images/Background/FirstPlanGrass.png", TOP_LEFT);
	buildingFirstPlanFirstLevelAnim[FirstPlanDuo] = Utils::LoadSprite("Assets/Images/Background/Anim/FirstPlanDuo.png", TOP_LEFT);
	buildingFirstPlanFirstLevelAnim[FirstPlanIdle] = Utils::LoadSprite("Assets/Images/Background/Anim/FirstPlanIdle.png", TOP_LEFT);
	buildingFirstPlanFirstLevelAnim[FirstPlanTree] = Utils::LoadSprite("Assets/Images/Background/Anim/FirstPlanTree.png", TOP_LEFT);
	buildingThirdPlanFirstLevelAnim[Picnic] = Utils::LoadSprite("Assets/Images/Background/Anim/Picnic.png", TOP_LEFT);
	buildingThirdPlanFirstLevel[ThirdPlanVillage] = Utils::LoadSprite("Assets/Images/Background/ThirdPlanVillage.png", TOP_LEFT);
	buildingThirdPlanFirstLevelAnim[OldDesign] = Utils::LoadSprite("Assets/Images/Background/Anim/OldDesign.png", TOP_LEFT);
	buildingThirdPlanFirstLevelAnim[Flinch] = Utils::LoadSprite("Assets/Images/Background/Anim/Flinch.png", TOP_LEFT);
	buildingThirdPlanFirstLevelAnim[LyingNut] = Utils::LoadSprite("Assets/Images/Background/Anim/LyingNut.png", TOP_LEFT);
	buildingFourthPlanFirstLevel[FourthPlanBlueHouse] = Utils::LoadSprite("Assets/Images/Background/FourthPlanBlueHouse.png", TOP_LEFT);
	buildingFourthPlanFirstLevelAnim[Kick] = Utils::LoadSprite("Assets/Images/Background/Anim/Kick.png", TOP_LEFT);
	buildingFourthPlanFirstLevelAnim[FireRun] = Utils::LoadSprite("Assets/Images/Background/Anim/FireRun.png", TOP_LEFT);
	buildingFourthPlanFirstLevel[RedHouse2] = Utils::LoadSprite("Assets/Images/Background/RedHouse2.png", TOP_LEFT);
	buildingThirdPlanSecondLevelAnim[CoruptNut] = Utils::LoadSprite("Assets/Images/Background/Anim/Noisette_IdleC.png", TOP_LEFT);

	buildingSecondPlanFirstLevel = Utils::LoadSprite("Assets/Images/Background/ShadowFountain.png", TOP_LEFT);

	animSort[Fountain] = new Animation;
	sf::IntRect rect = sf::IntRect(0, 0, 612 / 3, 337);
	buildingSortLevel1[Fountain].sprite.setTextureRect(rect);
	animSort[Fountain]->CreateLeftToRight(buildingSortLevel1[Fountain].sprite, 3, 1, { 612,337 });

	animSort[1] = new Animation;
	sf::IntRect rect2 = sf::IntRect(0, 0, 1664 / 8, 91);
	sleepingNut.sprite.setTextureRect(rect2);
	animSort[1]->CreateLeftToRight(sleepingNut.sprite, 8, 1, { 1664,91 });

	animationFirstPlanFirstLevel[FirstPlanDuo] = new Animation;
	sf::IntRect rect3 = sf::IntRect(0, 0, 1772 / 4, 225);
	buildingFirstPlanFirstLevel[FirstPlanDuo].setTextureRect(rect3);
	animationFirstPlanFirstLevel[FirstPlanDuo]->CreateLeftToRight(buildingFirstPlanFirstLevel[FirstPlanDuo], 4, 1, { 1772,225 });

	animationFirstPlanFirstLevel[FirstPlanIdle] = new Animation;
	sf::IntRect rect4 = sf::IntRect(0, 0, 892 / 4, 225);
	buildingFirstPlanFirstLevelAnim[FirstPlanIdle].setTextureRect(rect4);
	animationFirstPlanFirstLevel[FirstPlanIdle]->CreateLeftToRight(buildingFirstPlanFirstLevelAnim[FirstPlanIdle], 4, 1, { 892,192 });

	animationFirstPlanFirstLevel[FirstPlanTree] = new Animation;
	sf::IntRect rect5 = sf::IntRect(0, 0, 3280 / 4, 1571);
	buildingFirstPlanFirstLevelAnim[FirstPlanTree].setTextureRect(rect5);
	animationFirstPlanFirstLevel[FirstPlanTree]->CreateLeftToRight(buildingFirstPlanFirstLevelAnim[FirstPlanTree], 4, 1, { 3280,1571 });

	animationThirdPlanFirstLevel[Picnic] = new Animation;
	sf::IntRect rect6 = sf::IntRect(0, 0, 644 / 4, 89);
	buildingThirdPlanFirstLevelAnim[Picnic].setTextureRect(rect6);
	animationThirdPlanFirstLevel[Picnic]->CreateLeftToRight(buildingThirdPlanFirstLevelAnim[Picnic], 4, 1, { 644,89 });

	animationThirdPlanFirstLevel[OldDesign] = new Animation;
	sf::IntRect rect7 = sf::IntRect(0, 0, 180 / 3, 51);
	buildingThirdPlanFirstLevelAnim[OldDesign].setTextureRect(rect7);
	animationThirdPlanFirstLevel[OldDesign]->CreateLeftToRight(buildingThirdPlanFirstLevelAnim[OldDesign], 3, 1, { 180,51 });

	animationThirdPlanFirstLevel[Flinch] = new Animation;
	sf::IntRect rect8 = sf::IntRect(0, 0, 1848 / 8, 108);
	buildingThirdPlanFirstLevelAnim[Flinch].setTextureRect(rect8);
	animationThirdPlanFirstLevel[Flinch]->CreateLeftToRight(buildingThirdPlanFirstLevelAnim[Flinch], 8, 1, { 1848,108 });

	animationThirdPlanFirstLevel[LyingNut] = new Animation;
	sf::IntRect rect9 = sf::IntRect(0, 0, 744 / 8, 73);
	buildingThirdPlanFirstLevelAnim[LyingNut].setTextureRect(rect9);
	animationThirdPlanFirstLevel[LyingNut]->CreateLeftToRight(buildingThirdPlanFirstLevelAnim[LyingNut], 8, 1, { 744,73 });

	animationFourthPlanFirstLevel[Kick] = new Animation;
	sf::IntRect rect10 = sf::IntRect(0, 0, 148 / 4, 35);
	buildingFourthPlanFirstLevelAnim[Kick].setTextureRect(rect10);
	animationFourthPlanFirstLevel[Kick]->CreateLeftToRight(buildingFourthPlanFirstLevelAnim[Kick], 4, 1, { 148,35 });

	animationFourthPlanFirstLevel[FireRun] = new Animation;
	sf::IntRect rect11 = sf::IntRect(0, 0, 240 / 6, 40);
	buildingFourthPlanFirstLevelAnim[FireRun].setTextureRect(rect11);
	animationFourthPlanFirstLevel[FireRun]->CreateLeftToRight(buildingFourthPlanFirstLevelAnim[FireRun], 6, 1, { 240,40 });

	animationThirdPlanSecondLevel[CoruptNut] = new Animation;
	sf::IntRect rect12 = sf::IntRect(0, 0, 1270 / 10, 122);
	buildingThirdPlanSecondLevelAnim[CoruptNut].setTextureRect(rect12);
	animationThirdPlanSecondLevel[CoruptNut]->CreateLeftToRight(buildingFourthPlanFirstLevelAnim[CoruptNut], 10, 1, { 1270,122 });


	backGroundLevel1[0].setPosition(0.f, 0.f);
	backGroundLevel1[1].setPosition(0.f, 0.f);
	backGroundLevel1[2].setPosition(0.f, 0.f);
	backGroundLevel1[3].setPosition(0.f, 0.f);
	backGroundLevel1[4].setPosition(0.f, 0.f);
	backGroundLevel1[5].setPosition({ 9200 ,0.f });
	backGroundLevel1[6].setPosition({ 9200 ,0.f });
	backGroundLevel1[7].setPosition({ 9200 ,0.f });
	backGroundLevel1[8].setPosition({ 9200 ,0.f });
	backGroundLevel1[9].setPosition({ 0.f ,0.f });

	for (int i = 0; i < 5; i++)
	{
		backGroundLevel2[i].setPosition(0.f, 0.f);
	}

	//ICI//
	for (int i = 1; i < 8; i++)
	{
		backGroundLevel3[i].setPosition(0.f, 0.f);
	}
	backGroundLevel3[0].setPosition(-400.f, 0.f);
	backGroundLevel3[2].setPosition(200.f, 0.f);
	//PLUS ICI//
	maskTreeLevel3.setPosition(7385.f, 0.f);

	buildingSortLevel1[YellowHouse].sprite.setPosition({ -568,-50 });
	buildingSortLevel1[RedHouse].sprite.setPosition({ 1436,-27 });
	buildingSortLevel1[GreenHouse].sprite.setPosition({ 2022,34 });
	buildingSortLevel1[Fountain].sprite.setPosition({ 948,425 });
	sleepingNut.sprite.setPosition({ 4729,592 });
	buildingSortLevel1[Church].sprite.setPosition({ 472,-652 });
	buildingFirstPlanFirstLevel[FirstPlanGrass].setPosition({ 3920,984 });
	buildingFirstPlanFirstLevelAnim[FirstPlanDuo].setPosition({ 4227,855 });
	buildingFirstPlanFirstLevelAnim[FirstPlanIdle].setPosition({ 5649,992 });
	buildingFirstPlanFirstLevelAnim[FirstPlanTree].setPosition({ 7000,-437 });
	buildingThirdPlanFirstLevelAnim[Picnic].setPosition({ 3660,432 });
	buildingThirdPlanFirstLevel[ThirdPlanVillage].setPosition({ 4053,-103 });
	buildingThirdPlanFirstLevelAnim[OldDesign].setPosition({ 4577,295 });
	buildingThirdPlanFirstLevelAnim[Flinch].setPosition({ 5768,409 });
	buildingThirdPlanFirstLevelAnim[LyingNut].setPosition({ 6057,460 });
	buildingFourthPlanFirstLevel[FourthPlanBlueHouse].setPosition({ 5289,460 });
	buildingFourthPlanFirstLevelAnim[Kick].setPosition({ 5118,511 });
	buildingFourthPlanFirstLevelAnim[FireRun].setPosition({ 5482,475 });
	buildingFourthPlanFirstLevel[RedHouse2].setPosition({ 4920 ,415 });
	buildingThirdPlanSecondLevelAnim[CoruptNut].setPosition({ 1600,550 });
	buildingSecondPlanFirstLevel.setPosition(960.f, 720.f);
	spriteShadowLevel1.setPosition({ 8600.f,0.f });

	imageMapPathLevel1.create((unsigned int)spriteMapPathLevel1.getGlobalBounds().width, (unsigned int)spriteMapPathLevel1.getGlobalBounds().height);
	imageMapPathLevel1 = spriteMapPathLevel1.getTexture()->copyToImage();
	_gameData.tempImage1 = imageMapPathLevel1;

	imageMapPathLevel2.create((unsigned int)spriteMapPathLevel2.getGlobalBounds().width, (unsigned int)spriteMapPathLevel2.getGlobalBounds().height);
	imageMapPathLevel2 = spriteMapPathLevel2.getTexture()->copyToImage();
	_gameData.tempImage2 = imageMapPathLevel2;

	spriteMapPathLevel3.setPosition(0.f, 0.f);
	imageMapPathLevel3.create((unsigned int)spriteMapPathLevel3.getGlobalBounds().width, (unsigned int)spriteMapPathLevel3.getGlobalBounds().height);
	imageMapPathLevel3 = spriteMapPathLevel3.getTexture()->copyToImage();
	_gameData.tempImage3 = imageMapPathLevel3;

	timer = 0;
	alpha = { 255.f,0.f };


	buildingSortLevel1[YellowHouse].posY = 500;
	buildingSortLevel1[RedHouse].posY = 420;
	buildingSortLevel1[GreenHouse].posY = 562;
	buildingSortLevel1[Fountain].posY = 738;
	sleepingNut.posY = 700;
	buildingSortLevel1[Church].posY = 200;


	_gameData.tabBuilding.push_back(&buildingSortLevel1[Fountain]);
	_gameData.tabBuilding.push_back(&buildingSortLevel1[YellowHouse]);
	_gameData.tabBuilding.push_back(&buildingSortLevel1[RedHouse]);
	_gameData.tabBuilding.push_back(&buildingSortLevel1[GreenHouse]);
	_gameData.tabBuilding.push_back(&buildingSortLevel1[Church]);
	_gameData.sleepingNut.push_back(&sleepingNut);

	timerFireRun = 0;
	goRight = true;
	bridge = Utils::LoadSprite("Assets/Images/Background/Map3/Bridge.png", TOP_LEFT);
	bridge.setPosition(9635.f, 1104.f);
}

void Paralax::Reload(GameData& _gameData)
{

	_gameData.sortVector.clear();
	_gameData.tabMob.clear();
	_gameData.scarecrow.clear();
	_gameData.tabBuilding.clear();
	_gameData.sleepingNut.clear();
	backGroundLevel1[0].setPosition(0, 0.f);
	backGroundLevel1[1].setPosition(0, 0.f);
	backGroundLevel1[2].setPosition(0, 0.f);
	backGroundLevel1[3].setPosition(0, 0.f);
	backGroundLevel1[4].setPosition(-1.1f, 0.f);
	backGroundLevel1[5].setPosition({ 9200 ,0.f });
	backGroundLevel1[6].setPosition({ 9200 ,0.f });
	backGroundLevel1[7].setPosition({ 9200 ,0.f });
	backGroundLevel1[8].setPosition({ 9200 ,0.f });

	buildingSortLevel1[YellowHouse].sprite.setPosition({ -568,-50 });
	buildingSortLevel1[RedHouse].sprite.setPosition({ 1436,-27 });
	buildingSortLevel1[GreenHouse].sprite.setPosition({ 2022,34 });
	buildingSortLevel1[Fountain].sprite.setPosition({ 948,425 });
	sleepingNut.sprite.setPosition({ 4729,592 });
	buildingSortLevel1[Church].sprite.setPosition({ 472,-652 });
	buildingFirstPlanFirstLevel[FirstPlanGrass].setPosition({ 3920,984 });
	buildingFirstPlanFirstLevelAnim[FirstPlanDuo].setPosition({ 4227,855 });
	buildingFirstPlanFirstLevelAnim[FirstPlanIdle].setPosition({ 5649,992 });
	buildingFirstPlanFirstLevelAnim[FirstPlanTree].setPosition({ 7000,-437 });
	buildingThirdPlanFirstLevelAnim[Picnic].setPosition({ 3660,432 });
	buildingThirdPlanFirstLevel[ThirdPlanVillage].setPosition({ 4053,-103 });
	buildingThirdPlanFirstLevelAnim[OldDesign].setPosition({ 4577,295 });
	buildingThirdPlanFirstLevelAnim[Flinch].setPosition({ 5768,409 });
	buildingThirdPlanFirstLevelAnim[LyingNut].setPosition({ 6057,460 });
	buildingFourthPlanFirstLevel[FourthPlanBlueHouse].setPosition({ 5289,460 });
	buildingFourthPlanFirstLevelAnim[Kick].setPosition({ 5118,511 });
	buildingFourthPlanFirstLevelAnim[FireRun].setPosition({ 5482,475 });
	buildingFourthPlanFirstLevel[RedHouse2].setPosition({ 4920 ,415 });
	buildingSecondPlanFirstLevel.setPosition(960.f, 720.f);
	spriteShadowLevel1.setPosition({ 8600.f,0.f });

	if (_gameData.levelState == LEVEL1 || _gameData.gameState == ARENA)
	{

		_gameData.tabBuilding.push_back(&buildingSortLevel1[Fountain]);
		_gameData.tabBuilding.push_back(&buildingSortLevel1[YellowHouse]);
		_gameData.tabBuilding.push_back(&buildingSortLevel1[RedHouse]);
		_gameData.tabBuilding.push_back(&buildingSortLevel1[GreenHouse]);
		_gameData.tabBuilding.push_back(&buildingSortLevel1[Church]);
		_gameData.sleepingNut.push_back(&sleepingNut);
	}

	timerFireRun = 0;
	goRight = true;


	for (int i = 0; i < 5; i++)
	{
		backGroundLevel2[i].setPosition(0.f, 0.f);
	}

	for (int i = 0; i < 8; i++)
	{
		backGroundLevel3[i].setPosition(0.f, 0.f);
	}
}

float MoveParalax(float _x, float _speed, float& _dt, int _signed)
{
	if (_signed == 1)
	{
		_x -= _speed * _dt;
		if (_x < -WORKING_WIDTH)
		{
			_x += WORKING_WIDTH;
		}
		return _x;
	}
	else
	{
		_x += _speed * _dt;
		return _x;
	}
}


void Paralax::ChangeAlphaFirstPlan(GameData& _gameData)
{

	bool changeAlpha = false;

	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		sf::Vector2f posPlayer = _gameData.tabPlayers[i]->GetPos();
		sf::Vector2f sizePlayer = _gameData.tabPlayers[i]->GetHitBox()->getSize();

		if (posPlayer.y > 1050.f - sizePlayer.y / 2.f)
		{
			changeAlpha = true;
		}
	}

	if (changeAlpha == true)
	{
		timer += _gameData.dt * 4.f;
		if (timer > 1.f)
		{
			timer = 1.f;
		}
		alpha = Easings::Lerp({ ALPHA_MAX,0.f }, { ALPHA_MIN,0.f }, timer);
	}
	else
	{

		timer -= _gameData.dt * 4.f;
		if (timer < 0.f)
		{
			timer = 0.f;
		}
		alpha = Easings::Lerp({ ALPHA_MAX,0.f }, { ALPHA_MIN,0.f }, timer);
	}
	backGroundLevel1[0].setColor(sf::Color(255, 255, 255, (sf::Uint8)(alpha.x)));
	backGroundLevel1[9].setColor(sf::Color(255, 255, 255, (sf::Uint8)(alpha.x)));
	backGroundLevel2[0].setColor(sf::Color(255, 255, 255, (sf::Uint8)(alpha.x)));
	for (int i = 0; i < NBR_BAT_ANIM_1PLAN_LEVEL1; i++)
	{
		buildingFirstPlanFirstLevelAnim[i].setColor(sf::Color(255, 255, 255, (sf::Uint8)(alpha.x)));
	}
	for (int i = 0; i < NBR_BAT_1PLAN_LEVEL1; i++)
	{
		buildingFirstPlanFirstLevel[i].setColor(sf::Color(255, 255, 255, (sf::Uint8)(alpha.x)));
	}


}

void Paralax::UpdateAnim(GameData& _gameData)
{

	switch (_gameData.levelState)
	{
	case LEVEL1:
	{
		for (int i = 0; i < NBR_BAT_ANIM_1PLAN_LEVEL1; i++)
		{
			bool temp = true;
			animationFirstPlanFirstLevel[i]->UpdateLeftToRight(buildingFirstPlanFirstLevelAnim[i], _gameData.dt, 9, 0, temp, true);
		}
		for (int i = 0; i < NBR_BAT_ANIM_3PLAN_LEVEL1; i++)
		{
			bool temp = true;
			animationThirdPlanFirstLevel[i]->UpdateLeftToRight(buildingThirdPlanFirstLevelAnim[i], _gameData.dt, 6, 0, temp, true);
		}
		for (int i = 0; i < NBR_BAT_ANIM_4PLAN_LEVEL1; i++)
		{
			bool temp = true;
			animationFourthPlanFirstLevel[i]->UpdateLeftToRight(buildingFourthPlanFirstLevelAnim[i], _gameData.dt, 9, 0, temp, true);
		}
		UpdateFireNut(_gameData);
		break;
	}
	case LEVEL2:
	{
		for (int i = 0; i < NBR_BAT_ANIM_3PLAN_LEVEL2; i++)
		{
			bool temp = true;
			animationThirdPlanSecondLevel[i]->UpdateLeftToRight(buildingThirdPlanSecondLevelAnim[i], _gameData.dt, 9, 0, temp, true);
		}
		break;
	}
	default:
		break;
	}

}

void Paralax::UpdateFireNut(GameData& _gameData)
{
	sf::Vector2f pos = buildingFourthPlanFirstLevelAnim[FireRun].getPosition();

	if (goRight)
	{

		if (pos.x > FIRE_RUN_MAX)
		{
			pos.x = FIRE_RUN_MAX;
			timerFireRun = 0.f;
			goRight = false;

		}
		else
		{
			buildingFourthPlanFirstLevelAnim[FireRun].setScale(1, 1);
			pos = Easings::Lerp({ FIRE_RUN_MIN,pos.y }, { FIRE_RUN_MAX,pos.y }, timerFireRun);
			timerFireRun += _gameData.dt / 2.5f;
		}
	}
	else
	{
		if (pos.x < FIRE_RUN_MIN)
		{

			pos.x = FIRE_RUN_MIN;
			timerFireRun = 0.f;
			goRight = true;
		}
		else
		{
			buildingFourthPlanFirstLevelAnim[FireRun].setScale(-1, 1);
			pos = Easings::Lerp({ FIRE_RUN_MAX,pos.y }, { FIRE_RUN_MIN,pos.y }, timerFireRun);
			timerFireRun += _gameData.dt / 2.5f;
		}
	}

	buildingFourthPlanFirstLevelAnim[FireRun].setPosition(pos);
}

void Paralax::UpdateParalaxLevel1(GameData& _gameData, int _signed, float _speed)
{
	float dt = _gameData.dt;
	backGroundLevel1[0].setPosition(MoveParalax(backGroundLevel1[0].getPosition().x, (_speed * 2.5f), dt, -_signed), backGroundLevel1[0].getPosition().y);
	backGroundLevel1[2].setPosition(MoveParalax(backGroundLevel1[2].getPosition().x, _speed, dt, _signed), backGroundLevel1[2].getPosition().y);
	backGroundLevel1[3].setPosition(MoveParalax(backGroundLevel1[3].getPosition().x, _speed * 0.25f, dt, _signed), backGroundLevel1[3].getPosition().y);
	backGroundLevel1[4].setPosition(MoveParalax(backGroundLevel1[4].getPosition().x, _speed * 0.125f, dt, _signed), backGroundLevel1[4].getPosition().y);
	backGroundLevel1[5].setPosition(MoveParalax(backGroundLevel1[5].getPosition().x, _speed, dt, _signed), backGroundLevel1[5].getPosition().y);
	backGroundLevel1[6].setPosition(MoveParalax(backGroundLevel1[6].getPosition().x, _speed, dt, _signed), backGroundLevel1[6].getPosition().y);
	backGroundLevel1[7].setPosition(MoveParalax(backGroundLevel1[7].getPosition().x, _speed, dt, _signed), backGroundLevel1[7].getPosition().y);
	backGroundLevel1[9].setPosition(backGroundLevel1[0].getPosition());
	for (int i = 0; i < NBR_BAT_3PLAN_LEVEL1; i++)
	{
		buildingThirdPlanFirstLevel[i].setPosition(MoveParalax(buildingThirdPlanFirstLevel[i].getPosition().x, (_speed), dt, _signed), buildingThirdPlanFirstLevel[i].getPosition().y);
	}
	for (int i = 0; i < NBR_BAT_4PLAN_LEVEL1; i++)
	{
		buildingFourthPlanFirstLevel[i].setPosition(MoveParalax(buildingFourthPlanFirstLevel[i].getPosition().x, (_speed * 0.25f), dt, _signed), buildingFourthPlanFirstLevel[i].getPosition().y);
	}
	for (int i = 0; i < NBR_BAT_ANIM_3PLAN_LEVEL1; i++)
	{
		buildingThirdPlanFirstLevelAnim[i].setPosition(MoveParalax(buildingThirdPlanFirstLevelAnim[i].getPosition().x, (_speed), dt, _signed), buildingThirdPlanFirstLevelAnim[i].getPosition().y);
	}
	for (int i = 0; i < NBR_BAT_ANIM_4PLAN_LEVEL1; i++)
	{
		buildingFourthPlanFirstLevelAnim[i].setPosition(MoveParalax(buildingFourthPlanFirstLevelAnim[i].getPosition().x, (_speed * 0.25f), dt, _signed), buildingFourthPlanFirstLevelAnim[i].getPosition().y);
	}
	for (int i = 0; i < NBR_BAT_ANIM_1PLAN_LEVEL1; i++)
	{
		buildingFirstPlanFirstLevelAnim[i].setPosition(MoveParalax(buildingFirstPlanFirstLevelAnim[i].getPosition().x, (_speed * 2.5f), dt, -_signed), buildingFirstPlanFirstLevelAnim[i].getPosition().y);
	}
	for (int i = 0; i < NBR_BAT_1PLAN_LEVEL1; i++)
	{
		buildingFirstPlanFirstLevel[i].setPosition(MoveParalax(buildingFirstPlanFirstLevel[i].getPosition().x, (_speed * 2.5f), dt, -_signed), buildingFirstPlanFirstLevel[i].getPosition().y);

	}

	spriteMapPathLevel1.setPosition(backGroundLevel1[1].getPosition());
	spriteMapPathLevel1.setColor(sf::Color(255, 255, 255, 200));

}
void Paralax::UpdateParalaxLevel2(GameData& _gameData, int _signed, float _speed)
{
	float dt = _gameData.dt;
	backGroundLevel2[0].setPosition(MoveParalax(backGroundLevel2[0].getPosition().x, (_speed * 2.5f), dt, -_signed), backGroundLevel2[0].getPosition().y);
	backGroundLevel2[2].setPosition(MoveParalax(backGroundLevel2[2].getPosition().x, _speed * 2.5f / 2.f, dt, _signed), backGroundLevel2[2].getPosition().y);
	backGroundLevel2[3].setPosition(MoveParalax(backGroundLevel2[3].getPosition().x, _speed * 1.5f / 2.f, dt, _signed), backGroundLevel2[3].getPosition().y);
	backGroundLevel2[4].setPosition(MoveParalax(backGroundLevel2[4].getPosition().x, _speed / 2.f, dt, _signed), backGroundLevel2[4].getPosition().y);

	for (int i = 0; i < NBR_BAT_ANIM_3PLAN_LEVEL2; i++)
	{
		buildingThirdPlanSecondLevelAnim[i].setPosition(MoveParalax(buildingThirdPlanSecondLevelAnim[i].getPosition().x, (_speed * 2.5f / 2.f), dt, _signed), buildingThirdPlanSecondLevelAnim[i].getPosition().y);
	}

	spriteMapPathLevel2.setPosition(backGroundLevel2[1].getPosition().x, backGroundLevel2[1].getPosition().y);

	spriteMapPathLevel2.setColor(sf::Color(255, 255, 255, 200));

}
void Paralax::UpdateParalaxLevel3(GameData& _gameData, int _signed, float _speed)
{
	float dt = _gameData.dt;
	backGroundLevel3[0].setPosition(MoveParalax(backGroundLevel3[0].getPosition().x, (_speed * 2.5f), dt, -_signed), backGroundLevel3[0].getPosition().y);
	backGroundLevel3[2].setPosition(MoveParalax(backGroundLevel3[2].getPosition().x, _speed * 2.5f / 2.f, dt, _signed), backGroundLevel3[2].getPosition().y);
	backGroundLevel3[3].setPosition(MoveParalax(backGroundLevel3[3].getPosition().x, _speed * 1.5f / 2.f, dt, _signed), backGroundLevel3[3].getPosition().y);
	backGroundLevel3[4].setPosition(MoveParalax(backGroundLevel3[4].getPosition().x, _speed / 2.f, dt, _signed), backGroundLevel3[4].getPosition().y);
	backGroundLevel3[5].setPosition(MoveParalax(backGroundLevel3[4].getPosition().x, _speed / 3.f, dt, _signed), backGroundLevel3[5].getPosition().y);
	backGroundLevel3[6].setPosition(MoveParalax(backGroundLevel3[4].getPosition().x, _speed / 4.f, dt, _signed), backGroundLevel3[6].getPosition().y);
	backGroundLevel3[7].setPosition(MoveParalax(backGroundLevel3[4].getPosition().x, _speed / 5.f, dt, _signed), backGroundLevel3[7].getPosition().y);

	spriteMapPathLevel3.setPosition(backGroundLevel3[1].getPosition().x, backGroundLevel3[1].getPosition().y);

	spriteMapPathLevel3.setColor(sf::Color(255, 255, 255, 155));

}

void Paralax::Update(GameData& _gameData, int _signed, float _speed)
{
	switch (_gameData.levelState)
	{
	case LEVEL1:
	{
		UpdateParalaxLevel1(_gameData, _signed, _speed);
		break;
	}
	case LEVEL2:
	{
		UpdateParalaxLevel2(_gameData, _signed, _speed);
		break;
	}
	case LEVEL3:
	{
		UpdateParalaxLevel3(_gameData, _signed, _speed);

		//if(_gameData.tab)
		break;
	}
	default:
		break;
	}


}

void Paralax::DisplayLevel1(GameData& _gameData)
{
	//display des arbres
	for (int i = 8; i >= 5; i--)
	{
		_gameData.window.draw(backGroundLevel1[i]);

	}

	//display 5eme plan
	_gameData.window.draw(backGroundLevel1[4]);

	//display 4eme plan
	_gameData.window.draw(backGroundLevel1[3]);
	for (int i = 0; i < NBR_BAT_ANIM_4PLAN_LEVEL1; i++)
	{
		if (i != Kick)
		{
			_gameData.window.draw(buildingFourthPlanFirstLevelAnim[i]);
		}
	}
	for (int i = 0; i < NBR_BAT_4PLAN_LEVEL1; i++)
	{
		_gameData.window.draw(buildingFourthPlanFirstLevel[i]);
	}
	_gameData.window.draw(buildingFourthPlanFirstLevelAnim[Kick]);

	//display 3eme plan
	_gameData.window.draw(backGroundLevel1[2]);
	for (int i = 0; i < NBR_BAT_ANIM_3PLAN_LEVEL1; i++)
	{
		_gameData.window.draw(buildingThirdPlanFirstLevelAnim[i]);
	}
	for (int i = 0; i < NBR_BAT_3PLAN_LEVEL1; i++)
	{
		_gameData.window.draw(buildingThirdPlanFirstLevel[i]);
	}


	//display 2eme plan
	_gameData.window.draw(backGroundLevel1[1]);
	_gameData.window.draw(buildingSecondPlanFirstLevel);

	_gameData.window.draw(spriteShadowLevel1);
	//_window.draw(*spriteMapPath1);
}

void Paralax::DisplayLevel1(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	//display des arbres
	for (int i = 8; i >= 5; i--)
	{
		Utils::BlitSprite(backGroundLevel1[i], sf::Vector2f{ backGroundLevel1[i].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel1[i].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[i].getPosition().y }, 0, _red);
		Utils::BlitSprite(backGroundLevel1[i], sf::Vector2f{ backGroundLevel1[i].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel1[i].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[i].getPosition().y }, 0, _blue);
	}

	//display 5eme plan
	Utils::BlitSprite(backGroundLevel1[4], sf::Vector2f{ backGroundLevel1[4].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel1[4].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[4].getPosition().y }, 0, _red);
	Utils::BlitSprite(backGroundLevel1[4], sf::Vector2f{ backGroundLevel1[4].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel1[4].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[4].getPosition().y }, 0, _blue);

	//display 4eme plan
	Utils::BlitSprite(backGroundLevel1[3], sf::Vector2f{ backGroundLevel1[3].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel1[3].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[3].getPosition().y }, 0, _red);
	Utils::BlitSprite(backGroundLevel1[3], sf::Vector2f{ backGroundLevel1[3].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel1[3].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[3].getPosition().y }, 0, _blue);

	for (int i = 0; i < NBR_BAT_ANIM_4PLAN_LEVEL1; i++)
	{
		if (i != Kick)
		{
			Utils::BlitSprite(buildingFourthPlanFirstLevelAnim[i], sf::Vector2f{ buildingFourthPlanFirstLevelAnim[i].getPosition().x - (TROAD_GAP * (-1 * buildingFourthPlanFirstLevelAnim[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingFourthPlanFirstLevelAnim[i].getPosition().y }, 0, _red);
			Utils::BlitSprite(buildingFourthPlanFirstLevelAnim[i], sf::Vector2f{ buildingFourthPlanFirstLevelAnim[i].getPosition().x + (TROAD_GAP * (-1 * buildingFourthPlanFirstLevelAnim[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingFourthPlanFirstLevelAnim[i].getPosition().y }, 0, _blue);
		}
	}

	for (int i = 0; i < NBR_BAT_4PLAN_LEVEL1; i++)
	{
		Utils::BlitSprite(buildingFourthPlanFirstLevel[i], sf::Vector2f{ buildingFourthPlanFirstLevel[i].getPosition().x - (TROAD_GAP * (-1 * buildingFourthPlanFirstLevel[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingFourthPlanFirstLevel[i].getPosition().y }, 0, _red);
		Utils::BlitSprite(buildingFourthPlanFirstLevel[i], sf::Vector2f{ buildingFourthPlanFirstLevel[i].getPosition().x + (TROAD_GAP * (-1 * buildingFourthPlanFirstLevel[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingFourthPlanFirstLevel[i].getPosition().y }, 0, _blue);
	}

	Utils::BlitSprite(buildingFourthPlanFirstLevelAnim[Kick], sf::Vector2f{ buildingFourthPlanFirstLevelAnim[Kick].getPosition().x - (TROAD_GAP * (-1 * buildingFourthPlanFirstLevelAnim[Kick].getPosition().y + WORKING_HEIGHT) / 100.f),buildingFourthPlanFirstLevelAnim[Kick].getPosition().y }, 0, _red);
	Utils::BlitSprite(buildingFourthPlanFirstLevelAnim[Kick], sf::Vector2f{ buildingFourthPlanFirstLevelAnim[Kick].getPosition().x + (TROAD_GAP * (-1 * buildingFourthPlanFirstLevelAnim[Kick].getPosition().y + WORKING_HEIGHT) / 100.f),buildingFourthPlanFirstLevelAnim[Kick].getPosition().y }, 0, _blue);

	//display 3eme plan
	Utils::BlitSprite(backGroundLevel1[2], sf::Vector2f{ backGroundLevel1[2].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel1[2].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[2].getPosition().y }, 0, _red);
	Utils::BlitSprite(backGroundLevel1[2], sf::Vector2f{ backGroundLevel1[2].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel1[2].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[2].getPosition().y }, 0, _blue);

	for (int i = 0; i < NBR_BAT_ANIM_3PLAN_LEVEL1; i++)
	{
		Utils::BlitSprite(buildingThirdPlanFirstLevelAnim[i], sf::Vector2f{ buildingThirdPlanFirstLevelAnim[i].getPosition().x - (TROAD_GAP * (-1 * buildingThirdPlanFirstLevelAnim[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingThirdPlanFirstLevelAnim[i].getPosition().y }, 0, _red);
		Utils::BlitSprite(buildingThirdPlanFirstLevelAnim[i], sf::Vector2f{ buildingThirdPlanFirstLevelAnim[i].getPosition().x + (TROAD_GAP * (-1 * buildingThirdPlanFirstLevelAnim[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingThirdPlanFirstLevelAnim[i].getPosition().y }, 0, _blue);
	}
	for (int i = 0; i < NBR_BAT_3PLAN_LEVEL1; i++)
	{
		Utils::BlitSprite(buildingThirdPlanFirstLevel[i], sf::Vector2f{ buildingThirdPlanFirstLevel[i].getPosition().x - (TROAD_GAP * (-1 * buildingThirdPlanFirstLevel[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingThirdPlanFirstLevel[i].getPosition().y }, 0, _red);
		Utils::BlitSprite(buildingThirdPlanFirstLevel[i], sf::Vector2f{ buildingThirdPlanFirstLevel[i].getPosition().x + (TROAD_GAP * (-1 * buildingThirdPlanFirstLevel[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingThirdPlanFirstLevel[i].getPosition().y }, 0, _blue);
	}

	//display 2eme plan
	Utils::BlitSprite(backGroundLevel1[1], sf::Vector2f{ backGroundLevel1[1].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel1[1].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[1].getPosition().y }, 0, _red);
	Utils::BlitSprite(backGroundLevel1[1], sf::Vector2f{ backGroundLevel1[1].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel1[1].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[1].getPosition().y }, 0, _blue);

	Utils::BlitSprite(buildingSecondPlanFirstLevel, sf::Vector2f{ buildingSecondPlanFirstLevel.getPosition().x - (TROAD_GAP * (-1 * buildingSecondPlanFirstLevel.getPosition().y + WORKING_HEIGHT) / 100.f),buildingSecondPlanFirstLevel.getPosition().y }, 0, _red);
	Utils::BlitSprite(buildingSecondPlanFirstLevel, sf::Vector2f{ buildingSecondPlanFirstLevel.getPosition().x + (TROAD_GAP * (-1 * buildingSecondPlanFirstLevel.getPosition().y + WORKING_HEIGHT) / 100.f),buildingSecondPlanFirstLevel.getPosition().y }, 0, _blue);

	Utils::BlitSprite(spriteShadowLevel1, sf::Vector2f{ spriteShadowLevel1.getPosition().x + (TROAD_GAP * (-1 * spriteShadowLevel1.getPosition().y + WORKING_HEIGHT) / 100.f),spriteShadowLevel1.getPosition().y }, 0, _red);
	Utils::BlitSprite(spriteShadowLevel1, sf::Vector2f{ spriteShadowLevel1.getPosition().x - (TROAD_GAP * (-1 * spriteShadowLevel1.getPosition().y + WORKING_HEIGHT) / 100.f),spriteShadowLevel1.getPosition().y }, 0, _blue);
}

void Paralax::DisplayLevel2(GameData& _gameData)
{
	_gameData.window.draw(backGroundLevel2[4]);
	_gameData.window.draw(backGroundLevel2[3]);
	_gameData.window.draw(backGroundLevel2[2]);
	for (int i = 0; i < NBR_BAT_ANIM_3PLAN_LEVEL2; i++)
	{
		_gameData.window.draw(buildingThirdPlanSecondLevelAnim[i]);
	}
	_gameData.window.draw(backGroundLevel2[1]);
	_gameData.window.draw(backGroundLevel2[0]);

	//_gameData.window.draw(spriteMapPathLevel2);
}

void Paralax::DisplayLevel2(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	Utils::BlitSprite(backGroundLevel2[4], sf::Vector2f{ backGroundLevel2[4].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel2[4].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel2[4].getPosition().y }, 0, _red);
	Utils::BlitSprite(backGroundLevel2[4], sf::Vector2f{ backGroundLevel2[4].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel2[4].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel2[4].getPosition().y }, 0, _blue);

	Utils::BlitSprite(backGroundLevel2[3], sf::Vector2f{ backGroundLevel2[3].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel2[3].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel2[3].getPosition().y }, 0, _red);
	Utils::BlitSprite(backGroundLevel2[3], sf::Vector2f{ backGroundLevel2[3].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel2[3].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel2[3].getPosition().y }, 0, _blue);

	Utils::BlitSprite(backGroundLevel2[2], sf::Vector2f{ backGroundLevel2[2].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel2[2].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel2[2].getPosition().y }, 0, _red);
	Utils::BlitSprite(backGroundLevel2[2], sf::Vector2f{ backGroundLevel2[2].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel2[2].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel2[2].getPosition().y }, 0, _blue);

	for (int i = 0; i < NBR_BAT_ANIM_3PLAN_LEVEL2; i++)
	{
		Utils::BlitSprite(buildingThirdPlanSecondLevelAnim[i], sf::Vector2f{ buildingThirdPlanSecondLevelAnim[i].getPosition().x + (TROAD_GAP * (-1 * buildingThirdPlanSecondLevelAnim[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingThirdPlanSecondLevelAnim[i].getPosition().y }, 0, _red);
		Utils::BlitSprite(buildingThirdPlanSecondLevelAnim[i], sf::Vector2f{ buildingThirdPlanSecondLevelAnim[i].getPosition().x - (TROAD_GAP * (-1 * buildingThirdPlanSecondLevelAnim[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingThirdPlanSecondLevelAnim[i].getPosition().y }, 0, _blue);
	}

	Utils::BlitSprite(backGroundLevel2[1], sf::Vector2f{ backGroundLevel2[1].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel2[1].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel2[1].getPosition().y }, 0, _red);
	Utils::BlitSprite(backGroundLevel2[1], sf::Vector2f{ backGroundLevel2[1].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel2[1].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel2[1].getPosition().y }, 0, _blue);

	Utils::BlitSprite(backGroundLevel2[0], sf::Vector2f{ backGroundLevel2[0].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel2[0].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel2[0].getPosition().y }, 0, _red);
	Utils::BlitSprite(backGroundLevel2[0], sf::Vector2f{ backGroundLevel2[0].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel2[0].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel2[0].getPosition().y }, 0, _blue);
}

void Paralax::DisplayLevel3(GameData& _gameData)
{
	_gameData.window.draw(backGroundLevel3[7]);
	_gameData.window.draw(backGroundLevel3[6]);
	_gameData.window.draw(backGroundLevel3[5]);
	_gameData.window.draw(backGroundLevel3[4]);

	_gameData.window.draw(backGroundLevel3[3]);

	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if(_gameData.tabPlayers[i]->playerIsShrinks)
		{
			_gameData.tabPlayers[i]->Display(_gameData.window, *_gameData.audio, _gameData.camera, _gameData.levelState);

			
			for (int i = 0; i < _gameData.tabPlayers.size(); i++)
			{
				if (_gameData.tabPlayers[i]->GetState() != EAT_BY_BOSS)
				{
					if (_gameData.tabPlayers[i]->playerIsShrinks)
					{
						_gameData.shadow.setScale({ 1.5f * SHRINKS,1.5f * SHRINKS });
						if (_gameData.tabPlayers[i]->GetState() == PARRY)
						{
							_gameData.shadow.setScale({ 1.8f * SHRINKS,1.5f * SHRINKS });
						}
						else
						{
							_gameData.shadow.setScale({ 1.5f * SHRINKS,1.5f * SHRINKS });
						}
					}
					else
					{
						if (_gameData.tabPlayers[i]->GetState() == PARRY)
						{
							_gameData.shadow.setScale({ 1.8f,1.5f });
						}
						else
						{
							_gameData.shadow.setScale({ 1.5f,1.5f });
						}

					}

					(bool)(_gameData.tabPlayers[i]->GetIsSrinks()) ?
						((bool)(_gameData.tabPlayers[i]->GetLastDir() == LEFT) ?
							Utils::BlitSprite(_gameData.shadow, { _gameData.tabPlayers[i]->GetPos().x + (10 * SHRINKS) , _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rt) :
							Utils::BlitSprite(_gameData.shadow, { _gameData.tabPlayers[i]->GetPos().x - (10 * SHRINKS), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rt))
						:
						((bool)(_gameData.tabPlayers[i]->GetLastDir() == LEFT) ?
							Utils::BlitSprite(_gameData.shadow, { _gameData.tabPlayers[i]->GetPos().x + 10, _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rt) :
							Utils::BlitSprite(_gameData.shadow, { _gameData.tabPlayers[i]->GetPos().x - 10, _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rt))
						;
				}
			}
			_gameData.window.draw(_gameData.rtShadow);
			
		}
	}

	_gameData.window.draw(backGroundLevel3[2]);
	_gameData.window.draw(backGroundLevel3[1]);
	_gameData.window.draw(bridge);


		
	//_gameData.window.draw(spriteMapPathLevel3);
}

void Paralax::Display3DLevel3(GameData& _gameData)
{
	Utils::BlitSprite(backGroundLevel3[7], sf::Vector2f{ backGroundLevel3[7].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel3[7].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[7].getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(backGroundLevel3[7], sf::Vector2f{ backGroundLevel3[7].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel3[7].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[7].getPosition().y }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(backGroundLevel3[6], sf::Vector2f{ backGroundLevel3[6].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel3[6].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[6].getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(backGroundLevel3[6], sf::Vector2f{ backGroundLevel3[6].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel3[6].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[6].getPosition().y }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(backGroundLevel3[5], sf::Vector2f{ backGroundLevel3[5].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel3[5].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[5].getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(backGroundLevel3[5], sf::Vector2f{ backGroundLevel3[5].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel3[5].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[5].getPosition().y }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(backGroundLevel3[4], sf::Vector2f{ backGroundLevel3[4].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel3[4].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[4].getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(backGroundLevel3[4], sf::Vector2f{ backGroundLevel3[4].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel3[4].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[4].getPosition().y }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(backGroundLevel3[3], sf::Vector2f{ backGroundLevel3[3].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel3[3].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[3].getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(backGroundLevel3[3], sf::Vector2f{ backGroundLevel3[3].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel3[3].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[3].getPosition().y }, 0, _gameData.renderTextureBlue);

	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.tabPlayers[i]->playerIsShrinks)
		{
			_gameData.tabPlayers[i]->Display(*_gameData.audio,_gameData.renderTextureRed, _gameData.renderTextureBlue, _gameData.camera, _gameData.levelState);

			if (_gameData.tabPlayers[i]->GetState() != EAT_BY_BOSS)
			{
				if (_gameData.tabPlayers[i]->playerIsShrinks)
				{
					_gameData.shadow.setScale({ 1.5f * SHRINKS,1.5f * SHRINKS });
					if (_gameData.tabPlayers[i]->GetState() == PARRY)
					{
						_gameData.shadow.setScale({ 1.8f * SHRINKS,1.5f * SHRINKS });
					}
					else
					{
						_gameData.shadow.setScale({ 1.5f * SHRINKS,1.5f * SHRINKS });
					}
				}
				else
				{
					if (_gameData.tabPlayers[i]->GetState() == PARRY)
					{
						_gameData.shadow.setScale({ 1.8f,1.5f });
					}
					else
					{
						_gameData.shadow.setScale({ 1.5f,1.5f });
					}

				}
				if (_gameData.tabPlayers[i]->GetIsSrinks())
				{
					if (_gameData.tabPlayers[i]->GetLastDir() == LEFT)
					{
						Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x + (10 * SHRINKS)) + (TROAD_GAP * (-1 * _gameData.tabPlayers[i]->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtRed);
						Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x + (10 * SHRINKS)) - (TROAD_GAP * (-1 * _gameData.tabPlayers[i]->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtBlue);
					}
					else
					{
						Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x + (10 * SHRINKS)) + (TROAD_GAP * (-1 * _gameData.tabPlayers[i]->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtRed);
						Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x + (10 * SHRINKS)) - (TROAD_GAP * (-1 * _gameData.tabPlayers[i]->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtBlue);
					}
				}
				else
				{
					if (_gameData.tabPlayers[i]->GetLastDir() == LEFT)
					{
						Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x + 10) + (TROAD_GAP * (-1 * _gameData.tabPlayers[i]->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtRed);
						Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x - 10) - (TROAD_GAP * (-1 * _gameData.tabPlayers[i]->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtBlue);
					}
					else
					{
						Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x + 10) + (TROAD_GAP * (-1 * _gameData.tabPlayers[i]->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtRed);
						Utils::BlitSprite(_gameData.shadow, { (_gameData.tabPlayers[i]->GetPos().x - 10) - (TROAD_GAP * (-1 * _gameData.tabPlayers[i]->GetPos().y + WORKING_HEIGHT) / 100.f), _gameData.tabPlayers[i]->GetPos().y }, 0, *_gameData.rtBlue);
					}
				}
			}
		}
	}

	Utils::BlitSprite(backGroundLevel3[2], sf::Vector2f{ backGroundLevel3[2].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel3[2].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[2].getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(backGroundLevel3[2], sf::Vector2f{ backGroundLevel3[2].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel3[2].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[2].getPosition().y }, 0, _gameData.renderTextureBlue);

	Utils::BlitSprite(backGroundLevel3[1], sf::Vector2f{ backGroundLevel3[1].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel3[1].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[1].getPosition().y }, 0, _gameData.renderTextureRed);
	Utils::BlitSprite(backGroundLevel3[1], sf::Vector2f{ backGroundLevel3[1].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel3[1].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel3[1].getPosition().y }, 0, _gameData.renderTextureBlue);
}

void Paralax::Display(GameData& _gameData)
{
	switch (_gameData.levelState)
	{
	case LEVEL1:
	{
		DisplayLevel1(_gameData);
		break;
	}
	case LEVEL2:
	{
		DisplayLevel2(_gameData);
		break;
	}
	case LEVEL3:
	{
		DisplayLevel3(_gameData);
		break;
	}
	default:
		break;
	}

}

void Paralax::Display3D(GameData& _gameData)
{
	switch (_gameData.levelState)
	{
	case LEVEL1:
	{
		DisplayLevel1(_gameData.renderTextureRed, _gameData.renderTextureBlue);
		break;
	}
	case LEVEL2:
	{
		DisplayLevel2(_gameData.renderTextureRed, _gameData.renderTextureBlue);
		break;
	}
	case LEVEL3:
	{
		Display3DLevel3(_gameData);
			break;
	}
	default:
		break;
	}
}

void Paralax::DisplayFirstPlanLevel1(GameData& _gameData)
{
	_gameData.window.draw(backGroundLevel1[0]);

	for (int i = 0; i < NBR_BAT_ANIM_1PLAN_LEVEL1; i++)
	{
		_gameData.window.draw(buildingFirstPlanFirstLevelAnim[i]);
	}

	_gameData.window.draw(backGroundLevel1[9]);

	for (int i = 0; i < NBR_BAT_1PLAN_LEVEL1; i++)
	{
		_gameData.window.draw(buildingFirstPlanFirstLevel[i]);
	}

}

void Paralax::DisplayFirstPlanLevel1(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	Utils::BlitSprite(backGroundLevel1[0], sf::Vector2f{ backGroundLevel1[0].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel1[0].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[0].getPosition().y }, 0, _red);
	Utils::BlitSprite(backGroundLevel1[0], sf::Vector2f{ backGroundLevel1[0].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel1[0].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[0].getPosition().y }, 0, _blue);

	for (int i = 0; i < NBR_BAT_ANIM_1PLAN_LEVEL1; i++)
	{
		Utils::BlitSprite(buildingFirstPlanFirstLevelAnim[i], sf::Vector2f{ buildingFirstPlanFirstLevelAnim[i].getPosition().x + (TROAD_GAP * (-1 * buildingFirstPlanFirstLevelAnim[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingFirstPlanFirstLevelAnim[i].getPosition().y }, 0, _red);
		Utils::BlitSprite(buildingFirstPlanFirstLevelAnim[i], sf::Vector2f{ buildingFirstPlanFirstLevelAnim[i].getPosition().x - (TROAD_GAP * (-1 * buildingFirstPlanFirstLevelAnim[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingFirstPlanFirstLevelAnim[i].getPosition().y }, 0, _blue);
	}

	Utils::BlitSprite(backGroundLevel1[9], sf::Vector2f{ backGroundLevel1[9].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel1[9].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[9].getPosition().y }, 0, _red);
	Utils::BlitSprite(backGroundLevel1[9], sf::Vector2f{ backGroundLevel1[9].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel1[9].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel1[9].getPosition().y }, 0, _blue);

	for (int i = 0; i < NBR_BAT_1PLAN_LEVEL1; i++)
	{
		Utils::BlitSprite(buildingFirstPlanFirstLevel[i], sf::Vector2f{ buildingFirstPlanFirstLevel[i].getPosition().x + (TROAD_GAP * (-1 * buildingFirstPlanFirstLevel[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingFirstPlanFirstLevel[i].getPosition().y }, 0, _red);
		Utils::BlitSprite(buildingFirstPlanFirstLevel[i], sf::Vector2f{ buildingFirstPlanFirstLevel[i].getPosition().x - (TROAD_GAP * (-1 * buildingFirstPlanFirstLevel[i].getPosition().y + WORKING_HEIGHT) / 100.f),buildingFirstPlanFirstLevel[i].getPosition().y }, 0, _blue);
	}
}

void Paralax::DisplayFirstPlanLevel2(GameData& _gameData)
{
	_gameData.window.draw(backGroundLevel2[0]);
	_gameData.window.draw(spriteShadowLevel2);
}

void Paralax::DisplayFirstPlanLevel2(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	Utils::BlitSprite(backGroundLevel2[0], sf::Vector2f{ backGroundLevel2[0].getPosition().x + (TROAD_GAP * (-1 * backGroundLevel2[0].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel2[0].getPosition().y }, 0, _red);
	Utils::BlitSprite(backGroundLevel2[0], sf::Vector2f{ backGroundLevel2[0].getPosition().x - (TROAD_GAP * (-1 * backGroundLevel2[0].getPosition().y + WORKING_HEIGHT) / 100.f),backGroundLevel2[0].getPosition().y }, 0, _blue);

	Utils::BlitSprite(spriteShadowLevel2, sf::Vector2f{ spriteShadowLevel2.getPosition().x + (TROAD_GAP * (-1 * spriteShadowLevel2.getPosition().y + WORKING_HEIGHT) / 100.f),spriteShadowLevel2.getPosition().y }, 0, _red);
	Utils::BlitSprite(spriteShadowLevel2, sf::Vector2f{ spriteShadowLevel2.getPosition().x - (TROAD_GAP * (-1 * spriteShadowLevel2.getPosition().y + WORKING_HEIGHT) / 100.f),spriteShadowLevel2.getPosition().y }, 0, _blue);
}

void Paralax::DisplayFirstPlanLevel3(GameData& _gameData)
{
	_gameData.window.draw(backGroundLevel3[0]);

	if (_gameData.tabPlayers[0]->GetIsDrawnBehindTree())
	{
		Utils::BlitSprite(maskTreeLevel3, maskTreeLevel3.getPosition(), 0, _gameData.window);;
	}
}

void Paralax::DisplayFirstPlanLevel3(sf::RenderTexture& _red, sf::RenderTexture& _blue)
{
	Utils::BlitSprite(spriteShadowLevel2, sf::Vector2f{ spriteShadowLevel2.getPosition().x + (TROAD_GAP * (-1 * spriteShadowLevel2.getPosition().y + WORKING_HEIGHT) / 100.f),spriteShadowLevel2.getPosition().y }, 0, _red);
	Utils::BlitSprite(spriteShadowLevel2, sf::Vector2f{ spriteShadowLevel2.getPosition().x - (TROAD_GAP * (-1 * spriteShadowLevel2.getPosition().y + WORKING_HEIGHT) / 100.f),spriteShadowLevel2.getPosition().y }, 0, _blue);

	if (true) // rajouter la condition que le joueur est draw derrière
	{
		Utils::BlitSprite(maskTreeLevel3, sf::Vector2f{ maskTreeLevel3.getPosition().x + (TROAD_GAP * (-1 * maskTreeLevel3.getPosition().y + WORKING_HEIGHT) / 100.f),maskTreeLevel3.getPosition().y }, 0, _red);
		Utils::BlitSprite(maskTreeLevel3, sf::Vector2f{ maskTreeLevel3.getPosition().x - (TROAD_GAP * (-1 * maskTreeLevel3.getPosition().y + WORKING_HEIGHT) / 100.f),maskTreeLevel3.getPosition().y }, 0, _blue);
	}
}

void Paralax::DisplayFirstPlan(GameData& _gameData)
{
	switch (_gameData.levelState)
	{
	case LEVEL1:
	{
		DisplayFirstPlanLevel1(_gameData);
		break;
	}
	case LEVEL2:
	{
		DisplayFirstPlanLevel2(_gameData);
		break;
	}
	case LEVEL3:
	{
		DisplayFirstPlanLevel3(_gameData);
		break;
	}
	default:
		break;
	}
}

void Paralax::DisplayFirstPlan(sf::RenderTexture& _red, sf::RenderTexture& _blue, LevelState _level)
{
	switch (_level)
	{
	case LEVEL1:
	{
		DisplayFirstPlanLevel1(_red, _blue);
		break;
	}
	case LEVEL2:
	{
		DisplayFirstPlanLevel2(_red, _blue);
		break;
	}
	case LEVEL3:
	{
		DisplayFirstPlanLevel3(_red, _blue);
		break;
	}
	default:
		break;
	}
}