// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "TemplateDependantArray.h"

#include "phantom/lang/Placeholder.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
TemplateDependantArray::TemplateDependantArray(Type* a_pType, Expression* a_pSize,
                                               PlaceholderConstant* a_pDeductibleConstant)
    : Array(a_pType, 0), m_pItemCountExpression(a_pSize), m_pDeductibleConstant(a_pDeductibleConstant)
{
    removeFlags(PHANTOM_R_FLAG_NATIVE);
    addFlags(PHANTOM_R_FLAG_TEMPLATE_DEPENDANT);
    PHANTOM_ASSERT(reinterpret_cast<LanguageElement*>(a_pSize)->isTemplateDependant() ||
                   reinterpret_cast<LanguageElement*>(a_pSize)->isCompileTime());
}

void TemplateDependantArray::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    getItemType()->getQualifiedDecoratedName(a_Buf);
    char buf[32];
    snprintf(buf, 32, "[%s]",
             reinterpret_cast<LanguageElement*>(m_pItemCountExpression)->getQualifiedDecoratedName().c_str());
    a_Buf += (const char*)buf;
}

void TemplateDependantArray::getDecoratedName(StringBuffer& a_Buf) const
{
    getItemType()->getDecoratedName(a_Buf);
    char buf[32];
    snprintf(buf, 32, "[%s]",
             reinterpret_cast<LanguageElement*>(m_pItemCountExpression)->getQualifiedDecoratedName().c_str());
    a_Buf += (const char*)buf;
}

void TemplateDependantArray::getQualifiedName(StringBuffer& a_Buf) const
{
    getItemType()->getQualifiedName(a_Buf);
    char buf[32];
    snprintf(buf, 32, "[%s]",
             reinterpret_cast<LanguageElement*>(m_pItemCountExpression)->getQualifiedDecoratedName().c_str());
    a_Buf += (const char*)buf;
}

bool TemplateDependantArray::isSame(Symbol* a_pOther) const
{
    Placeholder*            pPH = a_pOther->asPlaceholder();
    TemplateDependantArray* pTDAT = pPH ? phantom::Object::Cast<TemplateDependantArray>(pPH->asSymbol()) : nullptr;
    if (pTDAT == nullptr)
        return false;
    return pTDAT &&
    reinterpret_cast<LanguageElement*>(pTDAT->m_pItemCountExpression)
    ->isSame(reinterpret_cast<LanguageElement*>(m_pItemCountExpression));
}

} // namespace lang
} // namespace phantom
