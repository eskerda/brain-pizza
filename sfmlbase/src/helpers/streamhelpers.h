#pragma once

#include <sstream>
#include <string>

//-----------------------------------------------------------------------------
//  sfml vector printing
//-----------------------------------------------------------------------------

#include <SFML/System/Vector2.hpp>
template <class T>
std::ostream& operator<<(std::ostream& os, sf::Vector2<T>& rhs) {
	os << rhs.x << ',' << rhs.y;
	return os;
}

//-----------------------------------------------------------------------------
//  print a matrix
//-----------------------------------------------------------------------------

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector< std::vector<T> >& t)
{
	for (unsigned int i = 0; i < t.size(); i++) {
		for (unsigned int j = 0; j < t[i].size(); j++) {
            os << t[i][j] << " ";
		}
		os << std::endl;
	}
	return os;
}
/*
//--------------------------- GetValueFromStream ------------------------------
//
//  grabs a value of the specified type from an input stream
//-----------------------------------------------------------------------------
template <typename T>
inline T GetValueFromStream(std::ifstream& stream)
{
  T val;

  stream >> val;

  //make sure it was the correct type
  if (stream.fail())
  {
    throw std::runtime_error("Attempting to retrieve wrong type from stream");
  }

  return val;
}

*/
//--------------------------- WriteBitsToStream ------------------------------------
//
// writes the value as a binary string of bits
//-----------------------------------------------------------------------------
template <typename T>
void WriteBitsToStream(std::ostream& stream, const T& val)
{
  int iNumBits = sizeof(T) * 8;

  while (--iNumBits >= 0)
  {
    if ((iNumBits+1) % 8 == 0) stream << " ";
    unsigned long mask = 1 << iNumBits;
    if (val & mask) stream << "1";
    else stream << "0";
  }
}

