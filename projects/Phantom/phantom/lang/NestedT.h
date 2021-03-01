// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

HAUNT_STOP;

#include <phantom/lang/Class.h>
#include <phantom/lang/ClassTypeT.h>
#include <phantom/lang/NativeVTableInspector.h>
#include <phantom/lang/NativeVTablePointerExtractor.h>
#include <phantom/lang/NativeVTableSizeComputer.h>
#include <phantom/lang/VirtualMethodTable.h>

namespace phantom
{
namespace lang
{
template<typename t_Ty, int t_meta_flags>
struct TplNestedModifiersFilter;

template<typename t_Ty, bool t_no_copy>
struct TplNestedModifiersFilterCopier
{
    static void copy(t_Ty* a_pDest, t_Ty const* a_pSrc)
    {
        PHANTOM_ASSERT(false,
                       "copy forbidden for the given type, remove an eventual 'PHANTOM_R_FLAG_NO_COPY' meta "
                       "specifier to enable this member_function for your class");
    }
};
template<typename t_Ty>
struct TplNestedModifiersFilterCopier<t_Ty, false> : public phantom::Copier<t_Ty>
{
};

template<typename t_Ty, int t_meta_flags>
struct TplNestedModifiersFilter<phantom::Copier<t_Ty>, t_meta_flags>
    : public TplNestedModifiersFilterCopier<t_Ty, (t_meta_flags & PHANTOM_R_FLAG_NO_COPY) == PHANTOM_R_FLAG_NO_COPY>
{
};

template<typename t_Ty, uint t_meta_flags = 0>
class NestedT : public MetaTypeOf<t_Ty>::type
{
public:
    static const uint MetaFlags = t_meta_flags;

    typedef PHANTOM_TYPENAME MetaTypeOf<t_Ty>::type BaseType;

    NestedT(StringView name, Modifiers a_Modifiers = 0) : BaseType(name, a_Modifiers)
    {
    }

    virtual bool hasCopyDisabled() const
    {
        return HasCopyDisabled<t_Ty>::value ||((t_meta_flags & PHANTOM_R_FLAG_NO_COPY) == PHANTOM_R_FLAG_NO_COPY);
    }

    virtual void copy(void* a_pDest, void const* a_pSrc) const
    {
        TplNestedModifiersFilter<phantom::Copier<t_Ty>, t_meta_flags>::copy(static_cast<t_Ty*>(a_pDest),
                                                                            static_cast<t_Ty const*>(a_pSrc));
    }
};

template<typename t_Ty, int t_ms>
struct TypeOf<NestedT<t_Ty, t_ms> >
{
    static Type* object()
    {
        return Class::metaClass;
    }
};

template<typename t_Ty, int t_ms>
struct MetaTypeOf<NestedT<t_Ty, t_ms> >
{
    typedef MetaTypeOf<Class>::type type;
};

} // namespace lang
} // namespace phantom
