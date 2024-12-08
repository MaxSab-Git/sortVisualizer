#include "BarGraph.h"

sv::BarGraph::BarGraph() : m_max(0), m_playSound(false)
{
	m_highlightSbf.loadFromFile("Ressources/highlight.wav");
	m_highlightSound.setBuffer(m_highlightSbf);
	m_highlightSound.setVolume(15.f);
}

void sv::BarGraph::setMaximumGraphSize(const sf::Vector2f& size)
{
	m_graphSize = size;
}

const sf::Vector2f& sv::BarGraph::getMaximumGraphSize() const
{
	return m_graphSize;
}

unsigned int sv::BarGraph::getMaxValue() const
{
	return m_max;
}

void sv::BarGraph::setPlaySound(bool playSound)
{
	m_playSound = playSound;
}

void sv::BarGraph::playSound(size_t index)
{
	if (m_playSound)
	{
		index = getBarIndex(index);
		m_highlightSound.stop();
		m_highlightSound.setPitch(-m_barGraph[index + 1].position.y / m_graphSize.y * 2.f);
		m_highlightSound.play();
	}
}

void sv::BarGraph::resize(size_t newSize, unsigned int maxValue)
{
	m_max = maxValue;
	m_barGraph.resize(newSize * 6);
	for (size_t i = 0; i < m_barGraph.size(); i++)
	{
		m_barGraph[i].color = sf::Color::White;
	}

	for (size_t i = 0; i < newSize; i++)
	{
		set(i, 0);
	}
}

void sv::BarGraph::set(size_t index, unsigned int barSize)
{
	if (m_max > 0 && !m_barGraph.empty() && m_max >= barSize)
	{
		const float barHeight = (m_graphSize.y / static_cast<float>(m_max)) * static_cast<float>(barSize);
		const float barWidth = m_graphSize.x / static_cast<float>(m_barGraph.size() / 6);

		float barOffset = static_cast<float>(index) * barWidth;
		index = getBarIndex(index);

		m_barGraph[index].position = sf::Vector2f(barOffset, 0.f);
		m_barGraph[index + 1].position = sf::Vector2f(barOffset, -barHeight);
		m_barGraph[index + 2].position = sf::Vector2f(barOffset + barWidth, -barHeight);
		m_barGraph[index + 3].position = m_barGraph[index].position;
		m_barGraph[index + 4].position = m_barGraph[index + 2].position;
		m_barGraph[index + 5].position = sf::Vector2f(barOffset + barWidth, 0.f);
	}
}

void sv::BarGraph::swap(size_t ibarLeft, size_t ibarRight)
{
	ibarLeft = getBarIndex(ibarLeft);
	ibarRight = getBarIndex(ibarRight);

	std::swap(m_barGraph[ibarLeft + 1].position.y, m_barGraph[ibarRight + 1].position.y);
	std::swap(m_barGraph[ibarLeft + 2].position.y, m_barGraph[ibarRight + 2].position.y);
	std::swap(m_barGraph[ibarLeft + 4].position.y, m_barGraph[ibarRight + 4].position.y);
}

void sv::BarGraph::swap(size_t ibarLeft, BarGraph& barRight, size_t ibarRight)
{
	ibarLeft = getBarIndex(ibarLeft);
	ibarRight = barRight.getBarIndex(ibarRight);

	std::swap(m_barGraph[ibarLeft + 1].position.y, barRight.m_barGraph[ibarRight + 1].position.y);
	std::swap(m_barGraph[ibarLeft + 2].position.y, barRight.m_barGraph[ibarRight + 2].position.y);
	std::swap(m_barGraph[ibarLeft + 4].position.y, barRight.m_barGraph[ibarRight + 4].position.y);
}

#include <iostream>

void sv::BarGraph::highlight(size_t index, bool highlight)
{
	this->highlight(index, highlight ? sf::Color::Red : sf::Color::White);
}

void sv::BarGraph::highlight(size_t index, const sf::Color& color)
{
	index = getBarIndex(index);
	for (size_t i = 0; i < 6; i++)
	{
		m_barGraph[index + i].color = color;
	}
}

void sv::BarGraph::reset()
{
	for (size_t i = 0; i < m_barGraph.size(); i++)
	{
		m_barGraph[i].color = sf::Color::White;
	}
}

size_t sv::BarGraph::getBarIndex(size_t index)
{
	return index * 6;
}

void sv::BarGraph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_barGraph.data(), m_barGraph.size(), sf::PrimitiveType::Triangles, states);
}
