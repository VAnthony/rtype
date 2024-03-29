#include "SaveConfig.hh"

SaveConfig::SaveConfig()
{
	_playerName = "player";
	_up = "";
	_down = "";
	_right = "";
	_left = "";
	_pew = "k";
	_superPew = "l";
	_music = 50;
	_sfx = 50;
	_iport = "10.16.252.95:42000";
}

SaveConfig::SaveConfig(const std::string &playerName, const std::string &up, const std::string &down, const std::string &right, const std::string &left, const std::string &pew, const std::string &superPew, int music, const int sfx, const std::string &iport)
{
	_playerName = playerName;
	_up = up;
	_down = down;
	_right = right;
	_left = left;
	_pew = pew;
	_superPew = superPew;
	_music = music;
	_sfx = sfx;
	_iport = iport;
}

SaveConfig::~SaveConfig()
{
}

void			SaveConfig::writeToFile()
{
	std::fstream		fileStream;

	fileStream.open(".menuConfig.xml",
		std::fstream::in |
		std::fstream::out |
		std::fstream::trunc);
	if (fileStream.is_open())
	{
		fileStream << "<Menu Config>\n";

		fileStream << "<PlayerName>" << _playerName << "</PlayerName>" << std::endl;
		fileStream << "<Up>" << _up << "</Up>" << std::endl;
		fileStream << "<Down>" << _down << "</Down>" << std::endl;
		fileStream << "<Right>" << _right << "</Right>" << std::endl;
		fileStream << "<Left>" << _left << "</Left>" << std::endl;
		fileStream << "<Pew>" << _pew << "</Pew>" << std::endl;
		fileStream << "<SuperPew>" << _superPew << "</SuperPew>" << std::endl;
		fileStream << "<Music>" << _music << "</Music>" << std::endl;
		fileStream << "<SFX>" << _sfx << "</SFX>" << std::endl;
		fileStream << "<Iport>" << _iport << "</Iport>" << std::endl;

		fileStream << "</Menu Config>\n" << std::endl;
	}
	//else
	//	std::cerr << "Error: Menu File Failed to Open!" << std::endl;

	fileStream.close();
}

void				SaveConfig::ssToPlayer(const std::string &haystack)
{
	if (haystack.find("<PlayerName>") != std::string::npos && haystack.find("</PlayerName>") != std::string::npos)
	{
		std::istringstream ss(haystack.substr(haystack.find("<PlayerName>") + 12, haystack.find("</PlayerName>") - 12));
		ss >> _playerName;
	}
}

void				SaveConfig::ssToUp(const std::string &haystack)
{
	if (haystack.find("<Up>") != std::string::npos && haystack.find("</Up>") != std::string::npos)
	{
		std::istringstream ss(haystack.substr(haystack.find("<Up>") + 4, haystack.find("</Up>") - 4));
		ss >> _up;
	}
}

void				SaveConfig::ssToDown(const std::string &haystack)
{
	if (haystack.find("<Down>") != std::string::npos && haystack.find("</Down>") != std::string::npos)
	{
		std::istringstream ss(haystack.substr(haystack.find("<Down>") + 6, haystack.find("</Down>") - 6));
		ss >> _down;
	}
}

void				SaveConfig::ssToRight(const std::string &haystack)
{
	if (haystack.find("<Right>") != std::string::npos && haystack.find("</Right>") != std::string::npos)
	{
		std::istringstream ss(haystack.substr(haystack.find("<Right>") + 7, haystack.find("</Right>") - 7));
		ss >> _right;
	}
}

void				SaveConfig::ssToLeft(const std::string &haystack)
{
	if (haystack.find("<Left>") != std::string::npos && haystack.find("</Left>") != std::string::npos)
	{
		std::istringstream ss(haystack.substr(haystack.find("<Left>") + 6, haystack.find("</Left>") - 6));
		ss >> _left;
	}
}

void				SaveConfig::ssToPew(const std::string &haystack)
{
	if (haystack.find("<Pew>") != std::string::npos && haystack.find("</Pew>") != std::string::npos)
	{
		std::istringstream ss(haystack.substr(haystack.find("<Pew>") + 5, haystack.find("</Pew>") - 5));
		ss >> _pew;
	}
}

void				SaveConfig::ssToSuperPew(const std::string &haystack)
{
	if (haystack.find("<SuperPew>") != std::string::npos && haystack.find("</SuperPew>") != std::string::npos)
	{
		std::istringstream ss(haystack.substr(haystack.find("<SuperPew>") + 10, haystack.find("</SuperPew>") - 10));
		ss >> _superPew;
	}
}

void				SaveConfig::ssToMusic(const std::string &haystack)
{
	if (haystack.find("<Music>") != std::string::npos && haystack.find("</Music>") != std::string::npos)
	{
		std::istringstream ss(haystack.substr(haystack.find("<Music>") + 7, haystack.find("</Music>")));
		ss >> _music;
	}
}

void				SaveConfig::ssToSFX(const std::string &haystack)
{
	if (haystack.find("<SFX>") != std::string::npos	&& haystack.find("</SFX>") != std::string::npos)
	{
		std::istringstream ss(haystack.substr(haystack.find("<SFX>") + 5, haystack.find("</SFX>")));
		ss >> _sfx;
	}
}

void				SaveConfig::ssToIport(const std::string &haystack)
{
	if (haystack.find("<Iport>") != std::string::npos && haystack.find("</Iport>") != std::string::npos)
	{
		std::istringstream ss(haystack.substr(haystack.find("<Iport>") + 7, haystack.find("</Iport>") - 7));
		ss >> _iport;
	}
}

void				SaveConfig::needleInHaystack(const std::string &haystack)
{
	ssToPlayer(haystack);
	ssToUp(haystack);
	ssToDown(haystack);
	ssToRight(haystack);
	ssToLeft(haystack);
	ssToPew(haystack);
	ssToSuperPew(haystack);
	ssToMusic(haystack);
	ssToSFX(haystack);
	ssToIport(haystack);
}

void				SaveConfig::readFromFile()
{
	std::fstream			fileStream;

	fileStream.open(".menuConfig.xml", std::fstream::in | std::fstream::binary | std::fstream::out);
	if (fileStream.is_open())
	{
		std::string line;

		while (getline(fileStream, line))
			needleInHaystack(line);
	}
	fileStream.close();
}

std::string		SaveConfig::getPlayerName() const
{
	return (_playerName);
}

void			SaveConfig::setPlayerName(const std::string &playerName)
{
	std::string pn;
	unsigned int i = 0;

	while (i < playerName.size() && i < 11)
	{
		if (playerName[i] != '|')
			pn += playerName[i];
		i++;
	}
	if (pn.size() == 0)
		pn = "player";
	_playerName = pn;
}

std::string		SaveConfig::getUp() const
{
	return (_up);
}

void			SaveConfig::setUp(const std::string &up)
{
	if (up == "|")
		_up = "z";
	else
		_up = up;
}

std::string		SaveConfig::getDown() const
{
	return (_down);
}

void			SaveConfig::setDown(const std::string &down)
{
	if (down == "|")
		_down = "s";
	else
		_down = down;
}

std::string		SaveConfig::getRight() const
{
	return (_right);
}

void			SaveConfig::setRight(const std::string &right)
{
	if (right == "|")
		_right = "d";
	else
		_right = right;
}

std::string		SaveConfig::getLeft() const
{
	return (_left);
}

void			SaveConfig::setLeft(const std::string &left)
{
	if (left == "|")
		_left = "q";
	else
		_left = left;
}

std::string		SaveConfig::getPew() const
{
	return (_pew);
}

void			SaveConfig::setPew(const std::string &pew)
{
	if (pew == "|" || pew.size() == 0)
		_pew = "k";
	else
		_pew = pew;
}

std::string		SaveConfig::getSuperPew() const
{
	return (_superPew);
}

void			SaveConfig::setSuperPew(const std::string &superPew)
{
	if (superPew == "|" || superPew.size() == 0)
		_superPew = "l";
	else
		_superPew = superPew;
}

int				SaveConfig::getMusic() const
{
  return (_music);
}

void			SaveConfig::setMusic(const int music)
{
	if (music < 10)
		_music = 0;
	else
	  _music = music;
}

int				SaveConfig::getSfx() const
{
  return (_sfx);
}

void			SaveConfig::setSfx(const int sfx)
{
	if (sfx < 10)
		_sfx = 0;
	else
	  _sfx = sfx;
}

std::string		SaveConfig::getIport() const
{
	return (_iport);
}

void			SaveConfig::setIport(const std::string &iport)
{
	_iport = iport;
}
