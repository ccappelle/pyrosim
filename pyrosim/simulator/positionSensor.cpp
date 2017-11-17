#ifndef _POSITION_SENSOR_CPP
#define _POSITION_SENSOR_CPP

#include "iostream"
#include "positionSensor.h"
#include "neuron.h"

POSITION_SENSOR::POSITION_SENSOR(int myID, int evalPeriod) {

	ID = myID;

	x = new double[evalPeriod];

	y = new double[evalPeriod];

	z = new double[evalPeriod];

	for ( int i = 0 ; i < 3 ; i++)

        	mySensorNeurons[i] = NULL;
}

POSITION_SENSOR::~POSITION_SENSOR(void) {

}

void POSITION_SENSOR::Connect_To_Sensor_Neuron(NEURON *sensorNeuron) {

	mySensorNeurons[ sensorNeuron->Get_Sensor_Value_Index() ] = sensorNeuron;
}

int  POSITION_SENSOR::Get_ID(void) {

        return ID;
}

void POSITION_SENSOR::Poll(dBodyID body, int t) {

        const dReal *pos;

        pos = dBodyGetPosition(body);


	x[t] = pos[0];

	y[t] = pos[1];

	z[t] = pos[2];
}

void POSITION_SENSOR::Update_Sensor_Neurons(int t) {

        if ( mySensorNeurons[0] )

                mySensorNeurons[0]->Set( x[t] );

        if ( mySensorNeurons[1] )

                mySensorNeurons[1]->Set( y[t] );

        if ( mySensorNeurons[2] )

                mySensorNeurons[2]->Set( z[t] );
}

void POSITION_SENSOR::Write_To_Python(int evalPeriod) {

        char outString[1000000];

        sprintf(outString,"%d %d ",ID,3);

        for ( int  t = 0 ; t < evalPeriod ; t++ ) {
                sprintf(outString,"%s %f %f %f ",outString,x[t],y[t],z[t]);
        }

        sprintf(outString,"%s \n",outString);

        std::cout << outString;
        
}

#endif
