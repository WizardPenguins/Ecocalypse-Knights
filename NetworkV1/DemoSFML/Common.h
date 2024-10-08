#ifndef MAIN_HPP
#define MAIN_HPP

#include "Utils.h"
#include "Screen.h"
#include <fstream>
#include "stdafx.h"

#define HEIGHT_LIMIT 475
#define HEIGHT_GAP 960
#define TROAD_GAP 1.2f
#define NB_STATES 12
#define GAP 500.f
#define SHRINKS 0.3f
#define TimerBetweenState 0.2f

constexpr float PROJECTILE_SPEED = 1500.f;
constexpr float LEVEL3_GAP = 3454.f;

struct Drop
{
	sf::Vector2f pos = {};
	sf::Sprite sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Food.png", CENTER_FOOT);
	sf::RectangleShape rect{ { (float)this->sprite.getTexture()->getSize().x / 2,(float)sprite.getTexture()->getSize().y / 2 } };
};

struct Building
{
	float posY = 0;
	sf::Sprite sprite;
	bool isAnim = false;
};



#pragma region Enum
enum class ControllerAxis
{
	LEFT_JOYSTICK_X,
	LEFT_JOYSTICK_Y,
	TRIGGERS_L2_R2,
	V,
	RIGHT_JOYSTICK_X,
	RIGHT_JOYSTICK_Y,
	DIR_CROSS_X,
	DIR_CROSS_Y
};

enum class ControllerButtonXBOX
{
	A,
	B,
	X,
	Y,
	LB,
	RB,
	SELECT,
	START,
	LEFT_STICK_PUSH,
	RIGHT_STICK_PUSH
};

enum class ControllerButtonPS4
{
	SQUARE,
	CROSS,
	CIRCLE,
	TRIANGLE,
	L1,
	R1,
	L2,
	R2,
	SELECT,
	START,
	LEFT_STICK_PUSH,
	RIGHT_STICK_PUSH,
	PLAYSTATION,
	TACTILE_PAD
};

enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT

};

enum class TypeParticle
{
	LEAF,
	CORRUPT,
};

enum GameStates
{
	PREMENU,
	MENU,
	NETWORK_CONNECTION,
	CHARACTER_SELECTION,
	GAME,
	SETTINGS,
	PAUSE,
	UPGRADE,
	GAMEOVER,
	SHOP,
	ARENA,
	COUNT_GAMESTATE
};

enum class TypeMob
{
	CAC,
	Distance,
	Mage,
	Buffer,
	Boss
};

enum class MobState
{
	Wait,
	Alive,
	Hit,
	Death,
	Dispawn,
};

enum class HazelState
{
	Idle,
	Target,
	Attack,
	Death,
	InFall,
	InUp,
	Knockback,
	Knockdown,
	NbState,
};

enum class MushroomState
{
	Idle,
	Target,
	Attack,
	Death,
	InFall,
	InUp,
	Knockback,
	Knockdown,
	NbState,
};

enum class PigState
{
	Target,
	Attack,
	Death,
	InFall,
	InUp,
	Knockback,
	Knockdown,
	NbState,
};

enum class BirdState
{
	Idle,
	Attack,
	Death,
	Hit,
	NbState,
};

enum class BoarState
{
	Idle,
	Target,
	Back,
	AttackNormal,//attack poing
	AttackH,//slash horizontale
	AttackV,//slash verticale
	AttackZenitsu,
	Shield,
	KnockBack,
	KnockDown,
	Death,
	NbState
};

enum class WitchState
{
	Idle,
	Invoc,
	Bramble,
	Invincible,
	Plante,
	Scream,
	Death,
	Normal,
	NbState,
};

enum class CrystalState
{
	Full,
	Middle,
	End,
	NbState,
};

enum LevelState
{
	MAP,
	LEVELSHOP,
	LEVEL1,
	LEVEL2,
	LEVEL3,
	LEVEL4
};

enum class Buff
{
	Life,
	Damage,
	Speed
};

enum Wave
{
	WAVE1,
	WAVE2,
	WAVE3,
	WAVE4,
	WAVE5,
	WAVE6
};

enum class AutorProjectile
{
	PLAYER,
	MOB
};

enum class SortVector
{
	Player,
	Mob,
	Projectile,
	Building,
	Food,
	Scarecrow,
	Ice,
	Crystal,
	Plante,
	Bramble,
	SlashV,
	SlashH,
	SleepingNut,
};

enum class TypeProjectile
{
	ARROW,
	DISTANCE_MAGIC_FIRE,
	DISTANCE_MAGIC_POISON,
	DISTANCE_MAGIC_ICE,
	DISTANCE_MAGIC_ELECTRICAL,
};

enum class Status
{
	None,
	OnFire,
	OnPoison,
	OnIce,
	OnElec,
	OnFuzzy,
};

enum class ChaosWin
{
	FT5,
	FT10,
	Infinity,
};

#pragma endregion
struct Particles
{
	sf::Vector2f pos;
	sf::Vector2f variationSpeed;
	float rotation;
	float scale;
	float scaleVariation;
	bool isUp = true;
	TypeParticle type;
};
class Animation;


struct Projectile
{
	int direction;
	sf::Vector2f pos;
	bool asAnime = false;
	sf::Vector2f posAutor;
	sf::Sprite sprite = Utils::LoadSprite("Assets/Images/Entities/Arrow.png", CENTER);
	int nbFrameX = 1;
	float speedAnime;
	Animation* projectileAnime;
	sf::RectangleShape* hitBox;
	AutorProjectile autor;
	TypeProjectile projetileType;
	int damage;
	int playerId;

	void Update(float _dt);

	void Display(sf::RenderTexture& _red, sf::RenderTexture& _blue)
	{
		hitBox->setPosition(pos);

		sprite.setPosition(pos.x + (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f), pos.y);
		_red.draw(sprite);

		sprite.setPosition(pos.x - (TROAD_GAP * (-1 * pos.y + WORKING_HEIGHT) / 100.f), pos.y);
		_blue.draw(sprite);
	}

	void Display(sf::RenderWindow& _window)
	{
		hitBox->setPosition(pos);
		sprite.setPosition(pos);
		_window.draw(sprite);
	}
};

struct Sort
{
	float posY;
	int i; //pos in vector
	SortVector sort; //wich vector
	int j; // for occasional 
};

struct HudBarMob
{
	sf::Sprite lifeBar = Utils::LoadSprite("Assets/Images/HUD/EnemyHPBar.png", CENTER);
	sf::Sprite pixelLife = Utils::LoadSprite("Assets/Images/HUD/EnemyHPPixel.png", TOP_LEFT);
};

struct HudBarBoss
{
	sf::Sprite lifeBar = Utils::LoadSprite("Assets/Images/HUD/BossHP/BossBar.png", CENTER);
	sf::Sprite pixelLifeBlue = Utils::LoadSprite("Assets/Images/HUD/BossHP/BossHPHigh.png", TOP_LEFT);
	sf::Sprite pixelLifeRed = Utils::LoadSprite("Assets/Images/HUD/BossHP/BossHPLow.png", TOP_LEFT);
	sf::Sprite changePhase = Utils::LoadSprite("Assets/Images/HUD/BossHP/BossCursor.png", CENTER);
	sf::Sprite headBossShadow;
	sf::Sprite headBoss;
};

class Entity;
class Player;
class Mob;
class Scarecrow;
class EntityManager;
class EntityManagerPVP;
class LevelManager;

class LoadingScreen;
class Upgrade;

class Audio;

class HUD;
class Camera;

class Network;

class PopUp;
struct StatusSprite;
struct ScarecrowSprite;
struct PigSprite;
struct HazelSprite;
struct MushroomSprite;
struct BirdSprite;
struct BoarSprite;
struct WitchSprite;
struct PlayersSprites;
class FlyingNut;
class TroaDimmenssionSahMaire;

class GameData
{
	//friend class EntityManager;
public:
	//Constructor/Destructor
	GameData();
	~GameData() = default;
	void InitSprite();
	void InitFlame();
	void InitCircleInvoc();
	void InitSpriteInventory();

	//Get/set
	void ChangeState(unsigned char _newState);
	void ChangeLevelState(LevelState _newLevelState);

	//Var
	sf::RenderWindow window;
	sf::Clock dtClock = sf::Clock();
	sf::Clock fpsClock;
	sf::Event event = sf::Event();
	float dt = 0.f;
	float timerButton = 0.f;

	int displayMode = 0;

	LoadingScreen* loadingScreen = nullptr;
	Upgrade* upgradeScreen = nullptr;
	Audio* audio = nullptr;
	HUD* hud = nullptr;
	Camera* camera = nullptr;
	Network* network = nullptr;
	EntityManager* entityMan = nullptr;
	EntityManagerPVP* entityManPVP = nullptr;
	LevelManager* levelManager = nullptr;

	HudBarMob hudBarMob;
	HudBarBoss hudBarBoss;
	ScarecrowSprite* scarecrowSprite = nullptr;
	PlayersSprites* playersSprite = nullptr;
	BoarSprite* boarSprite = nullptr;
	HazelSprite* hazelSprite = nullptr;
	MushroomSprite* mushroomSprite = nullptr;
	BirdSprite* birdSprite = nullptr;
	PigSprite* pigSprite = nullptr;
	WitchSprite* witchSprite = nullptr;
	StatusSprite* statusSprite = nullptr;
	FlyingNut* flyingNut = nullptr;
	TroaDimmenssionSahMaire* troaDimmenssionSahMaire = nullptr;

	LevelState levelState;

	sf::Time elapsed;

	sf::RenderTexture renderTextureBlue;
	sf::RenderTexture renderTextureRed;

	std::vector<Mob*> tabMob;
	std::vector<Scarecrow*> scarecrow;
	std::vector<Drop*> drop;
	std::vector<Player*> tabPlayers;
	std::vector<Building*> tabBuilding;
	std::vector<Building*> sleepingNut;
	std::vector<Sort*> sortVector;
	std::vector<Particles*> tabParticle;

	unsigned char gameState = MENU;
	unsigned char lastState = MENU;

	bool inTravel = false;
	int compteurPlayer = 0;
	Wave actualWave = WAVE1;
	bool mobRespawn = false;
	bool multiplayer = false;
	bool reloadArena = false;
	bool reloadGame = false;
	bool reloadMenu = false;
	bool reloadCamera = false;
	bool reloadCameraMenu = false;
	bool reloadCharacterSelection = false;
	bool reloadShop = false;
	bool reloadGameOver = false;
	bool reloadFlyingNut = false;
	bool showMobHpBar = false;
	int idPlayerInUpgrade = 0;
	bool is3DOn = false;
	int FrameCount3D = 0;
	bool friendShip = true;
	bool TutoActive = true;
	bool rightEye = true;
	bool arenaIsChose = false;
	bool teamfight = false;
	bool inCinematic = false;

	sf::RenderTexture* rt;
	sf::RenderTexture* rtRed;
	sf::RenderTexture* rtBlue;
	sf::Sprite rtShadow;
	sf::Sprite rtRedShadow;
	sf::Sprite rtBlueShadow;
	sf::Sprite shadow;

	sf::Vector2f startPosMenuSwordSprite;
	sf::Vector2f startPosPauseSwordSprite;
	sf::Vector2f startPosPauseSwordSpriteMulti;
	sf::Sprite swordSpriteMenu;
	sf::Sprite heartHit;
	sf::Sprite commingSoon;
	sf::Sprite circleInvoc;
	sf::Sprite spriteIconHealthPotion;
	sf::Sprite spriteIconSpeedBuffPotion;
	sf::Sprite spriteIconStrengthBuffPotion;
	sf::Sprite spriteIconMagicBuffPotion;
	
	std::vector<int> playerSelected;
	std::vector<int> playerTeam;
	std::vector<Projectile> projectile;

	std::vector<PopUp*> popUpVector;
	int currentPopUp = 0;
	sf::Image tempImage1;
	sf::Image tempImage2;
	sf::Image tempImage3;
	sf::Font* GameFont;
	ChaosWin choasWin = ChaosWin::Infinity;

	sf::Vector2f lastPosInMap = { 929.f, 1384.f };

	sf::Sprite flame;
	Animation* animFlame;

	sf::Sprite leaf;
	sf::Sprite corruption;
};

#endif