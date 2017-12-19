#ifndef _ACTUATOR_ROTARY_H
#define _ACTUATOR_ROTARY_H

#include "actuator.h"
#include "../object.h"
#include "../proprioceptiveSensor.h"

class ROTARY_ACTUATOR : public ACTUATOR {

private:
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
	ROTARY_ACTUATOR(void);
	~ROTARY_ACTUATOR(void);

	void Actuate(void);
	bool Connect_Sensor_To_Sensor_Neuron(int sensorID, NEURON *sensorNeuron);

	void Create_In_Simulator(dWorldID world, OBJECT** allObjects, int numObjects);
	bool Create_Proprioceptive_Sensor(int myID, int evalPeriod);

	void Draw(void) const;

	void Poll_Sensors(int t);
	void Update_Sensor_Neurons(int t);

	void Read_From_Python(void);
	void Write_To_Python(int evalPeriod) const;

};

#endif // _ACTUATOR_ROTARY_H
