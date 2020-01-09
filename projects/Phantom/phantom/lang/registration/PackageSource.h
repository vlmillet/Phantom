// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <haunt>

HAUNT_STOP;

#include "registration.h"

namespace phantom
{
namespace lang
{
struct PHANTOM_EXPORT_PHANTOM SourcePusher
{
    SourcePusher(StringView a_Name);
};

struct PHANTOM_EXPORT_PHANTOM PackagePusher
{
    PackagePusher(StringView a_Name);
};

struct PHANTOM_EXPORT_PHANTOM SourceOrPackagePoper
{
    SourceOrPackagePoper(StringView a_Name);
};
} // namespace lang
} // namespace phantom

#define PHANTOM_PACKAGE(package_name)                                                              \
    namespace                                                                                      \
    {                                                                                              \
    phantom::lang::PackagePusher PHANTOM_PP_CAT(Package, __COUNTER__)(package_name);         \
    }
#define PHANTOM_SOURCE(source_name)                                                                \
    namespace                                                                                      \
    {                                                                                              \
    phantom::lang::SourcePusher PHANTOM_PP_CAT(Source, __COUNTER__)(source_name);            \
    }

#define PHANTOM_END(...) PHANTOM_PP_VARARG(_PHNTM_END_, ##__VA_ARGS__)

#define _PHNTM_END_0() _PHNTM_END_1("")

#define _PHNTM_END_1(SrcOrPck)                                                                     \
    namespace                                                                                      \
    {                                                                                              \
    phantom::lang::SourceOrPackagePoper PHANTOM_PP_CAT(SourceOrPackageEnd,                   \
                                                             __COUNTER__)(SrcOrPck);               \
    }
