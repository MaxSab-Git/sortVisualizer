#pragma once
#include "../misc/SortingAlgorithm.h"

namespace sv
{
	class BucketSort : public SortingAlgorithm
	{
	public:
		BucketSort(size_t bucketSize, float timeMultiplier = 1.f);

		void sort(SortVisualizer& visualizer) override;
		const std::string& getAlgorithmName() const override;

	private:
		static const std::string name;

		size_t m_bucketSize;
	};
}