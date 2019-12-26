// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/phantom.h>
/* **************** Declarations ***************** */
/* *********************************************** */

namespace phantom
{
namespace reflection
{
class Source;

/// \brief  Represents a code position defined by a line, a column and an optional source.
struct PHANTOM_EXPORT_PHANTOM CodePosition
{
    CodePosition() = default;
    CodePosition(uint16 line, uint16 column) : line(line), column(column)
    {
    }

    bool operator==(const CodePosition& other) const
    {
        return line == other.line AND column == other.column;
    }

    bool operator!=(const CodePosition& other) const
    {
        return !operator==(other);
    }

    bool operator<(const CodePosition& other) const
    {
        return ((line < other.line) OR((line == other.line) AND column < other.column));
    }

    bool operator>(const CodePosition& other) const
    {
        return ((line > other.line) OR((line == other.line) AND column > other.column));
    }

    bool isValid() const
    {
        return line && column;
    }

    uint16 line = 0;
    uint16 column = 0;
};

/// \brief  Represents a code position defined by a line, a column and an optional source.
struct PHANTOM_EXPORT_PHANTOM CodeLocation
{
    CodeLocation() = default;
    CodeLocation(Source* a_pSource, const CodePosition& pos) : source(a_pSource), position(pos)
    {
    }

    bool operator==(const CodeLocation& other) const
    {
        return position == other.position AND source == other.source;
    }

    bool operator!=(const CodeLocation& other) const
    {
        return !operator==(other);
    }

    bool operator<(const CodeLocation& other) const
    {
        return ((source < other.source) OR((source == other.source) AND position < other.position));
    }

    bool operator>(const CodeLocation& other) const
    {
        return ((source > other.source) OR((source == other.source) AND position > other.position));
    }

    bool isValid() const
    {
        return source && position.isValid();
    }

    Source*      source = nullptr;
    CodePosition position;
};

struct PHANTOM_EXPORT_PHANTOM CodeRange
{
    bool operator==(const CodeRange& other) const
    {
        return begin == other.begin && end == other.end;
    }

    bool operator!=(const CodeRange& other) const
    {
        return !operator==(other);
    }

    bool operator<(const CodeRange& other) const
    {
        return begin < other.begin OR(begin == other.begin AND end < other.end);
    }

    bool operator>(const CodeRange& other) const
    {
        return operator!=(other) && !operator<(other);
    }

    bool containsCodePosition(const CodePosition& pos) const
    {
        return (pos.line >= begin.line AND pos.line <= end.line)
        AND(((pos.line != begin.line) OR(pos.column >= begin.column))
            AND((pos.line != end.line) OR(pos.column <= end.column)));
    }

    bool containsLine(uint16 a_Line) const
    {
        return a_Line >= begin.line AND a_Line <= end.line;
    }

    CodeRange combine(const CodeRange& other) const
    {
        CodeRange range;
        if (other.begin.isValid() && !begin.isValid())
            range.begin = other.begin;
        else if (!other.begin.isValid() && begin.isValid())
            range.begin = begin;
        else if (other.begin < begin)
            range.begin = other.begin;
        else
            range.begin = begin;

        if (other.end.isValid() && !end.isValid())
            range.end = other.end;
        else if (!other.end.isValid() && end.isValid())
            range.end = end;
        else if (other.end > end)
            range.end = other.end;
        else
            range.end = end;

        return range;
    }

    inline bool isValid() const
    {
        return begin.isValid() && end.isValid();
    }

    CodeRange operator|(const CodeRange& other) const
    {
        return combine(other);
    }

    CodePosition begin;
    CodePosition end;
};

/// \brief  Represents a code range in a source.
struct PHANTOM_EXPORT_PHANTOM CodeRangeLocation
{
    CodeRangeLocation() = default;
    CodeRangeLocation(Source* a_pSource, const CodeRange& a_Range)
        : source(a_pSource), range(a_Range)
    {
    }
    bool operator==(const CodeRangeLocation& other) const
    {
        return range == other.range && source == other.source;
    }

    bool operator!=(const CodeRangeLocation& other) const
    {
        return !operator==(other);
    }

    bool operator<(const CodeRangeLocation& other) const
    {
        return range < other.range;
    }

    bool operator>(const CodeRangeLocation& other) const
    {
        return range > other.range;
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this location contains the given line.
    ///
    /// \param  The line to test.
    ///
    /// \return true if it contains the line, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool containsLine(int a_iLine) const
    {
        return range.containsLine(uint16(a_iLine));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Query if this location contains the given code position.
    ///
    /// \param  pos The position to test.
    ///
    /// \return true if it contains the position, false if not.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    bool containsCodePosition(const CodePosition& pos) const
    {
        return range.containsCodePosition(pos);
    }

    inline CodeRangeLocation operator|(const CodeRangeLocation& other) const // union
    {
        CodeRangeLocation union_ = *this;
        if (other.range.begin < range.begin)
            union_.range.begin = other.range.begin;
        if (range.end < other.range.end)
            union_.range.end = other.range.end;
        return union_;
    }

    Source*   source;
    CodeRange range;
};

typedef phantom::SmallVector<CodeRangeLocation> CodeRangeLocations;
typedef phantom::SmallVector<CodePosition>      CodePositions;
typedef phantom::SmallVector<CodeRange>         CodeRanges;

} // namespace reflection
} // namespace phantom
