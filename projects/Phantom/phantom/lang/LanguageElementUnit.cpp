#include "LanguageElementUnit.h"

namespace phantom
{
namespace lang
{
void LanguageElementUnit::Delete(LanguageElement* a_pElem)
{
    a_pElem->rtti.metaClass->unregisterInstance(a_pElem->rtti.instance);
    a_pElem->terminate();
    a_pElem->~LanguageElement();
    // --free(...)-- we never deallocate individually
    // (we always deallocate per-source(script) or per-module(c++) chunks for speed)
}

} // namespace lang
} // namespace phantom
