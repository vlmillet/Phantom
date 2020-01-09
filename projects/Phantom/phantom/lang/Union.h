// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/ClassType.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace reflection
{
/// \brief  Represents an union type.
class PHANTOM_EXPORT_PHANTOM Union : public ClassType
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(Union);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an union with given name and optional modifiers.
    ///
    /// \param  a_strName   The name.
    /// \param  a_Modifiers (optional) the modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Union(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    PHANTOM_DTOR ~Union() override
    {
    }

    Union* asUnion() const override
    {
        return (Union*)this;
    }

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a native union with given name, size, alignment and optional modifiers.
    ///
    /// \param  a_strName       The name.
    /// \param  a_uiSize        The size.
    /// \param  a_uiAlignment   The alignment.
    /// \param  a_Modifiers     (optional) the modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Union(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers,
          uint a_uiFlags)
        : ClassType(TypeKind::Union, a_strName, a_uiSize, a_uiAlignment, a_Modifiers,
                    a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }
};

} // namespace reflection
} // namespace phantom
