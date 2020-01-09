// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/PrimitiveType.h>
/* **************** Declarations ***************** */

/* *********************************************** */
namespace phantom
{
namespace lang
{
/// \brief  Represents a C++ like enumeration.
///     \code{.cpp}
///         enum EValues
///         {
///             value0,
///             value1,
///             ...
///         };
///     \endcode

class PHANTOM_EXPORT_PHANTOM Enum : public PrimitiveType
{
    PHANTOM_DECLARE_LANGUAGE_ELEMENT_VISIT;

    PHANTOM_DECLARE_META_CLASS(Enum);

    template<class>
    friend class EnumT;

public:
    static PrimitiveType* IntType();

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an anonymous enum.
    ///
    /// \param [in,out] a_pUnderlyingIntType    The enum's underlying int type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Enum();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs an anonymous enum with explicit int type.
    ///
    /// \param [in,out] a_pUnderlyingIntType    The enum's underlying int type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Enum(PrimitiveType* a_pIntType);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a named enum.
    ///
    /// \param  a_strName                       The enum's name.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Enum(StringView a_strName);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a named enum with expliti int type.
    ///
    /// \param  a_strName                       The enum's name.
    /// \param [in,out] a_pUnderlyingIntType    The enum's underlying int type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Enum(StringView a_strName, PrimitiveType* a_pIntType);

    PHANTOM_DTOR ~Enum() override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Is a C++11 scoped enumeration ?
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool isScoped() const
    {
        return m_scoped;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Set as a C++11 scoped enumeration
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void setScoped()
    {
        m_scoped = true;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a constant at given position index.
    ///
    /// \param  i   Zero-based index of the constant.
    ///
    /// \return The constant at given position index.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline Constant* getConstant(size_t i) const
    {
        return m_Constants[i];
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constants matching the given pointed value.
    ///
    /// \param  a_pValue   pointer to enum value.
    ///
    /// \param a_Constants The constants matching the pointed value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getConstants(const void* a_pValue, Constants& a_Constants) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constants matching the given pointed value.
    ///
    /// \param  a_pValue   pointer to enum value.
    ///
    /// \param a_Constants The constants matching the pointed value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constants const& getConstants() const
    {
        return m_Constants;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the first constant matching the given pointed value.
    ///
    /// \param  a_pValue   pointer to enum value.
    ///
    /// \return The first constant matching the pointed value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constant* getFirstConstant(const void* a_pValue) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the first constant matching the given long long value.
    ///
    /// \param  a_pValue   pointer to enum value.
    ///
    /// \return The first constant matching the pointed value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constant* getFirstConstant(longlong a_Value) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default value of this enum or zero if no constant has been defined.
    ///
    /// \param  a_pBuffer   buffer receiving the default value.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void getDefaultValue(void* a_pBuffer) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the default constant of this enum or null if none present.
    ///
    /// \return  a_pBuffer   the default constant or null if none present
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constant* getDefaultConstant() const
    {
        if (m_Constants.size())
        {
            return m_Constants.front();
        }
        return nullptr;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the constant count.
    ///
    /// \return The constant count.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    inline size_t getConstantCount() const
    {
        return m_Constants.size();
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets a constant from its name.
    ///
    /// \param  a_strConstantName   The constant name.
    ///
    /// \return null if no constant found, else the constant.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Constant* getConstant(StringView a_strKey) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a constant to that enum.
    ///
    /// \param  a_pConstant The constant to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addConstant(Constant* a_pConstant);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a constant to that enum.
    ///
    /// \param  a_pConstant The constant to add.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addConstant(StringView a_Name, int a_Value);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds constants to that enum.
    ///
    /// \param  a_Values The constant values to add as pairs { "name", 5 }.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addConstants(StringView a_Name, ArrayView<Pair<StringView, int>> a_Values);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Adds a constant to that enum from code.
    ///
    /// \param  a_pConstant The constant code (ex: e_MyEnum_Value = 3).
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void addConstant(StringView a_strCode);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Removes a constant from that enum.
    ///
    /// \param  a_pConstant The constant to remove.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void removeConstant(Constant* a_pConstant);

    Enum* asEnum() const override
    {
        return const_cast<Enum*>(this);
    }
    PrimitiveType* asIntegralType() const override
    {
        return const_cast<Enum*>(this);
    }

    void valueFromString(StringView a_strIn, void* a_pDest) const override;
    void valueToString(StringBuffer& a_Buf, const void* a_pSrc) const override;

    bool isCopyable() const override
    {
        return true;
    }

    void* allocate(size_t n) const override
    {
        return m_pUnderlyingIntType->allocate(n);
    }
    void* allocate() const override
    {
        return m_pUnderlyingIntType->allocate();
    }
    void deallocate(void* a_pInstance, size_t n) const override
    {
        m_pUnderlyingIntType->deallocate(a_pInstance, n);
    }
    void deallocate(void* a_pInstance) const override
    {
        m_pUnderlyingIntType->deallocate(a_pInstance);
    }
    void construct(void* a_pInstance) const override
    {
        m_pUnderlyingIntType->construct(a_pInstance);
    }
    void destroy(void* a_pInstance) const override
    {
        m_pUnderlyingIntType->destroy(a_pInstance);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Searches for the constants matching the given value.
    ///
    /// \param  a_pInputValueAddress    The input value address.
    /// \param  a_Constants             The found constants.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void findConstantsWithValue(void* a_pSrc, Constants& out) const;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the underlying int type.
    ///
    /// \return The underlying int type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    PrimitiveType* getUnderlyingIntType() const
    {
        return m_pUnderlyingIntType;
    }

    bool convert(Type* a_pDstType, void* a_pDst, void const* a_pSrc) const override;

    Type* getUnderlyingType() const override
    {
        return m_pUnderlyingIntType;
    }

    Constant* createConstant(void* a_pSrc, StringView a_strName = "",
                             PrimitiveType* a_pPrimitiveType = nullptr) const override;

    void getUniqueName(StringBuffer& a_Buf) const override
    {
        return Type::getUniqueName(a_Buf);
    }
    void getQualifiedName(StringBuffer& a_Buf) const override
    {
        return Type::getQualifiedName(a_Buf);
    }
    void getDecoratedName(StringBuffer& a_Buf) const override
    {
        return Type::getDecoratedName(a_Buf);
    }
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override
    {
        return Type::getQualifiedDecoratedName(a_Buf);
    }

    using LanguageElement::getQualifiedName;
    using LanguageElement::getDecoratedName;
    using LanguageElement::getQualifiedDecoratedName;
    using LanguageElement::getUniqueName;

protected:
    void onElementRemoved(LanguageElement* a_pElement) override;
    /// \internal
    Enum(StringView a_strName, size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers, uint a_uiFlags);
    void onAncestorChanged(LanguageElement* a_pOwner) override;

private:
    Constants      m_Constants;
    PrimitiveType* m_pUnderlyingIntType;
    bool           m_scoped = false;
};

} // namespace lang
} // namespace phantom
