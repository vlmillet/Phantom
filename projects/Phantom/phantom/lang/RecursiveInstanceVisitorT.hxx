#pragma once

// haunt {

// clang-format off

#include "RecursiveInstanceVisitorT.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/class>
#include <phantom/struct>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/typedef>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("RecursiveInstanceVisitorT")

        PHANTOM_STRUCT_T((class), (Meta), InstanceT)
        {
            this_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(Meta*, void*)>()({"a_pMeta","a_pAddress"})
            .PHANTOM_T method<bool() const>("operator bool", &_::operator notypedef<bool>)
            .PHANTOM_T method<bool(InstanceT<Meta> const&) const>("operator==", &_::operator==)({"other"})
            .PHANTOM_T method<bool(InstanceT<Meta> const&) const>("operator!=", &_::operator!=)({"other"})
            .PHANTOM_T method<void*(ptrdiff_t)>("getAddressWithOffset", &_::getAddressWithOffset)({"off"})
            .PHANTOM_T method<void*() const>("getAddress", &_::getAddress)
            .PHANTOM_T method<Meta*() const>("getMeta", &_::getMeta)
            ;
        }
        PHANTOM_STRUCT_S((phantom::lang::BaseClass), InstanceT)
        {
            this_()
            .constructor<void(), default_>()
            .constructor<void(InstanceT<Class>, BaseClass const&)>()({"a_Derived","a_BaseClass"})
            .method<bool() const>("operator bool", &_::operator notypedef<bool>)
            .method<bool(const ::phantom::lang::InstanceT<BaseClass> &) const>("operator==", &_::operator==)({"other"})
            .method<bool(const ::phantom::lang::InstanceT<BaseClass> &) const>("operator!=", &_::operator!=)({"other"})
            .method<InstanceT<BaseClass>(InstanceT<Class>)>("rebind", &_::rebind)({"other"})
            .method<InstanceT<Class>() const>("getDerivedInstance", &_::getDerivedInstance)
            .method<InstanceT<Class>() const>("getBaseClassInstance", &_::getBaseClassInstance)
            .method<BaseClass const&() const>("getBaseClass", &_::getBaseClass)
            ;
        }
        PHANTOM_STRUCT_T((class), (Meta), MemberAccessT)
        {
            this_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(InstanceT<ClassType>, Meta*)>()({"a_Owner","a_pMember"})
            .PHANTOM_T method<bool() const>("operator bool", &_::operator notypedef<bool>)
            .PHANTOM_T method<bool(MemberAccessT<Meta> const&) const>("operator==", &_::operator==)({"other"})
            .PHANTOM_T method<bool(MemberAccessT<Meta> const&) const>("operator!=", &_::operator!=)({"other"})
            .PHANTOM_T method<Meta*() const>("getMemberMeta", &_::getMemberMeta)
            .PHANTOM_T method<InstanceT<ClassType>() const>("getOwner", &_::getOwner)
            ;
        }
    PHANTOM_END("RecursiveInstanceVisitorT")
PHANTOM_END("phantom.lang")
namespace _RecursiveInstanceVisitorT {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("RecursiveInstanceVisitorT")

        PHANTOM_STRUCT_T((class), (T), DerivedSelector)
        {
            using type = typedef_<PHANTOM_TYPENAME _::type>;
            this_()
            .PHANTOM_T typedef_<type>("type")
            ;
        }
        PHANTOM_STRUCT_S((void), DerivedSelector)
        {
            using type = typedef_<_::type>;
            this_()
            .typedef_<type>("type")
            ;
        }
    PHANTOM_END("RecursiveInstanceVisitorT")
PHANTOM_END("phantom.lang")
}
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("RecursiveInstanceVisitorT")

        PHANTOM_CLASS_T((class), (Derived), RecursiveInstanceVisitorT)
        {
            using Impl = typedef_<PHANTOM_TYPENAME _::Impl>;
            this_()
        .public_()
            .PHANTOM_T typedef_<Impl>("Impl")
            .PHANTOM_T method<Impl*()>("this_", &_::this_)
            .PHANTOM_T method<Impl const*() const>("this_", &_::this_)
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>) const>("shouldWriteValueMember", &_::shouldWriteValueMember)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>) const>("shouldReadValueMember", &_::shouldReadValueMember)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("traverseValueMembers", &_::traverseValueMembers)({"a_Input"})
            .PHANTOM_T method<bool(void*, Type*)>("visit", &_::visit)({"a_pAddress","a_pType"})
            .PHANTOM_T method<bool(InstanceT<Type>)>("visitType", &_::visitType)({""})
            .PHANTOM_T method<bool(InstanceT<Type>)>("endType", &_::endType)({""})
            .PHANTOM_T method<bool(InstanceT<Type>)>("walkUpVisitFromType", &_::walkUpVisitFromType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Type>)>("walkUpEndFromType", &_::walkUpEndFromType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Type>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Type>)>("traverseType", &_::traverseType)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<bool> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<bool> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(bool*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<char> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<char> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(char*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<int8> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<int8> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(int8*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<uint8> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<uint8> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(uint8*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<int16> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<int16> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(int16*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<uint16> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<uint16> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(uint16*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<int32> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<int32> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(int32*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<uint32> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<uint32> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(uint32*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<int64> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<int64> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(int64*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<uint64> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<uint64> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(uint64*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<float> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<float> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(float*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<double> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<double> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(double*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<longdouble> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::lang::InstanceT<FundamentalTypeT<longdouble> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(longdouble*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<wchar_t> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<wchar_t> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(wchar_t*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<char16_t> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<char16_t> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(char16_t*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<char32_t> >)>("traverse", &_::traverse)({"a_Input"})
            /// missing symbol(s) reflection (phantom::lang::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<char32_t> >)>("traverseFundamentalType", &_::traverseFundamentalType)({"a_Input"})
            .PHANTOM_T method<bool(char32_t*)>("traverseFundamental", &_::traverseFundamental)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<QualifiedType>)>("visitQualifiedType", &_::visitQualifiedType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<QualifiedType>)>("endQualifiedType", &_::endQualifiedType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<QualifiedType>)>("walkUpVisitFromQualifiedType", &_::walkUpVisitFromQualifiedType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<QualifiedType>)>("walkUpEndFromQualifiedType", &_::walkUpEndFromQualifiedType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<QualifiedType>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<QualifiedType>)>("traverseQualifiedType", &_::traverseQualifiedType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ConstType>)>("visitConstType", &_::visitConstType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ConstType>)>("endConstType", &_::endConstType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ConstType>)>("walkUpVisitFromConstType", &_::walkUpVisitFromConstType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ConstType>)>("walkUpEndFromConstType", &_::walkUpEndFromConstType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ConstType>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ConstType>)>("traverseConstType", &_::traverseConstType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<VolatileType>)>("visitVolatileType", &_::visitVolatileType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<VolatileType>)>("endVolatileType", &_::endVolatileType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<VolatileType>)>("walkUpVisitFromVolatileType", &_::walkUpVisitFromVolatileType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<VolatileType>)>("walkUpEndFromVolatileType", &_::walkUpEndFromVolatileType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<VolatileType>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<VolatileType>)>("traverseVolatileType", &_::traverseVolatileType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ConstVolatileType>)>("visitConstVolatileType", &_::visitConstVolatileType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ConstVolatileType>)>("endConstVolatileType", &_::endConstVolatileType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ConstVolatileType>)>("walkUpVisitFromConstVolatileType", &_::walkUpVisitFromConstVolatileType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ConstVolatileType>)>("walkUpEndFromConstVolatileType", &_::walkUpEndFromConstVolatileType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ConstVolatileType>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ConstVolatileType>)>("traverseConstVolatileType", &_::traverseConstVolatileType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<PointerType>)>("visitPointerType", &_::visitPointerType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<PointerType>)>("endPointerType", &_::endPointerType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<PointerType>)>("walkUpVisitFromPointerType", &_::walkUpVisitFromPointerType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<PointerType>)>("walkUpEndFromPointerType", &_::walkUpEndFromPointerType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<PointerType>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<PointerType>)>("traversePointerType", &_::traversePointerType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FunctionPointer>)>("visitFunctionPointer", &_::visitFunctionPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FunctionPointer>)>("endFunctionPointer", &_::endFunctionPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FunctionPointer>)>("walkUpVisitFromFunctionPointer", &_::walkUpVisitFromFunctionPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FunctionPointer>)>("walkUpEndFromFunctionPointer", &_::walkUpEndFromFunctionPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FunctionPointer>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FunctionPointer>)>("traverseFunctionPointer", &_::traverseFunctionPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MemberPointer>)>("visitMemberPointer", &_::visitMemberPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MemberPointer>)>("endMemberPointer", &_::endMemberPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MemberPointer>)>("walkUpVisitFromMemberPointer", &_::walkUpVisitFromMemberPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MemberPointer>)>("walkUpEndFromMemberPointer", &_::walkUpEndFromMemberPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MemberPointer>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MemberPointer>)>("traverseMemberPointer", &_::traverseMemberPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MethodPointer>)>("visitMethodPointer", &_::visitMethodPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MethodPointer>)>("endMethodPointer", &_::endMethodPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MethodPointer>)>("walkUpVisitFromMethodPointer", &_::walkUpVisitFromMethodPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MethodPointer>)>("walkUpEndFromMethodPointer", &_::walkUpEndFromMethodPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MethodPointer>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MethodPointer>)>("traverseMethodPointer", &_::traverseMethodPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FieldPointer>)>("visitFieldPointer", &_::visitFieldPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FieldPointer>)>("endFieldPointer", &_::endFieldPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FieldPointer>)>("walkUpVisitFromFieldPointer", &_::walkUpVisitFromFieldPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FieldPointer>)>("walkUpEndFromFieldPointer", &_::walkUpEndFromFieldPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FieldPointer>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FieldPointer>)>("traverseFieldPointer", &_::traverseFieldPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FunctionType>)>("visitFunctionType", &_::visitFunctionType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FunctionType>)>("endFunctionType", &_::endFunctionType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FunctionType>)>("walkUpVisitFromFunctionType", &_::walkUpVisitFromFunctionType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FunctionType>)>("walkUpEndFromFunctionType", &_::walkUpEndFromFunctionType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FunctionType>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<FunctionType>)>("traverseFunctionType", &_::traverseFunctionType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Pointer>)>("visitPointer", &_::visitPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Pointer>)>("endPointer", &_::endPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Pointer>)>("walkUpVisitFromPointer", &_::walkUpVisitFromPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Pointer>)>("walkUpEndFromPointer", &_::walkUpEndFromPointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Pointer>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Pointer>)>("traversePointer", &_::traversePointer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Reference>)>("visitReference", &_::visitReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Reference>)>("endReference", &_::endReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Reference>)>("walkUpVisitFromReference", &_::walkUpVisitFromReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Reference>)>("walkUpEndFromReference", &_::walkUpEndFromReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Reference>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Reference>)>("traverseReference", &_::traverseReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<LValueReference>)>("visitLValueReference", &_::visitLValueReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<LValueReference>)>("endLValueReference", &_::endLValueReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<LValueReference>)>("walkUpVisitFromLValueReference", &_::walkUpVisitFromLValueReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<LValueReference>)>("walkUpEndFromLValueReference", &_::walkUpEndFromLValueReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<LValueReference>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<LValueReference>)>("traverseLValueReference", &_::traverseLValueReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<RValueReference>)>("visitRValueReference", &_::visitRValueReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<RValueReference>)>("endRValueReference", &_::endRValueReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<RValueReference>)>("walkUpVisitFromRValueReference", &_::walkUpVisitFromRValueReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<RValueReference>)>("walkUpEndFromRValueReference", &_::walkUpEndFromRValueReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<RValueReference>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<RValueReference>)>("traverseRValueReference", &_::traverseRValueReference)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<PrimitiveType>)>("visitPrimitiveType", &_::visitPrimitiveType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<PrimitiveType>)>("endPrimitiveType", &_::endPrimitiveType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<PrimitiveType>)>("walkUpVisitFromPrimitiveType", &_::walkUpVisitFromPrimitiveType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<PrimitiveType>)>("walkUpEndFromPrimitiveType", &_::walkUpEndFromPrimitiveType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<PrimitiveType>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<PrimitiveType>)>("traversePrimitiveType", &_::traversePrimitiveType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Enum>)>("visitEnum", &_::visitEnum)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Enum>)>("endEnum", &_::endEnum)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Enum>)>("walkUpVisitFromEnum", &_::walkUpVisitFromEnum)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Enum>)>("walkUpEndFromEnum", &_::walkUpEndFromEnum)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Enum>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Enum>)>("traverseEnum", &_::traverseEnum)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Array>)>("visitArray", &_::visitArray)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Array>)>("endArray", &_::endArray)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Array>)>("walkUpVisitFromArray", &_::walkUpVisitFromArray)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Array>)>("walkUpEndFromArray", &_::walkUpEndFromArray)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Array>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Array>)>("traverseArray", &_::traverseArray)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("visitClassType", &_::visitClassType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("endClassType", &_::endClassType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("walkUpVisitFromClassType", &_::walkUpVisitFromClassType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("walkUpEndFromClassType", &_::walkUpEndFromClassType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("traverseClassType", &_::traverseClassType)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Structure>)>("visitStructure", &_::visitStructure)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Structure>)>("endStructure", &_::endStructure)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Structure>)>("walkUpVisitFromStructure", &_::walkUpVisitFromStructure)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Structure>)>("walkUpEndFromStructure", &_::walkUpEndFromStructure)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Structure>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Structure>)>("traverseStructure", &_::traverseStructure)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Union>)>("visitUnion", &_::visitUnion)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Union>)>("endUnion", &_::endUnion)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Union>)>("walkUpVisitFromUnion", &_::walkUpVisitFromUnion)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Union>)>("walkUpEndFromUnion", &_::walkUpEndFromUnion)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Union>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Union>)>("traverseUnion", &_::traverseUnion)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>)>("visitValueMember", &_::visitValueMember)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>)>("endValueMember", &_::endValueMember)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>)>("walkUpVisitFromValueMember", &_::walkUpVisitFromValueMember)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>)>("walkUpEndFromValueMember", &_::walkUpEndFromValueMember)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>)>("traverseValueMember", &_::traverseValueMember)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>)>("_traverseValueMember", &_::_traverseValueMember)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<Field>)>("visitField", &_::visitField)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<Field>)>("endField", &_::endField)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<Field>)>("walkUpVisitFromField", &_::walkUpVisitFromField)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<Field>)>("walkUpEndFromField", &_::walkUpEndFromField)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<Field>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<Field>)>("traverseField", &_::traverseField)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<Property>)>("visitProperty", &_::visitProperty)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<Property>)>("endProperty", &_::endProperty)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<Property>)>("walkUpVisitFromProperty", &_::walkUpVisitFromProperty)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<Property>)>("walkUpEndFromProperty", &_::walkUpEndFromProperty)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<Property>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(MemberAccessT<Property>)>("traverseProperty", &_::traverseProperty)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<BaseClass>)>("visitBaseClass", &_::visitBaseClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<BaseClass>)>("endBaseClass", &_::endBaseClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<BaseClass>)>("traverseBaseClass", &_::traverseBaseClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<BaseClass>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Class>)>("visitClass", &_::visitClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Class>)>("endClass", &_::endClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Class>)>("walkUpVisitFromClass", &_::walkUpVisitFromClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Class>)>("walkUpEndFromClass", &_::walkUpEndFromClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Class>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Class>)>("traverseClass", &_::traverseClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<VectorClass>)>("visitVectorClass", &_::visitVectorClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<VectorClass>)>("endVectorClass", &_::endVectorClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<VectorClass>)>("walkUpVisitFromVectorClass", &_::walkUpVisitFromVectorClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<VectorClass>)>("walkUpEndFromVectorClass", &_::walkUpEndFromVectorClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<VectorClass>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<VectorClass>)>("traverseVectorClass", &_::traverseVectorClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MapClass>)>("visitMapClass", &_::visitMapClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MapClass>)>("endMapClass", &_::endMapClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MapClass>)>("walkUpVisitFromMapClass", &_::walkUpVisitFromMapClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MapClass>)>("walkUpEndFromMapClass", &_::walkUpEndFromMapClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MapClass>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<MapClass>)>("traverseMapClass", &_::traverseMapClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<SetClass>)>("visitSetClass", &_::visitSetClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<SetClass>)>("endSetClass", &_::endSetClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<SetClass>)>("walkUpVisitFromSetClass", &_::walkUpVisitFromSetClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<SetClass>)>("walkUpEndFromSetClass", &_::walkUpEndFromSetClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<SetClass>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<SetClass>)>("traverseSetClass", &_::traverseSetClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<StringClass>)>("visitStringClass", &_::visitStringClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<StringClass>)>("endStringClass", &_::endStringClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<StringClass>)>("walkUpVisitFromStringClass", &_::walkUpVisitFromStringClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<StringClass>)>("walkUpEndFromStringClass", &_::walkUpEndFromStringClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<StringClass>)>("traverse", &_::traverse)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<StringClass>)>("traverseStringClass", &_::traverseStringClass)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Type>, size_t)>("visitContainerElement", &_::visitContainerElement)({"a_Input","a_Index"})
            .PHANTOM_T method<bool(InstanceT<Type>, size_t)>("endContainerElement", &_::endContainerElement)({"a_Input","a_Index"})
            .PHANTOM_T method<bool(InstanceT<Type>, size_t)>("traverseContainerElement", &_::traverseContainerElement)({"a_Input","a_Index"})
            .PHANTOM_T method<bool(InstanceT<Type>)>("visitContainer", &_::visitContainer)({"a_Input"})
            .PHANTOM_T method<bool(InstanceT<Type>)>("endContainer", &_::endContainer)({"a_Input"})
            ;
        }
    PHANTOM_END("RecursiveInstanceVisitorT")
PHANTOM_END("phantom.lang")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
