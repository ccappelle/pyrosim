#ifndef _TOUCH_SENSOR_CPP
#define _TOUCH_SENSOR_CPP

#include <iostream>
#include <sstream>

#include "touchSensor.h"
#include "neuron.h"

TOUCH_SENSOR::TOUCH_SENSOR(int myID, int evalPeriod) {

	ID = myID;

	values = new int[evalPeriod];

	for (int t = 0 ; t < evalPeriod ; t++ )

		values[t] = 0;

        mySensorNeuron = NULL;
}

TOUCH_SENSOR::~TOUCH_SENSOR(void) {

}

void TOUCH_SENSOR::Connect_To_Sensor_Neuron(int sensorValueIndex, NEURON *sensorNeuron) {

        mySensorNeuron = sensorNeuron;
}

void TOUCH_SENSOR::Fires(int t) {

	values[t] = 1;
}

int  TOUCH_SENSOR::Get_ID(void) {

        return ID;
}

void TOUCH_SENSOR::Update_Sensor_Neurons(int t) {

        if ( mySensorNeuron )

                mySensorNeuron->Set( values[t] );
}

void TOUCH_SENSOR::Write_To_Python(int evalPeriod) {

	std::ostringstream oss;

	oss << ID << " " << 1. << " ";
	for(int t=0; t<evalPeriod; t++)
		oss << values[t] << " ";
	oss << "\n";

	std::cout << oss.str();
}

#endif // _TOUCH_SENSOR_CPP
