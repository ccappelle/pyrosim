#ifndef _SENSOR_SENSOR_CPP
#define _SENSOR_SENSOR_CPP

#include <sstream>
#include <string>
#include <iostream>

#include "sensor.h"

void SENSOR::Write_To_Python(int evalPeriod) {

	std::ostringstream oss;

	oss << ID << " " << 1 << " ";
	for(int t=0; t<evalPeriod; t++)
		oss << values[t] << " ";
	oss << "\n";

	std::cout << oss.str();
}

#endif // _SENSOR_SENSOR_CPP
