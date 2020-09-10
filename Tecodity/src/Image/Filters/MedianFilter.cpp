#include "pch.h"
#include "MedianFilter.h"
#include <math.h>
#include <algorithm>

namespace Tecodity {
	void MedianFilter::Apply(Image& image) const
	{
		if (m_Filter.GetWidth() != m_Filter.GetHeight() ||
			m_Filter.GetWidth() % 2 == 0 ||
			m_Filter.GetHeight() % 2 == 0)
			return;

		Image copy = image;

		int size = m_Filter.GetWidth() * m_Filter.GetHeight();

		int middleX = m_Filter.GetWidth() / 2 + 1;
		int middleY = m_Filter.GetHeight() / 2 + 1;

		int left = middleX - m_Filter.GetWidth();
		int right = m_Filter.GetWidth() - middleX;
		int top = middleY - m_Filter.GetHeight();
		int bottom = m_Filter.GetHeight() - middleY;

		int width = image.GetMatrix().GetWidth();
		int height = image.GetMatrix().GetHeight();
		int coordX, coordY;


		//Filtering Left Border
		for (int x = 0; x < middleX; x++)
		{
			for (int y = 0; y < height; ++y)
			{
				std::vector<double> values;
				values.reserve(m_Filter.GetHeight() * m_Filter.GetWidth());
				for (int wx = left; wx <= right; ++wx)
				{
					coordX = x + wx;
					for (int wy = top; wy <= bottom; ++wy)
					{
						coordY = y + wy;
						if (coordX >= 0 && coordY >= 0 && coordY < height)
						{
							values.push_back(copy[coordX + coordY * width].GetValue() * (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth())));
						}
					}
				}

				std::sort(values.begin(), values.end());

				if (values.size() % 2 == 0)
				{
					image[x + y * width] = std::round((values[values.size() / 2] + values[values.size() / 2 + 1])/2);
				}
				else
				{
					image[x + y * width] = std::round(values[values.size()/2]);
				}
			}
		}

		//Filtering Right Border
		for (int x = width - middleX; x < width; x++)
		{
			for (int y = 0; y < height; ++y)
			{
				std::vector<double> values;
				values.reserve(m_Filter.GetHeight() * m_Filter.GetWidth());

				for (int wx = left; wx <= right; ++wx)
				{
					coordX = x + wx;
					for (int wy = top; wy <= bottom; ++wy)
					{
						coordY = y + wy;
						if (coordX < width && coordY >= 0 && coordY < height)
						{
							values.push_back(copy[coordX + coordY * width].GetValue() * (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth())));
						}
					}
				}

				std::sort(values.begin(), values.end());

				if (values.size() % 2 == 0)
				{
					image[x + y * width] = std::round((values[values.size() / 2] + values[values.size() / 2 + 1]) / 2);
				}
				else
				{
					image[x + y * width] = std::round(values[values.size() / 2]);
				}
			}
		}

		//Filtering Top Border
		for (int x = middleX; x < width - middleX; x++)
		{
			for (int y = 0; y < middleY; ++y)
			{
				std::vector<double> values;
				values.reserve(m_Filter.GetHeight() * m_Filter.GetWidth());

				for (int wx = left; wx <= right; ++wx)
				{
					coordX = x + wx;
					for (int wy = top; wy <= bottom; ++wy)
					{
						coordY = y + wy;
						if (coordX < width && coordY >= 0 && coordY < height)
						{
							values.push_back(copy[coordX + coordY * width].GetValue() * (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth())));
						}
					}
				}

				std::sort(values.begin(), values.end());

				if (values.size() % 2 == 0)
				{
					image[x + y * width] = std::round((values[values.size() / 2] + values[values.size() / 2 + 1]) / 2);
				}
				else
				{
					image[x + y * width] = std::round(values[values.size() / 2]);
				}
			}
		}

		//Filtering Top Border
		for (int x = middleX; x < width - middleX; x++)
		{
			for (int y = height - middleY; y < height; ++y)
			{
				std::vector<double> values;
				values.reserve(m_Filter.GetHeight()* m_Filter.GetWidth());

				for (int wx = left; wx <= right; ++wx)
				{
					coordX = x + wx;
					for (int wy = top; wy <= bottom; ++wy)
					{
						coordY = y + wy;
						if (coordX < width && coordY >= 0 && coordY < height)
						{
							values.push_back(copy[coordX + coordY * width].GetValue()* (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth())));
						}
					}
				}

				std::sort(values.begin(), values.end());

				if (values.size() % 2 == 0)
				{
					image[x + y * width] = std::round((values[values.size() / 2] + values[values.size() / 2 + 1]) / 2);
				}
				else
				{
					image[x + y * width] = std::round(values[values.size() / 2]);
				}
			}
		}

		//Filtering image center
		for (int x = middleX; x < width - middleX; ++x)
		{
			for (int y = middleY; y < height - middleY; ++y)
			{
				std::vector<double> values;
				values.reserve(m_Filter.GetHeight() * m_Filter.GetWidth());

				for (int wx = left; wx <= right; ++wx)
				{
					coordX = x + wx;
					for (int wy = top; wy <= bottom; ++wy)
					{
						coordY = y + wy;
						values.push_back(copy[coordX + coordY * width].GetValue()* (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth())));
					}
				}

				std::sort(values.begin(), values.end());

				if (values.size() % 2 == 0)
				{
					image[x + y * width] = std::round((values[values.size() / 2] + values[values.size() / 2 + 1]) / 2);
				}
				else
				{
					image[x + y * width] = std::round(values[values.size() / 2]);
				}
			}
		}
	}
}