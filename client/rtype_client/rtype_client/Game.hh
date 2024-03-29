#pragma once

#include <chrono>
#include <ctime>
#include <vector>
#include "IGraphManager.hpp"
#include "IEventManager.hpp"
#include "CmdManager.hh"
#include "AGUIElement.hh"
#include "RectDecor.hh"
#include "Sound.hh"
#include "SFMLSound.hh"
#include "InputCmd.hh"
#include "Serialize.hh"
#include "SocketClientUDP.hh"
#include "APage.hh"
#include "GUIPage.hh"
#include "PausePage.hh"
#include "ASocketClient.hh"
#include "ThreadPool.hh"
#include "Thread.hh"
#include "Mutex.hh"
#include "SaveConfig.hh"
#include "EndGamePage.hh"
#include "EntityType.hh"
#include "Player.hh"

#define NB_CELL_X (150)
#define NB_CELL_Y (80)
#define GAME_LOOP_DURATION (1)
#define RECEIVE_DURATION (1)
#define INPUT_DURATION (100)

class Game
{
public:
	Game();
	~Game();
	int launch();
	void setGraph(IGraphManager * graph);
	void setEvent(IEventManager * event);
	void setIp(const std::string &ip);
	void setPort(const int port);
	void setId(const int id);
	void setNbPlayer(const int nb);
	void setTCPSocket(ASocketClient * socket);

private:
	IGraphManager				*_graph;
	IEventManager				*_event;
	CmdManager					_cmdManager;
	std::vector<IGUIElement* >	_guiElement;
	int							_dificulty;
	std::pair<int, int>			_size;
	SFMLSound					_soundManager;
	Sound						_musicStage1;
	ICommand					*_input;
	Serialize					_serialize;
	ASocketClient				*_sock;
	APage						*_guiPage;
	APage						*_pausePage;
	PathFileManager				_fileManager;
	std::string					_ip;
	int							_port;
	int							_id;
	std::string					_playerName;
	int							_nbPlayer;
	std::string					_mode;
	ThreadPool					_pool;
	std::vector<IEntity* >		_entity;
	std::vector<char>			_refreshed;
	Mutex						_mutex;
	Mutex						_mutexRun;
	bool						_run;
	bool						_newEvent;
	std::pair<int, int>			_windowSize;
	std::pair<int, int>			_windowGameSize;
	std::string					_key;
	Sound						_pew;
	int							_bgX;
	ASocketClient				*_tcpSocket;
	IPage::PAGE					_curr_event;
	EndGameCmd					*_endGame;
	int							_score;
	int							_curr_wave;
	Mutex						_mutexPlayer;
	int							_hp;

	bool initSocket();
	void initGraphElements();
	void setPlsyerInfo(IEntity * entity);
	void manageEntity();
	void clearEntity();
	void updateGUI();
	void manageQuit();
	void initEndPage();
	void updateEntities(IEntity *);
};

