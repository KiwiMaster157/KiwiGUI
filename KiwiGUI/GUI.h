#pragma once

#include "Classes.h"
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

class Gui
{
public:
	friend struct GuiElement;

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

	sf::Transformable& spriteTransform(Key name);
	sf::Transformable& textTransform(Key name);

#pragma endregion Transforms

#pragma region

	/*
	//Throws std::invalid_argument if name already in use
	void insertElement(Key name, GuiElement::Type type);

	//Removes element and children recursively
	void removeElement(Key name);

	//
	void transferOwnership(Key transfer, Key fromOwner, Key toOwner);

	void moveForward(Key name, Key parent, int amount = 1);
	void moveBack(Key name, Key parent, int amount);

	void moveToFront(Key name, Key parent);
	void moveToBack(Key name, Key parent);

	*/

	void mouseMove(int x, int y);
	void mouseButtonDown(sf::Mouse::Button btn);
	void mouseButtonUp(sf::Mouse::Button btn);

	void draw(sf::RenderTarget& target);
	void draw(sf::RenderTarget& target, Key root);

	//Throw std::range_error on failure
	GuiElement& directElementAccess(const Key& name);

#pragma endregion Element Acccess

private:
	std::unordered_map<Key, GuiElement> m_lookupTable;
	Key m_root;

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