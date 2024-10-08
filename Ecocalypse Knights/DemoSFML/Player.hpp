#ifndef PLAYER_HPP
#define PLAYER_HPP
#include "stdafx.h"

#include "Common.h"
#include "Entity.h"
#include "Animation.h"
#include "PlayersSprites.h"
#include "MobSprite.hpp"

#define STATS_NBR 4

constexpr float JUMPRATIO = 2.2f;
constexpr float TIMER_ADD_MANA = 4.f;
constexpr float TIMER_COMBO = 0.8f;
constexpr float TIMERSTATUS = 4.f;
constexpr float TIMERSHIELD = 3.f;
constexpr float TIMERDASH = 2.5f;
constexpr unsigned short int JUMPDISTANCE = 210;
constexpr unsigned short int MAXSHIELDLIFE = 9;

enum class Stats
{
	STRENGTH,
	WISDOM,
	AGILITY,
	VIGOR
};

enum PlayerStates
{
	RUN,
	JUMP,
	IDLE,
	LIGHTATTACK,
	HEAVYATTACK,
	MAGICATTACK,
	BOWATTACK,
	HIT,
	HIT_GROUND,
	KNOCKDOWN,
	DEAD,
	PARRY,
	REVIVE,
	PRAY,
	EAT_BY_BOSS,
	FREEFALL,
	DASH,
	TAUNT
};

enum class TypeAOE
{
	FIRE,
	POISON,
	ICE,
	ELECTROCUTION,
};

enum class Cinematic
{
	FRONT,
	BACK
};

struct AOEMagicAttack
{
	sf::RectangleShape* hitbox = new sf::RectangleShape;
	TypeAOE typeAOE = TypeAOE::FIRE;
	bool isActive = false;
	int dir = 0;
};

struct PlayerData
{
	//misc
	PlayerStates state;
	int lastDir;
	sf::Vector2f pos;
	int playerSelected;
	int life;
	int mana;
	//sword attacks
	bool inAir;
	bool heavyGoUp;
	int comboNumber;
	//magic
	bool auraIsActive;
	bool isMagicDistance;
	bool isAOE;
	bool isPoisonActive;
	bool isIceActive;
	AOEMagicAttack aoeMagicAttack;
	//shield et parry
	bool isParrying;
	bool isBreaking;
	int shieldLife;
	bool parryIdle;
	bool parryMoveRight;
	bool parryMoveLeft;
	bool isPropulsed;
	//revive
	bool canBeRevived;
	bool hasFinishPraying;
};

class Player : public Entity
{
	friend class PlayerRunState;
	friend class PlayerJumpState;
	friend class PlayerKnockDownState;
	friend class PlayerLightAttackState;
	friend class PlayerHeavyAttackState;
	friend class PlayerMagicAttackState;
	friend class PlayerBowAttackState;
	friend class PlayerDeadState;
	friend class PlayerIdleState;
	friend class PlayerHitState;
	friend class PlayerHitGroundState;
	friend class PlayerParryState;
	friend class PlayerPrayState;
	friend class PlayerReviveState;
	friend class PlayerEatByBossState;
	friend class PlayerFreeFallState;
	friend class PlayerDashState;
	friend class PlayerTauntState;

public:
	sf::Sprite spriteShadow;
	//PlayerStates
	PlayerStates state = IDLE;

	//Constructor/Destructor
	Player(GameData& _gameData, int _id);
	~Player();

	//Function
	void Move(float _dt, Audio& _audio, bool& _inCinematic);
	void Fight(float _dt, std::vector<Projectile>* _arrow, Audio& _audio, bool& _multiplayer, Network& _network);
	void Init();
	void Update(GameData& _gameData, float _dtSlowMo, bool _dontMoveArena);
	void Display(sf::RenderWindow& _window, Audio& _audio, Camera* _camera, LevelState& _levelState);
	void Display(Audio& _audio, sf::RenderTexture& _textureRed, sf::RenderTexture& _textureBlue, Camera* _camera, LevelState& _levelState);
	void UpdateStatus(float _dt);
	void UpdateViperAnime(float _dt);
	void UpdateIceSpellAnime(float _dt);
	void UpdateAnime(float _dt);
	void UpdateViperHitBox(void);
	void UpdateIceSpellHitBox(void);
	void DisplayViperSpell(sf::RenderWindow& _window);
	void DisplayViperSpell(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayIceSpell(sf::RenderWindow& _window);
	void DisplayIceSpell(sf::RenderTexture& _textureRed, sf::RenderTexture& _textureBlue);
	void Input(Audio& _audio, float _timer);


	void UpdatePlayerTimers(GameData& _gameData);
	void UpdatePlayerInfos(void);
	void UpdatePlayerMapColliding(GameData& _gameData);
	void UpdateCinematic(GameData& _gameData);
	void LoadPlayerAtBegening(void);
	void UpdatePlayerSound(GameData& _gameData);

	void ChangePosYPlayerCinematic(GameData& _gameData, float _moy1, float _moy2, float _moy3, float _moy4);

	void InitNewAnimStatus(void);
	bool GetInAir(void);
	PlayerStates GetState(void);
	void SetInAir(bool _inAir);
	int GetID();
	void SetID(int _id);
	int GetLevel();
	void SetLevel(int _manaNbr);
	void LevelUp(Audio& _audio);
	void SetPlayerState(PlayerStates _state);
	sf::RectangleShape* GetLightAttack();
	sf::RectangleShape* GetHeavyAttack();
	bool GetLightAttackActived();
	bool GetHeavyAttackActived();
	bool GetLightAttackButton();
	bool GetHeavyAttackButton();
	bool GetMagicAttackButton();
	bool GetParryState();
	bool GetCanBeRevive();
	bool GetHasFinishPray();
	bool GetTeam();
	sf::Sprite GetSpriteAOE();
	sf::Sprite& GetSprite();
	PlayerStates GetLastState();
	int GetLastDir();
	sf::Vector2f GetPos();
	sf::Vector2f GetSpeed();
	sf::Vector2f GetPosInAir();
	sf::Vector2f GetViperSpellInfo();
	void SetPosInAir(sf::Vector2f _posInAir);
	int GetLife();
	int GetMaxLife();
	int GetDamage();
	int GetXp();
	int GetMaxXp();
	int GetMana();
	int GetMaxMana();
	int GetPlayerSelected();
	int GetComboNumber();
	float GetParryTimer();
	void SetLightAttackButton(bool _bool);
	void SetHeavyAttackButton(bool _bool);
	void SetMagicAttackButton(bool _bool);
	void SetCanBeRevive(bool _bool);
	void SetHasFinishPray(bool _bool);
	void SetLastState(PlayerStates _state);
	void SetLastDir(int _dir);
	void SetParryTimer(float _timer);
	void SetPlayerSelected(int);
	void SetPlayerTeam(int);
	int GetPlayerTeam(void);
	void SetNbAttackInAir(int);
	void SetSpriteColor(sf::Color _color);
	bool GetHeavyAttackGoUp(void);
	void SetCanActiveHitbox(bool _bool);
	void SetPropultionKnockDown(bool _bool);
	void SetStatus(Status _status);
	void SetShieldIsHit(bool _bool);
	void SetHeavyAttackGoUp(bool _bool);
	bool GetPropulstionKnockDown(void);
	bool GetIsManaRecoveringFinished();
	bool GetCanKnockDownHit(void);
	bool GetPVPHeavyAttackDown(void);
	void SetPVPHeavyAttackDown(bool _var);
	bool GetIsSrinks(void);
	bool GetIsDrawnBehindTree(void);
	bool GetFreefallEndCinematicBird(void);
	bool GetFreefallStartCinematicBoss(void);
	bool GetAsDoAFreeFall(void);
	//Stats
	void AddStrength();
	void AddWisdom();
	void AddAgility();
	void AddVigor();
	void AddSkillPoints();
	void RemoveStrength();
	void RemoveWisdom();
	void RemoveAgility();
	void RemoveVigor();
	int GetStrength();
	int GetStrengthDamage();
	int GetWisdom();
	int GetAgility();
	int GetVigor();
	void SetStrength(int _strength);
	void SetWisdom(int _wisdom);
	void SetAgility(int _agility);
	void SetVigor(int _vigor);
	int* GetStats();
	int GetSkillPoints();
	void SetSkillPoints(int _skillPoints);
	void SetStatsValue(void);
	int GetRealSpeed(void);
	float GetTimerInRed(void);
	bool GetFreeFall(void);
	void SetFreeFall(bool _bool);

	//Inventory
	bool GetHasPotion();
	bool GetHasSpeedBuff();
	bool GetHasStrengthBuff();
	bool GetHasMagicBuff();
	void SetHasPotion(bool _bool);
	void SetHasSpeedBuff(bool _bool);
	void SetHasStrengthBuff(bool _bool);
	void SetHasMagicBuff(bool _bool);
	void UpdateInventory(float _dt, Audio& _audio);
	bool GetHasAPotion(void);
	void ResetPotionEffectBetweenLevel(void);

	//sf::Image tempImage1;
	std::string asHitSomething = "NOTHING";
	bool asPlayedSound = false;
	bool playerIsShrinks = false;
	std::string color[4] = { "RED", "GREEN", "BLUE", "YELLOW" };

	//dataPlayer network
	void SetComboNumber(int _combo);
	void SetParryState(bool _bool);
	Status GetStatus(void);
	AOEMagicAttack GetAOEMagic(void);
	void SetAOEMagic(AOEMagicAttack _aoeMagic);
	bool GetIceSpellHere(void);
	void DesactivateHitbox(void);
	void ResetShieldLife(void);
	bool GetAuraActivated();
	void SetAuraActivated(bool _isActivated);
	bool GetMagicAttackAOE();
	void SetMagicAttackAOE(bool _isAOE);
	bool GetPoisonSpellActive();
	void SetIceMagicAOE(bool _isIceActive);
	void SetPoisonMagicAOE(bool _isPoisonActive);
	bool GetShieldIsBreaking();
	void SetShieldIsBreaking(bool _isBreaking);
	int GetShieldLife();
	unsigned char GetCurrentFrame();
	void SetShieldLife(int _life);
	bool GetIdleParry(void);
	bool GetRightParry(void);
	bool GetLeftParry(void);
	void SetIdleParry(bool);
	void SetRightParry(bool);
	void SetLeftParry(bool);
	bool GetMagicAttackDistance();
	void SetMagicAttackDistance(bool _isDistance);
protected:
	//variable
	sf::Vector2f realSpeed;
	PlayersSprites* allSprite;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Sprite auraSprite;
	sf::Sprite auraParticuleSprite;
	sf::Sprite spriteAOE;
	StatusSprite animeStatu;
	AOEMagicAttack hitBoxAOE;
	sf::Vector2f deaclageSprite;
	sf::Vector2f realDeaclageSprite;
	sf::Vector2f lastPos;
	bool inAir;
	bool inArena = false;
	bool lightAttackButtonRealeased = true;
	bool heavyAttackButtonRealeased = true;
	bool magicAttackButtonReleased = true;
	bool magicAttackButtonReleased2 = true;
	bool parryButtonReleased = true;
	bool useItemButtonReleased = true;
	bool JumpButtonReleased = true;
	bool auraIsActived = false;
	bool canBeRevive = false;
	bool canActiveHitBox = true;
	bool hasFinishPray = false;
	bool hasShootArrow = false;
	bool hasCastMagick = false;
	bool canDash = true;
	bool endKnockDown = false;
	bool shieldHisHit = false;
	bool shieldIsBreaking = false;
	bool propultionKnockDown = false;
	bool haspropulsed = false;
	bool isManaRecoveringFinished = false;
	bool pvpHeavyAttackDown = false;
	bool notMoveDeath = false;
	bool AOEMagicUnlock = false;
	bool freefallGoUp = false;
	bool freefallGoLeft = false;
	bool freefallStartCinematic = false;
	bool freefallEndCinematic = false;
	bool playerDontMove = false;
	bool asDoAFreeFall = false;
	float timerEasingfreefallCinematic = 0.f;
	sf::Vector2f startPosEasingfreefallCinematic;
	sf::Vector2f endPosEasingfreefallCinematic;
	sf::Vector2f deceleration = { 0.f,0.f };
	//variable for inventory
	bool hasAPotion = false;
	bool hasASpeedBuff = false;
	bool hasAStrengthBuff = false;
	bool hasAMagicBuff = false;
	int currentItem = 0;
	bool useItem = false;
	bool hasUseSpeedBuff = false;
	bool hasUseStrengthBuff = false;
	bool hasUseMagicBuff = false;
	float timerItemUse = 0.f;
	
	bool showPotionAnime = false;
	bool potionIsHeal = false;
	sf::Sprite potionEffect;
	Animation potionAnime;

	sf::Sprite shieldFX;
	Animation shieldFXAnime;
	bool showShieldFx = false;
	
	////////////////////////	
	int level;
	int shieldLife = MAXSHIELDLIFE;
	int playerSelected = 0;
	int playerTeam = 0;
	int comboNumber = 0;
	int compteurEasing;
	int compteurLongMagickManaConsume = 0;
	int dashNumber = 0;
	int knockDownBounce = 0;
	int nbAttackInAir = 2;
	float timerHitByEnemies = 0.f;
	float timerAddMana = TIMER_ADD_MANA;
	float timerCombo = TIMER_ADD_MANA;
	float timerStandUp = 0.f;
	float timerStatus = TIMERSTATUS;
	float timerDistanceMagic = 0.f;
	float timerShieldLife = TIMERSHIELD;
	float timerShieldInvicibility = 1.f;
	float timerBugResetState = 0.f;
	float timerWaitComboInAir = 0.f;
	float timerBetweenDash = TIMERDASH;
	float timerSin = 0.f;
	float timerInRed = 0.f;
	float timerTaunt = 0.f;

	PlayerStates lastState;
	int id;
	sf::RectangleShape lightAttackHitbox;
	sf::RectangleShape heavyAttackHitbox;
	sf::Sprite shieldSprite;
	bool lightAttackHitboxActived;
	bool heavyAttackHitboxActived;
	bool magicAttackHitboxActivated;
	bool isLoading = false;
	bool isParrying = false;
	bool endJump = false;
	bool maxUpDistanceHeavyAttack = false;
	bool magicAttackIsDistance = true;
	bool canKnockDownHit = false;
	int lastDir = RIGHT;
	float timerSound = 0;
	float parryTimerSound = 0;
	bool hasLanded = true;
	bool heavyAttackGoUp;
	bool freefallWave = false;
	bool freefallAnimeLeftToRight = true;
	bool collideWithYellow = true;
	sf::Vector2f startPosEasing;
	sf::Vector2f endPosEasing;
	float timerEasing = 0.f;
	float timerDamageEatByBoss = 0.f;
	float crossProductRatio = 1.f;
	int joystickIndex;
	//Player Stats
	int stats[STATS_NBR];
	int skillPoints;
	int compteurLineAOEMagic = 0;
	int compteurLineDeath = 0;
	int compteurLineRevive = 0;
	int compteurLineAura = 0;

	bool viperSpellIsActive = false;
	bool iceSpellIsActive = false;

	//cinematic
	Cinematic currentPhaseCinematic = Cinematic::BACK;
	sf::Vector2f startPosCinematic;
	sf::Vector2f endPosCinematic;
	float timerCinematic = 0.f;
	bool posAssigned = false;
	int currentEassing = 1;
	float posYMoy1;
	float posYMoy2;
	float posYMoy3;
	float posYMoy4;
	float posYCinematic;

	//rezo
	bool parryIdle = false;
	bool parryRightFrontRun = false;
	bool parryLeftFrontRun = false;

	Status status = Status::None;
	Status lastStatus = Status::None;
	Animation currentAnime;
	Animation animeAOE;
	Animation auraAnime;
	Animation auraParticuleAnime;
	Animation shieldAnime;

	PlayerRunState* playerRunState;
	PlayerJumpState* playerJumpState;
	PlayerKnockDownState* playerKnockDownState;
	PlayerLightAttackState* playerLightAttackState;
	PlayerHeavyAttackState* playerHeavyAttackState;
	PlayerMagicAttackState* playerMagicAttackState;
	PlayerBowAttackState* playerBowAttackState;
	PlayerDeadState* playerDeadState;
	PlayerIdleState* playerIdleState;
	PlayerHitState* playerHitState;
	PlayerHitGroundState* playerHitGroundState;
	PlayerParryState* playerParryState;
	PlayerPrayState* playerPrayState;
	PlayerReviveState* playerReviveState;
	PlayerEatByBossState* playerEatByBossState;
	PlayerFreeFallState* playerFreeFallState;
	PlayerDashState* playerDashState;
	PlayerTauntState* playerTauntState;
private:
	void InitStatsPlayer();
};

#endif