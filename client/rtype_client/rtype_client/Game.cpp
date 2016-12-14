#include "Game.hh"



Game::Game()
{
	_size.first = 0;
	_size.second = 0;
	_dificulty = 0;
}


Game::~Game()
{
	std::vector<AGUIElement* >::iterator		it;

	it = _guiElement.begin();
	while (it != _guiElement.end())
	{
		delete (*it);
		++it;
	}
	_guiElement.clear();
}

void Game::createUI()
{
	RectDecor *topUI = new RectDecor(_graph, _event, Rect(0, 0, 100, _size.first));
	RectDecor	*bottomUI = new RectDecor(_graph, _event, Rect(0, _size.second - 150, 150, _size.first));
	RectDecor	*lifeBar = new RectDecor(_graph, _event, Rect(80, _size.second - 130, 50, 150));
	RectDecor	*powerBar = new RectDecor(_graph, _event, Rect(80, _size.second - 75, 50, 150));
	RectDecor	*lifeIcon = new RectDecor(_graph, _event, Rect(20, _size.second - 130, 50, 50));
	RectDecor	*powerIcon = new RectDecor(_graph, _event, Rect(20, _size.second - 75, 50, 50));
	RectDecor	*dificulty = new RectDecor(_graph, _event, Rect(_size.first - 250, _size.second - 130, 100, 200));

	topUI->setBackgroundSprite("../../res/img/barreJeu.png");
	bottomUI->setBackgroundSprite("../../res/img/barreJeu.png");
	lifeBar->setBackgroundSprite("../../res/img/barreVie4.png");
	powerBar->setBackgroundSprite("../../res/img/barreDefense4.png");
	powerIcon->setBackgroundSprite("../../res/img/iconeDefense.png");
	lifeIcon->setBackgroundSprite("../../res/img/iconeVie.png");
	dificulty->setBackgroundSprite("../../res/img/fondCadre.png");
	_guiElement.push_back(topUI);
	_guiElement.push_back(bottomUI);
	_guiElement.push_back(lifeBar);
	_guiElement.push_back(powerBar);
	_guiElement.push_back(powerIcon);
	_guiElement.push_back(lifeIcon);
	_guiElement.push_back(dificulty);
}

#include <iostream>

void Game::drawUi()
{
	std::vector<AGUIElement* >::iterator		it;

	it = _guiElement.begin();
	while (it != _guiElement.end())
	{
		(*it)->draw();
		++it;
	}
	_graph->drawText("HEN TYPE", _size.first / 2 - 220, 0, 80, Color(135, 206, 250, 255), "../../res/fonts/Aerospace.ttf");
	_graph->drawText("Nb joueur", _size.first - 190, _size.second - 110, 15, Color(135, 206, 250, 255), "../../res/fonts/Aerospace.ttf");
	_graph->drawText("DIFFICULTE", _size.first - 190, _size.second - 70, 15, Color(135, 206, 250, 255), "../../res/fonts/Aerospace.ttf");
}

int Game::launch()
{
	std::chrono::high_resolution_clock::time_point      t1;
	std::chrono::high_resolution_clock::time_point	    t2;
	double												duration;
	int													i;
	bool												first = true;

	i = 100;
	_graph->setFullScreen(true);
	_size = _graph->getWindowSize();
	t1 = std::chrono::high_resolution_clock::now();
	createUI();
	while (_graph->isWindowOpen())
	{
		while (_event->refresh())
		{
			//if ((ret = buttonEvent()) == 1)
			//	return (true);
			//else if (ret == 2)
			//	return (false);
			if (_event->getCloseEvent() || _event->getKeyStroke() == "ECHAP")
				return (1);
		}
		_graph->clearWindow();
		t2 = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		_graph->setBackground("../../res/img/stars_background.jpg", -1, -1);
		if (first || duration >= 10000)
		{
			i += 10;
			if (i >= 1000)
				i = 0;
			first = false;
			t1 = std::chrono::high_resolution_clock::now();
		}
		_graph->drawRectangle(Color(255, 255, 255), Rect(i, 300, 50, 50));
		drawUi();
		_graph->refresh();
	}
	return (0);
}

void Game::setGraph(IGraphManager * graph)
{
	_graph = graph;
}

void Game::setEvent(IEventManager * event)
{
	_event = event;
}