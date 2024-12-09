#include "LuaHelper.h"

static sv::SortVisualizer* gvisualizer = nullptr;

static const luaL_Reg svLib[] = {
	{"isAborted", sv::LuaHelper::vAborted},
	{"getSize", sv::LuaHelper::vGetSize},
	{"setTimeMultiplier", sv::LuaHelper::vSetTimeMultiplier},
	{"swap", sv::LuaHelper::vSwap},
	{"compare", sv::LuaHelper::vCompare},
	{"write", sv::LuaHelper::vWrite},
	{"copy", sv::LuaHelper::vCopy},
	{"createAuxiliaryArray", sv::LuaHelper::vCreateAuxiliaryArray},
	{nullptr, nullptr}
};

void sv::LuaHelper::open_visualizerLib(lua_State* L, SortVisualizer& visualizer)
{
	lua_getglobal(L, "sv");
	if (lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
	}
	luaL_setfuncs(L, svLib, 0);
	lua_setglobal(L, "sv");
}

void sv::LuaHelper::setVisualizer(SortVisualizer* visualizer)
{
	gvisualizer = visualizer;
}

sv::VisualOperation::ArrayAccess sv::LuaHelper::strToArrayAccess(const char* str)
{
	if (strcmp(str, "mm") == 0) return sv::VisualOperation::ArrayAccess::Main;
	else if (strcmp(str, "am") == 0) return sv::VisualOperation::ArrayAccess::AuxMain;
	else if (strcmp(str, "ma") == 0) return sv::VisualOperation::ArrayAccess::MainAux;
	else if (strcmp(str, "aa") == 0) return sv::VisualOperation::ArrayAccess::Aux;

	return sv::VisualOperation::ArrayAccess::Main;
}

int sv::LuaHelper::vAborted(lua_State* L)
{
	SortVisualizer* visualizer = gvisualizer;
	if (visualizer)
	{
		lua_pushboolean(L, visualizer->isAborted());
	}

	return 1;
}

int sv::LuaHelper::vGetSize(lua_State* L)
{
	SortVisualizer* visualizer = gvisualizer;
	if (visualizer)
	{
		lua_pushinteger(L, visualizer->getArrayLength());
	}

	return 1;
}

int sv::LuaHelper::vSetTimeMultiplier(lua_State* L)
{
	SortVisualizer* visualizer = gvisualizer;
	if (visualizer)
	{
		float timeMul = static_cast<float>(luaL_checknumber(L, 1));

		visualizer->setSwapDelay(timeMul);
		visualizer->setCompareDelay(timeMul);
		visualizer->setWriteDelay(timeMul);
	}

	return 0;
}

int sv::LuaHelper::vSwap(lua_State* L)
{
	SortVisualizer* visualizer = gvisualizer;
	if (visualizer)
	{
		lua_Integer left = luaL_checkinteger(L, 1);
		lua_Integer right = luaL_checkinteger(L, 2);

		luaL_argcheck(L, 1 <= left && visualizer->getArrayLength() >= left, 1, "'left' out of range");
		luaL_argcheck(L, 1 <= right && visualizer->getArrayLength() >= right, 2, "'right' out of range");

		sv::VisualOperation::ArrayAccess access = sv::VisualOperation::ArrayAccess::Main;
		if (lua_isstring(L, 3))
		{
			access = strToArrayAccess(lua_tostring(L, 3));
		}

		lua_pushboolean(L, visualizer->visualizerSwap(left - 1, right - 1, access));

		return 1;
	}

	return 0;
}

int sv::LuaHelper::vCompare(lua_State* L)
{
	SortVisualizer* visualizer = gvisualizer;
	if (visualizer)
	{
		lua_Integer left = luaL_checkinteger(L, 1);
		lua_Integer right = luaL_checkinteger(L, 2);

		luaL_argcheck(L, 1 <= left && visualizer->getArrayLength() >= left, 1, "'left' out of range");
		luaL_argcheck(L, 1 <= right && visualizer->getArrayLength() >= right, 2, "'right' out of range");

		sv::VisualOperation::ArrayAccess access = sv::VisualOperation::ArrayAccess::Main;
		if (lua_isstring(L, 3))
		{
			access = strToArrayAccess(lua_tostring(L, 3));
		}

		bool result;
		lua_pushboolean(L, visualizer->visualizerCompare(left - 1, right - 1, result, SortVisualizer::less, access));
		lua_pushboolean(L, result); 
		
		return 2;
	}

	return 0;
}

int sv::LuaHelper::vWrite(lua_State* L)
{
	SortVisualizer* visualizer = gvisualizer;
	if (visualizer)
	{
		lua_Integer index = luaL_checkinteger(L, 1);
		lua_Integer value = luaL_checkinteger(L, 2);

		luaL_argcheck(L, 1 <= index && visualizer->getArrayLength() >= index, 1, "'index' out of range");
		luaL_argcheck(L, value >= 0 && value <= visualizer->getArrayLength(), 2, "'index' out of range");

		sv::VisualOperation::ArrayAccess access = sv::VisualOperation::ArrayAccess::Main;
		if (lua_isstring(L, 3))
		{
			access = strToArrayAccess(lua_tostring(L, 3));

			if (sv::VisualOperation::ArrayAccess::Aux != access) access = sv::VisualOperation::ArrayAccess::Main;
		}

		lua_pushboolean(L, visualizer->visualizerWrite(index - 1, value, access));

		return 1;
	}

	return 0;
}

int sv::LuaHelper::vCopy(lua_State* L)
{
	SortVisualizer* visualizer = gvisualizer;
	if (visualizer)
	{
		lua_Integer dest = luaL_checkinteger(L, 1);
		lua_Integer src = luaL_checkinteger(L, 2);

		luaL_argcheck(L, 1 <= dest && visualizer->getArrayLength() >= dest, 1, "'dest' out of range");
		luaL_argcheck(L, 1 <= src && visualizer->getArrayLength() >= src, 2, "'src' out of range");

		sv::VisualOperation::ArrayAccess access = sv::VisualOperation::ArrayAccess::Main;
		if (lua_isstring(L, 3))
		{
			access = strToArrayAccess(lua_tostring(L, 3));
		}

		lua_pushboolean(L, visualizer->visualizerCopy(dest - 1, src - 1, access));

		return 1;
	}

	return 0;
}

int sv::LuaHelper::vCreateAuxiliaryArray(lua_State* L)
{
	SortVisualizer* visualizer = gvisualizer;
	if (visualizer)
	{
		lua_Integer size = luaL_checkinteger(L, 1);

		luaL_argcheck(L, size >= 0 && visualizer->getArrayLength() >= size, 1, "'size' should be >= 0");

		visualizer->createAuxiliaryArray(size);
	}

	return 0;
}
