// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include "plugin.h"

#include <istream>

namespace phantom
{
class Value;
class PHANTOM_EXPORT_PHANTOM IValueStream
{
public:
    static const char* last_error();

    IValueStream(std::istream& a_InStream);
    ~IValueStream();

    bool fail() const;

    bool operator>>(Value& value);

private:
    struct IValueStreamPrivate* _private;
    std::istream&               m_InStream;
    bool                        error = false;
};
} // namespace phantom
