// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

/* ******************* Includes ****************** */
// #include "phantom/phantom.h"
#include "Message.h"

#include <phantom/new>
#include <phantom/reflection/CodeLocation.h>
#include <stdarg.h>
/* *********************************************** */
namespace phantom
{
//================================================
// Constructors / Destructor
//================================================

Message::Message(MessageType a_eType, StringView a_strText) : m_pParent(nullptr), m_eType(a_eType), m_strText(a_strText)
{
}

Message::~Message()
{
    clear();
}

//================================================
// Accessors
//================================================

Message* Message::getRootMessage() const
{
    return m_pParent == nullptr ? const_cast<Message*>(this) : m_pParent->getRootMessage();
}

Message* Message::getParent() const
{
    return m_pParent;
}

Message* Message::getChild(size_t a_uiIndex) const
{
    return m_Children[a_uiIndex];
}

size_t Message::getChildCount() const
{
    return m_Children.size();
}

void Message::setData(const phantom::Variant& a_Data)
{
    m_Data = a_Data;
}

const phantom::Variant& Message::getData() const
{
    return m_Data;
}

MessageType Message::getType() const
{
    return m_eType;
}

void Message::setType(MessageType a_eType)
{
    m_eType = a_eType;
}

MessageType Message::getMostValuableMessageType() const
{
    MessageType type = m_eType;
    auto        it = m_Children.begin();
    for (; it != m_Children.end(); ++it)
    {
        MessageType childType = (*it)->getMostValuableMessageType();
        if (childType > type)
            type = childType;
    }
    return type;
}

StringView Message::getText() const
{
    return m_strText;
}

void Message::setText(StringView a_strText)
{
    m_strText = a_strText;
}

StringView Message::getCategory() const
{
    return m_strCategory;
}

void Message::setCategory(StringView a_strCategory)
{
    m_strCategory = a_strCategory;
}

//================================================
// Operations
//================================================

void Message::format(const char* a_format, ...)
{
    va_list args;
    va_start(args, a_format);
    char buffer[512];
    buffer[511] = '\0';
    vsnprintf(buffer, 511, a_format, args);
    va_end(args);
    m_strText = buffer;
}

void Message::format(const char* a_format, va_list args)
{
    char buffer[512];
    buffer[511] = '\0';
    vsnprintf(buffer, 511, a_format, args);

    m_strText = buffer;
}
PHANTOM_STATIC_ASSERT(sizeof(bool) == 1);

void Message::addChild(Message* a_pChild)
{
    PHANTOM_ASSERT(a_pChild->m_pParent == nullptr);
    a_pChild->m_pParent = this;
    m_Children.push_back(a_pChild);
    PHANTOM_EMIT childAdded(a_pChild);
    emitDescendantRemovedCascade(a_pChild);
}

void Message::removeChild(Message* a_pChild)
{
    PHANTOM_ASSERT(a_pChild->m_pParent == this);
    auto it = m_Children.begin();
    for (; it != m_Children.end(); ++it)
    {
        if (*it == a_pChild)
            break;
    }
    PHANTOM_ASSERT(it != m_Children.end());
    emitDescendantRemovedCascade(a_pChild);
    PHANTOM_EMIT childRemoved(a_pChild);
    a_pChild->m_pParent = nullptr;
    m_Children.erase(it);
}

void Message::clear()
{
    auto it = m_Children.begin();
    for (; it != m_Children.end(); ++it)
    {
        (*it)->clear();
        PHANTOM_EMIT childRemoved(*it);
        PHANTOM_DELETE(Message)(*it);
    }
    m_Children.clear();
}

Message* Message::errorD(const Variant& a_Data, const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    Message* pMessage = messageD(MessageType::Error, a_Data, a_Format, args);
    va_end(args);
    return pMessage;
}

Message* Message::warningD(const Variant& a_Data, const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    Message* pMessage = messageD(MessageType::Warning, a_Data, a_Format, args);
    va_end(args);
    return pMessage;
}

Message* Message::informationD(const Variant& a_Data, const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    Message* pMessage = messageD(MessageType::Information, a_Data, a_Format, args);
    va_end(args);
    return pMessage;
}

Message* Message::successD(const Variant& a_Data, const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    Message* pMessage = messageD(MessageType::Success, a_Data, a_Format, args);
    va_end(args);
    return pMessage;
}

Message* Message::error(const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    Message* pMessage = message(MessageType::Error, a_Format, args);
    va_end(args);
    return pMessage;
}

Message* Message::warning(const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    Message* pMessage = message(MessageType::Warning, a_Format, args);
    va_end(args);
    return pMessage;
}

Message* Message::information(const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    Message* pMessage = message(MessageType::Information, a_Format, args);
    va_end(args);
    return pMessage;
}

Message* Message::success(const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    Message* pMessage = message(MessageType::Success, a_Format, args);
    va_end(args);
    return pMessage;
}

Message* Message::clone(bool recursive /*= true*/) const
{
    Message* cloned = PHANTOM_NEW(Message)(m_eType, m_strText);
    cloned->m_Data = m_Data;
    cloned->m_strCategory = m_strCategory;
    if (recursive)
    {
        for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
        {
            cloned->addChild((*it)->clone());
        }
    }
    return cloned;
}

void Message::emitMessageOpenedCascade(Message* a_pMessage)
{
    PHANTOM_EMIT messageOpened(a_pMessage);
    if (m_pParent)
        m_pParent->emitMessageOpenedCascade(a_pMessage);
}

void Message::open()
{
    PHANTOM_EMIT opened();
    emitMessageOpenedCascade(this);
}

Message* Message::messageD(MessageType a_eType, const phantom::Variant& a_Data, const char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    Message* pMessage = messageD(a_eType, a_Data, a_Format, args);
    va_end(args);
    return pMessage;
}

Message* Message::messageD(MessageType a_eType, const phantom::Variant& a_Data, const char* a_Format, va_list args)
{
    Message* pMessage = PHANTOM_NEW(Message)(a_eType);
    pMessage->setData(a_Data);
    size_t size = strlen(a_Format) + 512;
    char*  buffer = (char*)alloca(size);
    buffer[size - 1] = '\0';
    vsnprintf(buffer, size, a_Format, args);
    pMessage->setText(buffer);
    addChild(pMessage);
    return pMessage;
}

Message* Message::message(MessageType a_eType, const char* a_Format, va_list args)
{
    Message* pMessage = PHANTOM_NEW(Message)(a_eType);
    size_t   size = strlen(a_Format) + 512;
    char*    buffer = (char*)alloca(size);
    buffer[size - 1] = '\0';
    vsnprintf(buffer, size, a_Format, args);
    pMessage->setText(buffer);
    addChild(pMessage);
    return pMessage;
}

void Message::emitDescendantAddedCascade(Message* a_pMessage)
{
    if (m_pParent)
        m_pParent->emitDescendantAddedCascade(a_pMessage);
    PHANTOM_EMIT descendantAdded(a_pMessage);
}

void Message::emitDescendantRemovedCascade(Message* a_pMessage)
{
    PHANTOM_EMIT descendantRemoved(a_pMessage);
    if (m_pParent)
        m_pParent->emitDescendantRemovedCascade(a_pMessage);
}

void Message::copyChildrenTo(Message* a_pMessage) const
{
    for (auto it = m_Children.begin(); it != m_Children.end(); ++it)
    {
        a_pMessage->addChild((*it)->clone());
    }
}

Message* Message::removeLast()
{
    PHANTOM_ASSERT(m_Children.size());
    Message* pMessage = m_Children.back();
    removeChild(pMessage);
    return pMessage;
}

Message::messages_def Message::messages_def::operator()(MessageType a_eType, const phantom::Variant& a_Data,
                                                        char* a_Format, ...)
{
    va_list args;
    va_start(args, a_Format);
    m_pMessage->messageD(a_eType, a_Data, a_Format, args);
    va_end(args);
    return messages_def(m_pMessage);
}

} // namespace phantom
