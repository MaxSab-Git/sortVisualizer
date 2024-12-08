#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

namespace sv
{
	class BarGraph : public sf::Drawable, public sf::Transformable
	{
	public:
		BarGraph();

		void setMaximumGraphSize(const sf::Vector2f& size);
		const sf::Vector2f& getMaximumGraphSize() const;
		unsigned int getMaxValue() const;

		void setPlaySound(bool playSound);
		void playSound(size_t index);

		void resize(size_t newSize, unsigned int maxValue);
		void set(size_t index, unsigned int barSize);
		void swap(size_t ibarLeft, size_t ibarRight);
		void swap(size_t ibarLeft, BarGraph& barRight, size_t ibarRight);
		void highlight(size_t index, bool highlight);
		void highlight(size_t index, const sf::Color& color);
		void reset();

	private:
		size_t getBarIndex(size_t index);

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		std::vector<sf::Vertex> m_barGraph;
		unsigned int m_max;
		sf::Vector2f m_graphSize;

		sf::Sound m_highlightSound;
		sf::SoundBuffer m_highlightSbf;
		bool m_playSound;
	};
}

