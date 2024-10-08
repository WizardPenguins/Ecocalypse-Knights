#ifndef PLAYERTAUNT_H
#define PLAYERTAUNT_H
#include "stdafx.h"

#include "Common.h"
#include "Player.hpp"

class PlayerTauntState
{
public:

	//Constructor/Destructor
	PlayerTauntState(Player* _player);
	~PlayerTauntState() = default;

	//Function
	void Init();
	void Update(float _dt);
	void PlayerTauntStateRun(float _dt);
	void UpdateAnim(float _dt);

	Player* player;
};

#endif // !PLAYERTAUNT_H

