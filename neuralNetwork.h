#ifndef _NEURAL_NETWORK_H
#define _NEURAL_NETWORK_H

#include "neuron.h"

#include "synapse.h"

class NEURAL_NETWORK {

private:
	NEURON **neurons;

	int    numNeurons;

        SYNAPSE **synapses;

        int    numSynapses;

public:
	NEURAL_NETWORK(void);

	~NEURAL_NETWORK(void);

	void   Add_Bias_Neuron(int ID);

	void   Add_Hidden_Neuron(int ID, double tau);

	NEURON *Add_Motor_Neuron(int ID, double tau);

	NEURON *Add_Sensor_Neuron(int ID, int svIndex, double tau);

	void   Add_Synapse(void);

	void Update(void);

private:
	void Initialize_Neurons(void);

	void Initialize_Synapses(void);

	void Push_Current_Values_To_Previous_Values(void);

	void Reset_Neuron_Values(void);

	void Threshold_Neurons(void);

	void Update_Neurons(void);
};

#endif
