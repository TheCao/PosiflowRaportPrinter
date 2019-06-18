/*
 * raport.h
 *
 *  Created on: 29 kwi 2019
 *      Author: Bartosz Komar
 */

#ifndef RAPORT_H_
#define RAPORT_H_
#include "pdf.h"
#include <string>
#include <iostream>
#include <time.h>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include "Chart.h"


class Raport
{
	std::string mFileName;
	std::string mFileDirectory;
	PDF *mPdf;
	std::vector<double> *mMeasurements;
	std::vector<double> *mLeakages;
	std::vector<std::vector<double>> *mHisterezis;
	unsigned *mValveNo;
	const std::vector<std::vector<RGB>> *mLogo;
	const std::vector<std::vector<RGB>> *mLogo2;

public:
	Raport(std::string filename ,std::vector<double> *measurements=NULL, std::vector<double> *leakages=NULL, std::vector<std::vector<double>> *histerezis=NULL, unsigned *valveNo=0, const std::vector<std::vector<RGB>> *logo=NULL, const std::vector<std::vector<RGB>> *logo2=NULL, std::string directory = "", PDF *p=NULL):
		mFileName(filename), mFileDirectory(directory), mPdf(p), mMeasurements(measurements), mLeakages(leakages), mHisterezis(histerezis), mValveNo(valveNo), mLogo(logo), mLogo2(logo2)
		{
			mPdf = new PDF();
			createRaport();
		};

private:
	bool createRaport();
	bool saveRaport(std::string destinationDirectory = "./wydruki/");
	bool printRaport();
	void printOneParameter(std::string name, std::string unit, double value, int oy, int secondOx);
	void printSetup(int ox, int oy);
	void printLeakages(std::string unit, int ox, int oy);
	void printBatch(long batchNo, unsigned valveNo, int ox, int oy);
	void printDate(int ox, int oy);
	void printHisteresisTable(int ox, int oy);
	void printHisteresisChart(int ox, int oy);

};




#endif /* RAPORT_H_ */
