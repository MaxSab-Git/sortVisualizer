#pragma once
#include "ShuffleAlgorithm.h"

namespace sv
{
	class FullShuffle : public ShuffleAlgorithm
	{
	public:
		FullShuffle(float timeMultiplier = 2.0f);

		void shuffle(SortVisualizer& visualizer) override;

	private:
		float m_timeMultiplier;
	};
}