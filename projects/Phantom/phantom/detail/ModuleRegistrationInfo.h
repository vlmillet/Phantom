// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/detail/core.h>

HAUNT_STOP;

#define PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(step)                                                  \
    ((PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP(step)) / 2)

#include <phantom/detail/Registrer.h>
#include <phantom/detail/TypeInstallationInfo.h>
#include <phantom/utils/ArrayView.h>
#include <phantom/utils/SmallMap.h>
#include <phantom/utils/SmallMultimap.h>
#include <phantom/utils/SmallSet.h>
#include <phantom/utils/SmallVector.h>
#include <phantom/utils/StringView.h>

namespace phantom
{
namespace detail
{
struct _PHNTM_StaticGlobalRegistrer;
}

namespace lang
{
using RegistrerId = size_t;

struct TypeInstallationInfo;

class ModuleRegistrationInfo
{
public:
    ModuleRegistrationInfo(size_t a_ModuleHandle);

    void stepTypeInstallation(lang::Type* a_pType);

    void processRegistration();

    void stepRegistration(RegistrationStep a_Step);

    void installTypes(TypeInstallationStep step);

    RegistrerId                                    addRegistrer(ArrayView<RegistrationStep>                    a_RegistrationSteps,
                                                                phantom::detail::_PHNTM_StaticGlobalRegistrer* a_pRegistrer);
    phantom::detail::_PHNTM_StaticGlobalRegistrer* findRegistrer(RegistrerId a_UniqueId) const;
    phantom::detail::_PHNTM_StaticGlobalRegistrer* findRegistrer(StringView a_File, int a_Line, int a_Tag) const;
    void                                           removeRegistrer(RegistrerId a_UniqueId);

    RegistrationStep getCurrentRegistrationStep() const { return m_CurrentRegistrationStep; }

    void setModule(Module* a_pModule);

    StringView    m_Name;
    StringView    m_BinaryFileName;
    StringView    m_Source;
    uint          m_uiFlags;
    size_t        m_ModuleHandle;
    lang::Module* m_pModule;
    void (*m_OnLoad)();
    void (*m_OnUnload)();
    SmallVector<StringView, 64>        m_Dependencies;
    SmallVector<TypeInstallationInfo*> m_TypeInstallationInfos;
    SmallMap<RegistrerId, phantom::detail::_PHNTM_StaticGlobalRegistrer*,
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::_None) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::_Reserved) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Start) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Namespaces) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Enums) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Templates) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::ClassTypes) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::PostClassTypes) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Typedefs) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::PostTypedefs) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::TemplateSignatures) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::PostTypes) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Variables) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::PostVariables) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Functions) +
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::End)>
    m_RegistrersById;

#define _PHNTM_CASE_STEP(macro, step)                                                                                  \
    case RegistrationStep::step:                                                                                       \
    {                                                                                                                  \
        macro(m_##step##_Registrers)                                                                                   \
    }                                                                                                                  \
    break;

#define _PHNTM_APPLY_TO_REGISTRERS(stepVar, macro)                                                                     \
    switch (stepVar)                                                                                                   \
    {                                                                                                                  \
        _PHNTM_CASE_STEP(macro, _None)                                                                                 \
        _PHNTM_CASE_STEP(macro, _Reserved)                                                                             \
        _PHNTM_CASE_STEP(macro, Start)                                                                                 \
        _PHNTM_CASE_STEP(macro, Namespaces)                                                                            \
        _PHNTM_CASE_STEP(macro, Enums)                                                                                 \
        _PHNTM_CASE_STEP(macro, Templates)                                                                             \
        _PHNTM_CASE_STEP(macro, ClassTypes)                                                                            \
        _PHNTM_CASE_STEP(macro, PostClassTypes)                                                                        \
        _PHNTM_CASE_STEP(macro, Typedefs)                                                                              \
        _PHNTM_CASE_STEP(macro, PostTypedefs)                                                                          \
        _PHNTM_CASE_STEP(macro, TemplateSignatures)                                                                    \
        _PHNTM_CASE_STEP(macro, PostTypes)                                                                             \
        _PHNTM_CASE_STEP(macro, Variables)                                                                             \
        _PHNTM_CASE_STEP(macro, PostVariables)                                                                         \
        _PHNTM_CASE_STEP(macro, Functions)                                                                             \
        _PHNTM_CASE_STEP(macro, End)                                                                                   \
    default:                                                                                                           \
        PHANTOM_UNREACHABLE();                                                                                         \
    }

    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::_None)>
    m__None_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::_Reserved)>
    m__Reserved_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Start)>
    m_Start_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Namespaces)>
    m_Namespaces_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Enums)>
    m_Enums_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Templates)>
    m_Templates_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::ClassTypes)>
    m_ClassTypes_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::PostClassTypes)>
    m_PostClassTypes_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Typedefs)>
    m_Typedefs_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::PostTypedefs)>
    m_PostTypedefs_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::TemplateSignatures)>
    m_TemplateSignatures_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::PostTypes)>
    m_PostTypes_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Variables)>
    m_Variables_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::PostVariables)>
    m_PostVariables_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::Functions)>
    m_Functions_Registrers;
    SmallVector<phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY_FOR_STEP_HALF(RegistrationStep::End)>
    m_End_Registrers;

    RegistrationStep        m_CurrentRegistrationStep = RegistrationStep::_None;
    TypeInstallationStep    m_CurrentInstallationStep;
    SmallMap<hash64, Type*> m_HashToTypeMap;
    bool                    m_bInstalled;

    void        registerTypeByHash(uint64 a_Hash, lang::Type* a_pType);
    lang::Type* registeredTypeByHash(uint64 a_Hash);

    void addTypeInstallationInfos(TypeInstallationInfo* a_pTii);

private:
    friend class phantom::detail::DynamicCppInitializerH;
    static void _SetOnRequestMembersFunc(lang::ClassType* a_pType, TypeInstallationDelegate func);
};

} // namespace lang
} // namespace phantom
