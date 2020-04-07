// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Placeholder.h>
#include <phantom/lang/Template.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Placeholder template.
/// \code{.cpp}
///             ex : template<template<typename> PlaceholderTemplate> ...
/// \endcode
class PHANTOM_EXPORT_PHANTOM PlaceholderTemplate : public Template, public Placeholder
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

public:
    PlaceholderTemplate(StringView a_strName, TemplateSignature* a_pSignature)
        : Template(a_pSignature, a_strName, 0, PHANTOM_R_FLAG_TEMPLATE_DEPENDANT)
    {
    }

    bool         accepts(LanguageElement* a_pLanguageElement) const override;
    virtual bool isA(Type*) const { return true; }

    Symbol*              asSymbol() const override { return (PlaceholderTemplate*)this; }
    Placeholder*         asPlaceholder() const override { return (PlaceholderTemplate*)this; }
    PlaceholderTemplate* asPlaceholderTemplate() const override { return (PlaceholderTemplate*)this; }

    Placeholder* clone(uint a_Flags = 0) const override;
};

} // namespace lang
} // namespace phantom
