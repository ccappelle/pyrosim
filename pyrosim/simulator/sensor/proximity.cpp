#ifndef _SENSOR_PROXIMITY_CPP
#define _SENSOR_PROXIMITY_CPP

#include <iostream>
#include <drawstuff/drawstuff.h>
#include <math.h>
#include "proximity.h"
#include "object.h"
#include "../geomData.h"
#include "../myODEMath.h"

#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#define dsDrawSphere dsDrawSphereD
#endif

#define COORDINATE_SINGULARITY_CUTOFF 0.005
#define ANGULAR_SINGULARITY_CUTOFF 0.005

PROXIMITY_SENSOR::PROXIMITY_SENSOR(dSpaceID space, OBJECT* myObj, int myID, int evalPeriod) : SENSOR(myID, evalPeriod, 6),
                                                                                              basisObject(myObj),
                                                                                              cpx(0.), cpy(0.), cpz(0.), ts(0), justFired(false) {
	std::cin >> ox;
	std::cin >> oy;
	std::cin >> oz;
	std::cin >> sensorRadius;

	r = values[0]; t = values[1]; p = values[2];
	cr = values[3]; cg = values[4]; cb = values[5];

	for(int tt=0; tt<evalPeriod; tt++) {
		r[tt] = 0.0; t[tt] = 0.5; p[tt] = 0.5;
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

	if(justFired) {

		const dReal* start = dGeomGetPosition(sensorVolume);
		double end[3] = {cpx, cpy, cpz};
		dsSetColor(cr[ts], cg[ts], cb[ts]);
		dsDrawLine(start, end);

		justFired = false;
	}
}

void PROXIMITY_SENSOR::Set(double depth, dVector3 contactPoint, OBJECT* objectThatWasHit, int tt) {

	ts = tt;
	justFired = true;

	if ( depth/sensorRadius < r[tt] )
		return; // The proximity sensor stops when it hits its first object.

	r[tt] = depth/(sensorRadius*2.);

	// Computing the direction towards the contact point in the global coordinates
	const dReal* cen = dGeomGetPosition(sensorVolume);
	dVector3 contactDir;
	myDVector3Subtract(contactPoint, cen, contactDir);
	myDVector3Normalize(contactDir);

	// Computing the transformation matrix TO the body's coordinate system
	const dReal* sensorRotation = dGeomGetRotation(sensorVolume);
	dReal invertedSensorRotation[12];
	myDMatrix3Copy(sensorRotation, invertedSensorRotation);
	myDMatrix3Transpose(invertedSensorRotation); // R^{-1} = R^T! https://en.wikipedia.org/wiki/Rotation_matrix#Properties_of_a_rotation_matrix

	// Computing the direction towards the contact point in the coordinate system attached to the body...
	dVector3 localContactDir;
	myDMultiplyMat3Vec3(invertedSensorRotation, contactDir, localContactDir);
	// ...and translating it into spherical coordinates
	dReal theta = asin(localContactDir[2]);
	dReal phi;
	if(theta < ANGULAR_SINGULARITY_CUTOFF || M_PI-theta < ANGULAR_SINGULARITY_CUTOFF)
		phi = 0.;
	else if(abs(localContactDir[0]) < COORDINATE_SINGULARITY_CUTOFF)
		phi = localContactDir[1] > 0. ? M_PI_2 : -M_PI_2;
	else
		phi = atan(localContactDir[1]/localContactDir[0]);

	t[tt] = theta / M_PI;
	p[tt] = (phi + M_PI) / (2. * M_PI);

//	myPrintDVector3(std::cout, localContactDir);
//	std::cout << '\n';

	cpx = contactPoint[0]; cpy = contactPoint[1]; cpz = contactPoint[2]; // stored here for subsequent drawing

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
