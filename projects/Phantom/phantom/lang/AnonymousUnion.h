// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/AnonymousSection.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class  AnonymousUnion
///
/// \brief  Represents a namespace level anonymous union. ex: namespace [<optional>] { union { int
/// x; int y; } GlobalUnion; }
////////////////////////////////////////////////////////////////////////////////////////////////////

class PHANTOM_EXPORT_PHANTOM AnonymousUnion : public AnonymousSection
{
    PHANTOM_DECLARE_META_CLASS(AnonymousUnion);

public:
    AnonymousUnion(Modifiers modifiers = 0, uint a_uiFlags = 0);

    AnonymousUnion* asAnonymousUnion() const override
    {
        return (AnonymousUnion*)this;
    }
};

} // namespace lang
} // namespace phantom
