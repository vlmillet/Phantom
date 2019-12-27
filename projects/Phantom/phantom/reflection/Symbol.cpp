// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
#include "Symbol.h"

#include "Application.h"
#include "Module.h"
#include "Namespace.h"
#include "Template.h"
#include "TemplateSpecialization.h"
#include "phantom/detail/new.h"

#include <locale>
#include <phantom/utils/StringHash.h>
#include <phantom/utils/Variant.h>
/* *********************************************** */
namespace phantom
{
namespace reflection
{
static Symbols          empty_elements;
static MetaDatas        empty_metas;
static Annotations      empty_annotations;
static SymbolExtensions empty_extensions;

bool Symbol::IsCppIdentifier(StringView a_Name)
{
    if (a_Name.empty())
        return false;
    char first = a_Name[0];
    if (!std::isalpha(first, std::locale::classic()) && first != '_')
        return false;
    for (size_t i = 1; i < a_Name.size(); ++i)
    {
        char c = a_Name[i];
        if (!std::isalnum(c, std::locale::classic()) && c != '_')
            return false;
    }
    return true;
}

Symbol::Symbol(Modifiers a_Modifiers /* = 0*/, uint a_uiFlags /*=0*/)
    : LanguageElement(a_uiFlags), m_Modifiers(a_Modifiers)
{
    Register(this);
}

Symbol::Symbol(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : LanguageElement(a_uiFlags), m_strName(a_strName), m_Modifiers(a_Modifiers)
{
    PHANTOM_ASSERT(NOT(isProtected() AND isPrivate()), "o_private_access and o_protected_access cannot co-exist");
    Register(this);
}

Symbol::~Symbol()
{
}

void Symbol::terminate()
{
    if (m_pMetaDatas != nullptr)
    {
        PHANTOM_DELETE(MetaDatas) m_pMetaDatas;
        m_pMetaDatas = nullptr;
    }
    if (m_pAnnotations != nullptr)
    {
        PHANTOM_DELETE(Annotations) m_pAnnotations;
        m_pAnnotations = nullptr;
    }
    LanguageElement::terminate();
}

int Symbol::destructionPriority() const
{
    int prio = LanguageElement::destructionPriority();

    if (m_pMetaDatas != nullptr)
    {
        for (MetaDatas::value_type& pair : *m_pMetaDatas)
        {
            int typePrio = pair.second.type()->destructionPriority() - 1;
            if (typePrio < prio)
                prio = typePrio;
        }
    }
    return prio;
}

phantom::hash64 Symbol::computeHash() const
{
    StringBuffer buffer;
    getQualifiedDecoratedName(buffer);
    return ComputeHash(buffer.c_str(), buffer.size());
}

hash64 Symbol::ComputeHash(const char* a_Str, size_t a_Len)
{
    return makeStringHash(StringView(a_Str, a_Len));
}

void Symbol::onElementRemoved(LanguageElement* a_pElement)
{
    if (m_pExtensions)
    {
        auto found = std::find(m_pExtensions->begin(), m_pExtensions->end(), a_pElement);
        if (found != m_pExtensions->end())
        {
            m_pExtensions->erase(found);
            if (m_pExtensions->empty())
            {
                PHANTOM_DELETE(SymbolExtensions) m_pExtensions;
                m_pExtensions = nullptr;
            }
        }
    }
}

void Symbol::formatAnonymousName(StringBuffer& a_Buf) const
{
    char buf[64];
    snprintf(buf, 64, "__anonymous_%zx", (size_t)this);
    a_Buf += buf;
}

void Symbol::onAncestorChanged(LanguageElement* a_pAncestor)
{
    LanguageElement::onAncestorChanged(a_pAncestor);
    if (!isNative())
    {
        m_Hash = _computeHash();
    }
}

hash64 Symbol::_computeHash() const
{
    hash64 hash = computeHash();
    return hash;
}

const Variant& Symbol::getMetaData(StringView a_Name) const
{
    return getMetaData(StringHash(a_Name));
}

const Variant& Symbol::getMetaData(StringHash a_Name) const
{
    static Variant null;
    if (m_pMetaDatas == nullptr)
        return null;
    auto found = m_pMetaDatas->find(a_Name);
    return found == m_pMetaDatas->end() ? null : found->second;
}

bool Symbol::hasMetaData(StringHash a_Hash) const
{
    if (m_pMetaDatas == nullptr)
        return false;
    return m_pMetaDatas->find(a_Hash) != m_pMetaDatas->end();
}

bool Symbol::hasMetaData(StringView a_strName) const
{
    return hasMetaData(StringHash(a_strName));
}

const MetaDatas& Symbol::getMetaDatas() const
{
    return m_pMetaDatas ? *m_pMetaDatas : empty_metas;
}

bool Symbol::hasAnnotation(StringView a_strName) const
{
    return (m_pAnnotations AND m_pAnnotations->find(a_strName) != m_pAnnotations->end());
    if (m_pAnnotations == nullptr)
        return false;
    return m_pAnnotations->find(a_strName) != m_pAnnotations->end();
}

bool Symbol::addAnnotation(StringView a_strName)
{
    if (m_pAnnotations == nullptr)
        m_pAnnotations = PHANTOM_NEW(Annotations);
    return m_pAnnotations->insert(a_strName).second;
}

bool Symbol::removeAnnotation(StringView a_strName)
{
    if (m_pAnnotations AND m_pAnnotations->erase(a_strName) == 1)
    {
        if (m_pAnnotations->empty())
        {
            PHANTOM_DELETE(Annotations) m_pAnnotations;
            m_pAnnotations = nullptr;
        }
        return true;
    }
    return false;
}

const Annotations& Symbol::getAnnotations() const
{
    return m_pAnnotations ? *m_pAnnotations : empty_annotations;
}

SymbolExtension* Symbol::getExtension(Class* a_pClass, size_t a_Num /* = 0*/) const
{
    size_t c = 0;
    for (auto ext : getExtensions())
        if (ext->RTTI.metaClass->isA(a_pClass))
            if (c++ == a_Num)
                return ext;
    return nullptr;
}

const SymbolExtensions& Symbol::getExtensions() const
{
    const_cast<Symbol*>(this)->onElementsAccess();
    return m_pExtensions ? *m_pExtensions : empty_extensions;
}

void Symbol::addExtensions(const SymbolExtensions& a_Extensions)
{
    for (auto ext : a_Extensions)
        addExtension(ext);
}

void Symbol::addExtension(SymbolExtension* a_pExtension)
{
    if (m_pExtensions == nullptr)
        m_pExtensions = PHANTOM_NEW(SymbolExtensions);
    PHANTOM_ASSERT(std::find(m_pExtensions->begin(), m_pExtensions->end(), a_pExtension) == m_pExtensions->end());
    m_pExtensions->push_back(a_pExtension);
    addElement(a_pExtension);
}

void Symbol::removeExtension(SymbolExtension* a_pExtension)
{
    removeElement(a_pExtension);
}

void Symbol::setMetaData(StringHash a_Hash, const Variant& a_Value)
{
    (*m_pMetaDatas)[StringHash(a_Hash)] = a_Value;
}

void Symbol::setMetaData(StringHash a_Hash, Variant&& a_Value)
{
    (*m_pMetaDatas)[StringHash(a_Hash)] = std::move(a_Value);
}

void Symbol::setMetaData(StringView a_Name, const Variant& a_Value)
{
    size_t dotPos = a_Name.find_last_of('.');
    if (dotPos == String::npos)
    {
        PHANTOM_ASSERT(Symbol::IsCppIdentifier(a_Name), "meta '%.*s' is not an identifier name matching [a-zA-Z0-9_]+",
                       PHANTOM_STRING_AS_PRINTF_ARG(a_Name));
        if (m_pMetaDatas == nullptr)
            m_pMetaDatas = PHANTOM_NEW(MetaDatas);
        (*m_pMetaDatas)[StringHash(a_Name)] = a_Value;
    }
    else
    {
        String  symbol = a_Name.substr(0, dotPos);
        Symbol* pSymbol = Application::Get()->findSymbol(symbol, this);
        PHANTOM_ASSERT(pSymbol, "symbol not found '%s'", symbol.c_str());
        pSymbol->setMetaData(a_Name.substr(dotPos + 1), a_Value);
    }
}

void Symbol::setMetaData(StringView a_Name, Variant&& a_Value)
{
    if (m_pMetaDatas == nullptr)
        m_pMetaDatas = PHANTOM_NEW(MetaDatas);
    (*m_pMetaDatas)[StringHash(a_Name)] = (Variant &&) a_Value;
}

void Symbol::removeMetaData(StringHash a_NameHash)
{
    PHANTOM_ASSERT(m_pMetaDatas);
    auto found = m_pMetaDatas->find(a_NameHash);
    PHANTOM_ASSERT(found != m_pMetaDatas->end());
    m_pMetaDatas->erase(found);
    if (m_pMetaDatas->empty())
    {
        PHANTOM_DELETE(MetaDatas) m_pMetaDatas;
        m_pMetaDatas = nullptr;
    }
}

void Symbol::removeMetaData(StringView a_Name)
{
    removeMetaData(StringHash(a_Name));
}

void Symbol::setAccess(Access a_eAccess)
{
    m_eAccess = a_eAccess;
}

void Symbol::setModifiers(Modifiers a_Modifiers)
{
    m_Modifiers = a_Modifiers;
    PHANTOM_ASSERT(NOT(isPublic() AND isProtected()), "o_public_access and o_protected_access cannot co-exist");
}

void Symbol::addModifiers(Modifiers a_Modifiers)
{
    m_Modifiers |= a_Modifiers;
}

void Symbol::removeModifiers(Modifiers a_Modifiers)
{
    m_Modifiers &= ~a_Modifiers;
}

bool Symbol::isSame(LanguageElement* a_pOther) const
{
    if (this == a_pOther)
        return true;
    if (a_pOther->asSymbol() == nullptr)
        return false;
    return isSame(static_cast<Symbol*>(a_pOther));
}

bool Symbol::isSame(Symbol* a_pOther) const
{
    return (this == a_pOther OR(isNative() AND getHash() == a_pOther->getHash()));
}

bool Symbol::hasElementWithName(StringView a_strName) const
{
    if (m_pElements)
    {
        for (auto it = m_pElements->begin(); it != m_pElements->end(); ++it)
        {
            Symbol* pSymbol = (*it)->asSymbol();
            if (pSymbol AND pSymbol->getName() == a_strName)
                return true;
        }
    }
    return false;
}

Scope* Symbol::getNamingScope() const
{
    if (m_pNamespace)
        return m_pNamespace;
    if (auto pOwner = getOwner())
    {
        if (pOwner->asSource() == nullptr)
        {
            if (TemplateSpecialization* pSpec = pOwner->asTemplateSpecialization())
            {
                return pSpec->getTemplate()->getNamingScope();
            }
            return pOwner->asScope();
        }
    }
    return nullptr;
}

Namespace* Symbol::getNamespace() const
{
    return m_pNamespace;
}

void Symbol::getDoubles(Symbols& out) const
{
    Scope* pNamingScope = getNamingScope();
    if (pNamingScope)
    {
        pNamingScope->getElementDoubles(const_cast<Symbol*>(this), out);
    }
}

bool Symbol::hasPublicMember(Symbol* a_pSymbol) const
{
    return hasElement(a_pSymbol) AND a_pSymbol->isPublic();
}

bool Symbol::hasProtectedMember(Symbol* a_pSymbol) const
{
    return hasElement(a_pSymbol) AND a_pSymbol->isProtected();
}

bool Symbol::hasPrivateMember(Symbol* a_pSymbol) const
{
    return hasElement(a_pSymbol) AND a_pSymbol->isPrivate();
}

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wtautological-undefined-compare"
#endif // __clang__

void Symbol::setMetaDatas(MetaDatas&& a_MetaDatas)
{
    if (&a_MetaDatas == nullptr)
        return;
    if (m_pMetaDatas == nullptr)
    {
        m_pMetaDatas = PHANTOM_NEW(MetaDatas)(std::move(a_MetaDatas));
    }
    else
    {
        *m_pMetaDatas = std::move(a_MetaDatas);
    }
}

void Symbol::setMetaDatas(const MetaDatas& a_MetaDatas)
{
    if (&a_MetaDatas == nullptr)
        return;
    if (m_pMetaDatas == nullptr)
    {
        m_pMetaDatas = PHANTOM_NEW(MetaDatas)(a_MetaDatas);
    }
    else
    {
        *m_pMetaDatas = a_MetaDatas;
    }
}

void Symbol::addMetaDatas(const MetaDatas& a_MetaDatas)
{
    if (&a_MetaDatas == nullptr)
        return;
    for (auto it = a_MetaDatas.begin(); it != a_MetaDatas.end(); ++it)
    {
        setMetaData(it->first, it->second);
    }
}

#ifdef __clang__
#    pragma clang diagnostic pop
#endif // __clang__

void Symbol::getUniqueName(StringBuffer& a_Buf) const
{
    if (getOwner() == nullptr)
    {
        if (m_strName.size())
            getName(a_Buf);
        else
            formatAnonymousName(a_Buf);
        getUniqueTemplateDecoration(a_Buf);
        return;
    }
    /// if template specialization, use the original template instead of the specialization
    LanguageElement* pOwner = getOwner()->asTemplateSpecialization()
    ? static_cast<TemplateSpecialization*>(getOwner())->getTemplate()->getOwner()
    : getOwner();
    if (pOwner == nullptr)
    {
        if (m_strName.size())
            getName(a_Buf);
        else
            formatAnonymousName(a_Buf);
        getUniqueTemplateDecoration(a_Buf);
        return;
    }
    size_t prev = a_Buf.size();
    pOwner->getUniqueName(a_Buf);
    bool ownerEmpty = (a_Buf.size() - prev) == 0;
    if (!ownerEmpty) // no owner name
        a_Buf += '.';
    if (m_strName.size())
        getName(a_Buf);
    else
        formatAnonymousName(a_Buf);
    getUniqueTemplateDecoration(a_Buf);
}

void Symbol::getName(StringBuffer& a_Buf) const
{
    a_Buf += m_strName;
}

void Symbol::getQualifiedName(StringBuffer& a_Buf) const
{
    Scope* pNamingScope = getNamingScope();
    if (TemplateSpecialization* pSpec = getTemplateSpecialization())
    {
        return pSpec->getTemplate()->getQualifiedName(a_Buf);
    }
    LanguageElement* pNS = pNamingScope ? pNamingScope->asLanguageElement() : nullptr;
    if (pNS == nullptr)
    {
        if (m_strName.size())
            getName(a_Buf);
        else
            formatAnonymousName(a_Buf);
        return;
    }
    size_t prev = a_Buf.size();
    pNS->getQualifiedName(a_Buf);
    bool ownerEmpty = (a_Buf.size() - prev) == 0;
    if (!ownerEmpty) // no owner name
    {
        a_Buf += ':';
        a_Buf += ':';
    }
    if (m_strName.size())
        getName(a_Buf);
    else
        formatAnonymousName(a_Buf);
}

void Symbol::getQualifiedDecoratedName(StringBuffer& a_Buf) const
{
    getQualifiedName(a_Buf);
    getTemplateQualifiedDecoration(a_Buf);
}

void Symbol::setName(StringView a_strName)
{
    PHANTOM_ASSERT(getOwner() == nullptr,
                   "changing symbol name while attached to an owner leads to undefined behavior");
    m_strName = a_strName;
}

void Symbol::getDecoratedName(StringBuffer& a_Buf) const
{
    if (m_strName.size())
        getName(a_Buf);
    else
        formatAnonymousName(a_Buf);
    getTemplateDecoration(a_Buf);
}

void Symbol::getTemplateDecoration(StringBuffer& a_Buf) const
{
    TemplateSpecialization* pSpec = getTemplateSpecialization();
    if (pSpec == nullptr)
    {
        return;
    }
    pSpec->getDecoration(a_Buf);
}

void Symbol::getTemplateQualifiedDecoration(StringBuffer& a_Buf) const
{
    TemplateSpecialization* pSpec = getTemplateSpecialization();
    if (pSpec == nullptr)
    {
        return;
    }
    pSpec->getQualifiedDecoration(a_Buf);
}

void Symbol::getUniqueTemplateDecoration(StringBuffer& a_Buf) const
{
    TemplateSpecialization* pSpec = getTemplateSpecialization();
    if (pSpec == nullptr)
    {
        return;
    }
    a_Buf += '<';
    for (size_t i = 0; i < pSpec->getArgumentCount(); ++i)
    {
        if (i)
            a_Buf += ',';
        pSpec->getArgument(i)->getUniqueName(a_Buf);
    }
    if (a_Buf.back() == '>')
        a_Buf += ' ';
    a_Buf += '>';
}

Symbol* SymbolExtension::getSymbol() const
{
    return static_cast<Symbol*>(getOwner());
}

void Symbol::setUserData(UserData&& a_UserData)
{
    m_UserData = std::move(a_UserData);
}

hash64 Symbol::getHash() const
{
    if (m_Hash == 0)
        m_Hash = _computeHash();
    PHANTOM_ASSERT_DEBUG(computeHash() == m_Hash, "hash for symbol %s is inconsistent over time",
                         getQualifiedDecoratedName().c_str());
    return m_Hash;
}

} // namespace reflection
} // namespace phantom
