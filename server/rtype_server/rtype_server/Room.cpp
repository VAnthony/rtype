#include "Room.hh"

Room::Room(int id)
{
	_id = id;
}

Room::Room(int id, const std::string &name)
{
	_id = id;
	_name = name;
}

Room::~Room()
{
}

std::string				Room::getName() const
{
	return (_name);
}

void					Room::setName(const std::string &name)
{
	_name = name;
}

int						Room::getId() const
{
	return (_id);
}

void					Room::pauseGame()
{

}

bool					Room::addClient(ServerClient *client)
{
	_clients.push_back(client);
	return (true);
}

bool										Room::removeClient(ServerClient *client)
{
	std::vector<ServerClient *>::iterator	it;

	it = _clients.begin();
	while (it != _clients.end())
	{
		if ((*it) == client)
		{
			_clients.erase(it);
			return (true);
		}
		it++;
	}
	return (false);
}

std::vector<ServerClient *>		Room::getClients() const
{
	return (_clients);
}

int								Room::getNbClients() const
{
	return (_clients.size());
}