#ifndef _SENSOR_POSITION_H
#define _SENSOR_POSITION_H

#include <iostream>
#include <ode/ode.h>
#include "sensor.h"

class POSITION_SENSOR : public SENSOR {

private:
	double* x;
	double* y;
	double* z;

public:
	POSITION_SENSOR(int myID, int evalPeriod) : SENSOR(myID, evalPeriod, 3) {

		x = values[0]; y = values[1]; z = values[2];
//		std::cerr << "xyz values assigned: " << x << ' ' << y << ' ' << z << std::endl;
	};

	void Poll(dBodyID body, int t);
};

#endif // _SENSOR_POSITION_H
