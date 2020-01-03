// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

#include <phantom/detail/phantom.h>
#include <phantom/typeof>
#include <phantom/utils/SmallVector.h>

namespace phantom
{
struct DirectoryEntry;

typedef SmallVector<DirectoryEntry> DirectoryEntries;

struct PHANTOM_EXPORT_PHANTOM Path
{
    static bool ListDirectoryEntries(const Path& p, DirectoryEntries& entries, std::error_code& ec,
                                     bool recursive = false);
    static bool ListDirectoryEntries(const Path& p, DirectoryEntries& entries, bool recursive = false);

    static bool CreateDirectories(const Path& p, std::error_code& ec);
    static bool CreateDirectories(const Path& p);

    static Path TempFolder();

    static bool Exists(const Path& p)
    {
        return p.exists();
    }

    static bool IsAbsolute(const Path& p)
    {
        return p.isAbsolute();
    }

    static bool IsGenericAbsolute(const Path& p)
    {
        return p.isGenericAbsolute();
    }

    static bool IsWindowsAbsolute(const Path& p)
    {
        return p.isWindowsAbsolute();
    }

    static bool IsUnixAbsolute(const Path& p)
    {
        return p.isUnixAbsolute();
    }

    static bool IsDirectory(const Path& p)
    {
        return p.isDirectory();
    }

    static bool IsRegularFile(const Path& p)
    {
        return p.isRegularFile();
    }

    static bool IsSymLink(const Path& p)
    {
        return p.isSymLink();
    }

    static Path Absolute(const Path& p)
    {
        return p.absolute();
    }

    static bool Equivalent(const Path& p0, const Path& p1);

    static bool Copy(const Path& dst, const Path& src, std::error_code& ec);

    static bool Copy(const Path& dst, const Path& src);

    static bool Rename(const Path& dst, const Path& src, std::error_code& ec);

    static bool Rename(const Path& dst, const Path& src);

    static bool Remove(const Path& p, std::error_code& ec);

    static bool Remove(const Path& p);

    static bool RemoveAll(const Path& p, std::error_code& ec);

    static bool RemoveAll(const Path& p);

    static bool ResizeFile(const Path& p, size_t size, std::error_code& ec);

    static bool ResizeFile(const Path& p, size_t size);

    static Path GetCurrentWorkingDir();
    static void SetCurrentWorkingDir(const Path& _path);

    enum class Type
    {
        unknown,
        directory,
        regular,
        symlink,
    };
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS
    static const char separator = '\\';
#else
    static const char separator = '/';
#endif
    static const size_t npos = size_t(-1);

    Path()
    {
    }
    Path(StringView a_Path);
    Path(String const& a_Path) : Path(StringView(a_Path))
    {
    }
    Path(StringView a_Path, const char separator);
    Path(const char* a_Path);

    Path& operator=(const StringView a_Path);

    String& front()
    {
        return parts.front();
    }
    StringView front() const
    {
        return parts.front();
    }
    String& back()
    {
        return parts.back();
    }
    StringView back() const
    {
        return parts.back();
    }

    void popBack()
    {
        parts.pop_back();
    }

    void append(phantom::Path const& _child)
    {
        for (auto& c : _child)
            parts.push_back(c);
    }

    inline void removeExtension()
    {
        if (size())
            back() = stem();
    }

    String& operator[](size_t i)
    {
        return parts[i];
    }
    StringView operator[](size_t i) const
    {
        return parts[i];
    }

    Strings::const_iterator begin() const
    {
        return parts.begin();
    }

    Strings::const_iterator end() const
    {
        return parts.end();
    }

    String platformString() const;

    String genericString() const;

    String customString(StringView a_Separator) const;

    size_t find(const Path& subpath) const;
    size_t rfind(const Path& subpath) const;

    bool isAbsolute() const;

    bool isGenericAbsolute() const;

    bool isWindowsAbsolute() const;

    bool isUnixAbsolute() const;

    bool exists() const;

    void clear()
    {
        parts.clear();
    }

    Type getType() const;

    bool isDirectory() const;

    bool isRegularFile() const;

    bool isSymLink() const;

    Path parentPath() const;

    Path childPath(const Path& child) const;

    inline bool empty() const;

    inline bool isRoot() const;

    inline StringView filename() const;

    inline StringView stem() const;

    inline StringView extension() const;

    Path absolute() const;

    inline bool operator==(const Path& other) const;

    inline bool operator!=(const Path& other) const
    {
        return !operator==(other);
    }

    inline bool operator<(const Path& other) const
    {
        return absolute().genericString() < other.absolute().genericString();
    }

    size_t size() const
    {
        return parts.size();
    }

    inline Path relative(const Path& to) const
    {
        if (to.isAbsolute())
        {
            if (!isAbsolute())
                return absolute()._relative(to);
            return _relative(to);
        }
        else
        {
            if (!isAbsolute())
                return absolute()._relative(to.absolute());
            return _relative(to.absolute());
        }
    }

    bool hasChildPath(const Path& other) const;

    Path canonical() const;

    Path subPath() const;
    Path subPath(size_t i) const;

private:
    Path _relative(const Path& to) const;

private:
    Strings parts;
};

struct PHANTOM_EXPORT_PHANTOM DirectoryEntry
{
    DirectoryEntry() : m_Type(Path::Type::unknown)
    {
    }
    DirectoryEntry(const Path& p, Path::Type t) : m_EntryPath(p), m_Type(t)
    {
    }
    const Path& path() const
    {
        return m_EntryPath;
    }
    bool isRegularFile() const
    {
        return m_Type == Path::Type::regular;
    }
    bool isDirectory() const
    {
        return m_Type == Path::Type::directory;
    }
    bool isSymLink() const
    {
        return m_Type == Path::Type::symlink;
    }

private:
    Path       m_EntryPath;
    Path::Type m_Type;
};

inline bool Path::empty() const
{
    return parts.empty();
}

inline bool Path::isRoot() const
{
    return parts.size() == 1 AND isAbsolute();
}

inline StringView Path::filename() const
{
    return parts.size() ? StringView(parts.back()) : StringView();
}

inline StringView Path::stem() const
{
    if (empty())
        return "";
    StringView back(parts.back());
    return back.substr(0, back.find_last_of('.'));
}

inline StringView Path::extension() const
{
    if (empty())
        return "";
    StringView back(parts.back());
    size_t     ext = back.find_last_of('.');
    if (ext != String::npos)
        return back.substr(ext);
    return StringView();
}
inline bool Path::operator==(const Path& other) const
{
    if (parts.size() != other.parts.size())
        return false;
    for (size_t i = 0; i < other.parts.size(); ++i)
    {
        if (parts[i] != other.parts[i])
            return false;
    }
    return true;
}

} // namespace phantom
