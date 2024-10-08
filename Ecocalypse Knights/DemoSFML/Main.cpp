#include "stdafx.h"
#include "GameStates.h"

int main(void)
{
	srand((unsigned int)time(NULL));
	GameData gameData;
	GameState gameStates = GameState(gameData);	
	gameStates.Init(gameData);
	gameStates.Run(gameData);

	return 0;
}
