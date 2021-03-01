// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>

HAUNT_STOP;
#include "phantom/traits/HasStaticFunction.h"

#include <phantom/lang/Class.h>
#include <phantom/lang/TypeInfos.h>
#include <phantom/lang/Undefineds.h>
#include <phantom/lang/reflection.h>
#include <phantom/traits/IsDataPointer.h>
#include <phantom/traits/IsDefinedInContext.h>
#include <phantom/traits/IsFunctionPointer.h>
#include <phantom/traits/IsNullptrT.h>
#include <phantom/traits/IsStructure.h>
#include <phantom/traits/IsVoid.h>
#include <phantom/traits/Promote.h>

/// @cond ADVANCED

#define PHANTOM_TYPE_BY_NAME(...) (::phantom::lang::TypeOfUndefined<__VA_ARGS__>::object())

#define PHANTOM_PRECISE_TYPEOF(...) (phantom::lang::PreciseTypeOf<__VA_ARGS__>::object())

namespace phantom
{
PHANTOM_EXPORT_PHANTOM size_t _dllModuleHandleFromAddress(void const*);

namespace detail
{
PHANTOM_EXPORT_PHANTOM bool installed();
}
namespace lang
{
template<typename>
struct TypeOfUndefined;
template<typename>
struct MetaTypeOf;
template<typename>
struct TemplateSpecializationRegistrer;
} // namespace lang

namespace lang
{
namespace detail
{
PHANTOM_EXPORT_PHANTOM Module* currentModule();
PHANTOM_EXPORT_PHANTOM Type* findUsableType(size_t a_ModuleHandle, const TypeInfos& a_TypeInfos);
PHANTOM_EXPORT_PHANTOM Type*      findUsableType(const TypeInfos& a_TypeInfos);
PHANTOM_EXPORT_PHANTOM StringView getModuleName(size_t a_ModuleHandle);
PHANTOM_EXPORT_PHANTOM StringView getModuleName(Type*);
PHANTOM_EXPORT_PHANTOM size_t     getModuleHandle(Type*);
} // namespace detail

#if defined(PHANTOM_SAFE_SYMBOL_CAST)
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Symbol* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Type* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(PrimitiveType* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Namespace* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Enum* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Class* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Structure* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(MethodPointer* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(FieldPointer* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(FunctionPointer* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Pointer* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(LValueReference* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(RValueReference* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(ConstType* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(ConstVolatileType* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(VolatileType* a_pElement);
PHANTOM_EXPORT_PHANTOM Symbol* symbol_cast(Array* a_pElement);

PHANTOM_EXPORT_PHANTOM Type* type_cast(Type* a_pElement)
{
    return a_pElement;
}
PHANTOM_EXPORT_PHANTOM Type* type_cast(PrimitiveType* a_pElement);
PHANTOM_EXPORT_PHANTOM Type* type_cast(Enum* a_pElement);
PHANTOM_EXPORT_PHANTOM Type* type_cast(Class* a_pElement);
PHANTOM_EXPORT_PHANTOM Type* type_cast(Structure* a_pElement);
PHANTOM_EXPORT_PHANTOM Type* type_cast(MethodPointer* a_pElement);
PHANTOM_EXPORT_PHANTOM Type* type_cast(FieldPointer* a_pElement);
PHANTOM_EXPORT_PHANTOM Type* type_cast(FunctionPointer* a_pElement);
PHANTOM_EXPORT_PHANTOM Type* type_cast(Pointer* a_pElement);
PHANTOM_EXPORT_PHANTOM Type* type_cast(LValueReference* a_pElement);
PHANTOM_EXPORT_PHANTOM Type* type_cast(RValueReference* a_pElement);
PHANTOM_EXPORT_PHANTOM Type* type_cast(ConstType* a_pElement);
PHANTOM_EXPORT_PHANTOM Type* type_cast(ConstVolatileType* a_pElement);
PHANTOM_EXPORT_PHANTOM Type* type_cast(VolatileType* a_pElement);
PHANTOM_EXPORT_PHANTOM Type* type_cast(Array* a_pElement);

#    define PHANTOM_R_TYPE_CAST(...) ::phantom::lang::type_cast(__VA_ARGS__)
#    define PHANTOM_R_SYM_CAST(...) ::phantom::lang::symbol_cast(__VA_ARGS__)

#else
// dirty c-style cast to avoid includes (we know that every Type derived class is single inherited
#    define PHANTOM_R_TYPE_CAST(...) ((::phantom::lang::Type*)(__VA_ARGS__)) //((::phantom::lang::Type*)(__VA_ARGS__))
#    define PHANTOM_R_SYM_CAST(...)                                                                                    \
        ((::phantom::lang::Symbol*)(__VA_ARGS__)) //((::phantom::lang::Symbol*)(__VA_ARGS__))

#endif

namespace detail
{
enum meta_type_id
{
    meta_volatile_type,
    meta_const_volatile_type,
    meta_const_type,

    meta_data_pointer_type,
    meta_function_pointer_type,

    meta_member_pointer_type,
    meta_data_member_pointer_type,
    meta_member_function_pointer_type,

    meta_lvalue_reference_type,
    meta_rvalue_reference_type,

    meta_fundamental_type,
    meta_arithmetic_type,
    meta_integral_type,
    meta_floating_point_type,

    meta_structure,
    meta_class,
    meta_union,

    meta_special,

    meta_enum,

    meta_array,

    meta_nullptr_type,

    meta_todo,
};

template<typename t_Ty>
struct MetaTypeIdOf
{
    const static int value = std::is_const<t_Ty>::value
    ? std::is_volatile<t_Ty>::value ? meta_const_volatile_type : meta_const_type
    : std::is_volatile<t_Ty>::value ? meta_volatile_type
                                    : std::is_array<t_Ty>::value
    ? meta_array
    : std::is_floating_point<t_Ty>::value ? meta_floating_point_type
                                          : ::phantom::IsNullptrT<t_Ty>::value ? meta_nullptr_type
                                                                               : std::is_integral<t_Ty>::value
    ? meta_integral_type
    : std::is_enum<t_Ty>::value ? meta_enum
                                : (std::is_fundamental<t_Ty>::value) ? meta_fundamental_type
                                                                     : phantom::IsDataPointer<t_Ty>::value
    ? meta_data_pointer_type
    : std::is_lvalue_reference<t_Ty>::value ? meta_lvalue_reference_type
                                            : std::is_rvalue_reference<t_Ty>::value
    ? meta_rvalue_reference_type
    : std::is_pointer<t_Ty>::value ? meta_function_pointer_type
                                   : std::is_member_function_pointer<t_Ty>::value
    ? meta_member_function_pointer_type
    : std::is_member_object_pointer<t_Ty>::value ? meta_data_member_pointer_type
                                                 : std::is_union<t_Ty>::value
    ? meta_union
    : std::is_class<t_Ty>::value ? IsStructure<t_Ty>::value ? meta_structure : meta_class : meta_todo;
};

template<typename t_Ty, int t_id>
struct MetaTypeOfH
{
    static_assert(std::is_same<t_Ty, t_Ty*>::value, "MetaTypeOf not defined for the current type");
};

#define _PHNTM_SPEC_MTYPE_BSE_CLASS_SLVER(_meta_type_id_, ...)                                                         \
    template<typename t_Ty>                                                                                            \
    struct MetaTypeOfH<t_Ty, _meta_type_id_>                                                                           \
    {                                                                                                                  \
        typedef __VA_ARGS__ type;                                                                                      \
    };

_PHNTM_SPEC_MTYPE_BSE_CLASS_SLVER(meta_fundamental_type, phantom::lang::FundamentalTypeT<t_Ty>);
_PHNTM_SPEC_MTYPE_BSE_CLASS_SLVER(meta_arithmetic_type, phantom::lang::ArithmeticTypeT<t_Ty>);
_PHNTM_SPEC_MTYPE_BSE_CLASS_SLVER(meta_floating_point_type, phantom::lang::FloatingPointTypeT<t_Ty>);
_PHNTM_SPEC_MTYPE_BSE_CLASS_SLVER(meta_integral_type, phantom::lang::IntegralTypeT<t_Ty>);
_PHNTM_SPEC_MTYPE_BSE_CLASS_SLVER(meta_nullptr_type, phantom::lang::FundamentalTypeT<t_Ty>);
_PHNTM_SPEC_MTYPE_BSE_CLASS_SLVER(meta_structure, phantom::lang::StructureT<t_Ty>);
_PHNTM_SPEC_MTYPE_BSE_CLASS_SLVER(meta_union, phantom::lang::UnionT<t_Ty>);
_PHNTM_SPEC_MTYPE_BSE_CLASS_SLVER(meta_class, phantom::lang::ClassT<t_Ty>);
_PHNTM_SPEC_MTYPE_BSE_CLASS_SLVER(meta_special, phantom::lang::FundamentalTypeT<t_Ty>);
_PHNTM_SPEC_MTYPE_BSE_CLASS_SLVER(meta_enum, phantom::lang::EnumT<t_Ty>);

#undef _PHNTM_SPEC_MTYPE_BSE_CLASS_SLVER

template<typename t_Ty>
struct PrimitiveTypeKindH;

template<>
struct PrimitiveTypeKindH<void>
{
    const static TypeKind value = TypeKind::Void;
};
template<>
struct PrimitiveTypeKindH<char>
{
    const static TypeKind value = TypeKind::Char;
};
template<>
struct PrimitiveTypeKindH<unsigned char>
{
    const static TypeKind value = TypeKind::UChar;
};
template<>
struct PrimitiveTypeKindH<signed char>
{
    const static TypeKind value = TypeKind::SChar;
};
template<>
struct PrimitiveTypeKindH<short>
{
    const static TypeKind value = TypeKind::Short;
};
template<>
struct PrimitiveTypeKindH<unsigned short>
{
    const static TypeKind value = TypeKind::UShort;
};
template<>
struct PrimitiveTypeKindH<int>
{
    const static TypeKind value = TypeKind::Int;
};
template<>
struct PrimitiveTypeKindH<unsigned int>
{
    const static TypeKind value = TypeKind::UInt;
};
template<>
struct PrimitiveTypeKindH<long>
{
    const static TypeKind value = TypeKind::Long;
};
template<>
struct PrimitiveTypeKindH<unsigned long>
{
    const static TypeKind value = TypeKind::ULong;
};
template<>
struct PrimitiveTypeKindH<long long>
{
    const static TypeKind value = TypeKind::LongLong;
};
template<>
struct PrimitiveTypeKindH<unsigned long long>
{
    const static TypeKind value = TypeKind::ULongLong;
};
template<>
struct PrimitiveTypeKindH<float>
{
    const static TypeKind value = TypeKind::Float;
};
template<>
struct PrimitiveTypeKindH<double>
{
    const static TypeKind value = TypeKind::Double;
};
template<>
struct PrimitiveTypeKindH<long double>
{
    const static TypeKind value = TypeKind::LongDouble;
};
template<>
struct PrimitiveTypeKindH<bool>
{
    const static TypeKind value = TypeKind::Bool;
};
template<>
struct PrimitiveTypeKindH<std::nullptr_t>
{
    const static TypeKind value = TypeKind::NullPtr;
};
#if PHANTOM_HAS_BUILT_IN_WCHAR_T
template<>
struct PrimitiveTypeKindH<wchar_t>
{
    const static TypeKind value = TypeKind::WChar;
};
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR16_T
template<>
struct PrimitiveTypeKindH<char16_t>
{
    const static TypeKind value = TypeKind::Char16;
};
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR32_T
template<>
struct PrimitiveTypeKindH<char32_t>
{
    const static TypeKind value = TypeKind::Char32;
};
#endif

} // namespace detail

template<typename t_Ty>
struct MetaTypeOf : public detail::MetaTypeOfH<t_Ty, detail::MetaTypeIdOf<t_Ty>::value>
{
};

template<int Line>
struct id_tag
{
}; // To be used as a tag depending on current translation unit line

template<typename t_Ty>
struct TypeOfByName
{
    inline static Type* object()
    {
        auto&  infos = TypeInfosOf<t_Ty>::object();
        size_t moduleHandle = PHANTOM_MODULE_HANDLE((void*)&TypeOfByName<t_Ty>::object);
        auto   pType = detail::findUsableType(moduleHandle, infos);
        if (pType == nullptr)
        {
            pType = detail::findUsableType(infos);
            if (pType)
            {
#if PHANTOM_DEBUG_LEVEL == PHANTOM_DEBUG_LEVEL_FULL
                if (detail::getModuleHandle(pType) != moduleHandle)
                {
                    PHANTOM_LOG(Warning,
                                "type '%.*s' was found in module '%.*s' which is not a dependency of the current "
                                "module '%.*s'. This can cause undefined behavior when unloading modules.",
                                PHANTOM_STRING_AS_PRINTF_ARG(infos.qualifiedDecoratedName()),
                                PHANTOM_STRING_AS_PRINTF_ARG(detail::getModuleName(pType)),
                                PHANTOM_STRING_AS_PRINTF_ARG(detail::getModuleName(moduleHandle)));
                }
#endif
            }
            else if (phantom::detail::installed())
            {
                PHANTOM_LOG(Warning,
                            "cannot find type '%.*s' in any module, ensure it has lang "
                            "declared or, if it is a template instance, ensure the lang is "
                            "available above in the compilation unit (.hxx included for example)",
                            PHANTOM_STRING_AS_PRINTF_ARG(infos.qualifiedDecoratedName()));
            }
        }
        return pType;
    }
};

template<typename t_Ty>
struct TypeOfUndefined
{
    inline static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        static Type* s_pCache = nullptr;
        if (s_pCache == nullptr)
        {
            s_pCache = TypeOfByName<t_Ty>::object();
        }
        return s_pCache;
    }
};

template<typename t_Ty>
struct TypeOfDefinedMarker;

template<class T>
T* Class::cast(void* a_pBaseAddress) const
{
    return reinterpret_cast<T*>(cast((Class*)TypeOfUndefined<T>::object(), a_pBaseAddress));
}

template<class T>
T const* Class::cast(void const* a_pBaseAddress) const
{
    return reinterpret_cast<T const*>(cast((Class*)TypeOfUndefined<T>::object(), a_pBaseAddress));
}

template<typename t_Ty>
struct EnumOf
{
    PHANTOM_STATIC_ASSERT(std::is_enum<t_Ty>::value, "t_Ty must be an enum");
    static Enum* object() { return (Enum*)PHANTOM_TYPEOF(t_Ty); }
};

template<typename t_Ty>
struct UnionOf
{
    PHANTOM_STATIC_ASSERT(std::is_union<t_Ty>::value, "t_Ty must be an union");
    static Union* object() { return (Union*)PHANTOM_TYPEOF(t_Ty); }
};

template<typename t_Ty>
struct ClassTypeOf
{
    PHANTOM_STATIC_ASSERT(std::is_class<t_Ty>::value || std::is_union<t_Ty>::value,
                          "t_Ty must be a class, structure or union");
    static ClassType* object() { return (ClassType*)PHANTOM_TYPEOF(t_Ty); }
};

template<typename t_Ty>
struct PreciseTypeOf
{
    static PHANTOM_TYPENAME::phantom::lang::MetaTypeOf<t_Ty>::type* object()
    {
        return (PHANTOM_TYPENAME::phantom::lang::MetaTypeOf<t_Ty>::type*)(PHANTOM_TYPEOF(t_Ty));
    }
};

#define _PHNTM_SIMPLE_CPND_TYPE_TRAIT(Trait, suffix, func)                                                             \
    template<class t_Ty>                                                                                               \
    struct TypeOfUndefined<t_Ty suffix>                                                                                \
    {                                                                                                                  \
        static Type* object()                                                                                          \
        {                                                                                                              \
            PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();                                                                    \
            Type* t = PHANTOM_TYPEOF(t_Ty);                                                                            \
            return t ? (Type*)(t->func) : nullptr;                                                                     \
        }                                                                                                              \
    };

#define _PHNTM_SIMPLE_CPND_TYPE(suffix, func) _PHNTM_SIMPLE_CPND_TYPE_TRAIT(TypeOfUndefined, suffix, func)

_PHNTM_SIMPLE_CPND_TYPE(*, makePointer());
_PHNTM_SIMPLE_CPND_TYPE(&, makeLValueReference());
_PHNTM_SIMPLE_CPND_TYPE(&&, makeRValueReference());
_PHNTM_SIMPLE_CPND_TYPE(const, makeConst());
_PHNTM_SIMPLE_CPND_TYPE(volatile, makeVolatile());
_PHNTM_SIMPLE_CPND_TYPE(const volatile, makeConstVolatile());

#undef _PHNTM_SIMPLE_CPND_TYPE
#undef _PHNTM_SIMPLE_CPND_TYPE_TRAIT

// TypeOfUndefined

template<class t_Ty, size_t t_size>
struct TypeOfUndefined<t_Ty[t_size]>
{
    static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        return (Type*)PHANTOM_TYPEOF(t_Ty)->makeArray(t_size);
    }
};

template<class t_Ty, size_t t_size>
struct TypeOfUndefined<t_Ty const[t_size]>
{
    static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        return (Type*)((Type*)PHANTOM_TYPEOF(t_Ty)->makeConst())->makeArray(t_size);
    }
};

template<class t_Ty>
struct TypeOfUndefined<t_Ty[]>
{
    static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        return (Type*)PHANTOM_TYPEOF(t_Ty)->makeArray(0);
    }
};

template<class t_Ty>
struct TypeOfUndefined<t_Ty const[]>
{
    static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        return (Type*)((Type*)PHANTOM_TYPEOF(t_Ty)->makeConst())->makeArray(0);
    }
};

PHANTOM_EXPORT_PHANTOM FunctionType* functionType(Type* a_pReturnType, TypesView a_ParameterTypes,
                                                  Modifiers a_Qualifiers = Modifier::None);
PHANTOM_EXPORT_PHANTOM FunctionPointer* functionPointerType(Type* a_pReturnType, int a_eABI,
                                                            TypesView a_ParameterTypes);
PHANTOM_EXPORT_PHANTOM FunctionPointer* functionPointerType(FunctionType* a_pFunctionType, ABI a_eABI);
PHANTOM_EXPORT_PHANTOM InitializerListType* initializerListType(TypesView a_Types);
PHANTOM_EXPORT_PHANTOM MethodPointer* methodPointerType(ClassType* a_pObjectType, Type* a_pReturnType,
                                                        TypesView a_ParameterTypes,
                                                        Modifiers a_Qualifiers = Modifier::None);
PHANTOM_EXPORT_PHANTOM FieldPointer* fieldPointerType(ClassType* a_pObjectType, Type* a_pValueType);

/// function types
template<class R, class... Ps>
struct TypeOfUndefined<R(Ps...)>
{
    static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        return (Type*)functionType(TypeOfUndefined<R>::object(), Types{PHANTOM_TYPEOF(Ps)...});
    }
};

#pragma push_macro("cdecl")
#undef cdecl
/// function pointer types
template<class R, class... Ps>
struct TypeOfUndefined<R(PHANTOM_CALLCONV(cdecl)*)(Ps...)>
{
    static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        return (Type*)functionPointerType(PHANTOM_TYPEOF(R), ABI::CDecl, Types{PHANTOM_TYPEOF(Ps)...});
    }
};
#pragma pop_macro("cdecl")
#if PHANTOM_HAS_FASTCALL
template<class R, class... Ps>
struct TypeOfUndefined<R(PHANTOM_CALLCONV(fastcall)*)(Ps...)>
{
    static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        return (Type*)functionPointerType(PHANTOM_TYPEOF(R), ABI::FastCall, Types{PHANTOM_TYPEOF(Ps)...}, 0);
    }
};
#endif
#if PHANTOM_HAS_STDCALL
template<class R, class... Ps>
struct TypeOfUndefined<R(PHANTOM_CALLCONV(stdcall)*)(Ps...)>
{
    static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        return (Type*)functionPointerType(PHANTOM_TYPEOF(R), ABI::StdCall, Types{PHANTOM_TYPEOF(Ps)...});
    }
};
#endif

/// member function pointer types
template<class T, class R, class... Ps>
struct TypeOfUndefined<R (T::*)(Ps...)>
{
    static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        return (
        Type*)(methodPointerType((ClassType*)PHANTOM_TYPEOF(T), PHANTOM_TYPEOF(R), Types{PHANTOM_TYPEOF(Ps)...}));
    }
};

template<class T, class R, class... Ps>
struct TypeOfUndefined<R (T::*)(Ps...) const>
{
    static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        return (Type*)methodPointerType((ClassType*)PHANTOM_TYPEOF(T), PHANTOM_TYPEOF(R), Types{PHANTOM_TYPEOF(Ps)...},
                                        PHANTOM_R_CONST);
    }
};

template<class T, class R, class... Ps>
struct TypeOfUndefined<R (T::*)(Ps...) volatile>
{
    static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        return (Type*)methodPointerType((ClassType*)PHANTOM_TYPEOF(T), PHANTOM_TYPEOF(R), Types{PHANTOM_TYPEOF(Ps)...},
                                        PHANTOM_R_VOLATILE);
    }
};

template<class T, class R, class... Ps>
struct TypeOfUndefined<R (T::*)(Ps...) const volatile>
{
    static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        return (Type*)methodPointerType((ClassType*)PHANTOM_TYPEOF(T), PHANTOM_TYPEOF(R), Types{PHANTOM_TYPEOF(Ps)...},
                                        PHANTOM_R_CONST | PHANTOM_R_VOLATILE);
    }
};

template<class T, class R>
struct TypeOfUndefined<R(T::*)>
{
    static Type* object()
    {
        PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD();
        return (Type*)fieldPointerType((ClassType*)PHANTOM_TYPEOF(T), PHANTOM_TYPEOF(R));
    }
};

template<>
struct TypeOfUndefined<void>
{
    static Type* object() { return BuiltInTypes::TYPE_VOID; }
};
template<>
struct TypeOfUndefined<char>
{
    static Type* object() { return BuiltInTypes::TYPE_CHAR; }
};
template<>
struct TypeOfUndefined<unsigned char>
{
    static Type* object() { return BuiltInTypes::TYPE_UNSIGNED_CHAR; }
};
template<>
struct TypeOfUndefined<signed char>
{
    static Type* object() { return BuiltInTypes::TYPE_SIGNED_CHAR; }
};
template<>
struct TypeOfUndefined<short>
{
    static Type* object() { return BuiltInTypes::TYPE_SHORT; }
};
template<>
struct TypeOfUndefined<unsigned short>
{
    static Type* object() { return BuiltInTypes::TYPE_UNSIGNED_SHORT; }
};
template<>
struct TypeOfUndefined<int>
{
    static Type* object() { return BuiltInTypes::TYPE_INT; }
};
template<>
struct TypeOfUndefined<unsigned int>
{
    static Type* object() { return BuiltInTypes::TYPE_UNSIGNED_INT; }
};
template<>
struct TypeOfUndefined<long>
{
    static Type* object() { return BuiltInTypes::TYPE_LONG; }
};
template<>
struct TypeOfUndefined<unsigned long>
{
    static Type* object() { return BuiltInTypes::TYPE_UNSIGNED_LONG; }
};
template<>
struct TypeOfUndefined<long long>
{
    static Type* object() { return BuiltInTypes::TYPE_LONG_LONG; }
};
template<>
struct TypeOfUndefined<unsigned long long>
{
    static Type* object() { return BuiltInTypes::TYPE_UNSIGNED_LONG_LONG; }
};
template<>
struct TypeOfUndefined<float>
{
    static Type* object() { return BuiltInTypes::TYPE_FLOAT; }
};
template<>
struct TypeOfUndefined<double>
{
    static Type* object() { return BuiltInTypes::TYPE_DOUBLE; }
};
template<>
struct TypeOfUndefined<long double>
{
    static Type* object() { return BuiltInTypes::TYPE_LONG_DOUBLE; }
};
template<>
struct TypeOfUndefined<bool>
{
    static Type* object() { return BuiltInTypes::TYPE_BOOL; }
};
template<>
struct TypeOfUndefined<std::nullptr_t>
{
    static Type* object() { return BuiltInTypes::TYPE_NULLPTR_T; }
};
#if PHANTOM_HAS_BUILT_IN_WCHAR_T == 1
template<>
struct TypeOfUndefined<wchar_t>
{
    static Type* object() { return BuiltInTypes::TYPE_WCHAR_T; }
};
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR16_T == 1
template<>
struct TypeOfUndefined<char16_t>
{
    static Type* object() { return BuiltInTypes::TYPE_CHAR16_T; }
};
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR32_T == 1
template<>
struct TypeOfUndefined<char32_t>
{
    static Type* object() { return BuiltInTypes::TYPE_CHAR32_T; }
};
#endif

template<>
struct TypeOfUndefined<void*>
{
    static Type* object() { return BuiltInTypes::TYPE_VOID_PTR; }
};

template<class T>
bool Type::isA() const
{
    return isA(TypeOfUndefined<T>::object());
}

template<class T>
T const* Type::cast(void const* a_pPointer) const
{
    return reinterpret_cast<T const*>(cast(PHANTOM_TYPEOF(T), a_pPointer));
}

template<class T>
T* Type::cast(void* a_pPointer) const
{
    return reinterpret_cast<T*>(cast(PHANTOM_TYPEOF(T), a_pPointer));
}

} // namespace lang

} // namespace phantom

/// @endcond
