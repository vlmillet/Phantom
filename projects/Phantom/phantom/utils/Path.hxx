#pragma once

// haunt {

// clang-format off

#include "Path.h"

#if defined(_MSC_VER)
#   pragma warning(push, 0)
#elif defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wall"
#   pragma clang diagnostic ignored "-Wextra"
#endif

#include <phantom/namespace>
#include <phantom/package>
#include <phantom/source>
#include <phantom/struct>
#include <phantom/enum_class>
#include <phantom/static_constant>
#include <phantom/method>
#include <phantom/static_method>
#include <phantom/constructor>
#include <phantom/typedef>

#include <phantom/template-only-push>

#include "SmallString.hxx"
#include "SmallVector.hxx"
#include "StringView.hxx"

#include <phantom/template-only-pop>

namespace phantom {
PHANTOM_PACKAGE("phantom.utils")
    PHANTOM_SOURCE("Path")

        #if PHANTOM_NOT_TEMPLATE
        PHANTOM_REGISTER(Typedefs) { this_().typedef_<DirectoryEntries>("DirectoryEntries"); }
        PHANTOM_STRUCT(Path)
        {
            using String = typedef_< phantom::String>;
            using StringView = typedef_< phantom::StringView>;
            this_()
            /// missing symbol(s) reflection (, std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, DirectoryEntries&, ::std::error_code &, bool)>("ListDirectoryEntries", &_::ListDirectoryEntries)({"p","entries","ec","recursive"})["false"]
            .staticMethod<bool(const Path&, DirectoryEntries&, bool)>("ListDirectoryEntries", &_::ListDirectoryEntries)({"p","entries","recursive"})["false"]
            /// missing symbol(s) reflection (std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, ::std::error_code &)>("CreateDirectories", &_::CreateDirectories)({"p","ec"})
            .staticMethod<bool(const Path&)>("CreateDirectories", &_::CreateDirectories)({"p"})
            .staticMethod<::phantom::Path()>("TempFolder", &_::TempFolder)
            .staticMethod<bool(const Path&)>("Exists", &_::Exists)({"p"})
            .staticMethod<bool(const Path&)>("IsAbsolute", &_::IsAbsolute)({"p"})
            .staticMethod<bool(const Path&)>("IsGenericAbsolute", &_::IsGenericAbsolute)({"p"})
            .staticMethod<bool(const Path&)>("IsWindowsAbsolute", &_::IsWindowsAbsolute)({"p"})
            .staticMethod<bool(const Path&)>("IsUnixAbsolute", &_::IsUnixAbsolute)({"p"})
            .staticMethod<bool(const Path&)>("IsDirectory", &_::IsDirectory)({"p"})
            .staticMethod<bool(const Path&)>("IsRegularFile", &_::IsRegularFile)({"p"})
            .staticMethod<bool(const Path&)>("IsSymLink", &_::IsSymLink)({"p"})
            .staticMethod<::phantom::Path(const Path&)>("Absolute", &_::Absolute)({"p"})
            .staticMethod<bool(const Path&, const Path&)>("Equivalent", &_::Equivalent)({"p0","p1"})
            /// missing symbol(s) reflection (std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, const Path&, ::std::error_code &)>("Copy", &_::Copy)({"dst","src","ec"})
            .staticMethod<bool(const Path&, const Path&)>("Copy", &_::Copy)({"dst","src"})
            /// missing symbol(s) reflection (std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, const Path&, ::std::error_code &)>("Rename", &_::Rename)({"dst","src","ec"})
            .staticMethod<bool(const Path&, const Path&)>("Rename", &_::Rename)({"dst","src"})
            /// missing symbol(s) reflection (std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, ::std::error_code &)>("Remove", &_::Remove)({"p","ec"})
            .staticMethod<bool(const Path&)>("Remove", &_::Remove)({"p"})
            /// missing symbol(s) reflection (std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, ::std::error_code &)>("RemoveAll", &_::RemoveAll)({"p","ec"})
            .staticMethod<bool(const Path&)>("RemoveAll", &_::RemoveAll)({"p"})
            /// missing symbol(s) reflection (std::error_code) -> use the 'haunt.bind' to bind symbols with your custom haunt files
            // .staticMethod<bool(const Path&, size_t, ::std::error_code &)>("ResizeFile", &_::ResizeFile)({"p","size","ec"})
            .staticMethod<bool(const Path&, size_t)>("ResizeFile", &_::ResizeFile)({"p","size"})
            .staticMethod<::phantom::Path()>("GetCurrentWorkingDir", &_::GetCurrentWorkingDir)
            .staticMethod<void(const Path&)>("SetCurrentWorkingDir", &_::SetCurrentWorkingDir)({"_path"})
            .enum_<Type>().values({
                {"unknown",_::Type::unknown},
                {"directory",_::Type::directory},
                {"regular",_::Type::regular},
                {"symlink",_::Type::symlink}})
            .end()
            .constant("separator", _::separator)
            .constant("npos", _::npos)
            .constructor<void()>()
            .constructor<void(StringView)>()({"a_Path"})
            .constructor<void(String const&)>()({"a_Path"})
            .constructor<void(StringView, const char)>()({"a_Path","separator"})
            .constructor<void(const char*)>()({"a_Path"})
            .method<Path&(const StringView)>("operator=", &PHANTOM_REFLECTED_TYPE::operator=)({"a_Path"})
            .method<String&()>("front", &_::front)
            .method<StringView() const>("front", &_::front)
            .method<String&()>("back", &_::back)
            .method<StringView() const>("back", &_::back)
            .method<void()>("popBack", &_::popBack)
            .method<void(const ::phantom::Path &)>("append", &_::append)({"_child"})
            .method<::phantom::Path &()>("removeExtension", &_::removeExtension)
            .method<String&(size_t)>("operator[]", &_::operator[])({"i"})
            .method<StringView(size_t) const>("operator[]", &_::operator[])({"i"})
            .method<::phantom::SmallVector<SmallString<char, 16>, 16>::const_iterator() const>("begin", &_::begin)
            .method<::phantom::SmallVector<SmallString<char, 16>, 16>::const_iterator() const>("end", &_::end)
            .method<::phantom::SmallVector<SmallString<char, 16>, 16>::iterator()>("begin", &_::begin)
            .method<::phantom::SmallVector<SmallString<char, 16>, 16>::iterator()>("end", &_::end)
            .method<String() const>("platformString", &_::platformString)
            .method<String() const>("genericString", &_::genericString)
            .method<String(StringView) const>("customString", &_::customString)({"a_Separator"})
            .method<size_t(const Path&) const>("find", &_::find)({"subpath"})
            .method<size_t(const Path&) const>("rfind", &_::rfind)({"subpath"})
            .method<bool() const>("isAbsolute", &_::isAbsolute)
            .method<bool() const>("isGenericAbsolute", &_::isGenericAbsolute)
            .method<bool() const>("isWindowsAbsolute", &_::isWindowsAbsolute)
            .method<bool() const>("isUnixAbsolute", &_::isUnixAbsolute)
            .method<bool() const>("exists", &_::exists)
            .method<void()>("clear", &_::clear)
            .method<Type() const>("getType", &_::getType)
            .method<bool() const>("isDirectory", &_::isDirectory)
            .method<bool() const>("isRegularFile", &_::isRegularFile)
            .method<bool() const>("isSymLink", &_::isSymLink)
            .method<Path() const>("parentPath", &_::parentPath)
            .method<Path(const Path&) const>("childPath", &_::childPath)({"child"})
            .method<bool() const>("empty", &_::empty)
            .method<bool() const>("isRoot", &_::isRoot)
            .method<StringView() const>("filename", &_::filename)
            .method<StringView() const>("stem", &_::stem)
            .method<StringView() const>("extension", &_::extension)
            .method<Path() const>("absolute", &_::absolute)
            .method<Path() const>("finalPath", &_::finalPath)
            .method<bool(const Path&) const>("operator==", &_::operator==)({"other"})
            .method<bool(const Path&) const>("operator!=", &_::operator!=)({"other"})
            .method<bool(const Path&) const>("operator<", &_::operator<)({"other"})
            .method<size_t() const>("size", &_::size)
            .method<::phantom::Path(const Path&) const>("relative", &_::relative)({"to"})
            .method<bool(const Path&) const>("hasChildPath", &_::hasChildPath)({"other"})
            .method<Path() const>("canonical", &_::canonical)
            .method<Path() const>("subPath", &_::subPath)
            .method<Path(size_t) const>("subPath", &_::subPath)({"i"})
            ;
        }
        PHANTOM_STRUCT(DirectoryEntry)
        {
            this_()
            .constructor<void()>()
            .constructor<void(const Path&, ::phantom::Path::Type)>()({"p","t"})
            .method<const ::phantom::Path &() const>("path", &_::path)
            .method<bool() const>("isRegularFile", &_::isRegularFile)
            .method<bool() const>("isDirectory", &_::isDirectory)
            .method<bool() const>("isSymLink", &_::isSymLink)
            ;
        }
        #endif // PHANTOM_NOT_TEMPLATE
    PHANTOM_END("Path")
PHANTOM_END("phantom.utils")
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#elif defined(__clang__)
#   pragma clang diagnostic pop
#endif

// clang-format on

// haunt }
