#ifndef _PROPRIOCEPTIVE_SENSOR_CPP
#define _PROPRIOCEPTIVE_SENSOR_CPP

#include "iostream"

#include "proprioceptiveSensor.h"

#include "neuron.h"

extern int HINGE;
extern int SLIDER;
extern int THRUSTER;

PROPRIOCEPTIVE_SENSOR::PROPRIOCEPTIVE_SENSOR(int myID, int evalPeriod) {

	ID = myID;

	angles = new double[evalPeriod];

        mySensorNeuron = NULL;
}

PROPRIOCEPTIVE_SENSOR::~PROPRIOCEPTIVE_SENSOR(void) {

}

void PROPRIOCEPTIVE_SENSOR::Connect_To_Sensor_Neuron(NEURON *sensorNeuron) {

        mySensorNeuron = sensorNeuron;
}

int  PROPRIOCEPTIVE_SENSOR::Get_ID(void) {

        return ID;
}

void PROPRIOCEPTIVE_SENSOR::Poll(dJointID joint, int type, int t) {

        const dReal *pos;

        if(type==HINGE)
                angles[t] = dJointGetHingeAngle(joint);
        else if(type==SLIDER)
                angles[t] = dJointGetSliderPosition(joint);

}

void PROPRIOCEPTIVE_SENSOR::Update_Sensor_Neurons(int t) {

        if ( mySensorNeuron )
                mySensorNeuron->Set( angles[t] );
}

void PROPRIOCEPTIVE_SENSOR::Write_To_Python(int evalPeriod) {

        char outString[1000000];

        sprintf(outString,"%d %d ",ID,1);

        for ( int  t = 0 ; t < evalPeriod ; t++ ) 

                sprintf(outString,"%s %f ",outString,angles[t]);

        sprintf(outString,"%s \n",outString);

        std::cout << outString;
}

#endif
