// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/detail/fwd.h>
/* **************** Declarations ***************** */
/* *********************************************** */
namespace phantom
{
namespace reflection
{
struct AggregateField
{
    Type*  type = nullptr;
    size_t offset = 0;
};

using AggregateFields = SmallVector<AggregateField, 16>;

class PHANTOM_EXPORT_PHANTOM Aggregate
{
protected:
    Aggregate(Type* a_pThisType) : m_pThisType(a_pThisType)
    {
    }

public:
    virtual void getFields(AggregateFields& _fields) const = 0;
    void         getFlattenedFields(AggregateFields& _flattenedfields) const;

    Type*  asType() const;
    Class* asClass() const;
    Array* asArray() const;

private:
    Type* m_pThisType = nullptr;
};
} // namespace reflection
} // namespace phantom
