#ifndef _SENSOR_PROPRIOCEPTIVE_H
#define _SENSOR_PROPRIOCEPTIVE_H

#include <ode/ode.h>
#include "sensor.h"

class PROPRIOCEPTIVE_SENSOR : public SENSOR {

public:
	PROPRIOCEPTIVE_SENSOR(int myID, int evalPeriod) : SENSOR(myID, evalPeriod) {};

	void Poll(dJointID joint, int type, int t);
};

#endif // _SENSOR_PROPRIOCEPTIVE_H
