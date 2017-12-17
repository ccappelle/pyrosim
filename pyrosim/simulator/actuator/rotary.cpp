#ifndef _ACTUATOR_ROTARY_CPP
#define _ACTUATOR_ROTARY_CPP

#include "iostream"

#include "rotary.h"

#include <drawstuff/drawstuff.h>
#include "texturepath.h"
#include <cmath>

#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#endif

ROTARY_ACTUATOR::ROTARY_ACTUATOR(void) :
	firstObject(-1),
	secondObject(-1),
	x(0),
	y(0),
	z(0),
	normalX(0),
	normalY(0),
	normalZ(0),
	lowStop(0),
	highStop(0),
	speed(0.0),
	strength(0.0),
	joint(NULL)
	positionControl(true),
	proprioceptiveSensor(NULL),
	motorNeuron(NULL),
	first(NULL),
	second(NULL) {}

ROTARY_ACTUATOR::~ROTARY_ACTUATOR(void) {}

void ROTARY_ACTUATOR::Actuate(void) {

	if ( motorNeuron == NULL )
		return;

	double motorNeuronValue = motorNeuron->Get_Value();

	double zeroToOne = motorNeuronValue/2.0 + 0.5;

	double diff;

	double desiredTarget = zeroToOne * ( highStop - lowStop ) + lowStop;

	double currentTarget;

	if ( positionControl )
	{
		currentTarget = dJointGetHingeAngle(joint);
		diff = desiredTarget - currentTarget;
		dJointSetHingeParam(joint,dParamVel, speed * diff);
	}
	else
		dJointSetHingeParam(joint,dParamVel, 0.5*speed*desiredTarget);

	dJointSetHingeParam(joint,dParamFMax, strength);
}

int ROTARY_ACTUATOR::Connect_Sensor_To_Sensor_Neuron(int sensorID , NEURON *sensorNeuron) {

	if ( proprioceptiveSensor )
		if ( proprioceptiveSensor->Get_ID() == sensorID ) {

			proprioceptiveSensor->Connect_To_Sensor_Neuron(sensorNeuron);
			return true;
		}

	return false;
}

void ROTARY_ACTUATOR::Create_In_Simulator(dWorldID world, OBJECT** allObjects, int numObjects) {

	if ( firstObject >= 0 )
		first = allObjects[firstObject];
	if ( secondObject >= 0 )
		second = allObjects[secondObject];

	joint = dJointCreateHinge(world,0);

	if ( first == NULL )
		dJointAttach( joint , 0 , second->Get_Body() );
	else if( second == NULL )
		dJointAttach(joint, first->Get_Body(), 0);
	else
		dJointAttach(joint, first->Get_Body(), second->Get_Body());

	dJointSetHingeAnchor(joint, x, y, z);
	dJointSetHingeAxis(joint, normalX, normalY, normalZ);

	if ( positionControl == true ) {
		dJointSetHingeParam(joint, dParamLoStop, lowStop);
		dJointSetHingeParam(joint, dParamHiStop, highStop);
	}
}

void ROTARY_ACTUATOR::Create_Proprioceptive_Sensor(int myID, int evalPeriod) {

	proprioceptiveSensor = new PROPRIOCEPTIVE_SENSOR(myID, evalPeriod); // FIXME: where's the corresponding delete?
}

void ROTARY_ACTUATOR::Draw(void) {

	if( lowStop == highStop && positionControl == true )
		return;

	dVector3 jointPosition;
	dVector3 jointAxis;
	dMatrix3 rotation;
	dReal jointActuation;
	dReal radius = .025;
	dReal length = .3;

	float r = 1.;
	float g = .3;
	float b = .3;

	dsSetColorAlpha(r, g, b, 1.0);

	dJointGetHingeAnchor(joint, jointPosition);
	dJointGetHingeAxis(joint, jointAxis);

	dRFromZAxis(rotation, jointAxis[0],jointAxis[1], jointAxis[2]);
	dsDrawCylinder(jointPosition, rotation, length, radius);
}

void ROTARY_ACTUATOR::Poll_Sensors(int t) {

	if ( proprioceptiveSensor )

		proprioceptiveSensor->Poll(joint, type, t);
}

void ROTARY_ACTUATOR::Read_From_Python(void) {

	std::cin >> ID;

	std::cin >> firstObject;
	std::cin >> secondObject;
	std::cin >> x;
	std::cin >> y;
	std::cin >> z;
	std::cin >> normalX;
	std::cin >> normalY;
	std::cin >> normalZ;
	std::cin >> lowStop;
	std::cin >> highStop;
	std::cin >> speed;
	std::cin >> strength;
	std::cin >> positionControl;
}

void ROTARY_ACTUATOR::Update_Sensor_Neurons(int t) {

	if ( proprioceptiveSensor )

		proprioceptiveSensor->Update_Sensor_Neurons(t);
}

void ROTARY_ACTUATOR::Write_To_Python(int evalPeriod) {

	if ( proprioceptiveSensor )

		proprioceptiveSensor->Write_To_Python(evalPeriod);
}

#endif
