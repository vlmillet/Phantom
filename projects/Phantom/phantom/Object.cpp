// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include <phantom/Object.h>
/* *********************************************** */
namespace phantom
{
void Object::destroy()
{
    m_pClass->deleteInstance(m_pAddress);
}

Object Object::as(reflection::Class* a_pDest) const
{
    return Object(m_pClass->cast(a_pDest, m_pAddress), a_pDest);
}

} // namespace phantom
