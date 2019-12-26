// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <String>
#include <exception>
/* *********************************************** */

namespace phantom
{
namespace exception
{
/// @cond INTERNAL

#if (PHANTOM_COMPILER == PHANTOM_COMPILER_VISUAL_STUDIO)
typedef std::exception Exception;
#else
class Exception : public std::exception
{
public:
    Exception(void)
    {
    }
    Exception(const char* const& _What)
    {
    }
};
#endif

class InvalidArgumentException : public Exception
{
public:
    InvalidArgumentException() : Exception("Invalid argument")
    {
    }
    InvalidArgumentException(const char* const& _What) : Exception(_What)
    {
    }
};

class UnsupportedMethodException : public Exception
{
public:
    UnsupportedMethodException()
    {
    }
    UnsupportedMethodException(const char* const& _What) : Exception(_What)
    {
    }
};

/// @endcond

} // namespace exception

namespace reflection
{
class ParsingException : public exception::Exception
{
public:
    ParsingException()
    {
    }
    ParsingException(const char* const& _What) : phantom::exception::Exception(_What)
    {
    }
};

class RuntimeException : public exception::Exception
{
public:
    RuntimeException()
    {
    }
    RuntimeException(const char* const& _What) : phantom::exception::Exception(_What)
    {
    }
};
} // namespace reflection

} // namespace phantom
