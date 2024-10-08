#ifndef PLAYER_LIGHTATTACK_STATE_HPP
#define PLAYER_LIGHTATTACK_STATE_HPP
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"
#include "Easings.h"


class PlayerLightAttackState
{
public:

	//Constructor/Destructor
	PlayerLightAttackState(Player* _player);
	~PlayerLightAttackState() = default;

	//Function
	void Move(float _dt);
	void Init();
	void Update(float _dt);
	void PlayerLightAttackStateRun(float _dt);
	void UpdateAnim(float _dt);
	void UpdateHitBox();

	Player* player;
private:

};
#endif