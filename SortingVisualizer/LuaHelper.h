#pragma once
#include <lua.hpp>
#include "SortVisualizer.h"

namespace sv
{
	namespace LuaHelper
	{
		void open_visualizerLib(lua_State* L, SortVisualizer& visualizer);
		void setVisualizer(SortVisualizer* visualizer);

		sv::VisualOperation::ArrayAccess strToArrayAccess(const char* str);

		int vAborted(lua_State* L);
		int vGetSize(lua_State* L);
		int vSetTimeMultiplier(lua_State* L);
		int vSwap(lua_State* L);
		int vCompare(lua_State* L);
		int vWrite(lua_State* L);
		int vCopy(lua_State* L);
		int vCreateAuxiliaryArray(lua_State* L);
	};
}

