// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/PointerType.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace reflection
{
/// \brief  Represents a Data pointer type.
/// \code{.cpp}
///             int*
/// \endcode
class PHANTOM_EXPORT_PHANTOM Pointer : public PointerType
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(Pointer);

public:
    friend class Type;

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a pointer type from a given pointed type (must not be called directly,
    /// use Type::makePointer() instead).
    ///
    /// \param a_pType The pointer type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Pointer(Type* a_pPointeeType);

public:
    PHANTOM_DTOR ~Pointer() override;

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Gets the pointed type (ex: int* => int).
    ///
    /// \return The pointed type.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    Type* getPointeeType() const
    {
        return m_pUnderlyingType;
    }

    Type*    asAddressType() const override;
    Pointer* asPointer() const override;
    Type*    asClassAddressType() const override;
    Pointer* asClassPointer() const override;
    Type*    asConstClassAddressType() const override;
    Pointer* asConstClassPointer() const override;

    bool convert(Type* a_pDstType, void* a_pDst, void const* a_pSrc) const override;

    void valueFromString(StringView a_str, void* dest) const override;

    void valueToString(StringBuffer& a_Buf, const void* src) const override;

    void valueToLiteral(StringBuffer& a_Buf, const void* src) const override;

    uint getDataPointerLevel() const override;

    Type* removePointer() const override;

    Type* removePointerOrArray() const override;

    Type* removeAddress() const override;

    Type* removeAllConst() const override;

    Type* removeAllQualifiers() const override;

    bool isCopyable() const override;

    bool isEquivalent(Type* a_pType) const override;

    bool partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_DeducedConstants) const override;

    bool isSame(Symbol* a_pOther) const override;

    Type* getUnderlyingType() const override;

    Type* replicate(Type* a_pSource) const override;

    // 13.6.19
    // ----------------------
    /// \brief delegate for C++ standard : T *VQ & operator=(T *VQ &, T *);
    void assignment(void** a_pArgs, void* a_pOutput);

    // Built-in Delegates
    // 13.6.5
    /// \brief T *VQ & operator++(T *VQ &);
    void preIncrement(void** a_pArgs, void* a_pOutput);
    /// \brief T * operator++(T *VQ &, int);
    void postIncrement(void** a_pArgs, void* a_pOutput);
    /// \brief T *VQ & operator--(T *VQ &);
    void preDecrement(void** a_pArgs, void* a_pOutput);
    /// \brief T * operator--(T *VQ &, int);
    void postDecrement(void** a_pArgs, void* a_pOutput);

    // 13.6.6
    /// \brief T & operator*(T *);
    void dereference(void** a_pArgs, void* a_pOutput);

    // 13.6.7
    /// \brief T & operator+(T *);
    void plus(void** a_pArgs, void* a_pOutput);

    // 13.6.13
    /// \brief T * operator+(T *, std::ptrdiff_t);
    void add(void** a_pArgs, void* a_pOutput);
    /// \brief T * operator+(std::ptrdiff_t, T *);
    void addRev(void** a_pArgs, void* a_pOutput);
    /// \brief T & operator[](T *, std::ptrdiff_t);
    void bracket(void** a_pArgs, void* a_pOutput);
    /// \brief T & operator[](std::ptrdiff_t, T *);
    void bracketRev(void** a_pArgs, void* a_pOutput);
    /// \brief T * operator-(T *, std::ptrdiff_t);
    void subtract(void** a_pArgs, void* a_pOutput);

    // 13.6.14 ----------------------
    /// \brief std::ptrdiff_t operator-(T , T );
    void subtractPointer(void** a_pArgs, void* a_pOutput);

    // 13.6.21
    /// \brief T *VQ & operator+=(T *VQ &, std::ptrdiff_t);
    void assignmentAdd(void** a_pArgs, void* a_pOutput);
    /// \brief T *VQ & operator-=(T *VQ &, std::ptrdiff_t);
    void assignmentSubtract(void** a_pArgs, void* a_pOutput);

    using LanguageElement::getUniqueName;
    using LanguageElement::getQualifiedName;
    using LanguageElement::getDecoratedName;
    using LanguageElement::getQualifiedDecoratedName;

    void getUniqueName(StringBuffer& a_Buf) const override;
    void getQualifiedName(StringBuffer& a_Buf) const override;
    void getDecoratedName(StringBuffer& a_Buf) const override;
    void getQualifiedDecoratedName(StringBuffer& a_Buf) const override;

protected:
    void onReferencedElementRemoved(LanguageElement* a_pElement) override;
};

} // namespace reflection
} // namespace phantom
