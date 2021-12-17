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
            .constructor<void(Class*, size_t, Access)>()({"s","o","a"})["Access::Public"]
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
            .staticMethod<::phantom::lang::ClassBuilder(StringView, size_t)>("struct_", &_::struct_)({"a_Name","a_MinAlign"})["0"]
            .staticMethod<::phantom::lang::ClassBuilder(StringView, size_t)>("class_", &_::class_)({"a_Name","a_MinAlign"})["0"]
            .staticMethod<::phantom::lang::ClassBuilder(Scope*, StringView, size_t)>("struct_", &_::struct_)({"a_pOwnerScope","a_Name","a_MinAlign"})["0"]
            .staticMethod<::phantom::lang::ClassBuilder(Scope*, StringView, size_t)>("class_", &_::class_)({"a_pOwnerScope","a_Name","a_MinAlign"})["0"]
            .staticMethod<::phantom::lang::ClassBuilder(Scope*, Scope*, StringView, size_t)>("struct_", &_::struct_)({"a_pOwnerScope","a_pNamingScope","a_Name","a_MinAlign"})["0"]
            .staticMethod<::phantom::lang::ClassBuilder(Scope*, Scope*, StringView, size_t)>("class_", &_::class_)({"a_pOwnerScope","a_pNamingScope","a_Name","a_MinAlign"})["0"]
        
        .public_()
            .constructor<void(Scope*, Scope*, StringView, Access, size_t)>()({"a_pOwnerScope","a_pNamingScope","a_Name","a_Access","a_MinAlign"})["0"]
            .method<Scope*() const>("scope", &_::scope)
            .method<ClassBuilder&(Class*)>("inherits", &_::inherits)({"_class"})
            .method<ClassBuilder&(Type*, StringView, size_t, uint, StringView)>("field", &_::field)({"a_pType","a_Name","a_Align","a_FilterMask","a_DefaultValue"})["0"]["~0u"]["{}"]
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<ClassBuilder&(Type*, StringView, size_t, uint, Expression*)>("field", &_::field)({"a_pType","a_Name","a_Align","a_FilterMask","a_pDefaultExp"})
            .method<ClassBuilder&(Access)>("access", &_::access)({"a_Access"})
            .method<Class*() const>("forwardDecl", &_::forwardDecl)
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
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
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
            .constructor<void(StringView, Modifiers, uint)>()({"a_strName","a_Modifiers","a_uiFlags"})["0"]["0"]
        
        .protected_()
            .constructor<void(TypeKind, StringView, Modifiers, uint)>()({"a_eTypeKind","a_strName","a_Modifiers","a_uiFlags"})["0"]["0"]
            .constructor<void(TypeKind, StringView, size_t, size_t, Modifiers, uint)>()({"a_eTypeKind","a_strName","a_uiSize","a_uiAlignment","a_Modifiers","a_uiFlags"})
            .constructor<void(StringView, size_t, size_t, Modifiers, uint)>()({"a_strName","a_uiSize","a_uiAlignment","a_Modifiers","a_uiFlags"})
        
        .public_()
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<Class*(Class*) const>("getCommonBaseClass", &_::getCommonBaseClass)({"a_pClass"})
            .method<void(Methods&) const>("filtersNonOverloadedPureVirtualMethodsCascade", &_::filtersNonOverloadedPureVirtualMethodsCascade)({"a_Result"})
            .method<void(Methods&) const>("findPureVirtualMethods", &_::findPureVirtualMethods)({"a_Result"})
            .method<Subroutine*(StringView) const>("getSubroutineCascade", &_::getSubroutineCascade)({"a_strIdentifierString"})
            .method<Subroutine*(StringView, TypesView, Modifiers) const>("getSubroutineCascade", &_::getSubroutineCascade)({"a_strName","a_FunctionSignature","a_Modifiers"})["0"]
            .method<void(ValueMembers&) const>("getValueMembersCascade", &_::getValueMembersCascade)({"out"})
            .method<void(Method*, Methods&)>("findOverriddenMethods", &_::findOverriddenMethods)({"a_pOverridingCandidate","a_Result"})
            .method<void(StringView, Signature*, Methods&)>("findOverriddenMethods", &_::findOverriddenMethods)({"a_strName","a_pSignature","a_Result"})
            .method<void(VirtualMethodTable*, Methods*, Methods*) const>("sortBaseAndRootMethods", &_::sortBaseAndRootMethods)({"a_pBaseVMT","a_OutBaseMethods","a_OutRootMethods"})
            .method<Method*(StringView) const>("getMethodCascade", &_::getMethodCascade)({"a_strIdentifierString"})
            .method<Method*(StringView, TypesView, Modifiers) const>("getMethodCascade", &_::getMethodCascade)({"a_strName","a_ParameterTypes","a_Modifiers"})["0"]
            .method<Method*(StringView, Type*, Modifiers) const>("getMethodCascade", &_::getMethodCascade)({"a_strName","a_pSingleParameterType","a_Qualifiers"})["0"]
            .method<ValueMember*(StringView) const>("getValueMemberCascade", &_::getValueMemberCascade)({"a_strIdentifierString"})
            .method<void(Type*, ValueMembers&) const>("findPublicValueMembersPointingValueTypeCascade", &_::findPublicValueMembersPointingValueTypeCascade)({"a_pType","out"})
            .method<Field*(StringView) const>("getFieldCascade", &_::getFieldCascade)({"a_strName"})
            .method<Field*(size_t) const, virtual_|override_>("getFieldByOffset", &_::getFieldByOffset)({"a_uiOffset"})
            .method<Field*(size_t) const, virtual_|override_>("getFieldAtOffset", &_::getFieldAtOffset)({"a_uiOffset"})
            .using_("ClassType::getFields")
            .method<void(AggregateFields&) const, virtual_|override_>("getFields", &_::getFields)({"a_OutFields"})
            .method<Property*(StringView) const>("getPropertyCascade", &_::getPropertyCascade)({"a_strPropertyName"})
            .method<StaticMethod*(StringView, TypesView) const>("getStaticMethodCascade", &_::getStaticMethodCascade)({"a_strName","a_FunctionSignature"})
            .method<StaticMethod*(StringView) const>("getStaticMethodCascade", &_::getStaticMethodCascade)({"a_strIdentifierString"})
            .method<StaticField*(StringView) const>("getStaticFieldCascade", &_::getStaticFieldCascade)({"a_strName"})
            .method<size_t(size_t) const>("getVirtualMethodTableSize", &_::getVirtualMethodTableSize)({"a_uiIndex"})
            .method<bool(Type*, StringView, TypesView, Modifiers, uint, Subroutines*) const, virtual_|override_>("acceptsSubroutine", &_::acceptsSubroutine)({"a_pReturnType","a_strName","a_Types","a_Modifiers","a_uiFlags","a_pOutConflictingMethods"})["NULL"]
            .method<bool(Type*, StringView, TypesView, Modifiers, uint, Subroutines*) const, virtual_>("acceptsDerivedClassSubroutine", &_::acceptsDerivedClassSubroutine)({"a_pReturnType","a_strName","a_Types","a_Modifiers","a_uiFlags","a_pOutConflictingMethods"})
            .method<::phantom::lang::Class *(size_t) const>("getBaseClass", &_::getBaseClass)({"a_uiIndex"})
            .method<void*(void*, size_t) const>("baseClassCast", &_::baseClassCast)({"a_pInstance","index"})
            .method<const void*(const void*, size_t) const>("baseClassCast", &_::baseClassCast)({"a_pInstance","index"})
            .method<size_t(size_t) const>("getInheritanceLevel", &_::getInheritanceLevel)({"i"})
            .method<size_t(Class*) const>("getInheritanceLevelFromBase", &_::getInheritanceLevelFromBase)({"a_pClass"})
            .method<bool(Symbol*) const, virtual_|override_>("hasPublicMember", &_::hasPublicMember)({"a_pLanguageElement"})
            .method<bool(Symbol*) const, virtual_|override_>("hasProtectedMember", &_::hasProtectedMember)({"a_pSymbol"})
            .method<bool(Symbol*) const, virtual_|override_>("hasPrivateMember", &_::hasPrivateMember)({"a_pSymbol"})
            .method<bool() const, virtual_|override_>("isListInitializable", &_::isListInitializable)
            .method<size_t(size_t) const, virtual_>("getVirtualMethodCount", &_::getVirtualMethodCount)({"a_uiOffset"})
            .method<VirtualMethodTables const&() const>("getVirtualMethodTables", &_::getVirtualMethodTables)
            .method<void(Class*, Access), virtual_>("addBaseClass", &_::addBaseClass)({"a_pClass","a_Access"})["Access::Public"]
            .method<void(Class*, size_t, Access), virtual_>("addBaseClass", &_::addBaseClass)({"a_pBaseClass","a_uiOffset","a_Access"})["Access::Public"]
            .method<const BaseClasses&() const>("getBaseClasses", &_::getBaseClasses)
            .method<Access(Class*) const>("getBaseClassAccess", &_::getBaseClassAccess)({"a_pClass"})
            .method<Access(Class*) const>("getBaseClassAccessCascade", &_::getBaseClassAccessCascade)({"a_pClass"})
            .method<bool(Class*, Access*) const>("hasBaseClass", &_::hasBaseClass)({"a_pClass","a_pInheritanceAccess"})["nullptr"]
            .method<bool(Class*) const>("hasBaseClassCascade", &_::hasBaseClassCascade)({"a_pClass"})
            .method<Class*(size_t) const>("getDerivedClass", &_::getDerivedClass)({"a_uiIndex"})
            .method<Classes const&() const>("getDerivedClasses", &_::getDerivedClasses)
            .method<void(Classes&) const>("getDerivedClassesCascade", &_::getDerivedClassesCascade)({"out"})
            .method<Classes const&() const>("getDerivedClassesCascade", &_::getDerivedClassesCascade)
            .method<bool(Class*, Access*) const>("hasDerivedClass", &_::hasDerivedClass)({"a_pClass","a_pDerivedAccess"})["nullptr"]
            .method<bool(Class*, Access*) const>("hasDerivedClassCascade", &_::hasDerivedClassCascade)({"a_pClass","a_pDerivedAccess"})["nullptr"]
            .method<bool(LanguageElement*) const>("hasMemberCascade", &_::hasMemberCascade)({"a_pElement"})
            .method<ptrdiff_t(size_t) const>("getBaseClassOffset", &_::getBaseClassOffset)({"a_uiBaseClassIndex"})
            .method<void(Class*, ptrdiff_t)>("setBaseClassOffset", &_::setBaseClassOffset)({"a_pClass","a_Offset"})
            .method<ptrdiff_t(Class*) const>("getBaseClassOffset", &_::getBaseClassOffset)({"a_pBaseClass"})
            .method<ptrdiff_t(Class*) const>("getBaseClassOffsetCascade", &_::getBaseClassOffsetCascade)({"a_pBaseType"})
            .method<ptrdiff_t(Type*) const, virtual_|override_>("getPointerAdjustmentOffset", &_::getPointerAdjustmentOffset)({"a_pType"})
            .method<ptrdiff_t(Class*) const>("getPointerAdjustmentOffset", &_::getPointerAdjustmentOffset)({"a_pClass"})
            .method<bool() const>("isRootClass", &_::isRootClass)
            .method<bool(Class*) const>("isA", &_::isA)({"a_pType"})
            .method<bool(Type*) const, virtual_|override_>("isA", &_::isA)({"a_pType"})
            .method<TypeRelation(Type*) const, virtual_|override_>("getRelationWith", &_::getRelationWith)({"a_pType"})
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(ValueMember*, Expression*)>("setOverriddenDefaultExpression", &_::setOverriddenDefaultExpression)({"a_pValueMember","a_pExpression"})
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*(ValueMember*) const>("getOverriddenDefaultExpression", &_::getOverriddenDefaultExpression)({"a_pValueMember"})
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Expression*(ValueMember*) const>("getOverriddenDefaultExpressionCascade", &_::getOverriddenDefaultExpressionCascade)({"a_pValueMember"})
            .method<bool() const, virtual_|override_>("isPolymorphic", &_::isPolymorphic)
            .using_("Type::allocate")
            .using_("Type::deallocate")
            .method<void*() const, virtual_|override_>("allocate", &_::allocate)
            .method<void(void*) const, virtual_|override_>("deallocate", &_::deallocate)({"a_pInstance"})
            .method<void*(size_t) const, virtual_|override_>("allocate", &_::allocate)({"a_uiCount"})
            .method<void(void*, size_t) const, virtual_|override_>("deallocate", &_::deallocate)({"a_pInstance","a_uiCount"})
            .method<void(void*) const, virtual_|override_>("construct", &_::construct)({"a_pInstance"})
            .method<void(void*) const, virtual_|override_>("destroy", &_::destroy)({"a_pObject"})
            .method<void*() const>("getSingleton", &_::getSingleton)
            .method<size_t() const>("getInstanceCount", &_::getInstanceCount)
            .method<void*(Class*, void*) const>("upcast", &_::upcast)({"a_pBaseClass","a_pInstance"})
            .method<void const*(Class*, void const*) const>("upcast", &_::upcast)({"a_pBaseClass","a_pInstance"})
            .method<void*(Class*, void*) const>("downcast", &_::downcast)({"a_pDerivedClass","a_pInstance"})
            .method<void const*(Class*, void const*) const>("downcast", &_::downcast)({"a_pDerivedClass","a_pInstance"})
            .method<void*(Class*, void*) const>("cast", &_::cast)({"a_pBaseOrDerivedClass","a_pInstance"})
            .method<void const*(Class*, void const*) const>("cast", &_::cast)({"a_pBaseOrDerivedClass","a_pInstance"})
            .method<void*(Type*, void*) const, virtual_|override_>("upcast", &_::upcast)({"a_pTarget","a_pBaseAddress"})
            .method<void*(Type*, void*) const, virtual_|override_>("downcast", &_::downcast)({"a_pTarget","a_pBaseAddress"})
            .method<void*(Type*, void*) const, virtual_|final_>("cast", &_::cast)({"a_pTargetType","a_pBase"})
            .method<void const*(Type*, void const*) const, virtual_|final_>("cast", &_::cast)({"a_pTargetType","a_pInstance"})
            .method<void*() const, virtual_|override_>("newInstance", &_::newInstance)
            .method<void(void*) const, virtual_|override_>("deleteInstance", &_::deleteInstance)({"a_pInstance"})
            .method<void*(void*) const, virtual_|override_>("placementNewInstance", &_::placementNewInstance)({"a_pMemory"})
            .method<void*(void*, Constructor*, void**) const, virtual_|override_>("placementNewInstance", &_::placementNewInstance)({"a_pMemory","a_pConstructor","a_pArgs"})
            .method<void(void*) const, virtual_|override_>("placementDeleteInstance", &_::placementDeleteInstance)({"a_pInstance"})
            .method<const ::phantom::Variant &(StringView) const>("getMetaDataIncludingBases", &_::getMetaDataIncludingBases)({"a_Name"})
            .method<const ::phantom::Variant &(StringWithHash) const>("getMetaDataIncludingBases", &_::getMetaDataIncludingBases)({"a_Hash"})
            .method<void(StringView, Variants&) const>("getMetaDatasIncludingBases", &_::getMetaDatasIncludingBases)({"a_Name","a_MetaDatas"})
            .method<void(StringWithHash, Variants&) const>("getMetaDatasIncludingBases", &_::getMetaDatasIncludingBases)({"a_Hash","a_MetaDatas"})
            .method<bool(StringWithHash) const>("hasMetaDataIncludingBases", &_::hasMetaDataIncludingBases)({"a_strName"})
            .method<bool(StringView) const>("hasMetaDataIncludingBases", &_::hasMetaDataIncludingBases)({"a_strName"})
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<bool() const, virtual_|override_>("isCopyConstructible", &_::isCopyConstructible)
            .method<bool() const, virtual_|override_>("isMoveConstructible", &_::isMoveConstructible)
            .method<void(void*, void const*) const, virtual_|override_>("copyConstruct", &_::copyConstruct)({"a_pDest","a_pSrc"})
            .method<bool(void const*, void const*) const, virtual_|override_>("equal", &_::equal)({"a_pInstance0","a_pInstance1"})
            .method<bool() const, virtual_>("hasNewVTable", &_::hasNewVTable)
            .method<bool(Type*) const, virtual_|override_>("hasStrongDependencyOnType", &_::hasStrongDependencyOnType)({"a_pType"})
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
            .staticMethod<::phantom::lang::VirtualMethodTable *(Class*, void**, size_t)>("CreateVirtualMethodTable", &_::CreateVirtualMethodTable)({"a_pOwner","a_ppAddr","a_MethodCount"})
            .staticMethod<::phantom::lang::VirtualMethodTable *(Class*, VirtualMethodTable*, size_t)>("DeriveVirtualMethodTable", &_::DeriveVirtualMethodTable)({"a_pOwner","a_pBase","a_MethodCount"})
            .staticMethod<::phantom::lang::VirtualMethodTable *(Class*, VirtualMethodTable*)>("DeriveVirtualMethodTable", &_::DeriveVirtualMethodTable)({"a_pOwner","a_pBase"})
            .staticMethod<::phantom::lang::Class *(VirtualMethodTable*)>("VTablePrimaryClass", &_::VTablePrimaryClass)({"a_pVTable"})
            .method<::phantom::lang::VirtualMethodTable *(), virtual_>("createVirtualMethodTable", &_::createVirtualMethodTable)
            .method<::phantom::lang::VirtualMethodTable *(VirtualMethodTable*), virtual_>("deriveVirtualMethodTable", &_::deriveVirtualMethodTable)({"a_pVirtualMethodTable"})
            .method<SymbolExtension*(Class*, size_t) const>("getExtensionCascade", &_::getExtensionCascade)({"a_pSymbolExtensionClass","a_Num"})["0"]
            .method<void(void*), virtual_>("registerInstance", &_::registerInstance)({"a_pInstance"})
            .method<void(void*), virtual_>("unregisterInstance", &_::unregisterInstance)({"a_pInstance"})
            .method<InstanceCache*()>("getOrCreateInstanceCache", &_::getOrCreateInstanceCache)
        
        .protected_()
            .method<bool() const, virtual_|override_>("canBeDestroyed", &_::canBeDestroyed)
            .method<Strings() const>("getBaseClasseNames", &_::getBaseClasseNames)
            .method<void(Strings), virtual_>("setBaseClasseNames", &_::setBaseClasseNames)({"table"})
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
