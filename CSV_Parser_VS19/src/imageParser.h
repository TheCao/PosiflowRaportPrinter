/*
 * imageParser.h
 *
 *  Created on: 10 cze 2019
 *      Author: Bartosz Komar
 */

#ifndef IMAGEPARSER_H_
#define IMAGEPARSER_H_
#include <boost/filesystem.hpp> //included ONLY because boost should be included before Magick++. In other case u got bunch of errors. ;C

#include "rgb.h"

class ImageParser
{
public:
	ImageParser(boost::filesystem::path filePath): mFilePath(filePath) {}
	void parseImageToRGB();
	void parseTxtToRGB();
	void createHeaderFile(std::string headerName);
	std::vector<std::vector<RGB>> * getRGBVector();


private:
	boost::filesystem::path mFilePath;
	std::vector<std::vector<RGB>> mRGBVector;

};



#endif /* IMAGEPARSER_H_ */
