// license [
// This file is part of the Phantom project. Copyright 2011-2020 Vivien Millet.
// Distributed under the MIT license. Text available here at
// https://github.com/vlmillet/phantom
// ]

#pragma once

#include <haunt>
HAUNT_STOP;

#include <phantom/traits/IsDefaultConstructible.h>
#include <phantom/utils/Delegate.h>
#include <phantom/utils/Placement.h>
#include <phantom/utils/SmallMap.h>
#include <phantom/utils/StaticIf.h>

namespace phantom
{
using CleanupDelegate = phantom::Delegate<void(void*)>;

class PHANTOM_EXPORT_PHANTOM StaticGlobals
{
public:
    static void RegisterForCleanup(void* a_pAddr, void* a_pModuleHandle, CleanupDelegate a_Delegate);
    static void UnregisterForCleanup(void* a_pAddr);
    static bool TryUnregisterForCleanup(void* a_pAddr);
    static bool ReleaseInProgress(void* a_pModuleHandle);
    static void Release(void* a_pModuleStartAddress, void* a_pModuleEndAddress);
};

template<class T>
class StaticGlobal
{
public:
    StaticGlobal(bool a_AutoConstruct = true)
    {
        phantom::static_if<IsPublicOrProtectedDefaultConstructible<T>::value>([=](auto f) {
            if (f(a_AutoConstruct))
                this->construct();
        });
    }
    ~StaticGlobal()
    {
        destroy();
    }

    T& operator*() { return *m_Data; }

    T const& operator*() const { return *m_Data; }

    T* operator->() { return m_Data; }

    T const* operator->() const { return m_Data; }

    operator T const*() const { return m_Data; }

    operator T*() { return m_Data; }

    template<class... Args>
    void construct(Args... a_Args)
    {
        m_Data.construct(a_Args...);
        StaticGlobals::RegisterForCleanup(
        &m_Data, (void*)(PHANTOM_MODULE_HANDLE(&m_Data)),
        CleanupDelegate([](void* p) -> void { reinterpret_cast<RawPlacement<T>*>(p)->destroy(); }));
    }

    void destroy()
    {
		if (StaticGlobals::TryUnregisterForCleanup(&m_Data))
		{
			if (m_Data)
				m_Data.destroy();
		}
		else
		{
			PHANTOM_ASSERT(!StaticGlobals::ReleaseInProgress((void*)(PHANTOM_MODULE_HANDLE(&m_Data))) || !m_Data);
		}
    }

private:
    RawPlacement<T> m_Data;
};
} // namespace phantom
