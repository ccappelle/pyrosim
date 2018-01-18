#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#include <string>
#include <ode/ode.h>

#include "actuator/actuator.h"
#include "object.h"
#include "neuralNetwork.h"
#include "datastruct.h"

class ENVIRONMENT {

private:

	int numberOfBodies;

	int numberOfActuators;

	OBJECT ** objects;

	ACTUATOR ** actuators;

	NEURAL_NETWORK * neuralNetwork;

public:
	ENVIRONMENT(void);

	~ENVIRONMENT(void);

	void Actuate_Actuators(void);

	void Draw(int debug=0);

	void Get_Object_Position(float *xyz, int bodyID);

	void Poll_Sensors(int timeStep);

    void Read_From_Python(dWorldID world, dSpaceID space, Data *data);

	void Update_Neural_Network(int timeStep);

	void Update_Forces(int timeStep);

	void Write_Sensor_Data(int evalPeriod);

private:
    void Add_Motor_Neuron(int ID, int jointID, double tau, double alpha, double start);

	void Add_Sensor_Neuron(int ID, int sensorID, int sensorValueIndex);

	void Connect_Sensor_To_Sensor_Neuron( int sensorID , NEURON *sensorNeuron );

	void Create_Bias_Neuron( void );

	void Create_Function_Neuron(int evalPeriod);

	void Create_Hidden_Neuron( void );

	void Create_IsSeen_Sensor(int evalPeriod);

	void Create_Actuator(dWorldID world, dSpaceID space, int index, std::string actuatorTypeString);

	void Create_Light_Sensor(int evalPeriod);

	void Create_Light_Source(void);

    void Create_Motor_Neuron(void);

	void Create_Neural_Network(void);

	void Create_Object(dWorldID world, dSpaceID space, int index, int objType);

	void Make_Object_Susceptible_To_Adhesion_Kind(void);

	void Create_Ray_Sensor(dSpaceID space, int evalPeriod);

	void Create_Position_Sensor(int evalPeriod);

	void Create_Proprioceptive_Sensor(int evalPeriod);

	void Create_Sensor_Neuron(void);

	void Create_Synapse(void);

	void Create_Touch_Sensor(int evalPeriod);

	void Create_Vestibular_Sensor(int evalPeriod);

	void Update_Sensor_Neurons(int timeStep);
};

#endif
