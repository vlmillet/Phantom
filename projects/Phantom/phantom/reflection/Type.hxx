#pragma once

// haunt {

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

#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
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
            using AggregateFields = typedef_< phantom::reflection::AggregateFields>;
            using AlignmentComputer = typedef_<_::AlignmentComputer>;
            using DataElements = typedef_< phantom::reflection::DataElements>;
            using LanguageElements = typedef_< phantom::reflection::LanguageElements>;
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using RecursiveSpinMutex = typedef_< phantom::RecursiveSpinMutex>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Types = typedef_< phantom::reflection::Types>;
            this_()
            .inherits<::phantom::reflection::Symbol>()
        .public_()
            .method<void(::phantom::reflection::LanguageElementVisitor *, ::phantom::reflection::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
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
            .constructor<void(TypeKind, Modifiers, uint)>()["0"]["0"]
            .constructor<void(TypeKind, StringView, Modifiers, uint)>()["0"]["0"]
            .constructor<void(TypeKind, StringView, size_t, size_t, Modifiers, uint)>()
        
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
            .method<Type*() const, virtual_|override_>("asType", &_::asType)
            .method<LanguageElement*() const, virtual_>("asTemplateElement", &_::asTemplateElement)
            .method<LanguageElement*() const, virtual_>("asLanguageElement", &_::asLanguageElement)
            .method<bool() const, virtual_>("isPolymorphic", &_::isPolymorphic)
            .method<bool(Type*) const, virtual_>("hasStrongDependencyOnType", &_::hasStrongDependencyOnType)
            .method<Type*() const, virtual_>("getUnderlyingType", &_::getUnderlyingType)
            .method<Type*() const, virtual_>("addConst", &_::addConst)
            .method<Type*() const, virtual_>("addVolatile", &_::addVolatile)
            .method<Type*() const>("addConstLValueReference", &_::addConstLValueReference)
            .method<Type*() const>("addConstRValueReference", &_::addConstRValueReference)
            .method<Type*() const>("addConstVolatile", &_::addConstVolatile)
            .method<Type*(size_t) const, virtual_>("addArray", &_::addArray)
            .method<Type*() const, virtual_>("addLValueReference", &_::addLValueReference)
            .method<Type*() const, virtual_>("addRValueReference", &_::addRValueReference)
            .method<Type*() const, virtual_>("addPointer", &_::addPointer)
            .method<Type*(int) const, virtual_>("addCustom", &_::addCustom)
            .method<Type*() const, virtual_>("removeConst", &_::removeConst)
            .method<Type*() const, virtual_>("removeVolatile", &_::removeVolatile)
            .method<Type*() const, virtual_>("removeConstVolatile", &_::removeConstVolatile)
            .method<Type*() const, virtual_>("removeQualifiers", &_::removeQualifiers)
            .method<Type*() const>("removeConstReference", &_::removeConstReference)
            .method<Type*() const>("removeConstLValueReference", &_::removeConstLValueReference)
            .method<Type*() const, virtual_>("removePointerOrArray", &_::removePointerOrArray)
            .method<Type*() const, virtual_>("removeAddress", &_::removeAddress)
            .method<Type*() const, virtual_>("removeArray", &_::removeArray)
            .method<Type*() const, virtual_>("removeReference", &_::removeReference)
            .method<Type*() const, virtual_>("removeEverything", &_::removeEverything)
            .method<Type*() const, virtual_>("removeLValueReference", &_::removeLValueReference)
            .method<Type*() const, virtual_>("removeRValueReference", &_::removeRValueReference)
            .method<Type*() const, virtual_>("removePointer", &_::removePointer)
            .method<Type*() const, virtual_>("removeAllConst", &_::removeAllConst)
            .method<Type*() const, virtual_>("removeAllVolatile", &_::removeAllVolatile)
            .method<Type*() const, virtual_>("removeAllConstVolatile", &_::removeAllConstVolatile)
            .method<Type*() const, virtual_>("removeAllQualifiers", &_::removeAllQualifiers)
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
            // .method<void(void*) const>("scopedConstruct", &_::scopedConstruct)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<void(void*) const>("localConstruct", &_::localConstruct)
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
            .method<Pointer*() const>("makePointer", &_::makePointer)
            .method<LValueReference*() const>("makeLValueReference", &_::makeLValueReference)
            .method<RValueReference*() const>("makeRValueReference", &_::makeRValueReference)
            .method<Array*(size_t) const>("makeArray", &_::makeArray)
            .method<ConstType*() const>("makeConst", &_::makeConst)
            .method<VolatileType*() const>("makeVolatile", &_::makeVolatile)
            .method<ConstVolatileType*() const>("makeConstVolatile", &_::makeConstVolatile)
            .method<Template*() const>("getTemplate", &_::getTemplate)
            .method<ptrdiff_t(Type*) const, virtual_>("getPointerAdjustmentOffset", &_::getPointerAdjustmentOffset)
            /// missing symbol(s) reflection (phantom::reflection::PlaceholderMap) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<bool(LanguageElement*, size_t&, PlaceholderMap&) const, virtual_|override_>("partialAccepts", &_::partialAccepts)
            /// missing symbol(s) reflection (phantom::reflection::PlaceholderMap) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<bool(Type*, size_t&, PlaceholderMap&) const, virtual_>("partialAccepts", &_::partialAccepts)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
            .method<Pointer*() const, virtual_>("createPointer", &_::createPointer)
            .method<LValueReference*() const, virtual_>("createLValueReference", &_::createLValueReference)
            .method<RValueReference*() const, virtual_>("createRValueReference", &_::createRValueReference)
            .method<Array*(size_t) const, virtual_>("createArray", &_::createArray)
            .method<ConstType*() const, virtual_>("createConstType", &_::createConstType)
            .method<VolatileType*() const, virtual_>("createVolatileType", &_::createVolatileType)
            .method<ConstVolatileType*() const, virtual_>("createConstVolatileType", &_::createConstVolatileType)
            .method<void(Type*)>("removeExtendedType", &_::removeExtendedType)
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<void(LanguageElement*), virtual_|override_>("onAncestorChanged", &_::onAncestorChanged)
            .method<void(LanguageElement*), virtual_|override_>("onAncestorAboutToBeChanged", &_::onAncestorAboutToBeChanged)
        
        .public_()
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .field("kindCreated", &_::kindCreated)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .field("kindDestroyed", &_::kindDestroyed)
        
        .protected_()
            .field("m_eTypeKind", &_::m_eTypeKind)
            .field("m_pExtendedTypes", &_::m_pExtendedTypes)
            .field("m_uiSize", &_::m_uiSize)
            .field("m_uiAlignment", &_::m_uiAlignment)
            .field("m_ExtendedTypesMutex", &_::m_ExtendedTypesMutex)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Type")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
