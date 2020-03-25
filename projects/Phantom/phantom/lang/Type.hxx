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
            using RecursiveSpinMutex = typedef_< phantom::RecursiveSpinMutex>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Types = typedef_< phantom::lang::Types>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Symbol>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            /// missing symbol(s) reflection (phantom::Delegate) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .typedef_<TypeFilter>("TypeFilter")
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
            .method<TypeKind() const>("getTypeKind", &_::getTypeKind)
            .method<size_t() const>("getSize", &_::getSize)
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
            .method<Type*() const, virtual_|final_|override_>("asAddressType", &_::asAddressType)
            .method<bool() const>("isValuePointerKind", &_::isValuePointerKind)
            .method<uint8_t()>("getAddressLevel", &_::getAddressLevel)
            .method<ClassType*() const, virtual_|final_|override_>("asClassType", &_::asClassType)
            .method<Array*() const, virtual_|final_|override_>("asArray", &_::asArray)
            .method<Enum*() const, virtual_|final_|override_>("asEnum", &_::asEnum)
            .method<LValueReference*() const, virtual_|final_|override_>("asLValueReference", &_::asLValueReference)
            .method<RValueReference*() const, virtual_|final_|override_>("asRValueReference", &_::asRValueReference)
            .method<Reference*() const, virtual_|final_|override_>("asReference", &_::asReference)
            .method<Pointer*() const, virtual_|final_|override_>("asPointer", &_::asPointer)
            .method<ConstType*() const, virtual_|final_|override_>("asConstType", &_::asConstType)
            .method<VolatileType*() const, virtual_|final_|override_>("asVolatileType", &_::asVolatileType)
            .method<ConstVolatileType*() const, virtual_|final_|override_>("asConstVolatileType", &_::asConstVolatileType)
            .method<PointerType*() const, virtual_|final_|override_>("asPointerType", &_::asPointerType)
            .method<Class*() const, virtual_|final_|override_>("asClass", &_::asClass)
            .method<VectorClass*() const, virtual_|final_|override_>("asVectorClass", &_::asVectorClass)
            .method<MapClass*() const, virtual_|final_|override_>("asMapClass", &_::asMapClass)
            .method<SetClass*() const, virtual_|final_|override_>("asSetClass", &_::asSetClass)
            .method<StringClass*() const, virtual_|final_|override_>("asStringClass", &_::asStringClass)
            /// missing symbol(s) reflection (phantom::lang::ArrayClass) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<ArrayClass*() const, virtual_|final_|override_>("asArrayClass", &_::asArrayClass)
            .method<LanguageElement*() const, virtual_>("asTemplateElement", &_::asTemplateElement)
            .method<LanguageElement*() const, virtual_>("asLanguageElement", &_::asLanguageElement)
            .method<bool() const, virtual_>("isPolymorphic", &_::isPolymorphic)
            .method<bool(Type*) const, virtual_>("hasStrongDependencyOnType", &_::hasStrongDependencyOnType)
            .method<Type*() const>("getUnderlyingType", &_::getUnderlyingType)
            .method<Type*() const>("addConst", &_::addConst)
            .method<Type*() const>("addVolatile", &_::addVolatile)
            .method<Type*() const>("addConstLValueReference", &_::addConstLValueReference)
            .method<Type*() const>("addConstRValueReference", &_::addConstRValueReference)
            .method<Type*() const>("addConstVolatile", &_::addConstVolatile)
            .method<Type*(size_t) const>("addArray", &_::addArray)
            .method<Type*() const>("addLValueReference", &_::addLValueReference)
            .method<Type*() const>("addRValueReference", &_::addRValueReference)
            .method<Type*() const>("addPointer", &_::addPointer)
            .method<Type*(int) const, virtual_>("addCustom", &_::addCustom)
            .method<Type*() const>("removeConst", &_::removeConst)
            .method<Type*() const>("removeVolatile", &_::removeVolatile)
            .method<Type*() const>("removeConstVolatile", &_::removeConstVolatile)
            .method<Type*() const>("removeQualifiers", &_::removeQualifiers)
            .method<Type*() const>("removeConstReference", &_::removeConstReference)
            .method<Type*() const>("removeConstLValueReference", &_::removeConstLValueReference)
            .method<Type*() const>("removePointerOrArray", &_::removePointerOrArray)
            .method<Type*() const>("removeAddress", &_::removeAddress)
            .method<Type*() const>("removeArray", &_::removeArray)
            .method<Type*() const>("removeExtent", &_::removeExtent)
            .method<Type*() const>("removeArrays", &_::removeArrays)
            .method<Type*() const>("removeAllExtents", &_::removeAllExtents)
            .method<Type*() const>("removeReference", &_::removeReference)
            .method<Type*() const>("removeLValueReference", &_::removeLValueReference)
            .method<Type*() const>("removeRValueReference", &_::removeRValueReference)
            .method<Type*() const>("removePointer", &_::removePointer)
            .method<Type*() const>("removePointerType", &_::removePointerType)
            .method<Type*() const>("removeAllConst", &_::removeAllConst)
            .method<Type*() const>("removeAllVolatile", &_::removeAllVolatile)
            .method<Type*() const>("removeAllConstVolatile", &_::removeAllConstVolatile)
            .method<Type*() const>("removeAllQualifiers", &_::removeAllQualifiers)
            .method<Type*() const>("removeEverything", &_::removeEverything)
            .method<Type*(int) const, virtual_>("removeCustom", &_::removeCustom)
            .method<Type*(Type*) const, virtual_>("replicate", &_::replicate)
            .method<bool(Type*, void*, void const*) const, virtual_>("convert", &_::convert)
            .method<void*(Type*, void*) const, virtual_>("cast", &_::cast)
            .method<void const*(Type*, void const*) const, virtual_>("cast", &_::cast)
            .method<void*(Type*, void*) const, virtual_>("upcast", &_::upcast)
            .method<void*(Type*, void*) const, virtual_>("downcast", &_::downcast)
            .method<void*() const, virtual_>("allocate", &_::allocate)
            .method<void(void*) const, virtual_>("deallocate", &_::deallocate)
            .method<void*(size_t) const, virtual_>("allocate", &_::allocate)
            .method<void(void*, size_t) const, virtual_>("deallocate", &_::deallocate)
            .method<void(void*) const, virtual_>("construct", &_::construct)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<::phantom::lang::ScopedConstruction<ScopeExit<(lambda at C:/Development/Phantom/projects/phantom/phantom/lang/Type.h:1082:53)> >(void*) const>("scopedConstruct", &_::scopedConstruct)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<::phantom::lang::ScopedConstruction<ScopeExit<(lambda at C:/Development/Phantom/projects/phantom/phantom/lang/Type.h:1082:53)> >(void*) const>("localConstruct", &_::localConstruct)
            .method<void(void*) const, virtual_>("destroy", &_::destroy)
            .method<uint64_t(const void*) const, virtual_>("hash", &_::hash)
            .method<void*() const, virtual_>("newInstance", &_::newInstance)
            .method<void(void*) const, virtual_>("deleteInstance", &_::deleteInstance)
            .method<void*(void*) const, virtual_>("placementNewInstance", &_::placementNewInstance)
            .method<void(void*) const, virtual_>("placementDeleteInstance", &_::placementDeleteInstance)
            .method<TypeRelation(Type*) const, virtual_>("getRelationWith", &_::getRelationWith)
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
            .method<Type*() const, virtual_>("unsignedType", &_::unsignedType)
            .method<Type*() const, virtual_>("signedType", &_::signedType)
            .method<Type*() const, virtual_>("longType", &_::longType)
            .method<bool() const, virtual_>("isSignedInteger", &_::isSignedInteger)
            .method<bool() const, virtual_>("isUnsignedInteger", &_::isUnsignedInteger)
            .method<Type*() const, virtual_>("promote", &_::promote)
            .method<bool() const>("isPromotedArithmeticType", &_::isPromotedArithmeticType)
            .method<bool() const>("isPromotedIntegralType", &_::isPromotedIntegralType)
            .method<uint() const, virtual_>("getDataPointerLevel", &_::getDataPointerLevel)
            .method<Scope*() const>("getScope", &_::getScope)
            .method<void()>("removeFromScope", &_::removeFromScope)
            .method<Type*() const>("getOwnerType", &_::getOwnerType)
            .method<size_t() const>("getExtendedTypeCount", &_::getExtendedTypeCount)
            .method<Type*(size_t) const>("getExtendedType", &_::getExtendedType)
            .method<Type*(Type*) const, virtual_>("getCommonBaseAncestor", &_::getCommonBaseAncestor)
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
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
            .method<Type*() const>("createPointer", &_::createPointer)
            .method<LValueReference*() const>("createLValueReference", &_::createLValueReference)
            .method<RValueReference*() const>("createRValueReference", &_::createRValueReference)
            .method<Array*(size_t) const>("createArray", &_::createArray)
            .method<ConstType*() const>("createConstType", &_::createConstType)
            .method<VolatileType*() const>("createVolatileType", &_::createVolatileType)
            .method<ConstVolatileType*() const>("createConstVolatileType", &_::createConstVolatileType)
            .method<void(Type*)>("removeExtendedType", &_::removeExtendedType)
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<void(LanguageElement*), virtual_|override_>("onAncestorChanged", &_::onAncestorChanged)
            .method<void(LanguageElement*), virtual_|override_>("onAncestorAboutToBeChanged", &_::onAncestorAboutToBeChanged)
        
        .public_()
            .field("kindCreated", &_::kindCreated)
            .field("kindDestroying", &_::kindDestroying)
        
        .protected_()
            .field("m_pUnderlyingType", &_::m_pUnderlyingType)
            .field("m_pExtendedTypes", &_::m_pExtendedTypes)
            .field("m_uiSize", &_::m_uiSize)
            .field("m_uiAlignment", &_::m_uiAlignment)
            .field("m_eTypeKind", &_::m_eTypeKind)
            .field("m_ExtendedTypesMutex", &_::m_ExtendedTypesMutex)
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
