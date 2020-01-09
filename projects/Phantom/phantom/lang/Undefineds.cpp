// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include "Undefineds.h"

namespace phantom
{
namespace lang
{
Undefineds::Undefineds()
    : m_pPointerType(nullptr),
      m_pLValueReference(nullptr),
      m_pRValueReference(nullptr),
      m_pConstType(nullptr),
      m_pVolatileType(nullptr),
      m_pConstVolatileType(nullptr)
{
}

Type* Undefineds::makePointer()
{
    return m_pPointerType;
}

Type* Undefineds::makeLValueReference()
{
    return m_pLValueReference;
}

Type* Undefineds::makeRValueReference()
{
    return m_pRValueReference;
}

Type* Undefineds::makeConst()
{
    return m_pConstType;
}

Type* Undefineds::makeVolatile()
{
    return m_pVolatileType;
}

Type* Undefineds::makeConstVolatile()
{
    return m_pConstVolatileType;
}

Type* Undefineds::makeArray(size_t s)
{
    return m_Arrays[s];
}

} // namespace lang
} // namespace phantom
