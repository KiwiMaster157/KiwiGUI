#include "Gui.h"
#include "GuiElement.h"

namespace kiwi
{
namespace gui
{

#pragma region

void Gui::openTexture(Key name, const std::string& file)
{
	if (!m_textures[name].loadFromFile(file))
		throw std::invalid_argument(std::string("\"") + file + "\" is not a Texture file.");
}

void Gui::openFont(Key name, const std::string& file)
{
	if(!m_fonts[name].loadFromFile(file))
		throw std::invalid_argument(std::string("\"") + file + "\" is not a Font file.");
}

const sf::Texture& Gui::getTexture(Key name)
{
	auto itt = m_textures.find(name);
	if(itt == m_textures.end())
		throw std::range_error(std::string("\"") + std::string(name) + "\" does not name a Texture.");
	return itt->second;
}

const sf::Font& Gui::getFont(Key name)
{
	auto itt = m_fonts.find(name);
	if (itt == m_fonts.end())
		throw std::range_error(std::string("\"") + std::string(name) + "\" does not name a Font." );
	return itt->second;
}

void Gui::closeTexture(Key name)
{
	auto itt = m_textures.find(name);
	if (itt == m_textures.end())
		throw std::range_error(std::string("\"") + std::string(name) + "\" does not name a Texture.");
	m_textures.erase(name);
}

void Gui::closeFont(Key name)
{
	auto itt = m_fonts.find(name);
	if (itt == m_fonts.end())
		throw std::range_error(std::string("\"") + std::string(name) + "\" does not name a Font.");
	m_textures.erase(name);
}

void Gui::closeAllTextures()
{
	m_textures.clear();
}

void Gui::closeAllFonts()
{
	m_fonts.clear();
}

#pragma endregion Libraries

#pragma region

void Gui::setPosition(Key name, float x, float y)
{
	directElementAccess(name).offset = sf::Vector2f(x, y);
}

void Gui::setPosition(Key name, const sf::Vector2f& position)
{
	directElementAccess(name).offset = position;
}

void Gui::move(Key name, float x, float y)
{
	directElementAccess(name).offset += sf::Vector2f(x, y);
}

void Gui::move(Key name, const sf::Vector2f& offset)
{
	directElementAccess(name).offset += offset;
}

sf::Transformable& Gui::spriteTransform(Key name)
{
	return directElementAccess(name).sprite;
}

sf::Transformable& Gui::textTransform(Key name)
{
	return directElementAccess(name).text;
}

#pragma endregion Transforms

void Gui::mouseMove(int x, int y)
{
	mouseMoveHelper(m_root, sf::Vector2f(
		static_cast<float>(x), static_cast<float>(y)));
}

void Gui::mouseButtonDown(sf::Mouse::Button btn)
{
	mouseButtonDownHelper(m_root, btn);
}

void Gui::mouseButtonUp(sf::Mouse::Button btn)
{
	mouseButtonUpHelper(m_root, btn);
}

void Gui::draw(sf::RenderTarget& target)
{
	draw(target, m_root);
}

void Gui::draw(sf::RenderTarget& target, Key root)
{
	drawHelper(target, root, sf::Vector2f(0, 0));
	m_root = root;
}

GuiElement& Gui::directElementAccess(const Key& name)
{
	auto itt = m_lookupTable.find(name);
	if (itt == m_lookupTable.end())
		throw std::range_error(std::string("\"") + std::string(name) + "\" does not name a Texture.");
	return itt->second;
}

void Gui::drawHelper(sf::RenderTarget& target, Key name, sf::Vector2f offset)
{
	GuiElement& element = directElementAccess(name);
	if (!element.isActive())
		return;

	element.draw(target, getOffsetTransform(offset + element.offset));

	for (auto itt = element.children.rbegin(); itt != element.children.rend(); itt++)
	{
		drawHelper(target, *itt, offset + element.offset);
	}
}

bool Gui::mouseMoveHelper(Key name, sf::Vector2f point)
{
	GuiElement& element = directElementAccess(name);
	if (!element.isActive())
		return false;

	for (Key child : element.children)
	{
		if (mouseMoveHelper(child, point - element.offset))
			return true;
	}

	Flags output = element.mouseMove(point - element.offset);
	if (output & EVENT_CALLBACK)
		element.cb(name, element.getState());
	return output & EVENT_CONSUMED;
}

bool Gui::mouseButtonDownHelper(Key name, sf::Mouse::Button btn)
{
	GuiElement& element = directElementAccess(name);
	if (!element.isActive())
		return false;

	for (Key child : element.children)
	{
		if (mouseButtonDownHelper(child, btn))
			return true;
	}

	Flags output = element.mouseButtonDown(btn);
	if (output& EVENT_CALLBACK)
		element.cb(name, element.getState);
	return output & EVENT_CONSUMED;
}

bool Gui::mouseButtonUpHelper(Key name, sf::Mouse::Button btn)
{
	GuiElement& element = directElementAccess(name);
	if (!element.isActive())
		return false;

	for (Key child : element.children)
	{
		if (mouseButtonUpHelper(child, btn))
			return true;
	}

	Flags output = element.mouseButtonUp(btn);
	if (output& EVENT_CALLBACK)
		element.cb(name, element.getState);
	return output & EVENT_CONSUMED;
}

}
}