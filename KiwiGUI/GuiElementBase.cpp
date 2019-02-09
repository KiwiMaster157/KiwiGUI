#include "GuiElement.h"

namespace kiwi
{
namespace gui
{

#pragma region

bool GuiElementBase::isHovered() const
{
	return buttonFlags & BUTTON_HOVER_MASK;
}

bool GuiElementBase::isClicked() const
{
	return buttonFlags & (BUTTON_LMB_MASK | BUTTON_MMB_MASK | BUTTON_RMB_MASK);
}

bool GuiElementBase::isClicked(sf::Mouse::Button btn) const
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

bool GuiElementBase::isSelected() const
{
	return buttonFlags & BUTTON_SELECT_MASK;
}

bool GuiElementBase::isSensitive(sf::Mouse::Button btn) const
{
	switch (btn)
	{
	case sf::Mouse::Button::Left:
		return buttonFlags & BUTTON_LMB_SENSITIVE;
	case sf::Mouse::Button::Middle:
		return buttonFlags & BUTTON_MMB_SENSITIVE;
	case sf::Mouse::Button::Right:
		return buttonFlags & BUTTON_RMB_SENSITIVE;
	}
	return false;
}

bool GuiElementBase::isForced() const
{
	const Flags LMB = BUTTON_LMB_MASK | BUTTON_LMB_SENSITIVE;
	const Flags MMB = BUTTON_MMB_MASK | BUTTON_MMB_SENSITIVE;
	const Flags RMB = BUTTON_RMB_MASK | BUTTON_RMB_SENSITIVE;

	return ((buttonFlags & LMB) == LMB)
		|| ((buttonFlags & MMB) == MMB)
		|| ((buttonFlags & RMB) == RMB);
}

bool GuiElementBase::isForced(sf::Mouse::Button btn) const
{
	const Flags LMB = BUTTON_LMB_MASK | BUTTON_LMB_SENSITIVE;
	const Flags MMB = BUTTON_MMB_MASK | BUTTON_MMB_SENSITIVE;
	const Flags RMB = BUTTON_RMB_MASK | BUTTON_RMB_SENSITIVE;

	switch (btn)
	{
	case sf::Mouse::Button::Left:
		return (buttonFlags & LMB) == LMB;
	case sf::Mouse::Button::Middle:
		return (buttonFlags & MMB) == MMB;
	case sf::Mouse::Button::Right:
		return (buttonFlags & RMB) == RMB;
	}
	return false;
}

void GuiElementBase::setHovered(bool set)
{
	if (set)
		buttonFlags |= BUTTON_HOVER_MASK;
	else
		buttonFlags &= ~BUTTON_HOVER_MASK;
}

void GuiElementBase::setClicked(bool set, sf::Mouse::Button btn)
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

void GuiElementBase::setSelected(bool set)
{
	if (set)
		buttonFlags |= BUTTON_SELECT_MASK;
	else
		buttonFlags &= ~BUTTON_SELECT_MASK;
}

void GuiElementBase::setSensitive(bool set, sf::Mouse::Button btn)
{
	switch (btn)
	{
	case sf::Mouse::Button::Left:
		if (set)
			buttonFlags |= BUTTON_LMB_SENSITIVE;
		else
			buttonFlags &= ~BUTTON_LMB_SENSITIVE;
		break;
	case sf::Mouse::Button::Middle:
		if (set)
			buttonFlags |= BUTTON_MMB_SENSITIVE;
		else
			buttonFlags &= ~BUTTON_MMB_SENSITIVE;
		break;
	case sf::Mouse::Button::Right:
		if (set)
			buttonFlags |= BUTTON_RMB_SENSITIVE;
		else
			buttonFlags &= ~BUTTON_RMB_SENSITIVE;
		break;
	}
}

bool GuiElementBase::isActive() const
{
	return buttonFlags & BUTTON_ACTIVE_MASK;
}

void GuiElementBase::setActive(bool set)
{
	if (set)
		buttonFlags |= BUTTON_ACTIVE_MASK;
	else
		buttonFlags &= ~BUTTON_ACTIVE_MASK;
}

#pragma endregion Buttons

}
}