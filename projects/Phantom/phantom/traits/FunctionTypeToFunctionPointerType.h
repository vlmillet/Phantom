// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

namespace phantom
{
template<class R>
struct FunctionTypeToFunctionPointerType
{
    using type = R (*)();
};

template<class R, class... Params>
struct FunctionTypeToFunctionPointerType<R(Params...)>
{
    using type = R (*)(Params...);
};

template<class R, class... Params>
struct FunctionTypeToFunctionPointerType<R(Params..., ...)>
{
    using type = R (*)(Params..., ...);
};

#if PHANTOM_HAS_STDCALL
template<class R, class... Params>
struct FunctionTypeToFunctionPointerType<R __stdcall(Params...)>
{
    using type = R(__stdcall*)(Params...);
};
#endif

#if PHANTOM_HAS_FASTCALL
template<class R, class... Params>
struct FunctionTypeToFunctionPointerType<R __fastcall(Params...)>
{
    using type = R(__fastcall*)(Params...);
};
#endif

} // namespace phantom
