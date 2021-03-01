// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include "HasField.h"

#include <phantom/fwd.h>

PHANTOM_DEFINE_HAS_FIELD_NO_BASE_MACRO_PARAM(rtti);

namespace phantom
{
/// @cond INTERNAL

template<typename t_Ty>
struct HasEmbeddedRtti : public PHANTOM_PP_CAT(HasField_, rtti)<t_Ty, RTTI>
{
};

template<typename t_Ty>
struct HasEmbeddedProxyRtti : public PHANTOM_PP_CAT(HasField_, rtti)<t_Ty, EmbeddedProxyRtti>
{
};

template<typename t_Ty>
struct HasEmbeddedRttiNoBase : public PHANTOM_PP_CAT(HasFieldNoBase_, rtti)<t_Ty, RTTI>
{
};

template<typename t_Ty>
struct HasEmbeddedProxyRttiNoBase : public PHANTOM_PP_CAT(HasFieldNoBase_, rtti)<t_Ty, EmbeddedProxyRtti>
{
};
} // namespace phantom
