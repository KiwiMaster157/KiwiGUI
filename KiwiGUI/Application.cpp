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
	while (pollEvent(win, gui))
		retval = true;
	return retval;
}

bool Application::pollEvent(sf::Window& win, Gui& gui)
{
	sf::Event e;

	if (!win.pollEvent(e))
		return false;

	switch (e.type)
	{
	case sf::Event::Closed:
		closeCallback("CLOSE", 0);
		break;
	case sf::Event::GainedFocus:
		focusCallback("FOCUS", 1);
		break;
	case sf::Event::LostFocus:
		focusCallback("FOCUS", 0);
		break;
	case sf::Event::KeyPressed:
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
		resizeCallback("RESIZE", e.size.width << 16 | e.size.height);
		break;
	}

	return true;
}

}
}