#ifndef _JOINT_H
#define _JOINT_H

#include "object.h"
#include "proprioceptiveSensor.h"
#include "neuron.h"

class JOINT {

private:
	int	ID;
	int	firstObject;
	int	secondObject;
	int type; 
	
	double	x;
	double	y;
	double	z;
	double	normalX;
	double	normalY;
	double	normalZ;
	double  lowStop;
	double  highStop;
	int	positionControl;

	double  speed;
	double torque;

	dJointID joint;

        PROPRIOCEPTIVE_SENSOR *proprioceptiveSensor;

	NEURON *motorNeuron;

public:
	JOINT(int jointType);

	~JOINT(void);

	void Actuate(void);

        int  Connect_Sensor_To_Sensor_Neuron(int sensorID , NEURON *sensorNeuron);

        int  Connect_To_Motor_Neuron(int jointID, NEURON *mNeuron);

	void Create_In_Simulator(dWorldID world, OBJECT *firstObject, OBJECT *secondObject);

        void Create_Proprioceptive_Sensor(int myID, int evalPeriod);
        
    void Draw(void);

	int  Get_First_Object_Index(void);

	int  Get_Second_Object_Index(void);

        void Poll_Sensors(int t);

	void Read_From_Python(void);

	void Update_Sensor_Neurons(int t);

	void Write_To_Python(int evalPeriod);

private:
	//void Create_Fixed_Joint_In_Simulator(dWorldID world, OBJECT *firstObject, OBJECT *secondObject);

        void Create_Hinge_Joint_In_Simulator(dWorldID world, OBJECT *firstObject, OBJECT *secondObject);

	//int  Is_Fixed_Joint(OBJECT *firstObject, OBJECT *secondObject);

};

#endif
