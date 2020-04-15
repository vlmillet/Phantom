// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Reference.h"
/* *********************************************** */
namespace phantom
{
namespace lang
{
void Reference::onReferencedElementRemoved(LanguageElement* a_pElement)
{
    Type::onReferencedElementRemoved(a_pElement);
    if (m_pUnderlyingType == a_pElement)
    {
        m_pUnderlyingType = nullptr;
    }
}

} // namespace lang
} // namespace phantom
