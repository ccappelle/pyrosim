#ifndef _SENSOR_TOUCH_H
#define _SENSOR_TOUCH_H

#include <ode/ode.h>
#include "sensor.h"

class NEURON;

class TOUCH_SENSOR : public SENSOR {

public:
	TOUCH_SENSOR(int myID, int evalPeriod);
	~TOUCH_SENSOR(void) {};

	void Fires(int t);
	void Update_Sensor_Neurons(int t);
};

#endif // _SENSOR_TOUCH_H
