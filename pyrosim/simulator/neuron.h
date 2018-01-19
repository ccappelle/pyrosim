#ifndef _NEURON_H
#define _NEURON_H

#include <string>
#include <map>

const int SENSOR_NEURON   = 0;
const int BIAS_NEURON     = 1;
const int HIDDEN_NEURON   = 2;
const int MOTOR_NEURON    = 3;
const int FUNCTION_NEURON = 4;

const std::map<std::string, int> stringToNeuronTypeMap = {
	{"BiasNeuron", BIAS_NEURON},
	{"SensorNeuron", SENSOR_NEURON},
	{"HiddenNeuron", HIDDEN_NEURON},
	{"MotorNeuron", MOTOR_NEURON},
	{"FunctionNeuron", FUNCTION_NEURON}
};

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

	NEURON(int myID, int neuronType, int valueIndex, double tau, double a);

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

#endif // _NEURON_H
