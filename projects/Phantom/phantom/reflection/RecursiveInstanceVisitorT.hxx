#pragma once

// haunt {

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
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("RecursiveInstanceVisitorT")

        PHANTOM_STRUCT_T((class), (Meta), InstanceT)
        {
            this_()
            PHANTOM_IF((phantom::IsDefaultConstructible<PHANTOM_REFLECTED_TYPE>::value), 
            .PHANTOM_T constructor<void(), default_>()
            )
            .PHANTOM_T constructor<void(Meta*, void*)>()
            .PHANTOM_T method<bool() const>("operator bool", &_::operator notypedef<bool>)
            .PHANTOM_T method<bool(InstanceT<Meta> const&) const>("operator==", &_::operator==)
            .PHANTOM_T method<bool(InstanceT<Meta> const&) const>("operator!=", &_::operator!=)
            .PHANTOM_T method<void*(ptrdiff_t)>("getAddressWithOffset", &_::getAddressWithOffset)
            .PHANTOM_T method<void*() const>("getAddress", &_::getAddress)
            .PHANTOM_T method<Meta*() const>("getMeta", &_::getMeta)
            ;
        }
        PHANTOM_STRUCT_S((phantom::reflection::BaseClass), InstanceT)
        {
            this_()
            .constructor<void(), default_>()
            .constructor<void(InstanceT<Class>, BaseClass const&)>()
            .method<bool() const>("operator bool", &_::operator notypedef<bool>)
            .method<bool(const ::phantom::reflection::InstanceT<BaseClass> &) const>("operator==", &_::operator==)
            .method<bool(const ::phantom::reflection::InstanceT<BaseClass> &) const>("operator!=", &_::operator!=)
            .method<::phantom::reflection::InstanceT<BaseClass>(InstanceT<Class>)>("rebind", &_::rebind)
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
            .PHANTOM_T constructor<void(InstanceT<ClassType>, Meta*)>()
            .PHANTOM_T method<bool() const>("operator bool", &_::operator notypedef<bool>)
            .PHANTOM_T method<bool(MemberAccessT<Meta> const&) const>("operator==", &_::operator==)
            .PHANTOM_T method<bool(MemberAccessT<Meta> const&) const>("operator!=", &_::operator!=)
            .PHANTOM_T method<Meta*() const>("getMemberMeta", &_::getMemberMeta)
            .PHANTOM_T method<InstanceT<ClassType>() const>("getOwner", &_::getOwner)
            ;
        }
    PHANTOM_END("RecursiveInstanceVisitorT")
PHANTOM_END("phantom.reflection")
namespace _RecursiveInstanceVisitorT {
PHANTOM_PACKAGE("phantom.reflection")
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
PHANTOM_END("phantom.reflection")
}
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("RecursiveInstanceVisitorT")

        PHANTOM_CLASS_T((class), (Derived), RecursiveInstanceVisitorT)
        {
            using Impl = typedef_<PHANTOM_TYPENAME _::Impl>;
            this_()
        .public_()
            .PHANTOM_T typedef_<Impl>("Impl")
            .PHANTOM_T method<Impl*()>("this_", &_::this_)
            .PHANTOM_T method<Impl const*() const>("this_", &_::this_)
            .PHANTOM_T method<bool(MemberAccessT<Property>) const>("shouldUpdateProperty", &_::shouldUpdateProperty)
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("traverseValueMembers", &_::traverseValueMembers)
            .PHANTOM_T method<bool(void*, Type*)>("visit", &_::visit)
            .PHANTOM_T method<bool(InstanceT<Type>)>("visitType", &_::visitType)
            .PHANTOM_T method<bool(InstanceT<Type>)>("endType", &_::endType)
            .PHANTOM_T method<bool(InstanceT<Type>)>("walkUpVisitFromType", &_::walkUpVisitFromType)
            .PHANTOM_T method<bool(InstanceT<Type>)>("walkUpEndFromType", &_::walkUpEndFromType)
            .PHANTOM_T method<bool(InstanceT<Type>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<Type>)>("traverseType", &_::traverseType)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<bool> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<bool> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(bool*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<char> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<char> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(char*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<int8> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<int8> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(int8*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<uint8> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<uint8> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(uint8*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<int16> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<int16> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(int16*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<uint16> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<uint16> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(uint16*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<int32> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<int32> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(int32*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<uint32> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<uint32> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(uint32*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<int64> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<int64> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(int64*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<uint64> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<uint64> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(uint64*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<float> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<float> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(float*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<double> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<double> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(double*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<longdouble> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(::phantom::reflection::InstanceT<FundamentalTypeT<longdouble> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(longdouble*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<wchar_t> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<wchar_t> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(wchar_t*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<char16_t> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<char16_t> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(char16_t*)>("traverseFundamental", &_::traverseFundamental)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<char32_t> >)>("traverse", &_::traverse)
            /// missing symbol(s) reflection (phantom::reflection::FundamentalTypeT) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .PHANTOM_T method<bool(InstanceT<FundamentalTypeT<char32_t> >)>("traverseFundamentalType", &_::traverseFundamentalType)
            .PHANTOM_T method<bool(char32_t*)>("traverseFundamental", &_::traverseFundamental)
            .PHANTOM_T method<bool(InstanceT<ExtendedType>)>("visitExtendedType", &_::visitExtendedType)
            .PHANTOM_T method<bool(InstanceT<ExtendedType>)>("endExtendedType", &_::endExtendedType)
            .PHANTOM_T method<bool(InstanceT<ExtendedType>)>("walkUpVisitFromExtendedType", &_::walkUpVisitFromExtendedType)
            .PHANTOM_T method<bool(InstanceT<ExtendedType>)>("walkUpEndFromExtendedType", &_::walkUpEndFromExtendedType)
            .PHANTOM_T method<bool(InstanceT<ExtendedType>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<ExtendedType>)>("traverseExtendedType", &_::traverseExtendedType)
            .PHANTOM_T method<bool(InstanceT<QualifiedType>)>("visitQualifiedType", &_::visitQualifiedType)
            .PHANTOM_T method<bool(InstanceT<QualifiedType>)>("endQualifiedType", &_::endQualifiedType)
            .PHANTOM_T method<bool(InstanceT<QualifiedType>)>("walkUpVisitFromQualifiedType", &_::walkUpVisitFromQualifiedType)
            .PHANTOM_T method<bool(InstanceT<QualifiedType>)>("walkUpEndFromQualifiedType", &_::walkUpEndFromQualifiedType)
            .PHANTOM_T method<bool(InstanceT<QualifiedType>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<QualifiedType>)>("traverseQualifiedType", &_::traverseQualifiedType)
            .PHANTOM_T method<bool(InstanceT<ConstType>)>("visitConstType", &_::visitConstType)
            .PHANTOM_T method<bool(InstanceT<ConstType>)>("endConstType", &_::endConstType)
            .PHANTOM_T method<bool(InstanceT<ConstType>)>("walkUpVisitFromConstType", &_::walkUpVisitFromConstType)
            .PHANTOM_T method<bool(InstanceT<ConstType>)>("walkUpEndFromConstType", &_::walkUpEndFromConstType)
            .PHANTOM_T method<bool(InstanceT<ConstType>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<ConstType>)>("traverseConstType", &_::traverseConstType)
            .PHANTOM_T method<bool(InstanceT<VolatileType>)>("visitVolatileType", &_::visitVolatileType)
            .PHANTOM_T method<bool(InstanceT<VolatileType>)>("endVolatileType", &_::endVolatileType)
            .PHANTOM_T method<bool(InstanceT<VolatileType>)>("walkUpVisitFromVolatileType", &_::walkUpVisitFromVolatileType)
            .PHANTOM_T method<bool(InstanceT<VolatileType>)>("walkUpEndFromVolatileType", &_::walkUpEndFromVolatileType)
            .PHANTOM_T method<bool(InstanceT<VolatileType>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<VolatileType>)>("traverseVolatileType", &_::traverseVolatileType)
            .PHANTOM_T method<bool(InstanceT<ConstVolatileType>)>("visitConstVolatileType", &_::visitConstVolatileType)
            .PHANTOM_T method<bool(InstanceT<ConstVolatileType>)>("endConstVolatileType", &_::endConstVolatileType)
            .PHANTOM_T method<bool(InstanceT<ConstVolatileType>)>("walkUpVisitFromConstVolatileType", &_::walkUpVisitFromConstVolatileType)
            .PHANTOM_T method<bool(InstanceT<ConstVolatileType>)>("walkUpEndFromConstVolatileType", &_::walkUpEndFromConstVolatileType)
            .PHANTOM_T method<bool(InstanceT<ConstVolatileType>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<ConstVolatileType>)>("traverseConstVolatileType", &_::traverseConstVolatileType)
            .PHANTOM_T method<bool(InstanceT<PointerType>)>("visitPointerType", &_::visitPointerType)
            .PHANTOM_T method<bool(InstanceT<PointerType>)>("endPointerType", &_::endPointerType)
            .PHANTOM_T method<bool(InstanceT<PointerType>)>("walkUpVisitFromPointerType", &_::walkUpVisitFromPointerType)
            .PHANTOM_T method<bool(InstanceT<PointerType>)>("walkUpEndFromPointerType", &_::walkUpEndFromPointerType)
            .PHANTOM_T method<bool(InstanceT<PointerType>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<PointerType>)>("traversePointerType", &_::traversePointerType)
            .PHANTOM_T method<bool(InstanceT<FunctionPointer>)>("visitFunctionPointer", &_::visitFunctionPointer)
            .PHANTOM_T method<bool(InstanceT<FunctionPointer>)>("endFunctionPointer", &_::endFunctionPointer)
            .PHANTOM_T method<bool(InstanceT<FunctionPointer>)>("walkUpVisitFromFunctionPointer", &_::walkUpVisitFromFunctionPointer)
            .PHANTOM_T method<bool(InstanceT<FunctionPointer>)>("walkUpEndFromFunctionPointer", &_::walkUpEndFromFunctionPointer)
            .PHANTOM_T method<bool(InstanceT<FunctionPointer>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<FunctionPointer>)>("traverseFunctionPointer", &_::traverseFunctionPointer)
            .PHANTOM_T method<bool(InstanceT<MemberPointer>)>("visitMemberPointer", &_::visitMemberPointer)
            .PHANTOM_T method<bool(InstanceT<MemberPointer>)>("endMemberPointer", &_::endMemberPointer)
            .PHANTOM_T method<bool(InstanceT<MemberPointer>)>("walkUpVisitFromMemberPointer", &_::walkUpVisitFromMemberPointer)
            .PHANTOM_T method<bool(InstanceT<MemberPointer>)>("walkUpEndFromMemberPointer", &_::walkUpEndFromMemberPointer)
            .PHANTOM_T method<bool(InstanceT<MemberPointer>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<MemberPointer>)>("traverseMemberPointer", &_::traverseMemberPointer)
            .PHANTOM_T method<bool(InstanceT<MethodPointer>)>("visitMethodPointer", &_::visitMethodPointer)
            .PHANTOM_T method<bool(InstanceT<MethodPointer>)>("endMethodPointer", &_::endMethodPointer)
            .PHANTOM_T method<bool(InstanceT<MethodPointer>)>("walkUpVisitFromMethodPointer", &_::walkUpVisitFromMethodPointer)
            .PHANTOM_T method<bool(InstanceT<MethodPointer>)>("walkUpEndFromMethodPointer", &_::walkUpEndFromMethodPointer)
            .PHANTOM_T method<bool(InstanceT<MethodPointer>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<MethodPointer>)>("traverseMethodPointer", &_::traverseMethodPointer)
            .PHANTOM_T method<bool(InstanceT<FieldPointer>)>("visitFieldPointer", &_::visitFieldPointer)
            .PHANTOM_T method<bool(InstanceT<FieldPointer>)>("endFieldPointer", &_::endFieldPointer)
            .PHANTOM_T method<bool(InstanceT<FieldPointer>)>("walkUpVisitFromFieldPointer", &_::walkUpVisitFromFieldPointer)
            .PHANTOM_T method<bool(InstanceT<FieldPointer>)>("walkUpEndFromFieldPointer", &_::walkUpEndFromFieldPointer)
            .PHANTOM_T method<bool(InstanceT<FieldPointer>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<FieldPointer>)>("traverseFieldPointer", &_::traverseFieldPointer)
            .PHANTOM_T method<bool(InstanceT<FunctionType>)>("visitFunctionType", &_::visitFunctionType)
            .PHANTOM_T method<bool(InstanceT<FunctionType>)>("endFunctionType", &_::endFunctionType)
            .PHANTOM_T method<bool(InstanceT<FunctionType>)>("walkUpVisitFromFunctionType", &_::walkUpVisitFromFunctionType)
            .PHANTOM_T method<bool(InstanceT<FunctionType>)>("walkUpEndFromFunctionType", &_::walkUpEndFromFunctionType)
            .PHANTOM_T method<bool(InstanceT<FunctionType>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<FunctionType>)>("traverseFunctionType", &_::traverseFunctionType)
            .PHANTOM_T method<bool(InstanceT<Pointer>)>("visitPointer", &_::visitPointer)
            .PHANTOM_T method<bool(InstanceT<Pointer>)>("endPointer", &_::endPointer)
            .PHANTOM_T method<bool(InstanceT<Pointer>)>("walkUpVisitFromPointer", &_::walkUpVisitFromPointer)
            .PHANTOM_T method<bool(InstanceT<Pointer>)>("walkUpEndFromPointer", &_::walkUpEndFromPointer)
            .PHANTOM_T method<bool(InstanceT<Pointer>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<Pointer>)>("traversePointer", &_::traversePointer)
            .PHANTOM_T method<bool(InstanceT<Reference>)>("visitReference", &_::visitReference)
            .PHANTOM_T method<bool(InstanceT<Reference>)>("endReference", &_::endReference)
            .PHANTOM_T method<bool(InstanceT<Reference>)>("walkUpVisitFromReference", &_::walkUpVisitFromReference)
            .PHANTOM_T method<bool(InstanceT<Reference>)>("walkUpEndFromReference", &_::walkUpEndFromReference)
            .PHANTOM_T method<bool(InstanceT<Reference>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<Reference>)>("traverseReference", &_::traverseReference)
            .PHANTOM_T method<bool(InstanceT<LValueReference>)>("visitLValueReference", &_::visitLValueReference)
            .PHANTOM_T method<bool(InstanceT<LValueReference>)>("endLValueReference", &_::endLValueReference)
            .PHANTOM_T method<bool(InstanceT<LValueReference>)>("walkUpVisitFromLValueReference", &_::walkUpVisitFromLValueReference)
            .PHANTOM_T method<bool(InstanceT<LValueReference>)>("walkUpEndFromLValueReference", &_::walkUpEndFromLValueReference)
            .PHANTOM_T method<bool(InstanceT<LValueReference>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<LValueReference>)>("traverseLValueReference", &_::traverseLValueReference)
            .PHANTOM_T method<bool(InstanceT<RValueReference>)>("visitRValueReference", &_::visitRValueReference)
            .PHANTOM_T method<bool(InstanceT<RValueReference>)>("endRValueReference", &_::endRValueReference)
            .PHANTOM_T method<bool(InstanceT<RValueReference>)>("walkUpVisitFromRValueReference", &_::walkUpVisitFromRValueReference)
            .PHANTOM_T method<bool(InstanceT<RValueReference>)>("walkUpEndFromRValueReference", &_::walkUpEndFromRValueReference)
            .PHANTOM_T method<bool(InstanceT<RValueReference>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<RValueReference>)>("traverseRValueReference", &_::traverseRValueReference)
            .PHANTOM_T method<bool(InstanceT<PrimitiveType>)>("visitPrimitiveType", &_::visitPrimitiveType)
            .PHANTOM_T method<bool(InstanceT<PrimitiveType>)>("endPrimitiveType", &_::endPrimitiveType)
            .PHANTOM_T method<bool(InstanceT<PrimitiveType>)>("walkUpVisitFromPrimitiveType", &_::walkUpVisitFromPrimitiveType)
            .PHANTOM_T method<bool(InstanceT<PrimitiveType>)>("walkUpEndFromPrimitiveType", &_::walkUpEndFromPrimitiveType)
            .PHANTOM_T method<bool(InstanceT<PrimitiveType>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<PrimitiveType>)>("traversePrimitiveType", &_::traversePrimitiveType)
            .PHANTOM_T method<bool(InstanceT<Enum>)>("visitEnum", &_::visitEnum)
            .PHANTOM_T method<bool(InstanceT<Enum>)>("endEnum", &_::endEnum)
            .PHANTOM_T method<bool(InstanceT<Enum>)>("walkUpVisitFromEnum", &_::walkUpVisitFromEnum)
            .PHANTOM_T method<bool(InstanceT<Enum>)>("walkUpEndFromEnum", &_::walkUpEndFromEnum)
            .PHANTOM_T method<bool(InstanceT<Enum>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<Enum>)>("traverseEnum", &_::traverseEnum)
            .PHANTOM_T method<bool(InstanceT<Array>)>("visitArray", &_::visitArray)
            .PHANTOM_T method<bool(InstanceT<Array>)>("endArray", &_::endArray)
            .PHANTOM_T method<bool(InstanceT<Array>)>("walkUpVisitFromArray", &_::walkUpVisitFromArray)
            .PHANTOM_T method<bool(InstanceT<Array>)>("walkUpEndFromArray", &_::walkUpEndFromArray)
            .PHANTOM_T method<bool(InstanceT<Array>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<Array>)>("traverseArray", &_::traverseArray)
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("visitClassType", &_::visitClassType)
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("endClassType", &_::endClassType)
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("walkUpVisitFromClassType", &_::walkUpVisitFromClassType)
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("walkUpEndFromClassType", &_::walkUpEndFromClassType)
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<ClassType>)>("traverseClassType", &_::traverseClassType)
            .PHANTOM_T method<bool(InstanceT<Structure>)>("visitStructure", &_::visitStructure)
            .PHANTOM_T method<bool(InstanceT<Structure>)>("endStructure", &_::endStructure)
            .PHANTOM_T method<bool(InstanceT<Structure>)>("walkUpVisitFromStructure", &_::walkUpVisitFromStructure)
            .PHANTOM_T method<bool(InstanceT<Structure>)>("walkUpEndFromStructure", &_::walkUpEndFromStructure)
            .PHANTOM_T method<bool(InstanceT<Structure>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<Structure>)>("traverseStructure", &_::traverseStructure)
            .PHANTOM_T method<bool(InstanceT<Union>)>("visitUnion", &_::visitUnion)
            .PHANTOM_T method<bool(InstanceT<Union>)>("endUnion", &_::endUnion)
            .PHANTOM_T method<bool(InstanceT<Union>)>("walkUpVisitFromUnion", &_::walkUpVisitFromUnion)
            .PHANTOM_T method<bool(InstanceT<Union>)>("walkUpEndFromUnion", &_::walkUpEndFromUnion)
            .PHANTOM_T method<bool(InstanceT<Union>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<Union>)>("traverseUnion", &_::traverseUnion)
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>)>("visitValueMember", &_::visitValueMember)
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>)>("endValueMember", &_::endValueMember)
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>)>("walkUpVisitFromValueMember", &_::walkUpVisitFromValueMember)
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>)>("walkUpEndFromValueMember", &_::walkUpEndFromValueMember)
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(MemberAccessT<ValueMember>)>("traverseValueMember", &_::traverseValueMember)
            .PHANTOM_T method<bool(MemberAccessT<Field>)>("visitField", &_::visitField)
            .PHANTOM_T method<bool(MemberAccessT<Field>)>("endField", &_::endField)
            .PHANTOM_T method<bool(MemberAccessT<Field>)>("walkUpVisitFromField", &_::walkUpVisitFromField)
            .PHANTOM_T method<bool(MemberAccessT<Field>)>("walkUpEndFromField", &_::walkUpEndFromField)
            .PHANTOM_T method<bool(MemberAccessT<Field>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(MemberAccessT<Field>)>("traverseField", &_::traverseField)
            .PHANTOM_T method<bool(MemberAccessT<Property>)>("visitProperty", &_::visitProperty)
            .PHANTOM_T method<bool(MemberAccessT<Property>)>("endProperty", &_::endProperty)
            .PHANTOM_T method<bool(MemberAccessT<Property>)>("walkUpVisitFromProperty", &_::walkUpVisitFromProperty)
            .PHANTOM_T method<bool(MemberAccessT<Property>)>("walkUpEndFromProperty", &_::walkUpEndFromProperty)
            .PHANTOM_T method<bool(MemberAccessT<Property>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(MemberAccessT<Property>)>("traverseProperty", &_::traverseProperty)
            .PHANTOM_T method<bool(InstanceT<BaseClass>)>("visitBaseClass", &_::visitBaseClass)
            .PHANTOM_T method<bool(InstanceT<BaseClass>)>("endBaseClass", &_::endBaseClass)
            .PHANTOM_T method<bool(InstanceT<BaseClass>)>("traverseBaseClass", &_::traverseBaseClass)
            .PHANTOM_T method<bool(InstanceT<BaseClass>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<Class>)>("visitClass", &_::visitClass)
            .PHANTOM_T method<bool(InstanceT<Class>)>("endClass", &_::endClass)
            .PHANTOM_T method<bool(InstanceT<Class>)>("walkUpVisitFromClass", &_::walkUpVisitFromClass)
            .PHANTOM_T method<bool(InstanceT<Class>)>("walkUpEndFromClass", &_::walkUpEndFromClass)
            .PHANTOM_T method<bool(InstanceT<Class>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<Class>)>("traverseClass", &_::traverseClass)
            .PHANTOM_T method<bool(InstanceT<StlVectorClass>)>("visitStlVectorClass", &_::visitStlVectorClass)
            .PHANTOM_T method<bool(InstanceT<StlVectorClass>)>("endStlVectorClass", &_::endStlVectorClass)
            .PHANTOM_T method<bool(InstanceT<StlVectorClass>)>("walkUpVisitFromStlVectorClass", &_::walkUpVisitFromStlVectorClass)
            .PHANTOM_T method<bool(InstanceT<StlVectorClass>)>("walkUpEndFromStlVectorClass", &_::walkUpEndFromStlVectorClass)
            .PHANTOM_T method<bool(InstanceT<StlVectorClass>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<StlVectorClass>)>("traverseStlVectorClass", &_::traverseStlVectorClass)
            .PHANTOM_T method<bool(InstanceT<StlMapClass>)>("visitStlMapClass", &_::visitStlMapClass)
            .PHANTOM_T method<bool(InstanceT<StlMapClass>)>("endStlMapClass", &_::endStlMapClass)
            .PHANTOM_T method<bool(InstanceT<StlMapClass>)>("walkUpVisitFromStlMapClass", &_::walkUpVisitFromStlMapClass)
            .PHANTOM_T method<bool(InstanceT<StlMapClass>)>("walkUpEndFromStlMapClass", &_::walkUpEndFromStlMapClass)
            .PHANTOM_T method<bool(InstanceT<StlMapClass>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<StlMapClass>)>("traverseStlMapClass", &_::traverseStlMapClass)
            .PHANTOM_T method<bool(InstanceT<StlSetClass>)>("visitStlSetClass", &_::visitStlSetClass)
            .PHANTOM_T method<bool(InstanceT<StlSetClass>)>("endStlSetClass", &_::endStlSetClass)
            .PHANTOM_T method<bool(InstanceT<StlSetClass>)>("walkUpVisitFromStlSetClass", &_::walkUpVisitFromStlSetClass)
            .PHANTOM_T method<bool(InstanceT<StlSetClass>)>("walkUpEndFromStlSetClass", &_::walkUpEndFromStlSetClass)
            .PHANTOM_T method<bool(InstanceT<StlSetClass>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<StlSetClass>)>("traverseStlSetClass", &_::traverseStlSetClass)
            .PHANTOM_T method<bool(InstanceT<StringClass>)>("visitStringClass", &_::visitStringClass)
            .PHANTOM_T method<bool(InstanceT<StringClass>)>("endStringClass", &_::endStringClass)
            .PHANTOM_T method<bool(InstanceT<StringClass>)>("walkUpVisitFromStringClass", &_::walkUpVisitFromStringClass)
            .PHANTOM_T method<bool(InstanceT<StringClass>)>("walkUpEndFromStringClass", &_::walkUpEndFromStringClass)
            .PHANTOM_T method<bool(InstanceT<StringClass>)>("traverse", &_::traverse)
            .PHANTOM_T method<bool(InstanceT<StringClass>)>("traverseStringClass", &_::traverseStringClass)
            .PHANTOM_T method<bool(InstanceT<Type>, size_t)>("visitContainerElement", &_::visitContainerElement)
            .PHANTOM_T method<bool(InstanceT<Type>, size_t)>("endContainerElement", &_::endContainerElement)
            .PHANTOM_T method<bool(InstanceT<Type>, size_t)>("traverseContainerElement", &_::traverseContainerElement)
            .PHANTOM_T method<bool(InstanceT<Type>)>("visitContainer", &_::visitContainer)
            .PHANTOM_T method<bool(InstanceT<Type>)>("endContainer", &_::endContainer)
            ;
        }
    PHANTOM_END("RecursiveInstanceVisitorT")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
