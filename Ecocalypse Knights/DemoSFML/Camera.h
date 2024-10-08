#ifndef CAMERA_H
#define CAMERA_H

#include "Common.h"
#include "Paralax.h"
#include "stdafx.h"

class Camera
{
public:
	Camera(GameData& _gameData);
	~Camera() = default;

	void Reload(GameData& _gameData);
	void ReloadMenu(GameData& _gameData);
	void Move(GameData& _gameData);
	void TpPlayerLevel3(Player* _player, bool& _playerIsShrinks);
	void Update(GameData& _gameData);
	void UpdatePause(GameData& _gameData);
	void UpdateMenu(GameData& _gameData);
	void UpdateArena(void);
	void Display(GameData& _gameData);
	void Display3D(GameData& _gameData);
	void DisplayFirstPlan(GameData& _gameData);
	void DisplayFirstPlan(sf::RenderTexture& _red, sf::RenderTexture& _blue, LevelState _level,bool);
	sf::View& GetView();
	const float GetRatioX();
	const float GetRatioY();
	void SetRatio(float _ratio);
	void SetGoTo(sf::Vector2f _goTo, GameData& _gameData);
	void SetMoveMenuToPlayerSelection(bool _isMoving);
	const bool GetMoveMenuToPlayerSelection(void);
	void SetDefaultCenter(void);
	void SetDefaultZoom(void);
	void SetLastCenter(void);
	void SetLastZoom(void);
	void SetCenter(sf::Vector2f _center);
	Paralax& GetParalax();
	void CameraForFilterBlue();
	void ResetPosCameraAfterFilterBlue();
	
	float lastCenterX;
	float centerXGlobal;
	bool moveMenuToPlayerSelection;
	sf::Vector2f lastCenter;
	float lastRatio;
private:

	sf::View view;
	float ratio;
	Paralax paralax;
	
	sf::Vector2f speed;
	sf::Vector2f goTo;
	float goToRatio;
	sf::Vector2f lastCenterForRealWallah;
	sf::Vector2f lastZoomForRealWallah;

	float timer;
	bool bossDeath = true;

	sf::Vector2f posInMap;
};
#endif
