#pragma once

#include "ISoundManager.hpp"

class ASoundManager :
	public ISoundManager
{
public:
	ASoundManager();
	virtual ~ASoundManager();
	virtual bool init() = 0;
	virtual bool play(const Sound & sound) = 0;
	virtual bool stop(const Sound & sound) = 0;
	virtual bool pause(const Sound & sound) = 0;
	virtual bool setVolume(const int) = 0;
	virtual bool setMusicVolume(const int) = 0;
	virtual bool setSoundVolume(const int) = 0;
	virtual void stopAll() = 0;
};

