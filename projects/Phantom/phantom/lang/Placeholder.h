// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/ClassType.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief Represents a placeholder (in C++ for example it is used for 'auto' or template
/// parameters)
class PHANTOM_EXPORT_PHANTOM Placeholder
{
public:
    bool isSame(Placeholder* a_pOther) const
    {
        return asSymbol()->getHash() == a_pOther->asSymbol()->getHash();
    }

    virtual Symbol* asSymbol() const = 0;

    virtual Placeholder* asPlaceholder() const = 0;

    virtual PlaceholderType* asPlaceholderType() const
    {
        return nullptr;
    }

    virtual PlaceholderConstant* asPlaceholderConstant() const
    {
        return nullptr;
    }

    virtual PlaceholderClass* asPlaceholderClass() const
    {
        return nullptr;
    }

    virtual PlaceholderTemplate* asPlaceholderTemplate() const
    {
        return nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this placeholder accepts the given language element as final replacement.
    ///
    /// \param [in,out] a_pLanguageElement  The language element to test.
    ///
    /// \return true if it succeeds, false if it fails.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool accepts(LanguageElement*) const
    {
        return false;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Clones this placeholder.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual Placeholder* clone(uint a_Flags = 0) const = 0;
};

} // namespace lang
} // namespace phantom
