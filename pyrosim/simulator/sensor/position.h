#ifndef _SENSOR_POSITION_H
#define _SENSOR_POSITION_H

#include <ode/ode.h>
#include "sensor.h"

class NEURON;

class POSITION_SENSOR : public SENSOR {

private:
	double* x;
	double* y;
	double* z;
	NEURON* mySensorNeurons[3];

public:
	POSITION_SENSOR(int myID, int evalPeriod);

	~POSITION_SENSOR(void) {};

	void Connect_To_Sensor_Neuron(int sensorValueIndex, NEURON* sensorNeuron) { mySensorNeurons[ sensorValueIndex ] = sensorNeuron; };

	void Poll(dBodyID body, int t);

	void Update_Sensor_Neurons(int t);

	void Write_To_Python(int evalPeriod);
};

#endif // _SENSOR_POSITION_H
