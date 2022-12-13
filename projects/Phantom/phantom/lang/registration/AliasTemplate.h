// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "GlobalRegistrer.h"
#include "Namespace.h"

#include <phantom/lang/Alias.h>
#include <phantom/lang/Application.h>
#include <phantom/lang/Namespace.h>
#include <phantom/lang/Template.h>
#include <phantom/lang/TemplateSignature.h>
#include <phantom/lang/TemplateSpecialization.h>

#define _PHNTM_POST_ALIAS_TEMPLATE(...) PHANTOM_PP_VARARG_0_X(_PHNTM_POST_ALIAS_TEMPLATE, ##__VA_ARGS__)
#define _PHNTM_POST_ALIAS_TEMPLATE0(...)                                                                               \
    _PHNTM_AlTpl->setMetaData __VA_ARGS__;                                                                             \
    }
#define _PHNTM_POST_ALIAS_TEMPLATEX(...)                                                                               \
    _PHNTM_AlTpl->setMetaData __VA_ARGS__;                                                                             \
    }

#define PHANTOM_ALIAS_TEMPLATE(...) PHANTOM_PP_VARARG(_PHANTOM_ALIAS_TEMPLATE_, ##__VA_ARGS__)

#define PHANTOM_ALIAS_TEMPLATE_V(...) PHANTOM_PP_VARARG(_PHANTOM_ALIAS_TEMPLATE_V_, ##__VA_ARGS__)

namespace phantom
{
namespace lang
{
PHANTOM_EXPORT_PHANTOM void SolveAliasTemplateDefaultArguments(TemplateSignature* a_pTS, StringView a_Defaults);

PHANTOM_EXPORT_PHANTOM Alias* BuildAliasTemplate(StringView& a_TemplateDep, Template*& a_rpTemplate, Source* a_pSource,
                                                 StringView a_Name);

template<class T>
void SolveAliasTemplate(Source* a_pSource, RegistrationStep a_Step, Template*& a_rpTemplate, T& a_Builder,
                        StringView a_TemplateTypes, StringView a_TemplateParams, StringView a_Name,
                        StringView a_TemplateDep, StringView a_Defaults)
{
    if (a_Step == RegistrationStep::Typedefs)
    {
        TemplateSignature* pTS = TemplateSignature::Parse(a_pSource, a_TemplateTypes, a_TemplateParams,
                                                          a_Builder._PHNTM_getMeta(), PHANTOM_R_FLAG_NATIVE);
        PHANTOM_ASSERT(pTS, "cannot resolved template signature %.*s %.*s",
                       PHANTOM_STRING_AS_PRINTF_ARG(a_TemplateTypes), PHANTOM_STRING_AS_PRINTF_ARG(a_TemplateParams));

        a_rpTemplate = a_pSource->New<Template>(pTS, a_Name, PHANTOM_R_NONE, PHANTOM_R_FLAG_NATIVE);

        a_Builder._PHNTM_getMeta()->addTemplate(a_rpTemplate);
        a_Builder._PHNTM_getSource()->addTemplate(a_rpTemplate);
        a_Builder._PHNTM_getSource()->addTemplateSpecialization(a_rpTemplate->getEmptyTemplateSpecialization());
    }
    else // PostTypes
    {
        SolveAliasTemplateDefaultArguments(a_rpTemplate->getTemplateSignature(), a_Defaults);
        auto pAlias = BuildAliasTemplate(a_TemplateDep, a_rpTemplate, a_pSource, a_Name);
        //         if (std::is_same<T, NamespaceBuilder>::value)
        //             ((Namespace*)a_Builder._PHNTM_getNamingScope())->addAlias(pAlias);
        a_rpTemplate->getEmptyTemplateSpecialization()->setTemplated(pAlias);
    }
    a_Builder._PHNTM_getRegistrer()->_PHNTM_setLastSymbol(a_rpTemplate);
}
} // namespace lang
} // namespace phantom
#define _PHANTOM_ALIAS_TEMPLATE_5(TemplateTypes, TemplateParams, Defaults, Name, Aliased)                              \
    PHANTOM_REGISTER(Typedefs, PostTypes)                                                                              \
    {                                                                                                                  \
        static phantom::lang::Template* pAliasT = nullptr;                                                             \
        SolveAliasTemplate(namespace_()._PHNTM_getSource(), PHANTOM_REGISTRATION_STEP, pAliasT, this_(),               \
                           PHANTOM_PP_QUOTE TemplateTypes, PHANTOM_PP_QUOTE TemplateParams, PHANTOM_PP_QUOTE(Name),    \
                           PHANTOM_PP_IDENTITY(PHANTOM_PP_QUOTE)(PHANTOM_PP_REMOVE_PARENS(Aliased)),                   \
                           PHANTOM_PP_QUOTE Defaults);                                                                 \
    }                                                                                                                  \
    _PHNTM_PRE_TRAILING_METADATA _PHNTM_TRAILING_METADATA_COUNTER_MINUS_1

#define _PHANTOM_ALIAS_TEMPLATE_4(TemplateTypes, TemplateParams, Name, Aliased)                                        \
    _PHANTOM_ALIAS_TEMPLATE_5(TemplateTypes, TemplateParams, ($), Name, Aliased)

#define _PHANTOM_ALIAS_TEMPLATE_V_3(VariadicPair, Name, Aliased)                                                       \
    PHANTOM_REGISTER(Typedefs, PostTypes)                                                                              \
    {                                                                                                                  \
        static phantom::lang::Template* pAliasT = nullptr;                                                             \
        SolveAliasTemplate(namespace_()._PHNTM_getSource(), PHANTOM_REGISTRATION_STEP, pAliasT, this_(),               \
                           PHANTOM_PP_QUOTE_ARG_0 VariadicPair "...", PHANTOM_PP_QUOTE_ARG_1 VariadicPair,             \
                           PHANTOM_PP_QUOTE(Name),                                                                     \
                           PHANTOM_PP_IDENTITY(PHANTOM_PP_QUOTE)(PHANTOM_PP_REMOVE_PARENS(Aliased)), "$");             \
    }                                                                                                                  \
    _PHNTM_PRE_TRAILING_METADATA _PHNTM_TRAILING_METADATA_COUNTER_MINUS_1
