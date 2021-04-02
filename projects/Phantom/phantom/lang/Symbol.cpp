// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "Symbol.h"

#include "Application.h"
#include "Module.h"
#include "Namespace.h"
#include "Placeholder.h"
#include "Template.h"
#include "TemplateSpecialization.h"

#include <locale>
#include <phantom/utils/StringHash.h>
/* *********************************************** */
namespace phantom
{
namespace lang
{
static Symbols          empty_elements;
static MetaDatas        empty_metas;
static Annotations      empty_annotations;
static SymbolExtensions empty_extensions;

struct Symbol::ExtraData
{
    MetaDatas        metaDatas{};
    Annotations      annotations{};
    SymbolExtensions extensions{};
    UserData         userData{};
};

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
    : LanguageElement(a_uiFlags | PHANTOM_R_INTERNAL_FLAG_SYMBOL), m_Modifiers(a_Modifiers)
{
}

Symbol::Symbol(StringView a_strName, Modifiers a_Modifiers /*= 0*/, uint a_uiFlags /*= 0*/)
    : LanguageElement(a_uiFlags | PHANTOM_R_INTERNAL_FLAG_SYMBOL), m_strName(a_strName), m_Modifiers(a_Modifiers)
{
    PHANTOM_ASSERT(!(isProtected() && isPrivate()), "o_private_access and o_protected_access cannot co-exist");
}

void Symbol::terminate()
{
    setVisibility(Visibility::Private);
    setNamespace(nullptr);
    if (m_pExtraData)
        delete_<ExtraData>(m_pExtraData);
    LanguageElement::terminate();
}

int Symbol::destructionPriority() const
{
    int prio = LanguageElement::destructionPriority();

    if (m_pExtraData != nullptr)
    {
        for (MetaDatas::value_type& pair : m_pExtraData->metaDatas)
        {
            int typePrio = pair.second.type()->destructionPriority() - 1;
            if (typePrio < prio)
                prio = typePrio;
        }
    }
    return prio;
}

hash64 Symbol::computeHash() const
{
    PHANTOM_ASSERT_DEBUG(m_Hash != 0 /*for recompute checks*/ || testFlags(PHANTOM_R_INTERNAL_FLAG_SPECIAL) ||
                         getModule());
    hash64 h = getLocalHash();
    if (auto pNamingScope = getNamingScope())
    {
        Symbol* pNSSym = pNamingScope->asSymbol();
        while (!pNSSym)
        {
            CombineHash(h, pNamingScope->getOwner()->getElementIndex(pNamingScope));
            if (pNamingScope = pNamingScope->getNamingScope())
                pNSSym = pNamingScope->asSymbol();
            else
                break;
        }
        if (pNSSym)
            CombineHash(h, pNSSym->getHash());
    }
    return h;
}

hash64 Symbol::ComputeHash(const char* a_Str, size_t a_Len)
{
    return makeStringHash(StringView(a_Str, a_Len));
}

void Symbol::formatAnonymousName(StringBuffer& a_Buf) const
{
    char buf[64];
    snprintf(buf, 64, "__anonymous_%zx", (size_t)this);
    a_Buf += buf;
}

MetaDatas& Symbol::_metaDatas()
{
    return _extraData().metaDatas;
}

MetaDatas const& Symbol::_metaDatas() const
{
    return _extraData().metaDatas;
}

const Variant& Symbol::getMetaData(StringView a_Name) const
{
    return getMetaData(StringWithHash(a_Name));
}

const Variant& Symbol::getMetaData(StringWithHash a_Name) const
{
    static Variant null;
    if (m_pExtraData == nullptr)
        return null;
    auto& md = _metaDatas();
    auto  found = md.find(a_Name);
    return found == md.end() ? null : found->second;
}

bool Symbol::hasMetaData(StringWithHash a_Hash) const
{
    if (m_pExtraData == nullptr)
        return false;
    auto& md = _metaDatas();
    return md.find(a_Hash) != md.end();
}

bool Symbol::hasMetaData(StringView a_strName) const
{
    return hasMetaData(StringWithHash(a_strName));
}

const MetaDatas& Symbol::getMetaDatas() const
{
    return m_pExtraData ? m_pExtraData->metaDatas : empty_metas;
}

bool Symbol::hasAnnotation(StringView a_strName) const
{
    return (m_pExtraData && m_pExtraData->annotations.find(a_strName) != m_pExtraData->annotations.end());
}

bool Symbol::addAnnotation(StringView a_strName)
{
    return _annotations().insert(a_strName).second;
}

bool Symbol::removeAnnotation(StringView a_strName)
{
    if (m_pExtraData && _annotations().erase(a_strName) == 1)
        return true;
    return false;
}

const Annotations& Symbol::getAnnotations() const
{
    return m_pExtraData ? m_pExtraData->annotations : empty_annotations;
}

SymbolExtension* Symbol::getExtension(Class* a_pClass, size_t a_Num /* = 0*/) const
{
    size_t c = 0;
    for (auto ext : getExtensions())
        if (ext->rtti.metaClass->isA(a_pClass))
            if (c++ == a_Num)
                return ext;
    return nullptr;
}

const SymbolExtensions& Symbol::getExtensions() const
{
    const_cast<Symbol*>(this)->onElementsAccess();
    return m_pExtraData ? m_pExtraData->extensions : empty_extensions;
}

void Symbol::addExtensions(const SymbolExtensions& a_Extensions)
{
    for (auto ext : a_Extensions)
        addExtension(ext);
}

void Symbol::addExtension(SymbolExtension* a_pExtension)
{
    auto& exts = _extensions();
    PHANTOM_ASSERT(std::find(exts.begin(), exts.end(), a_pExtension) == exts.end());
    exts.push_back(a_pExtension);
}

void Symbol::setMetaData(StringWithHash a_Hash, const Variant& a_Value)
{
    _metaDatas()[a_Hash] = a_Value;
}

void Symbol::setMetaData(StringWithHash a_Hash, Variant&& a_Value)
{
    _metaDatas()[a_Hash] = std::move(a_Value);
}

void Symbol::setMetaData(StringView a_Name, const Variant& a_Value)
{
    size_t dotPos = a_Name.find_last_of('.');
    if (dotPos == String::npos)
    {
        PHANTOM_ASSERT(Symbol::IsCppIdentifier(a_Name), "meta '%.*s' is not an identifier name matching [a-zA-Z0-9_]+",
                       PHANTOM_STRING_AS_PRINTF_ARG(a_Name));
        _metaDatas()[StringWithHash(a_Name)] = a_Value;
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
    _metaDatas()[StringWithHash(a_Name)] = (Variant &&) a_Value;
}

void Symbol::removeMetaData(StringWithHash a_NameHash)
{
    PHANTOM_ASSERT(m_pExtraData);
    auto& md = _metaDatas();
    auto  found = md.find(a_NameHash);
    PHANTOM_ASSERT(found != md.end());
    md.erase(found);
}

void Symbol::removeMetaData(StringView a_Name)
{
    removeMetaData(StringWithHash(a_Name));
}

void Symbol::setAccess(Access a_eAccess)
{
    m_eAccess = a_eAccess;
}

void Symbol::setVisibility(Visibility a_eVis)
{
    if (m_eVisibility == a_eVis)
        return;

    if (Application::Get() == this)
        return;

    if (!getSource())
    {
        m_eVisibility = a_eVis;
        return;
    }

    onVisibilityChanging(m_eVisibility);
    if (m_eVisibility == Visibility::Public)
    {
        if (m_pNamespace && getSource() && getSource()->getVisibility() == Visibility::Public)
        {
            m_pNamespace->_unregisterSymbol(this);
        }
        else if (getSource() == this) // this is a source
        {
            for (auto p : getElements())
            {
                if (Symbol* pSymbol = p->asSymbol())
                {
                    pSymbol->onVisibilityChanging(pSymbol->getVisibility());
                    if (pSymbol->getVisibility() == Visibility::Public)
                        if (pSymbol->m_pNamespace)
                            pSymbol->m_pNamespace->_unregisterSymbol(pSymbol);
                }
            }
        }
    }
    m_eVisibility = a_eVis;
    if (m_eVisibility == Visibility::Public)
    {
        if (m_pNamespace && getSource() && getSource()->getVisibility() == Visibility::Public)
        {
            m_pNamespace->_registerSymbol(this);
        }
        else if (getSource() == this) // this is a source
        {
            for (auto p : getElements())
            {
                if (Symbol* pSymbol = p->asSymbol())
                {
                    if (pSymbol->getVisibility() == Visibility::Public)
                        if (pSymbol->m_pNamespace)
                            pSymbol->m_pNamespace->_registerSymbol(pSymbol);
                    pSymbol->onVisibilityChanged(pSymbol->getVisibility());
                }
            }
        }
    }
    onVisibilityChanged(m_eVisibility);
}

void Symbol::setModifiers(Modifiers a_Modifiers)
{
    m_Modifiers = a_Modifiers;
    PHANTOM_ASSERT(!(isPublic() && isProtected()), "o_public_access and o_protected_access cannot co-exist");
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
    return (this == a_pOther || (a_pOther->getOwner() && this->getOwner() && getHash() == a_pOther->getHash()));
}

bool Symbol::hasElementWithName(StringView a_strName) const
{
    for (auto pElm : getElements())
    {
        Symbol* pSymbol = pElm->asSymbol();
        if (pSymbol && pSymbol->getName() == a_strName)
            return true;
    }
    return false;
}

LanguageElement* Symbol::getNamingScope() const
{
    if (m_pNamespace)
        return m_pNamespace;
    if (auto pOwner = getOwner())
    {
        if (pOwner->asSource() == nullptr)
        {
            return pOwner;
        }
    }
    return nullptr;
}

Namespace* Symbol::getNamespace() const
{
    return m_pNamespace;
}

void Symbol::setNamespace(Namespace* a_pNS)
{
    if (m_pNamespace == a_pNS)
        return;
    Source* pSource = getSource();
    //     PHANTOM_ASSERT(asNamespace() || pSource == getOwner(),
    //                    "only namespaces or source level symbols can be added to namespaces");
    if (m_pNamespace)
    {
        onNamespaceChanging(m_pNamespace);
        if (getVisibility() == Visibility::Public && pSource && pSource->getVisibility() == Visibility::Public)
        {
            m_pNamespace->_unregisterSymbol(this);
        }
    }
    m_pNamespace = a_pNS;
    if (m_pNamespace)
    {
        if (getVisibility() == Visibility::Public && pSource && pSource->getVisibility() == Visibility::Public)
        {
            m_pNamespace->_registerSymbol(this);
        }
        onNamespaceChanged(m_pNamespace);
    }
}

void Symbol::getDoubles(Symbols& out) const
{
    LanguageElement* pNamingScope = getNamingScope();
    if (pNamingScope)
    {
        if (Scope* pScpe = pNamingScope->asScope())
            pScpe->getElementDoubles(const_cast<Symbol*>(this), out);
    }
}

bool Symbol::hasPublicMember(Symbol* a_pSymbol) const
{
    return hasElement(a_pSymbol) && a_pSymbol->isPublic();
}

bool Symbol::hasProtectedMember(Symbol* a_pSymbol) const
{
    return hasElement(a_pSymbol) && a_pSymbol->isProtected();
}

bool Symbol::hasPrivateMember(Symbol* a_pSymbol) const
{
    return hasElement(a_pSymbol) && a_pSymbol->isPrivate();
}

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wtautological-undefined-compare"
#endif // __clang__

void Symbol::setMetaDatas(MetaDatas&& a_MetaDatas)
{
    if (&a_MetaDatas == nullptr)
        return;
    _metaDatas() = std::move(a_MetaDatas);
}

void Symbol::setMetaDatas(const MetaDatas& a_MetaDatas)
{
    if (&a_MetaDatas == nullptr)
        return;
    _metaDatas() = a_MetaDatas;
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
    while (pOwner && pOwner->getName().empty())
        pOwner = pOwner->getOwner();
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
    LanguageElement* pNamingScope = getNamingScope();
    if (TemplateSpecialization* pSpec = getTemplateSpecialization())
    {
        return pSpec->getTemplate()->getQualifiedName(a_Buf);
    }
    if (pNamingScope)
    {
        while (pNamingScope && pNamingScope->getName().empty())
            pNamingScope = pNamingScope->getNamingScope();
        if (pNamingScope)
        {
            size_t prev = a_Buf.size();
            pNamingScope->getQualifiedDecoratedName(a_Buf);
            bool ownerEmpty = (a_Buf.size() - prev) == 0;
            if (!ownerEmpty) // no owner name
            {
                a_Buf += ':';
                a_Buf += ':';
            }
        }
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

hash64 Symbol::getLocalHash() const
{
    if (m_LocalHash == 0)
        m_LocalHash = computeLocalHash();
    return m_LocalHash;
}

hash64 Symbol::getRelativeHash(LanguageElement* a_pTo) const
{
    if (hasNamingScopeCascade(a_pTo))
    {
        hash64 h = getLocalHash();
        if (auto pScope = getNamingScope())
        {
            if (pScope != a_pTo && pScope->asSymbol())
            {
                CombineHash(h, static_cast<Symbol*>(pScope)->getRelativeHash(a_pTo));
            }
        }
        return h;
    }
    return getHash();
}

hash64 Symbol::computeLocalHash() const
{
    hash64 h = 0;
    if (m_strName.size())
        h = ComputeHash(m_strName.data(), m_strName.size());
    else
    {
        CombineHash(h, hash64(this));
    }
    CombineHash(h, getTemplateDecorationHash());
    return h;
}

void Symbol::getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    if (TemplateSpecialization* pSpec = getTemplateSpecialization())
    {
        return pSpec->getTemplate()->getRelativeName(a_pTo, a_Buf);
    }
    LanguageElement* pTo = a_pTo;
    while (pTo != Namespace::Global())
    {
        if (pTo == this)
            return getName(a_Buf);

        Symbols homonymous;
        pTo->getSymbolsWithName(m_strName, homonymous);
        if (!homonymous.empty() &&
            homonymous.front()->getNamingScope() !=
            getNamingScope()) // if any symbol matches the same name in between the current relative scope and this
                              // symbols scope, we interrupt look up and return full qualified name to avoid collisions
            return getQualifiedName(a_Buf);

        if (hasNamingScopeCascade(pTo))
            break;
        pTo = pTo->getNamingScope();
    }

    if (pTo == Namespace::Global())
        return getQualifiedName(a_Buf);

    LanguageElement* pNamingScope = getNamingScope();
    if (pNamingScope && pTo != pNamingScope)
    {
        while (pNamingScope && pNamingScope->getName().empty())
            pNamingScope = pNamingScope->getNamingScope();
        if (pNamingScope)
        {
            size_t prev = a_Buf.size();
            pNamingScope->getRelativeDecoratedName(pTo, a_Buf);
            bool ownerEmpty = (a_Buf.size() - prev) == 0;
            if (!ownerEmpty) // no owner name
            {
                a_Buf += ':';
                a_Buf += ':';
            }
        }
    }
    if (m_strName.empty())
        return formatAnonymousName(a_Buf);
    else
        return getName(a_Buf);
}

void Symbol::getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const
{
    if (m_strName == "ReviveSide" && a_pTo->getName() == "TurretComponentGD")
    {
        printf("");
    }
    if (TemplateSpecialization* pSpec = getTemplateSpecialization())
    {
        return pSpec->getRelativeDecoratedName(a_pTo, a_Buf);
    }
    LanguageElement* pTo = a_pTo;
    while (pTo != Namespace::Global())
    {
        if (pTo == this)
            return getDecoratedName(a_Buf);

        Symbols homonymous;
        pTo->getSymbolsWithName(m_strName, homonymous);
        if (!homonymous.empty() &&
            homonymous.front()->getNamingScope() != getNamingScope()) // if any symbol matches the same name in between
                                                                      // the current relative scope and this symbols
            // scope, we interrupt look up and return full qualified name to avoid collisions
            return getQualifiedDecoratedName(a_Buf);

        if (hasNamingScopeCascade(pTo))
            break;
        pTo = pTo->getNamingScope();
    }

    if (pTo == Namespace::Global())
        return getQualifiedDecoratedName(a_Buf);

    LanguageElement* pNamingScope = getNamingScope();
    if (pNamingScope && pTo != pNamingScope)
    {
        // skip anonymous when inside a qualified name
        while (pNamingScope && pNamingScope->getName().empty())
            pNamingScope = pNamingScope->getNamingScope();

        size_t prev = a_Buf.size();
        pNamingScope->getRelativeDecoratedName(pTo, a_Buf);
        bool ownerEmpty = (a_Buf.size() - prev) == 0;
        if (!ownerEmpty) // no owner name
        {
            a_Buf += ':';
            a_Buf += ':';
        }
    }
    if (m_strName.empty())
        return formatAnonymousName(a_Buf);
    else
        return getDecoratedName(a_Buf);
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

hash64 Symbol::getTemplateDecorationHash() const
{
    TemplateSpecialization* pSpec = getTemplateSpecialization();
    if (pSpec == nullptr)
    {
        return 0;
    }
    return pSpec->getDecorationHash();
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

TemplateSpecialization* Symbol::getTemplateSpecialization() const
{
    if (m_pOwner)
    {
        if (TemplateSpecialization* pSpec = m_pOwner->asTemplateSpecialization())
        {
            if (pSpec->getTemplated() == this)
                return pSpec;
        }
    }
    return nullptr;
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
    _userData() = std::move(a_UserData);
}

hash64 Symbol::getHash() const
{
    // clang-format off
    PHANTOM_ASSERT_DEBUG(isNative()
        || isTemplateDependant() 
        || this == Application::Get() 
		|| asNamespace()
		|| getModule());
    // clang-format on
    if (m_Hash == 0)
    {
        m_Hash = computeHash();
    }
    PHANTOM_ASSERT_DEBUG(computeHash() == m_Hash, "hash for symbol %s is inconsistent over time",
                         getQualifiedDecoratedName().c_str());
    return m_Hash;
}

Annotations& Symbol::_annotations()
{
    return _extraData().annotations;
}

Annotations const& Symbol::_annotations() const
{
    return _extraData().annotations;
}

UserData& Symbol::_userData()
{
    return _extraData().userData;
}

UserData const& Symbol::_userData() const
{
    return _extraData().userData;
}

SymbolExtensions& Symbol::_extensions()
{
    return _extraData().extensions;
}

SymbolExtensions const& Symbol::_extensions() const
{
    return _extraData().extensions;
}

Symbol::ExtraData& Symbol::_extraData()
{
    if (m_pExtraData == nullptr)
        m_pExtraData = new_<ExtraData>();
    return *m_pExtraData;
}

} // namespace lang
} // namespace phantom
