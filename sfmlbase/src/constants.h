#pragma once

//GCC defines DEBUG, while MSVC defines _DEBUG
#if defined _DEBUG && !(defined DEBUG)
    #define DEBUG
#endif

//NDEBUG is needed to disable assertions
#if !(defined DEBUG) && !(defined NDEBUG)
    #define NDEBUG
#endif

#define _USE_MATH_DEFINES
#undef __STRICT_ANSI__

//Includes
#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN    //http://support.microsoft.com/kb/166474
    #define VC_EXTRALEAN
    #define NOMINMAX
    #include <windows.h>
#endif

#include <SFML/Config.hpp>

#ifdef _MSC_VER
    #pragma warning(disable: 4305) //Initializing a float with a double
    #pragma warning(disable: 4244) //Cast int to float
#endif

#ifdef _MSC_VER
    #define FOREACH(a,b) for each (a in (b))
#else
    #define FOREACH(a,b) for (a: (b))
#endif

//Portable type defs
using sf::Int8;
using sf::Uint8;
using sf::Int16;
using sf::Uint16;
using sf::Int32;
using sf::Uint32;
//using sf::Int64;
//using sf::Uint64;
typedef unsigned int uint;
