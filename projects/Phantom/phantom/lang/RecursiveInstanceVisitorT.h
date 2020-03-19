// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "Array.h"
#include "Class.h"
#include "ConstType.h"
#include "ConstVolatileType.h"
#include "Enum.h"
#include "Field.h"
#include "FieldPointer.h"
#include "FunctionPointer.h"
#include "FunctionType.h"
#include "FundamentalsT.h"
#include "LValueReference.h"
#include "MapClass.h"
#include "MethodPointer.h"
#include "Pointer.h"
#include "Property.h"
#include "QualifiedType.h"
#include "RValueReference.h"
#include "SetClass.h"
#include "StringClass.h"
#include "Structure.h"
#include "Union.h"
#include "VectorClass.h"
#include "VolatileType.h"

#pragma warning(disable : 4100)

namespace phantom
{
namespace lang
{
/// @brief Represents an instance of a type as a couple [address, meta-element]. It serves as a
/// helper for casting applied to reflected instances.
template<class Meta>
struct InstanceT
{
    InstanceT() = default;
    InstanceT(Meta* a_pMeta, void* a_pAddress) : m_Meta(a_pMeta), m_pAddress(a_pAddress) {}

#if !defined(__HAUNT__) // fix haunt bug
    template<class T, typename = typename std::enable_if<std::is_base_of<Meta, T>::value>::type>
    InstanceT(InstanceT<T> const& a_Other) : m_Meta(a_Other.getMeta()), m_pAddress(a_Other.getAddress())
    {
    }
#endif

    template<class T>
    InstanceT<T> as()
    {
        if (T* casted = phantom::Object::Cast<T>(m_Meta))
        {
            return InstanceT<T>(casted, m_pAddress);
        }
        return InstanceT<T>();
    }

    template<class T>
    InstanceT<T> staticCast()
    {
        return InstanceT<T>(static_cast<T*>(m_Meta), m_pAddress);
    }

         operator bool() const { return m_pAddress != nullptr; }
    bool operator==(InstanceT<Meta> const& other) const
    {
        return m_pAddress == other.getAddress() && m_Meta == other.getMeta();
    }
    bool operator!=(InstanceT<Meta> const& other) const { return !operator==(other); }

    template<class T>
    InstanceT<T> getSubInstance(T* _meta, size_t _offset) const
    {
        return InstanceT<T>(_meta, (char*)m_pAddress + _offset);
    }

    void* getAddressWithOffset(ptrdiff_t off) { return (char*)m_pAddress + off; }

    template<class T>
    T* getAddressAs()
    {
        return (T*)m_Meta->cast(PHANTOM_TYPEOF(T), m_pAddress);
    }

    void* getAddress() const { return m_pAddress; }
    Meta* getMeta() const { return m_Meta; }

private:
    Meta* m_Meta = nullptr;
    void* m_pAddress = nullptr;
};

template<>
struct InstanceT<BaseClass>
{
    InstanceT() = default;
    InstanceT(InstanceT<Class> a_Derived, BaseClass const& a_BaseClass) : m_Derived(a_Derived), m_BaseClass(a_BaseClass)
    {
    }

         operator bool() const { return m_Derived; }
    bool operator==(InstanceT<BaseClass> const& other) const
    {
        return m_Derived == other.m_Derived && m_BaseClass == other.m_BaseClass;
    }
    bool operator!=(InstanceT<BaseClass> const& other) const { return !operator==(other); }

    InstanceT<BaseClass> rebind(InstanceT<Class> other)
    {
        PHANTOM_ASSERT(other.getMeta() == m_Derived.getMeta());
        return InstanceT<BaseClass>(other, m_BaseClass);
    }

    InstanceT<Class> getDerivedInstance() const { return m_Derived; }

    InstanceT<Class> getBaseClassInstance() const
    {
        return m_Derived.getSubInstance(m_BaseClass.baseClass, m_BaseClass.offset);
    }

    BaseClass const& getBaseClass() const { return m_BaseClass; }

private:
    InstanceT<Class> m_Derived;
    BaseClass        m_BaseClass;
};

template<class Meta>
struct MemberAccessT
{
    static_assert(std::is_base_of<ValueMember, Meta>::value, "Meta must derive from ValueMember");
    MemberAccessT() = default;
    MemberAccessT(InstanceT<ClassType> a_Owner, Meta* a_pMember) : m_Owner(a_Owner), m_pMemberMeta(a_pMember) {}

    template<class T, typename = typename std::enable_if<std::is_base_of<Meta, T>::value>::type>
    MemberAccessT(MemberAccessT<T> const& a_Other) : m_Owner(a_Other.getOwner()), m_pMemberMeta(a_Other.getMemberMeta())
    {
    }

    template<class T>
    MemberAccessT<T> as()
    {
        if (T* casted = phantom::Object::Cast<T>(m_pMemberMeta))
        {
            return MemberAccessT<T>(m_Owner, casted);
        }
        return MemberAccessT<T>();
    }
         operator bool() const { return m_Owner && m_pMemberMeta != nullptr; }
    bool operator==(MemberAccessT<Meta> const& other) const
    {
        return m_Owner == other.m_Owner && m_pMemberMeta == other.m_pMemberMeta;
    }
    bool operator!=(MemberAccessT<Meta> const& other) const { return !operator==(other); }

    Meta*                getMemberMeta() const { return m_pMemberMeta; }
    InstanceT<ClassType> getOwner() const { return m_Owner; }

private:
    InstanceT<ClassType> m_Owner;
    Meta*                m_pMemberMeta;
};

template<class Derived = void>
class RecursiveInstanceVisitorT;

namespace _RecursiveInstanceVisitorT
{
template<class T>
struct DerivedSelector
{
    using type = T;
};

template<>
struct DerivedSelector<void>
{
    using type = RecursiveInstanceVisitorT<void>;
};
} // namespace _RecursiveInstanceVisitorT

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief Traverses an instance lang. It is implemented with a template polymorphism
///        overriding "traverseXxx" means redefining all the traversing hierarchy from the current
///        "Symbol". overriding "visitXxx/endXxx" means redefining just the current symbol
///        enter/exit in the traversal process. overriding "walkUpVisitXxx/walkUpEndXxx" means
///        redefining just a part of the derivation of the current symbol
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class Derived>
class RecursiveInstanceVisitorT
{
public:
    using Impl = typename _RecursiveInstanceVisitorT::DerivedSelector<Derived>::type;

    Impl*       this_() { return static_cast<Impl*>(this); }
    Impl const* this_() const { return static_cast<Impl const*>(this); }

    bool shouldUpdateValueMember(MemberAccessT<ValueMember> a_Input) const { return false; }

    inline bool traverseValueMembers(InstanceT<ClassType> a_Input)
    {
        for (ValueMember* pVM : a_Input.getMeta()->getValueMembers())
        {
            if (!(traverse(MemberAccessT<ValueMember>(a_Input, pVM))))
                return false;
        }
        return true;
    }

    // ENTRY POINT

    bool visit(void* a_pAddress, Type* a_pType) { return traverse(InstanceT<Type>(a_pType, a_pAddress)); }

    // Type

    bool visitType(InstanceT<Type>) { return true; }
    bool endType(InstanceT<Type>) { return true; }
    bool walkUpVisitFromType(InstanceT<Type> a_Input) { return this_()->visitType(a_Input); }
    bool walkUpEndFromType(InstanceT<Type> a_Input) { return this_()->endType(a_Input); };

    bool traverse(InstanceT<Type> a_Input)
    {
        switch (a_Input.getMeta()->getTypeKind())
        {
        case TypeKind::Class:
            return traverse(InstanceT<Class>((Class*)a_Input.getMeta(), a_Input.getAddress()));
        case TypeKind::Enum:
            return traverse(InstanceT<Enum>((Enum*)a_Input.getMeta(), a_Input.getAddress()));
        case TypeKind::Pointer:
            return traverse(InstanceT<Pointer>((Pointer*)a_Input.getMeta(), a_Input.getAddress()));
        case TypeKind::Array:
            return traverse(InstanceT<Array>((Array*)a_Input.getMeta(), a_Input.getAddress()));
        case TypeKind::Bool:
            return traverse(
            InstanceT<FundamentalTypeT<bool>>((FundamentalTypeT<bool>*)a_Input.getMeta(), a_Input.getAddress()));
            ;
        case TypeKind::Char:
            return traverse(
            InstanceT<FundamentalTypeT<char>>((FundamentalTypeT<char>*)a_Input.getMeta(), a_Input.getAddress()));
            ;
        case TypeKind::Int8:
            return traverse(
            InstanceT<FundamentalTypeT<int8>>((FundamentalTypeT<int8>*)a_Input.getMeta(), a_Input.getAddress()));
            ;
        case TypeKind::UInt8:
            return traverse(
            InstanceT<FundamentalTypeT<uint8>>((FundamentalTypeT<uint8>*)a_Input.getMeta(), a_Input.getAddress()));
            ;
        case TypeKind::Int16:
            return traverse(
            InstanceT<FundamentalTypeT<int16>>((FundamentalTypeT<int16>*)a_Input.getMeta(), a_Input.getAddress()));
            ;
        case TypeKind::UInt16:
            return traverse(
            InstanceT<FundamentalTypeT<uint16>>((FundamentalTypeT<uint16>*)a_Input.getMeta(), a_Input.getAddress()));
            ;
        case TypeKind::Int32:
            return traverse(
            InstanceT<FundamentalTypeT<int32>>((FundamentalTypeT<int32>*)a_Input.getMeta(), a_Input.getAddress()));
            ;
        case TypeKind::UInt32:
            return traverse(
            InstanceT<FundamentalTypeT<uint32>>((FundamentalTypeT<uint32>*)a_Input.getMeta(), a_Input.getAddress()));
            ;
        case TypeKind::Int64:
            return traverse(
            InstanceT<FundamentalTypeT<int64>>((FundamentalTypeT<int64>*)a_Input.getMeta(), a_Input.getAddress()));
            ;
        case TypeKind::UInt64:
            return traverse(
            InstanceT<FundamentalTypeT<uint64>>((FundamentalTypeT<uint64>*)a_Input.getMeta(), a_Input.getAddress()));
            ;
        case TypeKind::Float:
            return traverse(
            InstanceT<FundamentalTypeT<float>>((FundamentalTypeT<float>*)a_Input.getMeta(), a_Input.getAddress()));
            ;
        case TypeKind::Double:
            return traverse(
            InstanceT<FundamentalTypeT<double>>((FundamentalTypeT<double>*)a_Input.getMeta(), a_Input.getAddress()));
            ;
        case TypeKind::LongDouble:
            return traverse(InstanceT<FundamentalTypeT<longdouble>>((FundamentalTypeT<longdouble>*)a_Input.getMeta(),
                                                                    a_Input.getAddress()));
            ;

        case TypeKind::VectorClass:
            return traverse(InstanceT<VectorClass>((VectorClass*)a_Input.getMeta(), a_Input.getAddress()));
        case TypeKind::SetClass:
            return traverse(InstanceT<SetClass>((SetClass*)a_Input.getMeta(), a_Input.getAddress()));
        case TypeKind::MapClass:
            return traverse(InstanceT<MapClass>((MapClass*)a_Input.getMeta(), a_Input.getAddress()));
        case TypeKind::StringClass:
            return traverse(InstanceT<StringClass>((StringClass*)a_Input.getMeta(), a_Input.getAddress()));

        case TypeKind::Union:
            return traverse(InstanceT<Union>((Union*)a_Input.getMeta(), a_Input.getAddress()));
        case TypeKind::Structure:
            return traverse(InstanceT<Structure>((Structure*)a_Input.getMeta(), a_Input.getAddress()));

            PHANTOM_IF_WCHAR_T(case TypeKind::WChar
                               : return traverse(InstanceT<FundamentalTypeT<wchar_t>>(
                               (FundamentalTypeT<wchar_t>*)a_Input.getMeta(), a_Input.getAddress())););
            PHANTOM_IF_CHAR16_T(case TypeKind::Char16
                                : return traverse(InstanceT<FundamentalTypeT<char16_t>>(
                                (FundamentalTypeT<char16_t>*)a_Input.getMeta(), a_Input.getAddress())););
            PHANTOM_IF_CHAR32_T(case TypeKind::Char32
                                : return traverse(InstanceT<FundamentalTypeT<char32_t>>(
                                (FundamentalTypeT<char32_t>*)a_Input.getMeta(), a_Input.getAddress())););

        case TypeKind::LValueReference:
            return traverse(InstanceT<LValueReference>((LValueReference*)a_Input.getMeta(), a_Input.getAddress()));
        case TypeKind::RValueReference:
            return traverse(InstanceT<RValueReference>((RValueReference*)a_Input.getMeta(), a_Input.getAddress()));
        case TypeKind::FunctionPointer:
            return traverse(InstanceT<FunctionPointer>((FunctionPointer*)a_Input.getMeta(), a_Input.getAddress()));
        case TypeKind::MethodPointer:
            return traverse(InstanceT<MethodPointer>((MethodPointer*)a_Input.getMeta(), a_Input.getAddress()));
        case TypeKind::FieldPointer:
            return traverse(InstanceT<FieldPointer>((FieldPointer*)a_Input.getMeta(), a_Input.getAddress()));
        case TypeKind::Function:
            return traverse(InstanceT<FunctionType>((FunctionType*)a_Input.getMeta(), a_Input.getAddress()));
        default:
            return this_()->traverseType(a_Input);
        }
    }

    bool traverseType(InstanceT<Type> a_Input) { return true; }

    // FundamentalTypeT<*>

    bool traverse(InstanceT<FundamentalTypeT<bool>> a_Input) { return this_()->traverseFundamentalType(a_Input); }
    bool traverseFundamentalType(InstanceT<FundamentalTypeT<bool>> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((bool*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    }
    bool traverseFundamental(bool* a_Input) { return true; };
    bool traverse(InstanceT<FundamentalTypeT<char>> a_Input) { return this_()->traverseFundamentalType(a_Input); }
    bool traverseFundamentalType(InstanceT<FundamentalTypeT<char>> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((char*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    }
    bool traverseFundamental(char* a_Input) { return true; };
    bool traverse(InstanceT<FundamentalTypeT<int8>> a_Input) { return this_()->traverseFundamentalType(a_Input); }
    bool traverseFundamentalType(InstanceT<FundamentalTypeT<int8>> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((int8*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    }
    bool traverseFundamental(int8* a_Input) { return true; };
    bool traverse(InstanceT<FundamentalTypeT<uint8>> a_Input) { return this_()->traverseFundamentalType(a_Input); }
    bool traverseFundamentalType(InstanceT<FundamentalTypeT<uint8>> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((uint8*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    }
    bool traverseFundamental(uint8* a_Input) { return true; };
    bool traverse(InstanceT<FundamentalTypeT<int16>> a_Input) { return this_()->traverseFundamentalType(a_Input); }
    bool traverseFundamentalType(InstanceT<FundamentalTypeT<int16>> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((int16*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    }
    bool traverseFundamental(int16* a_Input) { return true; };
    bool traverse(InstanceT<FundamentalTypeT<uint16>> a_Input) { return this_()->traverseFundamentalType(a_Input); }
    bool traverseFundamentalType(InstanceT<FundamentalTypeT<uint16>> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((uint16*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    }
    bool traverseFundamental(uint16* a_Input) { return true; };
    bool traverse(InstanceT<FundamentalTypeT<int32>> a_Input) { return this_()->traverseFundamentalType(a_Input); }
    bool traverseFundamentalType(InstanceT<FundamentalTypeT<int32>> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((int32*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    }
    bool traverseFundamental(int32* a_Input) { return true; };
    bool traverse(InstanceT<FundamentalTypeT<uint32>> a_Input) { return this_()->traverseFundamentalType(a_Input); }
    bool traverseFundamentalType(InstanceT<FundamentalTypeT<uint32>> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((uint32*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    }
    bool traverseFundamental(uint32* a_Input) { return true; };
    bool traverse(InstanceT<FundamentalTypeT<int64>> a_Input) { return this_()->traverseFundamentalType(a_Input); }
    bool traverseFundamentalType(InstanceT<FundamentalTypeT<int64>> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((int64*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    }
    bool traverseFundamental(int64* a_Input) { return true; };
    bool traverse(InstanceT<FundamentalTypeT<uint64>> a_Input) { return this_()->traverseFundamentalType(a_Input); }
    bool traverseFundamentalType(InstanceT<FundamentalTypeT<uint64>> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((uint64*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    }
    bool traverseFundamental(uint64* a_Input) { return true; };
    bool traverse(InstanceT<FundamentalTypeT<float>> a_Input) { return this_()->traverseFundamentalType(a_Input); }
    bool traverseFundamentalType(InstanceT<FundamentalTypeT<float>> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((float*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    }
    bool traverseFundamental(float* a_Input) { return true; };
    bool traverse(InstanceT<FundamentalTypeT<double>> a_Input) { return this_()->traverseFundamentalType(a_Input); }
    bool traverseFundamentalType(InstanceT<FundamentalTypeT<double>> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((double*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    }
    bool traverseFundamental(double* a_Input) { return true; };
    bool traverse(InstanceT<FundamentalTypeT<longdouble>> a_Input) { return this_()->traverseFundamentalType(a_Input); }
    bool traverseFundamentalType(InstanceT<FundamentalTypeT<longdouble>> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((longdouble*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    }
    bool traverseFundamental(longdouble* a_Input) { return true; };

    PHANTOM_IF_WCHAR_T(
    bool traverse(InstanceT<FundamentalTypeT<wchar_t>> a_Input) {
        return this_()->traverseFundamentalType(a_Input);
    } bool traverseFundamentalType(InstanceT<FundamentalTypeT<wchar_t>> a_Input) {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((wchar_t*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    } bool traverseFundamental(wchar_t* a_Input) { return true; });
    PHANTOM_IF_CHAR16_T(
    bool traverse(InstanceT<FundamentalTypeT<char16_t>> a_Input) {
        return this_()->traverseFundamentalType(a_Input);
    } bool traverseFundamentalType(InstanceT<FundamentalTypeT<char16_t>> a_Input) {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((char16_t*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    } bool traverseFundamental(char16_t* a_Input) { return true; });
    PHANTOM_IF_CHAR32_T(
    bool traverse(InstanceT<FundamentalTypeT<char32_t>> a_Input) {
        return this_()->traverseFundamentalType(a_Input);
    } bool traverseFundamentalType(InstanceT<FundamentalTypeT<char32_t>> a_Input) {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        if (!(this_()->traverseFundamental((char32_t*)a_Input.getAddress())))
            return false;
        if (!(this_()->walkUpEndFromPrimitiveType(a_Input)))
            return false;
        return true;
    } bool traverseFundamental(char32_t* a_Input) { return true; });

    // ExtendedType

    bool visitExtendedType(InstanceT<ExtendedType> a_Input) { return true; }
    bool endExtendedType(InstanceT<ExtendedType> a_Input) { return true; }
    bool walkUpVisitFromExtendedType(InstanceT<ExtendedType> a_Input)
    {
        if (!(this_()->walkUpVisitFromType(a_Input)))
            return false;
        return this_()->visitExtendedType(a_Input);
    }
    bool walkUpEndFromExtendedType(InstanceT<ExtendedType> a_Input)
    {
        if (!(this_()->endExtendedType(a_Input)))
            return false;
        return this_()->walkUpEndFromType(a_Input);
    }
    bool traverse(InstanceT<ExtendedType> a_Input) { return this_()->traverseExtendedType(a_Input); }
    bool traverseExtendedType(InstanceT<ExtendedType> a_Input)
    {
        if (!(this_()->walkUpVisitFromExtendedType(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromExtendedType(a_Input);
    };

    // QualifiedType

    bool visitQualifiedType(InstanceT<QualifiedType> a_Input) { return true; }
    bool endQualifiedType(InstanceT<QualifiedType> a_Input) { return true; }
    bool walkUpVisitFromQualifiedType(InstanceT<QualifiedType> a_Input)
    {
        if (!(this_()->walkUpVisitFromExtendedType(a_Input)))
            return false;
        return this_()->visitQualifiedType(a_Input);
    }
    bool walkUpEndFromQualifiedType(InstanceT<QualifiedType> a_Input)
    {
        if (!(this_()->endQualifiedType(a_Input)))
            return false;
        return this_()->walkUpEndFromExtendedType(a_Input);
    }
    bool traverse(InstanceT<QualifiedType> a_Input) { return this_()->traverseQualifiedType(a_Input); }
    bool traverseQualifiedType(InstanceT<QualifiedType> a_Input)
    {
        if (!(this_()->walkUpVisitFromQualifiedType(a_Input)))
            return false;
        if (!(traverse(InstanceT<Type>(a_Input.getMeta()->getUnderlyingType(), a_Input.getAddress()))))
            return false;
        return this_()->walkUpEndFromQualifiedType(a_Input);
    };

    // ConstType

    bool visitConstType(InstanceT<ConstType> a_Input) { return true; }
    bool endConstType(InstanceT<ConstType> a_Input) { return true; }
    bool walkUpVisitFromConstType(InstanceT<ConstType> a_Input)
    {
        if (!(this_()->walkUpVisitFromQualifiedType(a_Input)))
            return false;
        return this_()->visitConstType(a_Input);
    }
    bool walkUpEndFromConstType(InstanceT<ConstType> a_Input)
    {
        if (!(this_()->endConstType(a_Input)))
            return false;
        return this_()->walkUpEndFromQualifiedType(a_Input);
    }
    bool traverse(InstanceT<ConstType> a_Input) { return this_()->traverseConstType(a_Input); }
    bool traverseConstType(InstanceT<ConstType> a_Input)
    {
        if (!(this_()->walkUpVisitFromConstType(a_Input)))
            return false;
        if (!(traverse(InstanceT<Type>(a_Input.getMeta()->getUnderlyingType(), a_Input.getAddress()))))
            return false;
        return this_()->walkUpEndFromConstType(a_Input);
    };

    // VolatileType

    bool visitVolatileType(InstanceT<VolatileType> a_Input) { return true; }
    bool endVolatileType(InstanceT<VolatileType> a_Input) { return true; }
    bool walkUpVisitFromVolatileType(InstanceT<VolatileType> a_Input)
    {
        if (!(this_()->walkUpVisitFromQualifiedType(a_Input)))
            return false;
        return this_()->visitVolatileType(a_Input);
    }
    bool walkUpEndFromVolatileType(InstanceT<VolatileType> a_Input)
    {
        if (!(this_()->endVolatileType(a_Input)))
            return false;
        return this_()->walkUpEndFromQualifiedType(a_Input);
    }
    bool traverse(InstanceT<VolatileType> a_Input) { return this_()->traverseVolatileType(a_Input); }
    bool traverseVolatileType(InstanceT<VolatileType> a_Input)
    {
        if (!(this_()->walkUpVisitFromVolatileType(a_Input)))
            return false;
        if (!(traverse(InstanceT<Type>(a_Input.getMeta()->getUnderlyingType(), a_Input.getAddress()))))
            return false;
        return this_()->walkUpEndFromVolatileType(a_Input);
    };

    // ConstVolatileType

    bool visitConstVolatileType(InstanceT<ConstVolatileType> a_Input) { return true; }
    bool endConstVolatileType(InstanceT<ConstVolatileType> a_Input) { return true; }
    bool walkUpVisitFromConstVolatileType(InstanceT<ConstVolatileType> a_Input)
    {
        if (!(this_()->walkUpVisitFromQualifiedType(a_Input)))
            return false;
        return this_()->visitConstVolatileType(a_Input);
    }
    bool walkUpEndFromConstVolatileType(InstanceT<ConstVolatileType> a_Input)
    {
        if (!(this_()->endConstVolatileType(a_Input)))
            return false;
        return this_()->walkUpEndFromQualifiedType(a_Input);
    }
    bool traverse(InstanceT<ConstVolatileType> a_Input) { return this_()->traverseConstVolatileType(a_Input); }
    bool traverseConstVolatileType(InstanceT<ConstVolatileType> a_Input)
    {
        if (!(this_()->walkUpVisitFromConstVolatileType(a_Input)))
            return false;
        if (!(traverse(InstanceT<Type>(a_Input.getMeta()->getUnderlyingType(), a_Input.getAddress()))))
            return false;
        return this_()->walkUpEndFromConstVolatileType(a_Input);
    };

    // PointerType

    bool visitPointerType(InstanceT<PointerType> a_Input) { return true; }
    bool endPointerType(InstanceT<PointerType> a_Input) { return true; }
    bool walkUpVisitFromPointerType(InstanceT<PointerType> a_Input)
    {
        if (!(this_()->walkUpVisitFromExtendedType(a_Input)))
            return false;
        return this_()->visitPointerType(a_Input);
    }
    bool walkUpEndFromPointerType(InstanceT<PointerType> a_Input)
    {
        if (!(this_()->endPointerType(a_Input)))
            return false;
        return this_()->walkUpEndFromExtendedType(a_Input);
    }
    bool traverse(InstanceT<PointerType> a_Input) { return this_()->traversePointerType(a_Input); }
    bool traversePointerType(InstanceT<PointerType> a_Input)
    {
        if (!(this_()->walkUpVisitFromPointerType(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromPointerType(a_Input);
    };

    // FunctionPointer

    bool visitFunctionPointer(InstanceT<FunctionPointer> a_Input) { return true; }
    bool endFunctionPointer(InstanceT<FunctionPointer> a_Input) { return true; }
    bool walkUpVisitFromFunctionPointer(InstanceT<FunctionPointer> a_Input)
    {
        if (!(this_()->walkUpVisitFromPointerType(a_Input)))
            return false;
        return this_()->visitFunctionPointer(a_Input);
    }
    bool walkUpEndFromFunctionPointer(InstanceT<FunctionPointer> a_Input)
    {
        if (!(this_()->endFunctionPointer(a_Input)))
            return false;
        return this_()->walkUpEndFromPointerType(a_Input);
    }
    bool traverse(InstanceT<FunctionPointer> a_Input) { return this_()->traverseFunctionPointer(a_Input); }
    bool traverseFunctionPointer(InstanceT<FunctionPointer> a_Input)
    {
        if (!(this_()->walkUpVisitFromFunctionPointer(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromFunctionPointer(a_Input);
    };

    // MemberPointer

    bool visitMemberPointer(InstanceT<MemberPointer> a_Input) { return true; }
    bool endMemberPointer(InstanceT<MemberPointer> a_Input) { return true; }
    bool walkUpVisitFromMemberPointer(InstanceT<MemberPointer> a_Input)
    {
        if (!(this_()->walkUpVisitFromType(a_Input)))
            return false;
        return this_()->visitMemberPointer(a_Input);
    }
    bool walkUpEndFromMemberPointer(InstanceT<MemberPointer> a_Input)
    {
        if (!(this_()->endMemberPointer(a_Input)))
            return false;
        return this_()->walkUpEndFromType(a_Input);
    }
    bool traverse(InstanceT<MemberPointer> a_Input)
    {
        if (auto d0 = a_Input.as<FieldPointer>())
            return traverse(d0);
        else if (auto d1 = a_Input.as<MethodPointer>())
            return traverse(d1);
        return this_()->traverseType(a_Input);
    }
    bool traverseMemberPointer(InstanceT<MemberPointer> a_Input)
    {
        if (!(this_()->walkUpVisitFromMemberPointer(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromMemberPointer(a_Input);
    };

    // MethodPointer

    bool visitMethodPointer(InstanceT<MethodPointer> a_Input) { return true; }
    bool endMethodPointer(InstanceT<MethodPointer> a_Input) { return true; }
    bool walkUpVisitFromMethodPointer(InstanceT<MethodPointer> a_Input)
    {
        if (!(this_()->walkUpVisitFromMemberPointer(a_Input)))
            return false;
        return this_()->visitMethodPointer(a_Input);
    }
    bool walkUpEndFromMethodPointer(InstanceT<MethodPointer> a_Input)
    {
        if (!(this_()->endMethodPointer(a_Input)))
            return false;
        return this_()->walkUpEndFromMemberPointer(a_Input);
    }
    bool traverse(InstanceT<MethodPointer> a_Input) { return this_()->traverseMethodPointer(a_Input); }
    bool traverseMethodPointer(InstanceT<MethodPointer> a_Input)
    {
        if (!(this_()->walkUpVisitFromMethodPointer(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromMethodPointer(a_Input);
    };

    // FieldPointer

    bool visitFieldPointer(InstanceT<FieldPointer> a_Input) { return true; }
    bool endFieldPointer(InstanceT<FieldPointer> a_Input) { return true; }
    bool walkUpVisitFromFieldPointer(InstanceT<FieldPointer> a_Input)
    {
        if (!(this_()->walkUpVisitFromMemberPointer(a_Input)))
            return false;
        return this_()->visitFieldPointer(a_Input);
    }
    bool walkUpEndFromFieldPointer(InstanceT<FieldPointer> a_Input)
    {
        if (!(this_()->endFieldPointer(a_Input)))
            return false;
        return this_()->walkUpEndFromMemberPointer(a_Input);
    }
    bool traverse(InstanceT<FieldPointer> a_Input) { return this_()->traverseFieldPointer(a_Input); }
    bool traverseFieldPointer(InstanceT<FieldPointer> a_Input)
    {
        if (!(this_()->walkUpVisitFromFieldPointer(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromFieldPointer(a_Input);
    };

    // FunctionType

    bool visitFunctionType(InstanceT<FunctionType> a_Input) { return true; }
    bool endFunctionType(InstanceT<FunctionType> a_Input) { return true; }
    bool walkUpVisitFromFunctionType(InstanceT<FunctionType> a_Input)
    {
        if (!(this_()->walkUpVisitFromType(a_Input)))
            return false;
        return this_()->visitFunctionType(a_Input);
    }
    bool walkUpEndFromFunctionType(InstanceT<FunctionType> a_Input)
    {
        if (!(this_()->endFunctionType(a_Input)))
            return false;
        return this_()->walkUpEndFromType(a_Input);
    }
    bool traverse(InstanceT<FunctionType> a_Input) { return this_()->traverseFunctionType(a_Input); }
    bool traverseFunctionType(InstanceT<FunctionType> a_Input)
    {
        if (!(this_()->walkUpVisitFromFunctionType(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromFunctionType(a_Input);
    };

    // Pointer

    bool visitPointer(InstanceT<Pointer> a_Input) { return true; }
    bool endPointer(InstanceT<Pointer> a_Input) { return true; }
    bool walkUpVisitFromPointer(InstanceT<Pointer> a_Input)
    {
        if (!(this_()->walkUpVisitFromPointerType(a_Input)))
            return false;
        return this_()->visitPointer(a_Input);
    }
    bool walkUpEndFromPointer(InstanceT<Pointer> a_Input)
    {
        if (!(this_()->endPointer(a_Input)))
            return false;
        return this_()->walkUpEndFromPointerType(a_Input);
    }
    bool traverse(InstanceT<Pointer> a_Input) { return this_()->traversePointer(a_Input); }
    bool traversePointer(InstanceT<Pointer> a_Input)
    {
        if (!(this_()->walkUpVisitFromPointer(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromPointer(a_Input);
    };

    // Reference

    bool visitReference(InstanceT<Reference> a_Input) { return true; }
    bool endReference(InstanceT<Reference> a_Input) { return true; }
    bool walkUpVisitFromReference(InstanceT<Reference> a_Input)
    {
        if (!(this_()->walkUpVisitFromExtendedType(a_Input)))
            return false;
        return this_()->visitReference(a_Input);
    }
    bool walkUpEndFromReference(InstanceT<Reference> a_Input)
    {
        if (!(this_()->endReference(a_Input)))
            return false;
        return this_()->walkUpEndFromExtendedType(a_Input);
    }
    bool traverse(InstanceT<Reference> a_Input) { return this_()->traverseReference(a_Input); }
    bool traverseReference(InstanceT<Reference> a_Input)
    {
        if (!(this_()->walkUpVisitFromReference(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromReference(a_Input);
    };

    // LValueReference

    bool visitLValueReference(InstanceT<LValueReference> a_Input) { return true; }
    bool endLValueReference(InstanceT<LValueReference> a_Input) { return true; }
    bool walkUpVisitFromLValueReference(InstanceT<LValueReference> a_Input)
    {
        if (!(this_()->walkUpVisitFromReference(a_Input)))
            return false;
        return this_()->visitLValueReference(a_Input);
    }
    bool walkUpEndFromLValueReference(InstanceT<LValueReference> a_Input)
    {
        if (!(this_()->endLValueReference(a_Input)))
            return false;
        return this_()->walkUpEndFromReference(a_Input);
    }
    bool traverse(InstanceT<LValueReference> a_Input) { return this_()->traverseLValueReference(a_Input); }
    bool traverseLValueReference(InstanceT<LValueReference> a_Input)
    {
        if (!(this_()->walkUpVisitFromLValueReference(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromLValueReference(a_Input);
    };

    // RValueReference

    bool visitRValueReference(InstanceT<RValueReference> a_Input) { return true; }
    bool endRValueReference(InstanceT<RValueReference> a_Input) { return true; }
    bool walkUpVisitFromRValueReference(InstanceT<RValueReference> a_Input)
    {
        if (!(this_()->walkUpVisitFromReference(a_Input)))
            return false;
        return this_()->visitRValueReference(a_Input);
    }
    bool walkUpEndFromRValueReference(InstanceT<RValueReference> a_Input)
    {
        if (!(this_()->endRValueReference(a_Input)))
            return false;
        return this_()->walkUpEndFromReference(a_Input);
    }
    bool traverse(InstanceT<RValueReference> a_Input) { return this_()->traverseRValueReference(a_Input); }
    bool traverseRValueReference(InstanceT<RValueReference> a_Input)
    {
        if (!(this_()->walkUpVisitFromRValueReference(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromRValueReference(a_Input);
    };

    // PrimitiveType

    bool visitPrimitiveType(InstanceT<PrimitiveType> a_Input) { return true; }
    bool endPrimitiveType(InstanceT<PrimitiveType> a_Input) { return true; }
    bool walkUpVisitFromPrimitiveType(InstanceT<PrimitiveType> a_Input)
    {
        if (!(this_()->walkUpVisitFromType(a_Input)))
            return false;
        return this_()->visitPrimitiveType(a_Input);
    }
    bool walkUpEndFromPrimitiveType(InstanceT<PrimitiveType> a_Input)
    {
        if (!(this_()->endPrimitiveType(a_Input)))
            return false;
        return this_()->walkUpEndFromType(a_Input);
    }
    bool traverse(InstanceT<PrimitiveType> a_Input) { return this_()->traversePrimitiveType(a_Input); }
    bool traversePrimitiveType(InstanceT<PrimitiveType> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromPrimitiveType(a_Input);
    };

    // Enum

    bool visitEnum(InstanceT<Enum> a_Input) { return true; }
    bool endEnum(InstanceT<Enum> a_Input) { return true; }
    bool walkUpVisitFromEnum(InstanceT<Enum> a_Input)
    {
        if (!(this_()->walkUpVisitFromPrimitiveType(a_Input)))
            return false;
        return this_()->visitEnum(a_Input);
    }
    bool walkUpEndFromEnum(InstanceT<Enum> a_Input)
    {
        if (!(this_()->endEnum(a_Input)))
            return false;
        return this_()->walkUpEndFromPrimitiveType(a_Input);
    }
    bool traverse(InstanceT<Enum> a_Input) { return this_()->traverseEnum(a_Input); }
    bool traverseEnum(InstanceT<Enum> a_Input)
    {
        if (!(this_()->walkUpVisitFromEnum(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromEnum(a_Input);
    };

    // Array

    bool visitArray(InstanceT<Array> a_Input) { return true; }
    bool endArray(InstanceT<Array> a_Input) { return true; }
    bool walkUpVisitFromArray(InstanceT<Array> a_Input)
    {
        if (!(this_()->walkUpVisitFromExtendedType(a_Input)))
            return false;
        return this_()->visitArray(a_Input);
    }
    bool walkUpEndFromArray(InstanceT<Array> a_Input)
    {
        if (!(this_()->endArray(a_Input)))
            return false;
        return this_()->walkUpEndFromExtendedType(a_Input);
    }
    bool traverse(InstanceT<Array> a_Input) { return this_()->traverseArray(a_Input); }
    bool traverseArray(InstanceT<Array> a_Input)
    {
        if (!(this_()->walkUpVisitFromArray(a_Input)))
            return false;
        Type*  pValueType = a_Input.getMeta()->getItemType();
        size_t typeSz = pValueType->getSize();
        size_t count = a_Input.getMeta()->getItemCount();
        char*  data = (char*)a_Input.getAddress();
        if (!this_()->visitContainer(a_Input))
            return false;
        for (size_t i = 0; i < count; ++i)
        {
            if (!(this_()->traverseContainerElement(InstanceT<Type>(pValueType, data + i * typeSz), i)))
                return false;
        };
        if (!this_()->endContainer(a_Input))
            return false;
        return this_()->walkUpEndFromArray(a_Input);
    };

    // ClassType

    bool visitClassType(InstanceT<ClassType> a_Input) { return true; }
    bool endClassType(InstanceT<ClassType> a_Input) { return true; }
    bool walkUpVisitFromClassType(InstanceT<ClassType> a_Input)
    {
        if (!(this_()->walkUpVisitFromType(a_Input)))
            return false;
        return this_()->visitClassType(a_Input);
    }
    bool walkUpEndFromClassType(InstanceT<ClassType> a_Input)
    {
        if (!(this_()->endClassType(a_Input)))
            return false;
        return this_()->walkUpEndFromType(a_Input);
    }
    bool traverse(InstanceT<ClassType> a_Input)
    {
        if (auto d0 = a_Input.as<Class>())
            return traverse(d0);
        else if (auto d1 = a_Input.as<Structure>())
            return traverse(d1);
        else if (auto d2 = a_Input.as<Union>())
            return traverse(d2);
        return this_()->traverseType(a_Input);
    }
    bool traverseClassType(InstanceT<ClassType> a_Input)
    {
        if (!(this_()->walkUpVisitFromClassType(a_Input)))
            return false;
        if (!(this_()->traverseValueMembers(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromClassType(a_Input);
    };

    // Structure

    bool visitStructure(InstanceT<Structure> a_Input) { return true; }
    bool endStructure(InstanceT<Structure> a_Input) { return true; }
    bool walkUpVisitFromStructure(InstanceT<Structure> a_Input)
    {
        if (!(this_()->walkUpVisitFromClassType(a_Input)))
            return false;
        return this_()->visitStructure(a_Input);
    }
    bool walkUpEndFromStructure(InstanceT<Structure> a_Input)
    {
        if (!(this_()->endStructure(a_Input)))
            return false;
        return this_()->walkUpEndFromClassType(a_Input);
    }
    bool traverse(InstanceT<Structure> a_Input) { return this_()->traverseStructure(a_Input); }
    bool traverseStructure(InstanceT<Structure> a_Input)
    {
        if (!(this_()->walkUpVisitFromStructure(a_Input)))
            return false;
        if (!(this_()->traverseValueMembers(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromStructure(a_Input);
    };

    // Union

    bool visitUnion(InstanceT<Union> a_Input) { return true; }
    bool endUnion(InstanceT<Union> a_Input) { return true; }
    bool walkUpVisitFromUnion(InstanceT<Union> a_Input)
    {
        if (!(this_()->walkUpVisitFromClassType(a_Input)))
            return false;
        return this_()->visitUnion(a_Input);
    }
    bool walkUpEndFromUnion(InstanceT<Union> a_Input)
    {
        if (!(this_()->endUnion(a_Input)))
            return false;
        return this_()->walkUpEndFromClassType(a_Input);
    }
    bool traverse(InstanceT<Union> a_Input) { return this_()->traverseUnion(a_Input); }
    bool traverseUnion(InstanceT<Union> a_Input)
    {
        if (!(this_()->walkUpVisitFromUnion(a_Input)))
            return false;
        if (!(this_()->traverseValueMembers(a_Input)))
            return false;
        ;
        return this_()->walkUpEndFromUnion(a_Input);
    };

    // ValueMember

    bool visitValueMember(MemberAccessT<ValueMember> a_Input) { return true; }
    bool endValueMember(MemberAccessT<ValueMember> a_Input) { return true; }
    bool walkUpVisitFromValueMember(MemberAccessT<ValueMember> a_Input) { return this_()->visitValueMember(a_Input); }
    bool walkUpEndFromValueMember(MemberAccessT<ValueMember> a_Input) { return this_()->endValueMember(a_Input); }
    bool traverse(MemberAccessT<ValueMember> a_Input)
    {
        if (auto d0 = a_Input.as<Field>())
            return traverse(d0);
        else if (auto d1 = a_Input.as<Property>())
            return traverse(d1);
        return this_()->traverseValueMember(a_Input);
    }

    bool traverseValueMember(MemberAccessT<ValueMember> a_Input)
    {
        if (!(this_()->walkUpVisitFromValueMember(a_Input)))
            return false;
        if (!_traverseValueMember(a_Input))
            return false;
        return this_()->walkUpEndFromValueMember(a_Input);
    }

    bool _traverseValueMember(MemberAccessT<ValueMember> a_Input)
    {
        Type*  pValueType = a_Input.getMemberMeta()->getValueType();
        size_t valueTypeSz = pValueType->getSize();
        void*  buffer = alloca(valueTypeSz);
        pValueType->construct(buffer);
        bool set = this_()->shouldUpdateValueMember(a_Input);
        if (!set)
            a_Input.getMemberMeta()->getValue(a_Input.getOwner().getAddress(), buffer);
        bool res = traverse(InstanceT<Type>(pValueType, buffer));
        if (res && set)
            a_Input.getMemberMeta()->setValue(a_Input.getOwner().getAddress(), buffer);
        pValueType->destroy(buffer);
        return res;
    };

    // Field

    bool visitField(MemberAccessT<Field> a_Input) { return true; }
    bool endField(MemberAccessT<Field> a_Input) { return true; }
    bool walkUpVisitFromField(MemberAccessT<Field> a_Input)
    {
        if (!(this_()->walkUpVisitFromValueMember(a_Input)))
            return false;
        return this_()->visitField(a_Input);
    }
    bool walkUpEndFromField(MemberAccessT<Field> a_Input)
    {
        if (!(this_()->endField(a_Input)))
            return false;
        return this_()->walkUpEndFromValueMember(a_Input);
    }
    bool traverse(MemberAccessT<Field> a_Input) { return this_()->traverseField(a_Input); }
    bool traverseField(MemberAccessT<Field> a_Input)
    {
        if (!(this_()->walkUpVisitFromField(a_Input)))
            return false;
        if (!(traverse(a_Input.getOwner().getSubInstance(a_Input.getMemberMeta()->getValueType(),
                                                         a_Input.getMemberMeta()->getOffset()))))
            return false;
        ;
        return this_()->walkUpEndFromField(a_Input);
    };

    // Property

    bool visitProperty(MemberAccessT<Property> a_Input) { return true; }
    bool endProperty(MemberAccessT<Property> a_Input) { return true; }
    bool walkUpVisitFromProperty(MemberAccessT<Property> a_Input)
    {
        if (!(this_()->walkUpVisitFromValueMember(a_Input)))
            return false;
        return this_()->visitProperty(a_Input);
    }
    bool walkUpEndFromProperty(MemberAccessT<Property> a_Input)
    {
        if (!(this_()->endProperty(a_Input)))
            return false;
        return this_()->walkUpEndFromValueMember(a_Input);
    }
    bool traverse(MemberAccessT<Property> a_Input) { return this_()->traverseProperty(a_Input); }
    bool traverseProperty(MemberAccessT<Property> a_Input)
    {
        if (!(this_()->walkUpVisitFromProperty(a_Input)))
            return false;
        if (!_traverseValueMember(MemberAccessT<ValueMember>(a_Input.getOwner(), a_Input.getMemberMeta())))
            return false;
        return this_()->walkUpEndFromProperty(a_Input);
    };

    // BaseClass

    bool visitBaseClass(InstanceT<BaseClass> a_Input) { return true; }
    bool endBaseClass(InstanceT<BaseClass> a_Input) { return true; }

    bool traverseBaseClass(InstanceT<BaseClass> a_Input)
    {
        if (!(this_()->visitBaseClass(a_Input)))
            return false;
        if (!(traverse(a_Input.getBaseClassInstance())))
            return false;
        return this_()->endBaseClass(a_Input);
    }

    bool traverse(InstanceT<BaseClass> a_Input) { return this_()->traverseBaseClass(a_Input); }

    // Class

    bool visitClass(InstanceT<Class> a_Input) { return true; }
    bool endClass(InstanceT<Class> a_Input) { return true; }
    bool walkUpVisitFromClass(InstanceT<Class> a_Input)
    {
        if (!(this_()->walkUpVisitFromClassType(a_Input)))
            return false;
        return this_()->visitClass(a_Input);
    }
    bool walkUpEndFromClass(InstanceT<Class> a_Input)
    {
        if (!(this_()->endClass(a_Input)))
            return false;
        return this_()->walkUpEndFromClassType(a_Input);
    }
    bool traverse(InstanceT<Class> a_Input)
    {
        switch (a_Input.getMeta()->getTypeKind())
        {
        case TypeKind::VectorClass:
            return traverse(a_Input.staticCast<VectorClass>());
        case TypeKind::MapClass:
            return traverse(a_Input.staticCast<MapClass>());
        case TypeKind::SetClass:
            return traverse(a_Input.staticCast<SetClass>());
        case TypeKind::StringClass:
            return traverse(a_Input.staticCast<StringClass>());
        default:
            return this_()->traverseClass(a_Input);
        }
    }
    bool traverseClass(InstanceT<Class> a_Input)
    {
        if (!(this_()->walkUpVisitFromClass(a_Input)))
            return false;
        for (BaseClass const& bc : a_Input.getMeta()->getBaseClasses())
        {
            if (!(this_()->traverseBaseClass(InstanceT<BaseClass>(a_Input, bc))))
                return false;
        }
        if (!(this_()->traverseValueMembers(a_Input)))
            return false;
        return this_()->walkUpEndFromClass(a_Input);
    };

    // VectorClass

    bool visitVectorClass(InstanceT<VectorClass> a_Input) { return true; }
    bool endVectorClass(InstanceT<VectorClass> a_Input) { return true; }
    bool walkUpVisitFromVectorClass(InstanceT<VectorClass> a_Input)
    {
        if (!(this_()->walkUpVisitFromClass(a_Input)))
            return false;
        return this_()->visitVectorClass(a_Input);
    }
    bool walkUpEndFromVectorClass(InstanceT<VectorClass> a_Input)
    {
        if (!(this_()->endVectorClass(a_Input)))
            return false;
        return this_()->walkUpEndFromClass(a_Input);
    }
    bool traverse(InstanceT<VectorClass> a_Input) { return this_()->traverseVectorClass(a_Input); }
    bool traverseVectorClass(InstanceT<VectorClass> a_Input)
    {
        if (!(this_()->walkUpVisitFromVectorClass(a_Input)))
            return false;
        VectorClass* pVecClass = a_Input.getMeta();
        Type*        pValueType = pVecClass->getValueType();
        size_t       typeSz = pValueType->getSize();
        char*        data = (char*)pVecClass->data(a_Input.getAddress());
        if (!this_()->visitContainer(a_Input))
            return false;
        size_t count = pVecClass->size(a_Input.getAddress());
        for (size_t i = 0; i < count; ++i)
        {
            if (!(this_()->traverseContainerElement(InstanceT<Type>(pValueType, data + i * typeSz), i)))
                return false;
        };
        if (!this_()->endContainer(a_Input))
            return false;

        return this_()->walkUpEndFromVectorClass(a_Input);
    };

    // MapClass

    bool visitMapClass(InstanceT<MapClass> a_Input) { return true; }
    bool endMapClass(InstanceT<MapClass> a_Input) { return true; }
    bool walkUpVisitFromMapClass(InstanceT<MapClass> a_Input)
    {
        if (!(this_()->walkUpVisitFromClass(a_Input)))
            return false;
        return this_()->visitMapClass(a_Input);
    }
    bool walkUpEndFromMapClass(InstanceT<MapClass> a_Input)
    {
        if (!(this_()->endMapClass(a_Input)))
            return false;
        return this_()->walkUpEndFromClass(a_Input);
    }
    bool traverse(InstanceT<MapClass> a_Input) { return this_()->traverseMapClass(a_Input); }
    bool traverseMapClass(InstanceT<MapClass> a_Input)
    {
        if (!(this_()->walkUpVisitFromMapClass(a_Input)))
            return false;
        MapClass* pMapClass = a_Input.getMeta();
        Type*     pValueType = pMapClass->getValueType();
        if (!this_()->visitContainer(a_Input))
            return false;
        size_t count = pMapClass->size(a_Input.getAddress());
        for (size_t i = 0; i < count; ++i)
        {
            if (!(this_()->traverseContainerElement(
                InstanceT<Type>(pValueType, pMapClass->referenceAt(a_Input.getAddress(), i)), i)))
                return false;
        }
        if (!this_()->endContainer(a_Input))
            return false;
        return this_()->walkUpEndFromMapClass(a_Input);
    };

    // SetClass

    bool visitSetClass(InstanceT<SetClass> a_Input) { return true; }
    bool endSetClass(InstanceT<SetClass> a_Input) { return true; }
    bool walkUpVisitFromSetClass(InstanceT<SetClass> a_Input)
    {
        if (!(this_()->walkUpVisitFromClass(a_Input)))
            return false;
        return this_()->visitSetClass(a_Input);
    }
    bool walkUpEndFromSetClass(InstanceT<SetClass> a_Input)
    {
        if (!(this_()->endSetClass(a_Input)))
            return false;
        return this_()->walkUpEndFromClass(a_Input);
    }
    bool traverse(InstanceT<SetClass> a_Input) { return this_()->traverseSetClass(a_Input); }
    bool traverseSetClass(InstanceT<SetClass> a_Input)
    {
        if (!(this_()->walkUpVisitFromSetClass(a_Input)))
            return false;
        SetClass* pSetClass = a_Input.getMeta();
        Type*     pValueType = pSetClass->getValueType();
        if (!this_()->visitContainer(a_Input))
            return false;
        size_t count = pSetClass->size(a_Input.getAddress());
        for (size_t i = 0; i < count; ++i)
        {
            if (!(this_()->traverseContainerElement(
                InstanceT<Type>(pValueType, pSetClass->referenceAt(a_Input.getAddress(), i)), i)))
                return false;
        };
        if (!this_()->endContainer(a_Input))
            return false;
        return this_()->walkUpEndFromSetClass(a_Input);
    };

    // StringClass

    bool visitStringClass(InstanceT<StringClass> a_Input) { return true; }
    bool endStringClass(InstanceT<StringClass> a_Input) { return true; }
    bool walkUpVisitFromStringClass(InstanceT<StringClass> a_Input)
    {
        if (!(this_()->walkUpVisitFromClass(a_Input)))
            return false;
        return this_()->visitStringClass(a_Input);
    }
    bool walkUpEndFromStringClass(InstanceT<StringClass> a_Input)
    {
        if (!(this_()->endStringClass(a_Input)))
            return false;
        return this_()->walkUpEndFromClass(a_Input);
    }
    bool traverse(InstanceT<StringClass> a_Input) { return this_()->traverseStringClass(a_Input); }
    bool traverseStringClass(InstanceT<StringClass> a_Input)
    {
        if (!(this_()->walkUpVisitFromStringClass(a_Input)))
            return false;
        return this_()->walkUpEndFromStringClass(a_Input);
    };

    // Containers ..

    bool visitContainerElement(InstanceT<Type> a_Input, size_t a_Index) { return true; }

    bool endContainerElement(InstanceT<Type> a_Input, size_t a_Index) { return true; }

    bool traverseContainerElement(InstanceT<Type> a_Input, size_t a_Index)
    {
        if (!this_()->visitContainerElement(a_Input, a_Index))
            return false;
        if (!traverse(a_Input))
            return false;
        return this_()->endContainerElement(a_Input, a_Index);
    }

    bool visitContainer(InstanceT<Type> a_Input) { return true; }

    bool endContainer(InstanceT<Type> a_Input) { return true; }
};
} // namespace lang
} // namespace phantom
#pragma warning(default : 4100)
