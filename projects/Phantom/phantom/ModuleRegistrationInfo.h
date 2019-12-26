// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/phantom.h>

HAUNT_STOP;

#include "TypeInstallationInfo.h"
#include "phantom/ArrayView.h"
#include "phantom/Registrer.h"
#include "phantom/SmallMap.h"
#include "phantom/SmallMultimap.h"
#include "phantom/SmallSet.h"
#include "phantom/SmallVector.h"
#include "phantom/StringView.h"

namespace phantom
{
namespace detail
{
struct _PHNTM_StaticGlobalRegistrer;
}

namespace reflection
{
using RegistrerId = size_t;

struct TypeInstallationInfo;

class ModuleRegistrationInfo
{
public:
    ModuleRegistrationInfo(size_t a_ModuleHandle);

    void stepTypeInstallation(reflection::Type* a_pType);

    void processRegistration();

    void stepRegistration(RegistrationStep a_Step);

    void installTypes(TypeInstallationStep step);

    RegistrerId addRegistrer(ArrayView<RegistrationStep>                    a_RegistrationSteps,
                             phantom::detail::_PHNTM_StaticGlobalRegistrer* a_pRegistrer);
    phantom::detail::_PHNTM_StaticGlobalRegistrer* findRegistrer(RegistrerId a_UniqueId) const;
    phantom::detail::_PHNTM_StaticGlobalRegistrer* findRegistrer(StringView a_File, int a_Line,
                                                                 int a_Tag) const;
    void                                           removeRegistrer(RegistrerId a_UniqueId);

    RegistrationStep getCurrentRegistrationStep() const
    {
        return m_CurrentRegistrationStep;
    }

    void setModule(Module* a_pModule);

    StringView          m_Name;
    StringView          m_BinaryFileName;
    StringView          m_Source;
    uint                m_uiFlags;
    size_t              m_ModuleHandle;
    reflection::Module* m_pModule;
    void (*m_OnLoad)();
    void (*m_OnUnload)();
    SmallVector<StringView, 64>        m_Dependencies;
    SmallVector<TypeInstallationInfo*> m_TypeInstallationInfos;
    SmallMap<RegistrerId, phantom::detail::_PHNTM_StaticGlobalRegistrer*,
             PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY / 2 / sizeof(RegistrerId)>
    m_RegistrersById;
    SmallMultimap<RegistrationStep, phantom::detail::_PHNTM_StaticGlobalRegistrer*,
                  PHANTOM_CUSTOM_REGISTRATION_STATIC_MEMORY / 2 /
                  sizeof(Pair<RegistrationStep, Registrer>)>
                            m_RegistrersByStep;
    RegistrationStep        m_CurrentRegistrationStep = RegistrationStep::_None;
    TypeInstallationStep    m_CurrentInstallationStep;
    SmallMap<hash64, Type*> m_HashToTypeMap;
    bool                    m_bInstalled;

    void              registerTypeByHash(uint64 a_Hash, reflection::Type* a_pType);
    reflection::Type* registeredTypeByHash(uint64 a_Hash);

    void addTypeInstallationInfos(TypeInstallationInfo* a_pTii);

private:
    friend class phantom::detail::DynamicCppInitializerH;
    static void _SetOnRequestMembersFunc(reflection::ClassType*   a_pType,
                                         TypeInstallationDelegate func);
};

} // namespace reflection
} // namespace phantom
