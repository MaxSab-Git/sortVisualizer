#pragma once
#include "ShuffleAlgorithm.h"
#include <lua.hpp>

namespace sv
{
	class LuaShuffleAlgorithm : public ShuffleAlgorithm
	{
	public:
		LuaShuffleAlgorithm(lua_State* L = nullptr);

		void setLua(lua_State* L);
		void setLuaShuffleFuncName(const std::string& name);
		bool isFuncValid() const;

		void shuffle(SortVisualizer& visualizer) override;

	private:
		std::string m_name;
		std::string m_shuffleFuncName;
		lua_State* m_L;
	};
}