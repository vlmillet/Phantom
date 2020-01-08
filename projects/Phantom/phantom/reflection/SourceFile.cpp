// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
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
#include "phantom/detail/new.h"
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_LINUX
#    include <sys/stat.h>
#endif
/* *********************************************** */
namespace phantom
{
namespace reflection
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
            std::ofstream out(std::string(a_strPath.data(), a_strPath.size()));
            if (!out.is_open())
                return nullptr;
        }
    }
    else
    {
        if (!Path::CreateDirectories(Path(a_strPath).parentPath()))
            return nullptr;
        std::ofstream out(std::string(a_strPath.data(), a_strPath.size()));
        if (!out.is_open())
            return nullptr;
    }
    return PHANTOM_NEW(SourceFile)(Path::Absolute(a_strPath).canonical().genericString());
}

SourceFile* SourceFile::CreateOnDisk(Source* a_pSource, StringView a_Path, bool a_bOverwrite /*= false*/)
{
    SourceFile* pFile = CreateOnDisk(a_Path, a_bOverwrite);
    a_pSource->setSourceStream(pFile);
    return pFile;
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
    return PHANTOM_NEW(std::ifstream)(getPath().data());
}

void SourceFile::destroyInputStream(std::basic_istream<char>* a_pIn)
{
    PHANTOM_DELETE_IGNORE_CHECKS(std::ifstream) static_cast<std::ifstream*>(a_pIn);
}

SourceFile* SourceFile::clone() const
{
    return PHANTOM_NEW(SourceFile)(getPath());
}

} // namespace reflection
} // namespace phantom
