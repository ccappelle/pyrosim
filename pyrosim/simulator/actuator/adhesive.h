#ifndef _ACTUATOR_ADHESIVE_H
#define _ACTUATOR_ADHESIVE_H

#include "../object.h"
#include "actuator.h"
#include "../sensor/proprioceptiveAdhesive.h"

class ADHESIVE : public ACTUATOR {

private:
	int	firstObject;

	OBJECT *first;

	int adhesionKind;

	bool enabledNow;

	PROPRIOCEPTIVE_ADHESIVE_SENSOR* proprioceptiveSensor;

public:
	ADHESIVE(void) : first(NULL), adhesionKind(0), enabledNow(false), proprioceptiveSensor(NULL) {};

	~ADHESIVE(void) {

		if(proprioceptiveSensor)
			delete proprioceptiveSensor;
	};

	void Actuate(void);

	void Create_In_Simulator(dWorldID world, OBJECT ** allObjects, int numObjects, ACTUATOR** allActuators, int numActuators);

	void Draw(void) const {}; // drawing is done by objects automatically

	void Read_From_Python(void);

	bool Create_Proprioceptive_Sensor(int sensorID, int evalPeriod);
	void Poll_Sensors(int currentTimestep);
	void Update_Sensor_Neurons(int t);
	void Write_To_Python(int evalPeriod) const;
	bool Connect_Sensor_To_Sensor_Neuron(int sensorID, int sensorValueIndex, NEURON *sNeuron);
};

#endif // _ACTUATOR_ADHESIVE_H
