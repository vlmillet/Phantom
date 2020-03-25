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
            .PHANTOM_T staticMethod<Type*(Types&)>("ReadReturnTypeAndParameterTypes", &_::ReadReturnTypeAndParameterTypes)
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
            using Types = typedef_< phantom::lang::Types>;
            using TypesView = typedef_< phantom::lang::TypesView>;
            using ValueMembers = typedef_< phantom::lang::ValueMembers>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::lang::Type, ::phantom::lang::Scope, ::phantom::lang::Aggregate>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .struct_<ExtraData>()
                .method<void(), virtual_>("PHANTOM_CUSTOM_VIRTUAL_DELETE", &_::ExtraData::PHANTOM_CUSTOM_VIRTUAL_DELETE)
                .constructor<void()>()
                .field("m_AlignmentComputer", &_::ExtraData::m_AlignmentComputer)
                .field("m_uiDataTypeCount", &_::ExtraData::m_uiDataTypeCount)
                .field("m_BuildState", &_::ExtraData::m_BuildState)
            .end()
        
        .protected_()
            .constructor<void(TypeKind, ExtraData*, Modifiers, uint)>()["0"]["0"]
            .constructor<void(TypeKind, ExtraData*, StringView, Modifiers, uint)>()["0"]["0"]
            .constructor<void(TypeKind, StringView, size_t, size_t, Modifiers, uint)>()
        
        .public_()
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
            .method<void(Access)>("setDefaultAccess", &_::setDefaultAccess)
            .method<Access() const>("getDefaultAccess", &_::getDefaultAccess)
            .method<void(Constructor*), virtual_>("addConstructor", &_::addConstructor)
            .method<Constructor*(StringView), virtual_>("addConstructor", &_::addConstructor)
            .method<Constructor*(const Parameters&, Modifiers, uint)>("addConstructor", &_::addConstructor)["0"]["0"]
            .method<Constructor*(Modifiers, uint)>("addConstructor", &_::addConstructor)["0"]["0"]
            .method<Constructor*(Type*, Modifiers, uint)>("addConstructor", &_::addConstructor)["0"]
            .method<Method*(Modifiers, uint)>("addDestructor", &_::addDestructor)["0"]["0"]
            .method<void(ValueMember*), virtual_>("addValueMember", &_::addValueMember)
            .method<void(StaticField*), virtual_>("addStaticField", &_::addStaticField)
            .method<void(StaticField*), virtual_>("addField", &_::addField)
            .method<void(Field*), virtual_>("addField", &_::addField)
            .method<Field*(Type*, StringView, uint, Modifiers, uint)>("addField", &_::addField)["0"]["0"]
            .method<void(Property*), virtual_>("addProperty", &_::addProperty)
            .method<Property*(StringView, uint), virtual_>("addProperty", &_::addProperty)["PHANTOM_R_FILTER_PROPERTY"]
            .method<void(Method*), virtual_>("addMethod", &_::addMethod)
            .method<void(StaticMethod*)>("addMethod", &_::addMethod)
            .method<void(StaticMethod*), virtual_>("addStaticMethod", &_::addStaticMethod)
            .method<void(Subroutine*), virtual_|override_>("addSubroutine", &_::addSubroutine)
            .method<void(Constructor*), virtual_>("removeConstructor", &_::removeConstructor)
            .method<void(ValueMember*), virtual_>("removeValueMember", &_::removeValueMember)
            .method<void(StaticField*), virtual_>("removeStaticField", &_::removeStaticField)
            .method<void(Field*), virtual_>("removeField", &_::removeField)
            .method<void(Property*), virtual_>("removeProperty", &_::removeProperty)
            .method<void(Method*), virtual_>("removeMethod", &_::removeMethod)
            .method<void(StaticMethod*), virtual_>("removeStaticMethod", &_::removeStaticMethod)
            .method<Constructor*() const>("getDefaultConstructor", &_::getDefaultConstructor)
            .method<Constructor*() const>("getCopyConstructor", &_::getCopyConstructor)
            .method<Constructor*() const>("getMoveConstructor", &_::getMoveConstructor)
            .method<Constructor*(TypesView) const>("getConstructor", &_::getConstructor)
            .method<Constructor*(StringView) const>("getConstructor", &_::getConstructor)
            .method<Constructor*(size_t) const>("getConstructor", &_::getConstructor)
            .method<Constructor*(Type*) const>("getConstructor", &_::getConstructor)
            .method<Method*(Type*) const>("getConversionFunction", &_::getConversionFunction)
            .method<Method*() const>("getCopyAssignmentOperator", &_::getCopyAssignmentOperator)
            .method<Method*() const>("getMoveAssignmentOperator", &_::getMoveAssignmentOperator)
            .method<void(Types&, bool) const>("getFullConversionTypes", &_::getFullConversionTypes)["true"]
            .method<Destructor*() const>("getDestructor", &_::getDestructor)
            .method<ValueMember*(StringView) const>("getValueMember", &_::getValueMember)
            .method<const ValueMembers&() const>("getValueMembers", &_::getValueMembers)
            .method<Field*(size_t) const>("getField", &_::getField)
            .method<Field*(StringView) const>("getField", &_::getField)
            .method<const Fields&() const>("getFields", &_::getFields)
            .method<void(Fields&) const>("getAllFields", &_::getAllFields)
            .method<const DataElements&() const>("getDataElements", &_::getDataElements)
            .method<size_t(Field*) const>("getFieldIndex", &_::getFieldIndex)
            .method<Field*(size_t) const, virtual_>("getFieldByOffset", &_::getFieldByOffset)
            .method<Field*(size_t) const, virtual_>("getFieldAtOffset", &_::getFieldAtOffset)
            .method<Field*(const void*, const void*) const>("getFieldByAddress", &_::getFieldByAddress)
            .method<Field*(const void*, const void*) const>("getFieldAtAddress", &_::getFieldAtAddress)
            .method<StaticField*(StringView) const>("getStaticField", &_::getStaticField)
            .method<Property*(size_t) const>("getProperty", &_::getProperty)
            .method<Property*(StringView) const>("getProperty", &_::getProperty)
            .method<const Properties&() const>("getProperties", &_::getProperties)
            .method<Subroutine*(StringView) const>("getSubroutine", &_::getSubroutine)
            .method<Subroutine*(StringView, TypesView, Modifiers) const, virtual_|override_>("getSubroutine", &_::getSubroutine)["0"]
            .method<Method*(StringView) const>("getMethod", &_::getMethod)
            .method<Method*(StringView, TypesView, Modifiers) const>("getMethod", &_::getMethod)["0"]
            .method<Method*(Type*, StringView, TypesView, Modifiers) const>("getMethod", &_::getMethod)["0"]
            .method<Method*(void*) const, virtual_>("getMethodByPtr", &_::getMethodByPtr)
            .method<const Methods&() const>("getMethods", &_::getMethods)
            .method<StaticMethod*(StringView) const>("getStaticMethod", &_::getStaticMethod)
            .method<StaticMethod*(StringView, TypesView) const>("getStaticMethod", &_::getStaticMethod)
            .method<Method*(StringView) const>("getUniqueMethodWithName", &_::getUniqueMethodWithName)
            .method<void(Type*, ValueMembers&) const>("findPublicValueMembersPointingValueType", &_::findPublicValueMembersPointingValueType)
            .method<const Constructors&() const>("getConstructors", &_::getConstructors)
            .method<bool(const LanguageElements&) const>("matchesTemplateArguments", &_::matchesTemplateArguments)
            .method<void(MemberAnonymousSection*), virtual_>("addMemberAnonymousSection", &_::addMemberAnonymousSection)
            .method<MemberAnonymousSection*(StringView, Modifiers, uint), virtual_>("addMemberAnonymousSection", &_::addMemberAnonymousSection)["0"]["0"]
            .method<void(MemberAnonymousSection*), virtual_>("removeMemberAnonymousSection", &_::removeMemberAnonymousSection)
            .method<MemberAnonymousSections const&() const>("getMemberAnonymousSections", &_::getMemberAnonymousSections)
            .method<bool(Type*, StringView, TypesView, Modifiers, uint, Subroutines*) const, virtual_|override_>("acceptsSubroutine", &_::acceptsSubroutine)["nullptr"]
            .method<bool(Subroutine*, Subroutines*) const>("acceptsSubroutine", &_::acceptsSubroutine)["nullptr"]
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
            .method<void*(size_t) const, virtual_|override_>("allocate", &_::allocate)
            .method<void(void*) const, virtual_|override_>("construct", &_::construct)
            .method<void(void*) const, virtual_|override_>("destroy", &_::destroy)
            .method<void*() const, virtual_|override_>("newInstance", &_::newInstance)
            .method<void*(void*) const, virtual_|override_>("placementNewInstance", &_::placementNewInstance)
            .method<void*(void*, Constructor*, void**) const, virtual_>("placementNewInstance", &_::placementNewInstance)["NULL"]
            .method<void(void*) const, virtual_|override_>("deleteInstance", &_::deleteInstance)
            .method<void(void*) const, virtual_|override_>("placementDeleteInstance", &_::placementDeleteInstance)
            .method<void(void*, void const*) const, virtual_|override_>("copyConstruct", &_::copyConstruct)
            .method<void(void*, void*) const, virtual_|override_>("moveConstruct", &_::moveConstruct)
            .method<void(void*, void const*) const, virtual_|override_>("copyAssign", &_::copyAssign)
            .method<void(void*, void*) const, virtual_|override_>("moveAssign", &_::moveAssign)
            .using_("Type::fetchElements")
            .method<void(LanguageElements&, Class*) const, virtual_|override_>("fetchElements", &_::fetchElements)["nullptr"]
            .method<ExtraData*() const>("getExtraData", &_::getExtraData)
            .method<Template*() const>("getTemplate", &_::getTemplate)
            .method<bool() const, virtual_|override_>("isTemplateInstance", &_::isTemplateInstance)
            .method<TemplateSpecialization*() const>("getTemplateSpecialization", &_::getTemplateSpecialization)
            .method<TemplateSpecialization*(StringView, const LanguageElements&) const>("getTemplateSpecialization", &_::getTemplateSpecialization)
            .method<void(Symbol*)>("addFriend", &_::addFriend)
            .method<bool(Symbol*) const, virtual_|override_>("hasFriend", &_::hasFriend)
            .method<void() const, virtual_|override_>("checkCompleteness", &_::checkCompleteness)
            .method<bool(Symbol*) const, virtual_|override_>("isSame", &_::isSame)
            .method<bool(LanguageElement*) const, virtual_|override_>("isSame", &_::isSame)
            .method<bool(Type*) const, virtual_|override_>("hasStrongDependencyOnType", &_::hasStrongDependencyOnType)
            .method<void(), virtual_>("finalizeNative", &_::finalizeNative)
            .method<void(AggregateFields&) const, virtual_|override_>("getFields", &_::getFields)
            .method<void(AggregateFields&) const, virtual_|override_>("getFlattenedAggregateFields", &_::getFlattenedAggregateFields)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<bool() const, virtual_|override_>("canBeDestroyed", &_::canBeDestroyed)
            .method<void(LanguageElement*), virtual_|override_>("onElementAdded", &_::onElementAdded)
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
            .method<void(Symbol*), virtual_|override_>("addScopeElement", &_::addScopeElement)
            .method<void(Symbol*), virtual_|override_>("removeScopeElement", &_::removeScopeElement)
            .method<void(), virtual_|override_>("onElementsAccess", &_::onElementsAccess)
        
        .protected_()
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
