#ifndef PLAYER_PRAY_STATE_H
#define PLAYER_PRAY_STATE_H

#include "Common.h"
#include "Player.hpp"


class PlayerPrayState
{
public:

	//Constructor/Destructor
	PlayerPrayState(Player* _player);
	~PlayerPrayState() = default;

	//Function
	void Init();
	void Update(float _dt);
	void PlayerPrayStateRun(float _dt);
	void UpdateAnim(float _dt);

	Player* player;
private:

};
#endif
