#ifndef _SENSOR_PROPRIOCEPTIVE_CPP
#define _SENSOR_PROPRIOCEPTIVE_CPP

#include "proprioceptive.h"
#include "constants.h"

extern const int HINGE_ACTUATOR_ID;
extern const int SLIDER_ACTUATOR_ID;

void PROPRIOCEPTIVE_SENSOR::Poll(dJointID joint, int type, int t) {

	if(type==HINGE_ACTUATOR_ID)
		values[0][t] = dJointGetHingeAngle(joint);
	else if(type==SLIDER_ACTUATOR_ID)
		values[0][t] = dJointGetSliderPosition(joint);
}

#endif // _SENSOR_PROPRIOCEPTIVE_CPP
