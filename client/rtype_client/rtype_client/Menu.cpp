#include "AGUIElement.hh"
#include "Menu.hh"

Menu::Menu()
{
	_sv.readFromFile();
	_playerName = _sv.getPlayerName();
	_newEvent = true;
	_roomInfo = new RoomInfoCmd();
	_id = -1;
	_th = NULL;
	_run = true;
	_successEvent = IPage::NONE;
	_errorEvent = IPage::NONE;
	_roomList = NULL;
	_roomInfo = NULL;
	_getRoomList = false;
}

Menu::~Menu()
{
	delete _roomInfo;
}

bool Menu::init()
{
	_graph->setFullScreen(true);
	_music.setDuration(-1);
	_music.setMusic(true);
	if (!_fileManager.init())
		return (false);
	_music.setFilePath(_fileManager.getRoot() + "/res/sounds/opening.wav");
	_music.setLoop(true);
	_clickSound.setDuration(-1);
	_clickSound.setFilePath(_fileManager.getRoot() + "/res/sounds/buttonClick.wav");
	_soundManager.setMusicVolume(_sv.getMusic());
	_soundManager.setSoundVolume(_sv.getSfx());
	_soundManager.play(_music);
	_t1Conn = std::chrono::high_resolution_clock::now();
	return (true);
}

void Menu::receiveInfo()
{
	std::chrono::high_resolution_clock::time_point        t1Loop;
	std::chrono::high_resolution_clock::time_point        t2Loop;
	double												duration;
	ListRoomCmd											*listRoom;
	RoomInfoCmd											*roomInfo;
	int													res;

	t1Loop = std::chrono::high_resolution_clock::now();
	while (1)
	{
		t2Loop = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2Loop - _t1Loop).count();
		if (duration >= MENU_RECEIVE_DURATION)
			_t1Loop = std::chrono::high_resolution_clock::now();
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<unsigned long>((MENU_RECEIVE_DURATION - duration))));
		_mutexRun.lock();
		if (!_run)
		{
			_mutexRun.unlock();
			return;
		}
		_mutexRun.unlock();
		
		_cmdManager.receiveCmd();
		_mutexRoomList.lock();
		if (_getRoomList)
			_cmdManager.sendRoomList();
		_mutexRoomList.unlock();
		_cmdManager.sendCmd();
	}
}

void Menu::managePageEvent()
{
	switch (_curr_event)
	{
	case IPage::HOME:
		_mutexRoomList.lock();
		_getRoomList = false;
		_mutexRoomList.unlock();
		delete (_page);
		_newEvent = true;
		_page = new HomePage(_graph, _event, _fileManager, &_soundManager);
		std::cout << "Home Page" << std::endl;
		break;
	case IPage::CREATEROOM:
		_mutexRoomList.lock();
		_getRoomList = false;
		_mutexRoomList.unlock();
		delete (_page);
		_newEvent = true;
		_page = new CreateRoomPage(_graph, _event, _fileManager, &_soundManager);
		std::cout << "CreateRoom Page" << std::endl;
		break;
	case IPage::GAME:
		std::cout << "wait launch game" << std::endl;
		
		//_successEvent = IPage::GAME;
		//_errorEvent = _page->getPageType();

		_cmdManager.setStatus();

		break;
	case IPage::CREATEROOMACTION:
		std::cout << "CreateRoomAction" << std::endl;
		_newEvent = true;
		CreateRoomPage		*tmpPage;

		tmpPage = static_cast<CreateRoomPage* >(_page);
		
		_successEvent = IPage::INSIDEROOM;
		_errorEvent = IPage::CREATEROOM;
		
		_cmdManager.createRoom(tmpPage->getRoomName(), _playerName);
		//				delete (_page);
		break;
	case IPage::LOADING:
		delete (_page);
		_newEvent = true;
		_page = new LoadingPage(_graph, _event, _fileManager, &_soundManager);
		std::cout << "Loading Page" << std::endl;
		break;
	case IPage::PLAY:
		_mutexRoomList.lock();
		_getRoomList = true;
		_mutexRoomList.unlock();
		if (_page->getPageType() == IPage::INSIDEROOM)
		{
			_successEvent = IPage::PLAY;
			_errorEvent = IPage::INSIDEROOM;
			_cmdManager.leaveRoom();
			break;
		}
		_successEvent = IPage::PLAY;
		_errorEvent = IPage::PLAY;
		_cmdManager.getRoomList();
		delete _page;
		_page = new LobbyPage(_graph, _event, _fileManager, &_soundManager);
		_newEvent = true;
		std::cout << "ListRoom Page" << std::endl;
		break;
	case IPage::SETTINGS:
		delete (_page);
		_newEvent = true;
		SettingsPage *tmpSettings;
		tmpSettings = new SettingsPage(_graph, _event, _fileManager, &_soundManager);
		tmpSettings->setServerInfo(_ip, _port);
		_page = tmpSettings;
		std::cout << "Settings Page" << std::endl;
		break;
	case IPage::SETTINGSNEXT:
		delete (_page);
		_newEvent = true;
		_page = new SettingsNextPage(_graph, _event, _fileManager, &_soundManager);
		std::cout << "SettingsNext Page" << std::endl;
		break;
	case IPage::QUIT:
		_graph->close();
		break;
	}
}

void	Menu::manageWaiting()
{
	int res;

	_curr_event = IPage::NONE;
	if (!_cmdManager.isWaiting())
	{
		if ((res = _cmdManager.getLatsReply()) == -1)
			_curr_event = _page->event();
		else if (res == 0)
		{
			if (_successEvent == IPage::INSIDEROOM)
			{
				_mutexRoomList.lock();
				_getRoomList = false;
				_mutexRoomList.unlock();
				delete _page;
				_page = new InsideRoomPage(_graph, _event, _fileManager, &_soundManager);
			}
			else if (_successEvent == IPage::PLAY)
			{
				delete _page;
				_page = new LobbyPage(_graph, _event, _fileManager, &_soundManager);
			}
		}
		else
		{
			if (_errorEvent == IPage::INSIDEROOM)
			{
				_mutexRoomList.lock();
				_getRoomList = false;
				_mutexRoomList.unlock();
				delete _page;
				_page = new InsideRoomPage(_graph, _event, _fileManager, &_soundManager);
			}
			else if (_errorEvent == IPage::PLAY)
			{
				delete _page;
				_page = new LobbyPage(_graph, _event, _fileManager, &_soundManager);
			}
			else if (_errorEvent == IPage::CREATEROOM)
			{
				delete _page;
				_page = new CreateRoomPage(_graph, _event, _fileManager, &_soundManager);
			}
		}
	}
	else if (_page && _page->getPageType() != IPage::LOADING)
	{		
		delete _page;
		_page = new LoadingPage(_graph, _event, _fileManager, &_soundManager);
		_page->init();
	}
}

void	Menu::setRoomList()
{
	LobbyPage								*page;
	std::vector<RoomIdInfos>				rooms;
	std::vector<RoomIdInfos>::iterator		it;
	
	_roomList = _cmdManager.getRoomList();
	if (!_roomList)
		return;
	page = static_cast<LobbyPage* >(_page);
	rooms = _roomList->getAllRooms();
	it = rooms.begin();
	while (it != rooms.end())
	{
		page->addRoom(*it);
		++it;
	}
}

void	Menu::setRoomInfo()
{
	std::vector<PlayerInfo>					pl;
	std::vector<PlayerInfo>::iterator		it;
	InsideRoomPage							*page;
	
	_roomInfo = _cmdManager.getRoomInfo();
	if (!_roomInfo)
		return;
	pl = _roomInfo->getPlayersList();
	page = static_cast<InsideRoomPage* >(_page);
	page->setRoomName(_roomInfo->getName());
	it = pl.begin();
	while (it != pl.end())
	{
		page->addPlayer(it->first, it->second);
		++it;
	}
}

bool	Menu::refreshRoomInfo()
{
	if (_page && _page->getPageType() == IPage::PLAY)
	{
		_page->clear();
		_mutexReceive.lock();
		setRoomList();
		_mutexReceive.unlock();
		_page->init();
	}
	else if (_page && _page->getPageType() == IPage::INSIDEROOM)
	{
		_page->clear();
		_mutexReceive.lock();
		setRoomInfo();
		if (_cmdManager.getId() != -1)
		{
			_mutexReceive.unlock();
			_mutexRun.lock();
			_run = false;
			_mutexRun.unlock();
			_pool.joinAll();
			_run = true;
			_id = _cmdManager.getId();
			startGame();
			return (true);
		}
		_mutexReceive.unlock();
		_page->init();
	}
	return (false);
}

void	Menu::startGame()
{
	std::cout << "Game" << std::endl;
	_soundManager.stopAll();
	_game.setGraph(_graph);
	_game.setEvent(_event);
	_game.setPort(9999);
	_game.setId(_id);
	_game.setIp(_ip);
	if (_roomInfo)
		_game.setNbPlayer(_roomInfo->getPlayersList().size());
	_game.launch();
}

//void	Menu::reconnexion()
//{
//
//}

bool Menu::launch()
{
	  _page = new HomePage(_graph, _event, _fileManager, &_soundManager);
	  std::chrono::high_resolution_clock::time_point        t2Loop;
	  double												duration;

	  _t1Loop = std::chrono::high_resolution_clock::now();
	  _th = new Thread();
	  _th->createThread(std::bind(&Menu::receiveInfo, this));
	  _pool.addThread(_th);


	  while (_graph->isWindowOpen())
	  {
		  t2Loop = std::chrono::high_resolution_clock::now();
		  duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2Loop - _t1Loop).count();
		  if (duration >= MENU_LOOP_DURATION)
			  _t1Loop = std::chrono::high_resolution_clock::now();
		  else
			  std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<unsigned long>((MENU_LOOP_DURATION - duration))));

		  if (refreshRoomInfo())
			  return (true);

		  while (_event->refresh())
		  {

			  manageWaiting();

			  if (_page->getPageType() == IPage::PLAY)
			  {
				  LobbyPage		*lobbyPage;

				  lobbyPage = static_cast<LobbyPage*>(_page);
				  if (lobbyPage->getSelectedRoom().first != -84)
				  {
					  std::cout << "Join Room" << std::endl;
					  _cmdManager.joinRoom(lobbyPage->getSelectedRoom().first, _playerName);
				  }
			  }


			  if (_event->getCloseEvent() || _event->getKeyStroke() == "ECHAP")
					_graph->close();

			  managePageEvent();

		  }
		  if (_newEvent && _page)
		  {
			  _page->init();
			  _newEvent = false;
		  }
		  _graph->clearWindow();
		  _page->draw();
		  _graph->refresh();
	  }
	  _mutexRun.lock();
	  _run = false;
	  _mutexRun.unlock();
	  _pool.joinAll();
  return (true);
}





















//void	Menu::initLobby()
//{
//	LobbyPage		*page;
//	ListRoomCmd		*cmd;
//	std::vector<RoomIdInfos>		rooms;
//	std::vector<RoomIdInfos>::iterator		it;
//
//	page = new LobbyPage(_graph, _event, _fileManager, &_soundManager);
//	_page = page;
//	if (!(cmd = _cmdManager.getRoomList()))
//		return;
//	std::cout << "Got rooom list" << static_cast<int>(cmd->getCommandName()) << std::endl;
//	rooms = cmd->getAllRooms();
//	it = rooms.begin();
//	while (it != rooms.end())
//	{
//		page->addRoom(*it);
//		++it;
//	}
//}
//
//bool Menu::tryToConnect()
//{
//	bool	res;
//
//	res = false;
//	//_mutex->lock();
//	//_mutexReceive.lock();
//	if (_socket && !_socket->isConnected())
//	{
//		std::cout << "TRY TO CONNECT" << std::endl;
//		_socket->init(_ip, _port);
//		_socket->connectToServer();
//		if (_socket->isConnected())
//		{
//			_cmdManager.setSocket(_socket);
//			_cmdManager.handshake();
//		}
//	}
//	if (_socket)
//		res = _socket->isConnected();
//	//_mutexReceive.unlock();
//	//_mutex->unlock();
//	return (res);
//}
//
//void	Menu::manageReco(Thread *th)
//{
//	std::chrono::high_resolution_clock::time_point      t2Conn;
//	double												duration;
//
//	t2Conn = std::chrono::high_resolution_clock::now();
//	duration = std::chrono::duration_cast<std::chrono::milliseconds>(t2Conn - _t1Conn).count();
//	if (duration >= RECO_DURATION)
//	{
//		//_mutex->lock();
//		if (_socket && !_socket->isConnected())
//		{
//			if (th)
//			{
//				_pool.joinAll();
//				_pool.removeThread(th);
//			}
//			th = new Thread();
//			th->createThread(std::bind(&Menu::tryToConnect, this));
//			_pool.addThread(th);
//		}
//		//_mutex->unlock();
//		_t1Conn = std::chrono::high_resolution_clock::now();
//	}
//}
//
//void	Menu::setRoomInfo(RoomInfoCmd *roomInfo, InsideRoomPage *page)
//{
//	std::vector<PlayerInfo>					pl;
//	std::vector<PlayerInfo>::iterator		it;
//
//	pl = roomInfo->getPlayersList();
//	page->setRoomName(roomInfo->getName());
//	it = pl.begin();
//	while (it != pl.end())
//	{
//		page->addPlayer(it->first, it->second);
//		++it;
//	}
//}
//
//void	Menu::checkGameReady()
//{
//	int				res;
//
//	while (1)
//	{
//		if (_id != -1 || !_run)
//			return;
////		std::cout << "THREAD LAUNCH" << std::endl;
//		_cmdManager.receiveCmd();
//		//if ((res = _cmdManager.launchGame()) != -1)
//		//{
//			//_mutexReceive.lock();
//		//	std::cout << "SET ID = " << _id << std::endl;
//		//	_id = res;
//		//	_mutexReceive.unlock();
//		//	return;
//		//}
//		//if (_cmdManager.getRoomInfo() != NULL)
//		//{
//		//	_mutexReceive.lock();
//		//	_roomInfo = _cmdManager.getRoomInfo();
//		//	_mutexReceive.unlock();
//		//}
//	}
//}
//
//void	Menu::startGame()
//{
//	std::cout << "Game" << std::endl;
//	_soundManager.stopAll();
//	_game.setGraph(_graph);
//	_game.setEvent(_event);
//	_game.setPort(9999);
//	_game.setId(_id);
//	_game.setIp(_ip);
//	if (_roomInfo)
//		_game.setNbPlayer(_roomInfo->getPlayersList().size());
//	_pool.joinAll();
//	_game.launch();
//
//}
//
//void	Menu::manageLaunchGame()
//{
//	_newEvent = true;
//	_cmdManager.setStatus();
//	startGame();
//}
//
//void	Menu::refreshRoomInside()
//{
//	if (_page->getPageType() == IPage::INSIDEROOM && _th)
//	{
//		_page->clear();
//		if (_roomInfo)
//			setRoomInfo(_roomInfo, (static_cast<InsideRoomPage*>(_page)));
//		_page->init();
//	}
//}
//
//void	Menu::manageEvent()
//{
//	if (_page->getPageType() == IPage::PLAY)
//	{
//		LobbyPage		*lobbyPage;
//
//		lobbyPage = static_cast<LobbyPage* >(_page);
//		if (lobbyPage->getSelectedRoom().first != -84)
//		{
//			std::cout << lobbyPage->getSelectedRoom().first << std::endl;
//			if (!_cmdManager.joinRoom(lobbyPage->getSelectedRoom().first, _playerName))
//				std::cerr << "Can not join room" << std::endl;
//			else
//			{
//				RoomInfoCmd			*roomInfo1;
//				Thread				*th2;
//
//				delete _page;
//				_newEvent = true;
//				_page = new InsideRoomPage(_graph, _event, _fileManager, &_soundManager);
//				_page->clear();
//				_page->init();
//				if (_th == NULL)
//				{
//					_th = new Thread();
//					_th->createThread(std::bind(&Menu::checkGameReady, this));
//					_pool.addThread(_th);
//				}
//			}
//		}
//	}
//}
//
//bool Menu::launch()
//{
//  IPage::PAGE									curr_event;
//  _page = new HomePage(_graph, _event, _fileManager, &_soundManager);
//  bool											_newEvent;
//  Thread										*th = NULL;
//  std::pair<std::string, std::pair<int, int> >	tmp;
//
//  _newEvent = false;
//  _page->init();
//	_th = new Thread();
//	_th->createThread(std::bind(&Menu::checkGameReady, this));
//	_pool.addThread(_th);
//  while (_graph->isWindowOpen())
//    {
//	  //manageReco(th);
////	  _mutexReceive.lock();
//	  //refreshRoomInside();
//	  //if (_id != -1)
//	  //{
//		 // delete (_page);
//		 // startGame();
//		 // _mutexReceive.unlock();
//		 // return (1);
//	  //}
//	  //_mutexReceive.unlock();
//
//	  while (_event->refresh())
//		{
//		  int			res;
//		  if (!_cmdManager.isWaiting())
//			{
//			  _newEvent = true;
//			  if ((res = _cmdManager.getLatsReply()) == -1)
//				  curr_event = _page->event();
//			  else if (res == 0)
//			  {
//				  std::cout << "NOT ERROR = " << _successEvent  << std::endl;
//				  curr_event = _successEvent;
//			  }
//			  else
//				  curr_event = _errorEvent;
//		  }
//		  else if (_page->getPageType() != IPage::LOADING)
//			  curr_event = IPage::LOADING;
//
//			  manageEvent();
//
//		 switch (curr_event)
//		    {
//		    case IPage::HOME:
//		      delete (_page);
//		      _newEvent = true;
//		      _page = new HomePage(_graph, _event, _fileManager, &_soundManager);
//		      std::cout << "Home" << std::endl;
//		      break;
//		    case IPage::PLAY:
//
//				_successEvent = IPage::GAME;
//				_errorEvent = IPage::PLAY;
//
//				if (_page->getPageType() == IPage::INSIDEROOM)
//				{
//					if (!_cmdManager.leaveRoom())
//						break;
//				}
////		      delete (_page);
//			  _newEvent = true;
//			  initLobby();
//
//		      std::cout << "Lobby" << std::endl;
//		      break;
//		    case IPage::SAVE:
//				tmp = static_cast<SettingsPage *>(_page)->save();
//				_sv.readFromFile();
//				_ip = _sv.getIport().substr(0, _sv.getIport().find(":"));
//				_port = std::stoi(_sv.getIport().substr(_sv.getIport().find(":") + 1));
//				_soundManager.setMusicVolume(_sv.getMusic());
//				_soundManager.setSoundVolume(_sv.getSfx());
//				 break;
//		    case IPage::CREATEROOM:
//		      delete (_page);
//		      _newEvent = true;
//		      _page = new CreateRoomPage(_graph, _event, _fileManager, &_soundManager);
//		      std::cout << "RoomList" << std::endl;
//		      break;
//		    case IPage::INSIDEROOM:
//				delete (_page);
//			  _newEvent = true;
//		      _page = new InsideRoomPage(_graph, _event, _fileManager, &_soundManager);
//			  std::cout << "InsideRoom" << std::endl;
//			  break;
//		    case IPage::PAUSE:
//		      delete (_page);
//		      _newEvent = true;
//		      _page = new PausePage(_graph, _event, _fileManager, &_soundManager);
//		      std::cout << "Pause" << std::endl;
//		      break;
//		    case IPage::SETTINGS:
//		      delete (_page);
//		      _newEvent = true;
//			  SettingsPage *tmpSettings;
//		      tmpSettings = new SettingsPage(_graph, _event, _fileManager, &_soundManager);
//			  tmpSettings->setServerInfo(_ip, _port);
//			  _page = tmpSettings;
//		      std::cout << "Settings" << std::endl;
//		      break;
//		    case IPage::SETTINGSNEXT:
//		      delete (_page);
//		      _newEvent = true;
//		      _page = new SettingsNextPage(_graph, _event, _fileManager, &_soundManager);
//		      std::cout << "SettingsNext" << std::endl;
//		      break;
//			case IPage::GAME:
//				std::cout << "wait launch game" << std::endl;
//
//				_successEvent = IPage::GAME;
//				_errorEvent = _page->getPageType();
//
////				_mutexReceive.lock();
//				manageLaunchGame();
////				_mutexReceive.unlock();
//				break;
//			case IPage::ENDGAME:
//				delete (_page);
//				_newEvent = true;
//				_page = new PausePage(_graph, _event, _fileManager, &_soundManager);
//				std::cout << "Settings" << std::endl;
//				break;
//			case IPage::LOADING:
//				delete (_page);
//				_newEvent = true;
//				_page = new LoadingPage(_graph, _event, _fileManager, &_soundManager);
//				std::cout << "Loading" << std::endl;
//				break;
//			case IPage::QUIT:
//				_graph->close();
//				break;
//			case IPage::CREATEROOMACTION:
//				std::cout << "CreateRoomAction" << std::endl;
//				_newEvent = true;
//				CreateRoomPage		*tmpPage;
//				tmpPage = static_cast<CreateRoomPage* >(_page);
//
//				_successEvent = IPage::INSIDEROOM;
//				_errorEvent = _page->getPageType();
//
//				if (!_cmdManager.createRoom(tmpPage->getRoomName(), _playerName))
//				{
//					std::cerr << "Waiting for create room" << std::endl;
//					break;
//				}
//				//				delete (_page);
//				break;
//			default:
//			   break;
//	    }
//
//		if (_event->getCloseEvent() || _event->getKeyStroke() == "ECHAP")
//	    _graph->close();
//	  if (_newEvent && _page)
//	    {
//	      _page->init();
////	      _graph->clearWindow();
//	      //_page->draw();
////	      _graph->refresh();
//	      _newEvent = false;
//	    }
//	}
//      _graph->clearWindow();
//      _page->draw();
//	  _graph->refresh();
//    }
//	_pool.joinAll();
//  return (false);
//}
//


void Menu::setIp(const std::string & ip)
{
	_ip = ip;
}

void Menu::setPort(const int port)
{
	_port = port;
}

