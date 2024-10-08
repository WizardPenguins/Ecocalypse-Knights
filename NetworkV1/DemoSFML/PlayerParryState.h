#ifndef PLAYER_PARRY_HPP
#define PLAYER_PARRY_HPP
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"
#include "Easings.h"
#include "Camera.h"

class PlayerParryState
{
public:

	//Constructor/Destructor
	PlayerParryState(Player* _player);
	~PlayerParryState() = default;

	//Function
	void Move(float _dt);
	void Init();
	void Update(float _dt, bool& _multiplayer, Network& _network);
	void PlayerParryStateRun(float _dt, bool& _multiplayer, Network& _network);
	void UpdateAnim(float _dt, int _lineAnime);

	Player* player;
private:

};
#endif