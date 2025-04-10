#pragma once

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <cmath>

namespace MathUtils
{
	inline sf::Vector2f lerp(sf::Vector2f v1, sf::Vector2f v2, float t)
	{
		return v1 + (v2 - v1) * t;
	}

	constexpr float clamp(float value, float min, float max)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;
		return value;
	}

	inline float distance(sf::FloatRect v1, sf::FloatRect v2)
	{
		return std::sqrt(std::pow(v2.left - v1.left, 2.f) + std::pow(v2.top - v1.top, 2.f));
	}

	constexpr double getDigitCount(int num) {
		if (num == 0) return 1;
		return std::log10(std::abs(num)) + 1;
	}
}