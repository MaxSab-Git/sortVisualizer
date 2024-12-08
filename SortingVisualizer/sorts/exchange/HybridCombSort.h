#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class HybridCombSort : public SortingAlgorithm
	{
	public:
		HybridCombSort(float timeMultiplier = 4.f);

		void sort(SortVisualizer& visualizer);
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;
	};
}