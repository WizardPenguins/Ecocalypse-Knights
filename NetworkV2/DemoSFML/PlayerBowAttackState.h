#ifndef PLAYERBOWATTACKSTATE_H
#define PLAYERBOWATTACKSTATE_H
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"

class PlayerBowAttackState
{
public:

	//Constructor/Destructor
	PlayerBowAttackState(Player* _player);
	~PlayerBowAttackState() = default;

	//Function
	void Init();
	void Update(float _dt, std::vector<Projectile>* _arrow);
	void PlayerBowAttackStateRun();
	void UpdateAnim(float _dt);

	Player* player;
};

#endif // !PLAYERBOWATTACKSTATE_H