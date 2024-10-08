#include "stdafx.h"
#include "Network.h"
#include "Player.hpp"
#include "Mob.hpp"
#include "LevelManager.h"
#include "Boar.hpp"
#include "Witch.hpp"
#include "Camera.h"

//////////////////////////////////////////////////////////////////////////
////////////////////       PACKET       /////////////////////////////////
/////////////////////////////////////////////////////////////////////////

// Creation d'une structure data qui stocke les infos à envoyer du Player
PlayerData TransfusePlayerToData(Player& _player)
{
	PlayerData data;

	//misc
	data.state = _player.GetState();
	data.lastDir = _player.GetLastDir();
	data.pos = _player.GetPos();
	data.playerSelected = _player.GetPlayerSelected();
	data.life = _player.GetLife();
	data.mana = _player.GetMana();
	//sword attacks
	data.inAir = _player.GetInAir();
	data.heavyGoUp = _player.GetHeavyAttackGoUp();
	data.comboNumber = _player.GetComboNumber();
	//magic
	data.auraIsActive = _player.GetAuraActivated();
	//data.isAOE = _player.GetMagicAttackAOE();
	data.aoeMagicAttack = _player.GetAOEMagic();
	data.isIceActive = _player.GetIceSpellHere();
	data.isPoisonActive = _player.GetPoisonSpellActive();
	//shield and parry
	data.isParrying = _player.GetParryState();
	data.isBreaking = _player.GetShieldIsBreaking();
	data.shieldLife = _player.GetShieldLife();
	if (_player.GetState() == PARRY)
	{
		data.parryIdle = _player.GetIdleParry();
		data.parryMoveRight = _player.GetRightParry();
		data.parryMoveLeft = _player.GetLeftParry();
	}
	data.isPropulsed = _player.GetPropulstionKnockDown();
	//revive
	data.canBeRevived = _player.GetCanBeRevive();
	data.hasFinishPraying = _player.GetHasFinishPray();

	return data;
}

// Affectation des nouvelles valeurs reçus dans le Player
void TransfuseDataToPlayer(PlayerData& data, Player& _player)
{
	//magic
	if (_player.GetAuraActivated() != data.auraIsActive)
	{
		_player.SetAuraActivated(data.auraIsActive);
	}
	if (_player.GetIceSpellHere() != data.isIceActive)
	{
		_player.SetIceMagicAOE(data.isIceActive);
	}
	if (_player.GetPoisonSpellActive() != data.isPoisonActive)
	{
		_player.SetPoisonMagicAOE(data.isPoisonActive);
	}
	if (_player.GetMagicAttackAOE() != data.isAOE)
	{
		_player.SetMagicAttackAOE(data.isAOE);
	}
	if (_player.GetMagicAttackAOE())
	{
		_player.SetAOEMagic(data.aoeMagicAttack);
	}
	//misc
	if (_player.GetPlayerSelected() != data.playerSelected)
	{
		_player.SetPlayerSelected(data.playerSelected);
	}
	if (_player.GetPos() != data.pos)
	{
		_player.SetPos(data.pos);
	}
	if (_player.GetState() != data.state)
	{
		_player.SetPlayerState(data.state);
	}
	_player.SetLastDir(data.lastDir);
	
	if (_player.GetLife() != data.life)
	{
		_player.SetLife(data.life);
	}
	if (_player.GetMana() != data.mana)
	{
		_player.SetMana(data.mana);
	}
	//sword attacks
	if (_player.GetInAir() != data.inAir)
	{
		_player.SetInAir(data.inAir);
	}
	if (_player.GetHeavyAttackGoUp() != data.heavyGoUp)
	{
		_player.SetHeavyAttackGoUp(data.heavyGoUp);
	}
	if (_player.GetComboNumber() != data.comboNumber)
	{
		_player.SetComboNumber(data.comboNumber);
	}
	//shield and parry
	if (_player.GetParryState() != data.isParrying)
	{
		_player.SetParryState(data.isParrying);
	}
	if (_player.GetShieldIsBreaking() != data.isBreaking)
	{
		_player.SetShieldIsBreaking(data.isBreaking);
	}
	if (_player.GetShieldLife() != data.shieldLife)
	{
		_player.SetShieldLife(data.shieldLife);
	}
	if (_player.GetState() == PARRY)
	{
		if (_player.GetIdleParry() != data.parryIdle)
		{
			_player.SetIdleParry(data.parryIdle);
		}
		if (_player.GetRightParry() != data.parryMoveRight)
		{
			_player.SetRightParry(data.parryMoveRight);
		}
		if (_player.GetLeftParry() != data.parryMoveLeft)
		{
			_player.SetLeftParry(data.parryMoveLeft);
		}
	}
	if (_player.GetPropulstionKnockDown() != data.isPropulsed)
	{
		_player.SetPropultionKnockDown(data.isPropulsed);
	}
	//revive
	if (_player.GetCanBeRevive() != data.canBeRevived)
	{
		_player.SetCanBeRevive(data.canBeRevived);
	}
	if (_player.GetHasFinishPray() != data.hasFinishPraying)
	{
		_player.SetHasFinishPray(data.hasFinishPraying);
	}
}

sf::Packet& operator <<(sf::Packet& _packet, sf::RectangleShape& _rect)
{
	_packet << _rect.getGlobalBounds().left;
	_packet << _rect.getGlobalBounds().top;
	_packet << _rect.getGlobalBounds().width;
	_packet << _rect.getGlobalBounds().height;

	return _packet;
}

sf::Packet& operator >>(sf::Packet& _packet, sf::RectangleShape& _rect)
{
	sf::Vector2f pos;
	_packet >> pos.x;
	_packet >> pos.y;
	_rect.setPosition(pos);
	sf::Vector2f size;
	_packet >> size.x;
	_packet >> size.y;
	_rect.setSize(size);

	return _packet;
}

sf::Packet& operator <<(sf::Packet& _packet, AOEMagicAttack& _aoeMagic)
{
	sf::RectangleShape rect = *_aoeMagic.hitbox;
	_packet << rect;
	_packet << (int)_aoeMagic.typeAOE;
	_packet << _aoeMagic.isActive;
	_packet << _aoeMagic.dir;

	return _packet;
}

sf::Packet& operator >>(sf::Packet& _packet, AOEMagicAttack& _aoeMagic)
{
	_packet >> *_aoeMagic.hitbox;
	int temp;
	_packet >> temp;
	_aoeMagic.typeAOE = (TypeAOE)temp;
	_packet >> _aoeMagic.isActive;
	_packet >> _aoeMagic.dir;

	return _packet;
}

// operator Packet pour sf::Vector2f en envoi
sf::Packet& operator <<(sf::Packet& _packet, sf::Vector2f& _vec)
{
	_packet << _vec.x;
	_packet << _vec.y;

	return _packet;
}

// operator Packet pour les states du player en envoi
sf::Packet& operator <<(sf::Packet& _packet, PlayerStates& _state)
{
	int temp = (int)_state;
	_packet << temp;

	return _packet;
}

// operator Packet pour la struct contenant les données du Player en envoi
sf::Packet& operator <<(sf::Packet& _packet, PlayerData& _data)
{
	//misc
	_packet << _data.state;
	_packet << _data.lastDir;
	_packet << _data.pos;
	_packet << _data.playerSelected;
	_packet << _data.life;
	_packet << _data.mana;
	//sword attacks
	_packet << _data.inAir;
	_packet << _data.heavyGoUp;
	_packet << _data.comboNumber;
	//magic
	_packet << _data.auraIsActive;
	_packet << _data.isAOE;
	_packet << _data.aoeMagicAttack;
	_packet << _data.isIceActive;
	_packet << _data.isPoisonActive;
	//shield and parry
	_packet << _data.isParrying;
	_packet << _data.isBreaking;
	_packet << _data.shieldLife;
	if (_data.state == PARRY)
	{
		_packet << _data.parryIdle;
		_packet << _data.parryMoveRight;
		_packet << _data.parryMoveLeft;
	}
	_packet << _data.isPropulsed;
	//revive
	_packet << _data.canBeRevived;
	_packet << _data.hasFinishPraying;

	return _packet;
}

sf::Packet& operator <<(sf::Packet& packet, Player& character)
{
	PlayerData data = TransfusePlayerToData(character);
	packet << data;
	return packet;
}

// operator Packet pour sf::Vector2f en reception
sf::Packet& operator >>(sf::Packet& _packet, sf::Vector2f& _vec)
{
	_packet >> _vec.x;
	_packet >> _vec.y;

	return _packet;
}

// operator Packet pour les states du Player en reception
sf::Packet& operator >>(sf::Packet& _packet, PlayerStates& _state)
{
	int temp;
	_packet >> temp;
	_state = (PlayerStates)temp;

	return _packet;
}

// operator Packet pour la structure contenant les données du Player en reception
sf::Packet& operator >>(sf::Packet& _packet, PlayerData& _data)
{
	//misc
	_packet >> _data.state;
	_packet >> _data.lastDir;
	_packet >> _data.pos;
	_packet >> _data.playerSelected;
	_packet >> _data.life;
	_packet >> _data.mana;
	//sword attacks
	_packet >> _data.inAir;
	_packet >> _data.heavyGoUp;
	_packet >> _data.comboNumber;
	//magic
	_packet >> _data.auraIsActive;
	_packet >> _data.isAOE;
	_packet >> _data.aoeMagicAttack;
	_packet >> _data.isIceActive;
	_packet >> _data.isPoisonActive;
	//shield and parry
	_packet >> _data.isParrying;
	_packet >> _data.isBreaking;
	_packet >> _data.shieldLife;
	if (_data.state == PARRY)
	{
		_packet >> _data.parryIdle;
		_packet >> _data.parryMoveRight;
		_packet >> _data.parryMoveLeft;
	}
	_packet >> _data.isPropulsed;
	//revive
	_packet >> _data.canBeRevived;
	_packet >> _data.hasFinishPraying;

	return _packet;
}

sf::Packet& operator >>(sf::Packet& packet, Player& character)
{
	PlayerData data;
	packet >> data;
	TransfuseDataToPlayer(data, character);
	return packet;
}

// operator Packet pour le tableau de Player en reception
sf::Packet& operator >>(sf::Packet& packet, std::vector<Player*>& character)
{
	for (int i = 0; i < character.size(); i++)
	{
		PlayerData data;
		packet >> data;
		TransfuseDataToPlayer(data, *character.at(i));
	}
	return packet;
}

// operator Packet pour le tableau de Player en envoi
sf::Packet& operator <<(sf::Packet& packet, std::vector<Player*>& character)
{
	for (int i = 0; i < character.size(); i++)
	{
		PlayerData data = TransfusePlayerToData(*character.at(i));
		packet << data;
	}
	return packet;
}

//operator packet pour les données dans le character selection en reception
sf::Packet& operator >>(sf::Packet& _packet, PlayerSkins& _tabSkin)
{
	_packet >> _tabSkin.isReady;
	_packet >> _tabSkin.slidingUp;
	_packet >> _tabSkin.slidingDown;
	_packet >> _tabSkin.currentSkin;
	return _packet;
}


//operator packet pour les données dans le character selection en envoi
sf::Packet& operator <<(sf::Packet& _packet, PlayerSkins& _tabSkin)
{
	_packet << _tabSkin.isReady;
	_packet << _tabSkin.slidingUp;
	_packet << _tabSkin.slidingDown;
	_packet << _tabSkin.currentSkin;
	return _packet;
}

//////////////////////////////////////////////////////////////////////////
////////////////////       SERVER       /////////////////////////////////
/////////////////////////////////////////////////////////////////////////

int Server::GetNbPlayer()
{
	return nbPlayer;
}

void Server::SetNbPlayer(int _nb)
{
	this->nbPlayer = _nb;
}

Server::Server(int _port)
{
	this->listener.listen(_port);
	this->selector.add(this->listener);
	this->id = 0;
	this->nbPlayer = 1;
}

void Server::UpdateServer(GameData& _gameData, LevelState* _levelState)
{
	// Attente d'une communication sur le selector
	if (this->selector.wait())
	{
		if (this->clients.size() > 0)
		{
			this->SendReceive(_gameData, _levelState);
		}
	}
}

void Server::UpdateServerSelection(GameData& _gameData, std::vector<PlayerSkins>* _tabPlayerSkin)
{
	if (this->selector.wait())
	{
		if (this->clients.size() < 3 && _gameData.network->state == NetworkState::HOSTWAIT)
		{
			this->NewConnection(_gameData);
		}
		if (this->clients.size() > 0)
		{
			this->SendReceiveSelection(_gameData, *_tabPlayerSkin); //send and receive of the all player
		}
	}
}

void Server::UpdateServer(GameData& _gameData)
{
	if (this->selector.wait())
	{
		if (this->clients.size() < 3 && _gameData.network->state == NetworkState::HOSTWAIT)
		{
			this->NewConnection(_gameData);
		}
		if (this->clients.size() > 0)
		{
			this->SendReceive(_gameData); //send and receive of the all player
		}
	}
}

void Server::NewConnection(GameData& _gameData)
{
	if (this->selector.isReady(this->listener)) //if the listener have a new entry
	{
		//add it to the vector client and accept it from the listener
		sf::TcpSocket* socket = new sf::TcpSocket{};
		sf::Packet* newClient = new sf::Packet{};
		std::string pseudo;

		this->listener.accept(*socket);
		this->selector.add(*socket);
		this->clients.push_back(socket);

		if (socket->receive(*newClient) == sf::Socket::Done)//receive data from the newPlayer
		{
			*newClient >> pseudo;//receive pseudo

			this->nbPlayer++; //up number player

			Player* temp = new Player(_gameData, this->nbPlayer - 1); //add it to the vector
			temp->Init();
			_gameData.tabPlayers.push_back(temp);

			newClient->clear();//clear data in the packet

			//packet << id of the player << number player
			*newClient << this->nbPlayer - 1 << this->nbPlayer << _gameData.camera->GetView().getCenter().y;//send his ID and the number of player the ID correspond of the number player -1 because it start to 1
			socket->send(*newClient);
		}

		delete newClient;//delete the packet
		newClient = nullptr;
	}
}

void Server::SendReceive(GameData& _gameData)
{
	if (!this->selector.isReady(this->listener))
	{
		for (int i = 0; i < this->clients.size(); i++)
		{
			this->packet.clear();
			if (this->clients.at(i)->receive(this->packet) == sf::Socket::Done)
			{
				this->packet >> *_gameData.tabPlayers[i + 1] >> _gameData.gameState;
				this->packet.clear();

				for (int j = 0; j < _gameData.tabPlayers.size(); j++)
				{
					this->packet << this->nbPlayer << *_gameData.tabPlayers[j] << _gameData.gameState;
				}
				this->clients.at(i)->send(this->packet);
			}
		}
	}
}

void Server::SendReceiveSelection(GameData& _gameData, std::vector<PlayerSkins>& _tabPlayerSkin)
{
	if (!this->selector.isReady(this->listener))
	{
		for (int i = 0; i < this->clients.size(); i++)
		{
			this->packet.clear();
			if (this->clients.at(i)->receive(this->packet) == sf::Socket::Done)
			{
				//Recu des infos de selection de chaque client
				this->packet >> _tabPlayerSkin[i + 1];
				this->packet.clear();

				//Envoi des informations du serveur aux autres clients
				for (int j = 0; j < _gameData.tabPlayers.size(); j++)
				{
					//regarder ici si le nombre de player est suffisant
					this->packet << this->nbPlayer << _tabPlayerSkin[j];
					//envoi du state de jeu
					this->packet << _gameData.gameState;
				}
				this->clients.at(i)->send(this->packet);
			}
		}
	}
}

void Server::SendReceive(GameData& _gameData, LevelState* _levelState)
{
	if (!this->selector.isReady(this->listener))
	{
		for (int i = 0; i < this->clients.size(); i++)
		{
			this->packet.clear();
			if (this->clients.at(i)->receive(this->packet) == sf::Socket::Done)
			{
				int temp;
				this->packet >> temp >> *_gameData.tabPlayers[i + 1];
				if (*_levelState != temp)
				{
					/*if (*_levelState == MAP)
					{
						LevelState temp2 = (LevelState)temp;
						_gameData.levelManager->Init(temp2, _gameData);
					}*/
					*_levelState = (LevelState)temp;
				}
				this->packet.clear();

				for (int j = 0; j < _gameData.tabPlayers.size(); j++)
				{
					this->packet << (int)*_levelState << this->nbPlayer << *_gameData.tabPlayers[j] << _gameData.actualWave;
				}
				this->clients.at(i)->send(this->packet);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
////////////////////       CLIENT       /////////////////////////////////
/////////////////////////////////////////////////////////////////////////

Client::Client(int _port, std::string _path, std::string _name, GameData& _gameData)
{
	this->server = sf::IpAddress(_path); //ip address of the server connect
	this->name = _name; //name of the current client

	if (socket.connect(this->server, _port) == sf::Socket::Done) //test if we can connect to the server
	{
		this->packet << this->name; //send the name choose
		this->socket.send(this->packet); //send to the sever

		this->packet.clear(); //clear data in the packet

		this->socket.receive(this->packet);//set data receive of the sever in the packet
		float camY;
		this->packet >> this->id >> this->nbPlayer >> camY; //have the id to change the good player in the vector tabPlayer and the number of actual player in the vector of the server
		_gameData.camera->SetCenter({ _gameData.camera->GetView().getCenter().x, camY });
	}
}

void Client::SendToServer2(sf::Packet& _packet)
{
	if (this->socket.send(_packet) == sf::Socket::Status::Done)
	{
	}
	_packet.clear();
}

void Client::ReceiveToServer2(sf::Packet& _packet)
{
	if (this->socket.receive(_packet) == sf::Socket::Status::Done)
	{
	}
}

void LoadPacket(sf::Packet& _packet, Player* _player)
{
	_packet.clear();
	sf::Vector2f pos = _player->GetPos();
	bool inAir = _player->GetInAir();
	int dir = _player->GetLastDir();
	PlayerStates state = _player->GetState();
	_packet << pos << inAir << dir << (int)state;
}

void LoadPacket(sf::Packet& _packet, PlayerSkins& _playerSkin)
{
	_packet.clear();
	_packet << _playerSkin;
}

void LoadPacket(sf::Packet& _packet, Player* _player, GameData& _gameData, LevelState* _levelState)
{
	_packet.clear();
	_packet << (int)*_levelState;
	_packet << *_player;
}

void ReadPacket(sf::Packet& _packet, std::vector<Player*>* _player, int _id, GameData& _gameData, LevelState* _levelState)
{
	for (int i = 0; i < _player->size(); i++)
	{
		int nbPlayer;
		if (i == _id)
		{
			// Ecrasement des donnees du joueurs operant
			Player* playertest = new Player(_gameData, 8);
			int trash;

			_packet >> trash >> trash >> *playertest >> trash;

			delete playertest;
			playertest = nullptr;
		}
		else
		{
			int temp;
			int wave;
			_packet >> temp >> nbPlayer >> *_player->at(i) >> wave;
			_gameData.actualWave = (Wave)wave;
			if (temp != *_levelState)
			{
				*_levelState = (LevelState)temp;
			}
		}
	}
	_packet.clear();
}

void ReadPacket(sf::Packet& _packet, std::vector<Player*>* _player, int _id, GameData& _gameData, std::vector<PlayerSkins>* _tabPlayerSkin)
{
	for (int i = 0; i < _player->size(); i++)//on all player
	{
		if (i == _id) //if the current player in the packet is the same as the client just delete him
		{
			// Ecrasement des donnees du joueurs operant
			int trash;
			PlayerSkins temp2;
			_packet >> trash >> temp2 >> trash;
		}
		else
		{
			//change the current data of the player
			_packet >> _gameData.network->client->nbPlayer >> _tabPlayerSkin->at(i);
			//change the current state of the game
			unsigned char tempState;
			_packet >> tempState;
			if (tempState != _gameData.gameState && tempState >= 0 && tempState < NB_STATES)
			{
				_gameData.gameState = tempState;
				_gameData.camera->lastCenter = { WORKING_WIDTH / 2.f, WORKING_HEIGHT / 2.f };
				_gameData.camera->moveMenuToPlayerSelection = false;
				_gameData.network->state = NetworkState::CLIENT;
			}
		}
	}
	_packet.clear();//clear all data in the packet
}

//////////////////////////////////////////////////////////////////////////
////////////////////       NETWORK       ////////////////////////////////
/////////////////////////////////////////////////////////////////////////

std::string GetReplacedString(std::string _string)
{
	std::string str;

	for (int i = 0; i < _string.size(); i++)
	{
		str.append("*");
	}

	return str;
}

void Network::SetEnteredCharServer(sf::Event* _event)
{
	if (_event->type == sf::Event::TextEntered)
	{
		// Test d'appui sur la touche backspace
		if (_event->text.unicode == 8)
		{
			// On enleve la dernière lettre entree
			if (this->tabStr[0]->getSize() > 0)
			{
				this->tabStr[0]->erase(this->tabStr[0]->getSize() - 1, 1);
				this->tabText[0]->setString(*this->tabStr[0]);
			}
		}
		else if (_event->text.unicode != 37 && _event->text.unicode != 39) //On empêche les flèches du clavier de rentrer dans la string
		{
			// On ajoute la lettre ou le chiffre entre
			*this->tabStr[0] += _event->text.unicode;
			this->tabText[0]->setString(*this->tabStr[0]);
		}

		this->serverString = this->tabStr[0]->toAnsiString();
		// Transformation de la chaine de caractere en *
		*this->tabStr[3] = sf::String(GetReplacedString(this->serverString));
		this->tabText[3]->setString(*this->tabStr[3]);
	}
}

void Network::SetEnteredCharName(sf::Event* _event)
{
	if (_event->type == sf::Event::TextEntered)
	{
		// Test d'appui sur la touche backspace
		if (_event->text.unicode == 8)
		{
			// On enleve la dernière lettre entree
			this->tabStr[1]->erase(this->tabStr[1]->getSize() - 1, 1);
			this->tabText[1]->setString(*this->tabStr[1]);
		}
		else
		{
			// On ajoute la lettre ou le chiffre entre
			*this->tabStr[1] += _event->text.unicode;
			this->tabText[1]->setString(*this->tabStr[1]);
		}

		nameString = this->tabStr[1]->toAnsiString();
	}
}

void Network::InitText()
{
	// Chaines de caractere par defaut pour les boutons et les champs addresse ip et nom
	std::string temp = "enter ip address";
	std::vector<std::string> tabString = { temp, "enter name", "reveal", GetReplacedString(temp), "confirm", "host", "client" };
	this->font.loadFromFile("Assets/Fonts/HudFont.otf");

	for (int i = 0; i < tabString.size(); i++)
	{
		sf::String* tempString = new sf::String(tabString[i]);
		tabStr.push_back(tempString);
	}

	for (int i = 0; i < 7; i++)
	{
		sf::Text* temp = new sf::Text;
		temp->setCharacterSize(80);
		temp->setFont(this->font);
		temp->setFillColor(sf::Color(186, 116, 63 , 255));
		temp->setString(*this->tabStr[i]);
		this->tabText.push_back(temp);
	}

	this->tabText[0]->setPosition(WORKING_WIDTH / 3 + 80, WORKING_HEIGHT / 3 + GAP + 50);
	this->tabText[1]->setPosition(100, 320 + GAP);
	this->tabText[2]->setPosition(WORKING_WIDTH - 200, WORKING_HEIGHT - 200 + GAP);
	this->tabText[3]->setPosition(WORKING_WIDTH / 3 + 80, WORKING_HEIGHT / 3 + GAP + 50);
	this->tabText[4]->setPosition(WORKING_WIDTH / 2 - 50, WORKING_HEIGHT - 200 + GAP);
	//text des bouton host et client dont la position x est décalée pa rapport au rectangle du bouton et similairement pour le y
	this->tabText[5]->setPosition(WORKING_WIDTH / 2 + 75, WORKING_HEIGHT / 3 + 50 + GAP);
	this->tabText[6]->setPosition(WORKING_WIDTH / 2 + 75, WORKING_HEIGHT / 1.5 + 50 + GAP);
}

void Network::InitRectangle()
{
	this->bgText = new sf::RectangleShape;
	this->bgServerText = new sf::RectangleShape;
	this->bgNameText = new sf::RectangleShape;

	this->bgText->setPosition(100, 100 + GAP);
	this->bgText->setSize({ WORKING_WIDTH - 200, WORKING_HEIGHT - 200 });
	this->bgText->setFillColor(sf::Color::Blue);

	this->bgServerText->setPosition(100, 200 + GAP);
	this->bgServerText->setSize({ WORKING_WIDTH - 200, 50 });
	this->bgServerText->setFillColor(sf::Color::Red);

	this->bgNameText->setPosition(100, 300 + GAP);
	this->bgNameText->setSize({ WORKING_WIDTH - 200, 50 });
	this->bgNameText->setFillColor(sf::Color::Red);
}

void Network::InitButton()
{
	for (int i = 0; i < NBRBUTTON; i++)
	{
		sf::RectangleShape* button = new sf::RectangleShape;
		button->setSize({ 200, 100 });
		button->setFillColor(sf::Color::Cyan);
		this->tabButton.push_back(button);
	}
	this->tabButton[0]->setPosition(WORKING_WIDTH / 2, WORKING_HEIGHT / 3 + GAP);
	this->tabButton[1]->setPosition(WORKING_WIDTH / 2, WORKING_HEIGHT / 1.5 + GAP);

	this->revealButton = new sf::RectangleShape;
	this->revealButton->setSize({ 100, 50 });
	this->revealButton->setFillColor(sf::Color::Red);
	this->revealButton->setPosition(WORKING_WIDTH - 200, WORKING_HEIGHT - 200 + GAP);

	this->confirmButton = new sf::RectangleShape;
	this->confirmButton->setSize({ 150, 50 });
	this->confirmButton->setFillColor(sf::Color::Red);
	this->confirmButton->setPosition(WORKING_WIDTH / 2 - 50, WORKING_HEIGHT - 200 + GAP);
}

Network::Network()
{
	this->state = NetworkState::PENDING;
	InitText();
	InitRectangle();
	InitButton();
	this->wantReplaceServer = false;
	this->wantReplaceName = false;
	this->timerButton = 0;
	this->isRevealed = true;

	textureBackground.loadFromFile("Assets/Images/BackGround/TitleScreen.png");
	spriteBackground.setTexture(textureBackground);
	textureIPBar.loadFromFile("Assets/Images/Menu/IPSelected.png");
	spriteIPBar.setTexture(textureIPBar);
	spriteIPBar.setPosition(WORKING_WIDTH / 2 - WIDTH_IP_BAR / 2, WORKING_HEIGHT / 3 + GAP);
}

Network::~Network()
{
	delete this->server;
	delete this->client;
	for (int i = 0; i < NBRBUTTON; i++)
	{
		delete tabButton[i];
	}
	delete player;
	delete revealButton;
}

void Network::SynchronizeMob(GameData& _gameData, LevelState& _state)
{
	if (this->state == NetworkState::HOST)
	{
		SendTabMob(_gameData, _state);
	}
	else if (this->state == NetworkState::CLIENT)
	{
		ReceiveTabMob(_gameData, _state);
	}
}

void Network::SendTabMob(GameData& _gameData, LevelState& _state)
{
	for (int j = 0; j < this->server->clients.size(); j++)
	{
		int size = (int)_gameData.tabMob.size();
		int sizeCircle = (int)_gameData.levelManager->vectorCircle.size();
		this->packet << _gameData.mobRespawn << size << sizeCircle;
		for (int i = 0; i < size; i++)
		{
			sf::Vector2f pos = _gameData.tabMob[i]->GetPos();
			int mobHp = _gameData.tabMob[i]->GetLife();
			int mobState = (int)_gameData.tabMob[i]->GetState();
			this->packet << pos << mobHp << mobState;
			int target = _gameData.tabMob[i]->GetTargetPlayer();
			packet << target;
		}
		for (int i = 0; i < sizeCircle; i++)
		{
			packet << _gameData.levelManager->vectorCircle[i].pos;
		}
		size = (int)_gameData.drop.size();
		this->packet << size;
		for (int i = 0; i < size; i++)
		{
			sf::Vector2f pos = _gameData.drop[i]->pos;
			this->packet << pos;
		}
		this->server->clients.at(j)->send(this->packet);
	}
	this->packet.clear();
}

void Network::ReceiveTabMob(GameData& _gameData, LevelState& _state)
{
	if (this->client->socket.receive(this->packet) == sf::Socket::Done)
	{
		int size;
		int sizeCircle;
		this->packet >> _gameData.mobRespawn >> size >> sizeCircle;
		if (sizeCircle != _gameData.levelManager->vectorCircle.size() && sizeCircle != 0)
		{
			std::cout << "sizeCircle " << std::endl;
			std::cout << "vectorCircleSize " << std::endl;
			std::cout << "___________" << std::endl;
			_gameData.levelManager->CreateCircle(_gameData);
		}
		if (_gameData.tabMob.size() == size)
		{
			for (int i = 0; i < size; i++)
			{
				sf::Vector2f pos;
				int mobHp;
				int mobState;
				this->packet >> pos >> mobHp >> mobState;
				_gameData.tabMob[i]->SetPos(pos);
				_gameData.tabMob[i]->SetLife(mobHp);
				_gameData.tabMob[i]->SetState((MobState)mobState);
				int target;
				packet >> target;
				_gameData.tabMob[i]->SetTargetPlayer(target);
			}

			for (int i = 0; i < sizeCircle; i++)
			{
				packet >> _gameData.levelManager->vectorCircle[i].pos;
				_gameData.levelManager->vectorCircle[i].circleInvoc.setPosition(_gameData.levelManager->vectorCircle[i].pos);
			}

			packet >> size;
			bool recreate = false;
			if (size != _gameData.drop.size())
			{
				recreate = true;
				_gameData.drop.clear();
			}
			for (int i = 0; i < size; i++)
			{
				if (recreate)
				{
					Drop* temp = new Drop;
					_gameData.drop.push_back(temp);
				}
				this->packet >> _gameData.drop[i]->pos;
			}
		}
		else
		{
			_gameData.levelManager->CreateMobMulti(_gameData);
			for (int i = 0; i < size; i++)
			{
				sf::Vector2f pos;
				int mobHp;
				int mobState;
				this->packet >> pos >> mobHp >> mobState;
				_gameData.tabMob[i]->SetPos(pos);
				_gameData.tabMob[i]->SetLife(mobHp);
				_gameData.tabMob[i]->SetState((MobState)mobState);
				int target;
				packet >> target;
				_gameData.tabMob[i]->SetTargetPlayer(target);
			}
			for (int i = 0; i < sizeCircle; i++)
			{
				packet >> _gameData.levelManager->vectorCircle[i].pos;
				_gameData.levelManager->vectorCircle[i].circleInvoc.setPosition(_gameData.levelManager->vectorCircle[i].pos);
			}
			packet >> size;
			bool recreate = false;
			if (size != _gameData.drop.size())
			{
				recreate = true;
				_gameData.drop.clear();
			}
			for (int i = 0; i < size; i++)
			{
				if (recreate)
				{
					Drop* temp = new Drop;
					_gameData.drop.push_back(temp);
				}
				this->packet >> _gameData.drop[i]->pos;
			}
		}
	}
	this->packet.clear();
}

void Network::PendingStateUpdate(sf::Vector2i _mousePos, float _dt, GameData& _gameData)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->state == NetworkState::PENDING && timerButton == 0)
	{
		if (_mousePos.x > tabButton[0]->getPosition().x && _mousePos.x < tabButton[0]->getPosition().x + tabButton[0]->getSize().x)//click button host
		{
			if (_mousePos.y > tabButton[0]->getPosition().y - GAP && _mousePos.y < tabButton[0]->getPosition().y + tabButton[0]->getSize().y - GAP)
			{
				_gameData.tabPlayers.erase(_gameData.tabPlayers.begin(), _gameData.tabPlayers.end()); //erase the current nb player security

				this->server = new Server{ PORT }; //create the server

				Player* temp = new Player(_gameData, 0); //init tabPlayer
				temp->Init();
				_gameData.tabPlayers.push_back(temp);
				_gameData.gameState = CHARACTER_SELECTION;
				_gameData.camera->SetCenter({ _gameData.camera->GetView().getCenter().x, 1499.f });
				_gameData.network->state = NetworkState::HOSTWAIT;
				this->timerButton += _dt; //timer anti-spam
				this->serverString = sf::IpAddress::getLocalAddress().toString(); // get address to show him
				*this->tabStr[0] = sf::String(this->serverString);
				this->tabText[0]->setString(*this->tabStr[0]);
			}
			_gameData.multiplayer = true;
			this->isRevealed = true;
		}

		if (_mousePos.x > tabButton[1]->getPosition().x && _mousePos.x < tabButton[1]->getPosition().x + tabButton[1]->getSize().x)//click button client
		{
			if (_mousePos.y > tabButton[1]->getPosition().y - GAP && _mousePos.y < tabButton[1]->getPosition().y + tabButton[1]->getSize().y - GAP)
			{
				_gameData.tabPlayers.erase(_gameData.tabPlayers.begin(), _gameData.tabPlayers.end()); //erase the current nb player security

				this->state = NetworkState::CLIENTWAIT; // state client wait to connect on a server
				this->timerButton += _dt; //timer anti-spam
			}
		}
	}
	bool temp = true;
	_gameData.animFlame->UpdateLeftToRight(_gameData.flame, _gameData.dt, 9, 0, temp, true);
}

void Network::RevealAddress(sf::Vector2i _mousePos, float _dt)
{
	if (_mousePos.x > revealButton->getPosition().x && _mousePos.x < revealButton->getPosition().x + revealButton->getSize().x)
	{
		if (_mousePos.y > revealButton->getPosition().y - GAP && _mousePos.y < revealButton->getPosition().y + revealButton->getSize().y - GAP)
		{
			if (this->isRevealed)
			{
				isRevealed = false;
			}
			else
			{
				isRevealed = true;
			}
			this->timerButton += _dt;
		}
	}
}

// Fonction de test de changement de l'addresse Ip
void Network::ChangeServerText(sf::Vector2i _mousePos)
{
	if (_mousePos.x > bgServerText->getPosition().x && _mousePos.x < bgServerText->getPosition().x + bgServerText->getSize().x)
	{
		if (_mousePos.y > bgServerText->getPosition().y - GAP && _mousePos.y < bgServerText->getPosition().y + bgServerText->getSize().y - GAP)
		{
			wantReplaceServer = true;
			wantReplaceName = false;
			tabStr[0]->clear();
		}
	}
}

// Fonction de test de changement du nom
void Network::ChangeNameText(sf::Vector2i _mousePos)
{
	if (_mousePos.x > bgNameText->getPosition().x && _mousePos.x < bgNameText->getPosition().x + bgNameText->getSize().x)
	{
		if (_mousePos.y > bgNameText->getPosition().y - GAP && _mousePos.y < bgNameText->getPosition().y + bgNameText->getSize().y - GAP)
		{
			wantReplaceName = true;
			wantReplaceServer = false;
			tabStr[1]->clear();
		}
	}
}

void Network::ConfirmButton(sf::Vector2i _mousePos, float _dt, GameData& _gameData)
{
	if (_mousePos.x > confirmButton->getPosition().x && _mousePos.x < confirmButton->getPosition().x + confirmButton->getSize().x)//click button confirm
	{
		if (_mousePos.y > confirmButton->getPosition().y - GAP && _mousePos.y < confirmButton->getPosition().y + confirmButton->getSize().y - GAP)
		{
			this->client = new Client{ PORT , this->serverString, this->nameString, _gameData }; //init the client
			Player* temp = new Player(_gameData, 0);
			temp->Init();

			while (_gameData.tabPlayers.size() != this->client->nbPlayer - 1) //add player to have the same as the server send just before -1 because the client is add at the end
			{
				Player* temp2 = new Player(_gameData, 1);
				temp2->Init();
				_gameData.tabPlayers.push_back(temp2);
			}
			_gameData.tabPlayers.push_back(temp);

			//change state
			_gameData.gameState = CHARACTER_SELECTION;
			_gameData.lastState = NETWORK_CONNECTION;
			_gameData.multiplayer = true;
		}
	}
}

void Network::WaitStateUpdate(sf::Vector2i _mousePos, GameData& _gameData, std::vector<PlayerSkins>* _tabPlayerSkin)
{
	this->packet.clear();
	if (this->state == NetworkState::HOSTWAIT) //server
	{
		server->UpdateServerSelection(_gameData, _tabPlayerSkin); //update server to add connection to his tabClient and selectors
	}
	else if (this->state == NetworkState::CLIENTWAIT && _gameData.gameState != CHARACTER_SELECTION) //client
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && timerButton == 0) //just to enter some text
		{
			RevealAddress(_mousePos, _gameData.dt);
			ChangeServerText(_mousePos);
			ChangeNameText(_mousePos);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && timerButton == 0) //try to connect on a server
		{
			ConfirmButton(_mousePos, _gameData.dt, _gameData);
		}
	}
	else if (this->state == NetworkState::CLIENTWAIT)
	{
		PlayerSkins temp = _tabPlayerSkin->at(this->client->id);
		LoadPacket(this->packet, temp);
		this->client->SendToServer2(this->packet);

		this->client->ReceiveToServer2(this->packet);
		ReadPacket(this->packet, &_gameData.tabPlayers, this->client->id, _gameData, _tabPlayerSkin);

		// Si il y a eu une nouvelle connection ajout du joueur dans le tableau de joueurs
		while (_gameData.tabPlayers.size() != this->client->nbPlayer)
		{
			Player* temp2 = new Player(_gameData, 0);
			temp2->Init();
			_gameData.tabPlayers.push_back(temp2);
		}
	}
}

void Network::WaitStateUpdate(sf::Vector2i _mousePos, GameData& _gameData)
{
	if (this->state == NetworkState::HOSTWAIT) //server
	{
		server->UpdateServer(_gameData); //update server to add connection to his tabClient and selectors
	}
	else if (this->state == NetworkState::CLIENTWAIT) //client
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && timerButton == 0) //just to enter some text
		{
			RevealAddress(_mousePos, _gameData.dt);
			ChangeServerText(_mousePos);
			ChangeNameText(_mousePos);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && timerButton == 0) //try to connect on a server
		{
			ConfirmButton(_mousePos, _gameData.dt, _gameData);
		}
	}
}

void Network::IngameStateUpdateGame(GameData& _gameData, LevelState* _levelState)
{
	//changement du state du network apres le passage dans le character selection
	if (this->state == NetworkState::HOSTWAIT)
	{
		this->state = NetworkState::HOST;
	}
	if (this->state == NetworkState::HOST)
	{
		server->UpdateServer(_gameData, _levelState);
	}
	else if (this->state == NetworkState::CLIENT)
	{
		// Envoi et reception de packets
		LoadPacket(this->packet, _gameData.tabPlayers[this->client->id], _gameData, _levelState);
		this->client->SendToServer2(this->packet);

		this->client->ReceiveToServer2(this->packet);
		ReadPacket(this->packet, &_gameData.tabPlayers, this->client->id, _gameData, _levelState);

		// Si il y a eu une nouvelle connection ajout du joueur dans le tableau de joueurs
		while (_gameData.tabPlayers.size() != this->client->nbPlayer)
		{
			Player* temp2 = new Player(_gameData, 0);
			temp2->Init();
			_gameData.tabPlayers.push_back(temp2);
		}
	}
}

// Update avec envoi du levelState pour passer des niveaux à la map
void Network::IngameStateUpdate(GameData& _gameData, LevelState* _levelState)
{
	if (this->state == NetworkState::HOST)
	{
		server->UpdateServer(_gameData, _levelState);
	}
	else if (this->state == NetworkState::CLIENT)
	{
		// Envoi et reception de packets
		LoadPacket(this->packet, _gameData.tabPlayers[this->client->id], _gameData, _levelState);
		this->client->SendToServer2(this->packet);

		this->client->ReceiveToServer2(this->packet);
		ReadPacket(this->packet, &_gameData.tabPlayers, this->client->id, _gameData, _levelState);

		// Si il y a eu une nouvelle connection ajout du joueur dans le tableau de joueurs
		while (_gameData.tabPlayers.size() != this->client->nbPlayer)
		{
			Player* temp2 = new Player(_gameData, 0);
			temp2->Init();
			_gameData.tabPlayers.push_back(temp2);
		}
	}
}

// Update dans la boucle de jeu sans besoin de passer de la map aux niveaux
void Network::IngameStateUpdate(GameData& _gameData)
{
	if (this->state == NetworkState::HOST)//server
	{
		LevelState state = MAP;
		server->UpdateServer(_gameData, &state);//send receive data
	}
	else if (this->state == NetworkState::CLIENT)//client
	{
		LoadPacket(this->packet, _gameData.tabPlayers[this->client->id]);//add the current client to the packet
		this->client->SendToServer2(this->packet);//just send it

		this->client->ReceiveToServer2(this->packet);//just receive it
		//ReadPacket(this->packet, &_gameData.tabPlayers, this->client->id, _gameData);//read data inside


		while (_gameData.tabPlayers.size() != this->client->nbPlayer) //if new player connect add it to the vector
		{
			Player* temp2 = new Player(_gameData, 0);
			temp2->Init();
			_gameData.tabPlayers.push_back(temp2);
		}
	}
}

void Network::Update(float _dt, sf::RenderWindow* _window, GameData& _gameData)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*_window);
	LevelState state = MAP;
	IngameStateUpdate(_gameData, &state); //start the game
	WaitStateUpdate(mousePos, _gameData); //connect server and client
	PendingStateUpdate(mousePos, _dt, _gameData); //wait to conncet to a server

	//update timer anti-spam
	if (this->timerButton > 0)
	{
		this->timerButton += _dt;

		if (this->timerButton >= 0.5)
		{
			this->timerButton = 0;
		}
	}
}

void Network::PendingDisplay(sf::RenderWindow* _window, Camera* _camera, sf::Sprite* _flame)
{
	_window->setView(_camera->GetView());
	_window->draw(spriteBackground);
	_window->draw(*_flame);
}

void Network::HostWaitDisplay(sf::RenderWindow* _window)
{
	_window->draw(*this->bgText);
	_window->draw(*this->bgServerText);
	_window->draw(*this->bgNameText);
	_window->draw(*this->revealButton);
	_window->draw(*this->tabText[2]);
	if (isRevealed)
	{
		_window->draw(*this->tabText[0]);
	}
	else
	{
		_window->draw(*this->tabText[3]);
	}
}

void Network::ClientWaitDisplay(sf::RenderWindow* _window, Camera* _camera, sf::Sprite* _flame)
{
	_window->setView(_camera->GetView());
	_window->draw(spriteBackground);
	_window->draw(*_flame);
	_window->draw(spriteIPBar);
	if (isRevealed)
	{
		_window->draw(*this->tabText[0]);
	}
	else
	{
		_window->draw(*this->tabText[3]);
	}
}

void Network::Display(sf::RenderWindow* _window, Camera* _camera, sf::Sprite* _flame)
{
	if (this->state == NetworkState::PENDING)
	{
		PendingDisplay(_window, _camera, _flame);
	}
	else if (this->state == NetworkState::CLIENTWAIT)
	{
		ClientWaitDisplay(_window, _camera, _flame);
	}
}

sf::RectangleShape* Network::GetButtonAtIndex(int _index)
{
	return tabButton[_index];
}