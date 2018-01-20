#ifndef _NEURAL_NETWORK_CPP
#define _NEURAL_NETWORK_CPP

#include <iostream>
#include <cmath>
#include <cstdlib>

#include "neuralNetwork.h"

#include "constants.h"

extern const int SENSOR_NEURON;
extern const int BIAS_NEURON;
extern const int HIDDEN_NEURON;
extern const int MOTOR_NEURON;
extern const int FUNCTION_NEURON;

bool NEURAL_NETWORK::Is_A_Neuron_Type(std::string typeName) {
	return stringToNeuronTypeMap.find(typeName) != stringToNeuronTypeMap.end();
}

bool NEURAL_NETWORK::Is_A_Synapse_Type(std::string typeName) {
	return typeName.compare("Synapse") == 0;
}

void NEURAL_NETWORK::Read_Neuron_From_Python(std::string neuronTypeStr, ENVIRONMENT* env, Data* data) {

	neurons[numNeurons] = new NEURON();
	neurons[numNeurons]->Read_From_Python(neuronTypeStr, data);

	// Certain types of neurons get special treatment

	if(neuronTypeStr.compare("MotorNeuron") == 0) {

		int actuatorInputIndex, actuatorID;
		std::cin >> actuatorInputIndex;
		std::cin >> actuatorID;

		ACTUATOR* act = env->Get_Actuator(actuatorID);
		act->Connect_To_Motor_Neuron(actuatorInputIndex, neurons[numNeurons]);
	}

	if(neuronTypeStr.compare("SensorNeuron") == 0) {

		int sensorValueIndex, sensorID;
		std::cin >> sensorValueIndex;
		std::cin >> sensorID;

		// We attempt to connect the sensor neuron to every possible object and actuator
		// FIXME: replace once a global list of sensors is added to the environment
		bool done = false;
		for(int i=0; (!done) && i<env->Get_Number_Of_Objects(); i++) {
			if(env->Get_Object(i)->Connect_Sensor_To_Sensor_Neuron(sensorID, sensorValueIndex, neurons[numNeurons]))
				done = true;
		}
		for(int i=0; (!done) && i<env->Get_Number_Of_Actuators(); i++) {
			if(env->Get_Actuator(i)->Connect_Sensor_To_Sensor_Neuron(sensorID, sensorValueIndex, neurons[numNeurons]))
				done = true;
		}
	}

	numNeurons++;
}

void NEURAL_NETWORK::Read_Synapse_From_Python(std::string synapseTypeStr, ENVIRONMENT* environment, Data* data) {

    synapses[numSynapses] = new SYNAPSE();
    synapses[numSynapses]->Read_From_Python();
    numSynapses++;
}

void NEURAL_NETWORK::Update(int timeStep) {

	Push_Current_Values_To_Previous_Values();

	Reset_Neuron_Values(timeStep);
	Update_Synapses(timeStep);

	Update_Neurons();
	Threshold_Neurons();
}

// ------------------------- Private methods -----------------------------

void NEURAL_NETWORK::Push_Current_Values_To_Previous_Values(void) {

        for (int n = 0 ; n < numNeurons ; n++ )

		neurons[n]->Push_Current_Value_To_Previous_Value();
}

void NEURAL_NETWORK::Reset_Neuron_Values(int timeStep) {

	for ( int n = 0 ; n < numNeurons ; n++ )

		neurons[n]->Reset(timeStep);
}

void NEURAL_NETWORK::Threshold_Neurons(void) {

        for ( int n = 0 ; n < numNeurons ; n++ )

		neurons[n]->Threshold();
}

void NEURAL_NETWORK::Update_Synapses(int timeStep){
	for (int s=0; s<numSynapses; s++){
		synapses[s]->Update_Weight(timeStep);
	}
}

void NEURAL_NETWORK::Update_Neurons(void) {

	for ( int s = 0 ; s < numSynapses ; s++ ) {

		int sni = synapses[s]->Get_Source_Neuron_Index();
		int tni = synapses[s]->Get_Target_Neuron_Index();
		double w = synapses[s]->Get_Weight();
		double influence = w * neurons[sni]->Get_Previous_Value();
		neurons[tni]->Set( neurons[tni]->Get_Value() + influence );
	}

}

#endif // _NEURAL_NETWORK_CPP
