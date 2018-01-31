#ifndef _SENSOR_SENSOR_H
#define _SENSOR_SENSOR_H

#include "../neuron.h"

class SENSOR {

protected:
	int ID;
	double* values;
	NEURON* mySensorNeuron;

public:
	SENSOR(void) : ID(-1),
	               values(NULL),
	               mySensorNeuron(NULL) {};

	virtual void Connect_To_Sensor_Neuron(int sensorValueIndex, NEURON* sensorNeuron) {mySensorNeuron = sensorNeuron;};
	virtual int Get_ID(void) {return ID;};

	virtual void Write_To_Python(int evalPeriod);
};

#endif // _SENSOR_SENSOR_H
