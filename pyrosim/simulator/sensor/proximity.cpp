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
                                                                                              basisObject(myObj),
                                                                                              cpx(0.), cpy(0.), cpz(0.), ts(0) {
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

	dGeomSetOffsetPosition(sensorVolume, ox, oy, oz);

	GeomData* gd = new GeomData();
	gd->geomType = SENSOR_PROXIMITY;
	gd->objectPtr = myObj;

 	dGeomSetData(sensorVolume, static_cast<void*>(gd));
}

PROXIMITY_SENSOR::~PROXIMITY_SENSOR() {

	delete static_cast<GeomData*>(dGeomGetData(sensorVolume));
}

void PROXIMITY_SENSOR::Draw(void) {

	const dReal* start = dGeomGetPosition(sensorVolume);

	double end[3] = {cpx, cpy, cpz};

	dsSetColor(cr[ts], cg[ts], cb[ts]);

	dsDrawLine(start, end);
}

void PROXIMITY_SENSOR::Set(double depth, dVector3 contactPoint, OBJECT* objectThatWasHit, int tt) {

	ts = tt;

	if ( depth/sensorRadius < r[tt] )
		return; // The proximity sensor stops when it hits its first object.

	r[tt] = depth/sensorRadius;

	cpx = contactPoint[0]; cpy = contactPoint[1]; cpz = contactPoint[2];

	if ( objectThatWasHit ) {

		cr[tt] = objectThatWasHit->Get_Red_Component();
		cg[tt] = objectThatWasHit->Get_Green_Component();
		cb[tt] = objectThatWasHit->Get_Blue_Component();

		objectThatWasHit->IsSeen_Sensor_Fires(tt);
	}
	else {

		cr[tt] = 0.; cg[tt] = 0.; cb[tt] = 0.;
	}
}

#endif // _SENSOR_PROXIMITY_CPP
