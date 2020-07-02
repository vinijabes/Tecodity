#include "pch.h"

#include <iostream>
#include <time.h>

#include "Math/Matrix.h"
#include "Image/PBM/PBMMatrix.h"

#include "Image/Filters/NegativeFilter.h"
#include "Image/Filters/HorizontalFlipFilter.h"
#include "Image/Filters/GamaFilter.h"

int main()
{
	std::cout << "INIT" << std::endl;
	Tecodity::Image lena = Tecodity::PBMMatrix::Load("../lena640.pgm");

	auto negative = std::move(Tecodity::Image::ApplyFilter(lena, Tecodity::NegativeFilter()));
	Tecodity::PBMMatrix::Save("lenanegative640.pgm", negative);

	auto horizontalFlipped = std::move(Tecodity::Image::ApplyFilter(lena, Tecodity::HorizontalFlipFilter()));
	Tecodity::PBMMatrix::Save("lenaFlipped640.pgm", horizontalFlipped);

	auto gamaFiltered = std::move(Tecodity::Image::ApplyFilter(lena, Tecodity::GamaFilter(2, 0.4)));
	Tecodity::PBMMatrix::Save("lenaGama640.pgm", gamaFiltered);

	/*Tecodity::PBMMatrix::Save("lenaminus640.pgm", lena - 100);
	Tecodity::PBMMatrix::Save("lenaplus640.pgm", lena + 40);
	*/

	//auto rotated = Tecodity::PBMMatrix::Rotate90(lena);
	//Tecodity::PBMMatrix::Save("lenarotate640.pgm", rotated);
}