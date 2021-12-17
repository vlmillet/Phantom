#pragma once

// haunt {

// clang-format off

#include "Field.h"

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
#include <phantom/static_constant>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/friend>

#include <phantom/template-only-push>

#include <phantom/utils/StringView.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("Field")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Field)
        {
            using Fields = typedef_< phantom::lang::Fields>;
            using Modifiers = typedef_< phantom::lang::Modifiers>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            .inherits<::phantom::lang::ValueMember, ::phantom::lang::DataElement>()
        .public_()
            .method<void(::phantom::lang::LanguageElementVisitor *, ::phantom::lang::VisitorData), virtual_|override_>("visit", &_::visit)({"a_pVisitor","a_Data"})
        
        .public_()
            .staticMethod<::phantom::lang::Class *()>("MetaClass", &_::MetaClass)
        
        .public_()
            .constant("InvalidOffset", _::InvalidOffset)
            .constructor<void(Type*, StringView, uint, Modifiers, uint)>()({"a_pValueType","a_strName","a_uiFilterMask","a_Modifiers","a_uiFlags"})["PHANTOM_R_FILTER_FIELD"]["0"]["0"]
            .method<size_t() const, virtual_|override_>("getOffset", &_::getOffset)
            .method<void*(void const*) const, virtual_|override_>("getAddress", &_::getAddress)({"a_pInstance"})
            .method<MemberAnonymousSection*() const>("getMemberAnonymousSection", &_::getMemberAnonymousSection)
            .method<MemberAnonymousUnion*() const>("getEmbeddingMemberAnonymousUnion", &_::getEmbeddingMemberAnonymousUnion)
            .method<void(Type*)>("setValueType", &_::setValueType)({"a_pType"})
            .method<void(size_t)>("setAlignment", &_::setAlignment)({"a_Alignment"})
            .method<bool() const, virtual_|override_>("isWrittable", &_::isWrittable)
            .method<bool() const, virtual_|override_>("isReadable", &_::isReadable)
            .method<size_t() const, virtual_|override_>("getAlignment", &_::getAlignment)
            .method<size_t() const, virtual_|override_>("getSize", &_::getSize)
            .method<void(void const*, void*) const, virtual_|override_>("getValue", &_::getValue)({"a_pObject","a_pDest"})
            .method<void(void*, void const*) const, virtual_|override_>("setValue", &_::setValue)({"a_pObject","a_pSrc"})
            .method<void(void*, void*) const, virtual_|override_>("setValueMoved", &_::setValueMoved)({"a_pObject","src"})
            .method<Field*() const, virtual_|override_>("asField", &_::asField)
            .method<DataElement*() const, virtual_|override_>("asDataElement", &_::asDataElement)
            /// invalid declaration, some symbols have not been parsed correctly probably due to missing include path or missing #include in the .h
            // .method<void(Fields&) const, override_>("fetchFields", &_::fetchFields)({"a_Out"})
        
        .protected_()
            .constructor<void(Type*, StringView, size_t, uint, Modifiers, uint)>()({"a_pValueType","a_strName","a_uiOffset","a_uiFilterMask","a_Modifiers","a_uiFlags"})
            .method<void(size_t), virtual_|override_>("setOffset", &_::setOffset)({"a_uiOffset"})
        
        .protected_()
            .field("m_uiOffset", &_::m_uiOffset)
            .field("m_pMemberAnonymousSection", &_::m_pMemberAnonymousSection)
            .field("m_Alignment", &_::m_Alignment)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Field")
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
