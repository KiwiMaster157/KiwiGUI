#include "../KiwiGui.h"
#include <iostream>

int main(int argc, char** argv)
{
	sf::RenderWindow window;
	kiwi::gui::Application app;
	kiwi::gui::Gui gui;

	//Initialize Logic

	gui.openTexture("txtr0", "Trolls.png");

	gui.insertElement("Btn0", kiwi::gui::ROOT, kiwi::gui::Type::AntiCard);
	gui.setTexture("Btn0", "txtr0", { 0, 0, 153, 214 });
	gui.transformSprite("Btn0").setPosition(200, 200);
	gui.setCallback("Btn0", [&](int num) 
		{ 
			std::cout << "Btn0" << std::endl;
			//gui.moveToFront("Btn1");
		});
	gui.getBase("Btn0").setSensitive(true, sf::Mouse::Left);

	gui.insertElement("Btn1", "Btn0", kiwi::gui::Type::Cursor);
	gui.setTexture("Btn1", "txtr0", { 0, 0, 153, 214 });
	gui.setAnimation("Btn1", std::chrono::seconds(1), 4);

	app.closeCallback = [&](int num) { std::cout << "Closing"; window.close(); };
	app.keyCallbacks[sf::Keyboard::Space] = [](int num) { std::cout << "Space" << std::endl; };
	app.keyCallbacks[sf::Keyboard::W] = [&](int num) { gui.move("Btn0", 0, 10); };

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