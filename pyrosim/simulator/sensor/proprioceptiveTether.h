#ifndef _SENSOR_PROPRIOCEPTIVE_TETHER_H
#define _SENSOR_PROPRIOCEPTIVE_TETHER_H

#include <ode/ode.h>
#include "sensor.h"
#include "../object.h"

class PROPRIOCEPTIVE_TETHER_SENSOR : public SENSOR {

public:
	PROPRIOCEPTIVE_TETHER_SENSOR(int myID, int evalPeriod) : SENSOR(myID, evalPeriod, 2) {};
	void Poll(double tension, int t) {
		values[0][t] = tension;
		values[1][t] = tension;
	};
};

#endif // _SENSOR_PROPRIOCEPTIVE_TETHER_H
