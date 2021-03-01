// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

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
    PointerType(TypeKind _typeKind, Type* a_pPointeeType, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                Modifiers a_Modifiers = 0, uint a_uiFlags = 0)
        : ExtendedType(_typeKind, a_pPointeeType, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
    {
    }

public:
    Type* asPOD() const override { return (PointerType*)this; }

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
