#include "RoomManager.hh"

RoomManager::RoomManager()
{
}


RoomManager::~RoomManager()
{
}

void	RoomManager::setDlManager(DlManager *dlM)
{
  _dlManager = dlM;
}

int								RoomManager::addRoom(const std::string &roomName)
{
	static int					idRoom = 0;

	std::cout << "@@@@@@@@@@@@@@ ADD ROOM" << std::endl;
	_roomList.push_back(Room(idRoom, roomName, _dlManager));
	idRoom++;
	return (idRoom - 1);
}

bool							RoomManager::removeRoom(const int roomId)
{
	std::vector<Room>::iterator	it;

	std::cout << "@@@@@@@@@@@@@@ REMOVE ROOM" << std::endl;
	it = _roomList.begin();
	while (it != _roomList.end())
	{
		if ((*it).getId() == roomId)
		{
			_roomList.erase(it);
			return (true);
		}
		it++;
	}
	return (false);
}

Room									&RoomManager::getRoomByName(const std::string &roomName)
{
	std::vector<Room>::iterator	it;

	it = _roomList.begin();
	while (it != _roomList.end())
	{
		if ((*it).getName().compare(roomName) == 0)
			return (*it);
		it++;
	}
	throw (std::runtime_error("No such room"));
}

Room									*RoomManager::getRoomById(const int roomId)
{
	std::vector<Room>::iterator	it;

	it = _roomList.begin();
	while (it != _roomList.end())
	{
		if ((*it).getId() == roomId)
			return (&(*it));
		it++;
	}
	throw (std::runtime_error("No such room"));
}

std::vector<Room> &RoomManager::getRoomList()
{
	return (_roomList);
}

bool		RoomManager::addClientToRoom(ServerClient *client, const std::string &name)
{
	try
	{
		getRoomByName(name);
	}
	catch (const std::exception &error)
	{
		std::cerr << "############ " << error.what() << std::endl;
		return (false);
	}

	if (getRoomByName(name).addClient(client) == false)
		return (false);

	if (client->getCurrentRoom() != -1)
	{
		try
		{
			getRoomById(client->getCurrentRoom());
			getRoomById(client->getCurrentRoom())->removeClient(client);
		}
		catch (const std::exception &error)
		{
			std::cerr << "############ " << error.what() << std::endl;
		}
	}

	client->setCurrentRoom(getRoomByName(name).getId());

	return (true);
}

bool		RoomManager::addClientToRoom(ServerClient *client, const int id)
{
	std::cout << "@@@@@@@@@@@@@@ ADD CLIENT TO ROOM" << std::endl;
	try
	{
		getRoomById(id);
	}
	catch (const std::exception &error)
	{
		std::cerr << "############ " << error.what() << std::endl;
		return (false);
	}

	if (getRoomById(id)->addClient(client) == false)
		return (false);

	if (client->getCurrentRoom() != - 1)
	{
		try
		{
			getRoomById(client->getCurrentRoom());
			getRoomById(client->getCurrentRoom())->removeClient(client);
		}
		catch (const std::exception &error)
		{
			std::cerr << "############ " << error.what() << std::endl;
		}

	}
	client->setCurrentRoom(id);
	return (true);
}

bool		RoomManager::removeClientFromRoom(ServerClient *client, const int id)
{
	Room	*room;

	try
	{
		room = getRoomById(id);
	}
	catch (const std::exception &error)
	{
		std::cerr << "############ " << error.what() << std::endl;
		return (false);
	}
	room->removeClient(client);
	if (room->getNbClients() == 0)
		removeRoom(id);
	return (true);
}

std::vector<Room>						RoomManager::getRoomsReadyToLaunch() const
{
	std::vector<Room>					roomsReady;
	std::vector<Room>::const_iterator	it;

	if (_roomList.size() == 0)
		return (roomsReady);

	it = _roomList.begin();
	while (it != _roomList.end())
	{
		if ((*it).getNbClients() > 0
			&& (*it).getNbClients() == (*it).getNbClientsReady()
			&& (*it).isReady() == false)
			roomsReady.push_back((*it));
		it++;
	}
	return (roomsReady);
}

std::vector<Room>						RoomManager::getRoomsReadyToPlay() const
{
	std::vector<Room>					roomsReady;
	std::vector<Room>::const_iterator	it;

	if (_roomList.size() == 0)
		return (roomsReady);

	it = _roomList.begin();
	while (it != _roomList.end())
	{
		if ((*it).getNbClients() > 0
			&& (*it).getNbClients() == (*it).getNbClientsReady()
			&& (*it).isReady() == true)
			roomsReady.push_back((*it));
		it++;
	}
	return (roomsReady);
}
