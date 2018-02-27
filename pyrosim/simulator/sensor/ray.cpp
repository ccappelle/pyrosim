#ifndef _SENSOR_RAY_CPP
#define _SENSOR_RAY_CPP

#include <iostream>
#include <drawstuff/drawstuff.h>
#include "ray.h"
#include "object.h"
#include "../geomData.h"

#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#endif

RAY_SENSOR::RAY_SENSOR(dSpaceID space, OBJECT *myObj, int myID, int evalPeriod) : SENSOR(myID, evalPeriod, 4),
                                                                                  obj(myObj) {

	std::cin >> x;
	std::cin >> y;
	std::cin >> z;

	std::cin >> r1;
	std::cin >> r2;
	std::cin >> r3;

	std::cin >> maxDistance;

	distances = values[0];
	r = values[1]; g = values[2]; b = values[3];

	for(int t=0; t<evalPeriod; t++) {
		distances[t] = maxDistance;
		r[t] = 0.0; g[t] = 0.0; b[t] = 0.0;
	}

	ray = dCreateRay(space,maxDistance);

	Add_To_Object();
}

void RAY_SENSOR::Draw(double endX, double endY, double endZ, int t) {

	const dReal *start = dGeomGetPosition( ray );

	double end[3] = {endX,endY,endZ};

	dsSetColor(r[t],g[t],b[t]);

	dsDrawLine( start , end );
}

void RAY_SENSOR::Set(double dist, OBJECT *objectThatWasHit,int t) {

	if ( dist > distances[t] )

	// The ray sensor stops when it hits its first object.

		return;

	distances[t] = dist;

	if ( objectThatWasHit ) {

		r[t] = objectThatWasHit->Get_Red_Component();

                g[t] = objectThatWasHit->Get_Green_Component();

                b[t] = objectThatWasHit->Get_Blue_Component();

               objectThatWasHit->IsSeen_Sensor_Fires(t);
        }
}

void RAY_SENSOR::Add_To_Object(void) {

	dGeomSetBody(ray, obj->Get_Body());

	dGeomSetOffsetWorldPosition(ray, x, y, z);

	dMatrix3 R;

	dRFromZAxis(R, r1, r2, r3);

	dGeomSetOffsetWorldRotation(ray, R);

	GeomData* gd = new GeomData();
	gd->geomType = SENSOR_RAY;
	gd->objectPtr = obj;
	dGeomSetData(ray, static_cast<void*>(gd));

	dGeomRaySetParams(ray, true, true);
}

#endif // _SENSOR_RAY_CPP
