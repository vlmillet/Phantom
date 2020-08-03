#pragma once

// haunt {

// clang-format off

#include "Class.h"

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
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Class")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(BaseClass)
        {
            this_()
            .constructor<void()>()
            .constructor<void(Class*, size_t, Access)>()["Access::Public"]
            .field("access", &_::access)
            .field("baseClass", &_::baseClass)
            .field("offset", &_::offset)
            .method<::phantom::lang::Class *() const>("operator ::phantom::lang::Class *", &_::operator notypedef<::phantom::lang::Class *>)
            ;
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<BaseClasses>("BaseClasses"); }
        PHANTOM_STRUCT(ClassBuilder)
        {
            using StringView = typedef_< phantom::StringView>;
            this_()
        .public_()
            .staticMethod<::phantom::lang::ClassBuilder(StringView, size_t)>("struct_", &_::struct_)["0"]
            .staticMethod<::phantom::lang::ClassBuilder(StringView, size_t)>("class_", &_::class_)["0"]
            .staticMethod<::phantom::lang::ClassBuilder(Scope*, StringView, size_t)>("struct_", &_::struct_)["0"]
            .staticMethod<::phantom::lang::ClassBuilder(Scope*, StringView, size_t)>("class_", &_::class_)["0"]
            .staticMethod<::phantom::lang::ClassBuilder(Scope*, Scope*, StringView, size_t)>("struct_", &_::struct_)["0"]
            .staticMethod<::phantom::lang::ClassBuilder(Scope*, Scope*, StringView, size_t)>("class_", &_::class_)["0"]
        
        .public_()
            .constructor<void(Scope*, Scope*, StringView, Access, size_t)>()["0"]
            .method<Scope*() const>("scope", &_::scope)
            .method<ClassBuilder&(Class*)>("inherits", &_::inherits)
            .method<ClassBuilder&(Type*, StringView, size_t, uint)>("field", &_::field)["0"]["~0u"]
            .method<ClassBuilder&(Access)>("access", &_::access)
            .method<Class*()>("finalize", &_::finalize)
            .method<::phantom::lang::Class *()>("operator ::phantom::lang::Class *", &_::operator notypedef<::phantom::lang::Class *>)
            ;
        }
        PHANTOM_CLASS(Class)
        {
            using AggregateFields = typedef_< phantom::lang::AggregateFields>;
            using Classes = typedef_< phantom::lang::Classes>;
            using ExtraData = typedef_<_::ExtraData>;
            using Methods = typedef_< phantom::lang::Methods>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            using Strings = typedef_< phantom::Strings>;
            using Subroutines = typedef_< phantom::lang::Subroutines>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            using ValueMembers = typedef_< phantom::lang::ValueMembers>;
            using Variants = typedef_< phantom::Variants>;
            using VirtualMethodTables = typedef_< phantom::lang::VirtualMethodTables>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::ClassType>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
        
        .public_()
            .struct_<ExtraData>()
                .inherits<::phantom::lang::ClassType::ExtraData>()
                .constructor<void()>()
                .field("m_uiFieldMemoryOffset", &_::ExtraData::m_uiFieldMemoryOffset)
                .field("m_uiClassPtrOffset", &_::ExtraData::m_uiClassPtrOffset)
                .field("m_uiStateMachineDataPtrOffset", &_::ExtraData::m_uiStateMachineDataPtrOffset)
                .field("m_bHasVTablePtr", &_::ExtraData::m_bHasVTablePtr)
            .end()
        
        .public_()
            .constructor<void()>()
            .constructor<void(StringView, Modifiers, uint)>()["0"]["0"]
        
        .protected_()
            .constructor<void(TypeKind, StringView, Modifiers, uint)>()["0"]["0"]
            .constructor<void(TypeKind, StringView, size_t, size_t, Modifiers, uint)>()
            .constructor<void(StringView, size_t, size_t, Modifiers, uint)>()
        
        .public_()
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<Class*(Class*) const>("getCommonBaseClass", &_::getCommonBaseClass)
            .method<void(Methods&) const>("filtersNonOverloadedPureVirtualMethodsCascade", &_::filtersNonOverloadedPureVirtualMethodsCascade)
            .method<void(Methods&) const>("findPureVirtualMethods", &_::findPureVirtualMethods)
            .method<Subroutine*(StringView) const>("getSubroutineCascade", &_::getSubroutineCascade)
            .method<Subroutine*(StringView, TypesView, Modifiers) const>("getSubroutineCascade", &_::getSubroutineCascade)["0"]
            .method<void(ValueMembers&) const>("getValueMembersCascade", &_::getValueMembersCascade)
            .method<void(Method*, Methods&)>("findOverriddenMethods", &_::findOverriddenMethods)
            .method<void(StringView, Signature*, Methods&)>("findOverriddenMethods", &_::findOverriddenMethods)
            .method<void(VirtualMethodTable*, Methods*, Methods*) const>("sortBaseAndRootMethods", &_::sortBaseAndRootMethods)
            .method<Method*(StringView) const>("getMethodCascade", &_::getMethodCascade)
            .method<Method*(StringView, TypesView, Modifiers) const>("getMethodCascade", &_::getMethodCascade)["0"]
            .method<Method*(StringView, Type*, Modifiers) const>("getMethodCascade", &_::getMethodCascade)["0"]
            .method<ValueMember*(StringView) const>("getValueMemberCascade", &_::getValueMemberCascade)
            .method<void(Type*, ValueMembers&) const>("findPublicValueMembersPointingValueTypeCascade", &_::findPublicValueMembersPointingValueTypeCascade)
            .method<Field*(StringView) const>("getFieldCascade", &_::getFieldCascade)
            .method<Field*(size_t) const, virtual_|override_>("getFieldByOffset", &_::getFieldByOffset)
            .method<Field*(size_t) const, virtual_|override_>("getFieldAtOffset", &_::getFieldAtOffset)
            .using_("ClassType::getFields")
            .method<void(AggregateFields&) const, virtual_|override_>("getFields", &_::getFields)
            .method<Property*(StringView) const>("getPropertyCascade", &_::getPropertyCascade)
            .method<StaticMethod*(StringView, TypesView) const>("getStaticMethodCascade", &_::getStaticMethodCascade)
            .method<StaticMethod*(StringView) const>("getStaticMethodCascade", &_::getStaticMethodCascade)
            .method<StaticField*(StringView) const>("getStaticFieldCascade", &_::getStaticFieldCascade)
            .method<size_t(size_t) const>("getVirtualMethodTableSize", &_::getVirtualMethodTableSize)
            .method<bool(Type*, StringView, TypesView, Modifiers, uint, Subroutines*) const, virtual_|override_>("acceptsSubroutine", &_::acceptsSubroutine)["NULL"]
            .method<bool(Type*, StringView, TypesView, Modifiers, uint, Subroutines*) const, virtual_>("acceptsDerivedClassSubroutine", &_::acceptsDerivedClassSubroutine)
            .method<::phantom::lang::Class *(size_t) const>("getBaseClass", &_::getBaseClass)
            .method<void*(void*, size_t) const>("baseClassCast", &_::baseClassCast)
            .method<const void*(const void*, size_t) const>("baseClassCast", &_::baseClassCast)
            .method<size_t(size_t) const>("getInheritanceLevel", &_::getInheritanceLevel)
            .method<size_t(Class*) const>("getInheritanceLevelFromBase", &_::getInheritanceLevelFromBase)
            .method<bool(Symbol*) const, virtual_|override_>("hasPublicMember", &_::hasPublicMember)
            .method<bool(Symbol*) const, virtual_|override_>("hasProtectedMember", &_::hasProtectedMember)
            .method<bool(Symbol*) const, virtual_|override_>("hasPrivateMember", &_::hasPrivateMember)
            .method<bool() const, virtual_|override_>("isListInitializable", &_::isListInitializable)
            .method<size_t(size_t) const, virtual_>("getVirtualMethodCount", &_::getVirtualMethodCount)
            .method<VirtualMethodTables const&() const>("getVirtualMethodTables", &_::getVirtualMethodTables)
            .method<void(Class*, Access), virtual_>("addBaseClass", &_::addBaseClass)["Access::Public"]
            .method<void(Class*, size_t, Access), virtual_>("addBaseClass", &_::addBaseClass)["Access::Public"]
            .method<const BaseClasses&() const>("getBaseClasses", &_::getBaseClasses)
            .method<Access(Class*) const>("getBaseClassAccess", &_::getBaseClassAccess)
            .method<Access(Class*) const>("getBaseClassAccessCascade", &_::getBaseClassAccessCascade)
            .method<bool(Class*, Access*) const>("hasBaseClass", &_::hasBaseClass)["nullptr"]
            .method<bool(Class*) const>("hasBaseClassCascade", &_::hasBaseClassCascade)
            .method<Class*(size_t) const>("getDerivedClass", &_::getDerivedClass)
            .method<Classes const&() const>("getDerivedClasses", &_::getDerivedClasses)
            .method<void(Classes&) const>("getDerivedClassesCascade", &_::getDerivedClassesCascade)
            .method<Classes const&() const>("getDerivedClassesCascade", &_::getDerivedClassesCascade)
            .method<bool(Class*, Access*) const>("hasDerivedClass", &_::hasDerivedClass)["nullptr"]
            .method<bool(Class*, Access*) const>("hasDerivedClassCascade", &_::hasDerivedClassCascade)["nullptr"]
            .method<bool(LanguageElement*) const>("hasMemberCascade", &_::hasMemberCascade)
            .method<ptrdiff_t(size_t) const>("getBaseClassOffset", &_::getBaseClassOffset)
            .method<void(Class*, ptrdiff_t)>("setBaseClassOffset", &_::setBaseClassOffset)
            .method<ptrdiff_t(Class*) const>("getBaseClassOffset", &_::getBaseClassOffset)
            .method<ptrdiff_t(Class*) const>("getBaseClassOffsetCascade", &_::getBaseClassOffsetCascade)
            .method<ptrdiff_t(Type*) const, virtual_|override_>("getPointerAdjustmentOffset", &_::getPointerAdjustmentOffset)
            .method<ptrdiff_t(Class*) const>("getPointerAdjustmentOffset", &_::getPointerAdjustmentOffset)
            .method<bool() const>("isRootClass", &_::isRootClass)
            .method<bool(Class*) const>("isA", &_::isA)
            .method<bool(Type*) const, virtual_|override_>("isA", &_::isA)
            .method<TypeRelation(Type*) const, virtual_|override_>("getRelationWith", &_::getRelationWith)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(ValueMember*, Expression*)>("setOverriddenDefaultExpression", &_::setOverriddenDefaultExpression)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*(ValueMember*) const>("getOverriddenDefaultExpression", &_::getOverriddenDefaultExpression)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*(ValueMember*) const>("getOverriddenDefaultExpressionCascade", &_::getOverriddenDefaultExpressionCascade)
            .method<bool() const, virtual_|override_>("isPolymorphic", &_::isPolymorphic)
            .using_("Type::allocate")
            .using_("Type::deallocate")
            .method<void*() const, virtual_|override_>("allocate", &_::allocate)
            .method<void(void*) const, virtual_|override_>("deallocate", &_::deallocate)
            .method<void*(size_t) const, virtual_|override_>("allocate", &_::allocate)
            .method<void(void*, size_t) const, virtual_|override_>("deallocate", &_::deallocate)
            .method<void(void*) const, virtual_|override_>("construct", &_::construct)
            .method<void(void*) const, virtual_|override_>("destroy", &_::destroy)
            .method<void*() const>("getSingleton", &_::getSingleton)
            .method<size_t() const>("getInstanceCount", &_::getInstanceCount)
            .method<void*(Class*, void*) const>("upcast", &_::upcast)
            .method<void const*(Class*, void const*) const>("upcast", &_::upcast)
            .method<void*(Class*, void*) const>("downcast", &_::downcast)
            .method<void const*(Class*, void const*) const>("downcast", &_::downcast)
            .method<void*(Class*, void*) const>("cast", &_::cast)
            .method<void const*(Class*, void const*) const>("cast", &_::cast)
            .method<void*(Type*, void*) const, virtual_|override_>("upcast", &_::upcast)
            .method<void*(Type*, void*) const, virtual_|override_>("downcast", &_::downcast)
            .method<void*(Type*, void*) const, virtual_|final_>("cast", &_::cast)
            .method<void const*(Type*, void const*) const, virtual_|final_>("cast", &_::cast)
            .method<void*() const, virtual_|override_>("newInstance", &_::newInstance)
            .method<void(void*) const, virtual_|override_>("deleteInstance", &_::deleteInstance)
            .method<void*(void*) const, virtual_|override_>("placementNewInstance", &_::placementNewInstance)
            .method<void*(void*, Constructor*, void**) const, virtual_|override_>("placementNewInstance", &_::placementNewInstance)
            .method<void(void*) const, virtual_|override_>("placementDeleteInstance", &_::placementDeleteInstance)
            .method<const ::phantom::Variant &(StringView) const>("getMetaDataIncludingBases", &_::getMetaDataIncludingBases)
            .method<const ::phantom::Variant &(StringWithHash) const>("getMetaDataIncludingBases", &_::getMetaDataIncludingBases)
            .method<void(StringView, Variants&) const>("getMetaDatasIncludingBases", &_::getMetaDatasIncludingBases)
            .method<void(StringWithHash, Variants&) const>("getMetaDatasIncludingBases", &_::getMetaDatasIncludingBases)
            .method<bool(StringWithHash) const>("hasMetaDataIncludingBases", &_::hasMetaDataIncludingBases)
            .method<bool(StringView) const>("hasMetaDataIncludingBases", &_::hasMetaDataIncludingBases)
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<bool() const, virtual_|override_>("isCopyConstructible", &_::isCopyConstructible)
            .method<bool() const, virtual_|override_>("isMoveConstructible", &_::isMoveConstructible)
            .method<void(void*, void const*) const, virtual_|override_>("copyConstruct", &_::copyConstruct)
            .method<bool(void const*, void const*) const, virtual_|override_>("equal", &_::equal)
            .method<bool() const, virtual_>("hasNewVTable", &_::hasNewVTable)
            .method<bool(Type*) const, virtual_|override_>("hasStrongDependencyOnType", &_::hasStrongDependencyOnType)
            .method<ExtraData*() const>("getExtraData", &_::getExtraData)
            .method<void()>("addNewVirtualMethodTable", &_::addNewVirtualMethodTable)
            .method<void(), virtual_|override_>("addImplicitDefaultConstructor", &_::addImplicitDefaultConstructor)
            .method<void(), virtual_|override_>("addImplicitCopyConstructor", &_::addImplicitCopyConstructor)
            .method<void(), virtual_|override_>("addImplicitCopyAssignmentOperator", &_::addImplicitCopyAssignmentOperator)
            .method<void(), virtual_|override_>("addImplicitMoveConstructor", &_::addImplicitMoveConstructor)
            .method<void(), virtual_|override_>("addImplicitMoveAssignmentOperator", &_::addImplicitMoveAssignmentOperator)
            .method<void(), virtual_|override_>("addImplicitDestructor", &_::addImplicitDestructor)
            .method<bool() const, virtual_|override_>("canHaveImplicitDefaultConstructor", &_::canHaveImplicitDefaultConstructor)
            .method<bool() const, virtual_|override_>("canHaveImplicitCopyConstructor", &_::canHaveImplicitCopyConstructor)
            .method<bool() const, virtual_|override_>("canHaveImplicitCopyAssignmentOperator", &_::canHaveImplicitCopyAssignmentOperator)
            .method<bool() const, virtual_|override_>("canHaveImplicitMoveConstructor", &_::canHaveImplicitMoveConstructor)
            .method<bool() const, virtual_|override_>("canHaveImplicitMoveAssignmentOperator", &_::canHaveImplicitMoveAssignmentOperator)
            .staticMethod<::phantom::lang::VirtualMethodTable *(Class*, void**, size_t)>("CreateVirtualMethodTable", &_::CreateVirtualMethodTable)
            .staticMethod<::phantom::lang::VirtualMethodTable *(Class*, VirtualMethodTable*, size_t)>("DeriveVirtualMethodTable", &_::DeriveVirtualMethodTable)
            .staticMethod<::phantom::lang::VirtualMethodTable *(Class*, VirtualMethodTable*)>("DeriveVirtualMethodTable", &_::DeriveVirtualMethodTable)
            .staticMethod<::phantom::lang::Class *(VirtualMethodTable*)>("VTablePrimaryClass", &_::VTablePrimaryClass)
            .method<::phantom::lang::VirtualMethodTable *(), virtual_>("createVirtualMethodTable", &_::createVirtualMethodTable)
            .method<::phantom::lang::VirtualMethodTable *(VirtualMethodTable*), virtual_>("deriveVirtualMethodTable", &_::deriveVirtualMethodTable)
            .method<SymbolExtension*(Class*, size_t) const>("getExtensionCascade", &_::getExtensionCascade)["0"]
            .method<void(void*), virtual_>("registerInstance", &_::registerInstance)
            .method<void(void*), virtual_>("unregisterInstance", &_::unregisterInstance)
            .method<InstanceCache*()>("getOrCreateInstanceCache", &_::getOrCreateInstanceCache)
        
        .protected_()
            .method<bool() const, virtual_|override_>("canBeDestroyed", &_::canBeDestroyed)
            .method<Strings() const>("getBaseClasseNames", &_::getBaseClasseNames)
            .method<void(Strings), virtual_>("setBaseClasseNames", &_::setBaseClasseNames)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Class")
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
