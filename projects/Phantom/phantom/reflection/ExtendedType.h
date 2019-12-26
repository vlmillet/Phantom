// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/Type.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace reflection
{
/// \brief  Base class for pointer/address type representations (Data pointer types, function
/// pointer types, etc...).
class PHANTOM_EXPORT_PHANTOM ExtendedType : public Type
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(ExtendedType);

public:
    friend class Type;

protected:
    ExtendedType(Type* a_pUnderlyingType, TypeKind a_eTypeKind, StringView a_strName,
                 size_t a_uiSize, size_t a_uiAlignment, Modifiers a_Modifiers = 0,
                 uint a_uiFlags = 0);

public:
    PHANTOM_DTOR ~ExtendedType() override
    {
    }

    Type* asPOD() const override
    {
        return (ExtendedType*)this;
    }
    ExtendedType* asExtendedType() const override
    {
        return (ExtendedType*)this;
    }

    Type* getUnderlyingType() const override
    {
        return m_pUnderlyingType;
    }

    Type* removeEverything() const override
    {
        return m_pUnderlyingType->removeEverything();
    }

protected:
    void onReferencedElementRemoved(LanguageElement* a_pElement) override;

protected:
    Type* m_pUnderlyingType;
};

} // namespace reflection
} // namespace phantom
