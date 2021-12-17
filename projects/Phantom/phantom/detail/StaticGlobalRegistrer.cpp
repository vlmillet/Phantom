// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "StaticGlobalRegistrer.h"

#include "core_internal.h"

#include <phantom/lang/Module.h>
#include <phantom/lang/registration/registration.h>

namespace phantom
{
namespace detail
{
_PHNTM_StaticGlobalRegistrer::_PHNTM_StaticGlobalRegistrer(size_t a_ModuleHandle, StringView package, StringView source,
                                                           StringView a_File, int line, int tag,
                                                           RegistrationSteps a_Steps)
    : _PHNTM_file(a_File),
      _PHNTM_package(package),
      _PHNTM_source(source),
      _PHNTM_ModuleHandle(a_ModuleHandle),
      _PHNTM_steps(a_Steps),
      _PHNTM_line(line),
      _PHNTM_tag(tag)
{
    std::sort(_PHNTM_steps.begin(), _PHNTM_steps.end());
}

void _PHNTM_StaticGlobalRegistrer::_PHNTM_attach()
{
    if (_PHNTM_tag != -1)
    {
        dynamic_initializer_()->pushInstallation();
        _PHNTM_registrationUniqueId =
        dynamic_initializer_()->moduleRegistrationInfo(_PHNTM_ModuleHandle)->addRegistrer(_PHNTM_steps, this);
        dynamic_initializer_()->popInstallation();
    }
}

void _PHNTM_StaticGlobalRegistrer::_PHNTM_detach()
{
    if (_PHNTM_tag != -1)
    {
        if (_PHNTM_registrationUniqueId)
        {
            auto id = dynamic_initializer_()->makeRegistrerKey(_PHNTM_file, _PHNTM_line, _PHNTM_tag, true);
            PHANTOM_ASSERT(id == _PHNTM_registrationUniqueId);
            auto pMod = dynamic_initializer_()->getModuleRegistrationInfo(_PHNTM_ModuleHandle);
            if (pMod) // if not de-init before this
                pMod->removeRegistrer(_PHNTM_registrationUniqueId);
            _PHNTM_registrationUniqueId = 0;
        }
    }
}

void _PHNTM_StaticGlobalRegistrer::_PHNTM_forwardSourcePackagePush()
{
    if (!_PHNTM_package.empty())
        dynamic_initializer_()->pushPackage(_PHNTM_package);
    if (!_PHNTM_source.empty())
        dynamic_initializer_()->pushSource(_PHNTM_source);
}

void _PHNTM_StaticGlobalRegistrer::_PHNTM_forwardSourcePackagePop()
{
    if (!_PHNTM_source.empty())
        dynamic_initializer_()->popSource();
    if (!_PHNTM_package.empty())
        dynamic_initializer_()->popPackage();
}

_PHNTM_StaticGlobalRegistrer::~_PHNTM_StaticGlobalRegistrer()
{
    _PHNTM_detach();
}

void _PHNTM_StaticGlobalRegistrer::__PHNTM_process(RegistrationStep a_Step)
{
    PHANTOM_ASSERT(dynamic_initializer_()->moduleRegistrationInfo(_PHNTM_ModuleHandle)->getCurrentRegistrationStep() >=
                   a_Step,
                   "processing registration step before the module has entered it");
#if defined(PHANTOM_DEV)
#    pragma message(PHANTOM_TODO "cleanup this once quick registration is fully implemented")
#endif
    lang::detail::pushModule(_PHNTM_getModule());
    lang::detail::pushSource(_PHNTM_getSource());
    _PHNTM_process(a_Step);
    lang::detail::popSource();
    lang::detail::popModule();
}

lang::Source* _PHNTM_StaticGlobalRegistrer::_PHNTM_getSource() const
{
    // in case of a current module is being installed
    if (_PHNTM_source.empty())
    {
        if (lang::Module* pModule = lang::detail::currentModule())
        {
            return pModule->getAnonymousSource();
        }
    }
    if (!_PHNTM_source_cache)
    {
        lang::detail::pushModule(_PHNTM_getModule());
        _PHNTM_source_cache = dynamic_initializer_()->nativeSource(_PHNTM_file, _PHNTM_package, _PHNTM_source);
        lang::detail::popModule();
    }
    return _PHNTM_source_cache;
}

lang::Module* _PHNTM_StaticGlobalRegistrer::_PHNTM_getModule() const
{
    return dynamic_initializer_()->getModuleRegistrationInfo(_PHNTM_ModuleHandle)->m_pModule;
}

} // namespace detail
} // namespace phantom
