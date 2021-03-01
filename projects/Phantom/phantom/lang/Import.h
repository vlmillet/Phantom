// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Symbol.h>
/* **************** Declarations ***************** */

/* *********************************************** */
namespace phantom
{
class Phantom;
}

namespace phantom
{
namespace lang
{
/// \brief  Represents an import directive.
class PHANTOM_EXPORT_PHANTOM Import : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Import);

    friend class Source;

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an import directive from a source and .
    ///
    /// \param [in,out] a_pSource   If non-null, source for the.
    /// \param  a_Symbol            The symbol.
    /// \param  a_Modifiers         (optional) the modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Import(Source* a_pSource, StringView a_strSymbolName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    void initialize()
    {
        Symbol::initialize();
        addReferencedElement(m_pImportedSource);
    }

    virtual Import* asImport() const { return (Import*)this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the imported source.
    ///
    /// \return null if it fails, else the imported source.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Source* getImportedSource() const { return m_pImportedSource; }

public:
    Source* m_pImportedSource;
};

} // namespace lang
} // namespace phantom
