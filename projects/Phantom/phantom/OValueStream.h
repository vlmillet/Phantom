// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <iostream>
#include <phantom/StringBuffer.h>
#include <phantom/phantom.h>

namespace phantom
{
class Value;
class PHANTOM_EXPORT_PHANTOM OValueStream
{
public:
    OValueStream(std::ostream& a_OutStream, int tabSize = 4);

    OValueStream& operator<<(const Value& val);

private:
    bool isIndented(const Value& val) const;
    void indent();
    void unindent();
    void line(StringView a_Text);
    void tabs();
    void print(StringBuffer const& a_Text)
    {
        m_OutStream.write(a_Text.data(), a_Text.size());
    }
    void print(const char* a_pTxt)
    {
        m_OutStream.write(a_pTxt, strlen(a_pTxt));
    }
    void print(const char* a_pTxt, size_t a_Size)
    {
        m_OutStream.write(a_pTxt, a_Size);
    }

    std::ostream& m_OutStream;
    int           m_tabSize;
    int           m_indentation;
};
} // namespace phantom
