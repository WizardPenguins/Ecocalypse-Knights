#ifndef PLAYERHEAVYATTACKSTATE_H
#define PLAYERHEAVYATTACKSTATE_H

constexpr float TIMERHEAVYATTACK  = 0.5f;
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"
#include "Easings.h"

class PlayerHeavyAttackState
{
public:

	//Constructor/Destructor
	PlayerHeavyAttackState(Player* _player);
	~PlayerHeavyAttackState() = default;

	//Function
	void Input(Audio& _audio);
	void Move(float _dt);
	void Init();
	void Update(float _dt, Audio& _audio);
	void UpdateHitbox();
	void PlayerHeavyAttackStateRun(float _dt, Audio& _audio);
	void UpdateAnim(float _dt);

	Player* player;
};

#endif // !PLAYERHEAVYATTACKSTATE_H