#ifndef CHARACTER_SELECTION_H

#define CHARACTER_SELECTION_H
#include "stdafx.h"

#include"Common.h"
#include "States.h"
#include "HUD.h"



#define NB_SPRITES_SKINS 4
#define TOP_HEIGHT 40
#define MIDDLE_HEIGHT 435
#define BOTTOM_HEIGHT 830
#define NB_MAX_PLAYER 4

enum class SpritesPlayersSkin
{
	RED,
	GREEN,
	BLUE,	
	YELLOW
};

struct PlayerSkins
{
	sf::Sprite spriteSkinPlayer;
	sf::Sprite nextSkinSelected;
	sf::Sprite spriteFramePlayer;
	sf::Sprite spriteBackgroundFrame;
	sf::Sprite spriteLockSkin;
	sf::Sprite spriteLockTeam;
	
	sf::Sprite spriteNumberPlayerCurrent;
	sf::Sprite spriteTeamChoice;
	std::vector <sf::Sprite> tabSpriteNumberPlayer;
	sf::Vector2f posSpriteSkinPlayer;
	sf::Vector2f posNextSkinSelected;
	bool slidingUp = false;
	bool slidingDown = false;
	int currentSkin = 0;
	float timerSpriteSkinPlayer = 0;
	float timerNextSkinSelected = 0;
	bool isReady = false;
	bool teamSelected = false;
	int teamChoice = 0;
	float timerChoiceTeam = 0.f;
};

class CharacterSelection : public States
{
public:
	CharacterSelection(GameData& _gameData);
	~CharacterSelection();

	void Reload(GameData& _gameData);
	void UpdateAnim(GameData& _gameData);
	void Update(GameData& _gameData) override;
	void Display(GameData& _gameData) override;
	void Display3D(GameData& _gameData) override ;
	void JoystickPressed(GameData& _gameData)override;
	


	std::vector <PlayerSkins> playerSkins;
	std::vector <sf::Sprite> allPlayerLockSkins;
	
private:
	void LoadSprites();
	void InitSprites();
	void InitNumber();
	void LoadPlayerSkinSpriteNumber();
	void ReloadAlphaSprite(std::vector<Player*>& _tab);
	void UpdateNbPlayers(GameData& _gameData);
	void UpdateMulti(GameData& _gameData);
	void UpdatePos(GameData& _gameData) override;
	void UpdateJoystickMove(int _i, float _dt, bool _arena, bool _teamfight);
	void UpdateJoystickMoveNetwork(int _i, Network& _network);
	void UpdateEasing(int _i, float _dt, bool _moveMenuToPlayerSelection, Audio& _audio);
	void InitComponents(GameData& _gameData);
	bool IsSkinAlreadySelected(int _skin);
	void StartGame(GameData& _gameData);
	void StartStoryMode(GameData& _gameData);
	void StartArenaMode(GameData& _gameData);
	void StartArenaModeTeamfight(GameData& _gameData);
	void JoystickPressedLocal(int _nbPlayers, Audio& _audio, Camera& _camera, float _dt, bool _arena, bool _teamfight);
	void JoystickPressedMulti(Network& _network, Audio& _audio, Camera& _camera);
	void JoystickMovedMulti(float yPos1, int i);
	int GetFirstController(int _i);
	void FadeGoToGame(bool _moveMenuToPlayerSelection, int _i, float _dt, bool _teamfight, bool _arena, bool _multiplayer);
	void DisplayWarningController(GameData& _gameData);
	void DisplayWarningController(sf::RenderTexture& _red, sf::RenderTexture& _blue);
	
	sf::Texture textureBackground;
	sf::Sprite spriteBackground;

	sf::Texture textureGameName;
	sf::Sprite spriteGameName;
	sf::Sprite spriteMask;
	sf::Sprite manetteEncule;
	sf::Sprite shadowFountain;
	sf::Sprite playersBackgroundFrame;
	sf::Sprite spriteLockRoundNumber;
	sf::Sprite spriteRoundNumberChoice;
	std::vector <sf::Sprite> allPlayerSkinsSprites;
	std::vector <sf::Sprite> allPlayerFrameSprites;
	std::vector <sf::Sprite> allPlayerTeamChoiceSprites;
	std::vector <sf::Sprite> allPlayerRoundChoiceSprites;
	int buttonPressSound;
	int buttonSelectSound;
	
	bool transitionSoundCanBePlayed = true;

	bool reload;
	bool hasPressedPlay;
	int nbPlayerReady = 0;
	int nbPlayerTeamSelected = 0;
	int nbPlayerTeam0 = 0;
	int nbPlayerTeam1 = 0;
	float waitTime = 0.f;
	float timerChoiceNumberRounds = 0.f;
	unsigned int joystickCount = 0;
	unsigned int lastJoystickCount = 0;
	int choiceRound = 0;
	bool roundNumberIsSelected = false;
	HUD hud;

	sf::Sprite fountain;
	Animation* animFountain;
	
};
#endif