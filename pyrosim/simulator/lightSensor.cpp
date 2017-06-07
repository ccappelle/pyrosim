#ifndef _LIGHT_SENSOR_CPP
#define _LIGHT_SENSOR_CPP

#include "iostream"
#include "lightSensor.h"
#include "neuron.h"

LIGHT_SENSOR::LIGHT_SENSOR(int myID, int evalPeriod) {

	ID = myID;

	values = new double[evalPeriod];

	mySensorNeuron = NULL;
}

LIGHT_SENSOR::~LIGHT_SENSOR(void) {

}

void LIGHT_SENSOR::Connect_To_Sensor_Neuron(NEURON *sensorNeuron) {

	mySensorNeuron = sensorNeuron;
}

int  LIGHT_SENSOR::Get_ID(void) {

	return ID;
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

void LIGHT_SENSOR::Write_To_Python(int evalPeriod) {

        char outString[1000000];

        sprintf(outString,"%d %d ",ID,1);

        for ( int  t = 0 ; t < evalPeriod ; t++ )

                sprintf(outString,"%s %f ",outString,values[t]);

        sprintf(outString,"%s \n",outString);

        std::cout << outString;
}

#endif
