#include "Application.h"
#include "Gui.h"

#include <SFML/Window/Event.hpp>

namespace kiwi
{
namespace gui
{

bool Application::pollAllEvents(sf::Window& win, Gui& gui)
{
	bool retval = false;
	sf::Event e;
	while (win.pollEvent(e))
	{
		processEvent(e, win, gui);
		retval = true;
	}
	return retval;
}

void Application::processEvent(sf::Event e, sf::Window& win, Gui& gui)
{
	switch (e.type)
	{
	case sf::Event::Closed:
		if(closeCallback)
			closeCallback("CLOSE", 0);
		break;
	case sf::Event::GainedFocus:
		if(focusCallback)
			focusCallback("FOCUS", 1);
		break;
	case sf::Event::LostFocus:
		if(focusCallback)
			focusCallback("FOCUS", 0);
		break;
	case sf::Event::KeyPressed:
		if(keyCallbacks[e.key.code])
			keyCallbacks[e.key.code]("KEYBOARD", e.key.code);
		break;
	case sf::Event::MouseMoved:
		gui.mouseMove(e.mouseMove.x, e.mouseMove.y);
		break;
	case sf::Event::MouseButtonPressed:
		gui.mouseButtonDown(e.mouseButton.button);
		break;
	case sf::Event::MouseButtonReleased:
		gui.mouseButtonUp(e.mouseButton.button);
		break;
	case sf::Event::Resized:
		if(resizeCallback)
			resizeCallback("RESIZE", e.size.width << 16 | e.size.height);
		break;
	}
}

}
}