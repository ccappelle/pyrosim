#ifndef _SENSOR_TOUCH_CPP
#define _SENSOR_TOUCH_CPP

#include <iostream>
#include <sstream>

#include "touch.h"
#include "../neuron.h"

TOUCH_SENSOR::TOUCH_SENSOR(int myID, int evalPeriod) {
	ID = myID;

	values = new double[evalPeriod];

	for (int t = 0 ; t < evalPeriod ; t++ )
		values[t] = 0;
}

void TOUCH_SENSOR::Fires(int t) {

	values[t] = 1.;
}

void TOUCH_SENSOR::Update_Sensor_Neurons(int t) {

        if ( mySensorNeuron )

                mySensorNeuron->Set( values[t] );
}

#endif // _SENSOR_TOUCH_CPP
