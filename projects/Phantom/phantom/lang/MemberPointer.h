// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Type.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Base class for member pointer types representations.
class PHANTOM_EXPORT_PHANTOM MemberPointer : public Type
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(MemberPointer);

public:
    friend class Type;

protected:
    MemberPointer(TypeKind a_TypeId, ClassType* a_pObjectType, size_t a_uiSize, size_t a_uiAlignment,
                  Modifiers a_Modifiers = 0, uint a_uiFlags = 0);

public:
    PHANTOM_DTOR ~MemberPointer() override;

    Type* asPOD() const override
    {
        return (MemberPointer*)this;
    }
    MemberPointer* asMemberPointer() const override
    {
        return (MemberPointer*)this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the object class type associated with this member pointer type.
    ///
    /// \return The object class type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ClassType* getObjectType() const
    {
        return m_pObjectType;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Delegate helper for assignment.
    ///
    /// \param [in,out] a_pArgs     The arguments.
    /// \param [in,out] a_pOutput   The return value address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void assignment(void** a_pArgs, void* a_pOutput)
    {
        memcpy(a_pArgs[0], a_pArgs[1], getSize());
        *(void**)a_pOutput = a_pArgs[0];
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Delegate helper for equality.
    ///
    /// \param [in,out] a_pArgs     The arguments.
    /// \param [in,out] a_pOutput   The return value address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    using Type::equal;

    void equal(void** a_pArgs, void* a_pOutput)
    {
        *(bool*)a_pOutput = memcmp(a_pArgs[0], a_pArgs[1], getSize()) == 0;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Delegate helper for non equality.
    ///
    /// \param [in,out] a_pArgs     The arguments.
    /// \param [in,out] a_pOutput   The return value address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void notEqual(void** a_pArgs, void* a_pOutput)
    {
        *(bool*)a_pOutput = memcmp(a_pArgs[0], a_pArgs[1], getSize()) != 0;
    }

protected:
    ClassType* m_pObjectType;
};

} // namespace lang
} // namespace phantom
