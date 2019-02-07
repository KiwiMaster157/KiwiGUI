#include "GuiElement.h"

namespace kiwi
{
namespace gui
{

sf::Transform getOffsetTransform(sf::Vector2f offset)
{
	return sf::Transform(1, 0, offset.x, 0, 1, offset.y, 0, 0, 1);
}

#pragma region

Flags GuiElement::mouseMove(sf::Vector2f point)
{
	switch (type)
	{
	case Type::Button:
		if (contains(point - offset) != isHovered())
		{
			setHovered(!isHovered());
			updateTextureRect();
		}
		break;
	}
	
	return EVENT_NORMAL;
}

Flags GuiElement::mouseButtonDown(sf::Mouse::Button btn)
{
	switch (type)
	{
	case Type::Button:
		if (isHovered())
		{
			setClicked(true, btn);
			updateTextureRect();
			return EVENT_CONSUMED;
		}

		break;
	}

	return EVENT_NORMAL;
}

Flags GuiElement::mouseButtonUp(sf::Mouse::Button btn)
{
	switch (type)
	{
	case Type::Button:
		if (isClicked(btn))
		{
			setClicked(false, btn);
			updateTextureRect();
			buttonState = btn;
			if (isHovered())
				return EVENT_CONSUMED | EVENT_CALLBACK;
			return EVENT_CONSUMED;
		}
		else if (isHovered())
			return EVENT_CONSUMED;

		break;
	}
	
	return EVENT_NORMAL;
}

void GuiElement::draw(sf::RenderTarget& target, sf::Transform offset)
{
	switch (type)
	{
	case Type::Static:

		if (Clock::now() > advanceFrame)
		{
			currentFrame++;
			advanceFrame += period;

			if (currentFrame == animationLength)
				currentFrame = 0;
			else if (currentFrame == -animationLength)
				currentFrame--;

			updateTextureRect();
		}

		target.draw(sprite, offset);
		target.draw(text, offset);

		break;
	case Type::Button:
		
		target.draw(sprite, offset);
		target.draw(text, offset);

		break;
	}
}

int GuiElement::getState()
{
	switch (type)
	{
	case Type::Button:
		return buttonState;
		break;
	}
	return 0;
}

void GuiElement::updateTextureRect()
{
	sf::Vector2i coords(0, 0);
	switch (type)
	{
	case Type::Static:
		coords.x = currentFrame;
		break;
	case Type::Button:
		coords.x = isSelected();
		if (isHovered())
		{
			coords.y = 1;
			if (isClicked())
				coords.y = 2;
		}
		break;
	}

	sf::IntRect newRect = baseRect;
	newRect.left += newRect.width * coords.x;
	newRect.top += newRect.height * coords.y;
	sprite.setTextureRect(newRect);
}

bool GuiElement::isActive() const
{
	return buttonFlags & BUTTON_ACTIVE_MASK;
}

void GuiElement::setActive(bool set)
{
	if (set)
		buttonFlags |= BUTTON_ACTIVE_MASK;
	else
		buttonFlags &= ~BUTTON_ACTIVE_MASK;
}

#pragma endregion Basic UI

#pragma region

bool GuiElement::contains(sf::Vector2f point)
{
	return sprite.getLocalBounds().contains(
		sprite.getInverseTransform().transformPoint(point));
}

bool GuiElement::isHovered() const
{
	return buttonFlags & BUTTON_HOVER_MASK;
}

bool GuiElement::isClicked() const
{
	return buttonFlags & BUTTON_ANY_MASK;
}

bool GuiElement::isClicked(sf::Mouse::Button btn) const
{
	switch (btn)
	{
	case sf::Mouse::Button::Left:
		return buttonFlags & BUTTON_LMB_MASK;
	case sf::Mouse::Button::Middle:
		return buttonFlags & BUTTON_MMB_MASK;
	case sf::Mouse::Button::Right:
		return buttonFlags & BUTTON_RMB_MASK;
	}
	return false;
}

bool GuiElement::isSelected() const
{
	return buttonFlags & BUTTON_SELECT_MASK;
}

void GuiElement::setHovered(bool set)
{
	if (set)
		buttonFlags |= BUTTON_HOVER_MASK;
	else
		buttonFlags &= ~BUTTON_HOVER_MASK;
}

void GuiElement::setClicked(bool set, sf::Mouse::Button btn)
{
	switch (btn)
	{
	case sf::Mouse::Button::Left:
		if (set)
			buttonFlags |= BUTTON_LMB_MASK;
		else
			buttonFlags &= ~BUTTON_LMB_MASK;
		break;
	case sf::Mouse::Button::Middle:
		if (set)
			buttonFlags |= BUTTON_MMB_MASK;
		else
			buttonFlags &= ~BUTTON_MMB_MASK;
		break;
	case sf::Mouse::Button::Right:
		if (set)
			buttonFlags |= BUTTON_RMB_MASK;
		else
			buttonFlags &= ~BUTTON_RMB_MASK;
		break;
	}
}

void GuiElement::setSelected(bool set)
{
	if (set)
		buttonFlags |= BUTTON_SELECT_MASK;
	else
		buttonFlags &= ~BUTTON_SELECT_MASK;
}

#pragma endregion Buttons
}
}