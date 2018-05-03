#ifndef _SENSOR_TOUCH_H
#define _SENSOR_TOUCH_H

#include <ode/ode.h>
#include "sensor.h"

class NEURON;

class TOUCH_SENSOR : public SENSOR {

public:
	TOUCH_SENSOR(int myID, int evalPeriod) : SENSOR(myID, evalPeriod) {

		for(int t=0; t<evalPeriod; t++)
			values[0][t] = 0.;
	};

	void Fires(int t) {values[0][t] = 1.;};
};

#endif // _SENSOR_TOUCH_H
