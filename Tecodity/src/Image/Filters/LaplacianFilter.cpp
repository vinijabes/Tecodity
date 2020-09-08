#include "pch.h"
#include "LaplacianFilter.h"

namespace Tecodity {

	template <typename T> int sgn(T val)
	{
		return (T(0) < val) - (val < T(0));
	}

	void LaplacianFilter::Apply(Image& image) const
	{
		if (m_Filter.GetWidth() != m_Filter.GetHeight() ||
			m_Filter.GetWidth() != 3 ||
			m_Filter.GetHeight() != 3)
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

		int sign = sgn(*(m_Filter.GetData() + 4));

		image.EnableNegativeValues();

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
							sum += copy[coordX + coordY * width].GetValue() * (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth()));
						}
					}
				}

				image[x + y * width] = sum;
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
							sum += copy[coordX + coordY * width].GetValue() * (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth()));
						}
					}
				}

				image[x + y * width] = sum;
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
							sum += copy[coordX + coordY * width].GetValue() * (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth()));
						}
					}
				}

				image[x + y * width] = sum;
			}
		}

		//Filtering Top Border
		for (int x = middleX; x < width - middleX; x++)
		{
			for (int y = height - middleY; y < height; ++y)
			{
				int sum = 0;

				for (int wx = left; wx <= right; ++wx)
				{
					coordX = x + wx;
					for (int wy = top; wy <= bottom; ++wy)
					{
						coordY = y + wy;
						if (coordX < width && coordY >= 0 && coordY < height)
						{
							sum += copy[coordX + coordY * width].GetValue() * (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth()));
						}
					}
				}

				image[x + y * width] = sum;
			}
		}

		//Filtering image center
		for (int x = middleX; x < width - middleX; ++x)
		{
			for (int y = middleY; y < height - middleY; ++y)
			{
				int sum = 0;

				for (int wx = left; wx <= right; ++wx)
				{
					coordX = x + wx;
					for (int wy = top; wy <= bottom; ++wy)
					{
						coordY = y + wy;
						sum += copy[coordX + coordY * width].GetValue() * (*(m_Filter.GetData() + (wx - 1) + middleX + ((wy - 1) + middleY) * m_Filter.GetWidth()));
					}
				}

				image[x + y * width] = sum;
			}
		}
	}
}