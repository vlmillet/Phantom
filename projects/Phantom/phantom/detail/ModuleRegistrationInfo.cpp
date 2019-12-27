// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "ModuleRegistrationInfo.h"

#include <phantom/detail/StaticGlobalRegistrer.h>
#include <phantom/detail/phantom_priv.h>
#include <phantom/reflection/ClassType.h>
#include <phantom/reflection/Module.h>
#include <phantom/reflection/registration/registration.h>

// #define PHANTOM_CUSTOM_PLUGIN_ALWAYS_REGISTER_CLASS_MEMBERS

namespace phantom
{
namespace reflection
{
ModuleRegistrationInfo::ModuleRegistrationInfo(size_t a_ModuleHandle)
    : m_uiFlags(0),
      m_ModuleHandle(a_ModuleHandle),
      m_pModule(nullptr),
      m_OnLoad(0),
      m_OnUnload(0),
      m_CurrentInstallationStep(TypeInstallationStep::Uninstalled),
      m_HashToTypeMap(MemoryTraits::Default()),
      m_bInstalled(false)
{
}

RegistrerId ModuleRegistrationInfo::addRegistrer(ArrayView<RegistrationStep>                    a_RegistrationSteps,
                                                 phantom::detail::_PHNTM_StaticGlobalRegistrer* a_pRegistrer)
{
    RegistrerId id = dynamic_initializer_()->makeRegistrerKey(a_pRegistrer->_PHNTM_file, a_pRegistrer->_PHNTM_line,
                                                              a_pRegistrer->_PHNTM_tag, false);
    PHANTOM_ASSERT(a_pRegistrer);
    auto found = m_RegistrersById.find(id);
    if (found != m_RegistrersById.end())
        return 0;
    m_RegistrersById.insert(makePair(id, a_pRegistrer));
    for (auto step : a_RegistrationSteps)
    {
        if (step <= m_CurrentRegistrationStep)
        // catch up with previous and current steps
        {
            a_pRegistrer->__PHNTM_process(step);
        }
        else
        // register for incoming future steps
        {
            m_RegistrersByStep[step] = a_pRegistrer;
        }
    }
    return id;
}

phantom::detail::_PHNTM_StaticGlobalRegistrer* ModuleRegistrationInfo::findRegistrer(RegistrerId a_UniqueId) const
{
    auto found = m_RegistrersById.find(a_UniqueId);
    if (found != m_RegistrersById.end())
        return found->second;
    return nullptr;
}

phantom::detail::_PHNTM_StaticGlobalRegistrer* ModuleRegistrationInfo::findRegistrer(StringView a_File, int a_Line,
                                                                                     int a_Tag) const
{
    RegistrerId id = dynamic_initializer_()->makeRegistrerKey(a_File, a_Line, a_Tag, false);
    return findRegistrer(id);
}

void ModuleRegistrationInfo::removeRegistrer(RegistrerId a_UniqueId)
{
    auto found = m_RegistrersById.find(a_UniqueId);
    PHANTOM_ASSERT(found != m_RegistrersById.end());
    m_RegistrersById.erase(found);
}

void ModuleRegistrationInfo::setModule(reflection::Module* a_pModule)
{
    m_pModule = a_pModule;
    m_pModule->m_HashToTypeMap = std::move(m_HashToTypeMap);
    m_HashToTypeMap.clear();
}

void ModuleRegistrationInfo::registerTypeByHash(uint64 a_Hash, reflection::Type* a_pType)
{
    if (m_pModule == nullptr)
    {
        reflection::Type*& pType = m_HashToTypeMap[a_Hash];
        PHANTOM_ASSERT(pType == nullptr);
        pType = a_pType;
        return;
    }
    m_pModule->_registerType(a_Hash, a_pType);
}

reflection::Type* ModuleRegistrationInfo::registeredTypeByHash(uint64 a_Hash)
{
    if (m_pModule == nullptr)
    {
        auto found = m_HashToTypeMap.find(a_Hash);
        return found == m_HashToTypeMap.end() ? nullptr : found->second;
    }
    return m_pModule->findUsableType(a_Hash);
}

void ModuleRegistrationInfo::addTypeInstallationInfos(TypeInstallationInfo* a_pTii)
{
    registerTypeByHash(a_pTii->type->getHash(), a_pTii->type);
    m_TypeInstallationInfos.push_back(a_pTii);
    /// apply installation steps until the current one
    int i = int(TypeInstallationStep::TemplateSignature);
    for (; i <= int(m_CurrentInstallationStep); ++i)
    {
        if (a_pTii->type->getTypeKind() != reflection::TypeKind::Enum)
        {
            if (i == int(TypeInstallationStep::TemplateSignature))
            {
                ModuleRegistrationInfo::_SetOnRequestMembersFunc(
                static_cast<reflection::ClassType*>(a_pTii->type),
                TypeInstallationDelegate(a_pTii, &TypeInstallationInfo::exec));
            }
            if (i == int(TypeInstallationStep::Members))
            {
#if !defined(PHANTOM_CUSTOM_PLUGIN_ALWAYS_REGISTER_CLASS_MEMBERS)
                if ((m_uiFlags & PHANTOM_PLUGIN_REGISTER_CLASS_MEMBERS_ON_ACCESS) == 0)
#endif
                {
                    a_pTii->exec((TypeInstallationStep)i);
                }
                a_pTii->installSymbolExtenders();
            }
            else
                a_pTii->exec((TypeInstallationStep)i);
        }
    }
}

void ModuleRegistrationInfo::installTypes(TypeInstallationStep step)
{
    PHANTOM_ASSERT(m_CurrentInstallationStep < step);
    m_CurrentInstallationStep = step;
    if (step == TypeInstallationStep::TemplateSignature)
    {
        size_t count =
        m_TypeInstallationInfos.size(); // we handle a fixed amount of installation even if new appear during the loop
        for (size_t i = 0; i < count; ++i)
        {
            TypeInstallationInfo* pTii = m_TypeInstallationInfos[i];
            if (pTii->type->getTypeKind() != reflection::TypeKind::Enum)
            {
                PHANTOM_ASSERT(pTii->type->asClassType());
                _SetOnRequestMembersFunc(static_cast<reflection::ClassType*>(pTii->type),
                                         TypeInstallationDelegate(pTii, &TypeInstallationInfo::exec));
            }
        }
    }
    if (step == TypeInstallationStep::Members)
    {
#if !defined(PHANTOM_CUSTOM_PLUGIN_ALWAYS_REGISTER_CLASS_MEMBERS)
        if (((m_uiFlags & PHANTOM_PLUGIN_REGISTER_CLASS_MEMBERS_ON_ACCESS) == 0))
#endif
        {
            size_t count = m_TypeInstallationInfos
                           .size(); // we handle a fixed amount of installation even if new appear during the loop
            for (size_t i = 0; i < count; ++i)
            {
                TypeInstallationInfo* pTii = m_TypeInstallationInfos[i];
                if (pTii->type->getTypeKind() != reflection::TypeKind::Enum)
                {
                    struct FakeClassType : public reflection::ClassType
                    {
                    public:
                        using ClassType::_onElementsAccess;
                    };
                    static_cast<FakeClassType*>(pTii->type)->_onElementsAccess();
                }
                else
                    pTii->exec(step);
            }
        }
        for (TypeInstallationInfo* pTii : m_TypeInstallationInfos)
        {
            pTii->installSymbolExtenders();
        }
    }
    else
    {
        size_t count =
        m_TypeInstallationInfos.size(); // we handle a fixed amount of installation even if new appear during the loop
        for (size_t i = 0; i < count; ++i)
        {
            m_TypeInstallationInfos[i]->exec(step);
        }
    }
}
void ModuleRegistrationInfo::stepTypeInstallation(reflection::Type* a_pType)
{
    if (m_bInstalled OR m_CurrentInstallationStep == TypeInstallationStep::Uninstalled)
        return;
    size_t count =
    m_TypeInstallationInfos.size(); // we handle a fixed amount of installation even if new appear during the loop
    for (size_t i = 0; i < count; ++i)
    {
        TypeInstallationInfo* pTii = m_TypeInstallationInfos[i];
        if (pTii->type == a_pType)
        {
            int step = int(TypeInstallationStep::TemplateSignature);
            for (; step <= int(m_CurrentInstallationStep); ++step)
            {
                pTii->exec((TypeInstallationStep)step);
            }
            break;
        }
    }
}

void ModuleRegistrationInfo::processRegistration()
{
    for (RegistrationStep i = RegistrationStep::End; i <= RegistrationStep::End; reinterpret_cast<int&>(i)++)
    {
        stepRegistration(i);
    }
}

void ModuleRegistrationInfo::stepRegistration(RegistrationStep a_Step)
{
    PHANTOM_ASSERT(int(m_CurrentRegistrationStep) < int(a_Step));
    m_CurrentRegistrationStep = a_Step;
    for (ptrdiff_t i = 0; i < m_RegistrersByStep.upper_bound(a_Step) - m_RegistrersByStep.lower_bound(a_Step); ++i)
    {
        m_RegistrersByStep.lower_bound(a_Step)[i].second->__PHNTM_process(a_Step);
    }
    m_RegistrersByStep.erase(a_Step);
}

TypeInstallationInfo::TypeInstallationInfo(reflection::Type* a_pType, reflection::Source* a_pSource,
                                           TypeInstallFunc a_setupFunc)
    : installFunc(a_setupFunc), type(a_pType), m_pSource(a_pSource), steps(uint(TypeInstallationStep::Uninstalled))
{
}

void TypeInstallationInfo::exec(TypeInstallationStep a_Step)
{
    PHANTOM_ASSERT(a_Step != TypeInstallationStep::Uninstalled);
    if (steps & (1 << int(a_Step)))
        return;
    steps |= (1 << int(a_Step));
    reflection::Source* pSource = type->getSource();

    if (pSource == nullptr)
    // on the fly templates come here :
    {
        if (m_pSource)
            pSource = m_pSource;
        else
            pSource = phantom::reflection::detail::currentSource();
    }
    if (pSource)
        phantom::reflection::detail::pushSource(pSource);
    installFunc(type, a_Step);
    if (pSource)
        phantom::reflection::detail::popSource();
}

void TypeInstallationInfo::installSymbolExtenders()
{
}

void ModuleRegistrationInfo::_SetOnRequestMembersFunc(reflection::ClassType* a_pType, TypeInstallationDelegate func)
{
    PHANTOM_ASSERT(a_pType->m_OnDemandMembersFunc == nullptr,
                   "Type with same name already registered in the same module, please check that "
                   "you don't have duplicate reflection declaration or wrong .hxx include");
    a_pType->m_OnDemandMembersFunc = func;
}

} // namespace reflection

} // namespace phantom
