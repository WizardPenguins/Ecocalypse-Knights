#ifndef  GAME_H

#define GAME_H

#include "stdafx.h"

#include"States.h"
#include "Audio.h"
#include "Camera.h"
#include "MapManager.h"
#include "EntityManager.h"
#include "Mob.hpp"
#include "Player.hpp"
#include "HUD.h"
#include "LevelManager.h"
#include "Shop.h"
#include "Easings.h"
#include "Witch.hpp"

class Game : public States
{
public:
	Game(GameData& _gameData);
	~Game();

	void Reload(GameData& _gameData);
	void Update(GameData& _gameData) override;
	void Display(GameData& _gameData) override;
	void Display3D(GameData& _gameData) override ;
	void JoystickPressed(GameData& _gameData) override;
	void KeyPressed(GameData& _gameData) override;
	void LostFocus(GameData& _gameData) override;
	const bool& GetFadeOut(void) const;

	void SetAlpha(const float _alpha);
	void SetFadeOut(const bool _fadeOut);

private:
	void ShadowDisplay(GameData& _gameData);
	void ShadowDisplay3D(GameData& _gameData);
	void DisplayAllShadowOfWitch(Mob* _witch, sf::Sprite _shadow, sf::RenderTexture* _rt);
	void DisplayAllShadowOfWitch3D(Mob* _witch, sf::Sprite _shadow, sf::RenderTexture* _red, sf::RenderTexture* _blue);
	void DisplayPoisonSpell(std::vector<Player*>& _player,sf::RenderWindow& _window);
	void DisplayPoisonSpell(std::vector<Player*>& _player, sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayInvocCircle(std::vector<Mob*>& _mob,sf::RenderWindow& _window);
	void DisplayInvocCircle(std::vector<Mob*>& _mob, sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayBossBar(std::vector<Mob*>& _mob, EntityManager* _entiy, HudBarBoss& _spriteBoss, Wave& _wave, sf::RenderWindow& _window, sf::Font _font);
	void DisplayBossBar(std::vector<Mob*>& _mob, EntityManager* _entiy, HudBarBoss& _spriteBoss, Wave& _wave, sf::RenderTexture& _red, sf::RenderTexture& _blue, sf::Font _font);
	void DisplayPopUp(int _currentPopUp, std::vector<PopUp*>& _popUp, bool _active, sf::RenderWindow& _window);
	void DisplayPopUp3D(int _currentPopUp, std::vector<PopUp*>& _popUp, bool _active, sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayHand(std::vector<Mob*>& _mob, sf::RenderWindow& _window);
	void DisplayHand(std::vector<Mob*>& _mob, sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void EntitiesDisplay(GameData& _gameData);
	void EntitiesDisplay3D(GameData& _gameData);
	void ProjectileShadowDisplay(GameData& _gameData);
	void ProjectileShadowDisplay3D(GameData& _gameData);

	void UpdateSelectedPlayer(std::vector<Player*>& _player, std::vector<int> _playerSelected);
	void UpdateProjectile(std::vector<Projectile>& _proj, sf::Image& _image, float _dt, LevelState& _level);
	void UpdateFriendShip(EntityManagerPVP* _entity, std::vector<Player*>& _player, std::vector<Projectile>& _projectile, std::vector<Sort*>& _sort, std::vector<Building*>& _build,bool _activate);
	void UpdateMultiplayer(GameData& _gameData);
	void UpdateMob(std::vector<Mob*>& _mob,GameData& _gameData);
	void UpdatePopUp(int& _currentPopUp, std::vector<PopUp*>& _popUp,bool& _active,float& _dt);
	void UpdateDeath(GameData& _gameData);
	bool ArrowPointCollision(sf::Image& _image, std::vector<Projectile>& _tabProjectile, int _id, LevelState& _level);

	void Music(GameData& _gameData);

	void InitComponents(GameData& _gameData);
	void InitClasses(GameData& _gameData);
	void ReloadBetweenLevel(GameData& _gameData);
	void FadeManagement(GameData& _gameData);
	void UpdatePlayers(GameData& _gameData);
	void UpdateHandFade(float _dt);

	void Level1Handler(GameData& _gameData);
	void Level2Handler(GameData& _gameData);
	void Level3Handler(GameData& _gameData);
	void MapHandler(GameData& _gameData);

	void Level1Display(GameData& _gameData);
	void LevelDisplay3D(GameData& _gameData);

	void Level2Display(GameData& _gameData);

	void Level3Display(GameData& _gameData);

	sf::Vector2f startPosEasing;
	sf::Vector2f endPosEasing;
	sf::Vector2f currentPos;
	float timerEasing = 0.f;
	bool movingFront;

	MapManager mapManager;
	sf::Sprite nextLevel;
	
	Fade* fade = nullptr;
	int D3CompteurFps = 0;

	float timerDeath = 0.f;
	bool level1IsDone = false;
	bool level2IsDone = false;

	float timerSpawnLeaf = 0.f;
};

#endif 