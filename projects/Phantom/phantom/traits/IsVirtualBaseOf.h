// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

namespace phantom
{
namespace detail
{
#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    pragma warning(push)
#    pragma warning(disable : 4584 4250)
#elif defined(__GNUC__) && (__GNUC__ >= 4)
#    pragma GCC system_header
#endif

template<typename Base, typename Derived, typename tag>
struct is_virtual_base_of_impl : std::integral_constant<bool, false>
{
};

template<typename Base, typename Derived>
struct is_virtual_base_of_impl<Base, Derived, std::true_type>
{
    union max_align {
        unsigned      u;
        unsigned long ul;
        void*         v;
        double        d;
        long double   ld;
        long long     ll;
    };
    struct type_traits_internal_struct_X : public Derived, virtual Base
    {
        type_traits_internal_struct_X();
        type_traits_internal_struct_X(const type_traits_internal_struct_X&);
        type_traits_internal_struct_X& operator=(const type_traits_internal_struct_X&);
        ~type_traits_internal_struct_X() throw();
        max_align data[16];
    };
    struct type_traits_internal_struct_Y : public Derived
    {
        type_traits_internal_struct_Y();
        type_traits_internal_struct_Y(const type_traits_internal_struct_Y&);
        type_traits_internal_struct_Y& operator=(const type_traits_internal_struct_Y&);
        ~type_traits_internal_struct_Y() throw();
        max_align data[16];
    };
    static const bool value =
    (sizeof(type_traits_internal_struct_X) == sizeof(type_traits_internal_struct_Y));
};

template<typename Base, typename Derived>
struct is_virtual_base_of_impl2
{
    typedef std::integral_constant<
    bool, (std::is_base_of<Base, Derived>::value && !std::is_same<Base, Derived>::value)>
                                                             tag_type;
    typedef is_virtual_base_of_impl<Base, Derived, tag_type> imp;
    static const bool                                        value = imp::value;
};

#if PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO
#    pragma warning(pop)
#endif

} // namespace detail

template<class Base, class Derived>
struct IsVirtualBaseOf : public std::integral_constant<
                         bool, (::phantom::detail::is_virtual_base_of_impl2<Base, Derived>::value)>
{
};

template<class Base, class Derived>
struct IsVirtualBaseOf<Base&, Derived> : std::false_type
{
};
template<class Base, class Derived>
struct IsVirtualBaseOf<Base, Derived&> : std::false_type
{
};
template<class Base, class Derived>
struct IsVirtualBaseOf<Base&, Derived&> : std::false_type
{
};

} // namespace phantom
