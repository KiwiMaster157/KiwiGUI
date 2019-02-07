#pragma once

#include "Classes.h"

#include <string>

namespace kiwi
{
namespace gui
{

constexpr int KEY_LENGTH = 8;

class Key
{
public:
	Key();
	Key(const Key&) = default;
	Key(const char* cstring);
	Key(int value);//if value < 0 or value > 99,999,999 -> behavior is undefined

	explicit operator std::string() const;

	Key& operator=(Key src);
	Key& operator+=(Key src);

	//Throw std::range_error
	char& operator[](int position);
	const char& operator[](int position) const;

	int size() const;

	//Creates a positive number from the numeric characters in the Key.
	//Returns 0 if there are none.
	int toInt() const;

	void clear();

private:
	char m_data[KEY_LENGTH];
};

Key operator+(Key lhs, Key rhs);

bool operator==(Key lhs, Key rhs);
bool operator!=(Key lhs, Key rhs);
bool operator<(Key lhs, Key rhs);
bool operator>(Key lhs, Key rhs);
bool operator<=(Key lhs, Key rhs);
bool operator>=(Key lhs, Key rhs);

}
}