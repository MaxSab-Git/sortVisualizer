#include "LuaSortingAlgorithm.h"
#include "../../LuaHelper.h"
#include <lua.hpp>

sv::LuaSortingAlgorithm::LuaSortingAlgorithm(lua_State* L) : m_L(L)
{
}

void sv::LuaSortingAlgorithm::setLua(lua_State* L)
{
	m_L = L;
}

void sv::LuaSortingAlgorithm::setLuaSortFuncName(const std::string& name)
{
	m_sortFuncName = name;
	m_name = name;
}

void sv::LuaSortingAlgorithm::sort(SortVisualizer& visualizer)
{
	const float timer = 30.f / (visualizer.getArrayLength() * visualizer.getArrayLength());
	visualizer.setSwapDelay(timer);
	visualizer.setCompareDelay(timer);
	visualizer.setWriteDelay(timer);

	sv::LuaHelper::setVisualizer(&visualizer);
	if (luaL_loadfile(m_L, m_name.c_str()) == LUA_OK)
	{
		lua_pcall(m_L, 0, 0, 0);
	}

	visualizer.visualizerDone();
}

const std::string& sv::LuaSortingAlgorithm::getAlgorithmName() const
{
	return m_name;
}
