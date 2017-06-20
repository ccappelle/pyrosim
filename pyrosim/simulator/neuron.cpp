#ifndef _NEURON_CPP
#define _NEURON_CPP

#include "iostream"

#include "neuron.h"

#include "math.h"

extern int SENSOR_NEURON;

extern int BIAS_NEURON;

extern int FUNCTION_NEURON;


NEURON::NEURON(int myID, int neuronType, double tau, double a) {

	Initialize(myID,neuronType,tau,a);
	if ( type == BIAS_NEURON)
		value = 1.0;
}

NEURON::NEURON(int myID, int neuronType, int svIndex, double tau, double a) {

	Initialize(myID,neuronType,tau, a);
	sensorValueIndex = svIndex;
}

NEURON::NEURON(int myID, double *tv){
	Initialize(myID, FUNCTION_NEURON, 1.0, 1.0);

	timeValues = tv;
	value = timeValues[0];
}

NEURON::~NEURON(void) {

}

int  NEURON::Get_ID(void) {

	return ID;
}

int  NEURON::Get_Sensor_Value_Index(void) {

	if ( Get_Type() != SENSOR_NEURON ) 

		return 0;

	else
		return sensorValueIndex;
}

int  NEURON::Get_Type(void) {

	return type;
}

double NEURON::Get_Previous_Value(void) {

	return previousValue;
}

double NEURON::Get_Tau(void) {

	return tau;
}

double NEURON::Get_Value(void) {

	return value;
}

void NEURON::Print(void) {

	std::cerr << ID << " ";

        std::cerr << type << " ";

        std::cerr << sensorValueIndex << " ";

        std::cerr << value << "\n";
}

void NEURON::Push_Current_Value_To_Previous_Value(void) {

	previousValue = value;
}

void NEURON::Reset(int timeStep) {

        if ( type == BIAS_NEURON )

                value = 1.0;
        else if ( type == FUNCTION_NEURON)
        {
        	previousValue = value;
        	
        	value = timeValues[timeStep];
        }
	else if( timeStep >0)
		value = 0.0;
}

void NEURON::Set(double v) {
	if (type == BIAS_NEURON || type == FUNCTION_NEURON)
		return;

	value = v;
}

void NEURON::Threshold(void) {

	if ( (Get_Type() == SENSOR_NEURON) || (Get_Type() == BIAS_NEURON) || (Get_Type() == FUNCTION_NEURON))

		return;

	value = alpha * previousValue + tau * value;

	value = tanh(value);
}

// ------------------ Private methods -------------------

void NEURON::Initialize(int myID, int neuronType, double t, double a) {

        ID = myID;

        type = neuronType;

        sensorValueIndex = -1;

	tau = t;
        alpha = a;
        value = 0.0;

	previousValue = 0.0;

	//timeValues = NULL;

}

#endif
