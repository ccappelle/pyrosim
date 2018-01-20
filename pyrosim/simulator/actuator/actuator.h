#ifndef _ACTUATOR_ACTUATOR_H
#define _ACTUATOR_ACTUATOR_H

//#include "../datastruct.h"
//#include "../environment.h"
#include "../neuron.h"
#include "../object.h"

class ACTUATOR {

protected:
	/********* WARNING: DO NOT REDECLARE OR REINITIALIZE THOSE VARIABLES IN DERIVATIVE CLASSES *********/
	int ID;
	NEURON* motorNeuron;

public:
	ACTUATOR(void) : ID(-1), motorNeuron(NULL) {};

	virtual void Read_From_Python(void) = 0;
	//! Reads the actuator data supplied by the Python module through standard input
	virtual void Write_To_Python(int evalPeriod) const {};
	//! Supplies the data from sensors embedded into the actuator to Python through standard output

//	virtual void Create_In_Simulator(dWorldID world, ENVIRONMENT* environment, Data* data) = 0;
	virtual void Create_In_Simulator(dWorldID world, OBJECT** allObjects, int numObjects, ACTUATOR** allActuators, int numActuators) = 0;
	//! Creates the actuator in simulation
	virtual void Actuate(void) = 0;
	//! Actuates the actuator in simulation
	virtual void Draw(void) const = 0;
	//! Draws the actuator. Notice the const!

	virtual bool Connect_Sensor_To_Sensor_Neuron(int sensorID, int sensorValueIndex, NEURON *sNeuron) {return false;};
	//! Supplies the pointer to a sensor neuron to the sensor with id sensorID
	virtual void Connect_To_Motor_Neuron(int actuatorInputIndex, NEURON *mNeuron)
	{
		// actuatorInputIndex is for use in actuators with multiple inputs; by default it is ignored
		motorNeuron = mNeuron;
	};
	//! Stores the pointer to a motor neuron for future use

	virtual void Poll_Sensors(int currentTimestep) {};
	//! Polls the embedded sensors
	virtual void Update_Sensor_Neurons(int t) {};
	//! Updates the sensor neurons
	virtual bool Create_Proprioceptive_Sensor(int sensorID, int evalPeriod) {return false;};
	//! Attempts to create a proprioceptive sensor with sensorID that is evaluated once every evalPeriod iterations
};

#endif // _ACTUATOR_ACTUATOR_H
