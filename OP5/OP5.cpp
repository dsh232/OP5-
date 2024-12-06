#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>								//добавить чтобы инпут файл запрашивался снова если файл не найден. проверка на ввод. повтор программы

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

	std::string celOrFar() const{
		std::string result;
		if (typeTemp == 'C') {
			double fahrenheitTemp = (tempeture * 9.0 / 5.0) + 32.0;
			result = "Temperature in Celsius: " + std::to_string(tempeture) + " C\n" + "Temperature in Fahrenheit: " + std::to_string(fahrenheitTemp) + " F\n";
		}
		else if (typeTemp == 'F') {
			double celsiusTemp = (tempeture - 32.0) * 5.0 / 9.0;
			result = "Temperature in Fahrenheit: " + std::to_string(tempeture) + " F\n" + "Temperature in Celsius: " + std::to_string(celsiusTemp) + " C\n";
		}
		else {
			result = "Uncorrect type of temperature. \n";
		}
		return result;
	}

	// перегрузка оператора >>
	friend std::istream& operator>> (std::istream& input, TempetureMeasure& c)
	{
		input >> c.count >> c.tempeture >> c.typeTemp;
		if (c.count < 1 || input.fail()) {
			input.setstate(std::ios::failbit);
		}
		return input;
	}
	// перегрузка оператора <<
	friend std::ostream& operator<< (std::ostream& output, const TempetureMeasure& c)
	{
		output << c.count << " measure: " << c.tempeture << " " << c.typeTemp;
		return output;
	}
};


void inputFromFile(std::vector<TempetureMeasure>& temperatures, const std::string& file) {
	std::ifstream input_file(file); 
	if (!input_file.is_open()) {
		std::cout << "file is not found" << std::endl;
	}
	TempetureMeasure Tempeture;
	while (input_file >> Tempeture) {
		if (input_file.fail()) {   
			std::cout << "uncorrect input in file." << std::endl;
		}
		temperatures.push_back(Tempeture);
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
	bool answerContiune = true;
	std::cout << "how would you like to give us information? through console or file? (C/F): ";
	while (answerContiune) {
		char answer = charInput();
		if (answer == 'C') {
			std::cout << "enter how much of measure will be: ";
			int countMeasure = intInput();
			while (countMeasure < 1) {
				std::cout << "enter correct measure: ";
				countMeasure = intInput();
			}
			std::cout << "Enter your measures in this way: Measurement _ your value _ type of the temperature." << std::endl;
			for (int i = 0; i < countMeasure; i++) {
				TempetureMeasure temperature;
				if (std::cin >> temperature) {
					temperatures.push_back(temperature);
				}
				else {
					std::cout << "uncorrect enter. please try again. ";
					std::cin.clear();
					std::cin.ignore(1000, '\n');
					--i;
				}
			}
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
}