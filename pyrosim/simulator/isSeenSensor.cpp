#ifndef _IS_SEEN_SENSOR_CPP
#define _IS_SEEN_SENSOR_CPP

#include "iostream"
#include "isSeenSensor.h"
#include "neuron.h"

IS_SEEN_SENSOR::IS_SEEN_SENSOR(int myID, int evalPeriod){
    ID = myID;
    values = new int[evalPeriod];

    for (int t=0; t<evalPeriod; t++){
        values[t] = 0;
    }

    mySensorNeuron = NULL;
}

IS_SEEN_SENSOR::~IS_SEEN_SENSOR(void){}

void IS_SEEN_SENSOR::Connect_To_Sensor_Neuron(NEURON *sensorNeuron){
    mySensorNeuron = sensorNeuron;
}

void IS_SEEN_SENSOR::Fires(int t) {

    values[t] = 1;
}

int  IS_SEEN_SENSOR::Get_ID(void) {

        return ID;
}

void IS_SEEN_SENSOR::Update_Sensor_Neurons(int t) {
        if ( mySensorNeuron )
                mySensorNeuron->Set( values[t] );
}

void IS_SEEN_SENSOR::Write_To_Python(int evalPeriod) {

        char outString[100000];
        sprintf(outString,"%d %d ",ID,1);

        for ( int t=0; t < evalPeriod; t++ ){
            sprintf(outString,"%s %d ",outString,values[t]);
        }

        sprintf(outString,"%s \n",outString);
        std::cout << outString;
}

#endif