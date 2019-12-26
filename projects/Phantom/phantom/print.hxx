#pragma once

// haunt {

#include "print.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/function>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("print")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Functions) { this_().function<void(const char*, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(StringView, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(bool, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(char, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(schar, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(uchar, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(short, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(ushort, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(int, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(uint, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(long, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(ulong, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(longlong, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(ulonglong, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(float, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(double, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(longdouble, int, int)>("print", print)["0"]["0"];}
        PHANTOM_REGISTER(Functions) { this_().function<void(void*, int, int)>("print", print)["0"]["0"];}
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("print")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
