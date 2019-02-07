#pragma once

#include "Classes.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Mouse.hpp>

#include <unordered_map>

namespace kiwi
{
namespace gui
{

sf::Transform getOffsetTransform(sf::Vector2f offset);

struct GuiElement
{
#pragma region
#pragma region

	Flags mouseMove(sf::Vector2f point);
	Flags mouseButtonDown(sf::Mouse::Button btn);
	Flags mouseButtonUp(sf::Mouse::Button btn);

	void draw(sf::RenderTarget& target, sf::Transform offset);
	int getState(); //Return value is passed to callback when called

	void updateTextureRect();

	bool isActive() const;
	void setActive(bool set);

#pragma endregion Basic UI

#pragma region
	bool contains(sf::Vector2f point);
	
	bool isHovered() const;
	bool isClicked() const;
	bool isClicked(sf::Mouse::Button btn) const;
	bool isSelected() const;
	
	void setHovered(bool set);
	void setClicked(bool set, sf::Mouse::Button btn);
	void setSelected(bool set);
#pragma endregion Buttons
#pragma endregion Methods

#pragma region
#pragma region

	//Beginning = Front of screen
	//Ending = Back of screen
	std::vector<Key> children;

	enum class Type
	{
		Empty,		//Does nothing. Exists for organizational purposes.
		Static,		//Visual only. Can be animated.
		Button,		//Calls cb when clicked (press and release)
		FINAL		//Placeholder for end of list (not a valid type)
	} type = Type::Empty;

	sf::Text text;
	sf::Sprite sprite;

	sf::IntRect baseRect;

	sf::Vector2f offset = sf::Vector2f(0.0f, 0.0f);
#pragma endregion Basic UI

#pragma region
	TimePoint advanceFrame = TimePoint::max();
	Duration period = Duration::max();
	int currentFrame = 0;
	int animationLength = 0; //Negative = no repeat
#pragma endregion Animation

#pragma region
	Flags buttonFlags = 0;
	Callback cb;
	int buttonState = 0;
#pragma endregion Button
#pragma endregion Fields
};

//===== Button State Flags =====

//Masks for flags related to how the mouse has interacted with a button
const Flags BUTTON_HOVER_MASK = 1;
const Flags BUTTON_LMB_MASK = 2;
const Flags BUTTON_MMB_MASK = 4;
const Flags BUTTON_RMB_MASK = 8;
const Flags BUTTON_SELECT_MASK = 16;
const Flags BUTTON_ANY_MASK = BUTTON_LMB_MASK | BUTTON_MMB_MASK | BUTTON_RMB_MASK;

//When flag is false, element and children do nothing
const Flags BUTTON_ACTIVE_MASK = 32;

//===== Event Return Flags =====
const Flags EVENT_NORMAL = 0;
const Flags EVENT_CONSUMED = 1;
const Flags EVENT_CALLBACK = 2;

}
}