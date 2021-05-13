#pragma once

// haunt {

// clang-format off

#include "ClassType.h"

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
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("ClassType")

        PHANTOM_CLASS_T((class), (Sign), FunctionTypeH)
        {
            this_()
            ;
        }
        PHANTOM_STRUCT_TVS((class), (R), (class, Params), (R(Params...)), FunctionTypeH)
        {
            using Types = typedef_< phantom::lang::Types>;
            this_()
            .PHANTOM_T staticMethod<::phantom::lang::Type *(Types&)>("ReadReturnTypeAndParameterTypes", &_::ReadReturnTypeAndParameterTypes)({"a_Params"})
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(ClassType)
        {
            using AggregateFields = typedef_< phantom::lang::AggregateFields>;
            using Constructors = typedef_< phantom::lang::Constructors>;
            using DataElements = typedef_< phantom::lang::DataElements>;
            using ExtraData = typedef_<_::ExtraData>;
            using Fields = typedef_< phantom::lang::Fields>;
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using MemberAnonymousSections = typedef_< phantom::lang::MemberAnonymousSections>;
            using Methods = typedef_< phantom::lang::Methods>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using Parameters = typedef_< phantom::lang::Parameters>;
            using Properties = typedef_< phantom::lang::Properties>;
            using StringView = typedef_< phantom::StringView>;
            using Subroutines = typedef_< phantom::lang::Subroutines>;
            using Symbols = typedef_< phantom::lang::Symbols>;
            using Types = typedef_< phantom::lang::Types>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            using ValueMembers = typedef_< phantom::lang::ValueMembers>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Type, ::phantom::lang::Scope, ::phantom::lang::Aggregate>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .struct_<ExtraData>()
                .constructor<void()>()
                .field("m_AlignmentComputer", &_::ExtraData::m_AlignmentComputer)
                .field("m_uiDataTypeCount", &_::ExtraData::m_uiDataTypeCount)
                .field("m_BuildState", &_::ExtraData::m_BuildState)
            .end()
        
        .protected_()
            .constructor<void(TypeKind, Modifiers, uint)>()({"a_eTypeKind","a_Modifiers","a_uiFlags"})["0"]["0"]
            .constructor<void(TypeKind, StringView, Modifiers, uint)>()({"a_eTypeKind","a_strName","a_Modifiers","a_uiFlags"})["0"]["0"]
            .constructor<void(TypeKind, StringView, size_t, size_t, Modifiers, uint)>()({"a_eTypeKind","a_strName","a_uiSize","a_uiAlignment","a_Modifiers","a_uiFlags"})
        
        .public_()
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_>("terminate", &_::terminate)
            .using_("Type::asClass")
            .using_("Type::asClassType")
            .method<bool() const, virtual_|override_>("isCopyable", &_::isCopyable)
            .method<bool() const, virtual_|override_>("isCopyAssignable", &_::isCopyAssignable)
            .method<bool() const, virtual_|override_>("isCopyConstructible", &_::isCopyConstructible)
            .method<bool() const, virtual_|override_>("isMoveable", &_::isMoveable)
            .method<bool() const, virtual_|override_>("isMoveAssignable", &_::isMoveAssignable)
            .method<bool() const, virtual_|override_>("isMoveConstructible", &_::isMoveConstructible)
            .method<bool() const, virtual_|override_>("isDefaultInstanciable", &_::isDefaultInstanciable)
            .method<bool() const, virtual_|override_>("isDefaultConstructible", &_::isDefaultConstructible)
            .method<Scope*() const, virtual_|final_|override_>("asScope", &_::asScope)
            .method<bool() const, virtual_>("isListInitializable", &_::isListInitializable)
            .method<void(Access)>("setDefaultAccess", &_::setDefaultAccess)({"a_eAccess"})
            .method<Access() const>("getDefaultAccess", &_::getDefaultAccess)
            .method<void(Access)>("setCurrentAccess", &_::setCurrentAccess)({"a_eAccess"})
            .method<Access() const>("getCurrentAccess", &_::getCurrentAccess)
            .method<void(Constructor*)>("addConstructor", &_::addConstructor)({"a_pConstructor"})
            .method<Constructor*(const Parameters&, Modifiers, uint)>("addConstructor", &_::addConstructor)({"a_Parameters","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<Constructor*(Modifiers, uint)>("addConstructor", &_::addConstructor)({"a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<Constructor*(Type*, Modifiers, uint)>("addConstructor", &_::addConstructor)({"a_pSingleParameterType","a_Modifiers","a_uiFlags"})["0"]
            .method<Method*(Modifiers, uint)>("addDestructor", &_::addDestructor)({"a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<void(ValueMember*)>("addValueMember", &_::addValueMember)({"a_pValueMember"})
            .method<void(StaticField*)>("addStaticField", &_::addStaticField)({"a_pField"})
            .method<void(StaticField*)>("addField", &_::addField)({"a_pField"})
            .method<void(Field*)>("addField", &_::addField)({"a_pField"})
            .method<Field*(Type*, StringView, uint, Modifiers, uint)>("addField", &_::addField)({"a_pValueType","a_strName","a_uiFilterFlag","a_Modifiers","a_uiFlags"})["0"]["0"]
            .method<void(Property*)>("addProperty", &_::addProperty)({"a_pProperty"})
            .method<Property*(StringView, uint)>("addProperty", &_::addProperty)({"a_strName","a_uiFilterMask"})["PHANTOM_R_FILTER_PROPERTY"]
            .method<void(Method*)>("addMethod", &_::addMethod)({"a_pMethod"})
            .method<void(StaticMethod*)>("addMethod", &_::addMethod)({"a_pMethod"})
            .method<void(StaticMethod*)>("addStaticMethod", &_::addStaticMethod)({"a_pStaticMethod"})
            .method<void(Subroutine*), virtual_|override_>("addSubroutine", &_::addSubroutine)({"a_pSubroutine"})
            .method<Constructor*() const>("getDefaultConstructor", &_::getDefaultConstructor)
            .method<Constructor*() const>("getCopyConstructor", &_::getCopyConstructor)
            .method<Constructor*() const>("getMoveConstructor", &_::getMoveConstructor)
            .method<Constructor*(TypesView) const>("getConstructor", &_::getConstructor)({"a_Types"})
            .method<Constructor*(StringView) const>("getConstructor", &_::getConstructor)({"a_strParameterString"})
            .method<Constructor*(size_t) const>("getConstructor", &_::getConstructor)({"a_uiIndex"})
            .method<Constructor*(Type*) const>("getConstructor", &_::getConstructor)({"a_pType"})
            .method<Method*(Type*) const>("getConversionFunction", &_::getConversionFunction)({"a_pType"})
            .method<Method*() const>("getCopyAssignmentOperator", &_::getCopyAssignmentOperator)
            .method<Method*() const>("getMoveAssignmentOperator", &_::getMoveAssignmentOperator)
            .method<void(Types&, bool) const>("getFullConversionTypes", &_::getFullConversionTypes)({"out","a_bImplicits"})["true"]
            .method<Destructor*() const>("getDestructor", &_::getDestructor)
            .method<ValueMember*(StringView) const>("getValueMember", &_::getValueMember)({"a_strName"})
            .method<const ValueMembers&() const>("getValueMembers", &_::getValueMembers)
            .method<Field*(size_t) const>("getField", &_::getField)({"a_uiIndex"})
            .method<Field*(StringView) const>("getField", &_::getField)({"a_strName"})
            .method<const Fields&() const>("getFields", &_::getFields)
            .method<void(Fields&) const>("getAllFields", &_::getAllFields)({"a_Out"})
            .method<const DataElements&() const>("getDataElements", &_::getDataElements)
            .method<size_t(Field*) const>("getFieldIndex", &_::getFieldIndex)({"a_pField"})
            .method<::phantom::lang::Field *(size_t) const, virtual_>("getFieldByOffset", &_::getFieldByOffset)({"a_uiOffset"})
            .method<::phantom::lang::Field *(size_t) const, virtual_>("getFieldAtOffset", &_::getFieldAtOffset)({"a_uiOffset"})
            .method<Field*(const void*, const void*) const>("getFieldByAddress", &_::getFieldByAddress)({"a_pBaseAddress","a_pAddress"})
            .method<Field*(const void*, const void*) const>("getFieldAtAddress", &_::getFieldAtAddress)({"a_pBaseAddress","a_pAddress"})
            .method<StaticField*(StringView) const>("getStaticField", &_::getStaticField)({"a_strName"})
            .method<Property*(size_t) const>("getProperty", &_::getProperty)({"a_uiIndex"})
            .method<Property*(StringView) const>("getProperty", &_::getProperty)({"a_strName"})
            .method<const Properties&() const>("getProperties", &_::getProperties)
            .method<Subroutine*(StringView) const>("getSubroutine", &_::getSubroutine)({"a_strIdentifierString"})
            .method<Subroutine*(StringView, TypesView, Modifiers) const, virtual_|override_>("getSubroutine", &_::getSubroutine)({"a_strName","a_Types","a_Modifiers"})["0"]
            .method<Method*(StringView) const>("getMethod", &_::getMethod)({"a_strIdentifierString"})
            .method<Method*(StringView, TypesView, Modifiers) const>("getMethod", &_::getMethod)({"a_strName","a_Types","a_Qualifiers"})["0"]
            .method<Method*(Type*, StringView, TypesView, Modifiers) const>("getMethod", &_::getMethod)({"a_pReturnType","a_strName","a_ParameterTypes","a_Qualifiers"})["0"]
            .method<::phantom::lang::Method *(void*) const, virtual_>("getMethodByPtr", &_::getMethodByPtr)({"a_ppMethodPtr"})
            .method<const Methods&() const>("getMethods", &_::getMethods)
            .method<StaticMethod*(StringView) const>("getStaticMethod", &_::getStaticMethod)({"a_strIdentifierString"})
            .method<StaticMethod*(StringView, TypesView) const>("getStaticMethod", &_::getStaticMethod)({"a_strName","a_Types"})
            .method<Method*(StringView) const>("getUniqueMethodWithName", &_::getUniqueMethodWithName)({"a_strName"})
            .method<void(Type*, ValueMembers&) const>("findPublicValueMembersPointingValueType", &_::findPublicValueMembersPointingValueType)({"a_pType","out"})
            .method<const Constructors&() const>("getConstructors", &_::getConstructors)
            .method<bool(const LanguageElements&) const>("matchesTemplateArguments", &_::matchesTemplateArguments)({"a_Elements"})
            .method<void(MemberAnonymousSection*)>("addMemberAnonymousSection", &_::addMemberAnonymousSection)({"a_pMemberAnonymousSection"})
            .method<MemberAnonymousSections const&() const>("getMemberAnonymousSections", &_::getMemberAnonymousSections)
            .method<bool(Type*, StringView, TypesView, Modifiers, uint, Subroutines*) const, virtual_|override_>("acceptsSubroutine", &_::acceptsSubroutine)({"a_pReturnType","a_strName","a_Types","a_Modifiers","a_uiFlags","a_pOutConflictingSubroutines"})["nullptr"]
            .method<bool(Subroutine*, Subroutines*) const>("acceptsSubroutine", &_::acceptsSubroutine)({"a_pSubroutine","a_pOutConflictingSubroutines"})["nullptr"]
            .method<bool() const, virtual_>("canHaveImplicitDefaultConstructor", &_::canHaveImplicitDefaultConstructor)
            .method<bool() const, virtual_>("canHaveImplicitCopyConstructor", &_::canHaveImplicitCopyConstructor)
            .method<bool() const, virtual_>("canHaveImplicitCopyAssignmentOperator", &_::canHaveImplicitCopyAssignmentOperator)
            .method<bool() const, virtual_>("canHaveImplicitMoveConstructor", &_::canHaveImplicitMoveConstructor)
            .method<bool() const, virtual_>("canHaveImplicitMoveAssignmentOperator", &_::canHaveImplicitMoveAssignmentOperator)
            .method<void(), virtual_>("addImplicitDefaultConstructor", &_::addImplicitDefaultConstructor)
            .method<void(), virtual_>("addImplicitCopyConstructor", &_::addImplicitCopyConstructor)
            .method<void(), virtual_>("addImplicitCopyAssignmentOperator", &_::addImplicitCopyAssignmentOperator)
            .method<void(), virtual_>("addImplicitMoveConstructor", &_::addImplicitMoveConstructor)
            .method<void(), virtual_>("addImplicitMoveAssignmentOperator", &_::addImplicitMoveAssignmentOperator)
            .method<void(), virtual_>("addImplicitDestructor", &_::addImplicitDestructor)
            .method<void*() const, virtual_|override_>("allocate", &_::allocate)
            .method<void*(size_t) const, virtual_|override_>("allocate", &_::allocate)({"n"})
            .method<void(void*) const, virtual_|override_>("construct", &_::construct)({"a_pInstance"})
            .method<void(void*) const, virtual_|override_>("destroy", &_::destroy)({"a_pInstance"})
            .method<void*() const, virtual_|override_>("newInstance", &_::newInstance)
            .method<void*(void*) const, virtual_|override_>("placementNewInstance", &_::placementNewInstance)({""})
            .method<void*(void*, Constructor*, void**) const, virtual_>("placementNewInstance", &_::placementNewInstance)({"","a_pConstructor","a_pArgs"})["NULL"]
            .method<void(void*) const, virtual_|override_>("deleteInstance", &_::deleteInstance)({"a_pInstance"})
            .method<void(void*) const, virtual_|override_>("placementDeleteInstance", &_::placementDeleteInstance)({"a_pInstance"})
            .method<void(void*, void const*) const, virtual_|override_>("copyConstruct", &_::copyConstruct)({"a_pDest","a_pSrc"})
            .method<void(void*, void*) const, virtual_|override_>("moveConstruct", &_::moveConstruct)({"a_pDest","a_pSrc"})
            .method<void(void*, void const*) const, virtual_|override_>("copyAssign", &_::copyAssign)({"a_pDest","a_pSrc"})
            .method<void(void*, void*) const, virtual_|override_>("moveAssign", &_::moveAssign)({"a_pDest","a_pSrc"})
            .using_("Type::fetchElements")
            .method<void(LanguageElements&, Class*) const, virtual_|override_>("fetchElements", &_::fetchElements)({"out","a_pClass"})["nullptr"]
            .method<ExtraData*() const>("getExtraData", &_::getExtraData)
            .method<Template*() const>("getTemplate", &_::getTemplate)
            .method<bool() const, virtual_|override_>("isTemplateInstance", &_::isTemplateInstance)
            .method<TemplateSpecialization*() const>("getTemplateSpecialization", &_::getTemplateSpecialization)
            .method<TemplateSpecialization*(StringView, const LanguageElements&) const>("getTemplateSpecialization", &_::getTemplateSpecialization)({"a_strTemplateName","a_Arguments"})
            .method<void(Symbol*)>("addFriend", &_::addFriend)({"a_pFriend"})
            .method<bool(Symbol*) const, virtual_|override_>("hasFriend", &_::hasFriend)({"a_pFriend"})
            .method<Symbols const&() const>("getFriends", &_::getFriends)
            .method<void() const, virtual_|override_>("checkCompleteness", &_::checkCompleteness)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)({"a_pSymbol"})
            .method<bool(LanguageElement*) const, virtual_|override_>("isSame", &_::isSame)({"a_pElement"})
            .method<bool(Type*) const, virtual_|override_>("hasStrongDependencyOnType", &_::hasStrongDependencyOnType)({"a_pType"})
            .method<void(), virtual_>("finalizeNative", &_::finalizeNative)
            .method<void(AggregateFields&) const, virtual_|override_>("getFields", &_::getFields)({"_fields"})
            .method<void(AggregateFields&) const, virtual_|override_>("getFlattenedAggregateFields", &_::getFlattenedAggregateFields)({"_aggregateFields"})
        
        .protected_()
            .method<bool() const, virtual_|override_>("canBeDestroyed", &_::canBeDestroyed)
            .method<void(), virtual_|override_>("onElementsAccess", &_::onElementsAccess)
            .method<void(ExtraData*)>("setExtraData", &_::setExtraData)({"a_pExtraData"})
            .method<void(Symbol*), virtual_|override_>("onScopeSymbolAdded", &_::onScopeSymbolAdded)({"a_pSym"})
            .method<void(Symbol*), virtual_|override_>("onScopeSymbolRemoving", &_::onScopeSymbolRemoving)({"a_pSym"})
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("ClassType")
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
