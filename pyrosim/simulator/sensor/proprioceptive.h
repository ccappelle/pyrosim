#ifndef _SENSOR_PROPRIOCEPTIVE_H
#define _SENSOR_PROPRIOCEPTIVE_H

#include <ode/ode.h>
#include "sensor.h"

class NEURON;

class PROPRIOCEPTIVE_SENSOR : public SENSOR {

public:
	PROPRIOCEPTIVE_SENSOR(int myID, int evalPeriod) { ID = myID; values = new double[evalPeriod]; };

//	void Connect_To_Sensor_Neuron(int sensorValueIndex, NEURON* sensorNeuron);

	void Poll(dJointID joint, int type, int t);

	void Update_Sensor_Neurons(int t);

//	void Write_To_Python(int evalPeriod);
};

#endif // _SENSOR_PROPRIOCEPTIVE_H
