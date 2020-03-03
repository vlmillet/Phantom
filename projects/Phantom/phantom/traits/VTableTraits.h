// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>
#include <phantom/customize.h>

HAUNT_STOP;

// this file is not used in case we don't want compile time mode
#if !PHANTOM_CUSTOM_ENABLE_COMPILE_TIME_MODE
#    error vtable_traits.h shouldn't be included if 'PHANTOM_CUSTOM_ENABLE_COMPILE_TIME_MODE' is not true 
#endif

namespace phantom
{
template<typename t_Ty>
struct VTableCountOf;
template<typename t_Ty, int index>
struct VTableOffsetOf;
template<typename t_Ty, int index>
struct VTableBaseClassOf;
template<typename t_Ty, int index>
struct VTableTopClassOf;

namespace detail
{
template<typename t_Ty, size_t t_base_class_count>
struct VTableCountOfH_Recursive
{
    enum
    {
        value =
        +VTableCountOf<PHANTOM_TYPENAME BaseClassOf<t_Ty, t_base_class_count - 1>::type>::value +
        VTableCountOfH_Recursive<t_Ty, t_base_class_count - 1>::value
    };
};

template<typename t_Ty>
struct VTableCountOfH_Recursive<t_Ty, 0>
{
    enum
    {
        value = 1
    };
};

template<typename t_Ty>
struct VTableCountOfH_Recursive<t_Ty, 1>
{
    enum
    {
        value = VTableCountOf<PHANTOM_TYPENAME BaseClassOf<t_Ty, 0>::type>::value
    };
};

template<typename t_Ty, bool t_is_polymorphic>
struct VTableCountOfH : public VTableCountOfH_Recursive<t_Ty, BaseClassCountOf<t_Ty>::value>
{
};

template<typename t_Ty>
struct VTableCountOfH<t_Ty, false>
{
    enum
    {
        value = 0
    };
};

template<typename t_Ty, int id, size_t t_base_class_count, size_t offset>
struct VTableOffsetOfRecursive
{
    const static size_t base_index = BaseClassCountOf<t_Ty>::value - t_base_class_count;
    const static size_t temp_value =
    VTableOffsetOf<PHANTOM_TYPENAME BaseClassOf<t_Ty, base_index>::type, id>::value;
    const static size_t value = (temp_value == 0xffffff) // Not matching
    ? VTableOffsetOfRecursive<
      t_Ty, id - VTableCountOf<PHANTOM_TYPENAME BaseClassOf<t_Ty, base_index>::type>::value,
      t_base_class_count - 1,
      offset + sizeof(PHANTOM_TYPENAME BaseClassOf<t_Ty, base_index>::type)>::value
    : (offset + temp_value);
};

template<typename t_Ty, int id, size_t offset>
struct VTableOffsetOfRecursive<t_Ty, id, 0, offset>
{
    const static size_t value = (id == 0) ? offset : 0xffffff;
};

template<typename t_Ty, int id, size_t offset>
struct VTableOffsetOfRecursive<t_Ty, id, 1, offset>
{
    const static size_t base_index = BaseClassCountOf<t_Ty>::value - 1;
    const static size_t temp_value =
    VTableOffsetOf<PHANTOM_TYPENAME BaseClassOf<t_Ty, base_index>::type, id>::value;
    const static size_t value = (temp_value == 0xffffff) // Not matching
    ? (size_t)0xffffff
    : (offset + temp_value);
};

template<typename t_Ty, int id, bool t_is_polymorphic>
struct VTableOffsetOfH : public VTableOffsetOfRecursive<t_Ty, id, BaseClassCountOf<t_Ty>::value, 0>
{
};

template<typename t_Ty, int id>
struct VTableOffsetOfH<t_Ty, id, false>
{
    const static size_t value = 0xffffffff;
};

template<bool t_cond, typename t_True, typename t_False>
struct VTableClassTypeSelector
{
    typedef t_True type;
};

template<typename t_True, typename t_False>
struct VTableClassTypeSelector<false, t_True, t_False>
{
    typedef t_False type;
};

template<typename t_Ty, int id, size_t t_base_class_count>
struct VTableBaseClassOfRecursive
{
    const static size_t base_index = BaseClassCountOf<t_Ty>::value - t_base_class_count;
    typedef
    typename VTableBaseClassOf<typename BaseClassOf<t_Ty, base_index>::type, id>::type temp_type;
    typedef typename VTableClassTypeSelector<
    std::IsVoid<temp_type>::value // Not matching
    ,
    typename VTableBaseClassOfRecursive<
    t_Ty, id - VTableCountOf<typename BaseClassOf<t_Ty, base_index>::type>::value,
    t_base_class_count - 1>::type,
    temp_type>::type type;
};

template<typename t_Ty, int id>
struct VTableBaseClassOfRecursive<t_Ty, id, 0>
{
    typedef typename VTableClassTypeSelector<(id == 0), t_Ty, void>::type type;
};

template<typename t_Ty, int id>
struct VTableBaseClassOfRecursive<t_Ty, id, 1>
{
    const static size_t base_index = BaseClassCountOf<t_Ty>::value - 1;
    typedef
    typename VTableBaseClassOf<typename BaseClassOf<t_Ty, base_index>::type, id>::type temp_type;
    typedef typename VTableClassTypeSelector<std::IsVoid<temp_type>::value // Not matching
                                             ,
                                             void, temp_type>::type                    type;
};

template<typename t_Ty, int id, bool t_is_polymorphic>
struct VTableBaseClassOfH
    : public VTableBaseClassOfRecursive<t_Ty, id, BaseClassCountOf<t_Ty>::value>
{
};

template<typename t_Ty, int id>
struct VTableBaseClassOfH<t_Ty, id, false>
{
    typedef void type;
};

template<typename t_Ty, int id, size_t t_base_class_count>
struct VTableTopClassOfRecursive
{
    const static size_t base_index = BaseClassCountOf<t_Ty>::value - t_base_class_count;
    typedef
    typename VTableTopClassOf<typename BaseClassOf<t_Ty, base_index>::type, id>::type temp_type;
    typedef typename VTableClassTypeSelector<
    std::IsVoid<temp_type>::value // Not matching
    ,
    typename VTableTopClassOfRecursive<
    t_Ty, id - VTableCountOf<typename BaseClassOf<t_Ty, base_index>::type>::value,
    t_base_class_count - 1>::type,
    temp_type>::type type;
};

template<typename t_Ty, int id>
struct VTableTopClassOfRecursive<t_Ty, id, 0>
{
    typedef typename VTableClassTypeSelector<(id == 0), t_Ty, void>::type type;
};

template<typename t_Ty, int id>
struct VTableTopClassOfRecursive<t_Ty, id, 1>
{
    const static size_t base_index = BaseClassCountOf<t_Ty>::value - 1;
    typedef
    typename VTableTopClassOf<typename BaseClassOf<t_Ty, base_index>::type, id>::type temp_type;
    typedef typename VTableClassTypeSelector<std::IsVoid<temp_type>::value // Not matching
                                             ,
                                             void, temp_type>::type                   type;
};

template<typename t_Ty, int id, bool t_is_polymorphic>
struct VTableTopClassOfH : public VTableTopClassOfRecursive<t_Ty, id, BaseClassCountOf<t_Ty>::value>
{
};

template<typename t_Ty, int id>
struct VTableTopClassOfH<t_Ty, id, false>
{
    typedef void type;
};

template<typename t_Ty>
struct VTableTopClassOfH<t_Ty, 0, true>
{
    typedef t_Ty type;
};

template<typename t_Ty, int t_vtable_count>
struct VTableInfoExtractorRecursive
{
    const static size_t vtable_offset = VTableOffsetOf<t_Ty, t_vtable_count - 1>::value;
    typedef typename VTableTopClassOf<t_Ty, t_vtable_count - 1>::type top_class;
    static void apply(const void* a_pInstance, vector<vtable_info>& vtables)
    {
        size_t      instanceVirtualMethodCount = virtualMethodCountOf<top_class>();
        vtable_info vtable;
        vtable.count = instanceVirtualMethodCount;
        vtable.offset = vtable_offset;
        vtable.member_functions = *((void***)((byte*)a_pInstance + vtable_offset));
        VTableInfoExtractorRecursive<t_Ty, t_vtable_count - 1>::apply(a_pInstance, vtables);
        vtables.push_back(vtable);
    }
};

template<typename t_Ty>
struct VTableInfoExtractorRecursive<t_Ty, 0>
{
    static void apply(const void* a_pInstance, vector<vtable_info>& vtables)
    {
    }
};

template<typename t_Ty, bool t_is_polymorphic>
struct VTableInfoExtractorH : public VTableInfoExtractorRecursive<t_Ty, VTableCountOf<t_Ty>::value>
{
};

template<typename t_Ty>
struct VTableInfoExtractorH<t_Ty, false>
{
    static void apply(const void* a_pInstance, vector<vtable_info>& vtables)
    {
        PHANTOM_ASSERT("Non-polymorphic type doesn't have vtables so they cannot be copied");
    }
};

} // namespace detail

template<typename t_Ty>
struct VTableCountOf : public detail::VTableCountOfH<t_Ty, std::is_polymorphic<t_Ty>::value>
{
};

template<typename t_Ty, int id>
struct VTableOffsetOf : public detail::VTableOffsetOfH<t_Ty, id, std::is_polymorphic<t_Ty>::value>
{
};

template<typename t_Ty, int id>
struct VTableBaseClassOf
    : public detail::VTableBaseClassOfH<t_Ty, id, std::is_polymorphic<t_Ty>::value>
{
};
template<typename t_Ty, int id>
struct VTableTopClassOf
    : public detail::VTableTopClassOfH<t_Ty, id, std::is_polymorphic<t_Ty>::value>
{
};

template<typename t_Ty>
struct VTableInfoExtractor
    : public detail::VTableInfoExtractorH<t_Ty, std::is_polymorphic<t_Ty>::value>
{
};

} // namespace phantom
