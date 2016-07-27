#pragma once

#ifdef _MSC_VER

    #include "constants.h"

    #include <windows.h>
    #include <conio.h>
    #include <io.h>
    #include <fcntl.h>

    #include <iostream>

    void OpenConsole() {

        AllocConsole();

        HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
        int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
        FILE* hf_out = _fdopen(hCrt, "w");
        setvbuf(hf_out, NULL, _IONBF, 1);
        *stdout = *hf_out;

        HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
        hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
        FILE* hf_in = _fdopen(hCrt, "r");
        setvbuf(hf_in, NULL, _IONBF, 128);
        *stdin = *hf_in;

    }

    //default text colors can be found in wincon.h
    inline void SetTextColor(WORD colors) {
        HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, colors);
    }

    inline void PressAnyKeyToContinue() {
        //change text color to white
        SetTextColor(FOREGROUND_BLUE| FOREGROUND_RED | FOREGROUND_GREEN);
        std::cout << "\n\nPress any key to continue" << std::endl;
        while (!_kbhit()){}
    }

#endif
