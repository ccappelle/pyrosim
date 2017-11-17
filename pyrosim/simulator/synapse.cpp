#ifndef _SYNAPSE_CPP
#define _SYNAPSE_CPP

#include "iostream"

#include "synapse.h"

SYNAPSE::SYNAPSE(void) {
    sourceNeuronIndex = 0;
    targetNeuronIndex = 0;
    startWeight = 0.;
    endWeight = 0.;
    startTime = 0;
    endTime = 0;

    //Read_From_Python();
}

SYNAPSE::~SYNAPSE(void) {

}

void SYNAPSE::Read_From_Python(void){
    std::cin >> sourceNeuronIndex; 
    std::cin >> targetNeuronIndex;
    std::cin >> startWeight;
    std::cin >> endWeight;
    std::cin >> startTime;
    std::cin >> endTime;
    weight = startWeight;
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
    std::cerr << startWeight << " ";
    std::cerr << endWeight << " ";
    std::cerr << startTime << " ";
    std::cerr << endTime << "\n";
}

void SYNAPSE::Update_Weight(int time){
	if (time < startTime)
		weight = startWeight;
	else if (time>endTime)
		weight = endWeight;
	else if (startTime == endTime)
		weight = endWeight;
	else
	{
		double startRatio = 1.-(time-startTime)/double(endTime-startTime);
		double endRatio = 1.-(endTime-time)/double(endTime-startTime);
		weight = startRatio*startWeight + endRatio*endWeight;
	}
}
#endif
