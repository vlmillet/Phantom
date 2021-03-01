// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/lang/Constant.h>
#include <phantom/lang/MetaNewDelete.h>
#include <phantom/lang/TypeOf.h>
#include <phantom/traits/CopyTraits.h>
#include <phantom/traits/IsNullptrT.h>
#include <phantom/utils/LexicalCast.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
template<typename t_Ty>
class ConstantT : public Constant
{
    friend class phantom::lang::PrimitiveType;

    using SelfType = ConstantT<t_Ty>;
    using ValueTypeNoConst = std::remove_const_t<t_Ty>;
    PHANTOM_STATIC_ASSERT(phantom::IsCopyable<ValueTypeNoConst>::value, "constant value must be copyable");

public:
    ConstantT(Type* a_pValueType, StringView a_strName, t_Ty a_Value, Modifiers a_Modifiers, uint a_uiFlags)
        : Constant(a_pValueType, a_strName, a_Modifiers, a_uiFlags), m_Value(a_Value)
    {
    }
    ConstantT(StringView a_strName, t_Ty a_Value, Modifiers a_Modifiers, uint a_uiFlags)
        : Constant(PHANTOM_TYPEOF(t_Ty), a_strName, a_Modifiers, a_uiFlags), m_Value(a_Value)
    {
    }
    ConstantT(t_Ty a_Value, Modifiers a_Modifiers, uint a_uiFlags)
        : Constant(PHANTOM_TYPEOF(t_Ty), StringView(), a_Modifiers, a_uiFlags), m_Value(a_Value)
    {
    }

public:
    void getValue(void* a_pDest) const override { *reinterpret_cast<ValueTypeNoConst*>(a_pDest) = m_Value; }

    bool hasValue(const void* a_pSrc) const override { return *static_cast<const t_Ty*>(a_pSrc) == m_Value; }

    bool isSame(Symbol* a_pObject) const override
    {
        if (Constant* pOther = a_pObject->asConstant())
        {
            if (pOther == this)
                return true;
            if (pOther->asPlaceholder())
                return false;
            Type* pThisType = getValueType();
            Type* pOtherType = pOther->getValueType();
            if (pOtherType->isSame(pThisType))
            {
                void* val = alloca(pOtherType->getSize());
                pOther->getValue(val);
                return pThisType->equal(&m_Value, val);
            }
            else if (pOtherType->asIntegralType() && pThisType->asIntegralType()) // both integers
            {
                size_t max = std::max PHANTOM_PREVENT_MACRO_SUBSTITUTION(pThisType->getSize(), pOtherType->getSize());

                void* valThis = alloca(max);
                memset(valThis, 0, max);
                this->getValue(valThis);

                void* valOther = alloca(max);
                memset(valOther, 0, max);
                pOther->getValue(valOther);
                return memcmp(valThis, valOther, max) == 0;
            }
        }
        return false;
    }

    Constant* cloneImpl(LanguageElement* a_pOwner) const override
    {
        return a_pOwner->NewMeta<SelfType>(getName(), m_Value, getModifiers(), getFlags());
    }

    bool isZero() const override { return m_Value == t_Ty(0); }

    bool isIntegral() const override { return std::is_integral<t_Ty>::value; }

    void toString(StringBuffer& a_Buf) const override { m_pValueType->valueToString(a_Buf, &m_Value); }

    void toLiteral(StringBuffer& a_Buf) const override { m_pValueType->valueToLiteral(a_Buf, &m_Value); }

    void const* getAddress() const override { return &m_Value; }

private:
    t_Ty m_Value;
};

} // namespace lang
} // namespace phantom

/// @endcond
