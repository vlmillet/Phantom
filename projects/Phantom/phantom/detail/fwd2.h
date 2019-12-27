// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "fwd.h"

namespace phantom
{
HAUNT_PACKAGE("phantom");
PHANTOM_DECL_LISTABLE(State, 10);
PHANTOM_DECL_LISTABLE(Track, 4);
class MemoryContext;
HAUNT_END("phantom");
struct UserData;

template<class S>
class Functor;

struct NullMutex
{
    void lock()
    {
    }
    void unlock() noexcept
    {
    }
    bool try_lock()
    {
        return true;
    }
};

template<class S, class Mtx = NullMutex>
class Signal;

namespace reflection
{
HAUNT_PACKAGE("phantom.reflection");
PHANTOM_DECL_LISTABLE_S(Alias);
PHANTOM_DECL_LISTABLE_S(Class);
PHANTOM_DECL_LISTABLE(ValueMember, 10);
PHANTOM_DECL_LISTABLE(LanguageElementVisitor);
PHANTOM_DECL_LISTABLE(Scope);
PHANTOM_DECL_LISTABLE(Type);
PHANTOM_DECL_LISTABLE(Callable);
PHANTOM_DECL_LISTABLE(Enum);
PHANTOM_DECL_LISTABLE(ClassType);
PHANTOM_DECL_LISTABLE(Structure);
PHANTOM_DECL_LISTABLE(Union);
PHANTOM_DECL_LISTABLE(VirtualMethodTable);
PHANTOM_DECL_LISTABLE(Namespace);
PHANTOM_DECL_LISTABLE(TemplateSpecialization);
PHANTOM_DECL_LISTABLE(TemplateParameter);
PHANTOM_DECL_LISTABLE(Template);
PHANTOM_DECL_LISTABLE(Symbol);
PHANTOM_DECL_LISTABLE(Instruction);
PHANTOM_DECL_LISTABLE(FunctorProperty);
PHANTOM_DECL_LISTABLE(Aggregate);
class FunctorProperty;

HAUNT_SOURCE("FunctorProperty")
using FunctorProperties = SmallVector<FunctorProperty*>;
HAUNT_END("FunctorProperty")

using LanguageElementsView = ArrayView<LanguageElement*>;
using TypesView = ArrayView<Type*>;

HAUNT_PAUSE;
template<class, class t_Base = Type>
class TypeT;
template<class, class t_Base = ClassType>
class ClassTypeT;
template<class, class t_Base = Union>
class UnionT;
template<class, class t_Base = Structure>
class StructureT;
template<class, class t_Base = Class>
class ClassT;
HAUNT_RESUME;

HAUNT_END("phantom.reflection");
} // namespace reflection

namespace reflection
{
class ModuleRegistrationInfo;
}

class Object;
struct EmbeddedRtti;
struct EmbeddedProxyRtti;

struct FunctorID
{
    FunctorID() = default;
    FunctorID(size_t a_Lo, size_t a_Hi) : lo(a_Lo), hi(a_Hi)
    {
    }
    FunctorID(void* a_Lo, void* a_Hi) : lo(size_t(a_Lo)), hi(size_t(a_Hi))
    {
    }

    bool operator==(FunctorID const& a_Other) const
    {
        return lo == a_Other.lo && hi == a_Other.hi;
    }
    bool operator!=(FunctorID const& a_Other) const
    {
        return lo != a_Other.lo || hi != a_Other.hi;
    }

    bool operator<(FunctorID a_Other) const
    {
        if (hi == a_Other.hi)
        {
            return lo < a_Other.lo;
        }
        return hi < a_Other.hi;
    }

    bool isNull() const
    {
        return lo == 0 && hi == 0;
    }

    operator bool() const
    {
        return !isNull();
    }

private:
    size_t lo = 0;
    size_t hi = 0;
};
} // namespace phantom
