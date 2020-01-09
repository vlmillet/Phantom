// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "registration.h"

#include <phantom/detail/StaticGlobalRegistrer.h>
#include <phantom/detail/core.h>

namespace phantom
{
namespace lang
{
struct NamespaceBuilder;

struct PHANTOM_EXPORT_PHANTOM _PHNTM_GlobalRegistrer : phantom::detail::_PHNTM_StaticGlobalRegistrer,
                                                       _PHNTM_RegistrerKeyWords
{
    _PHNTM_REG_FRIENDS_NO_GLOBAL;
    friend struct phantom::lang::NamespaceBuilder;

public:
    // never put this function to cpp file
    PHANTOM_FORCEINLINE
    _PHNTM_GlobalRegistrer(RegistrationSteps a_Steps, TypeInfosGetter a_TypeInfosGetter, const char* a_pFile,
                           int a_Line, int a_Tag, bool a_bIsTemplateDependant)
        : _PHNTM_StaticGlobalRegistrer(
          PHANTOM_MODULE_HANDLE(this), a_bIsTemplateDependant ? StringView() : _PHNTM_currentPackageName(),
          a_bIsTemplateDependant ? StringView() : _PHNTM_currentSourceName(), a_pFile, a_Line, a_Tag, a_Steps),
          _PHNTM_TypeInfosGetter(a_TypeInfosGetter)
    {
    }

    _PHNTM_GlobalRegistrer& operator=(_PHNTM_GlobalRegistrer const&) = delete;
    _PHNTM_GlobalRegistrer& operator=(_PHNTM_GlobalRegistrer&&) = delete;
    _PHNTM_GlobalRegistrer(_PHNTM_GlobalRegistrer const&) = delete;
    _PHNTM_GlobalRegistrer(_PHNTM_GlobalRegistrer&&) = delete;
    ~_PHNTM_GlobalRegistrer();

    lang::Namespace* _PHNTM_getNamingScope();
    lang::Source*    _PHNTM_getOwnerScope();

    void _PHNTM_setLastSymbol(lang::Symbol* a_pSym)
    {
        _PHNTM_pLastSymbol = a_pSym;
    }
    lang::Symbol* _PHNTM_getLastSymbol() const
    {
        return _PHNTM_pLastSymbol;
    }

private:
    void              _PHNTM_process(phantom::RegistrationStep) override = 0;
    static StringView _PHNTM_currentSourceName();
    static StringView _PHNTM_currentPackageName();

private:
    TypeInfosGetter        _PHNTM_TypeInfosGetter;
    lang::Namespace* _PHNTM_pNamingScope = nullptr;
    lang::Source*    _PHNTM_pOwnerScope = nullptr;
    lang::Symbol*    _PHNTM_pLastSymbol = nullptr;
};

} // namespace lang
} // namespace phantom
