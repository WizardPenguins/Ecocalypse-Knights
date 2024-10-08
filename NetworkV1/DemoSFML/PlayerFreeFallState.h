#ifndef PLAYERFREEFALL_H
#define PLAYERFREEFALL_H
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"

class PlayerFreeFallState
{
public:

	//Constructor/Destructor
	PlayerFreeFallState(Player* _player);
	~PlayerFreeFallState() = default;

	//Function
	void Init();
	void Input(Audio& _audio);
	void Move(float _dt);
	void Update(float _dt, Audio& _audio);
	void PlayerFreeFallStateRun(float _dt, Audio& _audio);
	void UpdateAnim(float _dt);

	Player* player;
};

#endif // !PLAYERFREEFALL_H