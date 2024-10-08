#ifndef PLAYERIDLESTATE_H
#define PLAYERIDLESTATE_H
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"

class PlayerIdleState
{
public:

	//Constructor/Destructor
	PlayerIdleState(Player* _player);
	~PlayerIdleState() = default;

	//Function
	void Init();
	void Update(float _dt, Audio& _audio);
	void PlayerIdleStateRun(float _dt, Audio& _audio);
	void UpdateAnim(float _dt);

	void Input(Audio& _audio);
	Player* player;
};

#endif // !PLAYERIDLESTATE_H