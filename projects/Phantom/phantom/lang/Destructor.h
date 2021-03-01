// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Method.h>
#include <phantom/lang/Type.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a constructor member function.
class PHANTOM_EXPORT_PHANTOM Destructor : public Method
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

public:
    friend class Class;
    PHANTOM_DECLARE_META_CLASS(Destructor);

public:
    Destructor(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Destroys a single instance at a given place address.
    ///
    /// \param [in,out] a_pAddress  The address where to construct the instance.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void destroy(void* a_pAddress) const { invoke(a_pAddress, nullptr); }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Destroys N instances starting at a given place address.
    ///
    /// \param  a_uiCount           Number of instances to destroy.
    /// \param [in,out] a_pAddress  The start address where to destroy the instances.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline void destroy(size_t a_uiCount, void* a_pAddress)
    {
        size_t i = 0;
        while (a_uiCount--)
        {
            invoke((byte*)a_pAddress + (i++) * getOwner()->asType()->getSize(), nullptr);
        }
    }

    Destructor* asDestructor() const override { return (Destructor*)this; }

protected:
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

} // namespace lang
} // namespace phantom
