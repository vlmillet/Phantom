#pragma once

// haunt {

// clang-format off

#include "Symbol.h"

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
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/typedef>
#include <phantom/using>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/SmallMap.hxx>
#include <phantom/utils/SmallSet.hxx>
#include <phantom/utils/SmallString.hxx>
#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Symbol")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<MetaDatas>("MetaDatas"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<Annotations>("Annotations"); }
        PHANTOM_CLASS(SymbolExtension)
        {
            this_()
            .inherits<::phantom::lang::LanguageElement>()
        
        .protected_()
            .constructor<void(), default_>()
            .constructor<void(uint)>()
        
        .public_()
            .method<Symbol*() const>("getSymbol", &_::getSymbol)
            ;
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<SymbolExtensions>("SymbolExtensions"); }
        PHANTOM_CLASS(Symbol)
        {
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringBuffer = typedef_< phantom::StringBuffer>;
            using StringView = typedef_< phantom::StringView>;
            using Symbols = typedef_< phantom::lang::Symbols>;
            this_()
            .inherits<::phantom::lang::LanguageElement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .staticMethod<bool(StringView)>("IsCppIdentifier", &_::IsCppIdentifier)
            .staticMethod<hash64(const char*, size_t)>("ComputeHash", &_::ComputeHash)
            .staticMethod<void(hash64&, hash64)>("CombineHash", &_::CombineHash)
        
        .public_()
            .constructor<void(Modifiers, uint)>()["0"]["0"]
        
        .protected_()
            .constructor<void(StringView, Modifiers, uint)>()["0"]["0"]
        
        .public_()
            .method<void(), virtual_|override_>("terminate", &_::terminate)
            .method<int() const, virtual_|override_>("destructionPriority", &_::destructionPriority)
        
        .public_()
            .method<Type*() const, virtual_|override_>("toType", &_::toType)
            .method<::phantom::lang::Namespace *() const, virtual_>("toNamespace", &_::toNamespace)
            .method<void(Access)>("setAccess", &_::setAccess)
            .method<Access() const>("getAccess", &_::getAccess)
            .method<void(Visibility)>("setVisibility", &_::setVisibility)
            .method<Visibility() const>("getVisibility", &_::getVisibility)
            .method<void(Modifiers)>("setModifiers", &_::setModifiers)
            .method<void(Modifiers)>("addModifiers", &_::addModifiers)
            .method<void(Modifiers)>("removeModifiers", &_::removeModifiers)
            .method<Modifiers() const>("getModifiers", &_::getModifiers)
            .method<bool(Modifiers) const>("testModifiers", &_::testModifiers)
            .method<StringView() const>("getName", &_::getName)
            .method<hash64() const>("getHash", &_::getHash)
            .method<hash64() const>("getLocalHash", &_::getLocalHash)
            .method<hash64(LanguageElement*) const>("getRelativeHash", &_::getRelativeHash)
            .method<bool(StringView) const>("hasElementWithName", &_::hasElementWithName)
            .method<bool() const, virtual_>("isPOD", &_::isPOD)
            .method<bool() const>("isStatic", &_::isStatic)
            .method<bool() const>("isProtected", &_::isProtected)
            .method<bool() const>("isPrivate", &_::isPrivate)
            .method<bool() const>("isPublic", &_::isPublic)
            .method<bool() const>("isAbstract", &_::isAbstract)
            .method<bool() const>("isFinal", &_::isFinal)
            .method<bool() const>("isPureVirtual", &_::isPureVirtual)
            .method<bool() const>("isVirtual", &_::isVirtual)
            .method<bool() const>("isConst", &_::isConst)
            .method<bool() const>("isVolatile", &_::isVolatile)
            .method<bool() const>("isConstVolatile", &_::isConstVolatile)
            .method<bool() const>("isQualified", &_::isQualified)
            .method<bool() const>("isMutable", &_::isMutable)
            .method<bool() const>("isImportable", &_::isImportable)
            .method<bool() const>("isTransient", &_::isTransient)
            .method<bool(Symbol*) const, virtual_>("hasPublicMember", &_::hasPublicMember)
            .method<bool(Symbol*) const, virtual_>("hasProtectedMember", &_::hasProtectedMember)
            .method<bool(Symbol*) const, virtual_>("hasPrivateMember", &_::hasPrivateMember)
            .method<void(const MetaDatas&)>("setMetaDatas", &_::setMetaDatas)
            .method<void(MetaDatas&&)>("setMetaDatas", &_::setMetaDatas)
            .method<void(const MetaDatas&)>("addMetaDatas", &_::addMetaDatas)
            .method<void(StringView, const Variant&)>("setMetaData", &_::setMetaData)
            .method<void(StringView, Variant&&)>("setMetaData", &_::setMetaData)
            .method<void(StringWithHash, const Variant&)>("setMetaData", &_::setMetaData)
            .method<void(StringWithHash, Variant&&)>("setMetaData", &_::setMetaData)
            .method<void(StringView)>("removeMetaData", &_::removeMetaData)
            .method<void(StringWithHash)>("removeMetaData", &_::removeMetaData)
            .method<const ::phantom::Variant &(StringView) const>("getMetaData", &_::getMetaData)
            .method<const ::phantom::Variant &(StringWithHash) const>("getMetaData", &_::getMetaData)
            .method<bool(StringView) const>("hasMetaData", &_::hasMetaData)
            .method<bool(StringWithHash) const>("hasMetaData", &_::hasMetaData)
            .method<const MetaDatas&() const>("getMetaDatas", &_::getMetaDatas)
            .method<bool(StringView) const>("hasAnnotation", &_::hasAnnotation)
            .method<bool(StringView)>("addAnnotation", &_::addAnnotation)
            .method<bool(StringView)>("removeAnnotation", &_::removeAnnotation)
            .method<const Annotations&() const>("getAnnotations", &_::getAnnotations)
            .method<SymbolExtension*(Class*, size_t) const>("getExtension", &_::getExtension)["0"]
            .method<const SymbolExtensions&() const>("getExtensions", &_::getExtensions)
            .method<void(const SymbolExtensions&)>("addExtensions", &_::addExtensions)
            .method<void(SymbolExtension*)>("addExtension", &_::addExtension)
            .method<void(Symbols&) const>("getDoubles", &_::getDoubles)
            .method<bool(LanguageElement*) const, virtual_|override_>("isSame", &_::isSame)
            .method<bool(Symbol*) const, virtual_>("isSame", &_::isSame)
            .method<LanguageElement*() const, virtual_|override_>("getNamingScope", &_::getNamingScope)
            .method<Namespace*() const>("getNamespace", &_::getNamespace)
            .method<void(StringBuffer&) const>("getUniqueTemplateDecoration", &_::getUniqueTemplateDecoration)
            .method<void(StringBuffer&) const>("getTemplateDecoration", &_::getTemplateDecoration)
            .method<hash64() const>("getTemplateDecorationHash", &_::getTemplateDecorationHash)
            .method<void(StringBuffer&) const>("getTemplateQualifiedDecoration", &_::getTemplateQualifiedDecoration)
            .method<TemplateSpecialization*() const>("getTemplateSpecialization", &_::getTemplateSpecialization)
            .method<void(UserData&&)>("setUserData", &_::setUserData)
            .using_("LanguageElement::getQualifiedName")
            .using_("LanguageElement::getDecoratedName")
            .using_("LanguageElement::getQualifiedDecoratedName")
            .using_("LanguageElement::getRelativeDecoratedName")
            .using_("LanguageElement::getRelativeName")
            .using_("LanguageElement::getUniqueName")
            .method<void(StringBuffer&) const, virtual_|final_|override_>("getName", &_::getName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedName", &_::getQualifiedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getDecoratedName", &_::getDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getQualifiedDecoratedName", &_::getQualifiedDecoratedName)
            .method<void(LanguageElement*, StringBuffer&) const, virtual_>("getRelativeName", &_::getRelativeName)
            .method<void(LanguageElement*, StringBuffer&) const, virtual_|override_>("getRelativeDecoratedName", &_::getRelativeDecoratedName)
            .method<void(StringBuffer&) const, virtual_|override_>("getUniqueName", &_::getUniqueName)
        
        .protected_()
            .method<void(StringView)>("setName", &_::setName)
            .method<void(bool)>("setImportable", &_::setImportable)
            .method<hash64() const, virtual_>("computeHash", &_::computeHash)
            .method<hash64() const, virtual_>("computeLocalHash", &_::computeLocalHash)
            .method<void(StringBuffer&) const, virtual_>("formatAnonymousName", &_::formatAnonymousName)
            .method<void(Namespace*), virtual_>("onNamespaceChanging", &_::onNamespaceChanging)
            .method<void(Namespace*), virtual_>("onNamespaceChanged", &_::onNamespaceChanged)
            .method<void(Visibility), virtual_>("onVisibilityChanging", &_::onVisibilityChanging)
            .method<void(Visibility), virtual_>("onVisibilityChanged", &_::onVisibilityChanged)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Symbol")
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
