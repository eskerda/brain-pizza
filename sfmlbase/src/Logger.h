#pragma once

#include "constants.h"

#include <string>
#include <iostream>

#include "helpers/noncopyable.h"

//TODO: Add time to log entries
//TODO: Enable logging to file
class Log : noncopyable
{
	public:

    //static void UseConsole();
    //static void UseFile();

    static void d(const std::string& s) {
        std::cout << "[Debug] " << s << std::endl;
    }

    static void i(const std::string& s) {
        std::cout << "[Info] " << s << std::endl;
    }

    static void w(const std::string& s) {
        std::cerr << "[Warning] " << s << std::endl;
    }

    static void e(const std::string& s) {
        std::cerr << "[ERROR] " << s << std::endl;
#ifdef _MSC_VER
        MessageBoxA(0, s.c_str(), "Error!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#endif
    }

};
