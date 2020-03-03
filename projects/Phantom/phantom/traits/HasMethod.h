// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/traits/prerequisites.h>

/// @cond ADVANCED

namespace phantom
{
namespace detail
{
template<typename type>
class HasMethod_void_exp_result
{
};
template<typename type, typename U>
U const& operator,(U const&, detail::HasMethod_void_exp_result<type>);
template<typename type, typename U>
U& operator,(U&, HasMethod_void_exp_result<type>);
template<typename src_type, typename dest_type>
struct HasMethod_clone_constness
{
    typedef dest_type type;
};
template<typename src_type, typename dest_type>
struct HasMethod_clone_constness<const src_type, dest_type>
{
    typedef const dest_type type;
};
} // namespace detail

#define PHANTOM_DEFINE_HAS_METHOD_CONST_0(TraitName, foo)                                          \
    template<typename Type>                                                                        \
    class _____##TraitName                                                                         \
    {                                                                                              \
        class yes                                                                                  \
        {                                                                                          \
            char m;                                                                                \
        };                                                                                         \
        class no                                                                                   \
        {                                                                                          \
            yes m[2];                                                                              \
        };                                                                                         \
        struct BaseMixin                                                                           \
        {                                                                                          \
            void foo() const                                                                       \
            {                                                                                      \
            }                                                                                      \
        };                                                                                         \
        struct Base : public Type, public BaseMixin                                                \
        {                                                                                          \
        };                                                                                         \
        template<typename T, T t>                                                                  \
        class Helper                                                                               \
        {                                                                                          \
        };                                                                                         \
        template<typename U>                                                                       \
        static no deduce(                                                                          \
        U*, Helper<PHANTOM_TYPENAME Type::const_iterator (BaseMixin::*)() const, &U::foo>* = 0);   \
        static yes deduce(...);                                                                    \
                                                                                                   \
    public:                                                                                        \
        static const bool result = sizeof(yes) == sizeof(deduce((Base*)(0)));                      \
    };                                                                                             \
                                                                                                   \
    template<typename type, typename call_details>                                                 \
    struct TraitName                                                                               \
    {                                                                                              \
    private:                                                                                       \
        class yes                                                                                  \
        {                                                                                          \
        };                                                                                         \
        class no                                                                                   \
        {                                                                                          \
            yes m[2];                                                                              \
        };                                                                                         \
        struct derived : public type                                                               \
        {                                                                                          \
            using type::foo;                                                                       \
            no foo(...) const;                                                                     \
        };                                                                                         \
        typedef typename detail::HasMethod_clone_constness<type, derived>::type derived_type;      \
        template<typename T, typename due_type>                                                    \
        struct return_value_check                                                                  \
        {                                                                                          \
            static yes deduce(due_type);                                                           \
            static no  deduce(...);                                                                \
            static no  deduce(no);                                                                 \
            static no  deduce(detail::HasMethod_void_exp_result<type>);                            \
        };                                                                                         \
        template<typename T>                                                                       \
        struct return_value_check<T, PHANTOM_TYPENAME type::const_iterator>                        \
        {                                                                                          \
            static yes deduce(...);                                                                \
            static no  deduce(no);                                                                 \
        };                                                                                         \
        template<bool has, typename F>                                                             \
        struct impl                                                                                \
        {                                                                                          \
            static const bool value = false;                                                       \
        };                                                                                         \
        template<typename r>                                                                       \
        struct impl<true, r()>                                                                     \
        {                                                                                          \
            static const bool value =                                                              \
            sizeof(return_value_check<type, r>::deduce(                                            \
            (((derived_type const*)0)->foo(), detail::HasMethod_void_exp_result<type>()))) ==      \
            sizeof(yes);                                                                           \
        };                                                                                         \
                                                                                                   \
    public:                                                                                        \
        static const bool value = impl<_____##TraitName<type>::result, call_details>::value;       \
    };

} // namespace phantom

#define PHANTOM_DEFINE_HAS_METHOD(TraitName, foo) _PHNTM_DEFINE_HAS_METHOD(TraitName, foo, )
#define PHANTOM_DEFINE_HAS_METHOD_CONST(TraitName, foo)                                            \
    _PHNTM_DEFINE_HAS_METHOD(TraitName, foo, const)

#define _PHNTM_DEFINE_HAS_METHOD(TraitName, foo, opt_const)                                        \
    template<typename Type>                                                                        \
    class ____##TraitName                                                                          \
    {                                                                                              \
        class yes                                                                                  \
        {                                                                                          \
            char m;                                                                                \
        };                                                                                         \
        class no                                                                                   \
        {                                                                                          \
            yes m[2];                                                                              \
        };                                                                                         \
        struct BaseMixin                                                                           \
        {                                                                                          \
            void foo() opt_const                                                                   \
            {                                                                                      \
            }                                                                                      \
        };                                                                                         \
        struct Base : public Type, public BaseMixin                                                \
        {                                                                                          \
        };                                                                                         \
        template<typename T, T t>                                                                  \
        class Helper                                                                               \
        {                                                                                          \
        };                                                                                         \
        template<typename U>                                                                       \
        static no  deduce(U*, Helper<void (BaseMixin::*)() opt_const, &U::foo>* = 0);              \
        static yes deduce(...);                                                                    \
                                                                                                   \
    public:                                                                                        \
        static const bool result = sizeof(yes) == sizeof(deduce((Base*)(0)));                      \
    };                                                                                             \
                                                                                                   \
    template<typename type, typename call_details>                                                 \
    struct TraitName                                                                               \
    {                                                                                              \
    private:                                                                                       \
        class yes                                                                                  \
        {                                                                                          \
        };                                                                                         \
        class no                                                                                   \
        {                                                                                          \
            yes m[2];                                                                              \
        };                                                                                         \
        struct derived : public type                                                               \
        {                                                                                          \
            using type::foo;                                                                       \
            no foo(...) const;                                                                     \
        };                                                                                         \
        typedef                                                                                    \
        typename ::phantom::detail::HasMethod_clone_constness<type, derived>::type derived_type;   \
        template<typename T, typename due_type>                                                    \
        struct return_value_check                                                                  \
        {                                                                                          \
            static yes deduce(due_type);                                                           \
            static no  deduce(...);                                                                \
            static no  deduce(no);                                                                 \
            static no  deduce(::phantom::detail::HasMethod_void_exp_result<type>);                 \
        };                                                                                         \
        template<typename T>                                                                       \
        struct return_value_check<T, void>                                                         \
        {                                                                                          \
            static yes deduce(...);                                                                \
            static no  deduce(no);                                                                 \
        };                                                                                         \
        template<bool has, typename F>                                                             \
        struct impl                                                                                \
        {                                                                                          \
            static const bool value = false;                                                       \
        };                                                                                         \
        template<typename r, class... args>                                                        \
        struct impl<true, r(args...)>                                                              \
        {                                                                                          \
            static const bool value =                                                              \
            sizeof(return_value_check<type, r>::deduce(                                            \
            (((derived_type opt_const*)0)                                                          \
             ->foo((*(PHANTOM_TYPENAME std::remove_reference<args>::type*)0)...),                  \
             ::phantom::detail::HasMethod_void_exp_result<type>()))) == sizeof(yes);               \
        };                                                                                         \
                                                                                                   \
    public:                                                                                        \
        static const bool value = impl<____##TraitName<type>::result, call_details>::value;        \
    };

/// @endcond

#define _PHNTM_DEFINE_HAS_METHOD_L1(TraitName, name) _PHNTM_DEFINE_HAS_METHOD_L0(TraitName, name)

#define _PHNTM_DEFINE_HAS_METHOD_L0(TraitName, name) PHANTOM_DEFINE_HAS_METHOD(TraitName, name)
