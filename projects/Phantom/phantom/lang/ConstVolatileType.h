// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/ConstType.h>
#include <phantom/lang/QualifiedType.h>
#include <phantom/lang/VolatileType.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a const volatile type
///         \code{.cpp}
///                     int const volatile
///                     MyClass* const volatile
///                     ...
///         \endcode
class PHANTOM_EXPORT_PHANTOM ConstVolatileType : public QualifiedType
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(ConstVolatileType);

    friend class phantom::lang::Namespace;
    friend class phantom::lang::Type;

private:
    ConstVolatileType(Type* a_pType);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the type that became 'const volatile'.
    ///
    /// \return return the type that became 'const volatile'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getConstVolatiledType() const { return m_pUnderlyingType; }

    Type* replicate(Type* a_pSource) const override;

    virtual ConstVolatileType* asConstVolatileClass() const
    {
        return (m_pUnderlyingType && m_pUnderlyingType->asClass()) ? const_cast<ConstVolatileType*>(this) : nullptr;
    }

    bool partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_Deductions) const override;

    bool isSame(Symbol* a_pOther) const override;

    hash64 computeLocalHash() const override;
};

} // namespace lang
} // namespace phantom
