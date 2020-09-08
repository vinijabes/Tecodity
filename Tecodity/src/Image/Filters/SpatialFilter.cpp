#include "pch.h"
#include "SpatialFilter.h"
#include <math.h>

namespace Tecodity {
	void SpatialFilter::Apply(Image& image) const
	{
		if (m_Filter.GetWidth() != m_Filter.GetHeight() || 
			m_Filter.GetWidth() % 2 == 0 ||
			m_Filter.GetHeight() % 2 == 0) 
			return;

		Image copy = image;

		int divider = 0;
		int size = m_Filter.GetWidth() * m_Filter.GetHeight();
		for (int i = 0; i < size; i++)
		{
			divider += *(m_Filter.GetData() + i);
		}

		if (divider == 0) return;

		int middleX = m_Filter.GetWidth() / 2 + 1;
		int middleY = m_Filter.GetHeight() / 2 + 1;

		int left = middleX - m_Filter.GetWidth();
		int right = m_Filter.GetWidth() - middleX;
		int top = middleY - m_Filter.GetHeight();
		int bottom = m_Filter.GetHeight() - middleY;

		int width  = image.GetMatrix().GetWidth();
		int height = image.GetMatrix().GetHeight();
		int coordX, coordY;


		//Filtering Left Border
		for (int x = 0; x < middleX; x++)
		{
			for (int y = 0; y < height; ++y)
			{
				int sum = 0;
				int divider = 0;

				for (int wx = left; wx <= right; ++wx)
				{
					coordX = x + wx;
					for (int wy = top; wy <= bottom; ++wy)
					{
						coordY = y + wy;
						if (coordX >= 0 && coordY >= 0 && coordY < height)
						{
							divider += (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth()));
							sum += copy[coordX + coordY * width].GetValue() * (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth()));
						}
					}
				}

				if (divider != 0)
				{
					image[x + y * width] = std::round((double)sum / (double)divider);
				}
			}
		}

		//Filtering Right Border
		for (int x = width - middleX; x < width; x++)
		{
			for (int y = 0; y < height; ++y)
			{
				int sum = 0;
				int divider = 0;

				for (int wx = left; wx <= right; ++wx)
				{
					coordX = x + wx;
					for (int wy = top; wy <= bottom; ++wy)
					{
						coordY = y + wy;
						if (coordX < width && coordY >= 0 && coordY < height)
						{
							divider += (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth()));
							sum += copy[coordX + coordY * width].GetValue() * (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1)+ middleY) * m_Filter.GetWidth()));
						}
					}
				}

				if (divider != 0)
				{
					image[x + y * width] = std::round((double)sum / (double)divider);
				}
			}
		}

		//Filtering Top Border
		for (int x = middleX; x < width - middleX; x++)
		{
			for (int y = 0; y < middleY; ++y)
			{
				int sum = 0;
				int divider = 0;

				for (int wx = left; wx <= right; ++wx)
				{
					coordX = x + wx;
					for (int wy = top; wy <= bottom; ++wy)
					{
						coordY = y + wy;
						if (coordX < width && coordY >= 0 && coordY < height)
						{
							divider += (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth()));
							sum += copy[coordX + coordY * width].GetValue() * (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth()));
						}
					}
				}

				if (divider != 0)
				{
					image[x + y * width] = std::round((double)sum / (double)divider);
				}
			}
		}

		//Filtering Top Border
		for (int x = middleX; x < width - middleX; x++)
		{
			for (int y = height - middleY; y < height; ++y)
			{
				int sum = 0;
				int divider = 0;

				for (int wx = left; wx <= right; ++wx)
				{
					coordX = x + wx;
					for (int wy = top; wy <= bottom; ++wy)
					{
						coordY = y + wy;
						if (coordX < width && coordY >= 0 && coordY < height)
						{
							divider += (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth()));
							sum += copy[coordX + coordY * width].GetValue() * (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth()));
						}
					}
				}

				if (divider != 0)
				{
					image[x + y * width] = std::round((double)sum / (double)divider);
				}
			}
		}

		//Filtering image center
		for (int x = middleX; x < width - middleX; ++x) {
			for (int y = middleY; y < height - middleY; ++y)
			{
				int sum = 0;

				for (int wx = left; wx <= right; ++wx)
				{
					coordX = x + wx;
					for (int wy = top; wy <= bottom; ++wy)
					{
						coordY = y + wy;
						sum += copy[coordX + coordY*width].GetValue() * (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth()));
					}
				}

				image[x + y * width] = std::round((double) sum / (double)divider);
			}
		}
	}
}