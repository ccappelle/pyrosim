#ifndef _RAY_SENSOR_CPP
#define _RAY_SENSOR_CPP

#include "iostream"
#include "raySensor.h"
#include "object.h"
#include <drawstuff/drawstuff.h>
#include "neuron.h"

#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#endif

RAY_SENSOR::RAY_SENSOR(dSpaceID space, OBJECT *myObj, int myID, int evalPeriod) {

	ID = myID;

	obj = myObj;

        std::cin >> x;

        std::cin >> y;

        std::cin >> z;

        std::cin >> r1;

        std::cin >> r2;

        std::cin >> r3;

        std::cin >> maxDistance;

	Initialize(evalPeriod);

        ray = dCreateRay(space,maxDistance);

	Add_To_Object();

	for (int i = 0 ; i < 4 ; i++ )

        	mySensorNeurons[i] = NULL;
}

RAY_SENSOR::~RAY_SENSOR(void) {

}

void RAY_SENSOR::Add_To_Object(void) {

        dGeomSetBody(ray,obj->Get_Body());

        dGeomSetOffsetWorldPosition(ray,x,y,z);

	dMatrix3 R;

	dRFromZAxis(R,r1,r2,r3);

	dGeomSetOffsetWorldRotation(ray,R);

	dGeomSetData(ray,obj);

	dGeomRaySetParams(ray, true, true);
}

void RAY_SENSOR::Connect_To_Sensor_Neuron(NEURON *sensorNeuron) {

        mySensorNeurons[ sensorNeuron->Get_Sensor_Value_Index() ] = sensorNeuron;
}

void RAY_SENSOR::Draw(double endX, double endY, double endZ, int t) {

        const dReal *start = dGeomGetPosition( ray );

	double end[3] = {endX,endY,endZ};

        dsSetColor(r[t],g[t],b[t]);

        dsDrawLine( start , end );
}

int  RAY_SENSOR::Get_ID(void) {

        return ID;
}

void RAY_SENSOR::Initialize(int evalPeriod) {

        distances = new double[evalPeriod];

        r = new double[evalPeriod];

        g = new double[evalPeriod];

        b = new double[evalPeriod];

        for (int t=0;t<evalPeriod;t++) {

                distances[t] = maxDistance;

                r[t] = 0.0;

                g[t] = 0.0;

                b[t] = 0.0;
        }
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

void RAY_SENSOR::Update_Sensor_Neurons(int t) {

        if ( mySensorNeurons[0] )

                mySensorNeurons[0]->Set( distances[t] );

        if ( mySensorNeurons[1] )

                mySensorNeurons[1]->Set( r[t] );

        if ( mySensorNeurons[2] )

                mySensorNeurons[2]->Set( g[t] );

        if ( mySensorNeurons[3] )

                mySensorNeurons[3]->Set( b[t] );
}

void RAY_SENSOR::Write_To_Python(int evalPeriod) {

	char outString[100000];

	sprintf(outString,"%d %d ",ID,4);

	for ( int  t = 0 ; t < evalPeriod ; t++ ){
	   sprintf(outString,"%s %f %f %f %f ",outString,distances[t],r[t],g[t],b[t]);
       
    }

	sprintf(outString,"%s \n",outString);
    std::cout << outString;
}

#endif
