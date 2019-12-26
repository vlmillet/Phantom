// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/StlVectorClass.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace reflection
{
/// \brief  Base base for stl compliant String classes.
class PHANTOM_EXPORT_PHANTOM StringClass : public StlVectorClass
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(StringClass);

protected:
    StringClass(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers,
                uint a_uiFlags);

public:
    StringClass(StringView a_strName, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ~StringClass() override
    {
    }

    virtual StringClass* asStringClass() const
    {
        return const_cast<StringClass*>(this);
    }

    virtual void const* c_str(void const* a_pString) const = 0;

    virtual void assign(void* a_pString, const void* a_pChars, size_t a_Len) const = 0;
};

} // namespace reflection
} // namespace phantom
