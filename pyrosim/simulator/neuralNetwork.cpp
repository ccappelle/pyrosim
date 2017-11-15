#ifndef _NEURAL_NETWORK_CPP
#define _NEURAL_NETWORK_CPP

#include "iostream"
#include "neuralNetwork.h"
#include "math.h"


NEURAL_NETWORK::NEURAL_NETWORK(int nn, int s) {
	Initialize_Neurons(nn);
	Initialize_Synapses(s);
}

NEURAL_NETWORK::~NEURAL_NETWORK(void) {

}

void NEURAL_NETWORK::Add_Bias_Neuron(int ID) {
    neurons[numNeurons] = new NEURON(ID,BIAS_NEURON, 1.0, 1.0);
    numNeurons++;
}

void NEURAL_NETWORK::Add_Function_Neuron(int ID, double *timeValues){
	neurons[numNeurons] = new NEURON(ID, timeValues);
	numNeurons++;
}

void NEURAL_NETWORK::Add_Hidden_Neuron(int ID, double tau, double alpha) {
	neurons[numNeurons] = new NEURON(ID,HIDDEN_NEURON,tau, alpha);
	numNeurons++;
}

NEURON *NEURAL_NETWORK::Add_Motor_Neuron(int ID, double tau, double alpha, double start) {
    
    NEURON *newNeuron = new NEURON(ID,MOTOR_NEURON,tau, alpha);
    newNeuron->Set(start);
    neurons[numNeurons] = newNeuron;

	numNeurons++;
    return newNeuron;
}

NEURON *NEURAL_NETWORK::Add_Sensor_Neuron(int ID, int svIndex) {

	NEURON *newNeuron = new NEURON(ID,SENSOR_NEURON,svIndex,1.0,1.0);
    neurons.push_back(newNeuron);
	numNeurons++;

	return newNeuron;
}

void NEURAL_NETWORK::Add_Synapse(void) {
    SYNAPSE *s = new SYNAPSE();
    s->Read_From_Python();
    synapses.push_back(s);
    numSynapses ++; 
}

void NEURAL_NETWORK::Update(int timeStep) {

	Push_Current_Values_To_Previous_Values();

	Reset_Neuron_Values(timeStep);
	Update_Synapses(timeStep);

	Update_Neurons();
	Threshold_Neurons();
}

// ------------------------- Private methods -----------------------------

void NEURAL_NETWORK::Initialize_Neurons(int nn) {
        neurons.reserve(nn);
        numNeurons = 0;
}

void NEURAL_NETWORK::Initialize_Synapses(int s) {
        synapses.reserve(s);
        numSynapses = 0;
}

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

#endif
