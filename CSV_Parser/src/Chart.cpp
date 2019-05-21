/*
 * Chart.cpp
 *
 *  Created on: 16 maj 2019
 *      Author: Bartosz Komar
 */

#include "Chart.h"
#include <iostream>

Chart::Chart() {
	// TODO Auto-generated constructor stub

}

Chart::~Chart() {
	// TODO Auto-generated destructor stub
}
void Chart::setPdfPointer(PDF *pdf)
{
	mPdf = pdf;
}
bool Chart::printBackground(const std::string oxLabel, const std::string oyLabel)
{
	printArrowUp(mOx,mOy,6,mHeight+15);
	printArrowLeft(mOx-6,mOy,6,mWidth+10);
	this->mPdf->showTextXY(oxLabel, mOx +mWidth -60, mOy - 35);
	this->mPdf->showTextXY(oyLabel, mOx-25, mOy + mHeight + 15);
	std::string value;
	for(int i=1; i <=10;i++)
	{
		this->mPdf->drawLine(mOx+(mWidth*i*0.1),mOy,mOx+(mWidth*i*0.1), mOy+mHeight);
		value = std::to_string(int(mMAX_CURRENT*(i*0.1)));
		if(value.length() == 3) this->mPdf->showTextXY(value,mOx+(mWidth*i*0.1)-10,mOy-20);
		else if(value.length() == 4) this->mPdf->showTextXY(value,mOx+(mWidth*i*0.1)-15,mOy-20);

		this->mPdf->drawLine(mOx, mOy+(mHeight*i*0.1), mOx +mWidth, mOy+(mHeight*i*0.1));
		value = std::to_string(int(mMAX_FLOW*(i*0.1)));
		if(value.length() == 2) this->mPdf->showTextXY(value,mOx-25, mOy-5+(mHeight*i*0.1));
		else if(value.length() == 3) this->mPdf->showTextXY(value,mOx-28, mOy-5+(mHeight*i*0.1));
		else if(value.length() == 4) this->mPdf->showTextXY(value,mOx-40, mOy-5+(mHeight*i*0.1));
	}
	return true;
}

void Chart::printArrowUp(int ox, int oy,int width, int height)
{
	const vector<XY> pointVect = {XY(ox,oy),
								  XY(ox-width, oy),
				 	 	 	 	  XY(ox-width,oy+height-10),
								  XY(ox-width-5,oy+height-10),
								  XY(ox-width/2,oy+height),
								  XY(ox+5,oy+height-10),
								  XY(ox,oy+height-10),
								  };
	this->mPdf->fillPolygon(pointVect);
}

void Chart::printArrowLeft(int ox, int oy,int width, int height)
{
	const vector<XY> pointVect = {XY(ox,oy),
				 	 	 	 	  XY(ox,oy-width),
								  XY(ox+height,oy-width),
								  XY(ox+height,oy-width-5),
								  XY(ox+height+10,oy-width/2),
								  XY(ox+height,oy+5),
								  XY(ox+height,oy)};
	this->mPdf->fillPolygon(pointVect);
}

void Chart::printHisteresisValuesOnChart(std::vector<std::vector<double>> *histeresisValueVector)
{
	// change double,double to XY
	vector<XY> vectorUp, vectorDown;
	std::vector<double> flow = histeresisValueVector->at(1);
	std::vector<double> current = histeresisValueVector->at(2);
	double temp = 0.0;
	bool firstDecreasing = false;
	// histeresis values
	for(int i=0; i<(int)flow.size(); i++)
	{
		// 1 px = MAX_CURRENT/chart.width = 1,7mA
		// 1 px = MAX_FLOW/chart.height [l/ min] = 1
		int tempFlow = (int)(flow.at(i)*mHeight/mMAX_FLOW)+mOy;
		int tempCurr = (int)(current.at(i)*1000*mWidth/mMAX_CURRENT)+mOx;

		if(tempCurr > temp) vectorUp.push_back(XY(tempCurr,tempFlow));
		if(tempCurr < temp)
		{
			if(!firstDecreasing)
			{
				vectorDown.push_back(XY((int)((current.at(i)*1000*mWidth/mMAX_CURRENT)+mOx),(int)(flow.at(i)*mHeight/mMAX_FLOW)+mOy)); //first decreatsing value
				firstDecreasing = true;
			}
			vectorDown.push_back(XY(tempCurr,tempFlow));
		}
		temp = tempCurr;
	}
	this->mPdf->setLineWidth(2);
	this->mPdf->setLineColor(0,191,255);
	this->mPdf->drawLine(vectorUp);
	this->mPdf->setLineColor(220,20,60);
	this->mPdf->drawLine(vectorDown);
}

void Chart::setMaxFlow(int value)
{
	mMAX_FLOW = value;
}

void Chart::setMaxCurrent(int value)
{
	mMAX_CURRENT = value;
}
