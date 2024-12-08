#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class OptimizedBubbleSort : public SortingAlgorithm
	{
	public:
		OptimizedBubbleSort(float timeMultiplier = 30.f);

		void sort(SortVisualizer& visualizer) override;
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;
	};
}