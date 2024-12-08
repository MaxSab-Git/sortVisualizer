#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class PancakeSort : public SortingAlgorithm
	{
	public:
		PancakeSort(float timeMultiplier = 10.f);

		void sort(SortVisualizer& visualizer) override;
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;
	};
}