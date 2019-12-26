// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#include <phantom/random.h>

#if PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX
#    include <fcntl.h>
#    include <unistd.h>
#else // PHANTOM_OPERATING_SYSTEM_WINDOWS
#    include "windows.h"

#    include <wincrypt.h>
#    pragma comment(lib, "Advapi32.lib")
#endif

/// inspired from boost::unique_path but more generalist

namespace
{
#if PHANTOM_OPERATING_SYSTEM == PHANTOM_OPERATING_SYSTEM_WINDOWS

int acquire_crypt_handle(HCRYPTPROV& handle)
{
    if (::CryptAcquireContextW(&handle, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
        return 0;

    int errval = ::GetLastError();
    if (errval != NTE_BAD_KEYSET)
        return errval;

    if (::CryptAcquireContextW(&handle, 0, 0, PROV_RSA_FULL, CRYPT_NEWKEYSET | CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
        return 0;

    errval = ::GetLastError();
    // Another thread could have attempted to create the keyset at the same time.
    if (errval != NTE_EXISTS)
        return errval;

    if (::CryptAcquireContextW(&handle, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT))
        return 0;

    return ::GetLastError();
}

#endif

bool os_crypt(void* buf, std::size_t len)
{
#if PHANTOM_OPERATING_SYSTEM_FAMILY == PHANTOM_OPERATING_SYSTEM_FAMILY_UNIX

    int file = open("/dev/urandom", O_RDONLY);
    if (file == -1)
    {
        file = open("/dev/random", O_RDONLY);
        if (file == -1)
        {
            return false;
        }
    }

    size_t bytes_read = 0;
    while (bytes_read < len)
    {
        ssize_t n = read(file, buf, len - bytes_read);
        if (n == -1)
        {
            close(file);
            return false;
        }
        bytes_read += n;
        buf = static_cast<char*>(buf) + n;
    }

    close(file);

    return true;

#else // PHANTOM_OPERATING_SYSTEM_WINDOWS

    HCRYPTPROV handle;
    int        errval = acquire_crypt_handle(handle);

    if (!errval)
    {
        BOOL gen_ok = ::CryptGenRandom(handle, (DWORD)len, static_cast<unsigned char*>(buf));
        if (!gen_ok)
            errval = ::GetLastError();
        ::CryptReleaseContext(handle, 0);
    }

    if (!errval)
        return true;

    return false;
#endif
}

} // unnamed namespace

namespace phantom
{
namespace random
{
PHANTOM_EXPORT_PHANTOM void str(char* a_Buffer, StringView model)
{
    const char hex[] = "0123456789abcdef";
    char       ran[] = "123456789abcdef";

    assert(sizeof(ran) == 16);
    const int max_nibbles = sizeof(ran);

    int    nibbles_used = max_nibbles;
    size_t i = 0;
    for (; i < model.size(); ++i)
    {
        if (model[i] == '%')
        {
            if (nibbles_used == max_nibbles)
            {
                if (!os_crypt(ran, sizeof(ran)))
                {
                    a_Buffer[0] = '\0';
                    return;
                }
                nibbles_used = 0;
            }
            int c = ran[nibbles_used / 2];
            c >>= 4 * (nibbles_used++ & 1);
            a_Buffer[i] = hex[c & 0xf];
        }
        else
        {
            a_Buffer[i] = model[i];
        }
    }
    a_Buffer[i] = '\0';
}

PHANTOM_EXPORT_PHANTOM ulonglong integer()
{
    char ran[] = "123456789abcdef";

    assert(sizeof(ran) == 16);
    const int max_nibbles = sizeof(ran);

    int nibbles_used = max_nibbles;

    ulonglong result = 0;
    for (uint i = 0; i < sizeof(ulonglong) * 2; ++i)
    {
        if (nibbles_used == max_nibbles)
        {
            if (!os_crypt(ran, sizeof(ran)))
                return 0;
            nibbles_used = 0;
        }
        int c = ran[nibbles_used / 2];
        c >>= 4 * (nibbles_used++ & 1);
        result |= ulonglong(c & 0xf) << (i * 4);
    }

    return result;
}

PHANTOM_EXPORT_PHANTOM double normalized()
{
    return double(integer()) / double(std::numeric_limits<ulonglong>::max());
}

} // namespace random
} // namespace phantom
