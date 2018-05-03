#ifndef _SENSOR_PROPRIOCEPTIVE_ROTARY_H
#define _SENSOR_PROPRIOCEPTIVE_ROTARY_H

#include <ode/ode.h>
#include "sensor.h"

class PROPRIOCEPTIVE_ROTARY_SENSOR : public SENSOR {

public:
	PROPRIOCEPTIVE_ROTARY_SENSOR(int myID, int evalPeriod) : SENSOR(myID, evalPeriod, 1) {};
	virtual void Poll(dJointID joint, int t) {values[0][t] = dJointGetHingeAngle(joint);}; // made virtual to make it reloadable at _LINEAR_
};

#endif // _SENSOR_PROPRIOCEPTIVE_ROTARY_H
