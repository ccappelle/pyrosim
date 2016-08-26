#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#include <ode/ode.h>
#include "joint.h"
#include "object.h"
#include "neuralNetwork.h"

class ENVIRONMENT {

private:

	int numberOfBodies;

	int numberOfJoints;

	OBJECT **objects;

	JOINT **joints;

	NEURAL_NETWORK *neuralNetwork;

public:
	ENVIRONMENT(void);

	~ENVIRONMENT(void);

	void Actuate_Joints(void);

	void Draw(void);

        void Poll_Sensors(int t);

	void Read_From_Python(dWorldID world,dSpaceID space, int *evaluationTime);

	void Update_Neural_Network(int t);

	void Write_Sensor_Data(int evalPeriod);

private:
        void Add_Motor_Neuron(int ID, int jointID, int layer, double tau);

	void Add_Sensor_Neuron(int ID, int sensorID, int sensorValueIndex, int layer, double tau);

        void Connect_Motor_Neuron_to_Joint( int jointID, NEURON *motorNeuron );

	void Connect_Sensor_To_Sensor_Neuron( int sensorID , NEURON *sensorNeuron );

	void Create_Bias_Neuron( void );

	void Create_Hidden_Neuron( void );

	void Create_Joint( dWorldID world, dSpaceID space, int index);

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

	void Finished_Reading_From_Python(void);

        void Update_Sensor_Neurons(int t);
};

#endif
