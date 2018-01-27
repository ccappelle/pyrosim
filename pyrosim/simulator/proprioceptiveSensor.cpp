#ifndef _PROPRIOCEPTIVE_SENSOR_CPP
#define _PROPRIOCEPTIVE_SENSOR_CPP

#include <iostream>

#include "proprioceptiveSensor.h"
#include "constants.h"
#include "neuron.h"

extern const int HINGE_ACTUATOR_ID;
extern const int SLIDER_ACTUATOR_ID;

void PROPRIOCEPTIVE_SENSOR::Connect_To_Sensor_Neuron(int sensorValueIndex, NEURON *sensorNeuron) {

	mySensorNeuron = sensorNeuron;
}

void PROPRIOCEPTIVE_SENSOR::Poll(dJointID joint, int type, int t) {

	if(type==HINGE_ACTUATOR_ID)
		angles[t] = dJointGetHingeAngle(joint);
	else if(type==SLIDER_ACTUATOR_ID)
		angles[t] = dJointGetSliderPosition(joint);
}

void PROPRIOCEPTIVE_SENSOR::Update_Sensor_Neurons(int t) {

	if ( mySensorNeuron )
		mySensorNeuron->Set( angles[t] );
}

void PROPRIOCEPTIVE_SENSOR::Write_To_Python(int evalPeriod) {

	char outString[1000000];
	sprintf(outString,"%d %d ",ID,1);

	for (int t = 0 ; t < evalPeriod ; t++)
		sprintf(outString,"%s %f ",outString,angles[t]);

	sprintf(outString,"%s \n",outString);

	std::cout << outString;
}

#endif // _PROPRIOCEPTIVE_SENSOR_CPP
