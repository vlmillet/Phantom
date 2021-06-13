#pragma once

#include <phantom/plugin.h>

namespace phantom
{
namespace lang
{
struct PHANTOM_EXPORT_PHANTOM TemplateSubstitution
{
public:
    TemplateSubstitution();
    TemplateSubstitution(TemplateSpecialization* a_pInstantiation);
    void insert(Placeholder* a_pPlaceholder, LanguageElement* a_pArgument);

    void substituteVariadic(Placeholder* a_pPlaceholder, LanguageElement* a_pArgument);

    Placeholder*     getPlaceholder(size_t i) const { return m_Placeholders[i]; }
    LanguageElement* getArgument(size_t i) const { return m_Arguments[i]; }

    LanguageElements const& getArguments() const;

    TemplateSubstitution splitVariadics(Placeholder* a_pPH, LanguageElements& a_VariadicExpansion) const;

    LanguageElement* getArgument(Placeholder* a_pPlaceholder) const;

    void setInstantiation(TemplateSpecialization* a_pSpec);

    TemplateSpecialization* getInstantiation() const { return m_pInstantiation; }

    size_t size() const { return m_Arguments.size(); }

private:
    TemplateSpecialization* m_pInstantiation;
    Placeholders            m_Placeholders;
    LanguageElements        m_Arguments;
};
} // namespace lang
} // namespace phantom
