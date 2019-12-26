#pragma once

// haunt {

#include "Message.h"

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
#include <phantom/constructor>
#include <phantom/signal>
#include <phantom/field>

#include <phantom/template-only-push>

#include "Signal.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom")
    PHANTOM_SOURCE("Message")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_CLASS(Message)
        {
            using Messages = typedef_< phantom::Messages>;
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            using messages_def = typedef_<_::messages_def>;
            this_()(PHANTOM_R_FLAG_NO_COPY)
        .public_()
            .struct_<messages_def>()
                .constructor<void(Message*)>()
            .end()
            .constructor<void(MessageType, StringView)>()["MessageType::Undefined"]["\"\""]
            .method<Message*() const>("getRootMessage", &_::getRootMessage)
            .method<Message*() const>("getParent", &_::getParent)
            .method<Message*(size_t) const>("getChild", &_::getChild)
            .method<size_t() const>("getChildCount", &_::getChildCount)
            .method<Messages const&() const>("getChildren", &_::getChildren)
            .method<const phantom::Variant&() const>("getData", &_::getData)
            .method<void(const ::phantom::Variant &)>("setData", &_::setData)
            .method<MessageType() const>("getType", &_::getType)
            .method<void(MessageType)>("setType", &_::setType)
            .method<MessageType() const>("getMostValuableMessageType", &_::getMostValuableMessageType)
            .method<StringView() const>("getText", &_::getText)
            .method<void(StringView)>("setText", &_::setText)
            .method<StringView() const>("getCategory", &_::getCategory)
            .method<void(StringView)>("setCategory", &_::setCategory)
            .method<void(Message*), virtual_>("addChild", &_::addChild)
            .method<void(Message*), virtual_>("removeChild", &_::removeChild)
            .method<Message*(bool) const>("clone", &_::clone)["true"]
            /// missing symbol(s) reflection (va_list) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<void(const char*, va_list)>("format", &_::format)
            .method<void()>("open", &_::open)
            /// missing symbol(s) reflection (va_list) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Message*(MessageType, const ::phantom::Variant &, const char*, va_list)>("messageD", &_::messageD)
            /// missing symbol(s) reflection (va_list) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .method<Message*(MessageType, const char*, va_list)>("message", &_::message)
            .method<messages_def()>("messages", &_::messages)
            .method<void(Message*) const>("copyChildrenTo", &_::copyChildrenTo)
            .method<void()>("clear", &_::clear)
            .method<Message*()>("removeLast", &_::removeLast)
        
        .protected_()
            .method<void(Message*)>("emitMessageOpenedCascade", &_::emitMessageOpenedCascade)
            .field("PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD", &_::PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD)
        
        .public_()
            .signal("childAdded", &_::childAdded)
            .signal("childRemoved", &_::childRemoved)
            .signal("descendantAdded", &_::descendantAdded)
            .signal("descendantRemoved", &_::descendantRemoved)
            .signal("opened", &_::opened)
            .signal("messageOpened", &_::messageOpened)
        
        .protected_()
            .field("m_pParent", &_::m_pParent)
            .field("m_Children", &_::m_Children)
            .field("m_eType", &_::m_eType)
            .field("m_strCategory", &_::m_strCategory)
            .field("m_strText", &_::m_strText)
            .field("m_Data", &_::m_Data)
            .field("m_Flags", &_::m_Flags)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Message")
PHANTOM_END("phantom")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// haunt }
