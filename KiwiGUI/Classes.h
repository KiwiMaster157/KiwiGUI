#pragma once

#include <chrono>
#include <functional>
#include <string>
#include <vector>

namespace kiwi
{
namespace gui
{

using Flags = std::uint16_t;

using Clock = std::chrono::steady_clock;
using TimePoint = Clock::time_point;
using Duration = Clock::duration;

//String-like wrapper used for identifying GuiElements.
//Pass by value;
class Key;

//Responsible for polling for raw events and calling listeners.
struct Application;

//Interface for interacting with GUIElements.
class Gui;

//Individual elements in the GUI.
//This type is not visible to the end user.
struct GuiElement;

enum class Type
{
	Empty,			//Does nothing. Exists for organizational purposes.
	Static,			//Visual only. Can be animated.
	Button,			//Calls cb when clicked (press and release)
	ToggleButton,	//Calls cb and swaps state when when clicked
	Card,			//Can be dragged with mouse
	AntiCard,		//Can be dragged with mouse, but moves opposite direction
	Cursor,			//Moves with mouse, regardless of mouse clicks
	FINAL			//Placeholder for end of list (not a valid type)
};

using Callback = std::function<void(int)>;

}
}