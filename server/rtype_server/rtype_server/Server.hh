#pragma once

#include <ctime>

#include "SocketServerTCP.hh"
#include "SocketServerUDP.hh"
#include "ClientManager.hh"
#include "RoomManager.hh"
#include "CmdManager.hh"
#include "BasicCmd.hh"
#include "AMutex.hh"
#include "Thread.hh"
#include "ThreadPool.hh"
#include "Mutex.hh"
#include "PathFileManager.hh"

# define GAME_LOOP_TIME (100)


class CmdManager;

class Server
{
private:
	SocketServerTCP	_socketServerTCP;
	SocketServerUDP	_socketServerUDP;
	ClientManager	_clientManager;
	RoomManager		_roomManager;
	CmdManager		_cmdManager;
	ThreadPool		_pool;
	AMutex			*_mutex;
	int				_acknowledgementNumber;
	DlManager		*_dlManager;
	PathFileManager	_fileManager;
	
	void			processMsg(const std::vector<ClientMsg> &);
	void			processBasicCmd(ServerClient *, BasicCmd *);
	void			processGames();
	void			processUDPMessages(const std::vector<ICommand *> &);
	void			checkRoomsReadyToLaunch();

public:
	Server();
	~Server();

	bool	init();
	bool	launch();
	bool	TCPLoop();
	bool	UDPLoop();
};
