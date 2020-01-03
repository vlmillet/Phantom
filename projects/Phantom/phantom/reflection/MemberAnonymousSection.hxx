#pragma once

// haunt {

#include "MemberAnonymousSection.h"

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
#include <phantom/field>

#include <phantom/template-only-push>

#include <phantom/utils/SmallVector.hxx>
#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace reflection {
PHANTOM_PACKAGE("phantom.reflection")
    PHANTOM_SOURCE("MemberAnonymousSection")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(MemberAnonymousSection)
        {
            using DataElements = typedef_< phantom::reflection::DataElements>;
            using Fields = typedef_< phantom::reflection::Fields>;
            using MemberAnonymousSections = typedef_< phantom::reflection::MemberAnonymousSections>;
            using Modifiers = typedef_< phantom::reflection::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
            .inherits<::phantom::reflection::Symbol, ::phantom::reflection::DataElement>()
        .public_()
            .staticMethod<::phantom::reflection::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Modifiers, uint)>()["0"]["0"]
            .method<MemberAnonymousSection*() const, virtual_|override_>("asMemberAnonymousSection", &_::asMemberAnonymousSection)
            .method<DataElement*() const, virtual_|override_>("asDataElement", &_::asDataElement)
            .method<Field*() const>("getFirstField", &_::getFirstField)
            .method<void(Field*)>("addField", &_::addField)
            .method<Field*(StringView) const>("getField", &_::getField)
            .method<const DataElements&() const>("getDataElements", &_::getDataElements)
            .method<const Fields&() const>("getFields", &_::getFields)
            .method<ClassType*() const>("getRootOwner", &_::getRootOwner)
            .method<MemberAnonymousSection*() const>("getOwnerSection", &_::getOwnerSection)
            .method<MemberAnonymousSection*() const>("getRootSection", &_::getRootSection)
            .method<MemberAnonymousUnion*() const>("getEmbeddingMemberAnonymousUnion", &_::getEmbeddingMemberAnonymousUnion)
            .method<void(MemberAnonymousSection*)>("addMemberAnonymousSection", &_::addMemberAnonymousSection)
            .method<void(MemberAnonymousSection*)>("removeMemberAnonymousSection", &_::removeMemberAnonymousSection)
            .method<MemberAnonymousSection*(size_t) const>("getMemberAnonymousSection", &_::getMemberAnonymousSection)
            .method<size_t() const>("getMemberAnonymousSectionCount", &_::getMemberAnonymousSectionCount)
            .method<size_t() const, virtual_|override_>("getSize", &_::getSize)
            .method<size_t() const, virtual_|override_>("getAlignment", &_::getAlignment)
            .method<size_t() const, virtual_|override_>("getOffset", &_::getOffset)
            .method<void(Fields&) const, virtual_|override_>("fetchFields", &_::fetchFields)
        
        .protected_()
            .method<void(LanguageElement*), virtual_|override_>("onReferencedElementRemoved", &_::onReferencedElementRemoved)
            .method<void(LanguageElement*), virtual_|override_>("onElementRemoved", &_::onElementRemoved)
            .method<void(LanguageElement*), virtual_|override_>("onElementAdded", &_::onElementAdded)
            .method<void(size_t), virtual_|override_>("setOffset", &_::setOffset)
            .method<void(size_t&, size_t&) const, pure_virtual>("computeSizeAndAlignment", &_::computeSizeAndAlignment)
        
        .protected_()
            .field("m_Fields", &_::m_Fields)
            .field("m_MemberAnonymousSections", &_::m_MemberAnonymousSections)
            .field("m_DataElements", &_::m_DataElements)
            .field("m_uiSize", &_::m_uiSize)
            .field("m_uiAlignment", &_::m_uiAlignment)
            .field("m_uiOffset", &_::m_uiOffset)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("MemberAnonymousSection")
PHANTOM_END("phantom.reflection")
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
