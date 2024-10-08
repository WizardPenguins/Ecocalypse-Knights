#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Mob.hpp"
#include "Player.hpp"
#include "Audio.h"
#include "Boar.hpp"
#include "Easings.h"
#include "stdafx.h"
#include "Witch.hpp"

struct HudDamage
{
	sf::Text damage;
	sf::Text damageBack;
	sf::Sprite heartSprite;
	sf::Vector2f pos;
	sf::Vector2f startPosEasing;
	sf::Vector2f endPosEasing;
	float timerEasing = 0.f;
	bool displayHeart = false;
};

struct HudGold
{
	sf::Text gold;
	sf::Text goldBack;
	sf::Sprite goldSprite;
	sf::Vector2f pos;
	sf::Vector2f startPosEasing;
	sf::Vector2f endPosEasing;
	float timerEasing = 0.f;
};

class EntityManager
{
public:

	EntityManager(sf::Sprite _poison, sf::Sprite _slash);
	~EntityManager() = default;

	void EntityColliding(std::vector<Mob*>& _mob, std::vector<Player*>& _player, std::vector<Projectile>& _projectile, std::vector<Drop*>& _drop, std::vector<Sort*>& _sort, float _dt, sf::View& _view, Audio& _audio, std::vector<Building*>& _building, sf::Font& _font, Wave& _wave, std::vector<Scarecrow*>& _scarecrow, sf::Sprite& _spriteHeart);
	void UpdateVector(float _dt, std::vector<Player*>& _player, std::vector<Mob*>& _mob, std::vector<Scarecrow*>& _scarecrow, std::vector<Projectile>& _projectile, std::vector<Building*>& _building, std::vector<Drop*>& _food, std::vector<Sort*>& _sort, sf::Font& _font, sf::Sprite& _spriteHeart, std::vector<Building*>& _sleepingNut);
	void Reload(void);

	void UpdateWitchHitBox(Witch* _witch, std::vector<Player*>& _player, Audio& _audio);
	void PlayerArrowCollision(std::vector<Projectile>& _projectile, std::vector<Player*>& _player, Audio& _audio);
	void MobArrowCollision(std::vector<Projectile>& _projectile, std::vector<Mob*>& _mob, std::vector<Player*>& _player, Audio& _audio);
	void AttackOnMobCollision(std::vector<Mob*>& _mob, std::vector<Player*>& _player, Audio& _audio);
	void AttackOnPlayerCollision(std::vector<Mob*>& _mob, std::vector<Player*>& _player, float _dt, Audio& _audio);
	void PlayerLifeDropCollision(std::vector<Player*>& _player, std::vector<Drop*>& _drop, Audio& _audio);
	void DeleteMob(std::vector<Mob*>& _mob, std::vector<Scarecrow*>& _scarecrow);
	void PlayerDead(std::vector<Player*>& _player, Audio& _audio);
	void RevivePlayer(std::vector<Player*>& _player, Audio& _audio);
	void ErasedArrow(std::vector<Projectile>& _projectile,sf::View& _view);
	void DisplayDamageVector(sf::RenderWindow& _window);
	void DisplayGoldVector(sf::RenderWindow& _window);
	void DisplayDamageVector(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DisplayGoldVector(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void LaunchAIMob(std::vector<Mob*>& _mob, std::vector<Player*>& _player);
	void BoarSlash(Mob& _mob,std::vector<Player*>& _tabPlayer);
	void CollideBetweenMob(std::vector<Mob*>& _mob, int _wave);
	void DrawLifeBar(Mob* _mob, sf::Sprite& _lifeBar, sf::Sprite& _pixelBar, bool _draw, sf::RenderWindow& _window);
	void DrawLifeBar(Mob* _mob, sf::Sprite& _lifeBar, sf::Sprite& _pixelBar, bool _draw, sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DrawBossBar(std::string _race, HudBarBoss& _spriteBoss, Mob& _mob, Wave& _wave, sf::RenderWindow& _window, float _decalage, sf::Font _font, std::string _string);
	void DrawBossBar(std::string _race, HudBarBoss& _spriteBoss, Mob& _mob, Wave& _wave, sf::RenderTexture& _red, sf::RenderTexture& _blue, float _decalage, sf::Font _font, std::string _string);
	void PlayerLongMagicAttack(std::vector<Mob*>& _mob, std::vector<Player*>& _player);
	void TpPlayerBlockedInRed(std::vector<Player*>& _player);
	bool waveLaunch = false;

	//surdef
	void MobArrowCollision(std::vector<Projectile>& _projectile, std::vector<Scarecrow*>& _scarecrow, Audio& _audio);
	void AttackOnMobCollision(std::vector<Scarecrow*>& _scarecrow, std::vector<Player*>& _player, Audio& _audio);
	void PlayerLongMagicAttack(std::vector<Scarecrow*>& _scarecrow, std::vector<Player*>& _player, Audio& _audio);

private :
	std::vector<HudDamage*> damageHud;
	std::vector<HudGold*> goldHud;
	bool bossBarInit = false;
	int waveBossBar;
	sf::Text textTaMere;
	sf::Vector2u size;
	sf::Image tempImage;
	sf::Vector2f oneFrameSize;

	sf::Vector2f dimSlash;
	sf::Vector2f dimSlashOneFrame;
	sf::Image imageSlash;

	bool TestWitch(Mob* _mob);
	bool TestBoar(Mob* _mob, Player* _player);
	bool TestHazel(Mob* _mob);
	bool PlayerIsReady(Player* _player);
	void FillVectorGold(std::vector<Player*>& _player, sf::Font& _font);
	void FillVectorDamage(std::vector<Player*>& _player, std::vector<Mob*>& _mob, std::vector<Scarecrow*>& _scarecrow, sf::Font& _font, sf::Sprite& _spriteHeart);
	void FillVectorSort(std::vector<Projectile>& _projectile, std::vector<Player*>& _player, std::vector<Mob*>& _mob, std::vector<Building*>& _building, std::vector<Drop*>& _food, std::vector<Sort*>& _sort, std::vector<Scarecrow*>& _scarecrow, std::vector<Building*>& _sleepingNut);
	void UpdateDamageVector(float _dt);
	void UpdateGoldVector(float _dt);
};

#endif // !ENTITYMANAGER_H