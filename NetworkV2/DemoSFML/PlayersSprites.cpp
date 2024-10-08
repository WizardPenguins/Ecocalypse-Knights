#include "stdafx.h"
#include "PlayersSprites.h"
#include "Utils.h"

void PlayersSprites::LoadSprites(void)
{
	for (int i = 0; i < NB_PLAYERS; i++)
	{
		playerSprite[i][(int)AllSprite::IDLE] = Utils::LoadSprite("Assets/Images/Entities/Players/Idle/PlayerIdle"+ std::to_string(i) + ".png", CENTER_FOOT, 8, 1);
		playerSprite[i][(int)AllSprite::ATTACK1] = Utils::LoadSprite("Assets/Images/Entities/Players/Attack/1/PlayerAttack"+ std::to_string(i) + ".png", CENTER_FOOT, 5, 1);
		playerSprite[i][(int)AllSprite::ATTACK2] = Utils::LoadSprite("Assets/Images/Entities/Players/Attack/2/PlayerAttack"+ std::to_string(i) + ".png", CENTER_FOOT, 4, 1);
		playerSprite[i][(int)AllSprite::ATTACK3] = Utils::LoadSprite("Assets/Images/Entities/Players/Attack/3/PlayerAttack"+ std::to_string(i) + ".png", CENTER_FOOT, 8, 1);
		playerSprite[i][(int)AllSprite::DEATH] = Utils::LoadSprite("Assets/Images/Entities/Players/Death/PlayerDeath"+ std::to_string(i) + ".png", CENTER_FOOT, 9, 2);
		playerSprite[i][(int)AllSprite::HEAVY_ATTACK_UP] = Utils::LoadSprite("Assets/Images/Entities/Players/HeavyAttack/PlayerUpHeavyAttack" + std::to_string(i) + ".png", CENTER_FOOT, 19, 1);
		playerSprite[i][(int)AllSprite::HEAVY_ATTACK_DOWN] = Utils::LoadSprite("Assets/Images/Entities/Players/HeavyAttack/PlayerDownHeavyAttack" + std::to_string(i) +".png", CENTER_FOOT, 18, 1);
		playerSprite[i][(int)AllSprite::HIT] = Utils::LoadSprite("Assets/Images/Entities/Players/Hit/PlayerHit" + std::to_string(i) + ".png", CENTER_FOOT, 1, 1);
		playerSprite[i][(int)AllSprite::HIT_GROUND] = Utils::LoadSprite("Assets/Images/Entities/Players/HitGround/PlayerHitGround" + std::to_string(i) + ".png", CENTER_FOOT, 5, 1);
		playerSprite[i][(int)AllSprite::JUMP] = Utils::LoadSprite("Assets/Images/Entities/Players/Jump/PlayerJump" + std::to_string(i) + ".png", CENTER_FOOT, 17, 1);
		playerSprite[i][(int)AllSprite::MAGIC] = Utils::LoadSprite("Assets/Images/Entities/Players/Magic/MagicAttack" + std::to_string(i) + ".png", CENTER_FOOT, 13, 1);
		playerSprite[i][(int)AllSprite::MAGIC_DISTANCE] = Utils::LoadSprite("Assets/Images/Entities/Players/Magic/Player" + std::to_string(i) + "DistanceMagic.png", CENTER_FOOT, 46, 1);
		playerSprite[i][(int)AllSprite::MAGIC_AOE] = Utils::LoadSprite("Assets/Images/Entities/Players/Magic/Player" + std::to_string(i) + "AOEMagic.png", TOP_LEFT, 6, 8);
		playerSprite[i][(int)AllSprite::PARRY] = Utils::LoadSprite("Assets/Images/Entities/Players/Parry/PlayerParade" + std::to_string(i) + ".png", CENTER_FOOT, 16, 3);
		playerSprite[i][(int)AllSprite::REVIVE] = Utils::LoadSprite("Assets/Images/Entities/Players/Revive/PlayerRevive" + std::to_string(i) + ".png", CENTER_FOOT, 9, 2);
		playerSprite[i][(int)AllSprite::PRAY] = Utils::LoadSprite("Assets/Images/Entities/Players/Pray/PlayerPray" + std::to_string(i) + ".png", CENTER_FOOT, 19, 1);
		playerSprite[i][(int)AllSprite::RUN] = Utils::LoadSprite("Assets/Images/Entities/Players/Run/PlayerRun" + std::to_string(i) + ".png", CENTER_FOOT, 8, 1);
		playerSprite[i][(int)AllSprite::BOW] = Utils::LoadSprite("Assets/Images/Entities/Players/Bow/PlayerBowAttack" + std::to_string(i) + ".png", CENTER_FOOT, 11, 1);
		playerSprite[i][(int)AllSprite::ARROW] = Utils::LoadSprite("Assets/Images/Entities/Arrow.png", TOP_LEFT, 1, 1);
		playerSprite[i][(int)AllSprite::AURA] = Utils::LoadSprite("Assets/Images/Entities/Players/Aura/Aura.png", CENTER_FOOT, 20, 2);
		playerSprite[i][(int)AllSprite::AURA_PARTICLE] = Utils::LoadSprite("Assets/Images/Entities/Players/Aura/AuraParticule"+ std::to_string(i) + ".png", CENTER_FOOT, 20, 2);
		playerSprite[i][(int)AllSprite::KNOCKDOWN] = Utils::LoadSprite("Assets/Images/Entities/Players/KnockDown/Player" + std::to_string(i) + "KnockDown.png", CENTER_FOOT, 15, 1);
		playerSprite[i][(int)AllSprite::SHIELDSPRITE] = Utils::LoadSprite("Assets/Images/HUD/ShieldGauge.png", CENTER, 10, 1);
		playerSprite[i][(int)AllSprite::DASH] = Utils::LoadSprite("Assets/Images/Entities/Players/Dash/PlayerDash" + std::to_string(i) + ".png", CENTER, 9, 1);
	}
}