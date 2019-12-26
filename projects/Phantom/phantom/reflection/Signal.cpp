// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "Signal.h"

#include "Array.h"
#include "Constant.h"
#include "Field.h"
#include "Function.h"
#include "LocalVariable.h"
#include "Pointer.h"
#include "Signature.h"
/* *********************************************** */
namespace phantom
{
namespace reflection
{
Signal::Signal(ClassType* a_pClassType, size_t a_uiFieldOffset, StringView a_strName,
               StringView a_strSignature, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : Method(a_pClassType, a_strName, a_strSignature, a_Modifiers | PHANTOM_R_SLOT_METHOD,
             a_uiFlags),
      m_FieldOffset(a_uiFieldOffset)
{
    PHANTOM_ASSERT(!isVirtual(), "Signal cannot be virtual");
    PHANTOM_ASSERT(!isConst(), "Signal cannot be const");
}

Signal::Signal(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers /*= 0*/,
               uint a_uiFlags /*= 0*/)
    : Method(a_strName, a_pSignature, a_Modifiers | PHANTOM_R_SLOT_METHOD, a_uiFlags)
{
    PHANTOM_ASSERT(!isVirtual(), "Signal cannot be virtual");
    PHANTOM_ASSERT(!isConst(), "Signal cannot be const");
    size_t paramCount = m_pSignature->getParameterCount();
    for (size_t i = 0; i < paramCount; ++i)
    {
        m_pSignature->setParameterName(i, "a_" + lexical_cast<String>(i));
    }
}

Signal::Signal(StringView a_strName, Types a_Types, Modifiers a_Modifiers /*= 0*/,
               uint a_uiFlags /*= 0*/)
    : Method(a_strName, Signature::Create(PHANTOM_TYPEOF(void), a_Types), a_Modifiers, a_uiFlags)
{
    PHANTOM_ASSERT(!isVirtual(), "Signal cannot be virtual");
    PHANTOM_ASSERT(!isConst(), "Signal cannot be const");
}

Signal::Signal(StringView a_strName, Signature* a_pSignature, size_t a_FieldOffset,
               Modifiers a_Modifiers, uint a_uiFlags)
    : Method(a_strName, a_pSignature, a_Modifiers, a_uiFlags), m_FieldOffset(a_FieldOffset)
{
    PHANTOM_ASSERT(!isVirtual(), "Signal cannot be virtual");
    PHANTOM_ASSERT(!isConst(), "Signal cannot be const");
}

void* Signal::getInstance(void* a_pThis) const
{
    PHANTOM_ASSERT(getFieldOffset() != ~size_t(0));
    return reinterpret_cast<void*>(((byte*)a_pThis) + getFieldOffset());
}

void Signal::onAncestorChanged(LanguageElement* a_pLanguageElement)
{
    Method::onAncestorChanged(a_pLanguageElement);
    if (a_pLanguageElement == getOwner())
    {
        if (m_FieldOffset == ~size_t(0))
        {
            Field* pField = getOwner()->asClass()->getField("_PHNTM_signal_" + m_strName);
            PHANTOM_ASSERT(pField != nullptr);
            m_FieldOffset = pField->getOffset();
        }
    }
}

} // namespace reflection
} // namespace phantom
