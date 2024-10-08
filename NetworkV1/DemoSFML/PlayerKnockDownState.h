#ifndef PLAYER_KNOCKDOWN_STATE_HPP
#define PLAYER_KNOCKDOWN_STATE_HPP

#include "Common.h"
#include "Player.hpp"
#include "Easings.h"

class PlayerKnockDownState
{
public:

	//Constructor/Destructor
	PlayerKnockDownState(Player* _player);
	~PlayerKnockDownState() = default;

	//Function
	void InitGroundEasingPos(void);
	void Init(void);
	void InAirKnock(float _dt);
	void GroundKnock(float _dt);
	void Update(float _dt);
	void UpdateAnim(float _dt);

	Player* player;
private:

};
#endif