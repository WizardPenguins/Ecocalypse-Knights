#ifndef PARTICLE_H

#define PARTICLE_H
#include "stdafx.h"
#include "Common.h"

class Particle
{
public:

  static  void AddParticle(GameData& _gameData);
  static  void Update(GameData& _gameData);
  static  void Display(GameData& _gameData);

};


#endif // !PARTICLE_H
