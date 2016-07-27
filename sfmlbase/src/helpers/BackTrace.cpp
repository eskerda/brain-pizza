
#include "BackTrace.h"

#ifndef __GNUC__

void BackTrace::print (ostream& os) {
    cerr << "backtrace not available" << endl;
}

#else

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>
#include <cxxabi.h>


// Implementation based on
// http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes


void BackTrace::print (ostream& os) {

    cerr << "[bt]: BackTrace for process " << getpid() << ":" << endl;

    const int length = 50;
    void* array[length];
    int size = backtrace(array, length);
    char** messages = backtrace_symbols(array, size);

    // skip first stack frame (points here)
    for (int i = 1; i < size and messages != NULL; ++i) {
        char *mangled_name = 0;
        char *offset_begin = 0;
        char *offset_end = 0;

        // find parantheses and +address offset surrounding mangled name
        for (char *p = messages[i]; *p; ++p) {
            if (*p == '(') {
                mangled_name = p;
            } else if (*p == '+') {
                offset_begin = p;
            } else if (*p == ')') {
                offset_end = p;
                break;
            }
        }

        // if the line could be processed, attempt to demangle the symbol
        if (mangled_name and offset_begin and offset_end and mangled_name < offset_begin){
            *mangled_name++ = '\0';
            *offset_begin++ = '\0';
            *offset_end++ = '\0';

            int status;
            char * real_name = abi::__cxa_demangle(mangled_name, 0, 0, &status);

            // if demangling is successful, output the demangled function name
            if (status == 0) {
                os << "[bt]: (" << i << ") " << messages[i] << " : "
                          << real_name << "+" << offset_begin << offset_end
                          << endl;
            }
            // otherwise, output the mangled function name
            else {
                os << "[bt]: (" << i << ") " << messages[i] << " : "
                          << mangled_name << "+" << offset_begin << offset_end
                          << endl;
            }
            free(real_name);
        }
        // otherwise, print the whole line
        else {
            os << "[bt]: (" << i << ") " << messages[i] << endl;
        }
    }
    os << endl;

    free(messages);
}

#endif
