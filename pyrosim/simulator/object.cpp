#ifndef _OBJECT_CPP
#define _OBJECT_CPP

#include "constants.h"
#include "object.h"
#include "positionSensor.h"
#include "iostream"
#include <drawstuff/drawstuff.h>
#include "texturepath.h"

#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#endif

OBJECT::OBJECT(void) {

	ID = 0;

	raySensor = NULL;

	lightSensor = NULL;

	positionSensor = NULL;

	touchSensor = NULL;

	vestibularSensor = NULL;

	containsLightSource = false; 
}

OBJECT::~OBJECT(void) {

}

int OBJECT::Connect_Sensor_To_Sensor_Neuron(int sensorID , NEURON *sensorNeuron) {

	if ( lightSensor )

		if ( lightSensor->Get_ID() == sensorID ) {

			lightSensor->Connect_To_Sensor_Neuron(sensorNeuron);

			return true;
		}

        if ( positionSensor )

                if ( positionSensor->Get_ID() == sensorID ) {

                        positionSensor->Connect_To_Sensor_Neuron(sensorNeuron);

			return true;
		}

        if ( raySensor )

                if ( raySensor->Get_ID() == sensorID ) {

                        raySensor->Connect_To_Sensor_Neuron(sensorNeuron);

			return true;
		}


        if ( touchSensor )

                if ( touchSensor->Get_ID() == sensorID ) {

                        touchSensor->Connect_To_Sensor_Neuron(sensorNeuron);

			return true;
		}


        if ( vestibularSensor )

                if ( vestibularSensor->Get_ID() == sensorID ) {

                        vestibularSensor->Connect_To_Sensor_Neuron(sensorNeuron);

			return true;
		}

	return false;
}

void OBJECT::Create_Ray_Sensor(dSpaceID space, int myID, int evalPeriod) {

	raySensor = new RAY_SENSOR(space,this,myID,evalPeriod);
}

void OBJECT::Create_Light_Sensor(int myID, int evalPeriod) {

        lightSensor = new LIGHT_SENSOR(myID,evalPeriod);
}

void OBJECT::Create_Light_Source(void) {

	containsLightSource = true;
}

void OBJECT::Create_Position_Sensor(int myID, int evalPeriod) {

	positionSensor = new POSITION_SENSOR(myID,evalPeriod);
}

void OBJECT::Create_Touch_Sensor(int myID, int evalPeriod) {

	touchSensor = new TOUCH_SENSOR(myID,evalPeriod);
}

void OBJECT::Create_Vestibular_Sensor(int myID, int evalPeriod) {

        vestibularSensor = new VESTIBULAR_SENSOR(myID,evalPeriod);
}

void OBJECT::Draw(void) {

        dsSetColor(r,g,b);

	if ( myShape == BOX )

		DrawBox();
	else
		DrawCylinder();
}

void OBJECT::Draw_Ray_Sensor(double x, double y, double z, int t) {

	if ( raySensor )

		raySensor->Draw(x,y,z,t);
}

double OBJECT::Get_Blue_Component(void) {

        return b;
}

dBodyID OBJECT::Get_Body(void) {

	return body;
}

double OBJECT::Get_Green_Component(void) {

        return g;
}
double OBJECT::Get_Length(void) {

	return length;
}

double OBJECT::Get_Radius(void) {

	return radius;
}

double OBJECT::Get_Red_Component(void) {

	return r;
}

void OBJECT::Poll_Sensors(int numObjects, OBJECT **objects, int t) {

	if ( lightSensor ) {

		OBJECT *closestLightSource = Find_Closest_Light_Source(numObjects,objects);

		lightSensor->Poll(body,closestLightSource->Get_Body(),t);
	}

        if ( positionSensor )

                positionSensor->Poll(body,t);

	if ( vestibularSensor )

		vestibularSensor->Poll(body,t);
}

void OBJECT::Read_From_Python(dWorldID world, dSpaceID space, int shape) {

	myShape = shape;

	std::cin >> ID;

	std::cin >> x;

	std::cin >> y;

	std::cin >> z;

	if ( myShape == BOX ) {

		std::cin >> length;

		std::cin >> width;

		std::cin >> height;

		std::cin >> r;

                std::cin >> g;

                std::cin >> b;

		CreateBox(world,space, x,y,z, length,width,height);
	}
	else {

		std::cin >> r1;

                std::cin >> r2;

                std::cin >> r3;

		std::cin >> length; 

                std::cin >> radius; 

                std::cin >> r;

                std::cin >> g;

                std::cin >> b;

                CreateCylinder(world,space, x,y,z, r1,r2,r3, length,radius);
	}
}

void OBJECT::Set_Ray_Sensor(double distance, OBJECT *objectThatWasHit, int t) {

	if ( raySensor )

		raySensor->Set(distance,objectThatWasHit,t);
}

void OBJECT::Touch_Sensor_Fires(int t) {

	if ( touchSensor )

		touchSensor->Fires(t);
}

void OBJECT::Update_Sensor_Neurons(int t) {

        if ( raySensor )

                raySensor->Update_Sensor_Neurons(t);

        if ( lightSensor )

                lightSensor->Update_Sensor_Neurons(t);

        if ( positionSensor )

                positionSensor->Update_Sensor_Neurons(t);

        if ( touchSensor )

                touchSensor->Update_Sensor_Neurons(t);

        if ( vestibularSensor )

                vestibularSensor->Update_Sensor_Neurons(t);
}

void OBJECT::Write_To_Python(int evalPeriod) {

	if ( raySensor )

		raySensor->Write_To_Python(evalPeriod);

	if ( lightSensor )

		lightSensor->Write_To_Python(evalPeriod);

	if ( positionSensor )

		positionSensor->Write_To_Python(evalPeriod);

	if ( touchSensor )

		touchSensor->Write_To_Python(evalPeriod);

        if ( vestibularSensor )

                vestibularSensor->Write_To_Python(evalPeriod);
}

// ------------------------------- Private methods ------------------------------

int OBJECT::Contains_A_Light_Source(void) {

	return containsLightSource;
}

void OBJECT::CreateBox(dWorldID world, dSpaceID space,
                                double x, double y, double z,
                                double length, double width, double height) {

        dMass m;

        body = dBodyCreate (world);
        dBodySetPosition (body,x,y,z);
        dMassSetBox (&m,1,length,width,height);
        dMassAdjust (&m,1);
        dBodySetMass (body,&m);
        geom = dCreateBox(space,length,width,height);
        dGeomSetBody (geom,body);

        dGeomSetData(geom,this);
}

void OBJECT::CreateCylinder(dWorldID world, dSpaceID space,
                                                double x, double y, double z,
                                                double r1, double r2, double r3,
                                                double length, double radius) {

        dMass m;

        body = dBodyCreate (world);
        dBodySetPosition (body,x,y,z);

        dMatrix3 R;
        dRFromZAxis(R,r1,r2,r3);
    	dBodySetRotation(body,R);

        // dMassSetSphere (&m,1,radius);
	dMassSetCapsule(&m,1,1,radius,length);
        dMassAdjust (&m,1);
        dBodySetMass (body,&m);
        // geom = dCreateCylinder(space,radius,length);
        geom = dCreateCapsule(space,radius,length);
        dGeomSetBody (geom,body);

	dGeomSetData(geom,this);
}

double OBJECT::Distance_To(OBJECT *otherObject) {

	const dReal *myPos = dBodyGetPosition( body );

	const dReal *hisPos = dBodyGetPosition( otherObject->Get_Body() );

	double xDiff = myPos[0] - hisPos[0];

        double yDiff = myPos[1] - hisPos[1];

        double zDiff = myPos[2] - hisPos[2];

	return sqrt( pow(xDiff,2.0) + pow(yDiff,2.0) + pow(zDiff,2.0) );
}

void OBJECT::DrawBox(void) {

	// dsSetTexture (DS_WOOD);

	dReal sides[3] = {length,width,height};

	dsDrawBox (dBodyGetPosition(body),dBodyGetRotation(body),sides);
}

void OBJECT::DrawCylinder(void) {

	// dsSetTexture (DS_WOOD);

	dsDrawCapsule(dBodyGetPosition(body),dBodyGetRotation(body),length,radius);
}

OBJECT* OBJECT::Find_Closest_Light_Source(int numObjects, OBJECT **objects) {

	double distance = 1000.0;

	int    closestLightSource = 0;

	for (int i=0;i<numObjects;i++)

		if ( objects[i]->Contains_A_Light_Source() ) {

			double distanceToObject = Distance_To(objects[i]);

			if ( distanceToObject < distance ) {

				distance = distanceToObject;

				closestLightSource = i;
			}
		}

	return objects[closestLightSource];
}

#endif
