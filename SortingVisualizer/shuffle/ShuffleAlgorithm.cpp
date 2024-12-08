#include "ShuffleAlgorithm.h"

sv::ShuffleAlgorithm::ShuffleAlgorithm(float timeMultiplier) : m_timeMultiplier(timeMultiplier)
{
}

void sv::ShuffleAlgorithm::setTimeMultiplier(float multiplier)
{
	m_timeMultiplier = multiplier;
}

float sv::ShuffleAlgorithm::getTimeMultiplier() const
{
	return m_timeMultiplier;
}
