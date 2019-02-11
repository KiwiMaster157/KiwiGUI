#include "GuiElement.h"

//This document is split between GuiElement.cpp, GuiElementType.cpp

//In this file: Functions that customize behavior based on Type

namespace kiwi
{
namespace gui
{

Flags GuiElement::mouseMove(sf::Vector2f point)
{
	switch (type)
	{
	case Type::Static:
		switch (2 * contains(point - offset) + 1 * isHovered())
		{
		case 1:	//Not contained, hovered
			setHovered(false);
			return EVENT_CALLBACK;
		case 2:	//Contained, not hovered
			setHovered(true);
			return EVENT_CALLBACK;
		}
		break;
	case Type::Button:
	case Type::ToggleButton:
		setHovered(contains(point - offset));
		break;
	case Type::Card:
		if (isForced())
		{
			offset += point - previousCoords;
		}
		else
		{
			setHovered(contains(point - offset));
		}

		previousCoords = point;

		break;
	case Type::AntiCard:
		if (isForced())
		{
			offset -= point - previousCoords;
		}
		else
		{
			setHovered(contains(point - offset));
		}

		previousCoords = point;

		break;
	case Type::Cursor:
		offset = point;
		break;
	}

	return EVENT_NORMAL;
}

Flags GuiElement::mouseButtonDown(sf::Mouse::Button btn)
{
	switch (type)
	{
	case Type::Button:
	case Type::ToggleButton:
		if (isHovered())
		{
			setClicked(true, btn);
			return EVENT_CONSUMED;
		}
		break;
	case Type::Card:
	case Type::AntiCard:
		if (isHovered())
		{
			setClicked(true, btn);
		}
	}

	return EVENT_NORMAL;
}

Flags GuiElement::mouseButtonUp(sf::Mouse::Button btn)
{
	switch (type)
	{
	case Type::Button:
	case Type::Card:
	case Type::AntiCard:
		if (isClicked(btn))
		{
			setClicked(false, btn);
			if (isHovered() && isSensitive(btn))
				return EVENT_CONSUMED | EVENT_CALLBACK;
			return EVENT_CONSUMED;
		}
		else if (isHovered())
			return EVENT_CONSUMED;
		break;
	case Type::ToggleButton:
		if (isClicked(btn))
		{
			setClicked(false, btn);
			if (isHovered() && isSensitive(btn))
			{
				setSelected(!isSelected());
				return EVENT_CONSUMED | EVENT_CALLBACK;
			}
			return EVENT_CONSUMED;
		}
		else if (isHovered())
			return EVENT_CONSUMED;
		break;
	}

	return EVENT_NORMAL;
}

void GuiElement::draw(sf::RenderTarget & target, sf::Transform offset)
{
	switch (type)
	{
	case Type::Empty:
		return;
	case Type::Static:
	case Type::Cursor:

		if (Clock::now() > advanceFrame)
		{
			currentFrame++;
			advanceFrame += period;

			if (currentFrame == animationLength)
				currentFrame = 0;
			else if (currentFrame == -animationLength)
				currentFrame--;
		}

		break;
	}

	updateTextureRect();

	target.draw(sprite, offset);
	target.draw(text, offset);
}

int GuiElement::getState()
{
	switch (type)
	{
	case Type::Static:
		return isHovered();
	case Type::ToggleButton:
		return isSelected();
	}
	return 0;
}

void GuiElement::updateTextureRect()
{
	sf::Vector2i coords(0, 0);
	switch (type)
	{
	case Type::Static:
	case Type::Cursor:
		coords.x = currentFrame;
		break;
	case Type::Button:
		if (isHovered())
		{
			coords.y = 1;
			if (isForced())
				coords.y = 2;
		}
		break;
	case Type::ToggleButton:
		coords.x = isSelected();
		if (isHovered())
		{
			coords.y = 1;
			if (isForced())
				coords.y = 2;
		}
		break;
	}

	sf::IntRect newRect = baseRect;
	newRect.left += newRect.width * coords.x;
	newRect.top += newRect.height * coords.y;
	sprite.setTextureRect(newRect);
}

}
}