// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/reflection/AnonymousSection.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace reflection
{
////////////////////////////////////////////////////////////////////////////////////////////////////
/// \class  AnonymousStruct
///
/// \brief  Represents a namespace level anonymous struct. ex: namespace [<optional>] { struct { int
/// x; int y; } GlobalStruct; }
////////////////////////////////////////////////////////////////////////////////////////////////////

class PHANTOM_EXPORT_PHANTOM AnonymousStruct : public AnonymousSection
{
    PHANTOM_DECLARE_META_CLASS(AnonymousStruct);

public:
    AnonymousStruct(Modifiers modifiers = 0, uint a_uiFlags = 0);

    AnonymousStruct* asAnonymousStruct() const override
    {
        return (AnonymousStruct*)this;
    }
};

} // namespace reflection
} // namespace phantom
