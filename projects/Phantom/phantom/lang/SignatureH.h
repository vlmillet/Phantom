// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

#include <phantom/lang/Parameter.h>
#include <phantom/lang/Signature.h>

/// @cond INTERNAL

namespace phantom
{
namespace lang
{
#define _PHNTM_SIGNH_OVERL(...)                                                                                        \
    static Signature* Create(Source* a_pSource)                                                                        \
    {                                                                                                                  \
        return SignatureH<t_ReturnType(v_Params...)>::_Create(a_pSource, Modifiers(__VA_ARGS__));                      \
    }
#define _PHNTM_SIGNH_OVERL_VOID(...)                                                                                   \
    static Signature* Create(Source* a_pSource)                                                                        \
    {                                                                                                                  \
        return SignatureH<void(v_Params...)>::_Create(a_pSource, Modifiers(__VA_ARGS__));                              \
    }

template<typename t_Signature>
struct SignatureH;

template<typename t_ReturnType, class... v_Params>
struct SignatureH<t_ReturnType(v_Params...)>
{
    template<typename t_Signature>
    friend struct SignatureH;

    static Signature* Create(Source* a_pSource) { return _Create(a_pSource); }

protected:
    static Signature* _Create(Source* a_pSource, Modifiers a_Modifiers = Modifiers())
    {
        Type* pReturnType = PHANTOM_TYPEOF(t_ReturnType);
        PHANTOM_ASSERT(pReturnType,
                       "cannot resolve return type (in " PHANTOM_DIAGNOSTIC_FUNCTION
                       "): if using Haunt ensure you added the corresponding '.hxx.cpp' file to "
                       "your project, else ensure the return type has been reflected manually");

        Parameters params;
        _add(a_pSource, params, _get<v_Params>()...);
        return a_pSource->NewDeferred<Signature>(pReturnType, params, a_Modifiers, PHANTOM_R_FLAG_NATIVE);
    }
    inline static void _add(Source*, Parameters&) {} // recursion end

    template<typename... Args>
    inline static void _add(Source* a_pSource, Parameters& a_Params, Type* a_pType, Args... vargs)
    {
        a_Params.push_back(a_pSource->NewDeferred<Parameter>(a_pType));
        _add(a_pSource, a_Params, vargs...);
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
    static Signature* Create(Source* a_pSource) { return _Create(a_pSource); }

protected:
    static Signature* _Create(Source* a_pSource, Modifiers a_Modifiers = Modifiers())
    {
        /// Resolve first by static type resolving.
        /// If one fails, resolve via signature parsing (probably a pointer on an uncomplete class).
        Signature* pSign = SignatureH<t_ReturnType(v_Params...)>::_Create(a_pSource, a_Modifiers);
        pSign->setVariadic();
        return pSign;
    }
};

#define _PHNTM_SPEC_CV_REF_SIGNH(suffix, modifiers)                                                                    \
    template<typename t_ReturnType, class... v_Params>                                                                 \
    struct SignatureH<t_ReturnType(v_Params...) suffix> : SignatureH<t_ReturnType(v_Params...)>                        \
    {                                                                                                                  \
        using BaseType = SignatureH<t_ReturnType(v_Params...)>;                                                        \
        _PHNTM_SIGNH_OVERL(modifiers)                                                                                  \
    };                                                                                                                 \
    template<typename t_ReturnType, class... v_Params>                                                                 \
    struct SignatureH<t_ReturnType(v_Params..., ...) suffix> : SignatureH<t_ReturnType(v_Params..., ...)>              \
    {                                                                                                                  \
        using BaseType = SignatureH<t_ReturnType(v_Params..., ...)>;                                                   \
        _PHNTM_SIGNH_OVERL(modifiers)                                                                                  \
    };

_PHNTM_SPEC_CV_REF_SIGNH(&, PHANTOM_R_LVALUEREF);
_PHNTM_SPEC_CV_REF_SIGNH(&&, PHANTOM_R_RVALUEREF);
_PHNTM_SPEC_CV_REF_SIGNH(const, PHANTOM_R_CONST);
_PHNTM_SPEC_CV_REF_SIGNH(const&, PHANTOM_R_CONST | PHANTOM_R_LVALUEREF);
_PHNTM_SPEC_CV_REF_SIGNH(const&&, PHANTOM_R_CONST | PHANTOM_R_RVALUEREF);
_PHNTM_SPEC_CV_REF_SIGNH(const volatile, PHANTOM_R_CONST | PHANTOM_R_VOLATILE);
_PHNTM_SPEC_CV_REF_SIGNH(const volatile&, PHANTOM_R_CONST | PHANTOM_R_VOLATILE | PHANTOM_R_LVALUEREF);
_PHNTM_SPEC_CV_REF_SIGNH(const volatile&&, PHANTOM_R_CONST | PHANTOM_R_VOLATILE | PHANTOM_R_RVALUEREF);
_PHNTM_SPEC_CV_REF_SIGNH(volatile, PHANTOM_R_VOLATILE);
_PHNTM_SPEC_CV_REF_SIGNH(volatile&, PHANTOM_R_VOLATILE | PHANTOM_R_LVALUEREF);
_PHNTM_SPEC_CV_REF_SIGNH(volatile&&, PHANTOM_R_VOLATILE | PHANTOM_R_RVALUEREF);

} // namespace lang
} // namespace phantom

/// @endcond
