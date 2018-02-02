#ifndef _SENSOR_LIGHT_H
#define _SENSOR_LIGHT_H

#include <ode/ode.h>
#include "sensor.h"

class LIGHT_SENSOR : public SENSOR {

public:
	LIGHT_SENSOR(int myID, int evalPeriod) : SENSOR(myID, evalPeriod) {};
	void Poll(dBodyID body, dBodyID lightSource, int t);
};

#endif // _SENSOR_LIGHT_H
