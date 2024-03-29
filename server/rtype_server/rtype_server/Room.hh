#pragma once

#include "ServerClient.hh"
#include "Game.hh"
#include <string>
#include <vector>
class ServerClient;

#define MAX_PLAYERS	(4)

class Room
{
public:
  Room(const int, const std::string &, DlManager *);
	~Room();

	std::string					getName() const;
	void						setName(const std::string &);
	int							getId() const;
	void						pauseGame();
	bool						addClient(ServerClient *);
	bool						removeClient(ServerClient *);
	std::vector<ServerClient *> getClients() const;
	int							getNbClients() const;
	int							getNbClientsReady() const;
	void						setReady(const bool);
	bool						isReady() const;
	bool						updateGame();
	bool						initGame();
private:
	std::string					_name;
	int							_id;
	std::vector<ServerClient *>	_clients;
	bool						_ready;
	Game						*_gameManager;
};
