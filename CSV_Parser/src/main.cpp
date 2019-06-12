
#include <iostream>
#include "csv_parser.h"
#include "raport.h"
#include <dirent.h>
#include <boost/algorithm/string/replace.hpp>
#include "imageParser.h"


bool createRaport(std::string fileName);

std::vector<boost::filesystem::path> refreshFilesPathList(std::string directory="./");

bool moveFileToDirectoryByPath(boost::filesystem::path filePath, boost::filesystem::path dstDirectoryPath);
bool checkTestMeasurementsValues(boost::filesystem::path filePath);
bool checkTestLeakagesValues(boost::filesystem::path filePath);
bool parseDirectoryList(std::vector<boost::filesystem::path> *list);

ImageParser emersonLogo("./grafiki/ecorp_spot_288_50mm.txt");
ImageParser ascoLogo("./grafiki/asco.txt");
boost::filesystem::path nokCatalogue = "./raporty_nok";
// zrob config file ze sciezkami do katalogow i nazwami plikow
int main()
{

	// parse images
	emersonLogo.parseTxtToRGB();
	ascoLogo.parseTxtToRGB();

	std::vector<boost::filesystem::path> filesPathsList;
	while(true)
	{
		filesPathsList = refreshFilesPathList("./raporty");
		parseDirectoryList(&filesPathsList);

	}
	system("pause");
	return 0;
}



std::vector<boost::filesystem::path> refreshFilesPathList(std::string directory)
{
	std::vector<boost::filesystem::path> vec;
	for(auto &p : boost::filesystem::directory_iterator(directory))
	{
		if(boost::filesystem::is_regular_file(p.path()))
		{
			vec.push_back(p.path());
		}
	}
	return vec;
}

bool checkTestMeasurementsValues(boost::filesystem::path filePath)
{
	if(boost::filesystem::exists(filePath))
	{
		boost::filesystem::ifstream ifsFile(filePath);
		std::vector<std::string> tempVec;
		std::string tempStr;
		while(getline(ifsFile, tempStr))
		{
			tempVec.push_back(tempStr);
		}
		ifsFile.close();
		try
		{
			if(tempVec.at(1).find("\"True\";\"True\"")  != string::npos) // both test are valid
			{
				// still need leakages file to prepare report
				return true;
			}
			else
			{
				moveFileToDirectoryByPath(filePath, nokCatalogue); // move to specified folder with error test report
				// check if histeresis file is ready
				std::string tempStr = filePath.string();
				boost::replace_all(tempStr, "Dostrajanie", "Histereza");
				boost::filesystem::path histeresisFilePath = tempStr;
				if(boost::filesystem::exists(histeresisFilePath))
				{
					moveFileToDirectoryByPath(histeresisFilePath, nokCatalogue); // move to specified folder with error test report
				}
			}
		}
		catch(std::out_of_range &err)
		{
			std::cerr << "checkTestMeasurementsValues error: " << err.what() << std::endl;
		}
	}
	return false;
}
bool checkTestLeakagesValues(boost::filesystem::path filePath)
{
	if(boost::filesystem::exists(filePath))
	{
		boost::filesystem::ifstream ifsFile(filePath);
		std::vector<std::string> tempVec;
		std::string tempStr;
		while(getline(ifsFile, tempStr))
		{
			tempVec.push_back(tempStr);
		}
		ifsFile.close();
		try
		{
			if(tempVec.at(1).find("\"True\"")  != string::npos) // leakage test is valid
			{
				return true;
			}
			else
			{
				moveFileToDirectoryByPath(filePath, nokCatalogue); // move to specified folder with error test report
			}
		}
		catch(std::out_of_range &err)
		{
			std::cerr << "checkTestLeakagesValues error: " << err.what() << std::endl;
		}
	}
	return false;
}
bool parseDirectoryList(std::vector<boost::filesystem::path> *list)
{

	for(auto& filePath: *list)
	{
		if(filePath.filename().string().find("_Dostrajanie.csv") != string::npos)
		{
			if(checkTestMeasurementsValues(filePath))
			{

			}
		}
		else if(filePath.filename().string().find("_Wycieki.csv") != string::npos)
		{
			if(checkTestLeakagesValues(filePath))
			{
				// check if Measurements and Histeresis file connected to that Leakages File existing
				std::string tempStr1 = filePath.string();
				std::string tempStr2 = filePath.string();
				boost::replace_all(tempStr1, "Wycieki", "Histereza");
				boost::replace_all(tempStr2, "Wycieki", "Dostrajanie");
				boost::filesystem::path histeresisFilePath = tempStr1;
				boost::filesystem::path measurementsFilePath = tempStr2;
				if(boost::filesystem::exists(histeresisFilePath) && boost::filesystem::exists(measurementsFilePath))
				{
					std::string tempStr3 = filePath.filename().string();
					boost::replace_all(tempStr3, "_Wycieki", "");
					boost::replace_all(tempStr3, ".csv", "");
					std::cout << "Przygotuj raport dla : " << tempStr3 << std::endl;
					createRaport(tempStr3);
					// usun wykorzystane pliki jesli chcesz odwiezac liste plikow tylko od czasu do czasu
				}
			}
		}

	}

	return true;
}


bool moveFileToDirectory(std::string srcDirectory, std::string srcFileName, std::string destDirectory)
{
	try
	{
		if(!(boost::filesystem::exists(boost::filesystem::path(destDirectory))))
		{
			boost::filesystem::create_directory(boost::filesystem::path(destDirectory));
		}
		boost::filesystem::path src(srcDirectory+srcFileName);
		boost::filesystem::path dest(destDirectory+srcFileName);
		boost::filesystem::rename(src, dest);
	}
	catch(boost::filesystem::filesystem_error &error)
	{
		std::cerr << error.what() << std::endl;
	}

	return true;
}

bool moveFileToDirectoryByPath(boost::filesystem::path filePath, boost::filesystem::path dstDirectoryPath)
{
	boost::filesystem::path filename = filePath.filename();
	boost::filesystem::path dstPath = dstDirectoryPath/filename;
	boost::filesystem::rename(filePath, dstPath);
	return true;
}
bool createRaport(std::string fileName)
{
		std::vector<double> PWM, Flow, Current, Leakage, Measurements;
		unsigned vavleNo;
		std::vector<std::vector<double>> histerezis;

		//parse data from files
		CSVParser parser("./raporty/"+fileName+"_Histereza.csv");
		std::vector<std::vector<std::string>> dataList = parser.getData();
		if(dataList.empty())
		{
			return false;
		}
		else
		{
			parser.parseValuesHisteresis(&dataList, &PWM, &Flow, &Current, &vavleNo);
			histerezis.push_back(PWM); //at(0)
			histerezis.push_back(Flow); //at(1)
			histerezis.push_back(Current); //at(2)
			moveFileToDirectory("./raporty/",fileName+"_Histereza.csv", "./raporty_old/");
		}


		CSVParser parser2("./raporty/"+fileName+"_Wycieki.csv");
		std::vector<std::vector<std::string>> dataList2 = parser2.getData();
		if(dataList2.empty())
		{
			return false;
		}
		else
		{
			parser2.parseValuesLeakage(&dataList2,&Leakage,&vavleNo);
			moveFileToDirectory("./raporty/",fileName+"_Wycieki.csv", "./raporty_old/");
		}


		CSVParser parser3("./raporty/"+fileName+"_Dostrajanie.csv");
		std::vector<std::vector<std::string>> dataList3 = parser3.getData();
		if(dataList3.empty())
		{
			return false;
		}
		else
		{
			parser3.parseValuesFinal(&dataList3, &Measurements);
			moveFileToDirectory("./raporty/",fileName+"_Dostrajanie.csv", "./raporty_old/");
		}

		// prepare pdf raport file
		Raport raportFile(fileName+".pdf", &Measurements, &Leakage, &histerezis, &vavleNo, emersonLogo.getRGBVector(), ascoLogo.getRGBVector());

		return true;
}
