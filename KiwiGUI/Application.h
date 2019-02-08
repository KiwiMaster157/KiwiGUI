#pragma once

#include "Classes.h"
#include "Key.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>

#include <array>

namespace kiwi
{
namespace gui
{

struct Application
{
public:

	bool pollAllEvents(sf::Window& win, Gui& gui);

	bool pollEvent(sf::Window& win, Gui& gui);

	std::array<Callback, sf::Keyboard::KeyCount> keyCallbacks;
	Callback closeCallback;
	Callback focusCallback; //1 = gain, 0 = loss
	Callback resizeCallback;
};

}
}