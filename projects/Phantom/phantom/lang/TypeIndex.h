// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Type.h>
/* *********************************************** */

namespace phantom
{
namespace lang
{
/// @brief struct that can be used in associative containers for Type
/// @note don't use Type* directly if you are reflecting templates. Indeed templates can be
/// instanciated multiple times in different libraries.
struct TypeIndex
{
    TypeIndex() = default;
    TypeIndex(Type* a_pType) : hash(a_pType ? a_pType->getHash() : 0), type(a_pType)
    {
    }
    TypeIndex(TypeIndex const& _other) : hash(_other.hash), type(_other.type)
    {
    }
    TypeIndex& operator=(TypeIndex const& _other)
    {
        const_cast<Type*&>(type) = _other.type;
        const_cast<hash64&>(hash) = _other.hash;
        return *this;
    }

    bool operator==(TypeIndex const& _other) const
    {
        return hash == _other.hash;
    }
    bool operator!=(TypeIndex const& _other) const
    {
        return hash != _other.hash;
    }
    bool operator<(TypeIndex _other) const
    {
        return hash < _other.hash;
    }

    operator Type*() const
    {
        return type;
    }

    hash64 const hash = 0;
    Type* const  type = nullptr;
};
} // namespace lang
} // namespace phantom
