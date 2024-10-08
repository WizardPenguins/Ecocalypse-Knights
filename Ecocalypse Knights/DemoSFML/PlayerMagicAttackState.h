#ifndef PLAYER_MAGICATTACK_STATE_HPP
#define PLAYER_MAGICATTACK_STATE_HPP
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"
#include "Easings.h"

constexpr float TIMERMAGICATTACK = 1.0f;
constexpr float SPEED_DISTANCE_MAGICATTACK = 200.f;

class PlayerMagicAttackState
{
public:

	//Constructor/Destructor
	PlayerMagicAttackState(Player* _player);
	~PlayerMagicAttackState() = default;

	//Function
	void Init();
	void Update(float _dt, std::vector<Projectile>* _projectile, Audio& _audio);
	void Display(sf::RenderWindow& _window);
	void Display(sf::RenderTexture& _textureRed, sf::RenderTexture& _textureBlue);
	void UpdateAnim(float _dt, Audio& _audio);

	Player* player;
private:
	void AddDistanceMagic(std::vector<Projectile>* _projectile, Audio& _audio);
	void UseAOEMagic(Audio& _audio);
	void UseAOEMagicFire(void);
	void UseAOEMagicElectricity(void);
	void UpdateAnimeFire(float _dt);
	void UpdateAnimePoison(float _dt);
	void UpdateAnimeIce(float _dt, Audio& _audio);
	void UpdateAnimeElectricity(float _dt);
	void DisplayFireAOE(sf::RenderWindow& _window);
	void DisplayFireAOE(sf::RenderTexture& _textureRed, sf::RenderTexture& _textureBlue);
	void DisplayElectricityAOE(sf::RenderWindow& _window);
	void DisplayElectricityAOE(sf::RenderTexture& _textureRed, sf::RenderTexture& _textureBlue);
};

#endif