#ifndef _SENSOR_VESTIBULAR_H
#define _SENSOR_VESTIBULAR_H

#include <ode/ode.h>
#include "sensor.h"

class NEURON;

class VESTIBULAR_SENSOR : public SENSOR {

public:
	VESTIBULAR_SENSOR(int myID, int evalPeriod);
	~VESTIBULAR_SENSOR(void) {};

	void Poll(dBodyID body, int t);
	void Update_Sensor_Neurons(int t);
};

#endif // _SENSOR_VESTIBULAR_H
