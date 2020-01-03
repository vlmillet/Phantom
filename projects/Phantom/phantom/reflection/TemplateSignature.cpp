// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "TemplateSignature.h"

#include "PlaceholderConstant.h"
#include "PlaceholderTemplate.h"
#include "PlaceholderType.h"
#include "TemplateParameter.h"
#ifndef __DOXYGEN__
#    include <phantom/utils/StringUtil.h>
#endif
#include "Application.h"
#include "Namespace.h"

#include <phantom/detail/new.h>
#include <phantom/detail/phantom_priv.h>
/* *********************************************** */
namespace phantom
{
namespace reflection
{
TemplateSignature* TemplateSignature::Create(uint a_Flags)
{
    return PHANTOM_DEFERRED_NEW(TemplateSignature)(a_Flags);
}

TemplateSignature::TemplateSignature(uint a_uiFlags) : LanguageElement(a_uiFlags)
{
}

TemplateSignature::TemplateSignature(const TemplateParameters& a_Parameters, bool a_bVariadic,
                                     uint flags /*= PHANTOM_R_NONE*/)
    : LanguageElement(flags)
{
    for (auto it = a_Parameters.begin(); it != a_Parameters.end(); ++it)
    {
        addTemplateParameter(*it);
    }
    if (m_TemplateParameters.size())
        m_TemplateParameters.back()->m_bPack = a_bVariadic;
}

TemplateSignature* TemplateSignature::Parse(StringView a_strTemplateParameterTypes,
                                            StringView a_strTemplateParameterNames, LanguageElement* a_pContextScope,
                                            uint a_uiFlags /*= PHANTOM_R_NONE*/)
{
    if (a_pContextScope == nullptr)
        a_pContextScope = Namespace::Global();

    struct Param
    {
        String name;
        Type*  valueType;
    };
    SmallVector<Param> parsedParams;
    Types              templateTypes;
    Strings            types;
    StringUtil::Split(types, a_strTemplateParameterTypes, ",");
    Strings names;
    StringUtil::Split(names, a_strTemplateParameterNames, ",");
    PHANTOM_ASSERT(types.size() == names.size());
    bool bVariadic = false;
    for (size_t i = 0; i < types.size(); ++i)
    {
        StringUtil::ReplaceAll(names[i], " ", "");
        size_t pos;
        if ((pos = types[i].find("...")) == (types[i].size() - 3))
        {
            PHANTOM_ASSERT(i == (types.size() - 1), "template parameter pack must be the last parameter");
            bVariadic = true;
            types[i] = types[i].substr(0, pos);
        }
        if (types[i].find("typename") != String::npos OR types[i].find("class") != String::npos)
        {
            StringUtil::ReplaceAll(types[i], " ", "");
            if (types[i] == "typename" OR types[i] == "class")
            {
                parsedParams.push_back(Param{names[i], nullptr});
                continue;
            }
            goto else_;
        }
        else
        {
            StringUtil::ReplaceAll(types[i], " ", "");
        else_:
            Type* pType = Application::Get()->findCppType(types[i], a_pContextScope);
            if (pType == nullptr || pType->asIntegralType() == nullptr)
                return nullptr;
            parsedParams.push_back(Param{names[i], pType});
        }
    }

    TemplateParameters tParams;
    for (auto const& pram : parsedParams)
    {
        tParams.push_back(PHANTOM_DEFERRED_NEW(TemplateParameter)(
        pram.valueType ? static_cast<Placeholder*>(PHANTOM_DEFERRED_NEW(PlaceholderConstant)(
                         pram.valueType, pram.name, PHANTOM_R_NONE, a_uiFlags & PHANTOM_R_FLAG_NATIVE))
                       : static_cast<Placeholder*>(PHANTOM_DEFERRED_NEW(PlaceholderType)(
                         pram.name, PHANTOM_R_NONE, a_uiFlags & PHANTOM_R_FLAG_NATIVE)),
        nullptr, PHANTOM_R_NONE, a_uiFlags & PHANTOM_R_FLAG_NATIVE));
    }

    return PHANTOM_DEFERRED_NEW(TemplateSignature)(tParams, bVariadic, a_uiFlags);
}

TemplateSignature::~TemplateSignature()
{
}

TemplateParameter* TemplateSignature::addTemplateValueParameter(Type* a_pType, StringView a_Name)
{
    TemplateParameter* pParam = PHANTOM_NEW(TemplateParameter)(PHANTOM_NEW(PlaceholderConstant)(a_pType, a_Name));
    addTemplateParameter(pParam);
    return pParam;
}

TemplateParameter* TemplateSignature::addTemplateTypeParameter(StringView a_Name)
{
    TemplateParameter* pParam = PHANTOM_NEW(TemplateParameter)(PHANTOM_NEW(PlaceholderType)(a_Name));
    addTemplateParameter(pParam);
    return pParam;
}

TemplateParameter* TemplateSignature::addTemplateTemplateParameter(TemplateSignature* a_pTemplateSignature,
                                                                   StringView         a_Name)
{
    TemplateParameter* pParam =
    PHANTOM_NEW(TemplateParameter)(PHANTOM_NEW(PlaceholderTemplate)(a_Name, a_pTemplateSignature));
    addTemplateParameter(pParam);
    return pParam;
}

size_t TemplateSignature::getTemplateParameterIndex(StringView a_strName) const
{
    for (size_t i = 0; i < m_TemplateParameters.size(); ++i)
    {
        if (m_TemplateParameters[i]->getName() == a_strName)
            return i;
    }
    auto found = m_TemplateParameterAliasNames.find(a_strName);
    return found != m_TemplateParameterAliasNames.end() ? found->second : ~size_t(0);
}

size_t TemplateSignature::getTemplateParameterIndex(TemplateParameter* a_pParameter) const
{
    for (size_t i = 0; i < m_TemplateParameters.size(); ++i)
    {
        if (m_TemplateParameters[i] == a_pParameter)
            return i;
    }
    return ~size_t(0);
}

size_t TemplateSignature::getTemplateParameterIndex(Placeholder* a_pPlacelholder) const
{
    for (size_t i = 0; i < m_TemplateParameters.size(); ++i)
    {
        if (m_TemplateParameters[i]->getPlaceholder() == a_pPlacelholder)
            return i;
    }
    return ~size_t(0);
}

LanguageElement* TemplateSignature::getDefaultArgument(StringView a_strParameterName) const
{
    size_t index = getTemplateParameterIndex(a_strParameterName);
    PHANTOM_ASSERT(index != ~size_t(0));
    return m_TemplateParameters[index]->getDefaultArgument();
}

LanguageElement* TemplateSignature::getDefaultArgument(size_t i) const
{
    PHANTOM_ASSERT(i < m_TemplateParameters.size());
    return m_TemplateParameters[i]->getDefaultArgument();
}

void TemplateSignature::setDefaultArgument(StringView a_strParameterName, LanguageElement* a_pElement)
{
    size_t index = getTemplateParameterIndex(a_strParameterName);
    setDefaultArgument(index, a_pElement);
}

void TemplateSignature::setDefaultArgument(size_t a_uiIndex, LanguageElement* a_pElement)
{
    PHANTOM_ASSERT(a_uiIndex != ~size_t(0));
    m_TemplateParameters[a_uiIndex]->setDefaultArgument(a_pElement);
}

size_t TemplateSignature::getDefaultArgumentCount() const
{
    size_t i = m_TemplateParameters.size();
    size_t count = 0;
    while (i--)
    {
        if (m_TemplateParameters[i]->getDefaultArgument() == nullptr)
            return count;
        count++;
    }
    return count;
}

void TemplateSignature::addTemplateParameter(TemplateParameter* a_pTemplateParameter)
{
    PHANTOM_ASSERT(!isVariadic(), "variadic template signature cannot have more template parameters");
    PHANTOM_ASSERT(a_pTemplateParameter);
    PHANTOM_ASSERT(getTemplateParameterIndex(a_pTemplateParameter->getName()) == ~size_t(0),
                   "template parameter with this name already exists in this template signature");
    addElement(a_pTemplateParameter);
    m_TemplateParameters.push_back(a_pTemplateParameter);
    Symbol* pPH =
    (a_pTemplateParameter->getPlaceholder()) ? a_pTemplateParameter->getPlaceholder()->asSymbol() : nullptr;
    m_Placeholders.push_back(pPH);
    if (pPH)
    {
        addReferencedElement(pPH);
    }
}

void TemplateSignature::addTemplateParameterAliasName(size_t a_uiIndex, StringView a_strAlias)
{
    size_t existingIndex = getTemplateParameterIndex(a_strAlias);
    if (existingIndex == ~size_t(0))
    {
        m_TemplateParameterAliasNames[a_strAlias] = a_uiIndex;
    }
    else if (existingIndex != a_uiIndex)
    {
        PHANTOM_ASSERT(false, "parameter alias already used by another parameter");
    }
}

bool TemplateSignature::acceptsArguments(const LanguageElements& a_Arguments) const
{
    if (getTemplateParameters().size() != a_Arguments.size())
        return false;
    for (size_t i = 0; i < m_TemplateParameters.size(); ++i)
    {
        if (a_Arguments[i] == nullptr OR m_TemplateParameters[i] ==
            nullptr OR                   NOT(m_TemplateParameters[i]->acceptsArgument(a_Arguments[i])))
            return false;
    }
    return true;
}

bool TemplateSignature::isSame(TemplateSignature* a_pOther) const
{
    size_t count = m_TemplateParameters.size();
    if (count != a_pOther->m_TemplateParameters.size())
        return false;
    for (size_t i = 0; i < count; ++i)
    {
        if (NOT(m_TemplateParameters[i]->isSame(a_pOther->m_TemplateParameters[i])))
            return false;
    }
    return true;
}

bool TemplateSignature::isSame(Symbol* a_pLanguageElement) const
{
    TemplateSignature* pTS = a_pLanguageElement->asTemplateSignature();
    return pTS AND     isSame(pTS);
}

bool TemplateSignature::isVariadic() const
{
    return m_TemplateParameters.size() AND m_TemplateParameters.back()->isPack();
}

void TemplateSignature::setVariadic(bool a_bValue)
{
    PHANTOM_ASSERT(a_bValue != isVariadic() && m_TemplateParameters.size());
    m_TemplateParameters.back()->m_bPack = a_bValue;
}

TemplateSignature* TemplateSignature::clone(uint a_Flags) const
{
    TemplateParameters params(m_TemplateParameters.size());
    for (size_t i = 0; i < m_TemplateParameters.size(); ++i)
    {
        params[i] = m_TemplateParameters[i]->clone(a_Flags);
    }
    return PHANTOM_NEW(TemplateSignature)(params, isVariadic(), a_Flags);
}

} // namespace reflection
} // namespace phantom
