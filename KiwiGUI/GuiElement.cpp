#include "GuiElement.h"

//This document is split between GuiElement.cpp, GuiElementType.cpp

//In this file: functions common to all Types

namespace kiwi
{
namespace gui
{

sf::Transform getOffsetTransform(sf::Vector2f offset)
{
	return sf::Transform(1, 0, offset.x, 0, 1, offset.y, 0, 0, 1);
}

#pragma region

bool GuiElement::contains(sf::Vector2f point)
{
	return sprite.getLocalBounds().contains(
		sprite.getInverseTransform().transformPoint(point));
}

#pragma endregion Basic UI
}
}