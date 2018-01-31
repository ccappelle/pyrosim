#ifndef _SENSOR_LIGHT_H
#define _SENSOR_LIGHT_H

#include <ode/ode.h>
#include "sensor.h"

class NEURON;

class LIGHT_SENSOR : public SENSOR {

public:
	LIGHT_SENSOR(int myID, int evalPeriod);
	~LIGHT_SENSOR(void) {};

	void Poll(dBodyID body, dBodyID lightSource, int t);

	void Update_Sensor_Neurons(int t);
};

#endif // _SENSOR_LIGHT_H
