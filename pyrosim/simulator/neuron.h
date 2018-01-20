#ifndef _NEURON_H
#define _NEURON_H

#include <string>
#include <map>

#include "datastruct.h"

const int SENSOR_NEURON   = 0;
const int BIAS_NEURON     = 1;
const int HIDDEN_NEURON   = 2;
const int MOTOR_NEURON    = 3;
const int FUNCTION_NEURON = 4;

typedef std::map<std::string, int> StringToNeuronTypeMapType;
const StringToNeuronTypeMapType stringToNeuronTypeMap = {
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

	double* timeValues;

public:
	NEURON() : value(0.),
	           previousValue(0.),
	           tau(1.0),
	           alpha(1.0),
	           timeValues(NULL) {}; // NOTE: timeValues(NULL) was commented in Josh's initialization for some reason (A.B.)
	~NEURON(void) {};

	void Read_From_Python(std::string neuronTypeStr, Data* data);
	void Print(void);
	void Push_Current_Value_To_Previous_Value(void);
	void Reset(int timeStep);
	void Set( double v );
	void Threshold(void);

	int Get_ID(void) {return ID;};
	double Get_Previous_Value(void) {return previousValue;};
	double Get_Tau(void) {return tau;};
	double Get_Value(void) {return value;};
	int Get_Type(void) {return type;};
};

#endif // _NEURON_H
