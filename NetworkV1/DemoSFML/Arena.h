#ifndef ARENA_H

#define ARENA_H

#include"States.h"
#include "Audio.h"
#include "Camera.h"
#include "EntityManager.h"
#include "Player.hpp"
#include "HUD.h"
#include "Easings.h"

constexpr float TIMER_WIN = 4.f;

class Arena : public States
{
public:
	Arena(GameData& _gameData);

	void Reload(GameData& _gameData);
	void Update(GameData& _gameData) override;
	void Display(GameData& _gameData) override;
	void Display3D(GameData& _gameData) override ;
	void JoystickPressed(GameData& _gameData) override;
	const bool& GetFadeOut(void) const;

	void SetAlpha(const float _alpha);
	void SetFadeOut(const bool _fadeOut);

private:
	void InitComponents(GameData& _gameData);
	void InitClasses(GameData& _gameData);
	void FadeManagement(GameData& _gameData);
	void UpdatePlayers(GameData& _gameData);
	void DisplayPlayers(GameData& _gameData);

	void ArenaHandler(GameData& _gameData);
	void UpdateWinCondition(GameData& _gameData);
	void UpdateWinFade(GameData& _gameData);
	void UpdateFloatingHUD(GameData& _gameData);
	void ArenaDisplay(GameData& _gameData);

	void SkipTuto(GameData& _gameData);

	void Music(GameData& _gameData);
	bool ArrowPointCollision(sf::Image& _image, std::vector<Projectile>& _tabProjectile, int _id);
	void ShadowDisplay(GameData& _gameData);
	void ProjectileShadowDisplay(GameData& _gameData);
	void EntitiesDisplay(GameData& _gameData);

	void ShadowDisplay3D(GameData& _gameData);

	void ProjectileShadowDisplay3D(GameData& _gameData);

	void DisplayPoisonSpell(std::vector<Player*>& _player, sf::RenderTexture& _red, sf::RenderTexture& _blue);

	void EntitiesDisplay3D(GameData& _gameData);

	void ArenaDisplay3D(GameData& _gameData);

	void ReloadInterFight(GameData& _gameData);

	bool showTuto = true;
	sf::Sprite winSprite;
	bool uptadeWinCondition = true;
	float timerWin = TIMER_WIN;
	float timerEasing = 0.f;
	sf::Uint8 alpha = 0;
	bool diplayWinSprite = false;
	std::vector<sf::Text*> textWinCompteur;
	sf::Sprite crownHud;
	float timerSin = 0.f;
	sf::Sprite conffeti;
	Animation animeConffeti;
	int lineAnime = 0;
	sf::Sprite fightStart;
	Animation animeFightStart;
	int lineAnimeFight = 0;
	bool displayFight = true;
	bool leftArena = false;
	Fade* fade = nullptr;
	int D3CompteurFps = 0;
	sf::Sprite tutoSprite;

	// in free for all 
	std::vector<int>compteurWinPlayer;
	std::vector<sf::Sprite> CrownOnPlayer;
	std::vector<bool> displayBestPlayer;

	// in team fight
	std::vector<int>compteurWinTeam;
	std::vector<sf::Sprite> teamFlag;
	sf::Sprite winSpriteLeft;
	sf::Sprite winSpriteRight;
	sf::Sprite winSpriteMiddle;
	Animation animeLeft;
	Animation animeRight;
	Animation animeMidle;
	sf::Vector2f posWinCompteur;
	bool displayTeamScore = true;

	
};
#endif