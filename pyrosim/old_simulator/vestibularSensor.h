#ifndef _VESTIBULAR_SENSOR_H
#define _VESTIBULAR_SENSOR_H

#include <ode/ode.h>

class NEURON;

class VESTIBULAR_SENSOR {

private:

	int ID;

	double *angles;

        NEURON *mySensorNeuron;

public:
	VESTIBULAR_SENSOR(int myID, int evalPeriod);

	~VESTIBULAR_SENSOR(void);

        void Connect_To_Sensor_Neuron(NEURON *sensorNeuron);

        int  Get_ID(void);

	void Poll(dBodyID body, int t);

        void Update_Sensor_Neurons(int t);

	void Write_To_Python(int evalPeriod);
};

#endif
