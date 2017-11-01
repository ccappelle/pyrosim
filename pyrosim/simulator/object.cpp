#ifndef _OBJECT_CPP
#define _OBJECT_CPP

#include "constants.h"
#include "object.h"
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
    isSeenSensor = NULL;
	containsLightSource = false; 
}

OBJECT::~OBJECT(void) {

}

void OBJECT::Add_External_Force(float x, float y, float z, int timeStep){
    forces[timeStep][0] = x;
    forces[timeStep][1] = y;
    forces[timeStep][2] = z;
}

void OBJECT::Read_In_External_Force(void){
    int xPos,yPos,zPos,time;
    std::cin >> x;
    std::cin >> y;
    std::cin >> z;
    std::cin >> time;
    Add_External_Force(x,y,z,time);
}
void OBJECT::Apply_Stored_Forces(int timeStep){
    if (forces.find(timeStep)!= forces.end()){
        dBodyAddForce(body, forces[timeStep][0],forces[timeStep][1], forces[timeStep][2]);
    }

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
    if ( isSeenSensor )
        if ( isSeenSensor->Get_ID() == sensorID) {
            isSeenSensor->Connect_To_Sensor_Neuron(sensorNeuron);
            return true;
        }
    return false;
 }

void OBJECT::Create_IsSeen_Sensor(int myID, int evalPeriod){
    isSeenSensor = new IS_SEEN_SENSOR(myID, evalPeriod);
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
    const dReal *pos = dBodyGetPosition(body);
    const dReal *rot = dBodyGetRotation(body);

    // dsSetTexture (DS_WOOD);
    if (myShape == BOX){
        dReal sides[3] = {length,width,height};
        dsDrawBox (pos,rot,sides);
    }
    else if (myShape == CYLINDER)
        dsDrawCylinder(pos,rot,length,radius);
    else if (myShape == CAPSULE)
        dsDrawCapsule(pos,rot,length,radius);
    else if (myShape == SPHERE)
        dsDrawSphere(pos,rot,radius);
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

int OBJECT::Get_ID(void){
    return ID;
}
double OBJECT::Get_Green_Component(void) {
    return g;
}

int OBJECT::Get_Group(void){
    return collisionGroup;
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
    std::cin >> r1;
    std::cin >> r2;
    std::cin >> r3;
	
	if ( myShape == BOX ) {
		std::cin >> length;
		std::cin >> width;
		std::cin >> height;
	}
	else if (myShape == CYLINDER or myShape == CAPSULE) { //cylinder specific
		std::cin >> length; 
        std::cin >> radius;
	}
	else { //sphere specific
		std::cin >> radius;
	}
    std::cin >> mass;
    std::cin >> collisionGroup;
    std::cin >> r;
    std::cin >> g;
    std::cin >> b;

    CreateBody(world, space);

}

void OBJECT::Set_Ray_Sensor(double distance, OBJECT *objectThatWasHit, int t) {
	if ( raySensor )
		raySensor->Set(distance,objectThatWasHit,t);
}

void OBJECT::Touch_Sensor_Fires(int t) {
	if ( touchSensor )
		touchSensor->Fires(t);
}

void OBJECT::IsSeen_Sensor_Fires(int t){
    if ( isSeenSensor )
        isSeenSensor->Fires(t);
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
    std::cerr << "writing sensors in body " << this->Get_ID() << " to python " << std::endl;
	if ( raySensor ){
        std::cerr << "  writing ray sensor to python "  << std::endl;
		raySensor->Write_To_Python(evalPeriod);
    }

	if ( lightSensor ){
		lightSensor->Write_To_Python(evalPeriod);
        std::cerr << "  writing light sensor to python "  << std::endl;
    }
	if ( positionSensor ){
        std::cerr << "  writing position sensor to python "  << std::endl;
		positionSensor->Write_To_Python(evalPeriod);
    }
	if ( touchSensor ){
        std::cerr << "  writing touch sensor to python "  << std::endl;
		touchSensor->Write_To_Python(evalPeriod);
    }
    if ( vestibularSensor ){
        std::cerr << "  writing vestib sensor to python "  << std::endl;
        vestibularSensor->Write_To_Python(evalPeriod);
    }
    if ( isSeenSensor ){
        std::cerr << "  writing is seen sensor to python "  << std::endl;
        isSeenSensor->Write_To_Python(evalPeriod);
    }
}

// ------------------------------- Private methods ------------------------------

int OBJECT::Contains_A_Light_Source(void) {

	return containsLightSource;
}

void OBJECT::CreateBody(dWorldID world, dSpaceID space){

    dMass m;

    body = dBodyCreate (world);
    dBodySetPosition (body,x,y,z);

    dMatrix3 R;
    dRFromZAxis(R,r1,r2,r3);
    dBodySetRotation(body,R);

    if(myShape == BOX){
        dMassSetBoxTotal (&m,mass,length,width,height);
        geom = dCreateBox(space,length,width,height);
    }
    else if(myShape == CAPSULE){
        dMassSetCapsuleTotal(&m,mass,3,radius,length);
        geom = dCreateCapsule(space,radius,length);       
    }
    else if(myShape == CYLINDER){
        dMassSetCylinderTotal(&m,mass,3,radius,length);
        geom = dCreateCylinder(space,radius,length);       
    }    
    else if(myShape == SPHERE){
        dMassSetSphereTotal(&m,mass,radius);
        geom = dCreateSphere(space,radius);         
    }
    dMassRotate(&m, R);
    dBodySetMass (body,&m);

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


OBJECT* OBJECT::Find_Closest_Light_Source(int numObjects, OBJECT **objects) {

	double distance = 10000.0;
	int    closestLightSource = 0;

	for (int i=0;i<numObjects;i++){
		if ( objects[i]->Contains_A_Light_Source() ) {
			double distanceToObject = Distance_To(objects[i]);
			if ( distanceToObject < distance ) {
				distance = distanceToObject;
				closestLightSource = i;
			}
		}
    }

	return objects[closestLightSource];
}

#endif
