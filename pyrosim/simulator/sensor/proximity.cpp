#ifndef _SENSOR_PROXIMITY_CPP
#define _SENSOR_PROXIMITY_CPP

#include <iostream>
#include <drawstuff/drawstuff.h>
#include "proximity.h"
#include "object.h"
#include "../geomData.h"

#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#define dsDrawSphere dsDrawSphereD
#endif

PROXIMITY_SENSOR::PROXIMITY_SENSOR(dSpaceID space, OBJECT* myObj, int myID, int evalPeriod) : SENSOR(myID, evalPeriod, 6),
                                                                                              basisObject(myObj) {
	std::cin >> ox;
	std::cin >> oy;
	std::cin >> oz;
	std::cin >> sensorRadius;

	r = values[0]; t = values[1]; p = values[2];
	cr = values[3]; cg = values[4]; cb = values[5];

	for(int tt=0; tt<evalPeriod; tt++) {
		r[tt] = 0.0; t[tt] = 0.0; p[tt] = 0.0;
	}

	sensorVolume = dCreateSphere(space, sensorRadius);

	dGeomSetBody(sensorVolume, basisObject->Get_Body());

	dGeomSetOffsetWorldPosition(sensorVolume, ox, oy, oz);

	GeomData* gd = new GeomData();
	gd->geomType = SENSOR_PROXIMITY;
	gd->objectPtr = myObj;

 	dGeomSetData(sensorVolume, static_cast<void*>(gd));
}

PROXIMITY_SENSOR::~PROXIMITY_SENSOR() {

	delete static_cast<GeomData*>(dGeomGetData(sensorVolume));
}

void PROXIMITY_SENSOR::Draw(double endX, double endY, double endZ, int tt) {

	const dReal *start = dGeomGetPosition(sensorVolume);

	double end[3] = {endX,endY,endZ};

	dsSetColor(cr[tt], cg[tt], cb[tt]);

	dsDrawLine(start, end);
}

void PROXIMITY_SENSOR::Set(double dist, OBJECT* objectThatWasHit, int tt) {

	if ( dist > sensorRadius*(1.-r[tt]) )
		return; // The proximity sensor stops when it hits its first object.

	r[tt] = 1. - dist/sensorRadius;

	if ( objectThatWasHit ) {

		cr[tt] = objectThatWasHit->Get_Red_Component();
		cg[tt] = objectThatWasHit->Get_Green_Component();
		cb[tt] = objectThatWasHit->Get_Blue_Component();

		objectThatWasHit->IsSeen_Sensor_Fires(tt);
	}
}

#endif // _SENSOR_PROXIMITY_CPP
