/*
 * logFile.h
 *
 *  Created on: 25 cze 2019
 *      Author: Bartosz Komar
 */

#ifndef LOGFILE_H_
#define LOGFILE_H_
#include <iostream>
#include <time.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

void printAndLog(std::string message);
const std::string currentDateTime();

#endif /* LOGFILE_H_ */
