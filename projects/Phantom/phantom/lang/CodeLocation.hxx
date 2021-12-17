#pragma once

// haunt {

// clang-format off

#include "CodeLocation.h"

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
#include <phantom/struct>
#include <phantom/method>
#include <phantom/constructor>
#include <phantom/field>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include <phantom/utils/SmallVector.hxx>

#include <phantom/template-only-pop>

namespace phantom {
namespace lang {
PHANTOM_PACKAGE("phantom.lang")
    PHANTOM_SOURCE("CodeLocation")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_STRUCT(CodePosition)
        {
            this_()
            .constructor<void(), default_>()
            .constructor<void(uint16, uint16)>()({"line","column"})
            .method<bool(const CodePosition&) const>("operator==", &_::operator==)({"other"})
            .method<bool(const CodePosition&) const>("operator!=", &_::operator!=)({"other"})
            .method<bool(const CodePosition&) const>("operator<", &_::operator<)({"other"})
            .method<bool(const CodePosition&) const>("operator>", &_::operator>)({"other"})
            .method<bool() const>("isValid", &_::isValid)
            .field("line", &_::line)
            .field("column", &_::column)
            ;
        }
        PHANTOM_STRUCT(CodeLocation)
        {
            this_()
            .constructor<void(), default_>()
            .constructor<void(Source*, const CodePosition&)>()({"a_pSource","pos"})
            .method<bool(const CodeLocation&) const>("operator==", &_::operator==)({"other"})
            .method<bool(const CodeLocation&) const>("operator!=", &_::operator!=)({"other"})
            .method<bool(const CodeLocation&) const>("operator<", &_::operator<)({"other"})
            .method<bool(const CodeLocation&) const>("operator>", &_::operator>)({"other"})
            .method<bool() const>("isValid", &_::isValid)
            .field("source", &_::source)
            .field("position", &_::position)
            ;
        }
        PHANTOM_STRUCT(CodeRange)
        {
            this_()
            .method<bool(const CodeRange&) const>("operator==", &_::operator==)({"other"})
            .method<bool(const CodeRange&) const>("operator!=", &_::operator!=)({"other"})
            .method<bool(const CodeRange&) const>("operator<", &_::operator<)({"other"})
            .method<bool(const CodeRange&) const>("operator>", &_::operator>)({"other"})
            .method<bool(const CodePosition&) const>("containsCodePosition", &_::containsCodePosition)({"pos"})
            .method<bool(uint16) const>("containsLine", &_::containsLine)({"a_Line"})
            .method<CodeRange(const CodeRange&) const>("combine", &_::combine)({"other"})
            .method<bool() const>("isValid", &_::isValid)
            .method<CodeRange(const CodeRange&) const>("operator|", &_::operator|)({"other"})
            .field("begin", &_::begin)
            .field("end", &_::end)
            ;
        }
        PHANTOM_STRUCT(CodeRangeLocation)
        {
            this_()
            .constructor<void(), default_>()
            .constructor<void(Source*, const CodeRange&)>()({"a_pSource","a_Range"})
            .method<bool(const CodeRangeLocation&) const>("operator==", &_::operator==)({"other"})
            .method<bool(const CodeRangeLocation&) const>("operator!=", &_::operator!=)({"other"})
            .method<bool(const CodeRangeLocation&) const>("operator<", &_::operator<)({"other"})
            .method<bool(const CodeRangeLocation&) const>("operator>", &_::operator>)({"other"})
            .method<bool(int) const>("containsLine", &_::containsLine)({"a_iLine"})
            .method<bool(const CodePosition&) const>("containsCodePosition", &_::containsCodePosition)({"pos"})
            .method<::phantom::lang::CodeRangeLocation(const CodeRangeLocation&) const>("operator|", &_::operator|)({"other"})
            .field("source", &_::source)
            .field("range", &_::range)
            ;
        }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<CodeRangeLocations>("CodeRangeLocations"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<CodePositions>("CodePositions"); }
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<CodeRanges>("CodeRanges"); }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("CodeLocation")
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
