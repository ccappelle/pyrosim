#ifndef _SENSOR_PROXIMITY_H
#define _SENSOR_PROXIMITY_H

#include <ode/ode.h>
#include "sensor.h"

class OBJECT;

class PROXIMITY_SENSOR : public SENSOR {

private:
	// object to which the sensor is connected
	OBJECT* basisObject;

	// offset of the sensor - not currently used
	double ox, oy, oz;

	// spherical coordinates of the closest point
	double* r;
	double* t;
	double* p;

	// color of the closest point
	double* cr;
	double* cg;
	double* cb;

	// volume monitored by the sensor and its params
	double sensorRadius;
	dGeomID sensorVolume;

public:
	PROXIMITY_SENSOR(dSpaceID space, OBJECT *myObj, int myID, int evalPeriod);
	~PROXIMITY_SENSOR();

	void Draw(double endX, double endY, double endZ, int tt);
	void Set(double distance, OBJECT* objectThatWasHit, int tt);
};

#endif // _SENSOR_PROXIMITY_H
