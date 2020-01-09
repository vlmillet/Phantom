// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

HAUNT_STOP;

#include <phantom/reflection/Signature.h>

/// @cond INTERNAL

namespace phantom
{
namespace reflection
{
#define _PHNTM_SIGNH_OVERL(...)                                                                    \
    static Signature* Create()                                                                     \
    {                                                                                              \
        return SignatureH<t_ReturnType(v_Params...)>::_Create(Modifiers(__VA_ARGS__));             \
    }
#define _PHNTM_SIGNH_OVERL_VOID(...)                                                               \
    static Signature* Create()                                                                     \
    {                                                                                              \
        return SignatureH<void(v_Params...)>::_Create(Modifiers(__VA_ARGS__));                     \
    }

template<typename t_Signature>
struct SignatureH;

template<typename t_ReturnType, class... v_Params>
struct SignatureH<t_ReturnType(v_Params...)>
{
    template<typename t_Signature>
    friend struct SignatureH;

    static Signature* Create()
    {
        return _Create();
    }

protected:
    static Signature* _Create(Modifiers a_Modifiers = Modifiers())
    {
        /// Resolve first by static type resolving.
        /// If one fails, resolve via signature parsing (probably a pointer on an uncomplete class).
        Signature* pSignature = Signature::Create(a_Modifiers, PHANTOM_R_FLAG_NATIVE);
        Type*      pReturnType = PHANTOM_TYPEOF(t_ReturnType);
        PHANTOM_ASSERT(pReturnType,
                       "cannot resolve return type (in " PHANTOM_DIAGNOSTIC_FUNCTION
                       "): if using Haunt ensure you added the corresponding '.hxx.cpp' file to "
                       "your project, else ensure the return type has been reflected manually");
        pSignature->setReturnType(pReturnType);
        _add(pSignature, _get<v_Params>()...);
        return pSignature;
    }
    inline static void _add(Signature*)
    {
    } // recursion end

    template<typename... Args>
    inline static void _add(Signature* a_pSignature, Type* a_pType, Args... vargs)
    {
        a_pSignature->addParameter(a_pType);
        _add(a_pSignature, vargs...);
    }

    template<typename t_Param>
    inline static Type* _get()
    {
        Type* pType = PHANTOM_TYPEOF(t_Param);
        PHANTOM_ASSERT(pType,
                       "cannot resolve parameter type (in " PHANTOM_DIAGNOSTIC_FUNCTION
                       "): if using Haunt ensure you added the corresponding '.hxx.cpp' file to "
                       "your project, else ensure the return type has been reflected manually");
        return pType;
    }
};

// workaround visual studio 2015 bug
template<class... v_Params>
struct SignatureH<const void(v_Params...)> : public SignatureH<void(v_Params...)>
{
    _PHNTM_SIGNH_OVERL_VOID()
};
template<class... v_Params>
struct SignatureH<volatile void(v_Params...)> : public SignatureH<void(v_Params...)>
{
    _PHNTM_SIGNH_OVERL_VOID();
};
template<class... v_Params>
struct SignatureH<const volatile void(v_Params...)> : public SignatureH<void(v_Params...)>
{
    _PHNTM_SIGNH_OVERL_VOID();
};

template<typename t_ReturnType, class... v_Params>
struct SignatureH<t_ReturnType(v_Params..., ...)>
{
    static Signature* Create()
    {
        return _Create();
    }

protected:
    static Signature* _Create(Modifiers a_Modifiers = Modifiers())
    {
        /// Resolve first by static type resolving.
        /// If one fails, resolve via signature parsing (probably a pointer on an uncomplete class).
        Signature* pSign = SignatureH<t_ReturnType(v_Params...)>::_Create(a_Modifiers);
        pSign->setVariadic();
        return pSign;
    }
};

#define _PHNTM_SPEC_CV_REF_SIGNH(suffix, modifiers)                                                \
    template<typename t_ReturnType, class... v_Params>                                             \
    struct SignatureH<t_ReturnType(v_Params...) suffix> : SignatureH<t_ReturnType(v_Params...)>    \
    {                                                                                              \
        using BaseType = SignatureH<t_ReturnType(v_Params...)>;                                    \
        _PHNTM_SIGNH_OVERL(modifiers)                                                              \
    };                                                                                             \
    template<typename t_ReturnType, class... v_Params>                                             \
    struct SignatureH<t_ReturnType(v_Params..., ...) suffix>                                       \
        : SignatureH<t_ReturnType(v_Params..., ...)>                                               \
    {                                                                                              \
        using BaseType = SignatureH<t_ReturnType(v_Params..., ...)>;                               \
        _PHNTM_SIGNH_OVERL(modifiers)                                                              \
    };

_PHNTM_SPEC_CV_REF_SIGNH(const, PHANTOM_R_CONST);
_PHNTM_SPEC_CV_REF_SIGNH(const&, PHANTOM_R_CONST | PHANTOM_R_LVALUEREF);
_PHNTM_SPEC_CV_REF_SIGNH(const&&, PHANTOM_R_CONST | PHANTOM_R_RVALUEREF);
_PHNTM_SPEC_CV_REF_SIGNH(const volatile, PHANTOM_R_CONST | PHANTOM_R_VOLATILE);
_PHNTM_SPEC_CV_REF_SIGNH(const volatile&,
                         PHANTOM_R_CONST | PHANTOM_R_VOLATILE | PHANTOM_R_LVALUEREF);
_PHNTM_SPEC_CV_REF_SIGNH(const volatile&&,
                         PHANTOM_R_CONST | PHANTOM_R_VOLATILE | PHANTOM_R_RVALUEREF);
_PHNTM_SPEC_CV_REF_SIGNH(volatile, PHANTOM_R_VOLATILE);
_PHNTM_SPEC_CV_REF_SIGNH(volatile&, PHANTOM_R_VOLATILE | PHANTOM_R_LVALUEREF);
_PHNTM_SPEC_CV_REF_SIGNH(volatile&&, PHANTOM_R_VOLATILE | PHANTOM_R_RVALUEREF);

} // namespace reflection
} // namespace phantom

/// @endcond
