// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/MemberPointer.h>
/* ************* Class Declarations ************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a Data member pointer type.
/// \code{.cpp}
///     int (MyClassType::*data_member_pointer_type)
/// \endcode
class PHANTOM_EXPORT_PHANTOM FieldPointer : public MemberPointer
{
    PHANTOM_DECL_TYPE;

public:
    PHANTOM_DECLARE_META_CLASS(FieldPointer);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a Data member pointer type from a ClassType.
    ///
    /// \param  a_pClassType    The class type.
    /// \param  a_pValueType    The Data member value type.
    /// \param  a_uiModifiers   (optional) the Data member pointer type modifiers (for users
    /// personal requirements, none by default).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FieldPointer(ClassType* a_pObjectType, Type* a_pValueType, Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \internal
    /// \brief  Constructs a native Data member pointer type from a ClassType (for native internal
    /// implementations).
    ///
    /// \param  a_pClassType    The class type.
    /// \param  a_pValueType    The Data member value type.
    /// \param  a_uiSize        The size.
    /// \param  a_uiAlignment   The alignment.
    /// \param  a_uiModifiers   (optional) the Data member pointer type modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    FieldPointer(ClassType* a_pObjectType, Type* a_pValueType, size_t a_uiSize, size_t a_uiAlignment,
                 Modifiers a_Modifiers, uint a_uiFlags);

public:
    PHANTOM_DTOR ~FieldPointer() override {}

    FieldPointer* asFieldPointer() const override { return (FieldPointer*)this; }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Retrieves the Data member offset associated with the given Data member pointer.
    ///
    /// \param [in,out] a_pPointer  The pointer.
    ///
    /// \return The offset.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual size_t getOffset(void* a_pPointer) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets a value to the given object/data member pointer couple.
    ///
    ///         \code{.cpp}
    ///              (object->*data_member_pointer) = X // C++ equivalent expression
    ///         \endcode.
    ///
    /// \param  a_pPointer  The Data member pointer.
    /// \param  a_pObject   The object address.
    /// \param  a_pSrc      The value input buffer.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void setValue(void* a_pPointer, void* a_pObject, void const* a_pSrc) const;
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets a value to the given object/data member pointer couple.
    ///
    ///         \code{.cpp}
    ///              (object->*data_member_pointer) // C++ equivalent expression
    ///         \endcode.
    ///
    /// \param  a_pPointer  The Data member pointer.
    /// \param  a_pObject   The object address.
    /// \param  a_pDest     The value output buffer.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void getValue(void* a_pPointer, void const* a_pObject, void* a_pDest) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the object address adjusted by a Data member pointer.
    ///
    /// \param [in,out] a_pPointer  The Data member pointer.
    /// \param  a_pObject           The object address.
    ///
    /// \return The adjusted object address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* getAddress(void* a_pPointer, void const* a_pObject) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the value type of this Data member pointer.
    ///
    /// \return The value type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline Type* getValueType() const { return m_pValueType; }

protected:
    Type* m_pValueType;
};

} // namespace lang
} // namespace phantom
