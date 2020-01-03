// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "FunctorProperty.h"

#include "Parameter.h"
#include "Signature.h"
#include "phantom/detail/new.h"

#include <phantom/reflection/Signal.h>
/* *********************************************** */

namespace phantom
{
namespace reflection
{
FunctorProperty::FunctorProperty(Type* a_pType, StringView a_Name, uint a_uiFilterMask /*= PHANTOM_R_FILTER_PROPERTY*/)
    : ValueMember(a_pType, a_Name, a_uiFilterMask)
{
}

FunctorProperty::FunctorProperty(Type* a_pType, StringView a_Name, GetFunctor const& a_Get, SetFunctor const& a_Set,
                                 uint a_uiFilterMask /*= PHANTOM_R_FILTER_PROPERTY*/)
    : ValueMember(a_pType, a_Name, a_uiFilterMask), m_Set(a_Set), m_Get(a_Get)
{
}

FunctorProperty::FunctorProperty(Type* a_pType, StringView a_Name, GetFunctor const& a_Get,
                                 uint a_uiFilterMask /*= PHANTOM_R_FILTER_PROPERTY*/)
    : ValueMember(a_pType, a_Name, a_uiFilterMask), m_Get(a_Get)
{
}

void FunctorProperty::getValue(void const* a_pObject, void* a_pDest) const
{
    PHANTOM_ASSERT(isReadable());
    m_Get(a_pObject, a_pDest);
}

void FunctorProperty::setValue(void* a_pObject, void const* a_pSrc) const
{
    PHANTOM_ASSERT(isWrittable());
    m_Set(a_pObject, a_pSrc);
}

} // namespace reflection
} // namespace phantom
