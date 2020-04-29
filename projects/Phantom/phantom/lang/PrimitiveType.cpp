// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "PrimitiveType.h"

#include "Application.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
Constant* PrimitiveType::createConstant(LanguageElement*, void*, StringView /*= ""*/,
                                        PrimitiveType* /*= nullptr*/) const
{
    return nullptr;
}

phantom::lang::Constant* PrimitiveType::createConstant(void* a_pSrc, StringView a_strName /*= ""*/,
                                                       PrimitiveType* a_pPrimitiveType /*= nullptr*/) const
{
    return createConstant(Application::Get(), a_pSrc, a_strName, a_pPrimitiveType);
}

PrimitiveType::PrimitiveType(TypeKind a_eTypeKind) : Type(a_eTypeKind, "", 0, 0, PHANTOM_R_NONE, PHANTOM_R_NONE) {}

PrimitiveType::PrimitiveType(TypeKind a_eTypeKind, StringView a_strName, size_t a_uiSize, size_t a_uiAlignment,
                             Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Type(a_eTypeKind, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

PrimitiveType::PrimitiveType(TypeKind a_eTypeKind, Type* a_pUnderlyingType, StringView a_strName, size_t a_uiSize,
                             size_t a_uiAlignment, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Type(a_eTypeKind, a_pUnderlyingType, a_strName, a_uiSize, a_uiAlignment, a_Modifiers, a_uiFlags)
{
}

} // namespace lang
} // namespace phantom
