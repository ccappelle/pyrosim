#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#include <ode/ode.h>
#include "joint.h"
#include "object.h"
#include "neuralNetwork.h"
#include "datastruct.h"

class ENVIRONMENT {
 
private:

	int numberOfBodies;

	int numberOfJoints;
	int type; 
	
	OBJECT **objects;

	JOINT **joints;

	NEURAL_NETWORK *neuralNetwork;

public:
	ENVIRONMENT(void);

	~ENVIRONMENT(void);

	void Actuate_Joints(void);

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

    void Connect_Motor_Neuron_to_Joint( int jointID, NEURON *motorNeuron );

	void Connect_Sensor_To_Sensor_Neuron( int sensorID , NEURON *sensorNeuron );

	void Create_Bias_Neuron( void );

	void Create_Function_Neuron(int evalPeriod);
	
	void Create_Hidden_Neuron( void );

	void Create_IsSeen_Sensor(int evalPeriod);

	void Create_Joint( dWorldID world, dSpaceID space, int index, int jointType);

	void Create_Light_Sensor(int evalPeriod);

	void Create_Light_Source(void);

    void Create_Motor_Neuron(void);

	void Create_Neural_Network(void);

	void Create_Object(dWorldID world, dSpaceID space, int index, int objType);

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
