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
//Also contains GUI.
class Application;

//Interface for interacting with GUIElements.
class Gui;

//Individual elements in the GUI.
//This type is not visible to the end user.
struct GuiElement;

using Message = std::vector<int>;
using Callback = std::function<void(const Key&, int)>;

}
}