#pragma once
#include "SortingAlgorithm.h"
#include <lua.hpp>

namespace sv
{
	class LuaSortingAlgorithm : public SortingAlgorithm
	{
	public:
		LuaSortingAlgorithm(lua_State* L = nullptr);

		void setLua(lua_State* L);
		void setLuaSortFuncName(const std::string& name);

		void sort(SortVisualizer& visualizer) override;
		const std::string& getAlgorithmName() const override;

	private:
		std::string m_name;
		std::string m_sortFuncName;
		lua_State* m_L;
	};
}