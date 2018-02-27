#ifndef _SENSOR_RAY_H
#define _SENSOR_RAY_H

#include <ode/ode.h>
#include "sensor.h"

class OBJECT;

class RAY_SENSOR : public SENSOR {

private:
	double *distances;
	double *r, *g, *b;

	OBJECT *obj;
	dGeomID ray;

	double x, y, z;
	double r1, r2, r3;

	double maxDistance;

public:
	RAY_SENSOR(dSpaceID space, OBJECT *myObj, int myID, int evalPeriod);

	void Draw(double endX, double endY, double endZ, int t);

	void Set(double distance, OBJECT *objectThatWasHit, int t);

private:
	void Add_To_Object(void);
};

#endif // _SENSOR_RAY_H
