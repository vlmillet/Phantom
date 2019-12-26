// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/Method.h>
#include <phantom/reflection/Type.h>
/* *********************************************** */

namespace phantom
{
namespace reflection
{
/// \brief  Represents a constructor member function.
class PHANTOM_EXPORT_PHANTOM Constructor : public Method
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

public:
    template<typename t_Ty, bool t_is_default_constructible>
    friend struct phantom::reflection::detail::VTableSizeOfDefaultConstructibleH;

    friend class Class;

    PHANTOM_DECLARE_META_CLASS(Constructor);

public:
    Constructor();
    Constructor(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a single instance at a given place address.
    ///
    /// \param [in,out] a_pAddress  The address where to construct the instance.
    /// \param [in,out] a_ppArgs    The constructor arguments given as an array of value pointers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void construct(void* a_pAddress, void** a_ppArgs) const
    {
        invoke(a_pAddress, a_ppArgs);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs N instances starting at a given place address.
    ///
    /// \param  a_uiCount           Number of instances to construct.
    /// \param [in,out] a_pAddress  The start address where to construct the instances.
    /// \param [in,out] a_ppArgs    The constructor arguments given as an array of value pointers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void construct(size_t a_uiCount, void* a_pAddress, void** a_ppArgs)
    {
        size_t i = 0;
        while (a_uiCount--)
        {
            invoke((byte*)a_pAddress + (i++) * getOwner()->asType()->getSize(), a_ppArgs);
        }
    }

    Constructor* asConstructor() const override
    {
        return (Constructor*)this;
    }

protected:
    Constructor(ClassType* a_pClassType, StringView a_strName, StringView a_strSignature, Modifiers a_Modifiers = 0,
                uint a_uiFlags = 0);
    virtual void** getNativeVTablePointer() const
    {
        PHANTOM_ASSERT_NO_IMPL();
        return nullptr;
    }
    virtual size_t getNativeVTableSize() const
    {
        PHANTOM_ASSERT_NO_IMPL();
        return 0;
    }
};

} // namespace reflection
} // namespace phantom
