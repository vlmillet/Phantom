// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <phantom/detail/core.h>
#include <phantom/lang/reflection_fwd.h>
#include <phantom/thread/RecursiveSpinMutex.h>
#include <phantom/utils/Flags.h>

#define PHANTOM_R_FILTER_FIELD 0x1
#define PHANTOM_R_FILTER_PROPERTY 0x2
#define PHANTOM_R_FILTER_ALL (~::phantom::uint(0))

namespace phantom
{
namespace lang
{
PHANTOM_EXPORT_PHANTOM void conversionOperatorNameNormalizer(StringView a_strName, StringBuffer& a_Buf,
                                                             lang::LanguageElement* a_pScope);

enum class Access : char
{
    Undefined,
    Public,
    Protected,
    Private
};
using Accesses = SmallVector<Access, 10>;

enum class Visibility : char
{
    Public,    // lookup can find this symbol
    Protected, // lookup can find this symbol only from another which owns it
    Private    // lookup cannot find this symbol
};

typedef void (*SymbolExtender)(Symbol*);
using SymbolExtenders = SmallVector<SymbolExtender, 4>;

#define PHANTOM_R_VIRTUAL ::phantom::lang::Modifier::Virtual
#define PHANTOM_R_ABSTRACT ::phantom::lang::Modifier::Abstract
#define PHANTOM_R_PURE_VIRTUAL ::phantom::lang::Modifier::PureVirtual
#define PHANTOM_R_EXPLICIT ::phantom::lang::Modifier::Explicit
#define PHANTOM_R_STATIC ::phantom::lang::Modifier::Static
#define PHANTOM_R_FINAL ::phantom::lang::Modifier::Final
#define PHANTOM_R_TRANSIENT ::phantom::lang::Modifier::Transient
#define PHANTOM_R_VIRTUAL_GET ::phantom::lang::Modifier::VirtualGet // for properties
#define PHANTOM_R_VIRTUAL_SET ::phantom::lang::Modifier::VirtualSet // for properties
#define PHANTOM_R_NOEXCEPT ::phantom::lang::Modifier::NoExcept
#define PHANTOM_R_OVERRIDE ::phantom::lang::Modifier::Override
#define PHANTOM_R_SINGLETON ::phantom::lang::Modifier::Singleton
#define PHANTOM_R_CONST ::phantom::lang::Modifier::Const
#define PHANTOM_R_NOCONST ::phantom::lang::Modifier::NoConst
#define PHANTOM_R_LVALUEREF ::phantom::lang::Modifier::LValueRef // cpp11 signature lvalue ref-qualifiers
#define PHANTOM_R_RVALUEREF ::phantom::lang::Modifier::RValueRef // cpp11 signature rvalue ref-qualifiers
#define PHANTOM_R_VOLATILE ::phantom::lang::Modifier::Volatile
#define PHANTOM_R_MUTABLE ::phantom::lang::Modifier::Mutable
#define PHANTOM_R_INLINE ::phantom::lang::Modifier::Inline
#define PHANTOM_R_SLOT_METHOD ::phantom::lang::Modifier::Slot
#define PHANTOM_R_DEFAULTED ::phantom::lang::Modifier::Defaulted
#define PHANTOM_R_DELETED ::phantom::lang::Modifier::Deleted

#define o_virtual PHANTOM_R_VIRTUAL
#define o_abstract PHANTOM_R_ABSTRACT
#define o_pure_virtual PHANTOM_R_PURE_VIRTUAL
#define o_explicit PHANTOM_R_EXPLICIT
#define o_static PHANTOM_R_STATIC
#define o_final PHANTOM_R_FINAL
#define o_transient PHANTOM_R_TRANSIENT
#define o_virtual_get PHANTOM_R_VIRTUAL_GET
#define o_virtual_set PHANTOM_R_VIRTUAL_SET
#define o_noexcept PHANTOM_R_NOEXCEPT
#define o_override PHANTOM_R_OVERRIDE
#define o_singleton PHANTOM_R_SINGLETON
#define o_const PHANTOM_R_CONST
#define o_noconst PHANTOM_R_NOCONST
#define o_lvalueref PHANTOM_R_LVALUEREF
#define o_rvalueref PHANTOM_R_RVALUEREF
#define o_volatile PHANTOM_R_VOLATILE
#define o_mutable PHANTOM_R_MUTABLE
#define o_inline PHANTOM_R_INLINE
#define o_slot_method PHANTOM_R_SLOT_METHOD

#define PHANTOM_R_REFQUAL_MASK (PHANTOM_R_LVALUEREF | PHANTOM_R_RVALUEREF)
#define PHANTOM_R_METHOD_QUAL_MASK (PHANTOM_R_CONST | PHANTOM_R_VOLATILE | PHANTOM_R_REFQUAL_MASK)

#define PHANTOM_SINGLETONOF(ty) static_cast<ty*>(PHANTOM_CLASSOF(ty)->getSingleton())

struct Modifier
{
    enum Enum : int
    {
        None = 0x0,
        Virtual = 0x1,
        VirtualGet = 0x2,
        VirtualSet = 0x4,
        Override = 0x8 | Virtual,
        Abstract = 0x10,
        PureVirtual = Abstract | Virtual,
        Explicit = 0x40,
        Static = 0x80,
        Final = 0x100 | Virtual,
        Transient = 0x200,
        Const = 0x400,
        NoConst = 0x800,
        Volatile = 0x1000,
        Mutable = 0x2000,
        Inline = 0x4000,
        // Slot = 0x8000,
        NoExcept = 0x20000,
        Singleton = 0x40000,
        LValueRef = 0x80000,
        RValueRef = 0x100000,
        RefQualifiersMask = LValueRef | RValueRef,
        MethodQualifiersMask = RefQualifiersMask | Const | Volatile,
        Deleted = 0x00200000,   /// for deleted member functions
        Defaulted = 0x00400000, /// for defaulted member functions

        UserFirst = Defaulted << 1,
    };
};
PHANTOM_DECLARE_FLAGS(Modifiers, Modifier::Enum);
HAUNT_RAW(PHANTOM_REGISTER(Functions) {
    this_().function<::phantom::Flags<Modifier::Enum>(Modifier::Enum, Modifier::Enum)>("operator|", operator|);
})
HAUNT_RAW(PHANTOM_REGISTER(Functions) {
    this_().function<::phantom::Flags<Modifier::Enum>(Modifier::Enum, ::phantom::Flags<Modifier::Enum>)>(
    "operator|", operator|);
})
HAUNT_RAW(PHANTOM_REGISTER(Functions) {
    this_().function<::phantom::IncompatibleFlag(Modifier::Enum, int)>("operator|", operator|);
})

HAUNT_PAUSE;
PHANTOM_DECLARE_OPERATORS_FOR_FLAGS(Modifiers);
HAUNT_RESUME;

HAUNT_PAUSE;

#define PHANTOM_DECLARE_META_CLASS(_type_)                                                                             \
public:                                                                                                                \
	HAUNT_OFF using MetaClassType = _type_;																			   \
    static phantom::lang::Class* MetaClass();                                                                          \
                                                                                                                       \
private:

#define PHANTOM_CANCEL_META_CLASS()                                                                                    \
public:                                                                                                                \
    HAUNT_OFF static phantom::DummyClass* MetaClass();                                                                 \
                                                                                                                       \
private:

#define PHANTOM_DEFINE_META_CLASS(_type_)                                                                              \
    phantom::lang::Class* _type_::MetaClass()                                                                          \
    {                                                                                                                  \
        static auto meta = PHANTOM_PP_CAT(_type_, _PHANTOM_).this_()._PHNTM_getMeta();                                 \
        return meta;                                                                                                   \
    }

PHANTOM_EXPORT_PHANTOM void initializeSystem();
PHANTOM_EXPORT_PHANTOM void releaseSystem();

typedef std::pair<Type*, Type*> Type_Type;

namespace detail
{
PHANTOM_EXPORT_PHANTOM Type* findType(const char* a_strName, Namespace* a_pScope);
PHANTOM_EXPORT_PHANTOM Type* findType(StringView a_strName, Namespace* a_pScope);
PHANTOM_EXPORT_PHANTOM Type* findType(StringView a_strName, ClassType* a_pScope);
PHANTOM_EXPORT_PHANTOM Type* findType(StringView a_strName, Class* a_pScope);
PHANTOM_EXPORT_PHANTOM Type* findType(hash64 a_Hash);
} // namespace detail

HAUNT_RESUME;

struct PHANTOM_EXPORT_PHANTOM BuiltInTypes
{
public:
#if defined(TYPE_VOID)
#    error VOID defined
#endif
    static Type*  TYPE_VOID;
    static Type*  TYPE_CHAR;
    static Type*  TYPE_UNSIGNED_CHAR;
    static Type*  TYPE_SIGNED_CHAR;
    static Type*  TYPE_SHORT;
    static Type*  TYPE_UNSIGNED_SHORT;
    static Type*  TYPE_INT;
    static Type*  TYPE_UNSIGNED_INT;
    static Type*  TYPE_LONG;
    static Type*  TYPE_UNSIGNED_LONG;
    static Type*  TYPE_LONG_LONG;
    static Type*  TYPE_UNSIGNED_LONG_LONG;
    static Type*  TYPE_FLOAT;
    static Type*  TYPE_DOUBLE;
    static Type*  TYPE_LONG_DOUBLE;
    static Type*  TYPE_BOOL;
    static Type*  TYPE_NULLPTR_T;
    static Type*  TYPE_WCHAR_T;
    static Type*  TYPE_CHAR16_T;
    static Type*  TYPE_CHAR32_T;
    static Type*  TYPE_VOID_PTR;
    static Class* TYPE_STD_STRING;
    static Class* TYPE_STRING;

    static void Register();

private:
    friend PHANTOM_EXPORT_PHANTOM void phantom::lang::initializeSystem();
};

/// @cond INTERNAL.
class conversions;
/// @endcond

/// \brief  Represent the different category of types to identify them quickly.
enum class TypeKind : uint8_t
{
    Void,
    Bool,
    Char,
    SChar,
    Int8 = SChar,
    UChar,
    UInt8 = UChar,
    Short,
    Int16 = Short,
#if PHANTOM_HAS_BUILT_IN_WCHAR_T
    WChar,
#else
    WChar = Short,
#endif
#if PHANTOM_HAS_BUILT_IN_CHAR16_T
    Char16 = Short + 1 PHANTOM_IF_WCHAR_T(+1),
#else
    Char16 = Short,
#endif
    UShort = Short + 1 PHANTOM_IF_WCHAR_T(+1) PHANTOM_IF_CHAR16_T(+1),
    UInt16 = UShort,
    Int,
    Int32 = Int,
#if PHANTOM_HAS_BUILT_IN_CHAR32_T
    Char32,
#else
    Char32 = Int,
#endif
    UInt = Int + 1 PHANTOM_IF_CHAR32_T(+1),
    UInt32 = UInt,
    Long,
    ULong,
#if PHANTOM_SIZE_OF_LONG == 8
    Int64 = Long,
    UInt64 = ULong,
    LongLong,
    ULongLong,
#else
    LongLong,
    ULongLong,
    Int64 = LongLong,
    UInt64 = ULongLong,
#endif
    Float,
    Double,
    LongDouble,
    Enum,
    Pointer,
    FunctionPointer,
    NullPtr,
    FieldPointer,
    MethodPointer,
    LValueReference,
    RValueReference,
    Array,
    ClassType,
    Structure,
    Union,
    Class,
    VectorClass,
    SetClass,
    MapClass,
    StringClass,
    ArrayClass,
    InitializerList,
    Function,
    Unknown,
    Custom0,
    Custom1,
    Custom2,
    Custom3,
    Custom4,
    Custom5,
    Custom6,
    Custom7,
    Custom8,
    Custom9,
};

const TypeKind FirstClassKind = TypeKind::Class;
const TypeKind LastClassKind = TypeKind::ArrayClass;

const TypeKind FirstClassTypeKind = TypeKind::ClassType;
const TypeKind LastClassTypeKind = TypeKind::ArrayClass;

/// \brief  Operator types.
enum class OperatorKind
{
    Binary,
    UnaryPrefixed,
    UnaryPostfixed,
    Ternary,
    Postfixed,
    Vararg,
};

/// \brief  Filters user defined functions (conversion functions and constructors).
enum class UserDefinedFunctions
{
    None = 0,
    ImplicitsOnly,
    All,
};

/// \brief  Conversions types.
enum class CastKind
{
    Implicit,
    Const,
    Reinterpret,
    Static,
    Explicit,
};

/// @cond ADVANCED

} // namespace lang
} // namespace phantom

#if PHANTOM_CUSTOM_THREAD_SAFE

#    include <phantom/thread/LockGuard.h>

namespace phantom
{
namespace lang
{
struct PHANTOM_EXPORT_PHANTOM ReflectionMutex
{
    ReflectionMutex();
    ~ReflectionMutex();
    void  lock();
    void  unlock();
    void* m_private = nullptr;
};
PHANTOM_EXPORT_PHANTOM ReflectionMutex& read_mutex();
PHANTOM_EXPORT_PHANTOM ReflectionMutex& write_mutex();
} // namespace lang
} // namespace phantom

#    define _PHNTM_R_MTX_GUARD()                                                                                       \
        phantom::LockGuard<phantom::lang::ReflectionMutex> __grd_(phantom::lang::write_mutex());

#    define PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD()                                                                     \
        static phantom::RecursiveSpinMutex              __recursive_mutex_;                                            \
        phantom::LockGuard<phantom::RecursiveSpinMutex> __grd_(__recursive_mutex_);

#else
#    define PHANTOM_STATIC_RECURSIVE_MUTEX_GUARD()

#endif

/// @endcond
