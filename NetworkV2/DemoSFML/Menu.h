#ifndef MENU_H

#define MENU_H

#include "stdafx.h"

#include "States.h"
#include "Animation.h"
#include "Audio.h"
#include "Easings.h"

enum class ButtonTypePhase1
{
	STORY,
	ARENA,
	SETTINGS,
	QUIT,
};

enum class ButtonTypePhase2
{
	LOCAL,
	NETWORK,
	BACK,
};

enum class ButtonTypePhase3
{
	FREE_FOR_ALL,
	TEAM_FIGHT,
	BACK,
};

enum class PhaseType
{
	PHASE1,
	PHASE2,
	PHASE3,
	SWIFTING1_BACK,
	SWIFTING2_BACK,
	SWIFTING3_BACK,
	SWIFTING1_FRONT,
	SWIFTING2_FRONT,
	SWIFTING3_FRONT,
};

class Menu : public States
{
public:
	Menu(GameData& _gameData);
	~Menu();

	void Reload(GameData& _gameData);
	void Update(GameData& _gameData) override;
	void UpdateButtonPressedEffect(GameData& _gameData) override;
	void Display(GameData& _gameData) override;
	void Display3D(GameData& _gameData) override;
	void KeyPressed(GameData& _gameData)override;
	void JoystickPressed(GameData& _gameData)override;
	void SetAlpha(const float _alpha);

private:

	void InitSwordVector(void);
	void InitPhase1(GameData& _gameData);
	void InitPhase2(sf::Sprite& _sprite);
	void InitPhase3(void);
	void FadeManagement(GameData& _gameData);

	void UpdateSwordPos(float _dt, sf::Sprite& _swordSprite);
	void UpdateButtonPressedEffectPhase1(GameData& _gameData);
	void UpdateButtonPressedEffectPhase2(GameData& _gameData);
	void UpdateButtonPressedEffectPhase3(GameData& _gameData);

	void UpdateAnim(GameData& _gameData);
	void UpdateAlpha(GameData& _gameData);
	void UpdateButtonSwiftIn(GameData& _gameData);
	void UpdateButtonSwiftOut(GameData& _gameData);
	void UpdateThierrySwift(GameData& _gameData);	void JoystickHandler(GameData& _gameData);
	void GoToPhase1(GameData& _gameData);

	sf::Texture textureBackground;
	sf::Sprite spriteBackground;

	sf::Texture textureGameName;
	sf::Sprite spriteGameName;

	std::string musicKey;
	int buttonPressSound;
	int buttonSelectSound;

	int lastPos = (int)ButtonTypePhase1::STORY;
	int nextPos = (int)ButtonTypePhase1::STORY;
	bool isSwifting = false;
	bool swordHasSwoosh = false;
	float timerEasing;
	float timerEasingButton;

	PhaseType currentPhase = PhaseType::SWIFTING1_FRONT;
	PhaseType nextPhase = PhaseType::PHASE1;

	std::vector<sf::Vector2f> startPosEasing;
	std::vector<sf::Vector2f> endPosEasing;
	float timerEasing2 = 0.f;
	bool movingFront;

	sf::Vector2f swordCurrentPos;
	std::vector<sf::Vector2f> swordsPos;

	Fade* fade = nullptr;
};
#endif

