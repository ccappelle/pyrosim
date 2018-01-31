#ifndef _SENSOR_LIGHT_CPP
#define _SENSOR_LIGHT_CPP

#include <iostream>
#include "light.h"
#include "../neuron.h"

LIGHT_SENSOR::LIGHT_SENSOR(int myID, int evalPeriod) {

	ID = myID;

	values = new double[evalPeriod];

	mySensorNeuron = NULL;
}

void LIGHT_SENSOR::Poll(dBodyID body, dBodyID lightSource, int t) {

	const dReal *myPos = dBodyGetPosition(body);

	const dReal *lightPos = dBodyGetPosition(lightSource);

	double xDiff = myPos[0] - lightPos[0];

        double yDiff = myPos[1] - lightPos[1];

        double zDiff = myPos[2] - lightPos[2];

	double distance = sqrt( pow(xDiff,2.0) + pow(yDiff,2.0) + pow(zDiff,2.0) );

	// Light decays with the inverse of the square of the distance...

	values[t] = 1.0 / pow(distance,2.0);
}

void LIGHT_SENSOR::Update_Sensor_Neurons(int t) {

	if ( mySensorNeuron )

		mySensorNeuron->Set( values[t] );
}

#endif // _SENSOR_LIGHT_CPP
