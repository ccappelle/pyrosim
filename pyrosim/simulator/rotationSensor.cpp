#ifndef _ROTATION_SENSOR_CPP
#define _ROTATION_SENSOR_CPP

#include "iostream"
#include "rotationSensor.h"
#include "neuron.h"

ROTATION_SENSOR::ROTATION_SENSOR(int myID, int evalPeriod) {

	ID = myID;

	x = new double[evalPeriod];

	y = new double[evalPeriod];

	z = new double[evalPeriod];

	for ( int i = 0 ; i < 3 ; i++)

        	mySensorNeurons[i] = NULL;
}

ROTATION_SENSOR::~ROTATION_SENSOR(void) {

}

void ROTATION_SENSOR::Connect_To_Sensor_Neuron(NEURON *sensorNeuron) {

	mySensorNeurons[ sensorNeuron->Get_Sensor_Value_Index() ] = sensorNeuron;
}

int  ROTATION_SENSOR::Get_ID(void) {

        return ID;
}

void ROTATION_SENSOR::Poll(dBodyID body, int t) {

        const dReal *R = dBodyGetRotation(body);

        double a[3] = {0,0,1};

        x[t] = R[0]*a[0] + R[1]*a[1] + R[2]*a[2];

        y[t] = R[4]*a[0] + R[5]*a[1] + R[6]*a[2];

        z[t] = R[8]*a[0] + R[9]*a[1] + R[10]*a[2];
}

void ROTATION_SENSOR::Update_Sensor_Neurons(int t) {

        if ( mySensorNeurons[0] )

                mySensorNeurons[0]->Set( x[t] );

        if ( mySensorNeurons[1] )

                mySensorNeurons[1]->Set( y[t] );

        if ( mySensorNeurons[2] )

                mySensorNeurons[2]->Set( z[t] );
}

void ROTATION_SENSOR::Write_To_Python(int evalPeriod) {

        char outString[1000000];

        sprintf(outString,"%d %d ",ID,3);

        for ( int  t = 0 ; t < evalPeriod ; t++ ) {
                sprintf(outString,"%s %f %f %f ",outString,x[t],y[t],z[t]);
        }

        sprintf(outString,"%s \n",outString);

        std::cout << outString;
}

#endif
