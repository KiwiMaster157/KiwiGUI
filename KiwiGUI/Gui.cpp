#include "Gui.h"
#include "GuiElement.h"

namespace kiwi
{
namespace gui
{

Gui::Gui()
{
	m_lookupTable[ROOT].type = GuiElement::Type::Empty;
}

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

#pragma region

GuiElement& Gui::insertElement(Key name, Key parent, GuiElement::Type type)
{
	if (m_lookupTable.count(name))
		throw std::invalid_argument(std::string("\"") + std::string(name) + "\" already in use.");
	
	directElementAccess(parent).children.push_back(name);

	m_lookupTable[name].type = type;
	m_lookupTable[name].parent = parent;

	return m_lookupTable[name];
}

void Gui::removeElement(Key name)
{
	GuiElement& element = directElementAccess(name);
	GuiElement& parent = directElementAccess(element.parent);

	//Remove from parent's records
	for (auto itt = parent.children.begin(); itt != parent.children.end(); itt++)
	{
		if (*itt == name)
			parent.children.erase(itt);
	}

	for (Key k : element.children)
		removeElement(k);

	m_lookupTable.erase(name);
}

void Gui::transferOwnership(Key name, Key toOwner)
{
	Key temp = toOwner;

	//Check for circular dependency
	while (temp != ROOT)
	{
		directElementAccess(name);
		if (temp == name)
			throw std::logic_error("Transfer of ownership would result in circular ownership.");
		temp = directElementAccess(temp).parent;
	}

	GuiElement& element = directElementAccess(name);
	GuiElement& parent = directElementAccess(element.parent);

	//Remove from parent's records
	for (auto itt = parent.children.begin(); itt != parent.children.end(); itt++)
	{
		if (*itt == name)
			parent.children.erase(itt);
	}

	//Add to new owner
	directElementAccess(toOwner).children.push_back(name);
}

void Gui::moveForward(Key name, int amount)
{
	GuiElement& element = directElementAccess(name);
	GuiElement& parent = directElementAccess(element.parent);

	int index = 0;

	for (std::size_t i = 0; i < parent.children.size(); i++)
	{
		if (parent.children[i + amount] == name)
		{
			index = i;
			break;
		}
	}

	for (int i = 0; i < amount; i++)
	{
		parent.children[i + index] = parent.children[i + index + 1];
	}
}

void Gui::moveBackward(Key name, int amount)
{
	GuiElement& element = directElementAccess(name);
	GuiElement& parent = directElementAccess(element.parent);

	int index = 0;

	for (std::size_t i = amount; i < parent.children.size(); i++)
	{
		if (parent.children[i - amount] == name)
		{
			index = i;
			break;
		}
	}

	for (int i = amount - 1; i >= 0; i--)
	{
		parent.children[i + index] = parent.children[i + index - 1];
	}
}

void Gui::moveToFront(Key name)
{
	GuiElement& element = directElementAccess(name);
	GuiElement& parent = directElementAccess(element.parent);
	
	bool found = false;

	for (int i = parent.children.size() - 1; i > 0; i--)
	{
		if (found)
		{
			parent.children[i] = parent.children[i - 1];
		}
		else if (parent.children[i - 1] == name)
		{
			found = true;
		}
	}

	parent.children.front() = name;
}

void Gui::moveToBack(Key name)
{
	GuiElement& element = directElementAccess(name);
	GuiElement& parent = directElementAccess(element.parent);

	bool found = false;

	for (std::size_t i = 0; i < parent.children.size(); i++)
	{
		if (found)
		{
			parent.children[i - 1] = parent.children[i];
		}
		else if (parent.children[i] == name)
		{
			found = true;
		}
	}

	parent.children.back() = name;
}

void Gui::mouseMove(int x, int y)
{
	mouseMoveHelper(ROOT, sf::Vector2f(
		static_cast<float>(x), static_cast<float>(y)));
}

void Gui::mouseButtonDown(sf::Mouse::Button btn)
{
	mouseButtonDownHelper(ROOT, btn);
}

void Gui::mouseButtonUp(sf::Mouse::Button btn)
{
	mouseButtonUpHelper(ROOT, btn);
}

void Gui::draw(sf::RenderTarget& target)
{
	drawHelper(target, ROOT, sf::Vector2f(0, 0));
}

GuiElement& Gui::directElementAccess(const Key& name)
{
	auto itt = m_lookupTable.find(name);
	if (itt == m_lookupTable.end())
		throw std::range_error(std::string("\"") + std::string(name) + "\" does not name a Texture.");
	return itt->second;
}

#pragma endregion Element Access

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

	Flags output = element.mouseMove(point);
	if (output & EVENT_CALLBACK)
		if(element.cb)
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
		if(element.cb)
			element.cb(name, element.getState());
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
	if (output & EVENT_CALLBACK)
		element.cb(name, element.getState());
	return output & EVENT_CONSUMED;
}

}
}