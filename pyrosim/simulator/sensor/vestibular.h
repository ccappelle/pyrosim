#ifndef _SENSOR_VESTIBULAR_H
#define _SENSOR_VESTIBULAR_H

#include <ode/ode.h>
#include "sensor.h"

class VESTIBULAR_SENSOR : public SENSOR {

public:
	VESTIBULAR_SENSOR(int myID, int evalPeriod) : SENSOR(myID, evalPeriod) {};

	void Poll(dBodyID body, int t);
};

#endif // _SENSOR_VESTIBULAR_H
