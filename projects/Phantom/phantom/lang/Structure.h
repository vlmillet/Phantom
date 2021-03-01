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
////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief  Represents an extended POD class with user defined constructors (classic POD forbid
///         user defined constructors). They are designed for initialized row data structs. Any
///         class declared as structure to Phantom will never have its destructor called. Use
///         this to optimize your data flow and data-oriented paradigm (a common use is for
///         initialized math structures such as quaternion, matrix, etc...). It is the
///         responsability of the user to avoid any heap allocation which would require
///         destructor inside their defined constructors.
////////////////////////////////////////////////////////////////////////////////////////////////////

class PHANTOM_EXPORT_PHANTOM Structure : public ClassType
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(Structure);

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructor for native implementations.
    ///
    /// \param  a_strName       The name.
    /// \param  a_uiSize        The size.
    /// \param  a_uiAlignment   The alignment.
    /// \param  a_Modifiers     (optional) the modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Structure(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers, uint a_uiFlags);

public:
    Structure(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);
    void initialize();

    Type*      asPOD() const override { return (Structure*)this; }
    Structure* asStructure() const override { return (Structure*)this; }

    bool isCopyable() const override { return true; }

    void copyConstruct(void* a_pDest, void const* a_pSrc) const override { memcpy(a_pDest, a_pSrc, getSize()); }
};

} // namespace lang
} // namespace phantom
