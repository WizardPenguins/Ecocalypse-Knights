#include "stdafx.h"
#include "Particle.h"
#include "Camera.h"

void Particle::AddParticle(GameData& _gameData)
{
	Particles* tempParticle = new Particles;

	sf::Vector2f pos = _gameData.camera->GetView().getCenter();
	
	tempParticle->pos.x = (pos.x - WORKING_WIDTH/2) + rand() % (int)(WORKING_WIDTH *1.5);
	tempParticle->pos.y = (pos.y - WORKING_HEIGHT/2) - rand() % 50;

	tempParticle->rotation = 360 * rand() / (float)RAND_MAX;
	tempParticle->variationSpeed = { 500.f * rand() / (float)RAND_MAX+50.f,500.f * rand() / (float)RAND_MAX+50.f };

	tempParticle->scale = rand() / (float)RAND_MAX;
	tempParticle->scaleVariation = 1.f + rand() / (float)RAND_MAX;
	int random;
	if (pos.x < 5000)
	{
		random = 0;
	}
	else if (pos.x < 8000)
	{
		random = 50;
	}
	else
	{
		random = 100;
	}
	/* = (pos.x / 12810) * 100;*/
	if (rand() % 100 > random)
	{
		tempParticle->type = TypeParticle::LEAF;
	}
	else
	{
		tempParticle->type = TypeParticle::CORRUPT;
	}
	
	//Insertion dans le vector
	_gameData.tabParticle.push_back(tempParticle);
}

void Particle::Update(GameData& _gameData)
{
	for (int i = _gameData.tabParticle.size() - 1; i >= 0; i--)
	{
		//Déplacement
		_gameData.tabParticle[i]->pos.y += _gameData.tabParticle[i]->variationSpeed.y * _gameData.dt;
		_gameData.tabParticle[i]->pos.x -= _gameData.tabParticle[i]->variationSpeed.x * _gameData.dt;
		_gameData.tabParticle[i]->rotation += _gameData.dt * 10;

		if (_gameData.tabParticle[i]->isUp)
		{
			_gameData.tabParticle[i]->scale += _gameData.tabParticle[i]->scaleVariation * _gameData.dt;

			if (_gameData.tabParticle[i]->scale > 1.f)
			{
				_gameData.tabParticle[i]->scale = 1.f;
				_gameData.tabParticle[i]->isUp = false;
			}
		}
		else
		{
			_gameData.tabParticle[i]->scale -= _gameData.tabParticle[i]->scaleVariation * _gameData.dt;

			if (_gameData.tabParticle[i]->scale < 0.f)
			{
				_gameData.tabParticle[i]->scale = 0.f;
				_gameData.tabParticle[i]->isUp = true;
			}
		}

		if (_gameData.tabParticle[i]->pos.y > 5000)
		{
			//Suppression Swap'n'pop, on écrase l'élément à effacer avec le dernier élément et on pop
			_gameData.tabParticle[i] = _gameData.tabParticle[_gameData.tabParticle.size() - 1];
			_gameData.tabParticle.pop_back();

		}
	}
}

void Particle::Display(GameData& _gameData)
{
	for (int i = 0; i <_gameData.tabParticle.size(); i++)
	{
		if (_gameData.tabParticle[i]->type == TypeParticle::LEAF)
		{
			_gameData.leaf.setScale(_gameData.tabParticle[i]->scale, 0.5f);
			Utils::BlitSprite(_gameData.leaf, { _gameData.tabParticle[i]->pos.x, _gameData.tabParticle[i]->pos.y }, _gameData.tabParticle[i]->rotation, _gameData.window);
		}
		else
		{
			Utils::BlitSprite(_gameData.corruption, { _gameData.tabParticle[i]->pos.x, _gameData.tabParticle[i]->pos.y }, _gameData.tabParticle[i]->rotation, _gameData.window);
		}
	}
}

void Particle::Display3D(GameData& _gameData)
{
	for (int i = 0; i <_gameData.tabParticle.size(); i++)
	{
		if (_gameData.tabParticle[i]->type == TypeParticle::LEAF)
		{
			_gameData.leaf.setScale(_gameData.tabParticle[i]->scale, 0.5f);
			Utils::BlitSprite(_gameData.leaf, sf::Vector2f{ _gameData.tabParticle[i]->pos.x + (TROAD_GAP * (-1 * _gameData.tabParticle[i]->pos.y + WORKING_HEIGHT) / 100.f),_gameData.tabParticle[i]->pos.y }, _gameData.tabParticle[i]->rotation, _gameData.renderTextureRed);
			Utils::BlitSprite(_gameData.leaf, sf::Vector2f{ _gameData.tabParticle[i]->pos.x - (TROAD_GAP * (-1 * _gameData.tabParticle[i]->pos.y + WORKING_HEIGHT) / 100.f),_gameData.tabParticle[i]->pos.y }, _gameData.tabParticle[i]->rotation, _gameData.renderTextureBlue);
		}
		else
		{
			Utils::BlitSprite(_gameData.corruption, sf::Vector2f{ _gameData.tabParticle[i]->pos.x + (TROAD_GAP * (-1 * _gameData.tabParticle[i]->pos.y + WORKING_HEIGHT) / 100.f),_gameData.tabParticle[i]->pos.y }, _gameData.tabParticle[i]->rotation, _gameData.renderTextureRed);
			Utils::BlitSprite(_gameData.corruption, sf::Vector2f{ _gameData.tabParticle[i]->pos.x - (TROAD_GAP * (-1 * _gameData.tabParticle[i]->pos.y + WORKING_HEIGHT) / 100.f),_gameData.tabParticle[i]->pos.y }, _gameData.tabParticle[i]->rotation, _gameData.renderTextureBlue);
		}
	}
}