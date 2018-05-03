#ifndef _NEURON_H
#define _NEURON_H

class NEURON {

private:

	int ID;

	int type;

	int sensorValueIndex;

	double value;

	double previousValue;

	double tau;
	double alpha;

	double *timeValues;
	
public:
        NEURON(int myID, int neuronType, double tau, double a);

	NEURON(int myID, int neuronType, int svIndex, double tau, double a);

	NEURON(int myID,  double *timeValues);

	~NEURON(void);

	int Get_ID(void);

	double Get_Previous_Value(void);

	double Get_Tau(void);

	double Get_Value(void);

	int Get_Sensor_Value_Index(void);

	int Get_Type(void);

	void Print(void);

	void Push_Current_Value_To_Previous_Value(void);

	void Reset(int timeStep);

	void Set( double v );

	void Threshold(void);

private:
	void Initialize(int myID, int neuronType, double t, double a);

};

#endif
