#pragma once

#include "Classes.h"
#include "GuiElement.h"
#include "Key.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Mouse.hpp>

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

private:
	std::unordered_map<Key, GuiElement> m_lookupTable;

	std::map<Key, sf::Texture> m_textures;
	std::map<Key, sf::Font> m_fonts;

	//Returns true if event is consumed
	void drawHelper(sf::RenderTarget& target, Key name, sf::Vector2f offset);
	//'point' takes into account offfsets accumulated up to parent of target element
	bool mouseMoveHelper(Key name, sf::Vector2f point);
	bool mouseButtonDownHelper(Key name, sf::Mouse::Button btn);
	bool mouseButtonUpHelper(Key name, sf::Mouse::Button btn);


};

}
}