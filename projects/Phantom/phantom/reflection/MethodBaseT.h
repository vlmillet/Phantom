// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

/// @cond INTERNAL

/* ****************** Includes ******************* */
#include <phantom/reflection/CallHelpers.h>
#include <phantom/reflection/Method.h>
#include <phantom/reflection/NativeVTableInspector.h>
/* **************** Declarations ***************** */
/* *********************************************** */
namespace phantom
{
namespace reflection
{
class MethodBaseT : public Method
{
public:
    MethodBaseT(StringView a_strName, Signature* a_pSignature, Modifiers a_Modifiers = 0,
                uint a_uiFlags = 0)
        : Method(a_strName, a_pSignature, a_Modifiers, a_uiFlags | PHANTOM_R_FLAG_NATIVE)
    {
    }

    virtual void onAncestorChanged(LanguageElement* a_pOwner) override
    {
        if (a_pOwner == getOwner())
        {
            StringBuffer buffer;
            conversionOperatorNameNormalizer(m_strName, buffer, getOwner());
            m_strName.clear();
            m_strName.append(buffer.data(), buffer.size());
        }
    }
};

} // namespace reflection
} // namespace phantom

/// @endcond
