// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <cstdarg> // for 'va_list'
#include <ctime>   // for 'size_t'
#include <haunt>
#include <phantom/plugin.h>
#include <utility>

#define PHANTOM_DEF_LIST(type, ...) typedef phantom::SmallVector<type*, ##__VA_ARGS__> type##s;
#define PHANTOM_DEF_LIST_S(type, ...) typedef phantom::SmallVector<type*, ##__VA_ARGS__> type##es;
#define PHANTOM_DECL_LISTABLE(type, ...)                                                                               \
    HAUNT_SOURCE(#type) HAUNT_OFF class type;                                                                          \
    PHANTOM_DEF_LIST(type, ##__VA_ARGS__) HAUNT_END()
#define PHANTOM_DECL_LISTABLE_S(type, ...)                                                                             \
    HAUNT_SOURCE(#type) HAUNT_OFF class type;                                                                          \
    PHANTOM_DEF_LIST_S(type, ##__VA_ARGS__) HAUNT_END()
#define PHANTOM_DECL_TD_LISTABLE(origin, type, ...)                                                                    \
    HAUNT_SOURCE(#type) typedef origin type;                                                                           \
    typedef origin##s                  type##s;                                                                        \
    HAUNT_END()

namespace phantom
{
HAUNT_PAUSE;

template<class T>
class BasicStringView;

template<class S>
class Delegate;

template<class T, ::size_t StaticAllocSize>
class SmallString;

template<class>
struct Less;

template<class T, ::size_t StaticAllocSize = 4, class Pred = Less<T>>
class SmallSet;

template<class K, class V, ::size_t StaticAllocSize = 4, class Pred = Less<K>>
class SmallMap;

template<class K, class V, ::size_t StaticAllocSize = 4, class Pred = Less<K>>
class SmallMultimap;

template<class T, ::size_t StaticAllocSize = 4>
class SmallVector;

template<class T>
class ArrayView;

template<class T>
class Optional;

/// @cond ADVANCED
class Phantom;

template<class S>
class Delegate;

class Variant;
class DummyClass
{
};

class Object;

template<class>
struct Allocator;

template<class>
struct Copier;

template<class t_Ty>
struct DynamicDeleter;

template<class t_Ty>
struct DefaultAllocator;

template<class t_Ty>
struct DefaultConstructor;

template<class t_Ty>
struct DefaultInitializer;

template<class t_Ty>
struct DefaultInstaller;

template<class>
struct NewMetaH;

template<class>
struct DeleteMetaH;

template<class t_Ty>
struct DynamicDeleteExH;

template<class t_Ty>
struct DynamicDeleteMetaHelper;

namespace detail
{
class DynamicCppInitializerH;

template<class t_Ty, int>
struct DefaultInstallerH;

template<class t_Ty, int>
struct DefaultInitializerH;

} // namespace detail

struct Path;

HAUNT_RESUME;

HAUNT_PACKAGE("phantom");
class MemoryContext;
HAUNT_END("phantom");
struct UserData;

template<class S>
class Functor;

struct NullMutex
{
    void lock() {}
    void unlock() noexcept {}
    bool try_lock() { return true; }
};

template<class S, class Mtx = NullMutex>
class Signal;

/// @endcond

typedef SmallVector<Variant, 10> Variants;

} // namespace phantom
