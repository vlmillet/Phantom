#pragma once

// haunt {

// clang-format off

#include "Type.h"

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
#include <phantom/enum_class>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/Delegate.hxx>
#include <phantom/utils/Signal.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Type")

        PHANTOM_STRUCT_T((class), (ScopeExit), ScopedConstruction)
        {
            this_()
            .PHANTOM_T constructor<void(ScopeExit&&, void*)>()
            .PHANTOM_T method<void*() const>("operator void*", &_::operator notypedef<void*>)
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Type)
        {
            using AggregateFields = typedef_< phantom::lang::AggregateFields>;
            using AlignmentComputer = typedef_<_::AlignmentComputer>;
            using DataElements = typedef_< phantom::lang::DataElements>;
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using PlaceholderMap = typedef_< phantom::lang::PlaceholderMap>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using TypeFilter = typedef_<_::TypeFilter>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .typedef_<TypeFilter>("TypeFilter")
            .staticMethod<bool(Type*)>("DataPointerFilter", &_::DataPointerFilter)
            .staticMethod<bool(Type*)>("NoFilter", &_::NoFilter)
            .enum_<TypeRelation>().values({
                {"None",_::TypeRelation::None},
                {"Equal",_::TypeRelation::Equal},
                {"Child",_::TypeRelation::Child},
                {"Parent",_::TypeRelation::Parent},
                {"Compatible",_::TypeRelation::Compatible},
                {"GenericContentChild",_::TypeRelation::GenericContentChild},
                {"GenericContentParent",_::TypeRelation::GenericContentParent}})
            .end()
            .class_<AlignmentComputer>()
            .public_()
                .constructor<void(size_t)>()["0"]
                .method<void(DataElements const&, size_t&, size_t&)>("alignStruct", &_::AlignmentComputer::alignStruct)
                .method<void(DataElements const&, size_t&, size_t&)>("alignUnion", &_::AlignmentComputer::alignUnion)
                .method<size_t(Type*)>("push", &_::AlignmentComputer::push)
                .method<size_t(size_t, size_t)>("push", &_::AlignmentComputer::push)
                .method<size_t()>("align", &_::AlignmentComputer::align)
                .method<size_t() const>("maxAlignment", &_::AlignmentComputer::maxAlignment)
                .method<void(size_t)>("setMaxAlignement", &_::AlignmentComputer::setMaxAlignement)
                .method<void()>("reset", &_::AlignmentComputer::reset)
            .end()
            .staticMethod<int(TypeKind)>("ConversionRank", &_::ConversionRank)
        
        .protected_()
            .constructor<void(TypeKind, Type*, Modifiers, uint)>()["0"]["0"]
            .constructor<void(TypeKind, Modifiers, uint)>()["0"]["0"]
            .constructor<void(TypeKind, StringView, Modifiers, uint)>()["0"]["0"]
            .constructor<void(TypeKind, StringView, size_t, size_t, Modifiers, uint)>()
            .constructor<void(TypeKind, Type*, StringView, size_t, size_t, Modifiers, uint)>()
            .constructor<void(TypeKind, Type*, StringView, Modifiers, uint)>()["0"]["0"]
        
        .public_()
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<::phantom::lang::TypeKind() const>("getTypeKind", &_::getTypeKind)
            .method<size_t() const>("getSize", &_::getSize)
            .method<bool() const>("isSized", &_::isSized)
            .method<void(size_t)>("setSize", &_::setSize)
            .method<bool() const>("isVoid", &_::isVoid)
            .method<void(AggregateFields&) const, virtual_>("getFlattenedAggregateFields", &_::getFlattenedAggregateFields)
            .method<bool(Type*) const, virtual_>("isEquivalent", &_::isEquivalent)
            .method<size_t() const>("getAlignment", &_::getAlignment)
            .method<void(size_t)>("setAlignment", &_::setAlignment)
            .method<bool() const>("isSingleton", &_::isSingleton)
            .method<bool() const, virtual_>("isDefaultConstructible", &_::isDefaultConstructible)
            .method<bool() const, virtual_>("isDefaultInstanciable", &_::isDefaultInstanciable)
            .method<bool(Type*) const, virtual_>("isA", &_::isA)
            .method<bool() const>("isAuto", &_::isAuto)
            .method<bool() const>("hasAuto", &_::hasAuto)
            .method<Type*() const, virtual_|override_>("toType", &_::toType)
            .method<Type*() const, virtual_|final_|override_>("asType", &_::asType)
            .method<::phantom::lang::Type *() const, virtual_|final_|override_>("asAddressType", &_::asAddressType)
            .method<bool() const>("isValuePointerKind", &_::isValuePointerKind)
            .method<uint8_t()>("getAddressLevel", &_::getAddressLevel)
            .method<::phantom::lang::ClassType *() const, virtual_|final_|override_>("asClassType", &_::asClassType)
            .method<::phantom::lang::Array *() const, virtual_|final_|override_>("asArray", &_::asArray)
            .method<::phantom::lang::Enum *() const, virtual_|final_|override_>("asEnum", &_::asEnum)
            .method<::phantom::lang::LValueReference *() const, virtual_|final_|override_>("asLValueReference", &_::asLValueReference)
            .method<::phantom::lang::RValueReference *() const, virtual_|final_|override_>("asRValueReference", &_::asRValueReference)
            .method<::phantom::lang::Reference *() const, virtual_|final_|override_>("asReference", &_::asReference)
            .method<::phantom::lang::Pointer *() const, virtual_|final_|override_>("asPointer", &_::asPointer)
            .method<::phantom::lang::ConstType *() const, virtual_|final_|override_>("asConstType", &_::asConstType)
            .method<::phantom::lang::VolatileType *() const, virtual_|final_|override_>("asVolatileType", &_::asVolatileType)
            .method<::phantom::lang::ConstVolatileType *() const, virtual_|final_|override_>("asConstVolatileType", &_::asConstVolatileType)
            .method<::phantom::lang::PointerType *() const, virtual_|final_|override_>("asPointerType", &_::asPointerType)
            .method<::phantom::lang::Class *() const, virtual_|final_|override_>("asClass", &_::asClass)
            .method<::phantom::lang::VectorClass *() const, virtual_|final_|override_>("asVectorClass", &_::asVectorClass)
            .method<::phantom::lang::MapClass *() const, virtual_|final_|override_>("asMapClass", &_::asMapClass)
            .method<::phantom::lang::SetClass *() const, virtual_|final_|override_>("asSetClass", &_::asSetClass)
            .method<::phantom::lang::StringClass *() const, virtual_|final_|override_>("asStringClass", &_::asStringClass)
            /// missing symbol(s) reflection (phantom::lang::ArrayClass) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<::phantom::lang::ArrayClass *() const, virtual_|final_|override_>("asArrayClass", &_::asArrayClass)
            .method<::phantom::lang::LanguageElement *() const, virtual_>("asTemplateElement", &_::asTemplateElement)
            .method<::phantom::lang::LanguageElement *() const, virtual_>("asLanguageElement", &_::asLanguageElement)
            .method<bool() const, virtual_>("isPolymorphic", &_::isPolymorphic)
            .method<bool(Type*) const, virtual_>("hasStrongDependencyOnType", &_::hasStrongDependencyOnType)
            .method<Type*() const>("getUnderlyingType", &_::getUnderlyingType)
            .method<Type*() const>("addConst", &_::addConst)
            .method<Type*() const>("addVolatile", &_::addVolatile)
            .method<Type*() const>("addConstLValueReference", &_::addConstLValueReference)
            .method<Type*() const>("addConstRValueReference", &_::addConstRValueReference)
            .method<Type*() const>("addConstVolatile", &_::addConstVolatile)
            .method<Type*(size_t) const>("addArray", &_::addArray)
            .method<::phantom::lang::Type *() const>("addLValueReference", &_::addLValueReference)
            .method<::phantom::lang::Type *() const>("addRValueReference", &_::addRValueReference)
            .method<::phantom::lang::Type *() const>("addPointer", &_::addPointer)
            .method<::phantom::lang::Type *(int) const, virtual_>("addCustom", &_::addCustom)
            .method<::phantom::lang::Type *() const>("removeConst", &_::removeConst)
            .method<::phantom::lang::Type *() const>("removeVolatile", &_::removeVolatile)
            .method<::phantom::lang::Type *() const>("removeConstVolatile", &_::removeConstVolatile)
            .method<::phantom::lang::Type *() const>("removeQualifiers", &_::removeQualifiers)
            .method<::phantom::lang::Type *() const>("removeConstReference", &_::removeConstReference)
            .method<Type*() const>("removeConstLValueReference", &_::removeConstLValueReference)
            .method<::phantom::lang::Type *() const>("removePointerOrArray", &_::removePointerOrArray)
            .method<::phantom::lang::Type *() const>("removeAddress", &_::removeAddress)
            .method<::phantom::lang::Type *() const>("removeArray", &_::removeArray)
            .method<::phantom::lang::Type *() const>("removeExtent", &_::removeExtent)
            .method<::phantom::lang::Type *() const>("removeArrays", &_::removeArrays)
            .method<::phantom::lang::Type *() const>("removeAllExtents", &_::removeAllExtents)
            .method<Type*() const>("removeReference", &_::removeReference)
            .method<::phantom::lang::Type *() const>("removeLValueReference", &_::removeLValueReference)
            .method<::phantom::lang::Type *() const>("removeRValueReference", &_::removeRValueReference)
            .method<::phantom::lang::Type *() const>("removePointer", &_::removePointer)
            .method<::phantom::lang::Type *() const>("removePointerType", &_::removePointerType)
            .method<::phantom::lang::Type *() const>("removeAllConst", &_::removeAllConst)
            .method<::phantom::lang::Type *() const>("removeAllVolatile", &_::removeAllVolatile)
            .method<::phantom::lang::Type *() const>("removeAllConstVolatile", &_::removeAllConstVolatile)
            .method<Type*() const>("removeAllQualifiers", &_::removeAllQualifiers)
            .method<::phantom::lang::Type *() const>("removeEverything", &_::removeEverything)
            .method<::phantom::lang::Type *(int) const, virtual_>("removeCustom", &_::removeCustom)
            .method<::phantom::lang::Type *(Type*) const, virtual_>("replicate", &_::replicate)
            .method<bool(Type*, void*, void const*) const, virtual_>("convert", &_::convert)
            .method<void*(Type*, void*) const, virtual_>("cast", &_::cast)
            .method<const void*(Type*, void const*) const, virtual_>("cast", &_::cast)
            .method<void*(Type*, void*) const, virtual_>("upcast", &_::upcast)
            .method<void*(Type*, void*) const, virtual_>("downcast", &_::downcast)
            .method<void*() const, virtual_>("allocate", &_::allocate)
            .method<void(void*) const, virtual_>("deallocate", &_::deallocate)
            .method<void*(size_t) const, virtual_>("allocate", &_::allocate)
            .method<void(void*, size_t) const, virtual_>("deallocate", &_::deallocate)
            .method<void(void*) const, virtual_>("construct", &_::construct)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<::phantom::lang::ScopedConstruction<ScopeExit<(lambda at C:/Development/Phantom/projects/phantom/phantom/lang/Type.h:1079:53)> >(void*) const>("scopedConstruct", &_::scopedConstruct)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<::phantom::lang::ScopedConstruction<ScopeExit<(lambda at C:/Development/Phantom/projects/phantom/phantom/lang/Type.h:1079:53)> >(void*) const>("localConstruct", &_::localConstruct)
            .method<void(void*) const, virtual_>("destroy", &_::destroy)
            .method<uint64_t(const void*) const, virtual_>("hash", &_::hash)
            .method<void*() const, virtual_>("newInstance", &_::newInstance)
            .method<void(void*) const, virtual_>("deleteInstance", &_::deleteInstance)
            .method<void*(void*) const, virtual_>("placementNewInstance", &_::placementNewInstance)
            .method<void(void*) const, virtual_>("placementDeleteInstance", &_::placementDeleteInstance)
            .method<::phantom::lang::Type::TypeRelation(Type*) const, virtual_>("getRelationWith", &_::getRelationWith)
            .method<bool(void const*, void const*) const, virtual_>("equal", &_::equal)
            .method<void(StringView, void*) const, virtual_>("valueFromString", &_::valueFromString)
            .method<void(StringBuffer&, const void*) const, virtual_>("valueToString", &_::valueToString)
            .method<void(StringBuffer&, const void*) const, virtual_>("valueToLiteral", &_::valueToLiteral)
            .using_("Symbol::fetchElements")
            .method<void(LanguageElements&, Class*) const, virtual_>("fetchElements", &_::fetchElements)["nullptr"]
            .method<void(void*, void const*) const, virtual_>("copyConstruct", &_::copyConstruct)
            .method<void(void*, void*) const, virtual_>("moveConstruct", &_::moveConstruct)
            .method<void(void*, void const*) const, virtual_>("copyAssign", &_::copyAssign)
            .method<void(void*, void*) const, virtual_>("moveAssign", &_::moveAssign)
            .method<void(void*, void const*)>("copy", &_::copy)
            .method<bool() const, virtual_>("isCopyable", &_::isCopyable)
            .method<bool() const, virtual_>("isCopyConstructible", &_::isCopyConstructible)
            .method<bool() const, virtual_>("isCopyAssignable", &_::isCopyAssignable)
            .method<bool() const, virtual_>("isTriviallyCopyAssignable", &_::isTriviallyCopyAssignable)
            .method<bool() const, virtual_>("isTriviallyCopyConstructible", &_::isTriviallyCopyConstructible)
            .method<bool() const, virtual_>("isMoveable", &_::isMoveable)
            .method<bool() const, virtual_>("isMoveConstructible", &_::isMoveConstructible)
            .method<bool() const, virtual_>("isMoveAssignable", &_::isMoveAssignable)
            .method<bool() const, virtual_>("isTriviallyMoveAssignable", &_::isTriviallyMoveAssignable)
            .method<bool() const, virtual_>("isTriviallyMoveConstructible", &_::isTriviallyMoveConstructible)
            .method<bool() const, virtual_>("hasCopyDisabled", &_::hasCopyDisabled)
            .method<bool() const, virtual_>("hasMoveDisabled", &_::hasMoveDisabled)
            .method<::phantom::lang::Type *() const, virtual_>("unsignedType", &_::unsignedType)
            .method<::phantom::lang::Type *() const, virtual_>("signedType", &_::signedType)
            .method<::phantom::lang::Type *() const, virtual_>("longType", &_::longType)
            .method<bool() const, virtual_>("isSignedInteger", &_::isSignedInteger)
            .method<bool() const, virtual_>("isUnsignedInteger", &_::isUnsignedInteger)
            .method<::phantom::lang::Type *() const, virtual_>("promote", &_::promote)
            .method<bool() const>("isPromotedArithmeticType", &_::isPromotedArithmeticType)
            .method<bool() const>("isPromotedIntegralType", &_::isPromotedIntegralType)
            .method<uint() const, virtual_>("getDataPointerLevel", &_::getDataPointerLevel)
            .method<Scope*() const>("getScope", &_::getScope)
            .method<Type*() const>("getOwnerType", &_::getOwnerType)
            .method<size_t() const>("getExtendedTypeCount", &_::getExtendedTypeCount)
            .method<TypesView() const>("getExtendedTypes", &_::getExtendedTypes)
            .method<::phantom::lang::Type *(Type*) const, virtual_>("getCommonBaseAncestor", &_::getCommonBaseAncestor)
            .method<Pointer*() const>("getPointer", &_::getPointer)
            .method<LValueReference*() const>("getLValueReference", &_::getLValueReference)
            .method<RValueReference*() const>("getRValueReference", &_::getRValueReference)
            .method<Array*(size_t) const>("getArray", &_::getArray)
            .method<ConstType*() const>("getConstType", &_::getConstType)
            .method<VolatileType*() const>("getVolatileType", &_::getVolatileType)
            .method<ConstVolatileType*() const>("getConstVolatileType", &_::getConstVolatileType)
            .method<Type*(size_t) const>("makePointer", &_::makePointer)
            .method<Type*() const>("makePointer", &_::makePointer)
            .method<LValueReference*() const>("makeLValueReference", &_::makeLValueReference)
            .method<RValueReference*() const>("makeRValueReference", &_::makeRValueReference)
            .method<Array*(size_t) const>("makeArray", &_::makeArray)
            .method<ConstType*() const>("makeConst", &_::makeConst)
            .method<VolatileType*() const>("makeVolatile", &_::makeVolatile)
            .method<ConstVolatileType*() const>("makeConstVolatile", &_::makeConstVolatile)
            .method<Template*() const>("getTemplate", &_::getTemplate)
            .method<ptrdiff_t(Type*) const, virtual_>("getPointerAdjustmentOffset", &_::getPointerAdjustmentOffset)
            .method<bool(LanguageElement*, size_t&, PlaceholderMap&) const, virtual_|override_>("partialAccepts", &_::partialAccepts)
            .method<bool(Type*, size_t&, PlaceholderMap&) const, virtual_>("partialAccepts", &_::partialAccepts)
        
        .protected_()
            .method<void(Type*)>("setUnderlyingType", &_::setUnderlyingType)
            .method<void(uint16_t, uint16_t)>("setSizeAndAlignemnt", &_::setSizeAndAlignemnt)
            .method<void(Visibility), virtual_|override_>("onVisibilityChanging", &_::onVisibilityChanging)
            .method<void(Visibility), virtual_|override_>("onVisibilityChanged", &_::onVisibilityChanged)
        
        .public_()
            .field("kindCreated", &_::kindCreated)
            .field("kindDestroying", &_::kindDestroying)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Type")
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
