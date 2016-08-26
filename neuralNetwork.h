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

	int    numLayers;

	int	*firstNeuronInLayer;

	int	*lastNeuronInLayer;

	int	*firstIncomingSynapseToNeuron;

	int	*lastIncomingSynapseToNeuron;

public:
	NEURAL_NETWORK(void);

	~NEURAL_NETWORK(void);

	void   Add_Bias_Neuron(int ID, int layer);

	void   Add_Hidden_Neuron(int ID, int layer, double tau);

	NEURON *Add_Motor_Neuron(int ID, int layer, double tau);

	NEURON *Add_Sensor_Neuron(int ID, int svIndex, int layer, double tau);

	void   Add_Synapse(void);

	void Finished_Reading_From_Python(void);

	void Update(void);

private:
	void Assign_Synapses_To_Neurons(void);

	void Initialize_Neurons(void);

	void Initialize_Synapses(void);

	void Organize_Neurons_Into_Layers(void);

	void Reset_Neuron_Values(void);

	void Update_Layer(int l);

	void Update_Neuron_On_Layer(int tn, int l);
};

#endif
