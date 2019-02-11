#include "Gui.h"
#include "GuiElement.h"

#include <SFML/Window/Window.hpp>

namespace kiwi
{
namespace gui
{

Gui::Gui()
{
	m_lookupTable[ROOT].type = Type::Empty;
}

#pragma region

bool Gui::pollEvent(sf::Window& window)
{
	sf::Event e;
	if (!window.pollEvent(e))
		return false;
	processEvent(e, window);
	return true;
}

bool Gui::waitEvent(sf::Window& window)
{
	sf::Event e;
	window.waitEvent(e);
	processEvent(e, window);
	return true;
}

bool Gui::allEvents(sf::Window& window)
{
	sf::Event e;
	while (window.pollEvent(e))
		processEvent(e, window);
	return true;
}

void Gui::processEvent(sf::Event e, sf::Window& window)
{
	switch (e.type)
	{
	case sf::Event::Closed:
		if (m_closeCallback)
			m_closeCallback(0);
		else
			window.close();
		break;
	case sf::Event::GainedFocus:
		if (m_focusCallback)
			m_focusCallback(1);
		break;
	case sf::Event::LostFocus:
		if (m_focusCallback)
			m_focusCallback(0);
		break;
	case sf::Event::KeyPressed:
		if (m_keyCallback[e.key.code])
			m_keyCallback[e.key.code](e.key.code);
		break;
	case sf::Event::MouseEntered:
		if (m_migrationCallback)
			m_migrationCallback(1);
		break;
	case sf::Event::MouseLeft:
		if (m_migrationCallback)
			m_migrationCallback(0);
		break;
	case sf::Event::MouseMoved:
		mouseMove(e.mouseMove.x, e.mouseMove.y);
		break;
	case sf::Event::MouseButtonPressed:
		mouseButtonDown(e.mouseButton.button);
		break;
	case sf::Event::MouseButtonReleased:
		mouseButtonUp(e.mouseButton.button);
		break;
	case sf::Event::Resized:
		if (m_resizeCallback)
			m_resizeCallback(e.size.width << 16 | e.size.height);
		break;
	}
}

#pragma endregion Events

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

sf::Transformable& Gui::transformSprite(Key name)
{
	return directElementAccess(name).sprite;
}

sf::Transformable& Gui::transformText(Key name)
{
	return directElementAccess(name).text;
}

#pragma endregion Transforms

#pragma region

void Gui::insertElement(Key name, Key parent, Type type)
{
	if (m_lookupTable.count(name))
		throw std::invalid_argument(std::string("\"") + std::string(name) + "\" already in use.");
	
	directElementAccess(parent).children.push_back(name);

	m_lookupTable[name].type = type;
	m_lookupTable[name].parent = parent;
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

void Gui::setType(Key name, Type type)
{
	directElementAccess(name).type = type;
}

void Gui::setTexture(Key name, const sf::Texture& txt, sf::IntRect baseRect)
{
	GuiElement& e = directElementAccess(name);
	e.sprite.setTexture(txt);
	e.baseRect = baseRect;
}

void Gui::setTexture(Key name, Key txt, sf::IntRect baseRect)
{
	setTexture(name, getTexture(txt), baseRect);
}

void Gui::setAnimation(Key name, Duration period, int length)
{
	GuiElement& e = directElementAccess(name);
	if(e.type != Type::Cursor)
		e.type = Type::Static;
	e.period = period;
	e.advanceFrame = Clock::now() + period;
	e.animationLength = length;
}

void Gui::setAnimation(Key name, float period, int length)
{
	setAnimation(name, std::chrono::milliseconds(static_cast<int>(1000 * period + 0.5)), length);
}

void Gui::setCallback(Key name, Callback cb)
{
	directElementAccess(name).cb = cb;
}

void Gui::resetCallback(Key name)
{
	directElementAccess(name).cb = nullptr;
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

GuiElement& Gui::directElementAccess(Key name)
{
	auto itt = m_lookupTable.find(name);
	if (itt == m_lookupTable.end())
		throw std::range_error(std::string("\"") + std::string(name) + "\" does not name a Texture.");
	return itt->second;
}

GuiElementBase& Gui::getBase(Key name)
{
	return directElementAccess(name);
}

Type Gui::getType(Key name)
{
	return directElementAccess(name).type;
}

#pragma endregion Element Access

#pragma region

void Gui::setKeyCallback(sf::Keyboard::Key key, Callback cb)
{
	m_keyCallback[key] = cb;
}

void Gui::resetKeyCallback(sf::Keyboard::Key key)
{
	m_keyCallback[key] = nullptr;
}

void Gui::setCloseCallback(Callback cb)
{
	m_closeCallback = cb;
}

void Gui::resetCloseCallback()
{
	m_closeCallback = nullptr;
}

void Gui::setFocusCallback(Callback cb)
{
	m_focusCallback = cb;
}

void Gui::resetFocusCallback()
{
	m_focusCallback = nullptr;
}

void Gui::setMigrationCallback(Callback cb)
{
	m_migrationCallback = cb;
}

void Gui::resetMigrationCallback()
{
	m_migrationCallback = nullptr;
}

void Gui::setResizeCallback(Callback cb)
{
	m_resizeCallback = cb;
}

void Gui::resetResizeCallback()
{
	m_resizeCallback = nullptr;
}

#pragma endregion Keyboard & System

#pragma region
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
			element.cb(element.getState());
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
			element.cb(element.getState());
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
		element.cb(element.getState());
	return output & EVENT_CONSUMED;
}
#pragma endregion Implementation

}
}