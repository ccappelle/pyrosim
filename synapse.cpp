#ifndef _SYNAPSE_CPP
#define _SYNAPSE_CPP

#include "iostream"

#include "synapse.h"

SYNAPSE::SYNAPSE(void) {

        std::cin >> sourceNeuronIndex; 

        std::cin >> targetNeuronIndex;

	std::cin >> weight;
}

SYNAPSE::~SYNAPSE(void) {

}

int SYNAPSE::Get_Source_Neuron_Index(void) {

	return sourceNeuronIndex;
}

int SYNAPSE::Get_Target_Neuron_Index(void) {

        return targetNeuronIndex;
}

double SYNAPSE::Get_Weight(void) {

        return weight; 
}

void SYNAPSE::Print(void) {

	std::cerr << sourceNeuronIndex << " ";

        std::cerr << targetNeuronIndex << " ";

        std::cerr << weight << "\n";
}

#endif
