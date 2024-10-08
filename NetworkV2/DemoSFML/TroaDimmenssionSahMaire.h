#ifndef TROADIMMENSSIONSAHMAIRE_HPP
#define TROADIMMENSSIONSAHMAIRE_HPP

#include "stdafx.h"
#include "Common.h"

class TroaDimmenssionSahMaire
{
public:
	TroaDimmenssionSahMaire();
	void TakeSreenRedFilter(GameData& _gameData);
	void TakeSreenBlueFilter(GameData& _gameData);
	void DisplayFilter(GameData& _gameData);

private:
	sf::Shader red;
	sf::Shader blue;
	sf::Texture* leftEye;
	sf::Texture* rightEye;
};

#endif //!TROADIMMENSSIONSAMAIRE_HPP