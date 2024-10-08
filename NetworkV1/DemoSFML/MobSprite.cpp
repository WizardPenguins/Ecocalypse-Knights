#include "stdafx.h"
#include "MobSprite.hpp"

void HazelSprite::InitAnimCorrupted(SpriteAnim& _sprite, std::string _state, std::string _race)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == _race + "_" + _state)
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/" + _race + "/" + _race + "_" + _state + "C.png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void HazelSprite::InitCorruptedMob()
{
	for (int i = 0; i < (int)(HazelState::NbState); i++)
	{
		InitAnimCorrupted(this->spriteAnim[i], this->enumMobState[i], "Noisette");
	}

	InitFxAnim();
}

void BoarSprite::InitAnimBoar()
{
	for (int i = 0; i < (int)(BoarState::NbState); i++)
	{
		InitAllAnimBoar(this->spriteAnim[i], this->enumMobState[i]);
	}

	InitSlash2(this->spriteSlash[0], "SlashH");
	InitSlash(this->spriteSlash[1], "SlashV");
}

void BoarSprite::InitAllAnimBoar(SpriteAnim& _sprite, std::string _state)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Boar_" + _state)
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Boar/Boar_" + _state + ".png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void BoarSprite::InitSlash(SpriteAnim& _sprite, std::string _state)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Boar_" + _state)
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Boar/Boar_" + _state + ".png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void BoarSprite::InitSlash2(SpriteAnim& _sprite, std::string _state)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Boar_" + _state)
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Boar/Boar_" + _state + ".png", TOP_LEFT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void StatusSprite::InitAnimStatus()
{
	for (int i = 0; i < NbStatus; i++)
	{
		InitStatus(this->spriteAnim[i],this->enumStatus[i]);
	}
}

void StatusSprite::InitStatus(SpriteAnim& _sprite, std::string _status)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Status_" + _status)
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Status/" + _status + ".png", CENTER, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void ScarecrowSprite::InitAnimScarecrow()
{
	for (int i = 0; i < 3; i++)
	{
		InitScarecrow(this->spriteAnim[i], this->enumState[i]);
	}
}

void ScarecrowSprite::InitScarecrow(SpriteAnim& _sprite, std::string _state)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Scarecrow_" + _state)
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Scarecrow/Scarecrow_" + _state + ".png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void WitchSprite::InitAnimWitch()
{
	//witch
	for (int i = 0; i < (int)(WitchState::NbState); i++)
	{
		InitAllAnimWitch(this->spriteAnim[i], this->enumMobState[i]);
	}

	//crystal
	for (int i = 0; i < (int)CrystalState::NbState; i++)
	{
		InitCrystalsInvoc(this->spriteCrystalInvoc[i], (i + 1));
	}

	InitCircleInvoc(this->spriteCircleInvoc);
	InitCircleFxInvoc(this->spriteFxCircleInvoc);
	InitBrambleInvoc(this->spriteBrambleInvoc);
	InitShield(this->spriteShield);
	InitHitCrystal(this->spriteCrystalHit);
	InitSpritePlante(this->spritePlante);
	InitSpritePlanteDeath(this->spritePlanteDeath);
	InitScream(this->spriteScream);

	this->hazelnutCorrupted = new HazelSprite;
	InitCorruptedHazelnut(*this->hazelnutCorrupted);
}

void WitchSprite::InitAllAnimWitch(SpriteAnim& _sprite, std::string _state)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Witch_" + _state)
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Witch/Witch_" + _state + ".png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void WitchSprite::InitCircleInvoc(SpriteAnim& _sprite)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "CircleInvoc")
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Witch/CircleInvoc.png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void WitchSprite::InitCircleFxInvoc(SpriteAnim& _sprite)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "InvocFx")
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Witch/InvocFx.png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void WitchSprite::InitBrambleInvoc(SpriteAnim& _sprite)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Bramble")
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Witch/Bramble.png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void WitchSprite::InitCrystalsInvoc(SpriteAnim& _sprite,int _i)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Crystal" + std::to_string(_i))
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Witch/Crystal" + std::to_string(_i) + ".png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void WitchSprite::InitShield(SpriteAnim& _sprite)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Shield")
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Witch/Shield.png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void WitchSprite::InitHitCrystal(SpriteAnim& _sprite)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Crystal4")
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Witch/Crystal4.png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void WitchSprite::InitSpritePlante(SpriteAnim& _sprite)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Plante")
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Witch/Plante.png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void WitchSprite::InitSpritePlanteDeath(SpriteAnim& _sprite)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "PlanteDeath")
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Witch/PlanteDeath.png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void WitchSprite::InitScream(SpriteAnim& _sprite)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Scream")
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Witch/Scream.png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void WitchSprite::InitCorruptedHazelnut(HazelSprite& _mobSprite)
{
	_mobSprite.InitCorruptedMob();
}

void HazelSprite::InitAnimHazel()
{
	for (int i = 0; i < (int)(HazelState::NbState); i++)
	{
		InitAllAnimHazel(this->spriteAnim[i], this->enumMobState[i]);
	}

	InitFxAnim();
}

void HazelSprite::InitAllAnimHazel(SpriteAnim& _sprite, std::string _state)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Noisette_" + _state)
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Noisette/Noisette_" + _state + ".png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void HazelSprite::InitFxAnim()
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Noisette_HitFx")
			{
				infile >> this->spriteFx.nbAnim.x >> this->spriteFx.nbAnim.y >> this->spriteFx.animDim.x >> this->spriteFx.animDim.y >> this->spriteFx.speed >> this->spriteFx.loop;

				this->spriteFx.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Noisette/Noisette_HitFx.png", CENTER_FOOT, (int)this->spriteFx.nbAnim.x, (int)this->spriteFx.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void MushroomSprite::InitAnimMushroom()
{
	for (int i = 0; i < (int)(HazelState::NbState); i++)
	{
		InitAllAnimMushroom(this->spriteAnim[i], this->enumMobState[i]);
	}

	InitFxAnim();
}

void MushroomSprite::InitAllAnimMushroom(SpriteAnim& _sprite, std::string _state)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Mushroom_" + _state)
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Mushroom/Mushroom_" + _state + ".png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void MushroomSprite::InitFxAnim()
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Mushroom_HitFx")
			{
				infile >> this->spriteFx.nbAnim.x >> this->spriteFx.nbAnim.y >> this->spriteFx.animDim.x >> this->spriteFx.animDim.y >> this->spriteFx.speed >> this->spriteFx.loop;

				this->spriteFx.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Mushroom/Mushroom_HitFx.png", CENTER_FOOT, (int)this->spriteFx.nbAnim.x, (int)this->spriteFx.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void BirdSprite::InitAnimBird()
{
	for (int i = 0; i < (int)(BirdState::NbState); i++)
	{
		InitAllAnimBird(this->spriteAnim[i], this->enumMobState[i]);
	}

	InitProjectile(this->spriteAnimprojectile);
	InitHitFx(this->spriteFx);
}

void BirdSprite::InitAllAnimBird(SpriteAnim& _sprite, std::string _state)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Bird_" + _state)
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Bird/Bird_" + _state + ".png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void BirdSprite::InitProjectile(SpriteAnim& _sprite)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Bird_Projectile")
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Bird/Bird_Projectile.png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void BirdSprite::InitHitFx(SpriteAnim& _sprite)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Bird_Fx")
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Bird/Bird_Fx.png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void PigSprite::InitAnimPig()
{
	for (int i = 0; i < (int)(PigState::NbState); i++)
	{
		InitAllAnimPig(this->spriteAnim[i], this->enumMobState[i]);
	}

	InitFxHitAnim(this->spriteFxHit);
	InitFxAnim();
}

void PigSprite::InitAllAnimPig(SpriteAnim& _sprite, std::string _state)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Pig_" + _state)
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Pig/Pig_" + _state + ".png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void PigSprite::InitFxAnim()
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Pig_Fx")
			{
				infile >> this->spriteFx.nbAnim.x >> this->spriteFx.nbAnim.y >> this->spriteFx.animDim.x >> this->spriteFx.animDim.y >> this->spriteFx.speed >> this->spriteFx.loop;

				this->spriteFx.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Pig/Pig_Fx.png", CENTER_FOOT, (int)this->spriteFx.nbAnim.x, (int)this->spriteFx.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}

void PigSprite::InitFxHitAnim(SpriteAnim& _sprite)
{
	std::fstream infile("Assets/Images/Entities/Mobs/param.txt", std::ios::in);

	if (infile.is_open())
	{
		while (!infile.eof())
		{
			std::string current = "";
			infile >> current;
			if (current == "Pig_HitFx")
			{
				infile >> _sprite.nbAnim.x >> _sprite.nbAnim.y >> _sprite.animDim.x >> _sprite.animDim.y >> _sprite.speed >> _sprite.loop;

				_sprite.sprite = Utils::LoadSprite("Assets/Images/Entities/Mobs/Pig/Pig_HitFx.png", CENTER_FOOT, (int)_sprite.nbAnim.x, (int)_sprite.nbAnim.y);
				infile.close();
				return;
			}
			else
			{
				float del;
				infile >> del >> del >> del >> del >> del >> del;
			}
		}
	}

	infile.close();
}