#include "pch.h"

#include <iostream>
#include <time.h>

#include "Math/Matrix.h"
#include "Image/PBM/PBMMatrix.h"

#include "Image/Filters/ConvertRGBToCMY.h"
#include "Image/Filters/ConvertRGBToHSI.h"
#include "Image/Filters/ConvertGrayToRGB.h"

#include "Core/Pipeline.h"

int main()
{
	std::cout << "INIT" << std::endl;

	//Tecodity::Image strawberries = Tecodity::PBMMatrix::Load("strawberries.ppm");

	//auto rchannel = strawberries.GetRChannelImage();
	//auto gchannel = strawberries.GetGChannelImage();
	//auto bchannel = strawberries.GetBChannelImage();

	//Tecodity::PBMMatrix::Save("rrgbstrawberries.pgm", rchannel, Tecodity::PBMFormat::P2);
	//Tecodity::PBMMatrix::Save("grgbstrawberries.pgm", gchannel, Tecodity::PBMFormat::P2);
	//Tecodity::PBMMatrix::Save("brgbstrawberries.pgm", bchannel, Tecodity::PBMFormat::P2);

	//auto cmyimage = Tecodity::Image::ApplyFilter(strawberries, Tecodity::ConvertRGBToCMY());

	//auto rcmychannel = cmyimage.GetRChannelImage();
	//auto gcmychannel = cmyimage.GetGChannelImage();
	//auto bcmychannel = cmyimage.GetBChannelImage();

	//Tecodity::PBMMatrix::Save("ccmystrawberries.pgm", rcmychannel, Tecodity::PBMFormat::P2);
	//Tecodity::PBMMatrix::Save("mcmystrawberries.pgm", gcmychannel, Tecodity::PBMFormat::P2);
	//Tecodity::PBMMatrix::Save("ycmystrawberries.pgm", bcmychannel, Tecodity::PBMFormat::P2);

	//auto hsiimage = Tecodity::Image::ApplyFilter(strawberries, Tecodity::ConvertRGBToHSI());

	//auto rhsichannel = hsiimage.GetRChannelImage();
	//auto ghsichannel = hsiimage.GetGChannelImage();
	//auto bhsichannel = hsiimage.GetBChannelImage();

	//Tecodity::PBMMatrix::Save("hhsistrawberries.pgm", rhsichannel, Tecodity::PBMFormat::P2);
	//Tecodity::PBMMatrix::Save("shsistrawberries.pgm", ghsichannel, Tecodity::PBMFormat::P2);
	//Tecodity::PBMMatrix::Save("ihsistrawberries.pgm", bhsichannel, Tecodity::PBMFormat::P2);

	//Tecodity::Image WashingtonB = Tecodity::PBMMatrix::Load("WashingtonDCBand1-BLUE.pgm");
	//Tecodity::Image WashingtonG = Tecodity::PBMMatrix::Load("WashingtonDCBand2-GREEN.pgm");
	//Tecodity::Image WashingtonR = Tecodity::PBMMatrix::Load("WashingtonDCBand3-RED.pgm");
	//Tecodity::Image Washington4 = Tecodity::PBMMatrix::Load("WashingtonDCBand4.pgm");

	//Tecodity::Image image(WashingtonB.GetWidth(), WashingtonB.GetHeight(), Tecodity::ImageFormat::RGB);
	//image.SetRChannelFromImage(WashingtonR);
	//image.SetGChannelFromImage(WashingtonG);
	//image.SetBChannelFromImage(WashingtonB);

	//Tecodity::Image image2(WashingtonB.GetWidth(), WashingtonB.GetHeight(), Tecodity::ImageFormat::RGB);
	//image2.SetRChannelFromImage(Washington4);
	//image2.SetGChannelFromImage(WashingtonG);
	//image2.SetBChannelFromImage(WashingtonB);

	//Tecodity::PBMMatrix::Save("image1.ppm", image, Tecodity::PBMFormat::P3);
	//Tecodity::PBMMatrix::Save("image2.ppm", image2, Tecodity::PBMFormat::P3);

	//Tecodity::Image picker = Tecodity::PBMMatrix::Load("picker_phantom.pgm");
	//Tecodity::Image weld = Tecodity::PBMMatrix::Load("weld-original.pgm");

	//auto pickerrgb = Tecodity::Image::ApplyFilter(picker, Tecodity::ConvertGrayToRGB([](int color) {
	//	Tecodity::RGBColor result;
	//	result.c = 0;

	//	if (color > 10 && color < 40)
	//	{
	//		result.rgb.r = 0;
	//		result.rgb.g = 0;
	//		result.rgb.b = 255;
	//		return result;
	//	}
	//	else if (color > 40 && color < 90)
	//	{
	//		result.rgb.r = 255;
	//		result.rgb.g = 255;
	//		result.rgb.b = 0;
	//	}
	//	else if (color > 90 && color < 110)
	//	{
	//		result.rgb.r = 0;
	//		result.rgb.g = 255;
	//		result.rgb.b = 0;
	//	}
	//	else if (color > 110 && color < 125)
	//	{
	//		result.rgb.r = 255;
	//		result.rgb.g = 0;
	//		result.rgb.b = 0;
	//	}
	//	else if (color > 125)
	//	{
	//		result.rgb.r = 255;
	//		result.rgb.g = 255;
	//		result.rgb.b = 255;
	//	}

	//	return result;
	//	}));

	//auto weldrgb = Tecodity::Image::ApplyFilter(weld, Tecodity::ConvertGrayToRGB([](int color) {
	//	Tecodity::RGBColor result;

	//	result.c = 0;
	//	result.rgb.b = 255;

	//	if (color > 252)
	//	{
	//		result.rgb.r = 255;
	//		result.rgb.g = 255;
	//		result.rgb.b = 0;
	//		return result;
	//	}

	//	return result;
	//	}));

	//Tecodity::PBMMatrix::Save("rgbpicker_phantom.ppm", pickerrgb, Tecodity::PBMFormat::P3);
	//Tecodity::PBMMatrix::Save("rgbweld.ppm", weldrgb, Tecodity::PBMFormat::P3);

	Tecodity::Image weld = Tecodity::PBMMatrix::Load("weld-original.pgm");

	Tecodity::PipelineBuilder builder;
	builder.RegisterFactory<Tecodity::ConvertGrayToRGB>("teste", [](const Tecodity::StepInput& input) {
		return new Tecodity::ConvertGrayToRGB([](int color) {
			Tecodity::RGBColor result;

			result.c = 0;
			result.rgb.b = 255;

			if (color > 252)
			{
				result.rgb.r = 255;
				result.rgb.g = 255;
				result.rgb.b = 0;
				return result;
			}

			return result;
			});
		});

	Tecodity::Pipeline p = builder.Build();
	auto pipedWeld = p.Execute(weld);

	Tecodity::PBMMatrix::Save("pipetest.ppm", pipedWeld, Tecodity::PBMFormat::P3);
}