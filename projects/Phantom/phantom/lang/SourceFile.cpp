// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

/* ******************* Includes ****************** */
#include "SourceFile.h"

#include "Application.h"
#include "Module.h"
#include "Package.h"
#include "Source.h"
#ifndef __DOXYGEN__
#    include <fstream>
#    include <phantom/utils/Path.h>
#endif
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_LINUX
#    include <sys/stat.h>
#endif
/* *********************************************** */
namespace phantom
{
namespace lang
{
SourceFile::SourceFile(StringView a_strAbsolutePath) : SourceStream("file:", a_strAbsolutePath)
{
    Application::Get()->_registerSourceFile(this);
}

SourceFile::~SourceFile()
{
    Application::Get()->_unregisterSourceFile(this);
}

SourceFile* SourceFile::CreateOnDisk(StringView a_strPath, bool a_bOverwrite /*= false*/)
{
    if (Path::Exists(a_strPath))
    {
        if (a_bOverwrite)
        {
            Path::Remove(a_strPath);
            std::ofstream out(String(a_strPath.data(), a_strPath.size()).c_str());
            if (!out.is_open())
                return nullptr;
        }
    }
    else
    {
        if (!Path::CreateDirectories(Path(a_strPath).parentPath()))
            return nullptr;
        std::ofstream out(String(a_strPath.data(), a_strPath.size()).c_str());
        if (!out.is_open())
            return nullptr;
    }
    return new_<SourceFile>(Path::Absolute(a_strPath).canonical().genericString());
}

time_t SourceFile::getLastChangeTime() const
{
    struct stat s = {};
    if (stat(getPath().data(), &s) == -1)
    {
        return -1;
    }
    return s.st_mtime;
}

bool SourceFile::deleteFile()
{
    return Path::Remove(getPath());
}

void SourceFile::read(String& a_Out)
{
    std::ifstream in(getPath().data());
    a_Out.append((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

void SourceFile::write(StringView a_In)
{
    std::ofstream out(getPath().data());
    out << a_In;
}

std::basic_istream<char>* SourceFile::createInputStream()
{
    return phantom::new_<std::ifstream>(getPath().data());
}

void SourceFile::destroyInputStream(std::basic_istream<char>* a_pIn)
{
    phantom::delete_<std::ifstream>(static_cast<std::ifstream*>(a_pIn));
}

SourceFile* SourceFile::clone() const
{
    return new_<SourceFile>(getPath());
}

} // namespace lang
} // namespace phantom
