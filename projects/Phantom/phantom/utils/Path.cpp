// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#include "Path.h"

#include <phantom/detail/core.h>
#include <phantom/utils/StringUtil.h>

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
#    include <direct.h>
#    include <windows.h>
#else
#    include <sys/stat.h>
#    include <unistd.h>
#    if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_ORBIS
#        include <sys/dirent.h>
#    else
#        include <dirent.h>
#    endif
#    include <cstring>
#endif
#include <fstream>

namespace phantom
{
namespace
{
int _ComparePart(const String& _s0, const String& _s1)
{
#if defined(_MSC_VER)
#    pragma warning(disable : 4996)
    return _stricmp(_s0.c_str(), _s1.c_str());
#    pragma warning(default : 4996)
#else
    return strcmp(_s0.c_str(), _s1.c_str());
#endif
}
} // namespace

const char Path::separator;

const size_t Path::npos;

Path::Path(StringView a_Path, const char a_Separator)
{
    if (a_Path.empty())
        return;
    int i = 0;
    while ((a_Path[i] == ' ' || a_Path[i] == '\t') && i < a_Path.size())
        ++i;
    if (i == a_Path.size())
        return;
    if (a_Path[i] == a_Separator)
        parts.push_back("");
    char sep[2] = {a_Separator, '\0'};
    phantom::StringUtil::Split(parts, a_Path, sep);
    if (parts.size() && parts.back().empty())
        parts.pop_back();
}

Path::Path(const char* a_Path) : Path(StringView(a_Path, strlen(a_Path))) {}

Path::Path(StringView a_Path)
{
    size_t pathSize = a_Path.size();
    if (pathSize == 0)
        return;
#if PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX
    int i = 0;
    while ((a_Path[i] == ' ' || a_Path[i] == '\t') && i < pathSize)
        ++i;
    if (i == pathSize)
        return;
    if (a_Path[i] == '/')
        parts.push_back("");
#endif
    phantom::StringUtil::Split(parts, a_Path, "\\/");
    if (parts.size() && parts.back().empty())
        parts.pop_back();
}

Path& Path::operator=(const StringView a_Path)
{
    parts.clear();
    if (a_Path.empty())
        return *this;
#if PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX
    int i = 0;
    while ((a_Path[i] == ' ' || a_Path[i] == '\t') && i < a_Path.size())
        ++i;
    if (i == a_Path.size())
        return *this;
    if (a_Path[i] == '/')
        parts.push_back("");
#endif
    phantom::StringUtil::Split(parts, a_Path, "\\/");
    if (parts.size() && parts.back().empty())
        parts.pop_back();
    return *this;
}

String Path::platformString() const
{
    if (parts.empty())
        return "";
    String str;
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    for (auto it = parts.begin(); it != parts.end(); ++it)
    {
        if (it != parts.begin())
        {
            str += '\\';
        }
        str += *it;
    }
#else
    auto first = parts.begin();
    if (first->empty())
    {
        str += '/';
        first++;
    }
    for (auto it = first; it != parts.end(); ++it)
    {
        if (it != first)
            str += '/';
        str += *it;
    }
#endif
    return str;
}

String Path::genericString() const
{
    if (parts.empty())
        return "";
    String str;
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    for (auto it = parts.begin(); it != parts.end(); ++it)
    {
        if (it != parts.begin())
        {
            str += '/';
        }
        str += *it;
    }
#else
    auto first = parts.begin();
    if (first->empty())
    {
        str += '/';
        first++;
    }
    for (auto it = first; it != parts.end(); ++it)
    {
        if (it != first)
            str += '/';
        str += *it;
    }
#endif
    return str;
}

String Path::customString(StringView a_Separator) const
{
    if (parts.empty())
        return "";
    String str;
    for (auto it = parts.begin(); it != parts.end(); ++it)
    {
        if (it != parts.begin())
        {
            str += a_Separator;
        }
        str += *it;
    }
    return str;
}

size_t Path::find(const Path& subpath) const
{
    if (subpath.empty() || empty())
        return false;
    size_t foundIndex = npos;
    size_t subindex = 0;
    for (size_t i = 0; i < parts.size();)
    {
        StringView part = parts[i];
        if (part == subpath[subindex])
        {
            if (subindex == 0)
            {
                foundIndex = i;
            }
            subindex++;
            if (subindex == subpath.size())
            {
                return foundIndex;
            }
            ++i;
        }
        else if (subindex)
        {
            subindex = 0;
        }
        else
        {
            ++i;
        }
    }
    return npos;
}

size_t Path::rfind(const Path& subpath) const
{
    if (subpath.empty() || empty())
        return false;
    size_t    foundIndex = npos;
    size_t    subindex = 0;
    ptrdiff_t i(parts.size() - subpath.size());
    for (; i >= 0;)
    {
        StringView part = parts[i];
        if (part == subpath[subindex])
        {
            if (subindex == 0)
            {
                foundIndex = i;
            }
            subindex++;
            if (subindex == subpath.size())
            {
                return foundIndex;
            }
            ++i;
        }
        else if (subindex)
        {
            i = foundIndex - 1;
            subindex = 0;
        }
        else
        {
            --i;
        }
    }
    return npos;
}

bool Path::isWindowsAbsolute() const
{
    if (parts.size() && parts[0].size() == 2)
    {
        char drive = parts[0][0];
        if ((drive >= 'A' && drive <= 'Z') || (drive >= 'a' && drive <= 'z'))
        {
            if (parts[0][1] == ':')
                return true;
        }
    }
    return false;
}

bool Path::isUnixAbsolute() const
{
    return parts.size() && parts[0].empty();
}

bool Path::isGenericAbsolute() const
{
    return isUnixAbsolute() || isWindowsAbsolute();
}

bool Path::isAbsolute() const
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    if (parts.size() && parts[0].size() == 2)
    {
        char drive = parts[0][0];
        if ((drive >= 'A' && drive <= 'Z') || (drive >= 'a' && drive <= 'z'))
        {
            if (parts[0][1] == ':')
                return true;
        }
    }
    return false;
#else
    return parts.size() && parts[0].empty();
#endif
}

#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_ORBIS
#    define lstat stat
#endif

bool Path::exists() const
{
    if (empty())
        return false;
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    DWORD dwAttrib = GetFileAttributesA(platformString().c_str());

    return (dwAttrib != INVALID_FILE_ATTRIBUTES);
#else
    struct stat path_stat;
    return lstat(platformString().c_str(), &path_stat) == 0;
#endif
}

Path::Type Path::getType() const
{
    return isDirectory() ? Type::directory
                         : isRegularFile() ? Type::regular : isSymLink() ? Type::symlink : Type::unknown;
}

bool Path::isDirectory() const
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    DWORD dwAttrib = GetFileAttributesA(platformString().c_str());

    return dwAttrib != INVALID_FILE_ATTRIBUTES && ((dwAttrib & FILE_ATTRIBUTE_DIRECTORY) != 0);
#else
    struct stat path_stat;
    if (lstat(platformString().c_str(), &path_stat) == -1)
    {
        return false;
    }
    return S_ISDIR(path_stat.st_mode);
#endif
}

bool Path::isRegularFile() const
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    DWORD dwAttrib = GetFileAttributesA(platformString().c_str());

    return (dwAttrib != INVALID_FILE_ATTRIBUTES) && ((dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == 0);
#else
    struct stat path_stat;
    if (lstat(platformString().c_str(), &path_stat) == -1)
    {
        return false;
    }
    return S_ISREG(path_stat.st_mode);
#endif
}

bool Path::isSymLink() const
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    DWORD dwAttrib = GetFileAttributesA(platformString().c_str());
    return (dwAttrib != INVALID_FILE_ATTRIBUTES) &&
    ((dwAttrib & FILE_ATTRIBUTE_REPARSE_POINT) == FILE_ATTRIBUTE_REPARSE_POINT);
#else
    struct stat path_stat;
    lstat(platformString().c_str(), &path_stat);
    return S_ISLNK(path_stat.st_mode);
#endif
}

Path Path::parentPath() const
{
    if (empty())
        return Path();
    Path p;
    p.parts = parts;
    p.parts.pop_back();
    return p;
}

Path Path::childPath(const Path& child) const
{
    PHANTOM_ASSERT(empty() || !child.isAbsolute());
    Path p;
    p.parts = parts;
    p.parts.insert(p.parts.end(), child.begin(), child.end());
    return p;
}

Path Path::absolute() const
{
    if (isAbsolute())
        return *this;
    Path abs = GetCurrentWorkingDir();
    abs.parts.insert(abs.parts.end(), parts.begin(), parts.end());
    return abs;
}

Path Path::finalPath() const
{
    if (!isAbsolute())
        return absolute().finalPath();

    HANDLE hFile;

    Path finalPath;
    char lnk[256];
    memset(lnk, 0, 256);
    for (auto part : parts)
    {
        finalPath = finalPath.childPath(part);
        String str = finalPath.platformString();
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
        DWORD dwAttrib = GetFileAttributesA(str.c_str());
        if ((dwAttrib == INVALID_FILE_ATTRIBUTES))
            return *this;
        if (((dwAttrib & FILE_ATTRIBUTE_REPARSE_POINT) == FILE_ATTRIBUTE_REPARSE_POINT))
        {
            // reparse point :
            hFile = CreateFileA(str.c_str(),     // file to open
                                GENERIC_READ,    // open for reading
                                FILE_SHARE_READ, // share for reading
                                NULL,            // default security
                                OPEN_EXISTING,   // existing file only
                                (dwAttrib & FILE_ATTRIBUTE_DIRECTORY) ? FILE_FLAG_BACKUP_SEMANTICS
                                                                      : FILE_ATTRIBUTE_NORMAL, // normal file
                                NULL);

            if (hFile == INVALID_HANDLE_VALUE)
                return *this;
            GetFinalPathNameByHandleA(hFile, lnk, 256, VOLUME_NAME_DOS);
            ::CloseHandle(hFile);
#else
        struct stat path_stat;
        lstat(str.c_str(), &path_stat);
        if (S_ISLNK(path_stat.st_mode))
        {
            readlink(str.c_str(), lnk, 256);
#endif

            if (*lnk)
            {
                finalPath = Path(lnk + 4);
            }
        }
    }

    return finalPath;
} // namespace phantom

bool Path::hasChildPath(const Path& other) const
{
    for (size_t i = 0; i < parts.size(); ++i)
    {
        if (_ComparePart(parts[i], other.parts[i]) != 0)
            return false;
    }
    return true;
}

Path Path::canonical() const
{
    if (empty())
        return *this;
    Path   c = *this;
    size_t i = 0;
    if (isAbsolute())
    {
        i = 1; // skip unix root '/' or windows 'C:'
    }
    SmallVector<bool> c_stack;
    for (; i < c.parts.size(); ++i)
    {
        if (c.parts[i] == "..")
        {
            if (c_stack.size() && c_stack.back()) // prev
            {
                c.parts.erase(c.parts.begin() + i--);
                c.parts.erase(c.parts.begin() + i--);
                c_stack.pop_back();
            }
            else
            {
                c_stack.push_back(false);
            }
        }
        else if (c.parts[i] == ".")
        {
            if (c_stack.empty() || c_stack.back()) // prev
            {
                c.parts.erase(c.parts.begin() + i--);
            }
            else
            {
                c_stack.push_back(false);
            }
        }
        else
        {
            c_stack.push_back(true);
        }
    }
    return c;
}

Path Path::subPath() const
{
    return subPath(1);
}

Path Path::subPath(size_t i) const
{
    Path p;
    PHANTOM_ASSERT(i <= parts.size());
    for (; i < parts.size(); ++i)
    {
        p.parts.push_back(parts[i]);
    }
    return p;
}

Path Path::_relative(const Path& to) const
{
    Path relative_path;
    bool diffFound = false;
    if (to.parts.size() > parts.size()) //
    {
        for (size_t i = 0; i < to.parts.size(); ++i)
        {
            if (i < parts.size())
            {
                if (diffFound || (_ComparePart(parts[i], to.parts[i]) != 0))
                {
                    diffFound = true;
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
                    if (i == 0) // disk are not isSame on windows => relative do not exist (C: != D:
                                // for example)
                    {
                        return *this; // returns absolute
                    }
#endif
                    relative_path = Path("..").childPath(relative_path).childPath(parts[i]);
                }
            }
            else
            {
                relative_path = Path("..").childPath(relative_path);
            }
        }
    }
    else
    {
        for (size_t i = 0; i < parts.size(); ++i)
        {
            if (i < to.parts.size())
            {
                if (diffFound || (_ComparePart(parts[i], to.parts[i]) != 0))
                {
                    diffFound = true;
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
                    if (i == 0) // disk are not isSame on windows => relative do not exist (C: != D:
                                // for example)
                    {
                        return *this; // returns absolute
                    }
#endif
                    relative_path = Path("..").childPath(relative_path).childPath(parts[i]);
                }
            }
            else
            {
                relative_path = relative_path.childPath(parts[i]);
            }
        }
    }
    return relative_path;
}

bool Path::ListDirectoryEntries(const Path& p, DirectoryEntries& entries, std::error_code& ec,
                                bool recursive /*= false*/)
{
    if (!p.exists() || !p.isDirectory())
        return false;
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    WIN32_FIND_DATAA findFileData;
    HANDLE           h = ::FindFirstFileA((p.platformString() + "\\*.*").c_str(), &findFileData);
    if (h != INVALID_HANDLE_VALUE)
    {
        do
        {
            Type type = ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
            ? Type::regular
            : Type::directory;
            if (type == Type::directory &&
                (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0))
                continue;
            DirectoryEntry de(p.childPath(findFileData.cFileName), type);
            entries.push_back(de);
            if (type == Type::directory && recursive)
            {
                if (!(ListDirectoryEntries(de.path(), entries, ec, true)))
                {
                    entries.clear();
                    return false;
                }
            }
        } while (::FindNextFileA(h, &findFileData) != 0);
        ::FindClose(h);
        return true;
    }
    ec.assign(GetLastError(), std::system_category());
    return false;

#elif PHANTOM_OPERATING_SYSTEM != PHANTOM_OPERATING_SYSTEM_ORBIS
    DIR*           dp;
    struct dirent* ep;
    dp = opendir(p.platformString().c_str());

    if (dp != NULL)
    {
        while ((ep = readdir(dp)))
        {
            Type type = (ep->d_type == DT_DIR)
            ? Type::directory
            : (ep->d_type == DT_REG) ? Type::regular : (ep->d_type == DT_LNK) ? Type::symlink : Type::unknown;
            if (type == Type::directory && (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0))
                continue;
            entries.push_back(DirectoryEntry(p.childPath(ep->d_name), type));
            if (type == Type::directory && recursive)
            {
                if (!(ListDirectoryEntries(entries.back().path(), entries, ec, true)))
                {
                    entries.clear();
                    return false;
                }
            }
        }

        (void)closedir(dp);
        return true;
    }
    ec.assign(errno, std::system_category());
    return false;
#else
    return false;
#endif
}

bool Path::ListDirectoryEntries(const Path& p, DirectoryEntries& entries, bool recursive /*= false*/)
{
    std::error_code ec;
    return ListDirectoryEntries(p, entries, ec, recursive);
}

Path Path::TempFolder()
{
#if PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX
    return "/tmp/";
#elif PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_WINDOWS
    char buffer[256];
    GetTempPathA(256, buffer);
    return (buffer);
#else
#    error unsupported OS family
#endif
}

bool Path::Equivalent(const Path& p0, const Path& p1)
{
#if defined(PHANTOM_DEV)
#    pragma message(PHANTOM_TODO " : optimize Path::Equivalent")
#endif
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    String abs0 = p0.absolute().canonical().genericString();
    String abs1 = p1.absolute().canonical().genericString();
    auto   tolower = [](char c) -> char { return char(::tolower(c)); };
    std::transform(abs0.begin(), abs0.end(), abs0.begin(), tolower);
    std::transform(abs1.begin(), abs1.end(), abs1.begin(), tolower);
    return Absolute(abs0) == Absolute(abs1);
#else
    return p0.absolute().canonical() == p1.absolute().canonical();
#endif
}

bool Path::Copy(const Path& dst, const Path& src, std::error_code& ec)
{
    std::ifstream src_(src.platformString().c_str(), std::ios::binary);
    if (src_.fail())
    {
        ec.assign(1, std::iostream_category());
        return false;
    }
    std::ofstream dst_(dst.platformString().c_str(), std::ios::binary);
    if (dst_.fail())
    {
        ec.assign(1, std::iostream_category());
        return false;
    }

    dst_ << src_.rdbuf();
    return true;
}

bool Path::Copy(const Path& dst, const Path& src)
{
    std::error_code ec;
    return Copy(dst, src, ec);
}

bool Path::Rename(const Path& dst, const Path& src, std::error_code&)
{
    return ::rename(src.platformString().c_str(), dst.platformString().c_str());
}

bool Path::Rename(const Path& dst, const Path& src)
{
    std::error_code ec;
    return Copy(dst, src, ec);
}

bool Path::Remove(const Path& p, std::error_code& ec)
{
    bool result = true;
    if (p.isDirectory())
    {
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
        result = RemoveDirectoryA(p.platformString().c_str()) != 0;
        if (!result)
        {
            ec.assign(GetLastError(), std::system_category());
        }
#else
        result = rmdir(p.platformString().c_str()) == 0;
#endif
    }
    else
    {
#pragma warning(disable : 4996)
        if (IsSymLink(p))
#if defined(_MSC_VER)
            result = ::_unlink(p.platformString().c_str());
#else
            result = ::unlink(p.platformString().c_str());
#endif
        else
            result = ::remove(p.platformString().c_str()) == 0;
#pragma warning(default : 4996)
        if (!result)
        {
            ec.assign(errno, std::system_category());
        }
    }

    return result;
}

bool Path::Remove(const Path& p)
{
    std::error_code ec;
    return Remove(p, ec);
}

bool Path::ResizeFile(const Path& p, size_t size, std::error_code& ec)
{
    bool result;
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    HANDLE h(CreateFileA(p.platformString().c_str(), GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0));
    LARGE_INTEGER sz;
    sz.QuadPart = size;
    result = (h != INVALID_HANDLE_VALUE) && ::SetFilePointerEx(h, sz, 0, FILE_BEGIN) && ::SetEndOfFile(h);
    CloseHandle(h);
    if (!result)
    {
        ec.assign(GetLastError(), std::system_category());
    }
#else
    result = ::truncate(p.platformString().c_str(), size) == 0;
    if (!result)
    {
        ec.assign(errno, std::system_category());
    }
#endif
    return result;
}

bool Path::ResizeFile(const Path& p, size_t size)
{
    std::error_code ec;
    return ResizeFile(p, size, ec);
}

Path Path::GetCurrentWorkingDir()
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    char buffer[512];
    return _getcwd(buffer, 512);
#elif PHANTOM_OPERATING_SYSTEM != PHANTOM_OPERATING_SYSTEM_ORBIS
    char buffer[512];
    return getcwd(buffer, 512);
#else
    return "/";
#endif
}

void Path::SetCurrentWorkingDir(const Path& _path)
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    _chdir(_path.absolute().genericString().c_str());
#elif PHANTOM_OPERATING_SYSTEM != PHANTOM_OPERATING_SYSTEM_ORBIS
    chdir(_path.absolute().genericString().c_str());
#else
    PHANTOM_LOG(Error, "no chdir on this platform");
#endif
}

bool Path::RemoveAll(const Path& p, std::error_code& ec)
{
    if (p.isDirectory())
    {
        DirectoryEntries entries;
        bool             result = ListDirectoryEntries(p, entries, ec);
        for (auto it = entries.begin(); it != entries.end(); ++it)
        {
            result = RemoveAll(it->path(), ec) && result;
        }
        return Remove(p, ec) && result;
    }
    return Remove(p, ec);
}

bool Path::RemoveAll(const Path& p)
{
    std::error_code ec;
    return RemoveAll(p, ec);
}

bool Path::CreateDirectories(const Path& p, std::error_code& ec)
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    String strpath;
    auto   first = p.begin();
    if (p.isAbsolute())
    {
        strpath += *first++;
    }
    for (auto it = first; it != p.end(); ++it)
    {
        if (it != p.begin())
            strpath += '/';
        strpath += *it;
        BOOL result = CreateDirectoryA(strpath.c_str(), NULL);
        int  error = 0;
        if ((result == FALSE) && (((error = GetLastError()) != ERROR_ALREADY_EXISTS) || !(IsDirectory(strpath))))
        {
            ec.assign(error, std::system_category());
            return false;
        }
    }
    return true;

#else

    String strpath;
    auto   first = p.begin();
    if (p.isAbsolute())
    {
        strpath += '/';
        first++;
    }
    for (auto it = first; it != p.end(); ++it)
    {
        if (it != first)
            strpath += '/';
        strpath += *it;
        int result = mkdir(strpath.c_str(), ACCESSPERMS);
        if (result == 0 || (errno == EEXIST && IsDirectory(strpath)))
            continue;
        ec.assign(errno, std::system_category());
        return false;
    }
    return true;
#endif
}
bool Path::CreateDirectories(const Path& p)
{
    std::error_code ec;
    return CreateDirectories(p, ec);
}
} // namespace phantom
