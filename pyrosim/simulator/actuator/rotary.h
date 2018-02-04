#ifndef _ACTUATOR_ROTARY_H
#define _ACTUATOR_ROTARY_H

#include "actuator.h"
#include "../object.h"
#include "../sensor/proprioceptiveRotary.h"

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

	PROPRIOCEPTIVE_ROTARY_SENSOR* proprioceptiveSensor;

	OBJECT* first;
	OBJECT* second;

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

	~ROTARY_ACTUATOR(void) {
		if(proprioceptiveSensor)
			delete proprioceptiveSensor; // BEWARE! You have to overload this if you inherit from this class and add data members
	};

	// Functiontions are declared as virtual for making very similar derivatives like LINEAR_ACTUATOR

	virtual void Actuate(void);
	bool Connect_Sensor_To_Sensor_Neuron(int sensorID, int sensorValueIndex, NEURON *sensorNeuron);

	virtual void Create_In_Simulator(dWorldID world, OBJECT** allObjects, int numObjects, ACTUATOR** allActuators, int numActuators);
	virtual bool Create_Proprioceptive_Sensor(int myID, int evalPeriod);

	virtual void Draw(void) const;

	virtual void Poll_Sensors(int t);
	void Update_Sensor_Neurons(int t);

	virtual void Read_From_Python(void);
	void Write_To_Python(int evalPeriod) const;

};

#endif // _ACTUATOR_ROTARY_H
