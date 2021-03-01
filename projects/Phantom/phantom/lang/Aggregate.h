// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include "reflection_fwd.h"
/* **************** Declarations ***************** */
/* *********************************************** */
namespace phantom
{
namespace lang
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
    Aggregate(Type* a_pThisType) : m_pThisType(a_pThisType) {}

public:
    virtual void getFields(AggregateFields& _fields) const = 0;
    void         getFlattenedFields(AggregateFields& _flattenedfields) const;

    Type*  asType() const;
    Class* asClass() const;
    Array* asArray() const;

private:
    Type* m_pThisType = nullptr;
};
} // namespace lang
} // namespace phantom
