// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/detail/core.h>
#include <phantom/utils/SmallMap.h>

namespace phantom
{
namespace lang
{
class PHANTOM_EXPORT_PHANTOM Undefineds
{
public:
    Undefineds();
    Type* makePointer();
    Type* makeLValueReference();
    Type* makeRValueReference();
    Type* makeConst();
    Type* makeVolatile();
    Type* makeConstVolatile();
    Type* makeArray(size_t s);

private:
    Type*                   m_pPointerType;
    Type*                   m_pLValueReference;
    Type*                   m_pRValueReference;
    Type*                   m_pConstType;
    Type*                   m_pVolatileType;
    Type*                   m_pConstVolatileType;
    SmallMap<size_t, Type*> m_Arrays;
};
} // namespace lang
} // namespace phantom
