#include "pch.h"

#include "Image.h"
#include "Filters/FilterInterface.h"

namespace Tecodity {
	Image::Image(int width, int height, ImageFormat format)
		: m_Data(Matrix<int>(width,height)), m_Format(format)
	{
	}

	//Image::Image(Matrix<int> data, ImageFormat format)
	//	: m_Data(data), m_Format(format)
	//{
	//}

	Image::Image(Matrix<int>&& data, ImageFormat format)
		: m_Data(std::move(data)), m_Format(format)
	{
	}
	Image::Image(const Image& other)
		: m_Data(other.m_Data), m_Format(other.m_Format), m_NegativeValues(other.m_NegativeValues)
	{}

	Image::Image(const Image && other) noexcept
		: m_Data(std::move(other.m_Data)), m_Format(std::move(other.m_Format)), m_NegativeValues(other.m_NegativeValues)
	{}


	void Image::ApplyFilter(const FilterInterface& filter)
	{
		filter.Apply(*this);
	}

	Image Image::ApplyFilter(const Image& image, const FilterInterface& filter)
	{
		auto filtered = image;
		filtered.ApplyFilter(filter);
		return filtered;
	}
}