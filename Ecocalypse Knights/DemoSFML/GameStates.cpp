#include "stdafx.h"
#include "GameStates.h"
#include "Mob.hpp"
#include "Player.hpp"
#include "Multiplayer.h"
#include "EntityManager.h"
#include "Network.h"
#include "Upgrade.h"
#include "TroaDimmenssionSahMaire.h"
#include "Shop.h"
#include "Common.h"
#include <thread>

//FORWARD DECLARATION///////////////////////////////////////////////////////////////////////////////////
void LoadSound(Audio& _audio);																	
void InitCamera(std::vector<States*>& _states, GameData& _gameData,int&);							////////
void InitSound(std::vector<States*>& _states, GameData& _gameData,int&);							////////
void InitSprite(std::vector<States*>& _states, GameData& _gameData,int&);							////////
void InitEntityManager(std::vector<States*>& _states, GameData& _gameData,int&);					////////
void InitEntityPVP(std::vector<States*>& _states, GameData& _gameData,int&);						////////
void InitLevelManager(std::vector<States*>& _states, GameData& _gameData,int&);						////////
void InitMenu(std::vector<States*>& _states, GameData& _gameData,int&);								////////
void InitNetwork(std::vector<States*>& _states, GameData& _gameData,int&);							////////
void InitCharacterSelection(std::vector<States*>& _states, GameData& _gameData,int&);				////////
void InitGame(std::vector<States*>& _states, GameData& _gameData,int&);								////////
void InitSettings(std::vector<States*>& _states, GameData& _gameData,int&);							////////
void InitPause(std::vector<States*>& _states, GameData& _gameData,int&);							////////
void InitUpgrade(std::vector<States*>& _states, GameData& _gameData,int&);							////////
void InitGameOver(std::vector<States*>& _states, GameData& _gameData,int&);							////////
void InitShop(std::vector<States*>& _states, GameData& _gameData,int&);								////////
void InitArena(std::vector<States*>& _states, GameData& _gameData,int&);								
////////////////////////////////////////////////////////////////////////////////////////////////////////

//CHUT TA RIEN VU
struct id
{
	int id;
	float timer = 0.1f;
};
std::vector<std::thread> vecOfThreads;
std::vector<id*> vecOfThreadsId;
bool tabThread[NbFunct] = { false,false,false,false,false,false,false,false,false,false,false,false,false,false };
void (*ptr[NbFunct])(std::vector<States*>&, GameData&,int&) = { &InitSprite ,&InitCamera ,&InitSound ,&InitEntityManager ,&InitEntityPVP,&InitMenu,&InitNetwork,&InitCharacterSelection,&InitSettings ,&InitPause ,&InitUpgrade,&InitGameOver,&InitShop,&InitArena,&InitLevelManager,&InitGame };

GameState::GameState(GameData& _gameData)
{
	_gameData.displayMode = sf::Style::Fullscreen;
	_gameData.window.create(sf::VideoMode::getDesktopMode(), "Ecocalypse Knights", _gameData.displayMode);

	sf::Image icon;
	icon.loadFromFile("Assets/Images/Hud/Red_Head.png");
	_gameData.window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	_gameData.window.setFramerateLimit(144);
	_gameData.window.setMouseCursorVisible(false);
	_gameData.window.clear(sf::Color::Black);

	_gameData.renderTextureBlue.create(_gameData.window.getSize().x, _gameData.window.getSize().y);
	_gameData.renderTextureRed.create(_gameData.window.getSize().x, _gameData.window.getSize().y);

	//_gameData.window.setMouseCursorVisible(false);
	_gameData.audio = new Audio{};
	_gameData.audio->AddMusic("Assets/Sounds/Global_SFX/LoadingScreen.wav", "LoadingScreen");

	Screen::Init(_gameData.window);
	_gameData.audio->PlayMusic("LoadingScreen", true);
	states.resize(COUNT_GAMESTATE);

	//init splash Screen
	InitSplashScreen();
	RunSplashScreen(_gameData);

	//variable for multithread
	LoadingScreen gt(_gameData, states);
	int numThreads = std::thread::hardware_concurrency() - 1;

	// Add a Thread object to vector
	for (int i = 0; i < numThreads; i++)
	{
		if (currentFunct < NbFunct)
		{
			id* temp = new id{ currentFunct };
			vecOfThreadsId.push_back(temp);
			vecOfThreads.emplace_back(std::thread(ptr[currentFunct], std::ref(this->states), std::ref(_gameData), std::move(std::ref(vecOfThreadsId.at(i)->id))));
			currentFunct++;
		}
	}

	//display and update
	RunLoadingScreen(_gameData, gt);

	_gameData.dtClock.restart();
	_gameData.dt = 0.f;
}

GameState::~GameState()
{
	while (!states.empty())
	{
		delete states.back();
		states.pop_back();
	}
}

void GameState::Init(GameData& _gameData)
{
	_gameData.levelManager->Init(_gameData.levelState, _gameData);
	states[UPGRADE]->Init(_gameData);
	states[GAME]->Init(_gameData);
}

void GameState::RunSplashScreen(GameData& _gameData)
{
	sf::Clock clockTemp;
	float dtTemp;
	while (!this->endOfLogo)
	{
		dtTemp = clockTemp.restart().asSeconds();
		while (_gameData.window.pollEvent(_gameData.event))
		{
			if (_gameData.event.type == sf::Event::Closed)
			{
				_gameData.window.close();
			}
		}

		this->timerButton += dtTemp;
		JoysticPressed();
		UpdateSplashScreen(dtTemp);
		DisplaySplashScreen(_gameData.window);
	}
}

void GameState::RunLoadingScreen(GameData& _gameData, LoadingScreen& gt)
{
	while (!gt.loadingScreenFinish)
	{
		while (_gameData.window.pollEvent(_gameData.event))
		{
			if (_gameData.event.type == sf::Event::Closed)
			{
				_gameData.window.close();
			}
		}
		_gameData.window.clear(sf::Color(45, 45, 63));

		gt.loadingScreenCompteur = gt.compteur / NB_INIT;

		gt.UpdateAnim(_gameData.window);

		_gameData.window.draw(gt.backChargingBar);
		gt.frontChargingBar.setSize({ (gt.loadingScreenCompteur * gt.backChargingBar.getSize().x), gt.backChargingBar.getSize().y });

		if (gt.compteur == NB_INIT)
		{
			gt.frontChargingBar.setSize(gt.backChargingBar.getSize());
		}

		_gameData.window.draw(gt.frontChargingBar);
		_gameData.window.draw(gt.funnySentence);
		_gameData.window.display();
		UpdateMultiThread(_gameData, gt);
	}
}

void GameState::UpdateSplashScreen(float _dt)
{
	if (this->onCrea)
	{
		if (this->fadeIn)
		{
			this->creaLogo.setColor(sf::Color(255, 255, 255, this->creaLogo.getColor().a + _dt * SpeedFade));
			if (this->creaLogo.getColor().a >= 245)
			{
				this->fadeOut = true;
				this->fadeIn = false;
			}
		}
		else if (this->fadeOut)
		{
			this->creaLogo.setColor(sf::Color(255, 255, 255, this->creaLogo.getColor().a - _dt * SpeedFade));
			if (this->creaLogo.getColor().a <= 5)
			{
				this->fadeOut = false;
				this->fadeIn = true;
				this->onWizard = true;
				this->onCrea = false;
			}
		}
	}
	else if (this->onWizard)
	{
		if (this->fadeIn)
		{
			this->wizardLogo.setColor(sf::Color(255, 255, 255, this->wizardLogo.getColor().a + _dt * SpeedFade));
			if (this->wizardLogo.getColor().a >= 245)
			{
				this->fadeOut = true;
				this->fadeIn = false;
			}
		}
		else if (this->fadeOut)
		{
			this->wizardLogo.setColor(sf::Color(255, 255, 255, this->wizardLogo.getColor().a - _dt * SpeedFade));
			if (this->wizardLogo.getColor().a <= 5)
			{
				this->fadeOut = false;
				this->fadeIn = true;
				this->onWizard = false;
				this->endOfLogo = true;
			}
		}
	}
}

void GameState::DisplaySplashScreen(sf::RenderWindow& _window)
{
	_window.clear(sf::Color::Black);

	if (this->onCrea)
	{
		_window.draw(this->creaLogo);
	}
	else if (this->onWizard)
	{
		_window.draw(this->wizardLogo);
	}

	_window.display();
}

void GameState::InitSplashScreen()
{
	//for splash Screen
	this->creaLogo = Utils::LoadSprite("Assets/Images/PreMenu/logoCreajeux.png", CENTER);
	this->wizardLogo = Utils::LoadSprite("Assets/Images/PreMenu/Logo.png", CENTER);

	this->creaLogo.setPosition(sf::Vector2f{ WORKING_WIDTH / 2,WORKING_HEIGHT / 2 });
	this->wizardLogo.setPosition(sf::Vector2f{ WORKING_WIDTH / 2,WORKING_HEIGHT / 2 });

	this->creaLogo.setColor(sf::Color(255, 255, 255, 0));
	this->wizardLogo.setColor(sf::Color(255, 255, 255, 0));
}

void GameState::JoysticPressed()
{
	if (sf::Joystick::isConnected(0))
	{
		if (sf::Joystick::getIdentification(0).name == "Wireless Controller")
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonPS4::CROSS))
			{
				if (this->onCrea && this->timerButton >= 0.5f)
				{
					this->onCrea = false;
					this->onWizard = true;
					this->fadeOut = false;
					this->fadeIn = true;
					this->timerButton = 0.f;
				}
				else if (this->onWizard && this->timerButton >= 0.5f)
				{
					this->endOfLogo = true;
				}
			}
		}
		else
		{
			if (sf::Joystick::isButtonPressed(0, (unsigned int)ControllerButtonXBOX::A))
			{
				if (this->onCrea && this->timerButton >= 0.5f)
				{
					this->onCrea = false;
					this->onWizard = true;
					this->fadeOut = false;
					this->fadeIn = true;
					this->timerButton = 0.f;
				}
				else if (this->onWizard && this->timerButton >= 0.5f)
				{
					this->endOfLogo = true;
				}
			}
		}
	}
}

void GameState::Reload(void)
{

}

void GameState::ReloadNewGame(void)
{
}

void GameState::Update(GameData& _gameData)
{
	_gameData.dt = _gameData.dtClock.restart().asSeconds();
	if (_gameData.dt > 0.1f)
	{
		_gameData.dt = 0.1f;
	}
	_gameData.timerButton -= _gameData.dt;
	states[_gameData.gameState]->Update(_gameData);	
}

void GameState::PollEvent(GameData& _gameData)
{
	while (_gameData.window.pollEvent(_gameData.event))
	{
		if (_gameData.event.type == sf::Event::Closed)
		{
			_gameData.window.close();
		}
		if (!_gameData.camera->moveMenuToPlayerSelection && _gameData.timerButton <= 0.f)
		{
			states[_gameData.gameState]->KeyPressed(_gameData);
			states[_gameData.gameState]->MousePressed(_gameData);
			states[_gameData.gameState]->MouseMoved(_gameData);
			states[_gameData.gameState]->MouseReleased(_gameData);
			states[_gameData.gameState]->LostFocus(_gameData);
			states[_gameData.gameState]->JoystickPressed(_gameData);
		}
	}
}

void GameState::Display(GameData& _gameData)
{
	_gameData.window.clear(sf::Color::Black);
	_gameData.renderTextureRed.clear();
	_gameData.renderTextureBlue.clear();
	_gameData.rt->clear(sf::Color::Transparent);
	_gameData.rtSrinks->clear(sf::Color::Transparent);

	if (_gameData.gameState == PAUSE && !_gameData.is3DOn)
	{
		states[_gameData.lastState]->Display(_gameData);
	}
	else if (_gameData.gameState == PAUSE && _gameData.is3DOn)
	{
		states[_gameData.lastState]->Display3D(_gameData);
	}

	if (_gameData.is3DOn)
	{
		states[_gameData.gameState]->Display3D(_gameData);

		_gameData.renderTextureRed.display();
		_gameData.renderTextureBlue.display();

		sf::Sprite spriteBlue(_gameData.renderTextureBlue.getTexture());
		sf::Sprite spriteRed(_gameData.renderTextureRed.getTexture());

		spriteRed.setColor({ (sf::Uint8)(spriteRed.getColor().r * 0.299f),(sf::Uint8)(spriteRed.getColor().g * 0.587f),(sf::Uint8)(spriteRed.getColor().b * 0.114f) });
		spriteRed.setColor({ (sf::Uint8)(spriteRed.getColor().r * 255),(sf::Uint8)(spriteRed.getColor().g * 0),(sf::Uint8)(spriteRed.getColor().b * 0),255 });

		spriteBlue.setColor({ (sf::Uint8)(spriteBlue.getColor().r * 0.299f),(sf::Uint8)(spriteBlue.getColor().g * 0.587f),(sf::Uint8)(spriteBlue.getColor().b * 0.114f) });
		spriteBlue.setColor({ (sf::Uint8)(spriteBlue.getColor().r * 0),(sf::Uint8)(spriteBlue.getColor().g * 255),(sf::Uint8)(spriteBlue.getColor().b * 255),255 });

		if (_gameData.gameState == CHARACTER_SELECTION)
		{
			spriteRed.setPosition(0, 0 + HEIGHT_GAP - 1.f );
			spriteBlue.setPosition(0, 0 + HEIGHT_GAP - 1.f);

			if (_gameData.camera->moveMenuToPlayerSelection)
			{
				float ySlide = _gameData.camera->lastCenter.y - (WORKING_HEIGHT / 2);
				spriteRed.setPosition(0, ySlide);
				spriteBlue.setPosition(0, ySlide);
			}
		}
		else if (_gameData.gameState == MENU || _gameData.gameState == SETTINGS)
		{
			spriteRed. setPosition(0, GAP);
			spriteBlue.setPosition(0, GAP);

			if (_gameData.lastState == PAUSE && _gameData.gameState == SETTINGS)
			{
				_gameData.window.setView(_gameData.window.getDefaultView());
				_gameData.renderTextureRed.setView(_gameData.renderTextureRed.getDefaultView());
				_gameData.renderTextureBlue.setView(_gameData.renderTextureBlue.getDefaultView());

				spriteRed.setPosition(0, 0);
				spriteBlue.setPosition(0, 0);
			}

			if (_gameData.camera->moveMenuToPlayerSelection)
			{
				float ySlide = _gameData.camera->lastCenter.y - (WORKING_HEIGHT / 2);
				spriteRed. setPosition(0, ySlide);
				spriteBlue.setPosition(0, ySlide);
			}
		}

		_gameData.window.clear();

		_gameData.window.draw(spriteBlue, sf::BlendAdd);
		_gameData.window.draw(spriteRed, sf::BlendAdd);
	}
	else
	{
		states[_gameData.gameState]->Display(_gameData);
	}

	_gameData.window.display();
	_gameData.rt->display();
	_gameData.rtSrinks->display();
}

void GameState::Run(GameData& _gameData)
{
	while (_gameData.window.isOpen())
	{
		PollEvent(_gameData);

		/*_gameData.elapsed = _gameData.fpsClock.restart();
		std::cout << "FPS : " << (1.0f / _gameData.elapsed.asSeconds()) << std::endl;*/

		Update(_gameData);
		_gameData.audio->CheckNumberSound();

		Display(_gameData);
	}
}

void GameState::UpdateMultiThread(GameData& _gameData,LoadingScreen& gt)
{
	for (int i = 0 ; i < vecOfThreads.size();i++)
	{
		// If thread Object is Joinable then Join that thread.
		if (tabThread[vecOfThreadsId.at(i)->id])
		{
			vecOfThreadsId.at(i)->timer -= 0.001f;
			if (vecOfThreadsId.at(i)->timer <= 0)
			{
				vecOfThreads[i].join();
				gt.compteur++;
				tabThread[vecOfThreadsId.at(i)->id] = false;
				if (currentFunct < NB_INIT)
				{
					vecOfThreadsId.at(i)->id = currentFunct;
					vecOfThreadsId.at(i)->timer = 0.1f;
					vecOfThreads[i] = std::thread(ptr[currentFunct], std::ref(this->states), std::ref(_gameData), std::move(std::ref(vecOfThreadsId.at(i)->id)));
					currentFunct++;
				}

				if (gt.compteur == NB_INIT)
				{
					gt.loadingScreenFinish = true;
				}
			}
		}
	}
}

void GameState::SetLastState(GameData& _gameData)
{
	_gameData.lastState = _gameData.gameState;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LoadSound(Audio& _audio)
{
	// Global Sounds
#pragma region Global_Sounds
	_audio.AddSound("Assets/Sounds/Global_SFX/SwordCursor.wav", "SWORD_CURSOR", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Global_SFX/SwordCursorHinHin.wav", "SWORD_CURSOR_HINHIN", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Global_SFX/ButtonClickEffect.wav", "BUTTON_CLICK_EFFECT", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Global_SFX/ButtonWoodClickEffect.wav", "BUTTON_WOOD_CLICK_EFFECT", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Global_SFX/CharacterSelectionTransitionSound.wav", "BUTTON_TRANSITION_CHARACTER_SELECTION", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Global_SFX/UpgradeMinus.wav", "UPGRADE_MINUS", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Global_SFX/UpgradePlus.wav", "UPGRADE_PLUS", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Global_SFX/NutWaaa.wav", "NUT_WA", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Global_SFX/Bridge2.wav", "BRIDGE", playerSoundCategory::OSEF);

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
	_audio.AddSound("Assets/Sounds/Players/SFX/Dash.wav", "PLAYER_DASH", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/LevelUp3.wav", "PLAYER_LEVELUP", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/GlouGlou.wav", "PLAYER_GLOUGLOU", playerSoundCategory::OSEF);
	_audio.AddSound("Assets/Sounds/Players/SFX/Taunt.wav", "PLAYER_TAUNT", playerSoundCategory::OSEF);

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

	// Slime
	_audio.AddSound("Assets/Images/Entities/Mobs/Witch/Witch_Death.wav", "WitchDeath", playerSoundCategory::OSEF);

#pragma endregion
}

void InitCamera(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_gameData.camera = new Camera(_gameData);
	tabThread[_id] = true;
	int k = 5;
}

void InitSound(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	LoadSound(*_gameData.audio);
	tabThread[_id] = true;
	int k = 5;
}

void InitSprite(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_gameData.InitSprite();
	tabThread[_id] = true;
	int k = 5;
}

void InitEntityManager(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_gameData.entityMan = new EntityManager{ };
	tabThread[_id] = true;
	int k = 5;
}

void InitEntityPVP(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_gameData.entityManPVP = new EntityManagerPVP{};
	tabThread[_id] = true;
	int k = 5;
}

void InitLevelManager(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_gameData.levelManager = new LevelManager{};
	tabThread[_id] = true;
	int k = 5;
}

void InitMenu(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_states[MENU] = new Menu(_gameData);
	tabThread[_id] = true;
	int k = 5;
}

void InitNetwork(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_states[NETWORK_CONNECTION] = new Multiplayer(_gameData);
	tabThread[_id] = true;
	int k = 5;
}

void InitCharacterSelection(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_states[CHARACTER_SELECTION] = new CharacterSelection(_gameData);
	tabThread[_id] = true;
	int k = 5;
}

void InitGame(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_states[GAME] = new Game(_gameData);
	tabThread[_id] = true;
	int k = 5;
}

void InitSettings(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_states[SETTINGS] = new Settings(_gameData);
	tabThread[_id] = true;
	int k = 5;
}

void InitPause(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_states[PAUSE] = new Pause(_gameData);
	tabThread[_id] = true;
	int k = 5;
}

void InitUpgrade(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_states[UPGRADE] = new Upgrade(_gameData);
	tabThread[_id] = true;
	int k = 5;
}

void InitGameOver(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_states[GAMEOVER] = new GameOver(_gameData);
	tabThread[_id] = true;
	int k = 5;
}

void InitShop(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_states[SHOP] = new Shop();
	tabThread[_id] = true;
	int k = 5;
}

void InitArena(std::vector<States*>& _states, GameData&  _gameData , int& _id)
{
	_states[ARENA] = new Arena(_gameData);
	tabThread[_id] = true;
	int k = 5;
}