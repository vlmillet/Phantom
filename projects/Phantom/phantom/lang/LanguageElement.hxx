#pragma once

// haunt {

// clang-format off

#include "LanguageElement.h"

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
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/Delegate.hxx>
#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallSet.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("LanguageElement")

        PHANTOM_CLASS_T((class), (t_Scope), Members)
        {
            this_()
            ;
        }
        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Ellipsis)
        {
            this_()
            ;
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<PlaceholderMap>("PlaceholderMap"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<ModuleSet>("ModuleSet"); }
        PHANTOM_CLASS(LanguageElement)
        {
            using LanguageElements = typedef_< phantom::lang::LanguageElements>;
            using LanguageElementsView = typedef_< phantom::lang::LanguageElementsView>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using NewCallSite = typedef_<_::NewCallSite>;
            using String = typedef_< phantom::String>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using SymbolFilter = typedef_<_::SymbolFilter>;
            using Symbols = typedef_< phantom::lang::Symbols>;
            this_()
            .inherits<::phantom::Object>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, VisitorData), virtual_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .typedef_<SymbolFilter>("SymbolFilter")
            .staticMethod<::phantom::lang::Symbol *(Symbol*, bool)>("NoFilter", &_::NoFilter)
            .staticMethod<::phantom::lang::Symbol *(Symbol*, bool)>("PublicFilter", &_::PublicFilter)
            .staticMethod<::phantom::lang::Symbol *(Symbol*, bool)>("PublicIfUnamedSubSymbolFilter", &_::PublicIfUnamedSubSymbolFilter)
            .method<void()>("initialize", &_::initialize)
            .method<void(), virtual_|override_>("terminate", &_::terminate)
        
        .public_()
            .struct_<NewCallSite>()
                .constructor<void(LanguageElement*), explicit_>()
                .field("this_", &_::NewCallSite::this_)
            .end()
            .method<const ::phantom::CustomAllocator *() const, virtual_>("getAllocator", &_::getAllocator)
            .method<void(LanguageElement*)>("Delete", &_::Delete)
            .method<LanguageElementsView() const>("getElements", &_::getElements)
            .method<bool() const>("isTemplateElement", &_::isTemplateElement)
            .method<bool(LanguageElement*) const>("hasReferencedElement", &_::hasReferencedElement)
            .method<bool(LanguageElement*) const>("hasReferencingElement", &_::hasReferencingElement)
            .method<Symbol*() const>("asSymbol", &_::asSymbol)
            .method<::phantom::lang::Type *() const, virtual_>("asAddressType", &_::asAddressType)
            .method<::phantom::lang::Alias *() const, virtual_>("asAlias", &_::asAlias)
            .method<::phantom::lang::AnonymousSection *() const, virtual_>("asAnonymousSection", &_::asAnonymousSection)
            .method<::phantom::lang::AnonymousStruct *() const, virtual_>("asAnonymousStruct", &_::asAnonymousStruct)
            .method<::phantom::lang::AnonymousUnion *() const, virtual_>("asAnonymousUnion", &_::asAnonymousUnion)
            .method<::phantom::lang::PrimitiveType *() const, virtual_>("asArithmeticType", &_::asArithmeticType)
            .method<::phantom::lang::Array *() const, virtual_>("asArray", &_::asArray)
            /// missing symbol(s) reflection (phantom::lang::Block) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<::phantom::lang::Block *() const, virtual_>("asBlock", &_::asBlock)
            .method<::phantom::lang::Class *() const, virtual_>("asClass", &_::asClass)
            .method<::phantom::lang::Pointer *() const, virtual_>("asClassPointer", &_::asClassPointer)
            .method<::phantom::lang::LValueReference *() const, virtual_>("asClassLValueReference", &_::asClassLValueReference)
            .method<::phantom::lang::RValueReference *() const, virtual_>("asClassRValueReference", &_::asClassRValueReference)
            .method<::phantom::lang::Type *() const, virtual_>("asClassAddressType", &_::asClassAddressType)
            .method<::phantom::lang::ClassType *() const, virtual_>("asClassType", &_::asClassType)
            .method<::phantom::lang::VectorClass *() const, virtual_>("asVectorClass", &_::asVectorClass)
            .method<::phantom::lang::MapClass *() const, virtual_>("asMapClass", &_::asMapClass)
            .method<::phantom::lang::SetClass *() const, virtual_>("asSetClass", &_::asSetClass)
            .method<::phantom::lang::StringClass *() const, virtual_>("asStringClass", &_::asStringClass)
            /// missing symbol(s) reflection (phantom::lang::ArrayClass) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<::phantom::lang::ArrayClass *() const, virtual_>("asArrayClass", &_::asArrayClass)
            .method<::phantom::lang::Constant *() const, virtual_>("asConstant", &_::asConstant)
            .method<::phantom::lang::ConstType *() const, virtual_>("asConstClass", &_::asConstClass)
            .method<::phantom::lang::Pointer *() const, virtual_>("asConstClassPointer", &_::asConstClassPointer)
            .method<::phantom::lang::LValueReference *() const, virtual_>("asConstClassLValueReference", &_::asConstClassLValueReference)
            .method<::phantom::lang::Type *() const, virtual_>("asConstClassAddressType", &_::asConstClassAddressType)
            .method<::phantom::lang::Constructor *() const, virtual_>("asConstructor", &_::asConstructor)
            .method<::phantom::lang::ConstType *() const, virtual_>("asConstType", &_::asConstType)
            .method<Reference*() const>("asConstLValueReference", &_::asConstLValueReference)
            .method<::phantom::lang::ConstVolatileType *() const, virtual_>("asConstVolatileType", &_::asConstVolatileType)
            .method<::phantom::lang::ContainerClass *() const, virtual_>("asContainerClass", &_::asContainerClass)
            .method<::phantom::lang::DataElement *() const, virtual_>("asDataElement", &_::asDataElement)
            .method<::phantom::lang::FieldPointer *() const, virtual_>("asFieldPointer", &_::asFieldPointer)
            .method<::phantom::lang::Pointer *() const, virtual_>("asPointer", &_::asPointer)
            .method<::phantom::lang::Destructor *() const, virtual_>("asDestructor", &_::asDestructor)
            .method<::phantom::lang::Ellipsis *() const, virtual_>("asEllipsis", &_::asEllipsis)
            .method<::phantom::lang::Enum *() const, virtual_>("asEnum", &_::asEnum)
            /// missing symbol(s) reflection (phantom::lang::Evaluable) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<::phantom::lang::Evaluable *() const, virtual_>("asEvaluable", &_::asEvaluable)
            /// missing symbol(s) reflection (phantom::lang::Expression) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<::phantom::lang::Expression *() const, virtual_>("asExpression", &_::asExpression)
            .method<::phantom::lang::Field *() const, virtual_>("asField", &_::asField)
            .method<::phantom::lang::PrimitiveType *() const, virtual_>("asFloatingPointType", &_::asFloatingPointType)
            .method<::phantom::lang::Function *() const, virtual_>("asFunction", &_::asFunction)
            .method<::phantom::lang::FunctionPointer *() const, virtual_>("asFunctionPointer", &_::asFunctionPointer)
            .method<::phantom::lang::FunctionType *() const, virtual_>("asFunctionType", &_::asFunctionType)
            .method<::phantom::lang::PrimitiveType *() const, virtual_>("asFundamentalType", &_::asFundamentalType)
            .method<::phantom::lang::InitializerListType *() const, virtual_>("asInitializerListType", &_::asInitializerListType)
            .method<::phantom::lang::PrimitiveType *() const, virtual_>("asIntegralType", &_::asIntegralType)
            .method<::phantom::lang::LanguageElement *() const>("asLanguageElement", &_::asLanguageElement)
            .method<::phantom::lang::LocalVariable *() const, virtual_>("asLocalVariable", &_::asLocalVariable)
            .method<::phantom::lang::LValueReference *() const, virtual_>("asLValueReference", &_::asLValueReference)
            .method<::phantom::lang::MapClass *() const, virtual_>("asMapContainerClass", &_::asMapContainerClass)
            .method<::phantom::lang::MemberAnonymousSection *() const, virtual_>("asMemberAnonymousSection", &_::asMemberAnonymousSection)
            .method<::phantom::lang::MemberAnonymousStruct *() const, virtual_>("asMemberAnonymousStruct", &_::asMemberAnonymousStruct)
            .method<::phantom::lang::MemberAnonymousUnion *() const, virtual_>("asMemberAnonymousUnion", &_::asMemberAnonymousUnion)
            .method<::phantom::lang::MethodPointer *() const, virtual_>("asMethodPointer", &_::asMethodPointer)
            .method<::phantom::lang::MemberPointer *() const, virtual_>("asMemberPointer", &_::asMemberPointer)
            .method<::phantom::lang::Method *() const, virtual_>("asMethod", &_::asMethod)
            .method<::phantom::lang::Module *() const, virtual_>("asModule", &_::asModule)
            .method<::phantom::lang::Namespace *() const, virtual_>("asNamespace", &_::asNamespace)
            .method<::phantom::lang::Type *() const, virtual_>("asNullptrType", &_::asNullptrType)
            .method<::phantom::lang::Package *() const, virtual_>("asPackage", &_::asPackage)
            .method<::phantom::lang::PackageFolder *() const, virtual_>("asPackageFolder", &_::asPackageFolder)
            .method<::phantom::lang::Parameter *() const, virtual_>("asParameter", &_::asParameter)
            .method<::phantom::lang::Placeholder *() const, virtual_>("asPlaceholder", &_::asPlaceholder)
            .method<::phantom::lang::Type *() const, virtual_>("asPOD", &_::asPOD)
            .method<::phantom::lang::PointerType *() const, virtual_>("asPointerType", &_::asPointerType)
            .method<::phantom::lang::PrimitiveType *() const, virtual_>("asPrimitiveType", &_::asPrimitiveType)
            .method<::phantom::lang::Property *() const, virtual_>("asProperty", &_::asProperty)
            .method<::phantom::lang::Reference *() const, virtual_>("asReference", &_::asReference)
            .method<::phantom::lang::RValueReference *() const, virtual_>("asRValueReference", &_::asRValueReference)
            .method<::phantom::lang::Scope *() const, virtual_>("asScope", &_::asScope)
            .method<::phantom::lang::SequentialContainerClass *() const, virtual_>("asSequentialContainerClass", &_::asSequentialContainerClass)
            .method<::phantom::lang::SetClass *() const, virtual_>("asSetContainerClass", &_::asSetContainerClass)
            /// missing symbol(s) reflection (phantom::lang::Signal) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<::phantom::lang::Signal *() const, virtual_>("asSignal", &_::asSignal)
            .method<::phantom::lang::Signature *() const, virtual_>("asSignature", &_::asSignature)
            .method<::phantom::lang::Method *() const, virtual_>("asSlot", &_::asSlot)
            .method<::phantom::lang::Source *() const, virtual_>("asSource", &_::asSource)
            /// missing symbol(s) reflection (phantom::lang::Statement) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<::phantom::lang::Statement *() const, virtual_>("asStatement", &_::asStatement)
            .method<StaticField*() const, virtual_>("asStaticField", &_::asStaticField)
            .method<StaticMethod*() const, virtual_>("asStaticMethod", &_::asStaticMethod)
            .method<::phantom::lang::Structure *() const, virtual_>("asStructure", &_::asStructure)
            .method<::phantom::lang::Subroutine *() const, virtual_>("asSubroutine", &_::asSubroutine)
            .method<::phantom::lang::Template *() const, virtual_>("asTemplate", &_::asTemplate)
            .method<::phantom::lang::TemplateParameter *() const, virtual_>("asTemplateParameter", &_::asTemplateParameter)
            .method<::phantom::lang::TemplateSignature *() const, virtual_>("asTemplateSignature", &_::asTemplateSignature)
            .method<::phantom::lang::TemplateSpecialization *() const, virtual_>("asTemplateSpecialization", &_::asTemplateSpecialization)
            .method<::phantom::lang::Type *() const, virtual_>("asType", &_::asType)
            .method<::phantom::lang::Union *() const, virtual_>("asUnion", &_::asUnion)
            .method<::phantom::lang::ValueMember *() const, virtual_>("asValueMember", &_::asValueMember)
            .method<::phantom::lang::Variable *() const, virtual_>("asVariable", &_::asVariable)
            .method<::phantom::lang::VirtualMethodTable *() const, virtual_>("asVirtualMethodTable", &_::asVirtualMethodTable)
            .method<::phantom::lang::VolatileType *() const, virtual_>("asVolatileType", &_::asVolatileType)
            .method<::phantom::lang::Type *() const, virtual_>("toType", &_::toType)
            .method<bool(Symbol*) const, virtual_>("hasFriend", &_::hasFriend)
            .method<bool(LanguageElement*) const>("hasElement", &_::hasElement)
            .method<bool(Symbol*) const>("hasFriendCascade", &_::hasFriendCascade)
            .method<bool(LanguageElement*) const>("hasOwnerCascade", &_::hasOwnerCascade)
            .method<bool(LanguageElement*) const>("hasElementCascade", &_::hasElementCascade)
            .method<bool() const, virtual_>("isCompileTime", &_::isCompileTime)
            .method<::phantom::lang::LanguageElement *() const>("getOwner", &_::getOwner)
            .method<LanguageElement*() const>("getRootElement", &_::getRootElement)
            .method<void(LanguageElements&, Class*) const>("fetchElementsDeep", &_::fetchElementsDeep)["nullptr"]
            .method<void(LanguageElements&, Class*) const>("fetchElements", &_::fetchElements)["nullptr"]
            .method<void(StringView, Symbols&) const>("getSymbolsWithName", &_::getSymbolsWithName)
            .method<void(Symbols&) const>("getSymbols", &_::getSymbols)
            .method<bool(StringView) const>("hasSymbol", &_::hasSymbol)
            .method<Module*() const>("getModule", &_::getModule)
            .method<Package*() const>("getPackage", &_::getPackage)
            .method<::phantom::lang::Source *() const>("getSource", &_::getSource)
            .method<bool() const, virtual_>("canBeDestroyed", &_::canBeDestroyed)
            .method<bool() const, virtual_>("canBeUnloaded", &_::canBeUnloaded)
            .method<void() const, virtual_>("checkCompleteness", &_::checkCompleteness)
            .method<bool(LanguageElement*) const, virtual_>("isSame", &_::isSame)
            .method<::phantom::lang::LanguageElement *(), virtual_>("hatchExpression", &_::hatchExpression)
            .method<::phantom::lang::LanguageElement *() const, virtual_>("removeExpression", &_::removeExpression)
            .method<Symbol*() const>("removeExpressionAsSymbol", &_::removeExpressionAsSymbol)
            .method<ClassType*() const>("getEnclosingClassType", &_::getEnclosingClassType)
            /// missing symbol(s) reflection (phantom::lang::Statement) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Statement*() const>("getEnclosingStatement", &_::getEnclosingStatement)
            /// missing symbol(s) reflection (phantom::lang::Block) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Block*() const>("getEnclosingBlock", &_::getEnclosingBlock)
            .method<Scope*() const>("getEnclosingScope", &_::getEnclosingScope)
            .method<Namespace*() const>("getEnclosingNamespace", &_::getEnclosingNamespace)
            .method<Subroutine*() const>("getEnclosingSubroutine", &_::getEnclosingSubroutine)
            .method<TemplateSpecialization*() const>("getEnclosingTemplateSpecialization", &_::getEnclosingTemplateSpecialization)
            .method<bool(LanguageElement*, size_t&, PlaceholderMap&) const, virtual_>("partialAccepts", &_::partialAccepts)
            .method<void(LanguageElements&) const>("getElements", &_::getElements)
            .method<void(Symbols&) const>("fetchSymbols", &_::fetchSymbols)
            .method<void(Symbols&, SymbolFilter, bool, bool) const>("fetchSymbols", &_::fetchSymbols)["true"]["false"]
            .method<void(LanguageElements&) const>("getElementsDeep", &_::getElementsDeep)
            .method<Symbol*(StringView, Modifiers, uint) const>("getUniqueElement", &_::getUniqueElement)["0"]["0"]
            .method<size_t(LanguageElement*) const>("getElementIndex", &_::getElementIndex)
            /// missing symbol(s) reflection (std::basic_ostream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::basic_ostream<char> &) const>("dumpElementList", &_::dumpElementList)
            /// missing symbol(s) reflection (std::basic_ostream) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(::std::basic_ostream<char> &) const>("dumpElementListCascade", &_::dumpElementListCascade)
            .method<String() const, virtual_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<String(LanguageElement*) const, virtual_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)
            .method<String() const, virtual_>("getQualifiedName", &_::getQualifiedName)
            .method<String() const, virtual_>("getDecoratedName", &_::getDecoratedName)
            .method<String() const>("getName", &_::getName)
            .method<String() const>("getUniqueName", &_::getUniqueName)
            .method<void(StringBuffer&) const, virtual_>("getName", &_::getName)
            .method<void(StringBuffer&) const, virtual_>("getQualifiedName", &_::getQualifiedName)
            .method<void(LanguageElement*, StringBuffer&) const, virtual_>("getRelativeName", &_::getRelativeName)
            .method<void(StringBuffer&) const, virtual_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(LanguageElement*, StringBuffer&) const, virtual_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)
            .method<void(StringBuffer&) const, virtual_>("getUniqueName", &_::getUniqueName)
            .method<::phantom::lang::LanguageElement *() const, virtual_>("getNamingScope", &_::getNamingScope)
            .method<bool(LanguageElement*) const>("hasNamingScopeCascade", &_::hasNamingScopeCascade)
            .method<void()>("detach", &_::detach)
            .method<bool(Symbol*) const, virtual_>("isSymbolHidden", &_::isSymbolHidden)
            .method<uint() const>("getFlags", &_::getFlags)
            .method<void(uint)>("setFlag", &_::setFlag)
            .method<void(uint)>("addFlags", &_::addFlags)
            .method<void(uint)>("removeFlags", &_::removeFlags)
            .method<void(uint)>("setFlags", &_::setFlags)
            .method<bool(uint) const>("testFlags", &_::testFlags)
            .method<bool() const>("isNative", &_::isNative)
            .method<bool() const>("isTemplateDependant", &_::isTemplateDependant)
            .method<void()>("setTemplateDependant", &_::setTemplateDependant)
            .method<bool() const>("isAlwaysValid", &_::isAlwaysValid)
            .method<bool() const, virtual_>("isTemplateInstance", &_::isTemplateInstance)
            .method<void()>("setShared", &_::setShared)
            .method<bool() const>("isShared", &_::isShared)
            .method<void(const CodePosition&)>("setCodePosition", &_::setCodePosition)
            .method<const ::phantom::lang::CodeRange &() const>("getCodeRange", &_::getCodeRange)
            .method<void(const CodeRange&)>("setCodeRange", &_::setCodeRange)
            .method<const ::phantom::lang::CodePosition &() const>("getCodePosition", &_::getCodePosition)
            .method<CodeLocation() const>("getCodeLocation", &_::getCodeLocation)
            .method<CodeRangeLocation() const>("getCodeRangeLocation", &_::getCodeRangeLocation)
            .method<::phantom::lang::Source *() const, virtual_>("getCodeLocationSource", &_::getCodeLocationSource)
            .method<LanguageElement*(const CodePosition&) const>("getElementAtCodePosition", &_::getElementAtCodePosition)
            .method<LanguageElement*(uint16) const>("getElementAtLine", &_::getElementAtLine)
            .method<int() const, virtual_>("destructionPriority", &_::destructionPriority)
            .method<void(LanguageElement*)>("setOwner", &_::setOwner)
            .method<void(LanguageElement*)>("addReferencedElement", &_::addReferencedElement)
            .method<void(LanguageElement*)>("removeReferencedElement", &_::removeReferencedElement)
        
        .protected_()
            .constructor<void(uint)>()["0"]
        
        .protected_()
            .method<void(), virtual_>("onInvalidated", &_::onInvalidated)
            .method<void(), virtual_>("onElementsAccess", &_::onElementsAccess)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("LanguageElement")
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
