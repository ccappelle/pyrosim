#ifndef _LIGHT_SENSOR_H
#define _LIGHT_SENSOR_H

#include <ode/ode.h>

class NEURON;

class LIGHT_SENSOR {

private:

	int ID;

	double *values; 

	NEURON *mySensorNeuron;

public:
	LIGHT_SENSOR(int myID, int evalPeriod);

	~LIGHT_SENSOR(void);

	void Connect_To_Sensor_Neuron(NEURON *sensorNeuron);

	int  Get_ID(void);

	void Poll(dBodyID body, dBodyID lightSource, int t);

	void Update_Sensor_Neurons(int t);

	void Write_To_Python(int evalPeriod);
};

#endif
