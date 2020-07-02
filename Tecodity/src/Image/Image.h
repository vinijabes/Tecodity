#pragma once

#include "Math/Matrix.h"

namespace Tecodity {
	enum class ImageFormat
	{
		R,
		RGB
	};

	class FilterInterface;

	class Image
	{
	protected:
		struct ChannelData
		{
		private:
			int* m_Value;
		public:
			ChannelData(int* value)
				: m_Value(value)
			{}

			ChannelData& operator=(int newValue)
			{
				*m_Value = newValue;
				if (*m_Value > 255) *m_Value = 255;
				else if (*m_Value < 0) *m_Value = 0;

				return *this;
			}

			int GetValue() const { return *m_Value; }
		};
	public:
		Image(int width, int height, ImageFormat format);
		//Image(Matrix<int> data, ImageFormat format);
		Image(Matrix<int>&& data, ImageFormat format);
		
		Image(const Image& other);
		Image(const Image&& other) noexcept;

		void ApplyFilter(const FilterInterface& filter);
		static Image ApplyFilter(const Image& image, const FilterInterface& filter);

		Image& operator=(const Image& other)
		{
			this->m_Data = other.m_Data;
			return *this;
		}

		Image& operator=(Image&& other) noexcept
		{
			if (this == &other) return *this;
			m_Data = std::move(other.m_Data);

			return *this;
		}

		ChannelData operator[](int index)
		{
			return ChannelData(this->m_Data.GetData() + index);
		}

		Matrix<int>& GetMatrix() { return m_Data; }
		Matrix<int> GetMatrix() const { return m_Data; }

	private:
		ImageFormat m_Format;
		Matrix<int> m_Data;
	};
}