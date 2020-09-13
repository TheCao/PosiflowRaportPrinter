/*
 * raport.cpp
 *
 *  Created on: 29 kwi 2019
 *      Author: Bartosz Komar
 */
#include "raport.h"
#include "logFile.h"

static int sPrecisionValue = 2;

/*const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d/%m/%Y %X", &tstruct);

    return buf;
}*/
void Raport::printOneParameter(std::string name,std::string unit, double value, int oy, int secondOx)
{
	this->mPdf->setFont(PDF::TIMES, 14);
	std::string firstPart = name + ": ";
	this->mPdf->showTextXY(firstPart, 20,oy);
	std::string secondPart = (std::to_string(value).substr(0,std::to_string(value).find(".") + sPrecisionValue+1));
	this->mPdf->showTextXY(secondPart, secondOx,oy);
	this->mPdf->showTextXY(unit,secondOx+45, oy);

}
void Raport::printDate(int ox, int oy)
{
	this->mPdf->setFont(PDF::TIMES, 14);
	this->mPdf->showTextXY("Print time: " + currentDateTime(), ox, oy);

}
void Raport::printSetup(int ox, int oy)
{
	this->mPdf->setFont(PDF::TIMES, 16);
	this->mPdf->showTextXY("Setup parameters:", ox,oy);
	// resistance

	printOneParameter("Resistance", "", this->mMeasurements->at(0), oy-20,ox+80);
	this->mPdf->setFont(PDF::SYMBOL, 14);
	this->mPdf->showTextXY("W", ox+125,oy-20); // Ohm symbol
	//flow
	printOneParameter("Flow", "l/min", this->mMeasurements->at(1), oy-40,ox+80);
	//PWM
	printOneParameter("PWM","%", this->mMeasurements->at(2),  oy-60,ox+80);
	//current
	printOneParameter("Current","A", this->mMeasurements->at(3),  oy-80,ox+80);
	//freq
	printOneParameter("Frequency", "Hz",this->mMeasurements->at(4),  oy-100,ox+80);
}
void Raport::printLeakages(std::string unit, int ox, int oy)
{
	this->mPdf->setFont(PDF::TIMES, 16);
	this->mPdf->showTextXY("Leakages:", ox,oy);
	this->mPdf->setFont(PDF::TIMES, 14);
	std::string firstPart = "Leakage test no. ";
	for(int i=0; i<6;i++)
	{
		//if(this->mLeakages->at(i) != 0.0)
		{
			std::string temp = firstPart + std::to_string(i+1) + " :";
			this->mPdf->showTextXY(temp, ox, oy-20 - i*20 );
			std::string temp2 = (std::to_string(this->mLeakages->at(i)).substr(0,std::to_string(this->mLeakages->at(i)).find(".") + sPrecisionValue+1));
			if (mLeakages->at(i) <0) this->mPdf->showTextXY(temp2, ox+115, oy-20 - i*20);
			else this->mPdf->showTextXY(temp2, ox+119, oy-20 - i*20);
			this->mPdf->showTextXY(unit, ox+155, oy-20 - i*20);
		}
	}
}
void Raport::printHisteresisTable(int ox, int oy)
{
	this->mPdf->setFont(PDF::TIMES, 16);
	this->mPdf->showTextXY("Histeresis:", ox,oy);
	this->mPdf->setFont(PDF::TIMES, 14);
	std::string value;
	int actual_ox = ox, actual_oy=oy, actual_oy_increment = 0 ;

	// title verse
	this->mPdf->drawLine(actual_ox, actual_oy-26, actual_ox+134, actual_oy-26); //line under descriptions
	this->mPdf->showTextXY("PWM    Flow   Current",actual_ox+2,actual_oy-22);
	this->mPdf->drawLine(actual_ox+41, actual_oy-22, actual_ox+41,actual_oy-10);
	this->mPdf->drawLine(actual_ox+86, actual_oy-26, actual_ox+86,actual_oy-10);
	int iloscPomiarow = (int)mHisterezis->at(0).size();
	for(int i = 0; i <iloscPomiarow ; i++)
	{
		if(actual_oy <=30 || i == iloscPomiarow/2 )
		{
			actual_ox += 150;
			actual_oy = oy-20;
			actual_oy_increment = 0;
			this->mPdf->drawLine(actual_ox, actual_oy-6, actual_ox+134, actual_oy-6); //line under descriptions
			this->mPdf->showTextXY("PWM    Flow   Current",actual_ox+2,actual_oy-2);
			this->mPdf->drawLine(actual_ox+41, actual_oy-2, actual_ox+41,actual_oy+10);
			this->mPdf->drawLine(actual_ox+86, actual_oy-6, actual_ox+86,actual_oy+10);
		}
		actual_oy = oy - 20*(actual_oy_increment+2);
		// PWM
		this->mPdf->drawLine(actual_ox+41, actual_oy-3, actual_ox+41,actual_oy+20);
		value = (std::to_string(this->mHisterezis->at(0).at(i)).substr(0,std::to_string(this->mHisterezis->at(0).at(i)).find(".") + sPrecisionValue+1));
		if(value.length()==6) this->mPdf->showTextXY(value, actual_ox, actual_oy);
		if(value.length()==5) this->mPdf->showTextXY(value, actual_ox+4, actual_oy);
		if(value.length()==4) this->mPdf->showTextXY(value, actual_ox+8, actual_oy);
		this->mPdf->drawLine(actual_ox, actual_oy-5, actual_ox+134,actual_oy-5);

		// Flow
		this->mPdf->drawLine(actual_ox+86, actual_oy-3, actual_ox+86,actual_oy+20);
		value = (std::to_string(this->mHisterezis->at(1).at(i)).substr(0,std::to_string(this->mHisterezis->at(0).at(i)).find(".") + sPrecisionValue+1));
		if(value.length()==6) this->mPdf->showTextXY(value, actual_ox+44, actual_oy);
		if(value.length()==5) this->mPdf->showTextXY(value, actual_ox+47, actual_oy);
		if(value.length()==4) this->mPdf->showTextXY(value, actual_ox+55, actual_oy);

		// Current
		value = (std::to_string(this->mHisterezis->at(2).at(i)).substr(0,std::to_string(this->mHisterezis->at(0).at(i)).find(".") + sPrecisionValue+1));
		if(value.length()>=6) value.resize(5);
		if(value.length()==5) this->mPdf->showTextXY(value, actual_ox+94, actual_oy);
		if(value.length()==4) this->mPdf->showTextXY(value, actual_ox+98, actual_oy);

		actual_oy_increment += 1;
	}

}
void Raport::printHisteresisChart(int ox, int oy)
{
	const int CHART_HEIGHT = 400;
	const int CHART_WIDTH = 560;
	this->mPdf->setFont(PDF::TIMES, 16);
	this->mPdf->showTextXY("Histeresis chart:", ox,oy+CHART_HEIGHT+45);
	this->mPdf->setFont(PDF::TIMES, 14);
	Chart histeresisChart = Chart(ox,oy,CHART_WIDTH,CHART_HEIGHT);
	histeresisChart.setMaxCurrent(1000);
	histeresisChart.setMaxFlow(400);
	histeresisChart.setPdfPointer(mPdf);
	histeresisChart.printBackground("Current [mA]", "Flow [l/min]");
	histeresisChart.printHisteresisValuesOnChart(mHisterezis);
}
void Raport::printBatch(long batchNo, unsigned valveNo, int ox, int oy)
{
	this->mPdf->setFont(PDF::TIMES, 14);
	std::string batch = "Batch no. " + std::to_string(batchNo);
	std::string valve = "Valve no. " + std::to_string(valveNo);
	this->mPdf->showTextXY(batch, ox,oy);
	this->mPdf->showTextXY(valve, ox, oy-20);
}
bool Raport::createRaport()
{
	printAndLog("T1");
	this->mPdf->setFont(PDF::TIMES_BOLD, 20);
	this->mPdf->showTextXY("Posiflow raport summary", 200,750);
	this->mPdf->printLogo(475,735,120,50, mLogo);
	this->mPdf->printLogo(20,722,120,50, mLogo2);
	printAndLog("T2");
	printDate(20,700);
	printAndLog("T3");
	printBatch(this->mMeasurements->at(5), *this->mValveNo,20,680); //Measurements vector's 7 position -> Batch Number from _Dostrajanie.cvs file
	printAndLog("T4");
	printSetup(20, 630);
	printAndLog("T5");
	printLeakages("sccm/min", 350, 630);
	printAndLog("T6");
	//printHisteresisTable(20,500); //Old one.
	printHisteresisChart(30,40);
	printAndLog("T6");

	saveRaport();
	return true;
}

bool Raport::saveRaport(std::string destinationDirectory)
{
	std::string errorMessage;
	if(!this->mPdf->writeToFile(this->mFileName,errorMessage))
	{
		std::cerr << errorMessage << std::endl;
		return false;
	}
	else
	{
		//std::cout << "[" << currentDateTime() << "] File Successfully Written as " << this->mFileName << std::endl;
		//std::cout << "[" << currentDateTime() << "] Plik zapisano jako: " << this->mFileName << std::endl;
		printAndLog("Plik zapisano jako: " + this->mFileName);
		try
		{
			boost::filesystem::path src = ("./"+this->mFileName);
			if(!(boost::filesystem::exists(boost::filesystem::path(destinationDirectory))))
			{
				if(!boost::filesystem::create_directory(boost::filesystem::path(destinationDirectory)))
				{
					printAndLog("saveRaport error: " + destinationDirectory+ " not exists!");
				}
				else printAndLog("saveRaport error: " + destinationDirectory+ " created.");
			}
			boost::filesystem::path	dest = (destinationDirectory+this->mFileName);
			boost::filesystem::rename(src, dest); // move ready *.pdf to specified folder
		}
		catch (boost::filesystem::filesystem_error &error)
		{
			std::cerr << error.what() << std::endl;
		}

		return true;
	}
}

