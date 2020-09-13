/*
 * logFile.cpp
 *
 *  Created on: 25 cze 2019
 *      Author: Bartosz Komar
 */

#include "logFile.h"
const boost::filesystem::path logFilePath = "./logfile.txt";

void printAndLog(std::string message)
{
	// openFile

	boost::filesystem::ofstream logFile(logFilePath, std::ofstream::app);
	if(boost::filesystem::exists(logFilePath))
	{
		logFile << "[" << currentDateTime() << "] "<< message <<std::endl;
	}
	else
	{
		std::cout << "Error with logging data to: " << logFilePath << std::endl;
	}
	std::cout << "[" << currentDateTime() << "] "<< message <<std::endl;
	//closeFile
	logFile.close();
}


const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    localtime_s(&tstruct ,&now);
    strftime(buf, sizeof(buf), "%d/%m/%Y %X", &tstruct);

    return buf;
}
