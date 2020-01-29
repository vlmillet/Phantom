// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/fwd.h>
/* ************* Class Declarations ************** */
/* *********************************************** */

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM DataElement
{
public:
    virtual size_t getAlignment() const = 0;
    virtual size_t getSize() const = 0;
    virtual size_t getOffset() const = 0;
    virtual void   setOffset(size_t a_uiOffset) = 0;
    virtual void   fetchFields(Fields& a_Out) const = 0;

    Symbol* asSymbol() const { return m_pThisElement; }

protected:
    DataElement(Symbol* a_pThis) : m_pThisElement(a_pThis) {}

private:
    Symbol* m_pThisElement;
};
} // namespace lang
} // namespace phantom
