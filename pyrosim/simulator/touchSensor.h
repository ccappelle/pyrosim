#ifndef _TOUCH_SENSOR_H
#define _TOUCH_SENSOR_H

#include <ode/ode.h>

class NEURON;

class TOUCH_SENSOR {

private:

	int    ID;
	int *values;
    NEURON *mySensorNeuron;

public:
	TOUCH_SENSOR(int myID, int evalPeriod);

	~TOUCH_SENSOR(void);

        void Connect_To_Sensor_Neuron(NEURON *sensorNeuron);

	void Fires(int t);

        int  Get_ID(void);

        void Update_Sensor_Neurons(int t);

	void Write_To_Python(int evalPeriod);
};

#endif
