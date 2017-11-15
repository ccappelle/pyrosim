#ifndef _TOUCH_SENSOR_CPP
#define _TOUCH_SENSOR_CPP

#include "iostream"
#include "touchSensor.h"
#include "neuron.h"
#include <sstream>

TOUCH_SENSOR::TOUCH_SENSOR(int myID, int evalPeriod) {

	ID = myID;

	values = new int[evalPeriod];

	for (int t = 0 ; t < evalPeriod ; t++ )

		values[t] = 0;

        mySensorNeuron = NULL;
}

TOUCH_SENSOR::~TOUCH_SENSOR(void) {

}

void TOUCH_SENSOR::Connect_To_Sensor_Neuron(NEURON *sensorNeuron) {

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

    std::ostringstream outString;
    outString << ID << " " << 1 << " ";
    for (int t=0; t<evalPeriod; t++)
    {
        outString << values[t] << " ";
    }
    std::cout << outString.str() << std::endl;
}

#endif
