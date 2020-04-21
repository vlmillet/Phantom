// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

/* ****************** Includes ******************* */
#include <phantom/lang/Class.h>
/* **************** Declarations ***************** */

/* *********************************************** */

namespace phantom
{
namespace lang
{
/// \brief  Template dependant template instance.
/// \code{.cpp}
///             ex : MyClass<t_Ty>
/// \endcode
class PHANTOM_EXPORT_PHANTOM TemplateDependantTemplateInstance : public Type
{
    PHANTOM_DECL_TYPE;

public:
    TemplateDependantTemplateInstance(TemplateSpecialization* a_pTemplateSpecialization,
                                      const LanguageElements& a_Arguments, uint a_uiFlags = 0);
    TemplateDependantTemplateInstance(Template* a_pTemplate, const LanguageElements& a_Arguments, uint a_uiFlags = 0);

    bool isA(Type*) const override { return true; }

    Template*               getTemplate() const { return m_pTemplate; }
    TemplateSpecialization* getTemplateSpecialization() const { return m_pTemplateSpecialization; }

    const LanguageElements& getArguments() const { return m_Arguments; }
    bool                    isSame(Symbol* a_pOther) const override;

    bool isCopyable() const override { return true; }

    void getDecoration(StringBuffer& a_Buf) const;
    void getQualifiedDecoration(StringBuffer& a_Buf) const;
    void getRelativeDecoration(LanguageElement* a_pTo, StringBuffer& a_Buf) const;

    void   getDecoratedName(StringBuffer& a_Buf) const override;
    void   getQualifiedName(StringBuffer& a_Buf) const override;
    void   getQualifiedDecoratedName(StringBuffer& a_Buf) const override;
    void   getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override;
    void   getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override;
    hash64 computeLocalHash() const override;

    Class* promoteAsClass();

private:
    Template*               m_pTemplate = nullptr;
    TemplateSpecialization* m_pTemplateSpecialization = nullptr;
    LanguageElements        m_Arguments;
    mutable Class*          m_pAsClass = nullptr;
};

class PHANTOM_EXPORT_PHANTOM TemplateDependantClassPromotion : public Class
{
public:
    friend class LanguageElementVisitor;
    void visit(phantom::lang::LanguageElementVisitor* a_pVisitor, phantom::lang::VisitorData a_Data) override;

public:
    TemplateDependantClassPromotion(Type* a_pBase);

    void   getDecoratedName(StringBuffer& a_Buf) const override;
    void   getQualifiedName(StringBuffer& a_Buf) const override;
    void   getQualifiedDecoratedName(StringBuffer& a_Buf) const override;
    void   getRelativeName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override;
    void   getRelativeDecoratedName(LanguageElement* a_pTo, StringBuffer& a_Buf) const override;
    hash64 computeLocalHash() const override;

private:
    Type* m_pBase = nullptr;
};

} // namespace lang
} // namespace phantom
