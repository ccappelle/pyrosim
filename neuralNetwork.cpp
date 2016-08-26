#ifndef _NEURAL_NETWORK_CPP
#define _NEURAL_NETWORK_CPP

#include "iostream"

#include "neuralNetwork.h"

#include "math.h"

extern int MAX_NEURONS;

extern int MAX_SYNAPSES;

extern int SENSOR_NEURON;

extern int BIAS_NEURON;

extern int HIDDEN_NEURON;

extern int MOTOR_NEURON;

NEURAL_NETWORK::NEURAL_NETWORK(void) {

	numLayers = 1;

	Initialize_Neurons();

	Initialize_Synapses();

        firstNeuronInLayer = NULL;

        lastNeuronInLayer = NULL;

	firstIncomingSynapseToNeuron = NULL;

        lastIncomingSynapseToNeuron = NULL;
}

NEURAL_NETWORK::~NEURAL_NETWORK(void) {

}

void NEURAL_NETWORK::Add_Bias_Neuron(int ID, int layer) {

        neurons[numNeurons] = new NEURON(ID,BIAS_NEURON,layer,1.0);

        if ( ( neurons[numNeurons]->Get_Layer() + 1 ) > numLayers )

                numLayers = neurons[numNeurons]->Get_Layer() + 1;

        numNeurons++;

}

void NEURAL_NETWORK::Add_Hidden_Neuron(int ID, int layer, double tau) {

	neurons[numNeurons] = new NEURON(ID,HIDDEN_NEURON,layer,tau);

	if ( ( neurons[numNeurons]->Get_Layer() + 1 ) > numLayers )

		numLayers = neurons[numNeurons]->Get_Layer() + 1;

	numNeurons++;
}

NEURON *NEURAL_NETWORK::Add_Motor_Neuron(int ID,int layer, double tau) {

        NEURON *newNeuron = new NEURON(ID,MOTOR_NEURON,layer,tau);

        neurons[numNeurons] = newNeuron;

        if ( ( neurons[numNeurons]->Get_Layer() + 1 ) > numLayers )

                numLayers = neurons[numNeurons]->Get_Layer() + 1;

	numNeurons++;

        return newNeuron;
}

NEURON *NEURAL_NETWORK::Add_Sensor_Neuron(int ID, int svIndex,int layer, double tau) {

	NEURON *newNeuron = new NEURON(ID,SENSOR_NEURON,svIndex,layer,tau);

	neurons[numNeurons] = newNeuron;

        if ( ( neurons[numNeurons]->Get_Layer() + 1 ) > numLayers )

                numLayers = neurons[numNeurons]->Get_Layer() + 1;

	numNeurons++;

	return newNeuron;
}

void NEURAL_NETWORK::Add_Synapse(void) {

        synapses[numSynapses++] = new SYNAPSE(); 
}

void NEURAL_NETWORK::Finished_Reading_From_Python(void) {

	Organize_Neurons_Into_Layers();

	Assign_Synapses_To_Neurons();
}

void NEURAL_NETWORK::Update(void) {

	for ( int l = 1 ; l < numLayers ; l++ )

		Update_Layer(l);
}

// ------------------------- Private methods -----------------------------

void NEURAL_NETWORK::Assign_Synapses_To_Neurons(void) {

        firstIncomingSynapseToNeuron = new int[numNeurons];

        lastIncomingSynapseToNeuron = new int[numNeurons];

	for ( int n = 0 ; n < numNeurons ; n++ ) {

		firstIncomingSynapseToNeuron[n] = 1000000;

		lastIncomingSynapseToNeuron[n] = -1000000;
	}

	for ( int s = 0 ; s < numSynapses ; s++ ) {

		int tn = synapses[s]->Get_Target_Neuron_Index();

		if ( firstIncomingSynapseToNeuron[tn] > s )

			firstIncomingSynapseToNeuron[tn] = s;

                if ( lastIncomingSynapseToNeuron[tn] < s )

                        lastIncomingSynapseToNeuron[tn] = s;

	}
}

void NEURAL_NETWORK::Initialize_Neurons(void) {

        neurons = new NEURON * [MAX_NEURONS];

        for (int n = 0 ; n < MAX_NEURONS ; n++ )

                neurons[n] = NULL;

        numNeurons = 0;
}

void NEURAL_NETWORK::Initialize_Synapses(void) {

        synapses = new SYNAPSE * [MAX_SYNAPSES];

        for (int s = 0 ; s < MAX_SYNAPSES ; s++ )

                synapses[s] = NULL;

        numSynapses = 0;
}

void NEURAL_NETWORK::Organize_Neurons_Into_Layers(void) {

        firstNeuronInLayer = new int[numLayers];

        lastNeuronInLayer = new int[numLayers];

	for (int l = 0 ; l < numLayers ; l++ ) {

		firstNeuronInLayer[l] = 1000000;

		lastNeuronInLayer[l] = -1000000;
	}

	for (int n = 0 ; n < numNeurons ; n++ ) {

		int l = neurons[n]->Get_Layer();

		if ( firstNeuronInLayer[l] > n )

			firstNeuronInLayer[l] = n;

                if ( lastNeuronInLayer[l] < n )

                        lastNeuronInLayer[l] = n;
	}
}

void NEURAL_NETWORK::Update_Layer(int l) {

	for ( int tn = firstNeuronInLayer[l] ; tn <= lastNeuronInLayer[l] ; tn++ )

		Update_Neuron_On_Layer(tn,l);
}

void NEURAL_NETWORK::Update_Neuron_On_Layer(int tn, int l) {

	neurons[tn]->Push_Current_Value_To_Previous_Value();

	neurons[tn]->Reset();

	double influence = 0.0;

	for ( int s = firstIncomingSynapseToNeuron[tn] ; s <= lastIncomingSynapseToNeuron[tn] ; s++ ) {

		int sn = synapses[s]->Get_Source_Neuron_Index();

		double w = synapses[s]->Get_Weight();

		if ( neurons[sn]->Get_Layer() == neurons[tn]->Get_Layer() )

			influence = influence + neurons[sn]->Get_Previous_Value() * w;
		else
                        influence = influence + neurons[sn]->Get_Value() * w;

	}

	neurons[tn]->Set( neurons[tn]->Get_Previous_Value() + neurons[tn]->Get_Tau() * influence );
			
/*
			// This synapse is recurrent...

			neurons[tn]->Set( neurons[tn]->Get_Value() + neurons[sn]->Get_Previous_Value() * w );
		else
                        neurons[tn]->Set( neurons[tn]->Get_Value() + neurons[sn]->Get_Value() * w );
*/

	neurons[tn]->Threshold();
}

#endif
