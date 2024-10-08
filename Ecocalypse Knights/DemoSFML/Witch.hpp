#ifndef WITCH_HPP
#define WITCH_HPP

#pragma region Parameters
#define NbCrystal 2
#define LifeCrystal 100
#define TimerScream 12.f
#define TimerPlante 7.f
#define TimerBramble 2.f
#define TimerInvoc 5.f
#define TimerBeforeAttack1 1.f
#pragma endregion

#pragma region Includes
#include "stdafx.h"
#include "Mob.hpp"
#include "Audio.h"
#include "Animation.h"
#pragma endregion

struct Bramble
{
	SpriteAnim sprite;
	Animation* anim;
	sf::Vector2f pos;
	bool drawable;
	bool dispawn;
};

struct Plante
{
	SpriteAnim sprite;
	SpriteAnim spriteDeath;
	Animation* anim;
	Animation* animDeath;
	sf::Vector2f pos;
	float timerOnGround;
	sf::RectangleShape* hitBox;
	int playerEat;
	int line;

	bool isReady; // wait anim to go out
	bool toLong; // if the plante is too long on ground
	bool eatSomeone; // if someone was eat
	bool goOut; //when someone was too close of him
	bool spitOut; // when the player is spit out

	bool canDelete;
};

struct Crystal
{
	SpriteAnim sprite;
	SpriteAnim spriteHit;
	Animation* anim;
	Animation* animHitCrystal;
	CrystalState state;
	sf::Vector2f pos;
	int life;
	float timerFloating;
	float floating = 0.1f;
	sf::RectangleShape* hitBox;
	sf::Vector2f posOnGround; //only for the shadow
	bool hit = false;
	bool initHit = false;
	bool soundDeath;
};

class Witch 
{
public:
	Witch(Mob* _mob, WitchSprite* _sprite, Wave _wave);
	~Witch() = default;

	void Display(sf::RenderWindow& _window);
	void Display(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DrawInvocation(sf::RenderWindow& _window);
	void DrawInvocation(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayCrystal(sf::RenderWindow& _window, Crystal* _crystal);
	void DisplayCrystal(sf::RenderTexture& _red, sf::RenderTexture& _blue, Crystal* _crystal);
	void DisplayPlante(sf::RenderWindow& _window, Plante* _plante);
	void DisplayPlante(sf::RenderTexture& _red, sf::RenderTexture& _blue, Plante* _plante);
	void DisplayBramble(sf::RenderWindow& _window);
	void DisplayBramble(sf::RenderTexture& _red, sf::RenderTexture& _blue);

	void Update(float _dt, std::vector<Mob*>& _mob, Wave _wave, Audio& _audio, std::vector<Player*>& _player);

	WitchState GetState();
	void SetState(WitchState _state);
	WitchState GetLastState();
	void SetLastState(WitchState _state);
	sf::Vector2f GetDimSprite();
	Crystal* GetCrystal();
	bool GetHasSpawnCrystal();
	bool GetHasBramble();
	bool GetHasScream();
	bool GetHasSpawnPlant();
	bool GetPlayerBecomeFuzzy();
	void SetPlayerBecomeFuzzy(bool _bool);
	sf::RectangleShape* GetHitBoxBramble();
	std::vector<Plante*>& GetVectorPlant();
	int GetTargetPlayer();
	void SetTargetPlayer(int _newTarget);

private:
	void UpdateChoosePlayer(int _nbPlayer, Player* _player);
	void UpdateTimerAttack(float _dt);
	void UpdateChangeState();
	void UpdateActivate(float _dt, Plante* _plante);
	void UpdatePlanteTooLong(float _dt,Plante* _plante);
	void UpdateSpawn(float _dt,Plante* _plante);
	void UpdateHaveSomeone(float _dt, Plante* _plante, Audio& _audio);
	void UpdateWaitPlante(float _dt, Plante* _plante);
	void CreateNewPlant(Wave _wave);
	void AddCrystal(Audio& _audio);
	void CrystalHit(float _dt);
	void InitScreamFx();
	void UpdateScream(float _dt, Audio& _audio);
	void UpdateAnimPlante(Plante* _plante, float _dt);
	void UpdatePlante(float _dt, Audio& _audio);
	void AddPlanteOnGround(Wave _wave, Audio& _audio);
	void UpdateCrystalHit(float _dt, Audio& _audio);
	void UpdateHitBoxBramble();
	void DrawShield(sf::RenderWindow& _window);
	void DrawShield(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void InitShield();
	void UpdateAnimShield(float _dt);
	void UpdateAnimCrystal(float _dt);
	void InitFxCircleInvoc();
	void ChangePhaseCrystal();
	void UpdatePosCrystal(float _dt);
	void UpdateHealthCrystal(Audio& _audio);
	void InitAnimCrystal(SpriteAnim& _sprite,Animation* _anim);
	void InitCystal();
	void UpdateInvicible(float _dt, Audio& _audio);
	void UpdateAnimBramble(float _dt);
	void DispawnBramble();
	void AddBramble(float _playerY);
	void UpdatePosBramble(Wave _wave, float _playerY);
	void UpdateBrambleSpawn(float _dt, Wave _wave, float _playerY, Audio& _audio);
	void SpawnHazelNut(std::vector<Mob*>& _mob, Wave _wave, Audio& _audio);
	void UpdateAnimFx(float _dt, SpriteAnim& _sprite, Animation* _anim, int _line);
	void InitAnimAttack(SpriteAnim& _sprite, sf::RectangleShape& _shape);
	void UpdateInvoc(float _dt, std::vector<Mob*>& _mob, Wave _wave, Audio& _audio);
	void UpdateDeath(Audio& _audio);
	void UpdateIdleMove(float _dt);

	void DisplayFx(sf::RenderWindow&);
	void DisplayEntity(sf::RenderWindow&);
	void DisplayStatus(sf::RenderWindow&);	

	void DisplayFx(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayEntity(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayStatus(sf::RenderTexture& _red, sf::RenderTexture& _blue);

	void ChangePhase();
	void DestroyHitBoxAttack();
	void UpdateAnim(float _dt);
	void InitNewAnim();
	void Death(Audio& _audio);

private:
	WitchState state;
	WitchState lastState;
	WitchSprite sprite;

	std::vector<Bramble*> vecBramble; //vector that stock bramble
	sf::RectangleShape* hitBoxBramble; // hitBox for all of the bramble

	std::vector<Plante*> vecPlante; //vector that stock plante on ground
	Crystal crystal[NbCrystal]; // spawn crystal for invicible phase

	float timerFloating;
	float floating; //when boss go up and down

	//timer Attack
	float timerScream;
	float timerPlante;
	float timerBramble;
	float timerInvoc;
	float timerSoundAmbianceCristal = 0.f;
	float timerBeforeAttack;

	int nbInvoc;
	int line;
	int playerChoose;

	bool secondPhase;
	bool thirdPhase;
	bool finishAnim;
	bool initFx;

	bool hasInvocCircle;
	bool hasBramble;
	bool hasSpawnCrystal;
	bool hasSpawnPlant;
	bool playerBecomeFuzzy;
	bool hasScream;

	Mob* mob;
};

#endif // !WITCH_HPP