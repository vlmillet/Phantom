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
#include <phantom/lang/NativeVTableSizeComputer.h>

namespace phantom
{
namespace lang
{
template<typename t_Ty, typename t_Base>
class ClassT : public ClassTypeT<t_Ty, t_Base>
{
    typedef ClassTypeT<t_Ty, t_Base> base_type;
    typedef ClassT<t_Ty, t_Base>     SelfType;

public:
    ClassT(StringView name, Modifiers a_Modifiers = 0)
        : ClassTypeT<t_Ty, t_Base>(
          name,
          a_Modifiers |
          Modifiers(std::is_abstract<t_Ty>::value * PHANTOM_R_ABSTRACT | std::is_final<t_Ty>::value * PHANTOM_R_FINAL))
    {
    }

    VirtualMethodTable* createVirtualMethodTable() override
    {
        void** ppNativeVTable = nullptr;
        return Class::CreateVirtualMethodTable(this, ppNativeVTable, virtualMethodCountOf<t_Ty>());
    }

    VirtualMethodTable* deriveVirtualMethodTable(VirtualMethodTable* a_pVirtualMethodTable) override
    {
        PHANTOM_ASSERT(this->asClass());
        size_t uiOffset = ((Class*)this)->getBaseClassOffsetCascade(Class::VTablePrimaryClass(a_pVirtualMethodTable));
        if (uiOffset == 0)
        {
            return Class::DeriveVirtualMethodTable(this, a_pVirtualMethodTable, virtualMethodCountOf<t_Ty>());
        }
        else
        {
            return Class::DeriveVirtualMethodTable(this, a_pVirtualMethodTable);
        }
    }

    virtual void onElementsAccess() override { this->_onNativeElementsAccess(); }
};

template<typename t_Ty, typename t_Base>
struct TypeOf<ClassT<t_Ty, t_Base> >
{
    static Type* object() { return Class::metaClass; }
};

template<typename t_Ty, typename t_Base>
struct MetaTypeOf<ClassT<t_Ty, t_Base> >
{
    typedef MetaTypeOf<Class>::type type;
};

} // namespace lang

} // namespace phantom
