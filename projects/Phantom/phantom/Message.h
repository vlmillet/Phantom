// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once
#pragma once

/* ****************** Includes ******************* */
#include <phantom/Signal.h>
#include <phantom/Variant.h>
#include <phantom/phantom.h>
/* *********************************************** */

namespace phantom
{
/* ************************************************ *
 * \class Message
 * \brief Message.
 * ************************************************ */
class PHANTOM_EXPORT_PHANTOM Message
{
public:
    struct messages_def
    {
        messages_def(Message* a_pMessage) : m_pMessage(a_pMessage)
        {
        }
        messages_def operator()(MessageType a_eType, const phantom::Variant& a_Data, char* a_Format,
                                ...);

    protected:
        Message* m_pMessage;
    };

    //================================================
    // Enumerations
    //================================================

    //================================================
    // Constructors / Destructor
    //================================================

    Message(MessageType a_eType = MessageType::Undefined, StringView a_strText = "");
    ~Message();

    //================================================
    // Accessors
    //================================================

    Message* getRootMessage() const;

    Message* getParent() const;
    Message* getChild(size_t a_uiIndex) const;
    size_t   getChildCount() const;

    Messages const& getChildren() const
    {
        return m_Children;
    }

    const phantom::Variant& getData() const;
    void                    setData(const phantom::Variant& a_Data);

    MessageType getType() const;
    void        setType(MessageType a_eType);
    MessageType getMostValuableMessageType() const;

    StringView getText() const;
    void       setText(StringView a_strText);

    StringView getCategory() const;
    void       setCategory(StringView a_strCategory);

    //================================================
    // Operations
    //================================================

    virtual void addChild(Message* a_pChild);
    virtual void removeChild(Message* a_pChild);

    Message* clone(bool recursive = true) const;

    Message* errorD(const Variant& a_Data, const char* a_Format, ...);
    Message* warningD(const Variant& a_Data, const char* a_Format, ...);
    Message* informationD(const Variant& a_Data, const char* a_Format, ...);
    Message* successD(const Variant& a_Data, const char* a_Format, ...);

    Message* error(const char* a_Format, ...);
    Message* warning(const char* a_Format, ...);
    Message* information(const char* a_Format, ...);
    Message* success(const char* a_Format, ...);

    void format(const char* a_format, ...);
    void format(const char* a_format, va_list args);

    void open();

    Message* messageD(MessageType a_eType, const phantom::Variant& a_Data, const char* a_Format,
                      ...);
    Message* messageD(MessageType a_eType, const phantom::Variant& a_Data, const char* a_Format,
                      va_list args);
    Message* message(MessageType a_eType, const char* a_Format, va_list args);

    messages_def messages()
    {
        return messages_def(this);
    }

    void copyChildrenTo(Message* a_pMessage) const;

    void clear();

    Message* removeLast();

protected:
    //================================================
    // Private Operations
    //================================================

    void emitMessageOpenedCascade(Message* a_pMessage);

    EmbeddedRtti PHANTOM_CUSTOM_EMBEDDED_RTTI_FIELD;

public:
    Signal<void(Message*)> childAdded;
    Signal<void(Message*)> childRemoved;
    Signal<void(Message*)> descendantAdded;
    Signal<void(Message*)> descendantRemoved;
    Signal<void()>         opened;
    Signal<void(Message*)> messageOpened;

private:
    void emitDescendantAddedCascade(Message* a_pMessage);
    void emitDescendantRemovedCascade(Message* a_pMessage);

protected:
    Message*    m_pParent;
    Messages    m_Children;
    MessageType m_eType;
    String      m_strCategory;
    String      m_strText;
    Variant     m_Data;
    uint        m_Flags;
};

} // namespace phantom
