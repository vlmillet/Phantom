// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Symbol.h>
#include <phantom/traits/Dereferencer.h>
#include <phantom/traits/ReferenceToPointer.h>
#include <phantom/typeof>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Base class for value members representations (Data members, properties, ...).
class PHANTOM_EXPORT_PHANTOM ValueMember : public Symbol
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(ValueMember);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a value member with given value type, name, range, database mask and
    ///         optional modifiers.
    ///
    /// \param  a_pValueType            The value type.
    /// \param  a_strName               The name.
    /// \param  a_pRange                The range.
    /// \param  a_uiFilterMask   The database mask.
    /// \param  a_Modifiers             (optional) the modifiers.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    ValueMember(Type* a_pValueType, StringView a_strName, uint a_uiFilterMask, Modifiers a_Modifiers = 0,
                uint a_uiFlags = 0);
    PHANTOM_DTOR ~ValueMember() override
    {
    }

    ValueMember* asValueMember() const override
    {
        return (ValueMember*)this;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if the value is movable
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool isMovable() const
    {
        return true;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if the value is writtable (for example a property which has a set function).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool isWrittable() const = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Tests if the value is readable (for example a property which has a get function).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual bool isReadable() const = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the value of this value member for the given object.
    ///
    /// \param  a_pObject               The object.
    /// \param [in,out] a_pOutputBuffer The output buffer which will receive the value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void getValue(void const* a_pObject, void* a_pOutputBuffer) const = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the value of this value member for the given object.
    ///
    /// \param [in,out] a_pObject   The object.
    /// \param  a_pInputBuffer      The input buffer which contains the value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void setValue(void* a_pObject, void const* a_pInputBuffer) const = 0;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the value of this value member for the given object (value is moved instead of
    /// copied if possible).
    ///
    /// \param [in,out] a_pObject   The object.
    /// \param  a_pInputBuffer      The input buffer which contains the value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void setValueMoved(void* a_pObject, void* a_pInputBuffer) const
    {
        setValue(a_pObject, a_pInputBuffer);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the value of this value member for the given object.
    ///
    /// \param  a_pObject               The object.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<typename t_Ty>
    t_Ty get(void const* a_pObject) const
    {
        PHANTOM_ASSERT(getValueType() == PHANTOM_TYPEOF(t_Ty));
        PHANTOM_TYPENAME ReferenceToPointer<t_Ty>::type value;
        getValue(a_pObject, &value);
        return Dereferencer<t_Ty>::apply(value);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the value of this value member for the given object.
    ///
    /// \param  a_pObject               The object.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    template<typename t_Ty>
    void set(void* a_pObject, t_Ty a_Value)
    {
        PHANTOM_ASSERT(getValueType() == PHANTOM_TYPEOF(t_Ty));
        setValue(a_pObject, &a_Value);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the value type.
    ///
    /// \return The value type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline Type* getValueType() const
    {
        return m_pValueType;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the filter mask.
    ///
    /// \return The filter mask.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline uint getFilterMask() const
    {
        return m_uiFilterMask;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Sets the filter mask.
    ///
    /// \param a_Mask The filter mask.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setFilterMask(uint a_Mask);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Copies value from a source object to a destination object.
    ///
    /// \param  a_pDestinationObject    The destination object which will receive the copied value.
    /// \param  a_pSourceObject         The source object from which to take the source value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void copyValue(void* a_pDestinationObject, void const* a_pSourceObject) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the owner class type, if exists.
    ///
    /// \return null if owner is not a class type, else the owner as class type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline ClassType* getOwnerClassType() const
    {
        return getOwner()->asClassType();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the owner class, if exists.
    ///
    /// \return null if owner is not a class, else the owner as class.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline Class* getOwnerClass() const
    {
        return getOwner()->asClass();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the address of the value for the given object, if possible.
    ///
    /// \param  a_pObject   The object.
    ///
    /// \return null if address is not available, else the address.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    virtual void* getAddress(void const* a_pObject) const;

    void setDefaultExpression(Expression* a_pExpression);
    void setDefaultExpression(const char* a_pExpression);

    Expression* getDefaultExpression() const;

protected:
    void onReferencedElementRemoved(LanguageElement* a_pElement) override;

protected:
    Type*               m_pValueType{};
    mutable Expression* m_pDefaultExpression{};
    uint                m_uiFilterMask{};

private:
    mutable const char* m_pNativeDefaultExpressionStr{};
};

} // namespace lang
} // namespace phantom
