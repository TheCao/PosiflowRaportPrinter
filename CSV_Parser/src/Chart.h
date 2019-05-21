/*
 * Chart.h
 *
 *  Created on: 16 maj 2019
 *      Author: Bartosz Komar
 */

#ifndef CHART_H_
#define CHART_H_

#include "pdf.h"
#include <vector>

class Chart {
public:
	Chart();
	Chart(int ox, int oy, int width, int height):
		mOx(ox), mOy(oy), mWidth(width), mHeight(height)
	{
		mPdf = NULL;
		mMAX_CURRENT = 0;
		mMAX_FLOW = 0;
	};
	virtual ~Chart();
	bool printBackground(const std::string oxLabel, const std::string oyLabel);
	void printArrowUp(int ox, int oy,int width, int height );
	void printArrowLeft(int ox, int oy,int width, int height);
	void printHisteresisValuesOnChart(std::vector<std::vector<double>> *histeresisValueVector);
	void setPdfPointer(PDF *pdf);
	void setMaxFlow(int value);
	void setMaxCurrent(int value);

private:
	int mOx, mOy, mWidth, mHeight;
	PDF *mPdf;
	int mMAX_FLOW,mMAX_CURRENT;
};

#endif /* CHART_H_ */
