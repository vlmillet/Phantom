// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "GlobalRegistrer.h"

#include "phantom/detail/core_internal.h"
#include "phantom/reflection/Application.h"

namespace phantom
{
namespace reflection
{
_PHNTM_GlobalRegistrer::~_PHNTM_GlobalRegistrer()
{
}

reflection::Namespace* _PHNTM_GlobalRegistrer::_PHNTM_getNamingScope()
{
    if (!_PHNTM_pNamingScope)
    {
        StringView scope = _PHNTM_TypeInfosGetter().scope();
        if (!scope.empty())
            _PHNTM_pNamingScope = phantom::reflection::Namespace::Global()->findOrCreateNamespace(scope);
        else
            _PHNTM_pNamingScope = phantom::reflection::Namespace::Global();
    }
    return _PHNTM_pNamingScope;
}

reflection::Source* _PHNTM_GlobalRegistrer::_PHNTM_getOwnerScope()
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

} // namespace reflection
} // namespace phantom
