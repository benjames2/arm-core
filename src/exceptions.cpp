#include <iostream>
#include <string>

#include <inc/exceptions.h>

#define EXCEPTIONDEFINE(exname) \
exname::exname(std::string s) : s(#exname " : " + s) {} \
const char* exname::what(void) const noexcept { \
    return this->s.c_str(); \
}

EXCEPTIONDEFINE(FetchError)
EXCEPTIONDEFINE(DecodeError)
EXCEPTIONDEFINE(ExecuteError)
EXCEPTIONDEFINE(StackError)

#undef EXCEPTIONDEFINE
