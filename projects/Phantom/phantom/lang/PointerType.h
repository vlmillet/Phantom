// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once
// #pragma message("Including "__FILE__)

/* ****************** Includes ******************* */
#include <phantom/lang/ExtendedType.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Base class for pointer/address type representations (Data pointer types, function
/// pointer types, etc...).
class PHANTOM_EXPORT_PHANTOM PointerType : public ExtendedType
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(PointerType);

public:
    friend class Type;

protected:
    PointerType(Type* a_pPointeeType, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : ExtendedType(a_pPointeeType, TypeKind::Pointer, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
    {
    }

public:
    PHANTOM_DTOR ~PointerType() override
    {
    }

    Type* asPOD() const override
    {
        return (PointerType*)this;
    }
    PointerType* asPointerType() const override
    {
        return (PointerType*)this;
    }

    // 13.6.15
    // ----------------------
    /// \brief delegate for C++ standard : bool operator<(T , T );
    void less(void** a_pArgs, void* a_pOutput);
    /// \brief delegate for C++ standard : bool operator>(T , T );
    void greater(void** a_pArgs, void* a_pOutput);
    /// \brief delegate for C++ standard : bool operator<=(T , T );
    void lessEqual(void** a_pArgs, void* a_pOutput);
    /// \brief delegate for C++ standard : bool operator>=(T , T );
    void greaterEqual(void** a_pArgs, void* a_pOutput);
    /// \brief delegate for C++ standard : bool operator==(T , T );
    void equal(void** a_pArgs, void* a_pOutput);
    /// \brief delegate for C++ standard : bool operator!=(T , T );
    void notEqual(void** a_pArgs, void* a_pOutput);

    using Type::equal;

    // 13.6.19
    // ----------------------
    /// \brief delegate for C++ standard : T *VQ & operator=(T *VQ &, T *);
    void assignment(void** a_pArgs, void* a_pOutput);

protected:
};

} // namespace lang
} // namespace phantom
