#ifndef PLAYER_REVIVE_STATE_H
#define PLAYER_REVIVE_STATE_H

#include "Common.h"
#include "Player.hpp"
#include "Easings.h"

class PlayerReviveState
{
public:

	//Constructor/Destructor
	PlayerReviveState(Player* _player);
	~PlayerReviveState() = default;

	//Function
	void Init();
	void Update(float _dt);
	void PlayerReviveStateRun(float _dt);
	void UpdateAnim(float _dt);

	Player* player;
private:

};
#endif