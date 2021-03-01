// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "SourceFile.h"
#include "SourceStream.h"
/* *********************************************** */

namespace phantom
{
namespace lang
{
phantom::lang::SourceStream* SourceStream::CreateFromUrl(StringView a_Url)
{
    if (a_Url.find("file:") == 0)
        return SourceFile::CreateOnDisk(a_Url.substr(5), false);
    return nullptr;
}

} // namespace lang
} // namespace phantom
