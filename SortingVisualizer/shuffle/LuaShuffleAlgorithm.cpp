#include "LuaShuffleAlgorithm.h"
#include <filesystem>
#include "../LuaHelper.h"


sv::LuaShuffleAlgorithm::LuaShuffleAlgorithm(lua_State* L) : m_L(L)
{
}

void sv::LuaShuffleAlgorithm::setLua(lua_State* L)
{
	m_L = L;
}

void sv::LuaShuffleAlgorithm::setLuaShuffleFuncName(const std::string& name)
{
	m_shuffleFuncName = name;
}

bool sv::LuaShuffleAlgorithm::isFuncValid() const
{
	lua_getglobal(m_L, m_shuffleFuncName.c_str());
	if (lua_isfunction(m_L, -1))
	{
		lua_pop(m_L, 1);
		return true;
	}

	lua_pop(m_L, 1);
	return false;
}

void sv::LuaShuffleAlgorithm::shuffle(SortVisualizer& visualizer)
{
	const float timer = getTimeMultiplier() / visualizer.getArrayLength();
	visualizer.setSwapDelay(timer);
	visualizer.setCompareDelay(timer);
	visualizer.setWriteDelay(timer);

	sv::LuaHelper::setVisualizer(&visualizer);

	lua_getglobal(m_L, m_shuffleFuncName.c_str());
	if (lua_isfunction(m_L, -1))
	{
		lua_call(m_L, 0, 0);
	}

	visualizer.visualizerDone();
}

