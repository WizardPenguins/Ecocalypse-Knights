#ifndef PLAYER_RUN_STATE_HPP
#define PLAYER_RUN_STATE_HPP
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"

class PlayerRunState
{
public:

	//Constructor/Destructor
	PlayerRunState(Player* _player);
	~PlayerRunState() = default;

	//Function
	void Move(float _dt);
	void Init();
	void Update(float _dt, Audio& _audio, bool& _inCinematic);
	void PlayerRunStateRun(float _dt, Audio& _audio, bool& _inCinematic);
	void UpdateAnim(float _dt);

	void Input(Audio& _audio);
	Player* player;
private:

};
#endif