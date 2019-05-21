#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <fstream> //CSVParser:getData()
#include "csv_parser.h"
#include <iostream>

std::vector<std::vector<std::string>> CSVParser::getData()
{
	std::vector<std::vector<std::string> > dataList;
	std::string line = "";
	std::ifstream file(mFileName);
	if(file.is_open())
	{
		// Iterate through each line and split the content using delimeter
		while (getline(file, line))
		{
			std::vector<std::string> vec;
			boost::algorithm::split(vec, line, boost::is_any_of(mDelimeter));
			dataList.push_back(vec);
		}
		file.close();
	}
	else
	{
		std::cerr << "CSVParser::getData() error during opening file: " << mFileName << std::endl;
	}

	return dataList;
}

bool CSVParser::parseValuesHisteresis(std::vector<std::vector<std::string>> * list,
		std::vector<double> * PWM,
		std::vector<double> * Flow,
		std::vector<double> * Current,
		unsigned *valveNumber)
{
	list->erase(list->begin()); // remove first line with col descriptions
	for(std::vector<std::string> vec : *list)
	{
		// zamiana "," na "."
		boost::replace_all((*(begin(vec)+3)), ",", ".");
		boost::replace_all((*(begin(vec)+4)), ",", ".");
		boost::replace_all((*(begin(vec)+5)), ",", ".");

		// wyłuskanie pwm, flow i current i numer zaworu
		*valveNumber=std::stoi(*(begin(vec)+2));
		PWM->push_back(std::stod(*(begin(vec)+3)));
		Flow->push_back(std::stod(*(begin(vec)+4)));
		Current->push_back(std::stod(*(begin(vec)+5)));

	}
	return true;
}

bool CSVParser::parseValuesLeakage(std::vector<std::vector<std::string>> * list, std::vector<double> *Leakage, unsigned *valveNumber)
{
	list->erase(list->begin()); // remove first line with col descriptions
	std::vector<std::string> vec = list->at(0);
	// zamiana "," na "."
	for(auto i = end(vec)-6; i!=end(vec); i++)
	{
		boost::replace_all(*i, ",", ".");
	}

	*valveNumber=std::stoi(*(begin(vec)+2));
	// wrzucenie wyników testów szczelności do jednego kontenera
	for (int i = 3; i<=8; i++)
	{
		Leakage->push_back(std::stod(*(begin(vec)+i)));
	}
	return true;
}

bool CSVParser::parseValuesFinal(std::vector<std::vector<std::string>> * list, std::vector<double> *Measurements)
{
	list->erase(list->begin()); // remove first line with col descriptions
	std::vector<std::string> vec = list->at(0);
		// zamiana "," na "."
	for(auto i = end(vec)-7; i!=end(vec); i++)
	{
		boost::replace_all(*i, ",", ".");
	}
	for (int i = 2; i<=6; i++)
		{
			Measurements->push_back(std::stod(*(begin(vec)+i)));
		}
	return true;
}

