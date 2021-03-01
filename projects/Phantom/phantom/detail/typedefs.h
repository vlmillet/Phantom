// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <cassert>
#include <cstdint>
#include <phantom/fwd.h>

HAUNT_SOURCE("phantom.StringView");
namespace phantom
{
using StringView = BasicStringView<char>;
using StringViews = SmallVector<StringView, 10>;
} // namespace phantom
HAUNT_END("phantom.StringView");

HAUNT_SOURCE("phantom.String");
namespace phantom
{
#if !defined(PHANTOM_STRING_STATIC_SIZE)
#    define PHANTOM_STRING_STATIC_SIZE 16
#endif
#if !defined(PHANTOM_STRING_EXPAND_SIZE)
#    define PHANTOM_STRING_EXPAND_SIZE 16
#endif
#if !defined(PHANTOM_STRINGS_STATIC_SIZE)
#    define PHANTOM_STRINGS_STATIC_SIZE 16
#endif
#if !defined(PHANTOM_STRINGS_EXPAND_SIZE)
#    define PHANTOM_STRINGS_EXPAND_SIZE 16
#endif
using String = SmallString<char, PHANTOM_STRING_STATIC_SIZE>;
using Strings = SmallVector<String, PHANTOM_STRINGS_STATIC_SIZE>;
} // namespace phantom
HAUNT_END("phantom.String");

HAUNT_SOURCE("phantom.StringBuffer");
namespace phantom
{
using StringBuffer = SmallString<char, 128>;
}
HAUNT_END("phantom.StringBuffer");

HAUNT_SOURCE("phantom.Core")

namespace phantom
{
using VoidPtrs = SmallVector<void*, 16>;
using ConstVoidPtrs = SmallVector<const void*, 16>;
} // namespace phantom

#if defined(_LIBCPP_VERSION)
#    define PHANTOM_STD_NAMESPACE PHANTOM_STD_NAMESPACE::_LIBCPP_NAMESPACE
#    define PHANTOM_STD_NAMESPACE_LST std, _LIBCPP_NAMESPACE
#    define PHANTOM_STD_NAMESPACE_GCC_CXX11 PHANTOM_STD_NAMESPACE
#    define PHANTOM_STD_NAMESPACE_LST_GCC_CXX11 PHANTOM_STD_NAMESPACE_LST
#    define PHANTOM_STD_NAMESPACE_OPEN _LIBCPP_BEGIN_NAMESPACE_STD
#    define PHANTOM_STD_NAMESPACE_CLOSE _LIBCPP_END_NAMESPACE_STD
#    define PHANTOM_STD_NAMESPACE_GCC_CXX11_OPEN
#    define PHANTOM_STD_NAMESPACE_GCC_CXX11_CLOSE

#elif defined(__GLIBCXX__)
#    define PHANTOM_STD_NAMESPACE_OPEN                                                                                 \
        namespace std                                                                                                  \
        {                                                                                                              \
        _GLIBCXX_BEGIN_NAMESPACE_VERSION
#    define PHANTOM_STD_NAMESPACE_CLOSE                                                                                \
        _GLIBCXX_END_NAMESPACE_VERSION                                                                                 \
        }
#    define PHANTOM_STD_NAMESPACE_GCC_CXX11_OPEN _GLIBCXX_BEGIN_NAMESPACE_CXX11
#    define PHANTOM_STD_NAMESPACE_GCC_CXX11_CLOSE _GLIBCXX_END_NAMESPACE_CXX11
#    define PHANTOM_STD_NAMESPACE_LST std
#    define PHANTOM_STD_NAMESPACE std
#    define PHANTOM_STD_NAMESPACE_GCC_CXX11 PHANTOM_STD_NAMESPACE
#    define PHANTOM_STD_NAMESPACE_LST_GCC_CXX11 PHANTOM_STD_NAMESPACE_LST

#else
#    define PHANTOM_STD_NAMESPACE std
#    define PHANTOM_STD_NAMESPACE_LST std
#    define PHANTOM_STD_NAMESPACE_GCC_CXX11 PHANTOM_STD_NAMESPACE
#    define PHANTOM_STD_NAMESPACE_LST_GCC_CXX11 PHANTOM_STD_NAMESPACE_LST
#    define PHANTOM_STD_NAMESPACE_GCC_CXX11_OPEN
#    define PHANTOM_STD_NAMESPACE_GCC_CXX11_CLOSE
#    define PHANTOM_STD_NAMESPACE_OPEN                                                                                 \
        namespace std                                                                                                  \
        {
#    define PHANTOM_STD_NAMESPACE_CLOSE }

#endif

namespace phantom
{
enum
{
    max_type_size = 1024
};

HAUNT_PAUSE;

typedef float  float32;
typedef double float64;

#if (PHANTOM_COMPILER == PHANTOM_COMPILER_GCC) || (PHANTOM_COMPILER == PHANTOM_COMPILER_CLANG)
typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8_t   sint8;
typedef int16_t  sint16;
typedef int32_t  sint32;
typedef int64_t  sint64;

#elif (PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO)
typedef signed __int8    int8;
typedef __int16          int16;
typedef __int32          int32;
typedef __int64          int64;
typedef unsigned __int8  uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;
typedef signed __int8    sint8;
typedef signed __int16   sint16;
typedef signed __int32   sint32;
typedef signed __int64   sint64;

#else
#    error Int types must be defined for this compiler
#endif

typedef long long longlong;

typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef unsigned long long ulonglong;

typedef signed char      schar;
typedef signed short     sshort;
typedef signed int       sint;
typedef signed long      slong;
typedef signed long long slonglong;
typedef wchar_t          wchar;

#if PHANTOM_HAS_BUILT_IN_CHAR16_T
typedef char16_t char16;
#elif PHANTOM_HAS_BUILT_IN_WCHAR_T
typedef wchar_t          char16;
#else
typedef short char16;
#endif

#if PHANTOM_HAS_BUILT_IN_CHAR32_T
typedef char32_t char32;
#else
typedef int              char32;
#endif

#if defined(_UNICODE)
typedef wchar_t character;
#else
typedef char             character;

#endif

typedef long double longdouble;

typedef ::size_t      size_t;
typedef size_t        handle;
typedef void*         voidptr;
typedef unsigned char byte;

/// @cond ADVANCED

template<typename t_Ty>
struct make_float
{
    typedef float type;
};

template<>
struct make_float<double>
{
    typedef double type;
};

/// @endcond

HAUNT_RESUME;

struct hex64
{
    hex64() : value(0) {}
    inline explicit hex64(uint64 x) : value(x) {}
    inline hex64(const hex64& x) : value(x.value) {}
    explicit    operator uint64() const { return value; }
    inline bool operator<(const hex64& other) const { return value < other.value; }
    inline bool operator==(const hex64& other) const { return value == other.value; }
    inline bool operator!=(const hex64& other) const { return value != other.value; }

protected:
    uint64 value;
};

struct Closure
{
    void* address = 0;
    int   offset = 0;
};

namespace lang
{
class Type;
}

struct RttiMapData;

/// @cond ADVANCED
/// \brief  Buffer representation with memory and size.
struct BufferData
{
    BufferData() : memory(0), size(0) {}
    BufferData(byte* m, size_t s) : memory(m), size(s) {}
    byte*  memory;
    size_t size;
};
/// @endcond

HAUNT_OFF
typedef void (*DynamicDeleteFunc)(void*);

// ASSERTION ON TYPE SIZES (PHANTOM IS VERY STRICT ON THAT && RUNTIME COMPILATION CANNOT BE USED ON
// PLATFORMS ! SATISFYING THIS)

struct vtable_info
{
    vtable_info() : offset(0xffffffff), member_functions(NULL), count(0xffffffff) {}

    vtable_info(size_t a_offset, void** a_member_functions, size_t a_count)
        : offset(a_offset), member_functions(a_member_functions), count(a_count)
    {
    }

    bool operator<(const vtable_info& other) const { return offset < other.offset; }

    size_t offset;
    void** member_functions;
    size_t count;
};

typedef uint64 hash64;

HAUNT_END("phantom.Core");

namespace lang
{
HAUNT_SOURCE("phantom.lang.Core");

enum ABI : char
{
    CDecl,      /* Native C calling conventions */
    VarArgCall, /* Native C with optional variable arguments */
    StdCall,    /* Win32 STDCALL (same as cdecl if not Win32) */
    FastCall,   /* Win32 FASTCALL (same as cdecl if not Win32) or Win64 thiscall/fastcall*/
    Win64,      /* Win64 FASTCALL */
    ThisCall,   /* Member function x86 win32 call */
    PlaceholderCall,
    UnknownCall,
    DefaultCall = CDecl,
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#    if PHANTOM_ARCHITECTURE_X86_64 == PHANTOM_ARCHITECTURE
    MethodCall = Win64,
#    else
    MethodCall = ThisCall,
#    endif
#else
    MethodCall = CDecl,
#endif
};

typedef ABI ECallingConv; // synonyms

HAUNT_END("phantom.lang.Core");
} // namespace lang

template<typename t_Output, typename t_Input>
PHANTOM_FORCEINLINE t_Output union_cast(t_Input i)
{
    union {
        t_Output o;
        t_Input  i;
    } u;
    PHANTOM_STATIC_ASSERT(sizeof(t_Input) == sizeof(u) && sizeof(t_Output) == sizeof(t_Input),
                          "cannot union_cast the given types");
    u.i = i;
    return u.o;
}

} // namespace phantom

//#endif // defined(PHANTOM_USE_POOL_ALLOCATORS)
