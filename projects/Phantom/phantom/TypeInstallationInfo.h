// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/phantom.h>

HAUNT_STOP;

#include "Registrer.h"
#include "SmallMultimap.h"
#include "SmallVector.h"
#include "StringView.h"
#include "reflection/reflection.h"

namespace phantom
{
namespace reflection
{
using TypeInstallFunc = Delegate<void(reflection::Type*, TypeInstallationStep)>;

struct PHANTOM_EXPORT_PHANTOM TypeInstallationInfo
{
    TypeInstallationInfo() = default;
    // TypeInstallationInfo(reflection::Type* a_pType, reflection::Source* a_pSource,
    // TypeInstallFunc a_setupFunc, reflection::SymbolExtenders&& a_Exts);
    TypeInstallationInfo(reflection::Type* a_pType, reflection::Source* a_pSource,
                         TypeInstallFunc a_setupFunc);
    void                exec(TypeInstallationStep step);
    void                installSymbolExtenders();
    TypeInstallFunc     installFunc;
    reflection::Type*   type;
    reflection::Source* m_pSource;
    uint                steps;
    // reflection::SymbolExtenders extenders;
};

} // namespace reflection
} // namespace phantom
