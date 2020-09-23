
#include <iostream>
#include "csv_parser.h"
#include "raport.h"
#include "dirent.h"
#include <boost/algorithm/string/replace.hpp>
#include "imageParser.h"
// included RGB vectors as *h files
#include "AscoLogo.h"
#include "EmersonLogo.h"
#include "logFile.h"

bool createRaport(std::string fileName);

std::vector<boost::filesystem::path> refreshFilesPathList(std::string directory="./");

bool moveFileToDirectoryByPath(boost::filesystem::path filePath, boost::filesystem::path dstDirectoryPath);
bool checkTestMeasurementsValues(boost::filesystem::path filePath);
bool checkTestLeakagesValues(boost::filesystem::path filePath);
bool parseDirectoryList(std::vector<boost::filesystem::path> *list);

struct paths
{
	 boost::filesystem::path raportyNokCatalogue;
	 std::string raportyOldCatalogue;
	 std::string raportyCatalogue;

	//Constructor to create const paths
	 void setData(std::string cat, std::string cat2, std::string cat3)
	 {
		 raportyCatalogue = cat;
		 raportyNokCatalogue = cat2;
		 raportyOldCatalogue = cat3;
	 }

} paths;


int main(int argc, char** argv)
{
	if (argc == 4)
	{
		paths.setData(argv[1], argv[2], argv[3]);
		std::string s = "Arguments provided: " + std::string(argv[1]) + ", " + std::string(argv[2])
			+ ", " + std::string(argv[3]);
		printAndLog(s);
	}
	else
	{
		printAndLog("No arguments provided. Using bacis ones !");
		paths.setData("./raporty", "./raporty_nok", "./raporty_old/");
	}

	printAndLog("Program gotowy do pracy.");
	std::vector<boost::filesystem::path> filesPathsList;
	while(true)
	{
		filesPathsList = refreshFilesPathList(paths.raportyCatalogue);
		parseDirectoryList(&filesPathsList);

	}
	system("pause");
	return 0;
}

std::vector<boost::filesystem::path> refreshFilesPathList(const std::string directory)
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
				if(!boost::filesystem::exists(paths.raportyNokCatalogue))
				{
					if(!boost::filesystem::create_directory(paths.raportyNokCatalogue))
					{
						printAndLog("checkTestMeasurementsValues error: nokCatalogue not exists!");
					}
					else printAndLog("checkTestMeasurementsValues info: nokCatalogue created");


				}
				moveFileToDirectoryByPath(filePath, paths.raportyNokCatalogue); // move to specified folder with error test report
				printAndLog(filePath.string() + " bledny! Przeniesiono do " + paths.raportyNokCatalogue.string());
				// check if histeresis file exsists
				std::string tempStr = filePath.string();
				boost::replace_all(tempStr, "Dostrajanie", "Histereza");
				boost::filesystem::path histeresisFilePath = tempStr;
				if(boost::filesystem::exists(histeresisFilePath))
				{
					moveFileToDirectoryByPath(histeresisFilePath, paths.raportyNokCatalogue); // move to specified folder with error test report
					printAndLog(histeresisFilePath.string() + " bledny! Przeniesiono do " + paths.raportyNokCatalogue.string());
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
				moveFileToDirectoryByPath(filePath, paths.raportyNokCatalogue); // move to specified folder with error test report
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
					//std::cout << "[" << currentDateTime() << "] Tworzenie raportu dla : " << tempStr3 << std::endl;
					printAndLog("Tworzenie raportu dla : "+ tempStr3);
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

		if(!boost::filesystem::exists(paths.raportyOldCatalogue))
		{
			if(!boost::filesystem::create_directory(paths.raportyOldCatalogue))
			{
				printAndLog("createRaport error: raportyOldCatalogue not exists!");
			}
			else printAndLog("createRaport info: raportyOldCatalogue created");
		}
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
			moveFileToDirectory("./raporty/",fileName+"_Histereza.csv", paths.raportyOldCatalogue);
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
			moveFileToDirectory("./raporty/",fileName+"_Wycieki.csv", paths.raportyOldCatalogue);
		}


		CSVParser parser3("./raporty/"+fileName+"_Dostrajanie.csv");
		std::vector<std::vector<std::string>> dataList3 = parser3.getData();
		if(dataList3.empty())
		{
			return false;
		}
		else
		{
			parser3.parseValuesMeasurements(&dataList3, &Measurements);
			moveFileToDirectory("./raporty/",fileName+"_Dostrajanie.csv", paths.raportyOldCatalogue);
		}

		// prepare pdf raport file
		// TODO: Remove. Depreciated function.
		//Raport raportFile(fileName+".pdf", &Measurements, &Leakage, &histerezis, &vavleNo, emersonLogo.getRGBVector(), ascoLogo.getRGBVector());
		//Raport raportFile(fileName+".pdf", &Measurements, &Leakage, &histerezis, &vavleNo,  &emersonLogoVector, &ascoLogoVector);
		Raport raportFile(fileName + ".pdf", &Measurements, &Leakage, &histerezis, &vavleNo, &ascoLogoVector, &emersonLogoVector);
		return true;
}
