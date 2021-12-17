// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "GlobalRegistrer.h"

#include "phantom/detail/core_internal.h"
#include "phantom/lang/Application.h"

namespace phantom
{
namespace lang
{
_PHNTM_GlobalRegistrer::~_PHNTM_GlobalRegistrer() {}

lang::Scope* _PHNTM_GlobalRegistrer::_PHNTM_getNamingScope()
{
    if (!_PHNTM_pNamingScope)
    {
        StringView scope = _PHNTM_TypeInfosGetter().scope();
        if (!scope.empty())
            _PHNTM_pNamingScope = phantom::lang::Namespace::Global()->getScopeOrCreateNamespace(scope, "::");
        else
            _PHNTM_pNamingScope = phantom::lang::Namespace::Global();
    }
    return _PHNTM_pNamingScope;
}

lang::Source* _PHNTM_GlobalRegistrer::_PHNTM_getOwnerScope()
{
    if (!_PHNTM_pOwnerScope)
        _PHNTM_pOwnerScope = _PHNTM_getSource();
    return _PHNTM_pOwnerScope;
}

StringView _PHNTM_GlobalRegistrer::_PHNTM_currentSourceName()
{
    return phantom::dynamic_initializer_()->currentSource();
}

StringView _PHNTM_GlobalRegistrer::_PHNTM_currentPackageName()
{
    return phantom::dynamic_initializer_()->currentPackage();
}

} // namespace lang
} // namespace phantom
