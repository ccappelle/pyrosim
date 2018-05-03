#ifndef _SYNAPSE_H
#define _SYNAPSE_H

class SYNAPSE {

private:

	int sourceNeuronIndex;
	int targetNeuronIndex;
	double weight;
	double startWeight;
	double endWeight;
	int startTime;
	int endTime;

public:
        SYNAPSE(void);

	~SYNAPSE(void);
	void Read_From_Python(void);

	int  Get_Source_Neuron_Index(void);

	int  Get_Target_Neuron_Index(void);

	double Get_Weight(void);

	void Print(void);

	void Update_Weight(int time);
};

#endif
