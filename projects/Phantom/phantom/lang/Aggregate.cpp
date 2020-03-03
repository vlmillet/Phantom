// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "Aggregate.h"

#include <phantom/lang/Type.h>

namespace phantom
{
namespace lang
{
void Aggregate::getFlattenedFields(AggregateFields& _flattenedfields) const
{
    AggregateFields notFlattened;
    getFields(notFlattened);
    for (auto& f : notFlattened)
    {
        size_t i = _flattenedfields.size();
        if (f.type != nullptr)
        {
            f.type->getFlattenedAggregateFields(_flattenedfields);
            for (; i < _flattenedfields.size(); ++i)
            {
                _flattenedfields[i].offset += f.offset;
            }
        }
        else
        {
            _flattenedfields.push_back(f);
        }
    }
}

Type* Aggregate::asType() const
{
    return m_pThisType;
}
Class* Aggregate::asClass() const
{
    return m_pThisType->asClass();
}
Array* Aggregate::asArray() const
{
    return m_pThisType->asArray();
}

} // namespace lang
} // namespace phantom
