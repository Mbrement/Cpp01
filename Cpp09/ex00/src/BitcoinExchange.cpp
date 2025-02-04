#include "../header/BitcoinExchange.hpp"


BitcoinExchange::BitcoinExchange(std::string data) {
	_str = data;
	_data.open(data.c_str(), std::fstream::in);
	if (!_data.is_open())
		throw CreateExept();
	std::ifstream csv("data.csv");
	if (!csv.is_open())
	{
		std::cout << "Error  : Can't find csv" << std::endl;
		throw CreateExept();
	}

	std::string str;
	std::string str2;
	std::getline(csv, str);
	while (std::getline(csv, str, ','))
	{
		std::getline(csv, str2);
		_map[str] = std::atof(str2.c_str());
	}
}

BitcoinExchange	&BitcoinExchange::operator=(const BitcoinExchange &BitcoinExchange)
{
	_str = BitcoinExchange._str;
	
	_data.open(_str.c_str(), std::fstream::in);
	if (!_data.is_open())
	{
		throw CreateExept();
	}
	return *this;
}

BitcoinExchange::BitcoinExchange (const BitcoinExchange &BitcoinExchange)
{
	*this = BitcoinExchange;
}

BitcoinExchange::~BitcoinExchange()
{

}



BitcoinExchange::BitcoinExchange(){

}

bool check_date_format(std::string str)
{
	int month;
	int year;
	int day;
	if ((str.size() != 10 || str[4] != '-') && str[6] != '-')
		return false;
	for (int i = 0; str[i]; i++)
		if (i != 4 && i != 7 && !std::isdigit(str[i]))
			return false;

	if (str < "2009-01-02")
	{
		std::cout << "Data too old ";
		return false;
	}
	day = atoi(str.c_str() + 8);
	year = atoi(str.c_str());
	month = atoi(str.c_str() + 5);
	if (month > 12 || month < 1)
		return false;
	if (day > 31 || day < 1 || \
		(day > 30 && (month == 4 || month == 6 || month == 9 || month == 11)) || \
		(day > 29 && month == 2 && year % 4 == 0) || \
		(day > 28 && month == 2 && year % 4 != 0))
		return false;
	return true;
}

void BitcoinExchange::run()
{
	std::string str;
	std::getline(_data, str);
	if (str != "date | value")
		throw FormatExept();

	while (std::getline(_data, str, ' '))
	{
		size_t count = 0;
		if (!check_date_format(str))
		{
			std::cout << "Wrong format of line" << std::endl;
			std::getline(_data, str);
			continue;
		}

		std::string str2;
		std::getline(_data, str2, ' ');
		if (str2 != "|")
		{
			std::cout << "Wrong format of line" << std::endl;
			std::getline(_data, str);
			continue;
		}

		std::getline(_data, str2);
		for (int i = 0; str2[i]; i++)
		{
			if (!std::isdigit(str2[i]))
			{
				if (str2[i] == '.')
				{
					count++; 
					if (count > 1)
					{
						break;
					}
				}
				else
				{
					count += 2;
					break;
				}
			}
		}
		if (count > 1)
		{
			std::getline(_data, str);
			std::cout << "Wrong format of line" << std::endl;
			continue;
		}

		double tmp = std::atof(str2.c_str());
		if ( tmp > 1000.0 || tmp < 1.0 )
		{		
			std::cout << "Wrong format of line" << std::endl;
			continue;
		}
		std::map<std::string, float>::const_iterator it = _map.upper_bound(str);
		std::cout << (--it)->second * tmp << std::endl;
	}
}