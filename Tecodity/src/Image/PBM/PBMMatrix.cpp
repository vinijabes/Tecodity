#include "pch.h"

#include "PBMMatrix.h"
#include "PGM.h"
#include "PPM.h"

namespace Tecodity {

	PBMHeader PBMMatrix::ReadHeader(std::ifstream& input)
	{
		std::string line;
		PBMHeader header;

		input >> line;
		
		if (line == "P1")
			header.format = PBMFormat::P1;
		else if (line == "P2")
			header.format = PBMFormat::P2;
		else if (line == "P3")
			header.format = PBMFormat::P3;
		else if (line == "P4")
			header.format = PBMFormat::P4;
		else if (line == "P5")
			header.format = PBMFormat::P5;
		else if (line == "P6")
			header.format = PBMFormat::P6;
		
		input >> line;
		header.width = std::atoi(line.c_str());

		input >> line;
		header.height = std::atoi(line.c_str());

		if (header.format == PBMFormat::P2 || header.format == PBMFormat::P3)
		{
			input >> line;
			header.limit= std::atoi(line.c_str());
		}
		else
		{
			header.limit = 0;
		}

		return header;
	}

	void PBMMatrix::WriteHeader(std::ofstream& file, PBMFormat format, Matrix<int> matrix)
	{
		switch (format)
		{
		case PBMFormat::P2:
			file << "P2\n";
			break;
		case PBMFormat::P3:
			file << "P3\n";
			break;
		default:
			break;
		}
		file << matrix.GetWidth() << " " << matrix.GetHeight() << "\n";
		file << 255 << "\n";
	}

	Image PBMMatrix::Load(const std::string& path)
	{
		std::ifstream file(path, std::ifstream::in);
		PBMHeader header;

		if (file.is_open())
		{
			header = ReadHeader(file);
			Matrix<int> result;

			auto reader = PBMReader(file);
			result = Matrix<int>((int)header.width, (int)header.height);
			switch (header.format)
			{
			case PBMFormat::P2:
				result = std::move(PGM::Load(result, reader));
				return Image(std::move(result), ImageFormat::R);
			case PBMFormat::P3:
				result = std::move(PPM::Load(result, reader));
				return Image(std::move(result), ImageFormat::RGB);
			default:
				break;
			}
		}

		auto m = Matrix<int>();
		return Image(std::move(m), ImageFormat::R);
	}
	
	void PBMMatrix::Save(const std::string& path, const Image& matrix, PBMFormat format)
	{
		std::ofstream file(path, std::ofstream::out);
		std::string line;

		WriteHeader(file, format, matrix.GetMatrix());
		auto writer = PBMWriter(file);
		switch (format)
		{
		case PBMFormat::P2:
			PGM::Save(matrix.GetMatrix(), writer);
			break;
		case PBMFormat::P3:
			PPM::Save(matrix.GetMatrix(), writer);
		default:
			break;
		}
	}

	Matrix<int> PBMMatrix::Negative(const Matrix<int>& matrix)
	{
		Matrix<int> result = matrix;

		auto resultPtr = result.GetData();

		int i = matrix.GetWidth() * matrix.GetHeight();

		while (i--)
		{
			*resultPtr++ = 255 - *resultPtr;
		}


		return result;
	}

	Matrix<int> PBMMatrix::Rotate90(const Matrix<int>& matrix)
	{
		Matrix<int> result(matrix.GetHeight(), matrix.GetWidth());
		auto dataPtr = result.GetData();
		auto matrixPtr = matrix.GetData();

		for (int i = 0; i < matrix.GetHeight(); i++)
		{
			for (int j = 0; j < matrix.GetWidth(); j++)
			{
				//result[result.GetWidth() - j][i] = matrix[i][j];
				*(dataPtr + (result.GetWidth() - 1) - i + result.GetWidth() * j) = *(matrixPtr + i * matrix.GetWidth() + j);
			}
		}

		return result;
	}
}
