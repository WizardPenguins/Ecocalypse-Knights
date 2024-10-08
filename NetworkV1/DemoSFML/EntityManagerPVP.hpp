#ifndef ENTITYMANAGERPVP_HPP
#define ENTITYMANAGERPVP_HPP

#include "stdafx.h"
#include "Player.hpp"
#include "Common.h"

struct HudDamage2
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

class EntityManagerPVP
{
public:
	EntityManagerPVP() = default;
	~EntityManagerPVP() = default;
	void Reload(void);

	 //Story
	 void UpdatePVP(std::vector<Player*>& _player, std::vector<Projectile>& _projectile, std::vector<Sort*>& _sort, std::vector<Building*>& _build);

	 void ProjectileCollision(std::vector<Player*>& _player, std::vector<Projectile>& _projectile);
	 void AOEMAgicCollision(std::vector<Player*>& _player);
	 void AttackCollision(std::vector<Player*>& _player);
	 //Arena
	 void UpdatePVP(std::vector<Player*>& _player, std::vector<Projectile>& _projectile, std::vector<Sort*>& _sort, std::vector<Building*>& _build ,bool& _inTeamFight ,sf::Font& _font , sf::Sprite& _heartSprite, Audio& _audio, sf::View& _view, float _dt , bool& _friendShip);

	 void ProjectileCollision(std::vector<Player*>& _player, std::vector<Projectile>& _projectile, bool& _inTeamFight, bool& _friendShip);
	 void AOEMAgicCollision(std::vector<Player*>& _player, bool& _inTeamFight, bool& _friendShip);
	 void AttackCollision(std::vector<Player*>& _player, bool& _inTeamFight, bool& _friendShip);

	 void ErasedArrow(std::vector<Projectile>& _projectile, sf::View& _view);
	 void UpdateDamageVector(float _dt);
	 void DisplayDamageVector(sf::RenderWindow& _window);
	 void DisplayDamageVector(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	 void FillVectorDamage(std::vector<Player*>& _player, sf::Font& _font, sf::Sprite& _spriteHeart);
	 void PlayerDeath(std::vector<Player*>& _player);
	 void RevivePlayer(std::vector<Player*>& _player, Audio& _audio, bool& _inTeamFight);
	 void SortVector(std::vector<Sort*>& _sort,std::vector<Player*>& _player,std::vector<Building*>& _build, std::vector<Projectile>& _proj);

private:
	std::vector<HudDamage2*> damageHud;
};

#endif // !ENTITYMANAGERPVP_HPP