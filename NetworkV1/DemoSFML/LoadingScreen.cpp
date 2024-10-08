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

	funnyStringList[0] = "You must be nut to play this game";
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

void LoadingScreen::DisplayLoadingScreen()
{
	gameDataRef.audio->PlayMusic("LoadingScreen", true);
	compteur = 0.f;
	gameDataRef.camera = new Camera(gameDataRef);
	LoadSound(*gameDataRef.audio);
	compteur = 1.f;
	gameDataRef.InitSprite();
	compteur = 2.f;
	gameDataRef.entityMan = new EntityManager{ gameDataRef.playersSprite->playerSprite[1][(int)AllSprite::MAGIC_AOE],gameDataRef.boarSprite->spriteSlash[0].sprite};
	compteur = 3.f;
	gameDataRef.entityManPVP = new EntityManagerPVP{};
	compteur = 4.f;
	statesRef.resize(COUNT_GAMESTATE);
	gameDataRef.levelManager = new LevelManager{ gameDataRef.circleInvoc };
	compteur = 5.f;
	gameDataRef.troaDimmenssionSahMaire = new TroaDimmenssionSahMaire{};
	compteur = 6.f;
	statesRef[MENU] = new Menu(gameDataRef);
	compteur = 7.f;
	statesRef[NETWORK_CONNECTION] = new Multiplayer(gameDataRef);
	compteur = 8.f;
	statesRef[CHARACTER_SELECTION] = new CharacterSelection(gameDataRef);
	compteur = 9.f;
	statesRef[GAME] = new Game(gameDataRef);
	compteur = 10.f;
	statesRef[SETTINGS] = new Settings(gameDataRef);
	compteur = 11.f;
	statesRef[PAUSE] = new Pause(gameDataRef);
	compteur = 12.f;
	statesRef[UPGRADE] = new Upgrade(gameDataRef);
	compteur = 13.f;
	statesRef[GAMEOVER] = new GameOver(gameDataRef);
	compteur = 14.f;
	statesRef[SHOP] = new Shop(gameDataRef);
	compteur = 15.f;
	statesRef[ARENA] = new Arena(gameDataRef);
	compteur = 16.f;

	Sleep(1000);
	loadingScreenFinish = true;
}

sf::Font LoadingScreen::GetFont()
{
	return font;
}

void LoadingScreen::LoadSound(Audio& _audio)
{
	// Global Sounds
#pragma region Global_Sounds
	_audio.AddSound("Assets/Sounds/Global_SFX/SwordCursor.wav", "SWORD_CURSOR", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Global_SFX/SwordCursorHinHin.wav", "SWORD_CURSOR_HINHIN", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Global_SFX/ButtonClickEffect.wav", "BUTTON_CLICK_EFFECT", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Global_SFX/ButtonWoodClickEffect.wav", "BUTTON_WOOD_CLICK_EFFECT", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Global_SFX/CharacterSelectionTransitionSound.wav", "BUTTON_TRANSITION_CHARACTER_SELECTION", playerSoundCategory::OSEF);

#pragma endregion

	//Merchant
#pragma region Merchant
	_audio.AddSound("Assets/Sounds/Merchant/MerchantBuy1.wav", "MERCHANT_BUY1", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Merchant/MerchantBuy2.wav", "MERCHANT_BUY2", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Merchant/MerchantBuy3.wav", "MERCHANT_BUY3", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Merchant/MerchantBuy4.wav", "MERCHANT_BUY4", playerSoundCategory::OSEF);

	_audio.AddSound("Assets/Sounds/Merchant/MerchantGoodBye1.wav", "MERCHANT_GOODBYE1", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Merchant/MerchantNoPotion1.wav", "MERCHANT_NOPOTION1", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Merchant/MerchantWelcome1.wav", "MERCHANT_WELCOME1", playerSoundCategory::OSEF);

#pragma endregion

	// Global Player
#pragma region Global_Player

	_audio.AddSound("Assets/Sounds/Players/SFX/WalkFirstFeet.wav", "WALK_PLAYER_FIRST_FEET", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/WalkSecondFeet.wav", "WALK_PLAYER_SECOND_FEET", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/LightAttackOnHazelnut.wav", "LIGHT_ATTACK_ON_HAZELNUT", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/HeavyAttackBam.wav", "HEAVY_ATTACK_BAM", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/ArrowShoot.wav", "ARROW_SHOOT", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/ArrowLand.wav", "ARROW_LAND", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/LightAttack.wav", "LIGHT_ATTACK", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/HeavyAttack.wav", "HEAVY_ATTACK", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/PlayerParry.wav", "PLAYER_PARRY", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/FireBallTouch.wav", "FIREBALL", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/ElectricBallTouch.wav", "ELECTRICBALL", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/PoisonBallTouch.wav", "POISONBALL", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/IceBallTouch.wav", "ICEBALL", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/PlayerEatScream1.wav", "PLAYER_EAT", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/AngelVoiceRez.wav", "PLAYER_PRAY", playerSoundCategory::OSEF);
	
	//Player Yellow
#pragma region Player_Yellow
	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowGetHitScream1.wav", "PLAYER_YELLOW_GET_HIT_1", playerSoundCategory::GET_HIT);
	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowGetHitScream2.wav", "PLAYER_YELLOW_GET_HIT_2", playerSoundCategory::GET_HIT);
	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowGetHitScream3.wav", "PLAYER_YELLOW_GET_HIT_3", playerSoundCategory::GET_HIT);

	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowDeathScream1.wav", "PLAYER_YELLOW_DEATH_SCREAM_1", playerSoundCategory::DEATH);

	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowLightSpellScream1.wav", "PLAYER_YELLOW_LIGHT_SPELL_SCREAM_1", playerSoundCategory::SPELL_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowHeavySpellScream1.wav", "PLAYER_YELLOW_HEAVY_SPELL_SCREAM_1", playerSoundCategory::SPELL_SCREAM);

	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowLightAttackScream1.wav", "PLAYER_YELLOW_LIGHT_ATTACK_SCREAM_1", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowLightAttackScream2.wav", "PLAYER_YELLOW_LIGHT_ATTACK_SCREAM_2", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowLightAttackScream3.wav", "PLAYER_YELLOW_LIGHT_ATTACK_SCREAM_3", playerSoundCategory::ATTACK_SCREAM);

	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowHeavyAttackScream1.wav", "PLAYER_YELLOW_HEAVY_ATTACK_SCREAM_1", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowHeavyAttackScream2.wav", "PLAYER_YELLOW_HEAVY_ATTACK_SCREAM_2", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowHeavyAttackScream3.wav", "PLAYER_YELLOW_HEAVY_ATTACK_SCREAM_3", playerSoundCategory::ATTACK_SCREAM);

	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowJumpScream1.wav", "PLAYER_YELLOW_JUMP_SCREAM_1", playerSoundCategory::JUMP);
	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowLandScream1.wav", "PLAYER_YELLOW_LAND_SCREAM_1", playerSoundCategory::JUMP);

	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowNoManaScream1.wav", "PLAYER_YELLOW_NOMANA_SCREAM_1", playerSoundCategory::NOMANA);
	_audio.AddSound("Assets/Sounds/Players/Yellow/PlayerYellowREZScream1.wav", "PLAYER_YELLOW_REZ_SCREAM_1", playerSoundCategory::REZ);
#pragma endregion

	//Player Blue
#pragma region Player_Blue
	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueGetHitScream1.wav", "PLAYER_BLUE_GET_HIT_1", playerSoundCategory::GET_HIT);
	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueGetHitScream2.wav", "PLAYER_BLUE_GET_HIT_2", playerSoundCategory::GET_HIT);
	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueGetHitScream3.wav", "PLAYER_BLUE_GET_HIT_3", playerSoundCategory::GET_HIT);

	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueDeathScream1.wav", "PLAYER_BLUE_DEATH_SCREAM_1", playerSoundCategory::DEATH);

	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueLightSpellScream1.wav", "PLAYER_BLUE_LIGHT_SPELL_SCREAM_1", playerSoundCategory::SPELL_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueHeavySpellScream1.wav", "PLAYER_BLUE_HEAVY_SPELL_SCREAM_1", playerSoundCategory::SPELL_SCREAM);

	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueLightAttackScream1.wav", "PLAYER_BLUE_LIGHT_ATTACK_SCREAM_1", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueLightAttackScream2.wav", "PLAYER_BLUE_LIGHT_ATTACK_SCREAM_2", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueLightAttackScream3.wav", "PLAYER_BLUE_LIGHT_ATTACK_SCREAM_3", playerSoundCategory::ATTACK_SCREAM);

	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueHeavyAttackScream1.wav", "PLAYER_BLUE_HEAVY_ATTACK_SCREAM_1", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueHeavyAttackScream2.wav", "PLAYER_BLUE_HEAVY_ATTACK_SCREAM_2", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueHeavyAttackScream3.wav", "PLAYER_BLUE_HEAVY_ATTACK_SCREAM_3", playerSoundCategory::ATTACK_SCREAM);

	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueJumpScream1.wav", "PLAYER_BLUE_JUMP_SCREAM_1", playerSoundCategory::JUMP);
	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueLandScream1.wav", "PLAYER_BLUE_LAND_SCREAM_1", playerSoundCategory::JUMP);

	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueNoManaScream1.wav", "PLAYER_BLUE_NOMANA_SCREAM_1", playerSoundCategory::NOMANA);
	_audio.AddSound("Assets/Sounds/Players/Blue/PlayerBlueREZScream1.wav", "PLAYER_BLUE_REZ_SCREAM_1", playerSoundCategory::REZ);

#pragma endregion

	//Player Red
#pragma region Player_Red
	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedGetHitScream1.wav", "PLAYER_RED_GET_HIT_1", playerSoundCategory::GET_HIT);
	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedGetHitScream2.wav", "PLAYER_RED_GET_HIT_2", playerSoundCategory::GET_HIT);
	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedGetHitScream3.wav", "PLAYER_RED_GET_HIT_3", playerSoundCategory::GET_HIT);

	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedDeathScream1.wav", "PLAYER_RED_DEATH_SCREAM_1", playerSoundCategory::DEATH);

	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedLightSpellScream1.wav", "PLAYER_RED_LIGHT_SPELL_SCREAM_1", playerSoundCategory::SPELL_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedHeavySpellScream1.wav", "PLAYER_RED_HEAVY_SPELL_SCREAM_1", playerSoundCategory::SPELL_SCREAM);

	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedLightAttackScream1.wav", "PLAYER_RED_LIGHT_ATTACK_SCREAM_1", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedLightAttackScream2.wav", "PLAYER_RED_LIGHT_ATTACK_SCREAM_2", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedLightAttackScream3.wav", "PLAYER_RED_LIGHT_ATTACK_SCREAM_3", playerSoundCategory::ATTACK_SCREAM);

	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedHeavyAttackScream1.wav", "PLAYER_RED_HEAVY_ATTACK_SCREAM_1", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedHeavyAttackScream2.wav", "PLAYER_RED_HEAVY_ATTACK_SCREAM_2", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedHeavyAttackScream3.wav", "PLAYER_RED_HEAVY_ATTACK_SCREAM_3", playerSoundCategory::ATTACK_SCREAM);

	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedJumpScream1.wav", "PLAYER_RED_JUMP_SCREAM_1", playerSoundCategory::JUMP);
	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedLandScream1.wav", "PLAYER_RED_LAND_SCREAM_1", playerSoundCategory::JUMP);

	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedNoManaScream1.wav", "PLAYER_RED_NOMANA_SCREAM_1", playerSoundCategory::NOMANA);
	_audio.AddSound("Assets/Sounds/Players/Red/PlayerRedREZScream1.wav", "PLAYER_RED_REZ_SCREAM_1", playerSoundCategory::REZ);
#pragma endregion

	//Player Green
#pragma region Player_Green
	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenGetHitScream1.wav", "PLAYER_GREEN_GET_HIT_1", playerSoundCategory::GET_HIT);
	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenGetHitScream2.wav", "PLAYER_GREEN_GET_HIT_2", playerSoundCategory::GET_HIT);
	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenGetHitScream3.wav", "PLAYER_GREEN_GET_HIT_3", playerSoundCategory::GET_HIT);

	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenDeathScream1.wav", "PLAYER_GREEN_DEATH_SCREAM_1", playerSoundCategory::DEATH);

	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenLightSpellScream1.wav", "PLAYER_GREEN_LIGHT_SPELL_SCREAM_1", playerSoundCategory::SPELL_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenHeavySpellScream1.wav", "PLAYER_GREEN_HEAVY_SPELL_SCREAM_1", playerSoundCategory::SPELL_SCREAM);

	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenLightAttackScream1.wav", "PLAYER_GREEN_LIGHT_ATTACK_SCREAM_1", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenLightAttackScream2.wav", "PLAYER_GREEN_LIGHT_ATTACK_SCREAM_2", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenLightAttackScream3.wav", "PLAYER_GREEN_LIGHT_ATTACK_SCREAM_3", playerSoundCategory::ATTACK_SCREAM);

	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenHeavyAttackScream1.wav", "PLAYER_GREEN_HEAVY_ATTACK_SCREAM_1", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenHeavyAttackScream2.wav", "PLAYER_GREEN_HEAVY_ATTACK_SCREAM_2", playerSoundCategory::ATTACK_SCREAM);
	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenHeavyAttackScream3.wav", "PLAYER_GREEN_HEAVY_ATTACK_SCREAM_3", playerSoundCategory::ATTACK_SCREAM);

	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenJumpScream1.wav", "PLAYER_GREEN_JUMP_SCREAM_1", playerSoundCategory::JUMP);
	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenLandScream1.wav", "PLAYER_GREEN_LAND_SCREAM_1", playerSoundCategory::JUMP);

	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenNoManaScream1.wav", "PLAYER_GREEN_NOMANA_SCREAM_1", playerSoundCategory::NOMANA);
	_audio.AddSound("Assets/Sounds/Players/Green/PlayerGreenREZScream1.wav", "PLAYER_GREEN_REZ_SCREAM_1", playerSoundCategory::REZ);
#pragma endregion



#pragma endregion

	//Mobs
#pragma region Mobs

	// Hazelnut
	_audio.AddSound("Assets/Sounds/Enemies/Hazelnut/HazelnutAttack1.wav", "HAZELNUT_ATTACK_1", playerSoundCategory::HAZELNUT);
	_audio.AddSound("Assets/Sounds/Enemies/Hazelnut/HazelnutAttack2.wav", "HAZELNUT_ATTACK_2", playerSoundCategory::HAZELNUT);
	_audio.AddSound("Assets/Sounds/Enemies/Hazelnut/HazelnutAttack3.wav", "HAZELNUT_ATTACK_3", playerSoundCategory::HAZELNUT);
	_audio.AddSound("Assets/Sounds/Enemies/Hazelnut/HazelnutGetHit1.wav", "HAZELNUT_GET_HIT_1", playerSoundCategory::HAZELNUT);
	_audio.AddSound("Assets/Sounds/Enemies/Hazelnut/HazelnutGetHit2.wav", "HAZELNUT_GET_HIT_2", playerSoundCategory::HAZELNUT);
	_audio.AddSound("Assets/Sounds/Enemies/Hazelnut/HazelnutGetHit3.wav", "HAZELNUT_GET_HIT_3", playerSoundCategory::HAZELNUT);
	_audio.AddSound("Assets/Sounds/Enemies/Hazelnut/HazelnutDeath.wav", "HAZELNUT_DEATH", playerSoundCategory::HAZELNUT);

	// Mushroom					  
	_audio.AddSound("Assets/Sounds/Enemies/Mushroom/MushroomAttack1.wav", "MUSHROOM_ATTACK1", playerSoundCategory::MUSHROOM);
	_audio.AddSound("Assets/Sounds/Enemies/Mushroom/MushroomGetHit.wav", "MUSHROOM_GET_HIT", playerSoundCategory::MUSHROOM);
	_audio.AddSound("Assets/Sounds/Enemies/Mushroom/MushroomDeath.wav", "MUSHROOM_DEATH", playerSoundCategory::MUSHROOM);
	
	// Pig					  
	_audio.AddSound("Assets/Sounds/Enemies/Pig/PigAttack1.wav", "PIG_ATTACK", playerSoundCategory::PIG);
	_audio.AddSound("Assets/Sounds/Enemies/Pig/PigGetHit1.wav", "PIG_GET_HIT", playerSoundCategory::PIG);
	_audio.AddSound("Assets/Sounds/Enemies/Pig/PigDeath1.wav", "PIG_DEATH", playerSoundCategory::PIG);
	
	// Bird					  
	_audio.AddSound("Assets/Sounds/Enemies/Bird/BirdAttack1.wav", "BIRD_ATTACK", playerSoundCategory::BIRD);
	_audio.AddSound("Assets/Sounds/Enemies/Bird/BirdGetHit1.wav", "BIRD_GET_HIT", playerSoundCategory::BIRD);
	_audio.AddSound("Assets/Sounds/Enemies/Bird/BirdDeath1.wav", "BIRD_DEATH", playerSoundCategory::BIRD);

	// Hamurai
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigPunch.wav", "BOAR_SCREAM_ATTACK1", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigSword1.wav", "BOAR_ATTACK1", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigSword2.wav", "BOAR_ATTACK2", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigSuffering.wav", "BOAR_SUFFERING", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigDeath.wav", "BOAR_DEATH", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigStep.wav", "BOAR_STEP", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/ZenitsuSlash.wav", "BOAR_ZENITSU", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigBreathless.wav", "BOAR_BREATHLESS", playerSoundCategory::HAMOURAI);

	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigGroink1.wav", "BOAR_GROINK_1", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigGroink2.wav", "BOAR_GROINK_2", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigGroink3.wav", "BOAR_GROINK_3", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigGroink4.wav", "BOAR_GROINK_4", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigGroink5.wav", "BOAR_GROINK_5", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigGroink6.wav", "BOAR_GROINK_6", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigGroink7.wav", "BOAR_GROINK_7", playerSoundCategory::HAMOURAI);
	_audio.AddSound("Assets/Sounds/Enemies/Hamurai/PigGroink8.wav", "BOAR_GROINK_8", playerSoundCategory::HAMOURAI);

	// Winoaky
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchSpellBrambleScream1.wav", "WITCH_BRAMBLE_SCREAM_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchUltimateScream1.wav", "WITCH_ULTIMATE_SCREAM_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchGetHitScream1.wav", "WITCH_GET_HIT_SCREAM_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchGetHitScream2.wav", "WITCH_GET_HIT_SCREAM_2", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchGetHitScream3.wav", "WITCH_GET_HIT_SCREAM_3", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchSummonHazelnutScream1.wav", "WITCH_SUMMON_SCREAM_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchDeathScream1.wav", "WITCH_DEATH_SCREAM_1", playerSoundCategory::WINOAKY);

	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchDeathSound1.wav", "WITCH_DEATH_SOUND_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchCristalGetHitSound1.wav", "WITCH_CRISTAL_SOUND_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchCristalDestroySound1.wav", "WITCH_CRISTAL_DESTROY_SOUND_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchCristalSoundAmbiance1.wav", "WITCH_CRISTAL_SOUND_AMBIANCE_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchSummonHazelnutSound1.wav", "WITCH_SUMMON_SOUND_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchSummonPlantSound1.wav", "WITCH_SUMMON_PLANT_SOUND_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchShieldBrokenSound1.wav", "WITCH_SHIELD_BROKEN_SOUND_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchPlantEatSound1.wav", "WITCH_PLANT_EAT_SOUND_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchPlantSpitSound1.wav", "WITCH_PLANT_SPIT_SOUND_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchPlantBurpSound1.wav", "WITCH_PLANT_BURP_SOUND_1", playerSoundCategory::WINOAKY);
	_audio.AddSound("Assets/Sounds/Enemies/Witch/WitchSpellBrambleSound1.wav", "WITCH_SPELL_BRAMBLE_SOUND_1", playerSoundCategory::WINOAKY);
	

#pragma endregion

}