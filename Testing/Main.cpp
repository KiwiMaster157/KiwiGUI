#include "../KiwiGui.h"
#include <iostream>

int main(int argc, char** argv)
{
	sf::RenderWindow window;
	kiwi::gui::Application app;
	kiwi::gui::Gui gui;

	//Initialize Logic

	gui.openTexture("txtr0", "Trolls.png");

	kiwi::gui::GuiElement& btn = gui.insertElement("Btn0", kiwi::gui::ROOT, kiwi::gui::GuiElement::Type::Card);
	btn.sprite.setTexture(gui.getTexture("txtr0"));
	btn.baseRect = sf::IntRect(0, 214, 153, 214);
	btn.updateTextureRect();
	btn.sprite.setPosition(200, 200);
	btn.cb = [](kiwi::gui::Key str, int num) { std::cout << "Btn0" << std::endl; };
	btn.setSensitive(true, sf::Mouse::Left);

	kiwi::gui::GuiElement& btn2 = gui.insertElement("Btn1", "Btn0", kiwi::gui::GuiElement::Type::Static);
	btn2.sprite.setTexture(gui.getTexture("txtr0"));
	btn2.baseRect = sf::IntRect(0, 0, 153, 214);
	btn2.updateTextureRect();
	btn2.advanceFrame = kiwi::gui::Clock::now();
	btn2.period = std::chrono::milliseconds(1000);
	btn2.animationLength = 4;


	app.closeCallback = [&](kiwi::gui::Key str, int num) { window.close(); };
	app.keyCallbacks[sf::Keyboard::Space] = [](kiwi::gui::Key str, int num) { std::cout << "Space" << std::endl; };
	app.keyCallbacks[sf::Keyboard::W] = [&](kiwi::gui::Key str, int num) { gui.move("Btn0", 0, 10); };


	window.create(sf::VideoMode(720, 640), "Test window");

	//Main loop
	while (window.isOpen())
	{
		app.pollAllEvents(window, gui);

		window.clear();

		gui.draw(window);
		
		window.display();
	}

	return 0;
}