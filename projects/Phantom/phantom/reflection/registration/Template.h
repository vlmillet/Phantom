#pragma once

#include <haunt>

HAUNT_STOP;

#include <phantom/StaticGlobalRegistrer.h>
#include <phantom/phantom.h>

namespace phantom
{
namespace reflection
{
struct PHANTOM_EXPORT_PHANTOM TemplateRegistrer
    : public ::phantom::detail::_PHNTM_StaticGlobalRegistrer
{
    TemplateRegistrer(StringView (*func)(int), const char* a_strFile, int line, int tag);
    void _PHNTM_process(phantom::RegistrationStep PHANTOM_REGISTRATION_STEP) override;

private:
    StringView (*m_func)(int);
};
} // namespace reflection
} // namespace phantom
