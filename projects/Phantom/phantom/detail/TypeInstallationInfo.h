// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/detail/core.h>

HAUNT_STOP;

#include "Registrer.h"

#include <phantom/lang/reflection.h>
#include <phantom/utils/SmallMultimap.h>
#include <phantom/utils/SmallVector.h>
#include <phantom/utils/StringView.h>

namespace phantom
{
namespace lang
{
using TypeInstallFunc = Delegate<void(lang::Type*, TypeInstallationStep)>;

struct PHANTOM_EXPORT_PHANTOM TypeInstallationInfo
{
    TypeInstallationInfo() = default;
    // TypeInstallationInfo(lang::Type* a_pType, lang::Source* a_pSource,
    // TypeInstallFunc a_setupFunc, lang::SymbolExtenders&& a_Exts);
    TypeInstallationInfo(lang::Type* a_pType, lang::Source* a_pSource, TypeInstallFunc a_setupFunc);
    void                exec(TypeInstallationStep step);
    void                installSymbolExtenders();
    TypeInstallFunc     installFunc;
    lang::Type*   type;
    lang::Source* m_pSource;
    uint                steps;
    // lang::SymbolExtenders extenders;
};

} // namespace lang
} // namespace phantom
