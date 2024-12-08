#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>								//добавить чтобы инпут файл запрашивался снова если файл не найден. проверка на ввод. повтор программы
#include <sstream>

double string_to_double(const std::string& s)
{
	std::istringstream i(s);
	double x;
	if (!(i >> x))
		return -1;
	return x;
}
char charInput()       //проверка корректного ввода символов
{
	char input;
	while (!(std::cin >> input) || std::cin.peek() != '\n')
	{
		std::cin.clear();
		while (std::cin.get() != '\n');
		std::cout << "Uncorrect input. " << std::endl;
	}
	return input;
}
double doubleInput()		//проверка на ввод.
{
	double input;
	while (!(std::cin >> input) //соответсвие введенного значения типу переменной
		|| std::cin.peek() != '\n')  //проверка на конец ввода
	{
		std::cin.clear();	//"лечим" поток. +состояние good
		while (std::cin.get() != '\n');
		std::cout << "Ucorrect input. Only numbers please. " << std::endl;
	}
	return input;
}
int intInput()		//проверка на ввод.
{
	int input;
	while (!(std::cin >> input) //соответсвие введенного значения типу переменной
		|| std::cin.peek() != '\n')  //проверка на конец ввода
	{
		std::cin.clear();	//"лечим" поток. +состояние good
		while (std::cin.get() != '\n');
		std::cout << "Uncorrect input. Only numbers please. " << std::endl;
	}
	return input;
}

class TempetureMeasure {
	//1 
	// 36.6 
	// C
	int count;
	double tempeture;
	char typeTemp;
public:
	TempetureMeasure() : count(0), tempeture(0.0), typeTemp(' ') {}

	TempetureMeasure(int newcount, double newtempeture, char newtypetemp) {
		count = newcount;
		tempeture = newtempeture;
		typeTemp = newtypetemp;
	}

	TempetureMeasure(const TempetureMeasure& c) {
		count = c.count;
		tempeture = c.tempeture;
		typeTemp = c.typeTemp;
	}

	~TempetureMeasure() {}

	double getTempeture() {
		return tempeture;
	}
	char getTypeTemp() {
		return typeTemp;
	}

	std::string celOrFar() const {
		std::string result;
		if (typeTemp == 'C') {
			double fahrenheitTemp = (tempeture * 9.0 / 5.0) + 32.0;
			result = "Temperature in Celsius: " + std::to_string(tempeture) + " C\n" + "Temperature in Fahrenheit: " + std::to_string(fahrenheitTemp) + " F\n\n";
		}
		else if (typeTemp == 'F') {
			double celsiusTemp = (tempeture - 32.0) * 5.0 / 9.0;
			result = "Temperature in Fahrenheit: " + std::to_string(tempeture) + " F\n" + "Temperature in Celsius: " + std::to_string(celsiusTemp) + " C\n\n";
		}
		else {
			result = "ERROR. Uncorrect type of temperature. \n";
		}
		return result;
	}
	friend std::istream& operator>>(std::istream& is, TempetureMeasure& r) // перегрузка оператора взятия из потока
	{
		std::string protCount;
		std::string protTempeture;
		std::string protTypeTemp;

		is >> protCount;
		r.count = string_to_double(protCount);
		if (r.count == -1) {
			r.tempeture = 0;
			r.typeTemp = 'E';
			return is;
		}
		else {
			is >> protTempeture;
			r.tempeture = string_to_double(protTempeture);
			if (r.tempeture == -1)
			{
				r.typeTemp = 'E';
				return is;
			}
			else {
				is >> protTypeTemp;
				if (protTypeTemp != "C" and protTypeTemp != "F") {
					r.typeTemp = 'E';
					return is;
				}
				r.typeTemp = protTypeTemp.back();
			}
		}

		return is;
	}

	// перегрузка оператора <<
	friend std::ostream& operator<< (std::ostream& output, const TempetureMeasure& c)
	{
		output << std::endl << c.count << " measure: " << c.tempeture << " " << c.typeTemp;
		return output;
	}
};

void inputFromFile(std::vector<TempetureMeasure>& temperatures, const std::string& file) {
	std::ifstream input_file(file);
	if (!input_file.is_open()) {
		std::cout << "file is not found" << std::endl;
	}
	TempetureMeasure Tempeture;
	while (!input_file.eof()) {
		input_file >> Tempeture;
		if (!(Tempeture.getTypeTemp() == 'E' and temperatures.size() > 0 and temperatures.back().getTypeTemp() == 'E')) temperatures.push_back(Tempeture);
	}
	input_file.close();
}

void outputToFile(const std::vector<TempetureMeasure>& temperatures, const std::string& filename) {
	std::ofstream outputFile(filename);
	for (const auto& temperature : temperatures) {
		outputFile << temperature << std::endl;
		outputFile << temperature.celOrFar();
	}
	outputFile.close();
}

std::vector<TempetureMeasure> readConsole() {
	bool asking = true;
		std::vector<TempetureMeasure> temps;
		asking = true;
		std::cout << "enter how many measure of temperature will be: ";
		int m = intInput();
		if (m < 0) {
			while (asking) {
				std::cout << "enter how many measure of temperature will be: ";
				m = intInput();
				if (m > 0) asking = false;
			}
		}
		for (int j = 1; j < m + 1; j++) {
			std::cout << "enter your " << j << " temperature: ";
			double tempp = doubleInput();
			std::cout << "enter type (C/F): ";
			char typeTemp = charInput();
			TempetureMeasure temp = TempetureMeasure(j, tempp, typeTemp);
			temps.push_back(temp);
		}

	return temps;
}

std::vector<std::string> explode(const std::string& delimiter, const std::string& str)
{
	std::vector<std::string> strings;

	int strleng = str.length();
	int delleng = delimiter.length();
	if (delleng == 0)
		return strings;//no change

	int i = 0;
	int k = 0;
	while (i < strleng)
	{
		int j = 0;
		while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
			j++;
		if (j == delleng)//found delimiter
		{
			strings.push_back(str.substr(k, i - k));
			i += delleng;
			k = i;
		}
		else
		{
			i++;
		}
	}
	strings.push_back(str.substr(k, i - k));
	return strings;
}


int main() {
	std::vector<TempetureMeasure> temperatures;
	std::cout << "This program will show you your measurement in Celsius and Fahrenheit \n" << std::endl;
	
	bool is_need_next_run = true; //создание флага для повторного запуска программы
	while (is_need_next_run) {
		std::cout << "how would you like to give us information? through console or file? (C/F): ";
		bool answerContiune = true;
		while (answerContiune) {
			char answer = charInput();
			if (answer == 'C') {
				temperatures = readConsole();
				answerContiune = false;
			}

			else if (answer == 'F') {
				std::cout << "what is your input file name?: ";
				std::string filename;
				std::cin >> filename;
				inputFromFile(temperatures, filename);
				answerContiune = false;
			}
			else {
				std::cout << "uncorrect input. File or Console? ";
			}
		}

		answerContiune = true;
		std::cout << "where would you like us to write information? in console or in file? (C/F): ";
		while (answerContiune) {
			char answer = charInput();
			if (answer == 'C') {
				for (const auto& Tempeture : temperatures) {
					std::cout << Tempeture << std::endl;
					std::cout << Tempeture.celOrFar(); //Перевод температуры из одной единицы измерения в другую
				}
				answerContiune = false;
			}

			else if (answer == 'F') {
				std::cout << "What is your output file name?: ";
				std::string filename;
				std::cin >> filename;
				outputToFile(temperatures, filename);
				answerContiune = false;
			}
			else {
				std::cout << "uncorrect input. File or Console? ";
			}
		}
		temperatures.clear();
		bool is_need_processed = false;
		do {
			std::cout << "Wanna contiune? (Y/N):  ";
			char answer = charInput();
			if (answer == 'N') {
				std::cout << "Bye!" << std::endl;
				is_need_next_run = false;
				is_need_processed = true;
			}
			else if (answer == 'Y') {
				std::cout << "Contuine? ofc!" << std::endl;
				
				is_need_processed = true;
			}

		} while (!is_need_processed);

	}
}