// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/QualifiedType.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents a const type
///         \code{.cpp}
///                     int const
///                     MyClass* const
///                     ...
///         \endcode
class PHANTOM_EXPORT_PHANTOM ConstType : public QualifiedType
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(ConstType);

    friend class phantom::lang::Namespace;
    friend class phantom::lang::Type;

private:
    ConstType(Type* a_pType);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the type that became 'const'.
    ///
    /// \return return the type that became 'const'.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getConstedType() const { return m_pUnderlyingType; }

    Type*         addConst() const override;
    Type*         addVolatile() const override;
    virtual Type* addConstVolatile() const;

    Type* removeConstVolatile() const override { return m_pUnderlyingType; }
    Type* removeConst() const override { return m_pUnderlyingType; }
    Type* removeAllConst() const override { return m_pUnderlyingType->removeAllConst(); }

    Type* replicate(Type* a_pSource) const override;

    ConstType* asConstType() const override { return const_cast<ConstType*>(this); }
    ConstType* asConstClass() const override
    {
        return (m_pUnderlyingType && m_pUnderlyingType->asClass()) ? const_cast<ConstType*>(this) : nullptr;
    }

    bool partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_Deductions) const override;

    bool isSame(Symbol* a_pOther) const override;

    virtual bool isConstConvertibleTo(Type* a_pType) const { return isSame(a_pType); }

    Type* getUnderlyingType() const override { return m_pUnderlyingType; }

    void getUniqueName(StringBuffer& a_Buf) const override;

    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;

    Type* removeEverything() const override { return m_pUnderlyingType->removeEverything(); }

protected:
    ConstVolatileType* createConstVolatileType() const override { return nullptr; }
    ConstType*         createConstType() const override { return nullptr; }
    VolatileType*      createVolatileType() const override { return nullptr; }
};

} // namespace lang
} // namespace phantom
