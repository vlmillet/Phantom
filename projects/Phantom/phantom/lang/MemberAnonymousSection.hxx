#pragma once

// haunt {

// clang-format off

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

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("MemberAnonymousSection")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(MemberAnonymousSection)
        {
            using DataElements = typedef_< phantom::lang::DataElements>;
            using Fields = typedef_< phantom::lang::Fields>;
            using MemberAnonymousSections = typedef_< phantom::lang::MemberAnonymousSections>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::Symbol, ::phantom::lang::DataElement>()
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constructor<void(Modifiers, uint)>()({"modifiers","a_uiFlags"})["0"]["0"]
            .method<MemberAnonymousSection*() const, virtual_|override_>("asMemberAnonymousSection", &_::asMemberAnonymousSection)
            .method<DataElement*() const, virtual_|override_>("asDataElement", &_::asDataElement)
            .method<Field*() const>("getFirstField", &_::getFirstField)
            .method<void(Field*)>("addField", &_::addField)({"a_pField"})
            .method<Field*(StringView) const>("getField", &_::getField)({"a_strName"})
            .method<const DataElements&() const>("getDataElements", &_::getDataElements)
            .method<const Fields&() const>("getFields", &_::getFields)
            .method<ClassType*() const>("getRootOwner", &_::getRootOwner)
            .method<MemberAnonymousSection*() const>("getOwnerSection", &_::getOwnerSection)
            .method<MemberAnonymousSection*() const>("getRootSection", &_::getRootSection)
            .method<MemberAnonymousUnion*() const>("getEmbeddingMemberAnonymousUnion", &_::getEmbeddingMemberAnonymousUnion)
            .method<void(MemberAnonymousSection*)>("addMemberAnonymousSection", &_::addMemberAnonymousSection)({"a_pMemberAnonymousSection"})
            .method<size_t() const, virtual_|override_>("getSize", &_::getSize)
            .method<size_t() const, virtual_|override_>("getAlignment", &_::getAlignment)
            .method<size_t() const, virtual_|override_>("getOffset", &_::getOffset)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<void(Fields&) const, override_>("fetchFields", &_::fetchFields)({"a_Out"})
        
        .protected_()
            .method<void(size_t), virtual_|override_>("setOffset", &_::setOffset)({"a_uiOffset"})
            .method<void(size_t&, size_t&) const, pure_virtual>("computeSizeAndAlignment", &_::computeSizeAndAlignment)({"a_uiSize","a_uiAlignment"})
        
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
