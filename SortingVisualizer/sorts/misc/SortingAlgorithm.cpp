#include "SortingAlgorithm.h"

sv::SortingAlgorithm::SortingAlgorithm(float timeMultiplier) : m_timeMultiplier(timeMultiplier)
{
}

sv::SortingAlgorithm::~SortingAlgorithm()
{
}

void sv::SortingAlgorithm::setTimeMultiplier(float multiplier)
{
	m_timeMultiplier = multiplier;
}

float sv::SortingAlgorithm::getTimeMultiplier() const
{
	return m_timeMultiplier;
}
