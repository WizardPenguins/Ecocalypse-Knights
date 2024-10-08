#include "stdafx.h"
#include "PlayerBowAttackState.h"

void PlayerBowAttackState::Init()
{
	player->sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::BOW];
	player->currentAnime.CreateLeftToRight(player->sprite, 11, 1, player->sprite.getTexture()->getSize());
	player->hasShootArrow = false;
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { -24 ,20 } : this->player->deaclageSprite = { 24 ,20 };
}

void PlayerBowAttackState::Update(float _dt, std::vector<Projectile>* _projectile)
{
	UpdateAnim(_dt);
	(bool)(player->lastDir == LEFT) ? this->player->deaclageSprite = { -24 ,20 } : this->player->deaclageSprite = { 24 ,20 };
	if (player->currentAnime.currentFrame == 8 && !player->hasShootArrow)
	{
		player->hasShootArrow = true;

		int dir;
		if (player->lastDir == RIGHT)
		{
			dir = -1;
		}
		else
		{
			dir = 1;
		}

		Projectile temp = { dir, {player->pos.x, player->pos.y - 80}, false};
		temp.sprite = player->allSprite->playerSprite[player->playerSelected][(int)AllSprite::ARROW];
		temp.playerId = player->id;
		temp.autor = AutorProjectile::PLAYER;
		temp.projetileType = TypeProjectile::ARROW;
		temp.posAutor = { player->pos.x,player->pos.y };
		temp.hitBox = new sf::RectangleShape(sf::Vector2f{ (float)(temp.sprite.getTexture()->getSize().x),(float)(temp.sprite.getTexture()->getSize().y) });
		if (dir == 1)
		{
			temp.sprite.setScale({ -1,1 });
			temp.hitBox->setScale({ -1, 1 });
		}
		temp.damage = 5 + (player->stats[(int)Stats::AGILITY] * 5 ); 
		_projectile->push_back(temp);
	}
	else if (player->currentAnime.currentFrame == player->currentAnime.maxFrame)
	{
		player->SetPlayerState(IDLE);
	}
}

void PlayerBowAttackState::UpdateAnim(float _dt)
{
	bool temp = true;
	player->currentAnime.UpdateLeftToRight(player->sprite, _dt, 14 + player->stats[(int)Stats::AGILITY], 0, temp, false);
}
	
void PlayerBowAttackState::PlayerBowAttackStateRun()
{

}

PlayerBowAttackState::PlayerBowAttackState(Player* _player)
{
	this->player = _player;
}