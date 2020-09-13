/*
 * imageParser.cpp
 *
 *  Created on: 10 cze 2019
 *      Author: Bartosz Komar
 */

#include "imageParser.h"
#include <iostream>
#include <regex>


//logos
#include "AscoLogo.h"

void  ImageParser::parseImageToRGB()
{

}

void ImageParser::parseTxtToRGB()
{
	// check file
	if(boost::filesystem::exists(mFilePath))
	{
		boost::filesystem::ifstream file(mFilePath);
		std::string tempStr;
		int imageWidth, imageHeight, actualLine = 0;
		std::vector<int> imageSize;
		std::vector<RGB> rgbVectorX;
		std::regex integer("[[:digit:]]+");
		while(getline(file, tempStr))
		{
			if(actualLine == 0)
			{
				for(std::sregex_iterator i = std::sregex_iterator(tempStr.begin(), tempStr.end(), integer);
								i!=std::sregex_iterator();i++)
				{
					std::smatch m = *i;
					imageSize.push_back(stoi(m.str()));
				}
				imageWidth = imageSize.at(0);
				imageHeight = imageSize.at(1);
				actualLine++;
			}
			else
			{
				// find ":" in "0,0: (255,255,255)  #FFFFFF  white" as begin
				std::size_t location = tempStr.find(":");
				std::size_t location2 = tempStr.find("#");
				std::vector<unsigned char> tempRGBValues;
				//get pixel position
				std::vector<int> pixelPosition;
				for(std::sregex_iterator i = std::sregex_iterator(tempStr.begin(), tempStr.begin()+location, integer);
												i!=std::sregex_iterator();i++)
				{
					std::smatch m = *i;
					pixelPosition.push_back(stoi(m.str()));
				}
				//populate RGB vector
				for(std::sregex_iterator i = std::sregex_iterator(tempStr.begin()+location, tempStr.begin()+location2, integer);
												i!=std::sregex_iterator();i++)
				{
					std::smatch m = *i;
					tempRGBValues.push_back(stoi(m.str()));
				}
				if(pixelPosition.at(0)<imageWidth-1)
				{
					rgbVectorX.push_back({tempRGBValues.at(0),tempRGBValues.at(1),tempRGBValues.at(2)});

				}
				if(pixelPosition.at(0)==imageWidth-1)
				{
					mRGBVector.push_back(rgbVectorX);
					rgbVectorX.clear();
				}
			}
		}
		file.close();
	}
	else
	{
		std::cerr << "parseTxtToRGB function error: wrong file path \n";
	}

}

void ImageParser::createHeaderFile(std::string headerName)
{
	//create file
	boost::filesystem::path headerPath("./"+headerName+".h");
	boost::filesystem::create_directory(headerPath.parent_path());

	boost::filesystem::ofstream file(headerPath);
	file << "#include \"rgb.h\" \n";
	file << "const std::vector<std::vector<RGB>> " << headerName << " = {";
	for(auto const&vect1: mRGBVector)
	{
		file << "{";
		for(auto const & vect2: vect1) //wiersz
		{

			file << "{" << (int)vect2.mRed << "," << (int)vect2.mGreen << "," << (int)vect2.mBlue << "}";
			if(&vect2 != &vect1.back()) file << ", ";
		}

		file << "}";
		if(&vect1 != &mRGBVector.back()) file << ", ";
		file << "\n";

	}
	file << "};";
	file.close();
	std::cout << "File " << headerName << " parsed and saved! \n";
}

std::vector<std::vector<RGB>> * ImageParser::getRGBVector()
{
	std::vector<std::vector<RGB>> * ret = &mRGBVector;
	return ret;
}

