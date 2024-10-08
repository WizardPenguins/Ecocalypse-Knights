#include "stdafx.h"
#include "PlayerHitState.h"

void PlayerHitState::Move()
{

}

void PlayerHitState::Init()
{
	this->player->deaclageSprite = { 0,0 };
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::HIT];
	player->lightAttackHitboxActived = false;
	player->heavyAttackHitboxActived = false;
	if (player->playerSelected == 0 || player->playerSelected == 3)
	{
		player->hitBoxAOE.isActive = false;
	}
}

void PlayerHitState::Update(float _dt, Audio& _audio)
{
	int tempRandGetHit = rand() % 3;
	player->timerHitByEnemies += _dt;
	if (player->timerHitByEnemies >= 0.3f)
	{
		_audio.PlaySoundNotRepeated("PLAYER_" + player->color[player->GetPlayerSelected()] + "_GET_HIT_" + std::to_string(rand() % 3 + 1), 100.f);

		player->timerHitByEnemies = 0;
		if (player->freefallWave)
		{
			player->SetPlayerState(FREEFALL);
		}
		else
		{
			player->SetPlayerState(IDLE);
		}
	}
}

void PlayerHitState::UpdateAnim()
{

}

void PlayerHitState::Input()
{

}

void PlayerHitState::PlayerHitStateRun(float _dt, Audio& _audio)
{
	Update(_dt, _audio);
}

PlayerHitState::PlayerHitState(Player* _player)
{
	this->player = _player;
}