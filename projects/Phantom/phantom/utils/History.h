#pragma once

#include <phantom/plugin.h>
#include "SmallVector.h"
#include "String.h"
#include "StringView.h"

namespace phantom
{
	struct PHANTOM_EXPORT_PHANTOM History
	{
		static const size_t callStackSize = 15;
		struct Entry
		{
			Entry() = default;
			Entry(const char* _text);

			String text;
			void* callStack[callStackSize];
		};

		void record(StringView _view);
		void record();
		void record(const char* _Format, ...);
		void pop() { entries.pop_back(); }
		void dump();

                SmallVector<Entry, 1> entries;
        };
}

#if PHANTOM_DEBUG_LEVEL
#define WOLF_CORE_HISTORY_RECORD(...) this->HISTORY.record(__VA_ARGS__)
#define WOLF_CORE_HISTORY_RECORD_EXT(_this, ...) _this->HISTORY.record(__VA_ARGS__)
#define WOLF_CORE_HISTORY_TRACE_CODE(...) do { WOLF_CORE_HISTORY_RECORD("code(%s|%d) %s", __FILE__, __LINE__, #__VA_ARGS__); __VA_ARGS__; } while(false)
#define WOLF_CORE_HISTORY_DECL(...) HAUNT_OFF Wolf::History HISTORY
#else
#define WOLF_CORE_HISTORY_RECORD(...)
#define WOLF_CORE_HISTORY_RECORD_EXT(_this, ...) 
#define WOLF_CORE_HISTORY_TRACE_CODE(...) do {__VA_ARGS__} while(false)
#define WOLF_CORE_HISTORY_DECL(...)
#endif
