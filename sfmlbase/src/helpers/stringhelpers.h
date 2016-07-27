#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

//-----------------------------------------------------------------------------
//  convert a type to string
//-----------------------------------------------------------------------------
template <class T>
inline std::string ttos(const T& t) {
  std::ostringstream buffer;
  buffer << t;
  return buffer.str();
}

inline std::string ftos(const float& f, int precission = 1) {
  std::ostringstream buffer;
  buffer << std::fixed << std::setprecision(precission) << f;
  return buffer.str();
}

template <class T >
inline std::string ttos(const float& f) {
  return ftos(f,2);
}

template <class T>
inline std::string ttos(const bool& b) {
  if (b) return "true";
  return "false";
}

//-----------------------------------------------------------------------------
//  split a string into several
//-----------------------------------------------------------------------------

static std::vector<std::string> stringsplit(std::string line, char delimiter = ',')
{
	std::vector<std::string> elems;
	std::stringstream ss(line);
	std::string item;
	while(std::getline(ss, item, delimiter)) elems.push_back(item);
	return elems;
}
