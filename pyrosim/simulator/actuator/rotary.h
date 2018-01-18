#ifndef _ACTUATOR_ROTARY_H
#define _ACTUATOR_ROTARY_H

#include "actuator.h"
#include "../object.h"
#include "../proprioceptiveSensor.h"

class ROTARY_ACTUATOR : public ACTUATOR {

protected:
	int	firstObject;
	int	secondObject;

	double x;
	double y;
	double z;
	double normalX;
	double normalY;
	double normalZ;
	double lowStop;
	double highStop;
	int	positionControl;

	double speed;
	double strength;

	dJointID joint;

	PROPRIOCEPTIVE_SENSOR *proprioceptiveSensor;

	OBJECT *first;
	OBJECT *second;

public:
	ROTARY_ACTUATOR(void) :
		firstObject(-1), secondObject(-1),
		x(0), y(0), z(0),
		normalX(0), normalY(0), normalZ(0),
		lowStop(0), highStop(0),
		speed(0.0), strength(0.0),
		joint(NULL),
		positionControl(true),
		proprioceptiveSensor(NULL),
		first(NULL),
		second(NULL) {};

	// Virtual functiontions are declared that way for making very similar derivatives like LINEAR_ACTUATOR

	virtual void Actuate(void);
	bool Connect_Sensor_To_Sensor_Neuron(int sensorID, NEURON *sensorNeuron);

	virtual void Create_In_Simulator(dWorldID world, OBJECT** allObjects, int numObjects, ACTUATOR** allActuators, int numActuators);
	bool Create_Proprioceptive_Sensor(int myID, int evalPeriod);

	virtual void Draw(void) const;

	virtual void Poll_Sensors(int t);
	void Update_Sensor_Neurons(int t);

	virtual void Read_From_Python(void);
	void Write_To_Python(int evalPeriod) const;

};

#endif // _ACTUATOR_ROTARY_H
