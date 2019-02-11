#pragma once

#include "Classes.h"
#include "GuiElement.h"
#include "Key.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include <array>
#include <map>
#include <string>
#include <unordered_map>

namespace kiwi
{
namespace gui
{

const Key ROOT = "ROOT";

class Gui
{
public:
	Gui();

#pragma region

	bool pollEvent(sf::Window& window);
	bool waitEvent(sf::Window& window);
	
	bool allEvents(sf::Window& window);

	void processEvent(sf::Event e, sf::Window& window);

#pragma endregion Events

#pragma region

	//Throw std::invalid_argument on failure
	void openTexture(Key name, const std::string& file);
	void openFont(Key name, const std::string& file);

	//Throw std::range_error on failure
	const sf::Texture& getTexture(Key name);
	const sf::Font& getFont(Key name);

	//Throw std::range_error on failure
	void closeTexture(Key name);
	void closeFont(Key name);

	void closeAllTextures();
	void closeAllFonts();

#pragma endregion Libraries

#pragma region
	void setPosition(Key name, float x, float y);
	void setPosition(Key name, const sf::Vector2f& position);

	void move(Key name, float x, float y);
	void move(Key name, const sf::Vector2f& offset);

	sf::Transformable& transformSprite(Key name);
	sf::Transformable& transformText(Key name);

#pragma endregion Transforms

#pragma region

	
	//Throws std::invalid_argument if name already in use
	//Throws std::range_error if parent could not be found
	void insertElement(Key name, Key parent, Type type);

	//Removes element and children recursively
	void removeElement(Key name);

	void transferOwnership(Key name, Key toOwner);

	void setType(Key name, Type type);

	//Helper mutators
	//Throw std::range_error if element not found
	//Optionally can use braced list to create rectangle
	void setTexture(Key name, const sf::Texture& txt, sf::IntRect baseRect);
	
	void setTexture(Key name, Key txt, sf::IntRect baseRect);
	
	//Negative length = no loop
	//If type != Cursor, type = Static
	void setAnimation(Key name, Duration period, int length);
	void setAnimation(Key name, float periodSeconds, int length);

	void setCallback(Key name, Callback cb);
	void resetCallback(Key name);

	//Negative amounts result in undefined behavior
	//Moving an item out of array bounds = UB
	void moveForward(Key name, int amount = 1);
	void moveBackward(Key name, int amount = 1);

	void moveToFront(Key name);
	void moveToBack(Key name);

	void mouseMove(int x, int y);
	void mouseButtonDown(sf::Mouse::Button btn);
	void mouseButtonUp(sf::Mouse::Button btn);

	void draw(sf::RenderTarget& target);

	//Throw std::range_error on failure
	GuiElement& directElementAccess(Key name);
	GuiElementBase& getBase(Key name);
	Type getType(Key name);

#pragma endregion Element Acccess

#pragma region

	//Called on key press event
	void setKeyCallback(sf::Keyboard::Key key, Callback cb);
	void resetKeyCallback(sf::Keyboard::Key key);

	//Called on window close event
	void setCloseCallback(Callback cb);
	void resetCloseCallback();

	//Called when focus is gained or lost.
	//0 = lost; 1 = gained.
	void setFocusCallback(Callback cb);
	void resetFocusCallback();

	//Called when mouse enters or leaves window
	//0 = left, 1 = entered
	void setMigrationCallback(Callback cb);
	void resetMigrationCallback();

	//Called when window is resized.
	//(newX << 16) | newY
	void setResizeCallback(Callback cb);
	void resetResizeCallback();

#pragma endregion Keyboard & System

private:
	std::unordered_map<Key, GuiElement> m_lookupTable;

	std::map<Key, sf::Texture> m_textures;
	std::map<Key, sf::Font> m_fonts;

	std::array<Callback, sf::Keyboard::KeyCount> m_keyCallback;
	Callback m_closeCallback;
	Callback m_focusCallback; //1 = gain, 0 = loss
	Callback m_migrationCallback; //1 = enter, 0 = leave
	Callback m_resizeCallback;

	//Returns true if event is consumed
	void drawHelper(sf::RenderTarget& target, Key name, sf::Vector2f offset);
	//'point' takes into account offfsets accumulated up to parent of target element
	bool mouseMoveHelper(Key name, sf::Vector2f point);
	bool mouseButtonDownHelper(Key name, sf::Mouse::Button btn);
	bool mouseButtonUpHelper(Key name, sf::Mouse::Button btn);

};

}
}