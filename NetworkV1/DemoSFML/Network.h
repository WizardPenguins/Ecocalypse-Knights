#ifndef NETWORK_H
#define NETWORK_H

#define NBRBUTTON 2
#define PORT 5000
#define NBTEXT 5
#define WIDTH_IP_BAR 900

#include "stdafx.h"
#include "Common.h"
#include "CharacterSelection.h"

enum class NetworkState
{
	HOST,
	HOSTWAIT,
	CLIENT,
	CLIENTWAIT,
	PENDING,
};

struct Data
{
	LevelState* state;
	std::vector<PlayerSkins>* tabSkin;
	sf::Vector2f* swordPos;
};

class Server
{
public:
	Server(int _port);
	~Server() = default;

	void UpdateServer(GameData& _gameData, LevelState* _levelState);
	void UpdateServerSelection(GameData& _gameData, std::vector<PlayerSkins>* _tabPlayerSkin);
	void UpdateServer(GameData& _gameData);
	void UpdateServer(GameData& _gameData, sf::Vector2f& _swordPos);
	void NewConnection(GameData& _gameData);
	void SendReceive(GameData& _gameData);
	void SendReceive(GameData& _gameData, sf::Vector2f& _swordPos);
	void SendReceive(GameData& _gameData, LevelState* _levelState);
	void SendReceiveSelection(GameData& _gameData, std::vector<PlayerSkins>& _tabPlayerSkin);
	int GetNbPlayer();
	void SetNbPlayer(int _nb);
	int id;
	std::vector<sf::TcpSocket*> clients;
	sf::SocketSelector selector;
	sf::TcpListener listener;
	int nbPlayer;//start to 1

private:
	sf::Packet packet;
};

class Client
{
public:
	Client(int _port, std::string _path, std::string _name, GameData& _gameData);
	~Client() = default;

	void SendToServer2(sf::Packet& _packet);
	void ReceiveToServer2(sf::Packet& _packet);
	sf::TcpSocket socket;
	sf::IpAddress server;
	std::string name;
	sf::Packet packet;
	int id;
	int nbPlayer; //start to 1

private:
};

class Network
{
private:
	std::vector<sf::RectangleShape*> tabButton;
	std::vector<sf::RectangleShape*> tabBg;
	sf::RectangleShape* revealButton;
	sf::RectangleShape* confirmButton;
	sf::Texture textureBackground;
	sf::Sprite spriteBackground;
	sf::Packet packet;
	sf::Font font;
	sf::RectangleShape* bgServerText;
	sf::RectangleShape* bgNameText;

	void InitText();
	void InitRectangle();
	void InitButton();
	void RevealAddress(sf::Vector2i _mousePos, float _dt);
	void ChangeServerText(sf::Vector2i _mousePos);
	void ChangeNameText(sf::Vector2i _mousePos);
	void ConfirmButton(sf::Vector2i, float _dt, GameData& _gameData);
	void SendTabMob(GameData& _gameData, LevelState& _state);
	void ReceiveTabMob(GameData& _gameData, LevelState& _state);
	void SynchroMob(GameData& _gameData, int _size);
	void SynchroDrop(GameData& _gameData, int _size);
	void SynchroCircle(GameData& _gameData, int _sizeCircle);
	void AcceptConnection(GameData& _gameData);
	void SendMob(GameData& _gameData);

public:
	NetworkState state;

	Server* server;
	Client* client;
	Player* player;


	std::vector<sf::Text*> tabText;
	sf::RectangleShape* bgText;

	sf::Texture textureIPBar;
	sf::Sprite spriteIPBar;

	std::string serverString = "";
	std::string nameString = "";
	std::vector<sf::String*> tabStr;

	bool wantReplaceServer;
	bool wantReplaceName;
	bool isRevealed;
	bool replacePlayer;
	float timerButton;

	Data data;

	Network();
	~Network();

	void Update(float _dt, sf::RenderWindow* _window, GameData& _gameData);
	void PendingStateUpdate(sf::Vector2i _mousePos, float _dt, GameData& _gameData);
	void WaitStateUpdate(sf::Vector2i _mousePos, GameData& _gameData, std::vector<PlayerSkins>* _tabPlayerSkin);
	void WaitStateUpdate(sf::Vector2i _mousePos, GameData& _gameData);
	void IngameStateUpdateGame(GameData& _gameData, LevelState* _levelState);
	void IngameStateUpdate(GameData& _gameData);
	void IngameStateUpdatePause(GameData& _gameData, sf::Vector2f& _swordPos);
	void IngameStateUpdate(GameData& _gameData, LevelState* _levelState);
	void Display(sf::RenderWindow* _window, Camera* _camera, sf::Sprite* _flame);
	void PendingDisplay(sf::RenderWindow* _window, Camera* _camera, sf::Sprite* _flame);
	void HostWaitDisplay(sf::RenderWindow* _window);
	void ClientWaitDisplay(sf::RenderWindow* _window, Camera* _camera, sf::Sprite* _flame);
	void SetEnteredCharServer(sf::Event* _event);
	void SetEnteredCharName(sf::Event* _event);
	void SynchronizeMob(GameData& _gameData, LevelState& _state);
	sf::RectangleShape* GetButtonAtIndex(int _index);
	void ParserNetworkSend(GameData& _gameData, Data& _data, int _id);
	void ParserNetworkReceive(GameData& _gameData);
	void PlayerNetworkReceive(GameData& _gameData, int _i);
	void UpdateParser(GameData& _gameData, Data& _data);
	void UpdateParserReceive(GameData& _gameData);
	void UpdateParserSend(GameData& _gameData, Data& _data);
	
};


#endif