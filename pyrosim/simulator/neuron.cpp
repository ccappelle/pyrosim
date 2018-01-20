#ifndef _NEURON_CPP
#define _NEURON_CPP

#include <iostream>
#include <cmath>

#include "neuron.h"

extern const int BIAS_NEURON;
extern const int FUNCTION_NEURON;
extern const int SENSOR_NEURON;

void NEURON::Read_From_Python(std::string neuronTypeStr, Data* data) {

	type = stringToNeuronTypeMap.at(neuronTypeStr); // WARNING: C++11-only idiom

	std::cin >> ID;

	if(type == FUNCTION_NEURON) {
		timeValues = new double[data->evaluationTime];
		for(int i=0; i<data->evaluationTime; i++) {
			std::cin >> timeValues[i];
		}
		return;
	}

	if(type == SENSOR_NEURON)
		return;

	if(type == BIAS_NEURON) {
		value = 1.0;
		return;
	}

	std::cin >> tau;
	std::cin >> alpha;

	if(type == HIDDEN_NEURON)
		return;

	if(type == MOTOR_NEURON) {
		double start;
		std::cin >> start;
		Set(start);
		return;
	}

	std::cerr << "Unrecognized neuron type index " << type << std::endl;
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

#endif // _NEURON_CPP
