#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class ShellSort : public SortingAlgorithm
	{
	public:
		ShellSort(float timeMultiplier = 4.f);

		void sort(SortVisualizer& visualizer);
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;
	};
}