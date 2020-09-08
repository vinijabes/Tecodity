#pragma once

#include "Math/Matrix.h"

namespace Tecodity {
	enum class ImageFormat
	{
		R,
		RGB,
		CMY,
		HSI
	};

	class FilterInterface;

	union RGBColor
	{
		int c;
		struct rgb
		{
			signed r : 9;
			signed g : 9;
			signed b : 9;
		} rgb;
	};

	class Image
	{
	protected:
		struct ChannelData
		{
		private:
			int* m_Value;
			bool m_Negative;
		public:
			ChannelData(int* value, bool negative)
				: m_Value(value), m_Negative(negative)
			{}

			ChannelData& operator=(int newValue)
			{
				*m_Value = newValue;

				/*if (*m_Value > 255) *m_Value = 255;
				else if (!m_Negative && *m_Value < 0) *m_Value = 0;*/

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
			this->m_NegativeValues = other.m_NegativeValues;
			return *this;
		}

		Image& operator=(Image&& other) noexcept
		{
			if (this == &other) return *this;
			m_Data = std::move(other.m_Data);
			m_NegativeValues = other.m_NegativeValues;

			return *this;
		}

		ChannelData operator[](int index)
		{
			return ChannelData(this->m_Data.GetData() + index, m_NegativeValues);
		}

		ChannelData operator[](int index) const
		{
			return ChannelData(this->m_Data.GetData() + index, m_NegativeValues);
		}

		void Add(const Image& other)
		{
			if (m_Data.GetWidth() == other.m_Data.GetWidth() && m_Data.GetHeight() == other.m_Data.GetHeight())
			{
				int size = m_Data.GetWidth() * m_Data.GetHeight();

				for (int i = 0; i < size; ++i)
				{
					this->operator[](i) = this->operator[](i).GetValue() + other[i].GetValue();
				}
			}
		}

		void Multiply(const int a)
		{
			int size = m_Data.GetWidth() * m_Data.GetHeight();

			for (int i = 0; i < size; ++i)
			{
				this->operator[](i) = this->operator[](i).GetValue() * a;
			}
		}

		Image GetRChannelImage() {
			Image image(this->GetWidth(), this->GetHeight(), ImageFormat::R);

			int i = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
			while (--i)
			{
				RGBColor color = { this->operator[](i).GetValue() };
				image[i] = color.rgb.r;
			}

			return std::move(image);
		}

		Image GetGChannelImage()
		{
			Image image(this->GetWidth(), this->GetHeight(), ImageFormat::R);

			int i = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
			while (--i)
			{
				RGBColor color = { this->operator[](i).GetValue() };
				image[i] = color.rgb.g;
			}

			return std::move(image);
		}

		Image GetBChannelImage()
		{
			Image image(this->GetWidth(), this->GetHeight(), ImageFormat::R);

			int i = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
			while (--i)
			{
				RGBColor color = { this->operator[](i).GetValue() };
				image[i] = color.rgb.b;
			}

			return std::move(image);
		}

		void SetRChannelFromImage(const Image& image)
		{

			int i = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
			while (--i)
			{
				RGBColor color = { this->operator[](i).GetValue() };
				color.rgb.r = image[i].GetValue();
				this->operator[](i) = color.c;
			}

		}

		void SetGChannelFromImage(const Image& image)
		{

			int i = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
			while (--i)
			{
				RGBColor color = { this->operator[](i).GetValue() };
				color.rgb.g = image[i].GetValue();
				this->operator[](i) = color.c;
			}

		}

		void SetBChannelFromImage(const Image& image)
		{

			int i = image.GetMatrix().GetWidth() * image.GetMatrix().GetHeight();
			while (--i)
			{
				RGBColor color = { this->operator[](i).GetValue() };
				color.rgb.b = image[i].GetValue();
				this->operator[](i) = color.c;
			}

		}

		Matrix<int>& GetMatrix() { return m_Data; }
		Matrix<int> GetMatrix() const { return m_Data; }

		int GetWidth() const { return m_Data.GetWidth(); }
		int GetHeight() const { return m_Data.GetHeight(); }

		void EnableNegativeValues() { m_NegativeValues = true; }

		ImageFormat GetFormat() const { return m_Format; }
		void SetFormat(ImageFormat format) { m_Format = format; }

	private:
		ImageFormat m_Format;
		Matrix<int> m_Data;
		bool m_NegativeValues = false;
	};
}