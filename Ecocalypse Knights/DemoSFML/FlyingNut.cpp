#include "stdafx.h"
#include "FlyingNut.h"
#include "Easings.h"
#include "Player.hpp"
#include "Audio.h"

#define DistAxeY 35.f

FlyingNut::FlyingNut()
{
	spriteNut = Utils::LoadSprite("Assets/Images/BackGround/Anim/LauchedNut.png",CENTER);
	spriteGrass = Utils::LoadSprite("Assets/Images/BackGround/Anim/Grass.png",TOP_LEFT,8,1);
	starSprite = Utils::LoadSprite("Assets/Images/BackGround/Anim/Star.png",CENTER);
	grassAnime.CreateLeftToRight(spriteGrass, 8, 1, spriteGrass.getTexture()->getSize());
	flyingNutPos = { 4821.f + (float)(spriteNut.getTexture()->getSize().x / 2) ,600.f + (float)(spriteNut.getTexture()->getSize().y / 2)};
	hitbox = new sf::RectangleShape();
	hitbox->setSize((sf::Vector2f)spriteNut.getTexture()->getSize());
	hitbox->setOrigin({ hitbox->getSize().x / 2, hitbox->getSize().y });
	hitbox->setPosition({ 4821.f + hitbox->getSize().x/2  ,610.f + hitbox->getSize().y });
	startPosEasing = { 0,flyingNutPos.y };
	startPosEasing2 = { flyingNutPos.x,0 };
	endPosEasing = { 0, flyingNutPos.y - 550 };
	endPosEasing2 = { flyingNutPos.x + 1100, 0 };
	timer = 0.f;
	timer2 = 0.f;
}

void FlyingNut::Reload(GameData& _gameData)
{
	hisLunched = false;
	showStar = false;
	angleNut = 0.f;
	flyingNutPos = { 4821.f + (float)(spriteNut.getTexture()->getSize().x / 2) ,600.f + (float)(spriteNut.getTexture()->getSize().y / 2) };
	hitbox->setPosition({ 4821.f + hitbox->getSize().x / 2  ,610.f + hitbox->getSize().y });
	startPosEasing = { 0,flyingNutPos.y };
	startPosEasing2 = { flyingNutPos.x,0 };
	endPosEasing = { 0, flyingNutPos.y - 550 };
	endPosEasing2 = { flyingNutPos.x + 1100, 0 };
	timer = 0.f;
	timer2 = 0.f;
	_gameData.reloadFlyingNut = false;
}

void FlyingNut::Update(GameData& _gameData)
{
	if (_gameData.reloadFlyingNut)
	{
		Reload(_gameData);
	}

	for (int i = 0; i < _gameData.tabPlayers.size(); i++)
	{
		if (_gameData.tabPlayers[i]->state == LIGHTATTACK)
		{
			if (_gameData.tabPlayers[i]->GetPos().y >= hitbox->getPosition().y - DistAxeY && _gameData.tabPlayers[i]->GetPos().y <= hitbox->getPosition().y + DistAxeY)
			{
				if (Utils::AABB_AABB_Collision(hitbox, _gameData.tabPlayers[i]->GetLightAttack()) && !hisLunched)
				{
					hisLunched = true;
					_gameData.audio->PlaySoundNotRepeated("NUT_WA");
				}
			}
		}
		else if (_gameData.tabPlayers[i]->state == HEAVYATTACK)
		{
			if (_gameData.tabPlayers[i]->GetPos().y >= hitbox->getPosition().y - DistAxeY && _gameData.tabPlayers[i]->GetPos().y <= hitbox->getPosition().y + DistAxeY)
			{
				if (Utils::AABB_AABB_Collision(hitbox, _gameData.tabPlayers[i]->GetHeavyAttack()) && !hisLunched)
				{
					hisLunched = true;
					_gameData.audio->PlaySoundNotRepeated("NUT_WA");
				}
			}
		}
	}
	
	if (hisLunched)
	{
		grassAnime.UpdateRightToLeft(spriteGrass, _gameData.dt, 6, 0, true, false);

		if (timer2 < 1)
		{
			sf::Vector2f temp;
			temp = Easings::Lerp(startPosEasing2, endPosEasing2, Easings::OutQuad(timer));
			flyingNutPos.x = temp.x;
			timer += _gameData.dt / 3.f;
		}

		if (timer < 1)
		{
			sf::Vector2f temp;
			temp = Easings::Lerp(startPosEasing, endPosEasing, Easings::OutCirc(timer));
			flyingNutPos.y = temp.y;
			timer += _gameData.dt / 3.f;
			angleNut++;
			angleNut++;
			angleNut++;
			angleNut++;
			if (timer >= 0.1f)
			{
				spriteNut.setScale((1.f - timer) / (1.f - 0.1f), (1.f - timer) / (1.f - 0.1f));
			}
		}
		else if (timer >= 1 && timer < 1.5f)
		{
			timer += _gameData.dt / 2.f;
			showStar = true;
			angleNut++;
			angleNut++;
			angleNut++;
			if (timer >= 1.1f)
			{
				starSprite.setScale((1.5f - timer) / (1.5f - 1.1f), (1.5f - timer) / (1.5f - 1.1f));
			}
		}
		else
		{
			showStar = false;
		}
	}
}

void FlyingNut::Display(GameData& _gameData)
{
	if (hisLunched)
	{
		for (int i = 0; i < _gameData.sortVector.size(); i++)
		{
			if (_gameData.sortVector[i]->sort == SortVector::SleepingNut)
			{
				_gameData.sortVector.erase(_gameData.sortVector.begin() + i);
				_gameData.sleepingNut.clear();
			}
		}

		Utils::BlitSprite(spriteNut, flyingNutPos, angleNut, _gameData.window);
		Utils::BlitSprite(spriteGrass, { 4675,575 }, 0, _gameData.window);
	}
	if (showStar)
	{
		Utils::BlitSprite(starSprite, { 4821.f + (float)(spriteNut.getTexture()->getSize().x / 2) + 1100 ,600.f + (float)(spriteNut.getTexture()->getSize().y / 2) -550 }, angleNut, _gameData.window);
	}
}

void FlyingNut::Display3D(GameData& _gameData)
{
	if (hisLunched)
	{
		for (int i = 0; i < _gameData.sortVector.size(); i++)
		{
			if (_gameData.sortVector[i]->sort == SortVector::SleepingNut)
			{
				_gameData.sortVector.erase(_gameData.sortVector.begin() + i);
				_gameData.sleepingNut.clear();
			}
		}

		Utils::BlitSprite(spriteNut, sf::Vector2f{ flyingNutPos.x - (TROAD_GAP * (-1 * flyingNutPos.y + WORKING_HEIGHT) / 100.f),flyingNutPos.y }, angleNut, _gameData.renderTextureRed);
		Utils::BlitSprite(spriteNut, sf::Vector2f{ flyingNutPos.x + (TROAD_GAP * (-1 * flyingNutPos.y - WORKING_HEIGHT) / 100.f),flyingNutPos.y }, angleNut, _gameData.renderTextureBlue);

		Utils::BlitSprite(spriteGrass, sf::Vector2f{ 4675 - (TROAD_GAP * (-1 * 575 + WORKING_HEIGHT) / 100.f),575 }, 0, _gameData.renderTextureRed);
		Utils::BlitSprite(spriteGrass, sf::Vector2f{ 4675 + (TROAD_GAP * (-1 * 575 + WORKING_HEIGHT) / 100.f),575 }, 0, _gameData.renderTextureBlue);
	}
	if (showStar)
	{
		Utils::BlitSprite(starSprite, sf::Vector2f{ (4821.f + (float)(spriteNut.getTexture()->getSize().x / 2) + 1100) + (TROAD_GAP * (-1 * (600.f + (float)(spriteNut.getTexture()->getSize().y / 2) - 550) + WORKING_HEIGHT) / 100.f),(600.f + (float)(spriteNut.getTexture()->getSize().y / 2) - 550) }, angleNut, _gameData.renderTextureRed);
		Utils::BlitSprite(starSprite, sf::Vector2f{ (4821.f + (float)(spriteNut.getTexture()->getSize().x / 2) + 1100) - (TROAD_GAP * (-1 * (600.f + (float)(spriteNut.getTexture()->getSize().y / 2) - 550) + WORKING_HEIGHT) / 100.f),(600.f + (float)(spriteNut.getTexture()->getSize().y / 2) - 550) }, angleNut, _gameData.renderTextureBlue);
	}
}