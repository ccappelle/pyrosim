#ifndef _SENSOR_PROPRIOCEPTIVE_CPP
#define _SENSOR_PROPRIOCEPTIVE_CPP

#include <iostream>

#include "proprioceptive.h"
#include "constants.h"
#include "neuron.h"

extern const int HINGE_ACTUATOR_ID;
extern const int SLIDER_ACTUATOR_ID;

void PROPRIOCEPTIVE_SENSOR::Poll(dJointID joint, int type, int t) {

	if(type==HINGE_ACTUATOR_ID)
		values[t] = dJointGetHingeAngle(joint);
	else if(type==SLIDER_ACTUATOR_ID)
		values[t] = dJointGetSliderPosition(joint);
}

void PROPRIOCEPTIVE_SENSOR::Update_Sensor_Neurons(int t) {

	if ( mySensorNeuron )
		mySensorNeuron->Set( values[t] );
}

#endif // _SENSOR_PROPRIOCEPTIVE_CPP
