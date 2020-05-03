// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Template.h"

#include "Application.h"
#include "Constant.h"
#include "Placeholder.h"
#include "Source.h"
#include "TemplateParameter.h"
#include "TemplateSignature.h"
#include "TemplateSpecialization.h"
#include "phantom/detail/core_internal.h"

/* *********************************************** */
#pragma warning(disable : 4996)

namespace phantom
{
namespace lang
{
Template::Template(Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*=0*/)
    : Symbol("", a_Modifiers,
             a_uiFlags | PHANTOM_R_ALWAYS_VALID) /// templates are considered always valid because if any error
                                                 /// is found in their declaration, it's of interest of their
                                                 /// empty specialization (it avoids contaminate every
                                                 /// specialization if one happens to be invalid)
{
}

Template::Template(TemplateSignature* a_pSignature, StringView a_strName, Modifiers a_Modifiers /*= 0*/,
                   uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags | PHANTOM_R_ALWAYS_VALID)
{
	PHANTOM_ASSERT(a_pSignature);
    createEmptyTemplateSpecialization(a_pSignature);
}

Template* Template::Parse(LanguageElement* a_pOwner, StringView a_strTemplateTypes, StringView a_strTemplateParam,
                          StringView a_strName, LanguageElement* a_pContextScope, Modifiers a_Modifiers /*= 0*/,
                          uint a_uiFlags /*= 0*/)
{
    return a_pOwner->NewDeferred<Template>(TemplateSignature::Parse(a_pOwner, a_strTemplateTypes,
                                                                           a_strTemplateParam, a_pContextScope,
                                                                           a_uiFlags & PHANTOM_R_FLAG_NATIVE),
                                                  a_strName, a_Modifiers, a_uiFlags);
}

Template::Template(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Symbol(a_strName, a_Modifiers, a_uiFlags | PHANTOM_R_ALWAYS_VALID)
{
}

Template::~Template() {}

Scope* Template::getScope() const
{
    return getOwner() ? getOwner()->asScope() : nullptr;
}

TemplateParameters const& Template::getTemplateParameters() const
{
    return getTemplateSignature()->getTemplateParameters();
}

size_t Template::getTemplateParameterIndex(StringView a_strName) const
{
    return getTemplateSignature()->getTemplateParameterIndex(a_strName);
}

size_t Template::getTemplateParameterIndex(TemplateParameter* a_pTemplateParameter) const
{
    return getTemplateSignature()->getTemplateParameterIndex(a_pTemplateParameter);
}

LanguageElement* Template::getDefaultArgument(StringView a_strParameterName) const
{
    return getTemplateSignature()->getDefaultArgument(a_strParameterName);
}

LanguageElement* Template::getDefaultArgument(size_t i) const
{
    return getTemplateSignature()->getDefaultArgument(i);
}

void Template::setDefaultArgument(StringView a_strParameterName, LanguageElement* a_pElement)
{
    getTemplateSignature()->setDefaultArgument(a_strParameterName, a_pElement);
}

void Template::setDefaultArgument(size_t a_uiIndex, LanguageElement* a_pElement)
{
    getTemplateSignature()->setDefaultArgument(a_uiIndex, a_pElement);
}

size_t Template::getDefaultArgumentCount() const
{
    return getTemplateSignature()->getDefaultArgumentCount();
}

void Template::addTemplateParameter(TemplateParameter* a_pTemplateParameter)
{
    getTemplateSignature()->addTemplateParameter(a_pTemplateParameter);
}

TemplateSignature* Template::getTemplateSignature() const
{
    return getEmptyTemplateSpecialization() ? getEmptyTemplateSpecialization()->getTemplateSignature() : nullptr;
}

void Template::addTemplateParameterAliasName(size_t a_uiIndex, StringView a_strAlias)
{
    getTemplateSignature()->addTemplateParameterAliasName(a_uiIndex, a_strAlias);
}

void Template::createEmptyTemplateSpecialization(TemplateSignature* a_pTemplateSignature)
{
	a_pTemplateSignature->NewDeferred<TemplateSpecialization>
    (this, a_pTemplateSignature, a_pTemplateSignature->getPlaceholders(), getFlags() & PHANTOM_R_FLAG_NATIVE);
}

TemplateSpecialization* Template::createEmptyTemplateSpecialization(TemplateSignature* a_pTemplateSignature,
                                                                    Symbol*            a_pBody)
{
    PHANTOM_ASSERT(a_pBody && !isNative());
    TemplateSpecialization* pSpec = a_pTemplateSignature->New<TemplateSpecialization>(
    this, a_pTemplateSignature, a_pTemplateSignature->getPlaceholders(), a_pBody, getFlags() & PHANTOM_R_FLAG_NATIVE);
    return pSpec;
}

TemplateSpecialization* Template::getTemplateSpecialization(TemplateSpecialization* a_pTemplateSpecialization) const
{
    for (TemplateSpecialization* pSpec : m_TemplateSpecializations)
    {
        if (pSpec->getSource() && pSpec->getSource()->getVisibility() == Visibility::Private)
            continue;
        if (pSpec->isSame(a_pTemplateSpecialization))
            return pSpec;
    }
    return nullptr;
}

TemplateSpecialization* Template::getTemplateSpecialization(LanguageElementsView a_Arguments) const
{
    for (TemplateSpecialization* pSpec : m_TemplateSpecializations)
    {
        if (pSpec->getSource() && pSpec->getSource()->getVisibility() == Visibility::Private)
            continue;
        if (pSpec->matches(a_Arguments))
            return pSpec;
    }
    return nullptr;
}

TemplateSpecialization* Template::getTemplateSpecialization(const PlaceholderMap& arguments) const
{
    LanguageElements contiguous = getEmptyTemplateSpecialization()->getArguments();
    for (auto it = arguments.begin(); it != arguments.end(); ++it)
    {
        contiguous[static_cast<TemplateParameter*>(it->first->asSymbol()->getOwner())->getIndex()] = it->second;
    }
    return getTemplateSpecialization(contiguous);
}

TemplateSpecialization* Template::getTemplateInstantiation(LanguageElementsView a_Arguments) const
{
    TemplateSignature* pSign = getTemplateSignature();
    size_t             paramCount = pSign->getTemplateParameters().size();
    if (a_Arguments.size() == paramCount)
        return getTemplateSpecialization(a_Arguments);

    if (a_Arguments.size() + pSign->getDefaultArgumentCount() < paramCount)
    {
        return nullptr;
    }

    LanguageElements newArgs = a_Arguments;
    while (newArgs.size() < paramCount)
    {
        newArgs.push_back(pSign->getDefaultArgument(newArgs.size()));
    }
    return getTemplateSpecialization(newArgs);
}

void Template::addTemplateSpecialization(TemplateSpecialization* a_pTemplateSpecialization)
{
    PHANTOM_ASSERT(a_pTemplateSpecialization && a_pTemplateSpecialization->getTemplate() == this);
    TemplateSpecialization* pEqual = getTemplateSpecialization(a_pTemplateSpecialization->getArguments());
    PHANTOM_ASSERT(!pEqual || pEqual->getModule() != a_pTemplateSpecialization->getModule());
    if (getNamespace()) /// template specialization belongs so same namespace as their template (even
                      /// if they can belong to different owners)
    {
        PHANTOM_ASSERT(a_pTemplateSpecialization->getNamespace() == nullptr);
    }
    m_TemplateSpecializations.push_back(a_pTemplateSpecialization);
    addReferencedElement(a_pTemplateSpecialization);
}

TemplateSpecialization* Template::createTemplateSpecialization(const LanguageElements& arguments,
                                                               ClassType*              a_pTemplated,
                                                               TemplateSignature*      a_pTemplateSignature)
{
    if (a_pTemplateSignature == nullptr)
        a_pTemplateSignature = New<TemplateSignature>(isNative() * PHANTOM_R_FLAG_NATIVE);
    TemplateSpecialization* pSpec = a_pTemplated
    ? a_pTemplateSignature->NewDeferred<TemplateSpecialization>(this, a_pTemplateSignature, arguments, a_pTemplated,
                                                   getFlags() & PHANTOM_R_FLAG_NATIVE)
    : a_pTemplateSignature->NewDeferred<TemplateSpecialization>(this, a_pTemplateSignature, arguments,
                                                   getFlags() & PHANTOM_R_FLAG_NATIVE);
    return pSpec;
}

bool Template::acceptsArguments(const LanguageElements& a_Arguments) const
{
    return getTemplateSignature()->acceptsArguments(a_Arguments);
}

bool Template::mapArguments(const LanguageElements& a_Arguments, PlaceholderMap& a_Out) const
{
    size_t i = 0;
    size_t count = getTemplateSignature()->getTemplateParameters().size();
    for (; i < count; ++i)
    {
        if (i < a_Arguments.size())
        {
            a_Out[getTemplateSignature()->getTemplateParameters()[i]->getPlaceholder()] = a_Arguments[i];
        }
        else
        {
            a_Out[getTemplateSignature()->getTemplateParameters()[i]->getPlaceholder()] = getDefaultArgument(i);
        }
    }
    return true;
}

bool Template::isVariadic() const
{
    return getTemplateSignature()->isVariadic();
}

static String Template_removeParens(const char* arg)
{
    char c;
    while ((c = *arg) == ' ' || (c == '\t') || (c == '('))
        arg++;
    String str = arg;
    while ((c = str.back()) == ' ' || (c == '\t') || (c == ')'))
        str.pop_back();
    return str;
}

static String Template_catSplittedTemplate(const char* arg, const char**& a_rpNext)
{
    String str;
    int    templateLevel = 0;
    do
    {
        if (str.size())
            str += ',';
        str += arg;
        char c;
        while ((c = *arg++))
        {
            switch (c)
            {
            case '<':
                templateLevel++;
                break;
            case '>':
                templateLevel--;
                break;
            }
        }
    } while (templateLevel && (arg = *a_rpNext++));
    return str;
}

void Template::setNativeDefaultArgumentStrings(const char** a_ppArgs, LanguageElement* /*= nullptr*/)
{
    if (getTemplateParameters().back()->getDefaultArgument() != nullptr)
        return;
    if (*a_ppArgs == 0)
        return;
    if (strcmp(*a_ppArgs, "_") == 0)
        return;

    auto&        tplParms = getTemplateParameters();
    const char*  arg;
    const char** args = a_ppArgs;
    Strings      str_args;
    while ((arg = *args++))
    {
        str_args.push_back(Template_catSplittedTemplate(arg, args));
    }
    size_t defaultArgCount = str_args.size();
    size_t paramCount = getTemplateParameters().size();
    PHANTOM_ASSERT(defaultArgCount <= paramCount, "too many template default arguments");
    for (size_t i = 0; i < defaultArgCount; ++i)
    {
        size_t paramIndex = paramCount - defaultArgCount + i;
#if defined(PHANTOM_DEV)
#    pragma message(PHANTOM_TODO "remove default argument from native template")
#endif
        if (getDefaultArgument(paramIndex))
            continue;
        /// remove parens from protected macro arguments
        String strArg = str_args[i];
        strArg = Template_removeParens(strArg.c_str());

        Constant* pC = nullptr;
        int       num = 0;
        if (strArg == "true")
        {
            pC = Constant::Create(this, true);
        }
        else if (strArg == "false")
        {
            pC = Constant::Create(this, false);
        }
        else if (sscanf(strArg.c_str(), "%d", &num) == 1)
        {
            pC = Constant::Create(this, num);
        }

        if (pC)
        {
            if (testFlags(PHANTOM_R_FLAG_NATIVE))
                pC->setFlag(PHANTOM_R_FLAG_NATIVE);
            setDefaultArgument(paramIndex, pC);
            continue;
        }

        bool foundAsParm = false;
        for (size_t pi = 0; pi < paramIndex; ++pi)
        {
            if (tplParms[pi]->getName() == strArg)
            {
                foundAsParm = true;
                setDefaultArgument(paramIndex, tplParms[pi]->getPlaceholder()->asSymbol());
                break;
            }
        }

        if (foundAsParm)
            continue;

        LanguageElement* pElement = Application::Get()->findCppType(strArg, getTemplateSignature());
        if (pElement == nullptr)
        {
            pElement = Application::Get()->findCppSymbol(strArg, getTemplateSignature());
            if (pElement)
            {
                if (Constant* pConst = pElement->asConstant())
                {
                    setDefaultArgument(paramIndex, pConst);
                    continue;
                }
            }
        }
        else
        {
            setDefaultArgument(paramIndex, pElement);
            continue;
        }
        PHANTOM_WARNING(false, "cannot solve template default argument '%s'", strArg.c_str());
    }
}

} // namespace lang
} // namespace phantom
