#include <string>
#include <vector>
#include <iterator>

class CSVParser
{
	std::string mFileName;
	std::string mDelimeter;

public:
	CSVParser(std::string filename, std::string delm = ";") :
			mFileName(filename), mDelimeter(delm)
	{ }
//TODO dopisac sprawdzanie bledow sciezki, delimitera itp
	std::vector<std::vector<std::string> > getData();
	bool parseValuesHisteresis(std::vector<std::vector<std::string>> * list, std::vector<double> * PWM, std::vector<double> * Flow, std::vector<double> * Current, unsigned *valveNumber);
	bool parseValuesLeakage(std::vector<std::vector<std::string>> * list, std::vector<double> *Leakage, unsigned *valveNumber);
	bool parseValuesFinal(std::vector<std::vector<std::string>> * list, std::vector<double> *Measurements);

};

#ifndef CVS_PARSER_H_
#define CVS_PARSER_H_





#endif /* CVS_PARSER_H_ */
