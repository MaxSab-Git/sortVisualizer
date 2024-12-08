#pragma once
#include "../../SortVisualizer.h"

namespace sv
{
	class SortingAlgorithm
	{
	public:
		SortingAlgorithm(float timeMultiplier = 1.f);
		~SortingAlgorithm();

		virtual void sort(SortVisualizer& visualizer) = 0;
		virtual const std::string& getAlgorithmName() const = 0;

		void setTimeMultiplier(float multiplier);
		float getTimeMultiplier() const;

	private:
		float m_timeMultiplier;
	};
}

