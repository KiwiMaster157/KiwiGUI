#pragma once

#include "Classes.h"
#include "Key.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Mouse.hpp>

#include <unordered_map>

//Implementation is split between GuiElement.cpp, GuiElementType.cpp

namespace kiwi
{
namespace gui
{


//===== Button State Flags =====

//Masks for flags related to how the mouse has interacted with a button
const Flags BUTTON_HOVER_MASK = 1;
const Flags BUTTON_LMB_MASK = 2;
const Flags BUTTON_MMB_MASK = 4;
const Flags BUTTON_RMB_MASK = 8;
const Flags BUTTON_SELECT_MASK = 16;

const Flags BUTTON_LMB_SENSITIVE = 32;
const Flags BUTTON_MMB_SENSITIVE = 64;
const Flags BUTTON_RMB_SENSITIVE = 128;

//When flag is false, element and children do nothing
const Flags BUTTON_ACTIVE_MASK = 256;

//===== Event Return Flags =====
const Flags EVENT_NORMAL = 0;
const Flags EVENT_CONSUMED = 1;
const Flags EVENT_CALLBACK = 2;

sf::Transform getOffsetTransform(sf::Vector2f offset);

struct GuiElementBase
{
#pragma region
	bool isHovered() const;
	bool isClicked() const;
	bool isClicked(sf::Mouse::Button btn) const;
	bool isSelected() const;
	bool isSensitive(sf::Mouse::Button btn) const;

	//Forced = clicked && sensitive
	bool isForced() const;
	bool isForced(sf::Mouse::Button btn) const;

	void setHovered(bool set);
	void setClicked(bool set, sf::Mouse::Button btn);
	void setSelected(bool set);
	void setSensitive(bool set, sf::Mouse::Button btn);

	bool isActive() const;
	void setActive(bool set);

#pragma endregion Buttons

	Flags buttonFlags = BUTTON_ACTIVE_MASK;
};

struct GuiElement : public GuiElementBase
{
#pragma region
#pragma region

	// Customize element types
	//Implemented in GuiElementType.cpp
	Flags mouseMove(sf::Vector2f point);
	Flags mouseButtonDown(sf::Mouse::Button btn);
	Flags mouseButtonUp(sf::Mouse::Button btn);

	void draw(sf::RenderTarget& target, sf::Transform offset);
	int getState(); //Return value is passed to callback when called
	void updateTextureRect();
	
	//Implemented in GuiElement.cpp
	bool contains(sf::Vector2f point);

#pragma endregion Basic UI
#pragma endregion Methods

#pragma region
#pragma region

	//Beginning = Front of screen
	//Ending = Back of screen
	std::vector<Key> children;
	Key parent;

	Type type = Type::Empty;

	sf::Text text;
	sf::Sprite sprite;

	sf::IntRect baseRect = sf::IntRect(0, 0, 0, 0);

	sf::Vector2f offset = sf::Vector2f(0, 0);
#pragma endregion Basic UI

#pragma region
	TimePoint advanceFrame = TimePoint::max();
	Duration period = Duration::max();
	int currentFrame = 0;
	int animationLength = 0; //Negative = no repeat
#pragma endregion Animation

#pragma region
	Callback cb;
	int buttonState = 0;
#pragma endregion Button

#pragma region
	sf::Vector2f previousCoords = sf::Vector2f(0, 0);
#pragma endregion Card
#pragma endregion Fields
};

}
}