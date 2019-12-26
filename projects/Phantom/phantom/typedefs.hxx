#pragma once

// haunt {

#include "typedefs.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/source>
#include <phantom/struct>
#include <phantom/enum>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include "SmallString.hxx"
#include "SmallVector.hxx"
#include "StringView.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_SOURCE("phantom.StringView")

    #if PHANTOM_NOT_TEMPLATE
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<StringView>("StringView"); }
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<StringViews>("StringViews"); }
    #endif // PHANTOM_NOT_TEMPLATE
PHANTOM_END("phantom.StringView")
}
namespace phantom {
PHANTOM_SOURCE("phantom.String")

    #if PHANTOM_NOT_TEMPLATE
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<String>("String"); }
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<Strings>("Strings"); }
    #endif // PHANTOM_NOT_TEMPLATE
PHANTOM_END("phantom.String")
}
namespace phantom {
PHANTOM_SOURCE("phantom.StringBuffer")

    #if PHANTOM_NOT_TEMPLATE
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<StringBuffer>("StringBuffer"); }
    #endif // PHANTOM_NOT_TEMPLATE
PHANTOM_END("phantom.StringBuffer")
}
namespace phantom {
PHANTOM_SOURCE("phantom.Core")

    #if PHANTOM_NOT_TEMPLATE
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<VoidPtrs>("VoidPtrs"); }
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<ConstVoidPtrs>("ConstVoidPtrs"); }
    #endif // PHANTOM_NOT_TEMPLATE
PHANTOM_END("phantom.Core")
}
namespace phantom {
PHANTOM_SOURCE("phantom.Core")

    #if PHANTOM_NOT_TEMPLATE
    PHANTOM_REGISTER(Enums) { this_().enum_<>().values({
        {"max_type_size",max_type_size}});
    }
    PHANTOM_STRUCT(hex64)
    {
        this_()
        .constructor<void()>()
        .constructor<void(uint64), explicit_>()
        .constructor<void(const hex64&)>()
        .method<uint64() const, explicit_>("operator uint64", &_::operator notypedef<uint64>)
        .method<bool(const hex64&) const>("operator<", &_::operator<)
        .method<bool(const hex64&) const>("operator==", &_::operator==)
        .method<bool(const hex64&) const>("operator!=", &_::operator!=)
    
    .protected_()
        .field("value", &_::value)
        ;
    }
    PHANTOM_STRUCT(Closure)
    {
        this_()
        .field("address", &_::address)
        .field("offset", &_::offset)
        ;
    }
    PHANTOM_STRUCT(BufferData)
    {
        this_()
        .constructor<void()>()
        .constructor<void(byte*, size_t)>()
        .field("memory", &_::memory)
        .field("size", &_::size)
        ;
    }
    PHANTOM_STRUCT(vtable_info)
    {
        this_()
        .constructor<void()>()
        .constructor<void(size_t, void**, size_t)>()
        .method<bool(const vtable_info&) const>("operator<", &_::operator<)
        .field("offset", &_::offset)
        .field("member_functions", &_::member_functions)
        .field("count", &_::count)
        ;
    }
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<hash64>("hash64"); }
    #endif // PHANTOM_NOT_TEMPLATE
PHANTOM_END("phantom.Core")
namespace reflection {
PHANTOM_SOURCE("phantom.reflection.Core")

    #if PHANTOM_NOT_TEMPLATE
    PHANTOM_REGISTER(Enums) { this_().enum_<ABI>().values({
        {"CDecl",CDecl},
        {"VarArgCall",VarArgCall},
        {"StdCall",StdCall},
        {"FastCall",FastCall},
        {"Win64",Win64},
        {"ThisCall",ThisCall},
        {"PlaceholderCall",PlaceholderCall},
        {"UnknownCall",UnknownCall},
        {"DefaultCall",DefaultCall},
        {"MethodCall",MethodCall}});
    }
    PHANTOM_REGISTER(Typedefs) { this_().typedef_<ECallingConv>("ECallingConv"); }
    #endif // PHANTOM_NOT_TEMPLATE
PHANTOM_END("phantom.reflection.Core")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }

namespace std
{
PHANTOM_SOURCE("std.integers");
PHANTOM_TYPEDEF(size_t);
PHANTOM_TYPEDEF(ptrdiff_t);
PHANTOM_TYPEDEF(intptr_t);
PHANTOM_TYPEDEF(uintptr_t);
PHANTOM_END("std.integers");
PHANTOM_SOURCE("std.args");
PHANTOM_TYPEDEF(va_list);
PHANTOM_END("std.args");
} // namespace std

PHANTOM_SOURCE("c.def");
PHANTOM_TYPEDEF(ptrdiff_t);
PHANTOM_TYPEDEF(intptr_t);
PHANTOM_TYPEDEF(uintptr_t);
PHANTOM_END("c.def");
PHANTOM_SOURCE("c.args");
PHANTOM_TYPEDEF(va_list);
PHANTOM_END("c.args");
