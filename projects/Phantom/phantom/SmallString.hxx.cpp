// license [
// This file is part of the Phantom project. Copyright 2015-2019 Vivien Millet.
// Distributed under license available here at http://www.wiwila.com/tools/phantom/license/
// ]

#include "SmallString.hxx"

#if defined(_PHANTOM_MEASURE_AVERAGE_STRING_SIZE)
namespace phantom
{
PHANTOM_EXPORT_PHANTOM size_t SmallStringTotalSize = 0;
PHANTOM_EXPORT_PHANTOM size_t SmallStringTotalCount = 0;
} // namespace phantom
#endif

#if defined(_MSC_VER)
#    pragma warning(push, 0)
#elif defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wall"
#    pragma clang diagnostic ignored "-Wextra"
#endif

PHANTOM_REGISTER(ClassTypes)
{
    ::phantom::reflection::BuiltInTypes::TYPE_STRING = (::phantom::reflection::Class*)PHANTOM_TYPEOF(phantom::String);
}
