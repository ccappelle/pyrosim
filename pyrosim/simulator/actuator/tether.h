#ifndef _ACTUATOR_TETHER_H
#define _ACTUATOR_TETHER_H

#include <cassert>

#include "../object.h"
#include "actuator.h"

class TETHER : public ACTUATOR {

private:
	int	firstObject;
    int secondObject;

	// double pos1[3], pos2[3];
	const double* pos1;
	const double* pos2;

	double currentLength, previousLength;
	double forceConstant;
	double dampeningCoefficient;

	double currentTension;

	OBJECT* first;
    OBJECT* second;

	NEURON* firstMotorNeuron;
	NEURON* secondMotorNeuron;

public:
	TETHER(void) : forceConstant(1.0),
	               dampeningCoefficient(10.),
	               first(NULL),
	               second(NULL) {};

	// Since we're using multiple motor neurons, we will have to redefine all methods in actuator.h

	void Read_From_Python(void);
	void Write_To_Python(int evalPeriod) const {};

	void Create_In_Simulator(dWorldID world, OBJECT** allObjects, int numObjects, ACTUATOR** allActuators, int numActuators);
	void Actuate(void);
	void Draw(void) const;

	bool Connect_Sensor_To_Sensor_Neuron(int sensorID, int sensorValueIndex, NEURON *sNeuron) {return false;};
	void Connect_To_Motor_Neuron(int actuatorInputIndex, NEURON *mNeuron);

	void Poll_Sensors(int currentTimestep) {};
	void Update_Sensor_Neurons(int t) {};
	bool Create_Proprioceptive_Sensor(int sensorID, int evalPeriod) {return false;};

private:
	inline double Get_Current_Length(void) {
		return sqrt( (pos1[0]-pos2[0])*(pos1[0]-pos2[0]) + (pos1[1]-pos2[1])*(pos1[1]-pos2[1]) + (pos1[2]-pos2[2])*(pos1[2]-pos2[2]) );
	};

	void Update_Geometry(void);
};

#endif // _ACTUATOR_TETHER_H
