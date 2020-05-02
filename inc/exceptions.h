#pragma once

#include <iostream>
#include <string>
#include <exception>

#define EXCEPTIONDECLARE(exname) \
struct exname : public std::exception { \
    std::string s; \
    const char* what(void) const noexcept override; \
    exname(std::string s); \
};

// just a list of the exceptions we want to create. 
// these names are mirrored in src/exceptions.cpp
EXCEPTIONDECLARE(FetchError)
EXCEPTIONDECLARE(DecodeError)
EXCEPTIONDECLARE(ExecuteError)

#undef EXCEPTIONDECLARE
