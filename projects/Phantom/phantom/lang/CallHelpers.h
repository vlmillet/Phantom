// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/lang/ExecutionContext.h>
#include <phantom/traits/CopyTraits.h>
#include <phantom/traits/MoveTraits.h>
#include <phantom/traits/SimplifiedType.h>

HAUNT_STOP;

namespace phantom
{
namespace lang
{
template<typename t_Ty>
struct CallAddresser
{
    static void* address(t_Ty call)
    {
        return nullptr;
    }
    enum
    {
        value = false
    };
};

template<typename t_Ty>
struct CallAddresser<t_Ty&>
{
    static void* address(t_Ty& call)
    {
        return (void*)&call;
    }
    enum
    {
        value = true
    };
};

template<typename t_Ty>
struct CallAddresser<t_Ty const&>
{
    static void* address(t_Ty const& call)
    {
        return nullptr;
    }
    enum
    {
        value = false
    };
};

template<typename t_Ty>
struct CallReturnTypeH
{
    typedef t_Ty type;
};

template<typename t_Ty>
struct CallReturnTypeH<t_Ty&>
{
    typedef t_Ty* type;
};

template<typename t_Ty>
struct CallReturnTypeH<t_Ty&&>
{
    typedef t_Ty* type;
};

template<typename t_Ty>
struct CallReturnTypeH<const t_Ty>
{
    typedef t_Ty type;
};

template<typename t_Ty>
struct CallReturnTypeH<const volatile t_Ty>
{
    typedef t_Ty type;
};

template<typename t_Ty>
struct CallReturnTypeH<volatile t_Ty>
{
    typedef t_Ty type;
};

template<typename t_Ty>
struct CallReturnConverter
{
    static t_Ty&& apply(t_Ty&& input)
    {
        return std::move(input);
    }
};

template<typename t_Ty>
struct CallReturnConverter<t_Ty&>
{
    static t_Ty* apply(t_Ty& input)
    {
        return &input;
    }
};

template<typename t_Ty>
struct CallReturnConverter<t_Ty&&>
{
    static t_Ty* apply(t_Ty&& input)
    {
        return &input;
    }
};

namespace detail
{
template<class t_RetType, bool t_is_copy_assignable>
struct CallReturnerH
{
    static bool apply(void* a_pReturnAddress, t_RetType&& input)
    {
        *reinterpret_cast<t_RetType*>(a_pReturnAddress) = std::move(input);
        return true;
    }
};
template<class t_RetType>
struct CallReturnerH<t_RetType, false>
{
    static bool apply(void*, t_RetType&&)
    {
        return false;
    }
};
} // namespace detail

template<typename t_RetType>
struct CallReturner
    : public detail::CallReturnerH<t_RetType,
                                   phantom::IsCopyAssignableAndNotDisabled<t_RetType>::value ||
                                   phantom::IsCopyAssignableAndNotDisabled<t_RetType>::value>
{
};

template<typename t_RetTypeNoRef>
struct CallReturner<t_RetTypeNoRef&>
{
    static bool apply(void* a_pReturnAddress, t_RetTypeNoRef& input)
    {
        *reinterpret_cast<t_RetTypeNoRef**>(a_pReturnAddress) = &input;
        return true;
    }
};

template<typename t_RetTypeNoRef>
struct CallReturner<t_RetTypeNoRef&&>
{
    static bool apply(void* a_pReturnAddress, t_RetTypeNoRef&& input)
    {
        *reinterpret_cast<t_RetTypeNoRef**>(a_pReturnAddress) = &input;
        return true;
    }
};

template<int e_eConvention, typename t_Signature>
struct FunctionPointerTypeFromFunctionType;

template<typename t_ReturnType, typename... v_Params>
struct FunctionPointerTypeFromFunctionType<ABI::VarArgCall, t_ReturnType(v_Params..., ...)>
{
    typedef t_ReturnType (*type)(v_Params..., ...);
};

/// STDCALL

#if PHANTOM_HAS_STDCALL

template<typename t_ReturnType, typename... v_Params>
struct FunctionPointerTypeFromFunctionType<ABI::StdCall, t_ReturnType(v_Params...)>
{
    typedef t_ReturnType(PHANTOM_CALLCONV(stdcall) * type)(v_Params...);
};

#endif

/// FASTCALL

#if PHANTOM_HAS_FASTCALL

template<typename t_ReturnType, typename... v_Params>
struct FunctionPointerTypeFromFunctionType<ABI::FastCall, t_ReturnType(v_Params...)>
{
    typedef t_ReturnType(PHANTOM_CALLCONV(fastcall) * type)(v_Params...);
};

#endif

/// CDECL

#if defined(cdecl)
#    define _PHNTM_CDECL_WAS_DEFINED
#    pragma push_macro("cdecl")
#    undef cdecl
#endif

template<typename t_ReturnType, typename... v_Params>
struct FunctionPointerTypeFromFunctionType<ABI::CDecl, t_ReturnType(v_Params...)>
{
    typedef t_ReturnType(PHANTOM_CALLCONV(cdecl) * type)(v_Params...);
};

#if defined(_PHNTM_CDECL_WAS_DEFINED)
#    undef _PHNTM_CDECL_WAS_DEFINED
#    pragma pop_macro("cdecl")
#endif

} // namespace lang
} // namespace phantom
