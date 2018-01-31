#ifndef _SENSOR_RAY_H
#define _SENSOR_RAY_H

#include <ode/ode.h>
#include "sensor.h"

class OBJECT;
class NEURON;

class RAY_SENSOR : public SENSOR {

private:
	double *distances;

	OBJECT *obj;

	dGeomID ray;

	double x, y, z;

	double r1,r2,r3;

	double *r, *g, *b;

    double maxDistance;

    NEURON *mySensorNeurons[4];

public:
	RAY_SENSOR(dSpaceID space, OBJECT *myObj, int myID, int evalPeriod);

	~RAY_SENSOR(void) {};

	void Add_To_Object(void);

    void Connect_To_Sensor_Neuron(int sensorValueIndex, NEURON *sensorNeuron);

	void Draw(double endX, double endY, double endZ, int t);

	void Initialize(int evalPeriod);

	void Reset(void);

	void Set(double distance, OBJECT *objectThatWasHit, int t);

	void Update_Sensor_Neurons(int t);

	void Write_To_Python(int evalPeriod);
};

#endif // _SENSOR_RAY_H
