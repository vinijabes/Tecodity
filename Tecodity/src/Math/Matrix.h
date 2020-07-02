#pragma once

#include "Core/Core.h"
#include <stdlib.h>
#include <stdint.h>
#include <memory>
#include <fstream>

#include <functional>
#include <iostream>

namespace Tecodity {

	template<class T>
	class MatrixInterface {};

	template<class T>
	class Matrix : public MatrixInterface<T>
	{
	public:
		Matrix()
			: m_SizeX(0), m_SizeY(0), m_Elements(nullptr)
		{
			TRACE("Default constructor");
		}

		Matrix(uint32_t sizex, uint32_t sizey, T value = T())
			: m_SizeX(sizex), m_SizeY(sizey)
		{
			TRACE("Matrix constructor");
			m_Elements = std::unique_ptr<T[]>(new T[sizex * sizey]);

			auto ptr = m_Elements.get();
			for (int i = 0; i < sizex * sizey; i++)
			{
				*(ptr++) = value;
			}			
		}

		Matrix(const Matrix<T>& other)
			:m_SizeX(other.m_SizeX), m_SizeY(other.m_SizeY)
		{
			TRACE("Matrix Copy constructor");

			m_Elements = std::unique_ptr<T[]>(new T[other.m_SizeX * other.m_SizeY]);
			memcpy(m_Elements.get(), other.m_Elements.get(), other.m_SizeX * other.m_SizeY * sizeof(T));
		}

		Matrix(Matrix<T>&& other) noexcept :
			m_SizeX(std::move(other.m_SizeX)),
			m_SizeY(std::move(other.m_SizeY)),
			m_Elements(std::move(other.m_Elements))
		{
			TRACE("Matrix Move constructor");
		}

		~Matrix()
		{
			TRACE("Matrix destructor");
		}

		uint32_t GetWidth() const { return m_SizeX; }
		uint32_t GetHeight() const { return m_SizeY; }

		T* operator[](int index)
		{
			return m_Elements.get() + index * m_SizeX;
		}

		const T* operator[](int index) const
		{
			return m_Elements.get() + index * m_SizeX;
		}

		friend Matrix<T> operator+(const Matrix<T>& matrix, const Matrix<T>& other)
		{
			return Matrix<T>::Add(matrix, other);
		}

		friend Matrix<T> operator+(const Matrix<T>& matrix, const T& other)
		{
			return Matrix<T>::Add(matrix, other);
		}

		friend Matrix<T>& operator+=(Matrix<T>& matrix, const Matrix<T>& other)
		{
			return matrix.Add(other);
		}

		friend Matrix<T> operator-(const Matrix<T>& matrix, const Matrix<T>& other)
		{
			return Matrix<T>.Sub(matrix, other);
		}

		friend Matrix<T> operator-(const Matrix<T>& matrix, const T& other)
		{
			return Sub(matrix, other);
		}

		friend Matrix<T>& operator-=(Matrix<T>& matrix, const Matrix<T>& other)
		{
			return matrix.Sub(other);
		}

		Matrix<T>& operator=(const Matrix<T>& other)
		{
			TRACE("Matrix assigment operator");

			if (m_Elements && m_SizeX != other.m_SizeX && m_SizeY != other.m_SizeY)
			{
				m_Elements = std::unique_ptr<T[]>(new T[m_SizeX * m_SizeY]);
			}

			m_SizeX = other.m_SizeX;
			m_SizeY = other.m_SizeY;

			memcpy(m_Elements.get(), other.m_Elements.get(), m_SizeX * m_SizeY * sizeof(T));

			return *this;
		}

		Matrix<T>& operator=(Matrix<T>&& other) noexcept
		{
			if (this == &other) return *this;
			TRACE("Matrix Move operator");

			m_SizeX = other.m_SizeX;
			m_SizeY = other.m_SizeY;
			m_Elements = std::move(other.m_Elements);

			return *this;
		}

		void Print()
		{
			ForEach([this](T& elem, uint32_t i, uint32_t j) {
				std::cout << "[" << i << "][" << j << "] = " << elem << std::endl;
				});
		}

		void ForEach(std::function<void(T&, uint32_t, uint32_t)> cb)
		{
			auto pointer = m_Elements.get();
			for (int i = 0; i < m_SizeY; i++)
			{
				for (int j = 0; j < m_SizeX; j++)
				{
					cb(*pointer, i, j);
					++pointer;
				}
			}
		}

		T* GetData() { return m_Elements.get(); }
		T* GetData() const { return m_Elements.get(); }

	public:

		virtual Matrix<T>& Add(const Matrix<T>& t2)
		{
			auto resultPtr = this->m_Elements.get();
			auto otherPtr = t2.m_Elements.get();

			for (int i = 0; i < m_SizeX * m_SizeY; ++i)
			{
				*(resultPtr++) += *(otherPtr++);
			}

			return *this;
		}

		virtual Matrix<T>& Add(const T& t2)
		{
			auto resultPtr = this->m_Elements.get();

			for (int i = 0; i < m_SizeX * m_SizeY; ++i)
			{
				*resultPtr += t2;
				if (*resultPtr > 255) *resultPtr = 255;
				++resultPtr;
			}

			return *this;
		}

		static Matrix<T> Add(const Matrix<T>& t, const Matrix<T>& t2)
		{
			Matrix<T> result(t);
			return result.Add(t2);
		}

		static Matrix<T> Add(const Matrix<T>& t, const T& t2)
		{
			Matrix<T> result(t);
			return result.Add(t2);
		}

		virtual Matrix<T>& Sub(const Matrix<T>& t2)
		{
			auto resultPtr = this->m_Elements.get();
			auto otherPtr = t2.m_Elements.get();

			for (int i = 0; i < m_SizeX * m_SizeY; ++i)
			{
				*resultPtr -= *otherPtr++;
				if (*resultPtr < 0) *resultPtr = 0;
				++resultPtr;
			}

			return *this;
		}

		virtual Matrix<T>& Sub(const T& t2)
		{
			auto resultPtr = this->m_Elements.get();

			for (int i = 0; i < m_SizeX * m_SizeY; ++i)
			{
				*resultPtr -= t2;
				if (*resultPtr < 0) *resultPtr = 0;
				++resultPtr;
			}

			return *this;
		}

		static Matrix<T> Sub(const Matrix<T>& t, const Matrix<T>& t2)
		{
			Matrix<T> result(t);
			return result.Sub(t2);
		}

		static Matrix<T> Sub(const Matrix<T>& t, const T& t2)
		{
			Matrix<T> result(t);
			return result.Sub(t2);
		}



	public:
		static Matrix<T> Create(const std::string& path);
		static void Save(const std::string& path, const Matrix<T> matrix);

	private:
		uint32_t m_SizeX;
		uint32_t m_SizeY;

		std::unique_ptr<T[]> m_Elements;
	};

	template<class T>
	inline Matrix<T> Matrix<T>::Create(const std::string& path)
	{
		std::ifstream file(path, std::ifstream::in);
		std::string line;

		Matrix<T> matrix(0, 0);

		if (file.is_open())
		{
			file >> line;
			auto sizex = std::atoi(line.c_str());

			file >> line;
			auto sizey = std::atoi(line.c_str());

			matrix = Matrix<T>(sizey, sizex);
			auto elements = matrix.m_Elements;

			while (file.peek() != EOF)
			{
				file >> line;
				*(elements++) = std::atoi(line.c_str());
			}

		}

		return matrix;
	}

	template<class T>
	inline void Matrix<T>::Save(const std::string& path, const Matrix<T> matrix)
	{
		std::ofstream file(path, std::ofstream::out);
		std::string line;

		if (file.is_open())
		{
			file << matrix.m_SizeY << " ";
			file << matrix.m_SizeY << " ";

			auto elements = matrix.m_Elements.get();
			for (int i = 0; i < matrix.m_SizeX * matrix.m_SizeY - 1; ++i)
			{
				file << *(elements++) << " ";
			}
			file << *(elements++);
		}
	}


}