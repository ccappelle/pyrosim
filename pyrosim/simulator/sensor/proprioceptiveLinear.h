#ifndef _SENSOR_PROPRIOCEPTIVE_LINEAR_H
#define _SENSOR_PROPRIOCEPTIVE_LINEAR_H

#include <ode/ode.h>
#include "sensor.h"

class PROPRIOCEPTIVE_LINEAR_SENSOR : public PROPRIOCEPTIVE_ROTARY_SENSOR {

public:
	PROPRIOCEPTIVE_LINEAR_SENSOR(int myID, int evalPeriod) : PROPRIOCEPTIVE_ROTARY_SENSOR(myID, evalPeriod) {};
	void Poll(dJointID joint, int t) {values[0][t] = dJointGetSliderPosition(joint);};
};

#endif // _SENSOR_PROPRIOCEPTIVE_LINEAR_H
