// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include <phantom/detail/core.h>
#include <phantom/utils/SmallVector.h>
#include <phantom/utils/StringView.h>

namespace phantom
{
namespace detail
{
struct PHANTOM_EXPORT_PHANTOM _PHNTM_StaticGlobalRegistrer
{
    _PHNTM_StaticGlobalRegistrer(size_t a_ModuleHandle, StringView package, StringView source, StringView file,
                                 int line, int tag, RegistrationSteps a_Steps);
    ~_PHNTM_StaticGlobalRegistrer();
    void                              __PHNTM_process(RegistrationStep);
    virtual void                      _PHNTM_process(RegistrationStep) = 0;
    reflection::Source*               _PHNTM_getSource() const;
    reflection::Module*               _PHNTM_getModule() const;
    void                              _PHNTM_attach();
    void                              _PHNTM_detach();
    StringView                        _PHNTM_file;
    StringView                        _PHNTM_package;
    StringView                        _PHNTM_source;
    size_t                            _PHNTM_ModuleHandle;
    size_t                            _PHNTM_registrationUniqueId;
    SmallVector<RegistrationStep, 20> _PHNTM_steps;
    int                               _PHNTM_line;
    int                               _PHNTM_tag;

private:
    mutable reflection::Source* _PHNTM_source_cache = nullptr;
};
} // namespace detail
} // namespace phantom
