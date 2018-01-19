#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H

#include <string>
#include <ode/ode.h>

#include "object.h"
#include "neuralNetwork.h"
#include "datastruct.h"
#include "constants.h"

/***** ACTUATOR SUPPORT DEFINITIONS *****/

// Generic part:

#include "actuator/actuator.h"

// We create a map from strings to a function that returns ACTUATOR*
// More about this design pattern:
// https://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
template<typename actuatorClass> ACTUATOR* createActuatorInstance() { return static_cast<ACTUATOR*>( new actuatorClass ); }
typedef std::map<std::string, ACTUATOR* (*) ()> StringToActuatorMapType;

// Expand the piece of code below whenever you create a new ACTUATOR class:

#include "actuator/rotary.h"
#include "actuator/linear.h"
#include "actuator/thruster.h"
#include "actuator/adhesive.h"
#include "actuator/rope.h"

const StringToActuatorMapType stringToActuatorMap = {
	{"HingeJoint", &createActuatorInstance<ROTARY_ACTUATOR>},
	{"SliderJoint", &createActuatorInstance<LINEAR_ACTUATOR>},
	{"Thruster", &createActuatorInstance<THRUSTER>},
	{"AdhesiveJoint", &createActuatorInstance<ADHESIVE>},
	{"Rope", &createActuatorInstance<ROPE>}
};

/***** END ACTUATOR SUPPORT DEFINITIONS *****/

class ENVIRONMENT {

private:
	int numberOfBodies;

	int numberOfActuators;

	OBJECT* objects[MAX_OBJECTS];

	ACTUATOR* actuators[MAX_ACTUATORS];

	NEURAL_NETWORK* neuralNetwork;

public:
	ENVIRONMENT(void) : numberOfBodies(0),
	                    numberOfActuators(0),
	                    neuralNetwork(new NEURAL_NETWORK) {};

	~ENVIRONMENT(void) {}; // FIXME: destroy the neural network when its destructor is sane

	void Actuate_Actuators(void);

	void Draw(int debug=0);

	void Get_Object_Position(float* xyz, int bodyID);

	void Poll_Sensors(int timeStep);

    void Read_From_Python(dWorldID world, dSpaceID space, Data* data);

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

#endif // _ENVIRONMENT_H
