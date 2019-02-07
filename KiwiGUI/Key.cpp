#include "Key.h"

#include <stdexcept>

namespace kiwi
{
namespace gui
{

Key::Key()
{
	clear();
}

Key::Key(const char* cstring)
{
	for (int i = 0; i < KEY_LENGTH; i++)
		m_data[i] = cstring[i];
}

Key::Key(int value)
{
	int powers = 1;
	int i;
	while (value > powers)
		powers *= 10;
	for (i = 0; (i < KEY_LENGTH) && (powers /= 10); i++)
	{
		m_data[i] = '0' + (value / powers);
		value %= powers;
	}
	if (i < KEY_LENGTH)
		m_data[i] = 0;
}

Key::operator std::string() const
{
	std::string retval;
	int length = size();
	retval.resize(length);
	for (int i = 0; i < length; i++)
		retval[i] = m_data[i];
	return retval;
}

Key& Key::operator=(Key src)
{
	for (int i = 0; i < KEY_LENGTH; i++)
	{
		m_data[i] = src.m_data[i];
	}

	return *this;
}

Key& Key::operator+=(Key src)
{
	int j = 0;
	for (int i = size(); i < KEY_LENGTH; i++)
	{
		m_data[i] = src.m_data[j];
		j++;
	}

	return *this;
}

char& Key::operator[](int position)
{
	if (position < 0 || position >= KEY_LENGTH)
		throw std::range_error(std::string("Index out of key range: ") + std::to_string(position));
	return m_data[position];
}

const char& Key::operator[](int position) const
{
	if (position < 0 || position >= KEY_LENGTH)
		throw std::range_error(std::string("Index out of key range: ") + std::to_string(position));
	return m_data[position];
}

int Key::size() const
{
	int i = 0;

	while (i < KEY_LENGTH && m_data[i])
		i++;

	return i;
}

int Key::toInt() const
{
	int length = size();
	int retval = 0;

	for (int i = 0; i < length; i++)
	{
		if (std::isdigit(m_data[i]))
			retval = retval * 10 + m_data[i] - '0';
	}

	return retval;
}

void Key::clear()
{
	for (int i = 0; i < KEY_LENGTH; i++)
		m_data[i] = 0;
}

Key operator+(Key lhs, Key rhs)
{
	lhs += rhs;
	return lhs;
}

bool operator==(Key lhs, Key rhs)
{
	for (int i = 0; i < KEY_LENGTH && (lhs[i] || rhs[i]); i++)
	{
		if (lhs[i] != rhs[i])
			return false;
	}
	return true;
}

bool operator!=(Key lhs, Key rhs)
{
	return !(lhs == rhs);
}

bool operator<(Key lhs, Key rhs)
{
	for (int i = 0; i < KEY_LENGTH && (lhs[i] || rhs[i]); i++)
	{
		if (lhs[i] < rhs[i])
			return true;
		else if (lhs[i] > rhs[i])
			return false;
	}
	return false;
}

bool operator>(Key lhs, Key rhs)
{
	return rhs < lhs;
}

bool operator<=(Key lhs, Key rhs)
{
	return !(rhs < lhs);
}

bool operator>=(Key lhs, Key rhs)
{
	return !(lhs < rhs);
}

}
}