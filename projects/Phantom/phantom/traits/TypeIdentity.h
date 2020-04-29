#pragma once

namespace phantom
{
template<typename T>
struct TypeIndentity
{
    using type = T;
};

template<typename T>
using TypeIndentityT = typename TypeIndentity<T>::type;

} // namespace phantom
