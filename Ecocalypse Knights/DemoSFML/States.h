#ifndef STATES_H
#define STATES_H
#include "stdafx.h"

#include "Common.h"
#include "Animation.h"
#include "Fade.h"
#include "Button.h"

class States
{
public:
	virtual ~States();

	//vitual obligatory
	virtual void Update(GameData& _gameData) = 0;
	virtual void Display(GameData& _gameData) = 0;
	virtual void Display3D(GameData& _gameData) = 0;

	//vitual potencial 
	virtual void KeyPressed(GameData& _gameData) {};
	virtual void JoystickPressed(GameData& _gameData) {};
	virtual void MousePressed(GameData& _gameData) {};
	virtual void MouseMoved(GameData& _gameData) {};
	virtual void MouseReleased(GameData& _gameData) {};
	virtual void LostFocus(GameData& _gameData) {};
	virtual void UpdatePos(GameData& _gameData) {};
	virtual void UpdateButtonPressedEffect(GameData& _gameData) {};
	virtual void Init(GameData& _gameData) {};

protected:

	void UpdateAllButtonsAnim(const float& _dt);
	void SetButtonSelectedPressed(GameData& _gameData);
	void UpdateButtonsSelection(GameData& _gameData);
	void DisplayButtons(sf::RenderWindow& _window);
	void DisplayButtonsMulti(sf::RenderWindow& _window);
	void DisplayButtons3D(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	void DownUpKeysButtons(GameData& _gameData);
	void RightLeftKeysButtons(GameData& _gameData);
	void DownUpKeysButtonsMulti(GameData& _gameData);
	void DownUpJoysticksButtons(GameData& _gameData);
	void LeftRightJoysticksButtons(GameData& _gameData);

	float joystickTimer = 0;
	unsigned char buttonSelected = 0;
	std::vector<Button*> buttonVect;
	std::vector<Button*> buttonVectMulti;
	Fade* fade = nullptr;
};

#endif // !STATES_H



