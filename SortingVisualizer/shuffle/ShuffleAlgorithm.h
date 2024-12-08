#pragma once
#include "../SortVisualizer.h"

namespace sv
{
	class ShuffleAlgorithm
	{
	public:
		ShuffleAlgorithm(float timeMultiplier = 2.0f);

		void setTimeMultiplier(float multiplier);
		float getTimeMultiplier() const;
		virtual void shuffle(SortVisualizer& visualizer) = 0;

	private:
		float m_timeMultiplier;
	};
}