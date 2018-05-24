#include "Byte.h"
#include <string>
#include <iomanip>

Byte::~Byte()
{
}

std::string Byte::ToString()
{
	char buff[100];
	snprintf(buff, sizeof(buff), "0x%02X", store);
	std::string buffAsStdStr = buff;

	return buffAsStdStr;
}
