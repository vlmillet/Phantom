// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Reference.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Represents an l-value reference type.
/// \code{.cpp}
///     ex: int&, float&, unsigned const&, etc...
/// \endcode
class PHANTOM_EXPORT_PHANTOM LValueReference : public Reference
{
    PHANTOM_DECL_TYPE;

    PHANTOM_DECLARE_META_CLASS(LValueReference);

public:
    friend class Type;

protected:
    LValueReference(Type* a_pReferencedType);

public:
    LValueReference* asClassLValueReference() const override;
    LValueReference* asConstClassLValueReference() const override;

    Type* replicate(Type* a_pInput) const override;

    bool isCopyable() const override { return true; }

    bool partialAccepts(Type* a_pType, size_t& a_Score, PlaceholderMap& a_DeducedConstants) const override;

    bool isSame(Symbol* a_pOther) const override;

    hash64 computeLocalHash() const override;
};

} // namespace lang
} // namespace phantom
