#ifndef _SENSOR_IS_SEEN_CPP
#define _SENSOR_IS_SEEN_CPP

#include <iostream>
#include "isSeen.h"
#include "../neuron.h"

IS_SEEN_SENSOR::IS_SEEN_SENSOR(int myID, int evalPeriod){
    ID = myID;
    values = new double[evalPeriod];

    for (int t=0; t<evalPeriod; t++)
        values[t] = 0;

    mySensorNeuron = NULL;
}

void IS_SEEN_SENSOR::Fires(int t) {
    values[t] = 1.;
}

void IS_SEEN_SENSOR::Update_Sensor_Neurons(int t) {
        if ( mySensorNeuron )
                mySensorNeuron->Set( values[t] );
}

#endif // _SENSOR_IS_SEEN_CPP
