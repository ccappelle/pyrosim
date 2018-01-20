#ifndef _ACTUATOR_ROTARY_CPP
#define _ACTUATOR_ROTARY_CPP

#include <drawstuff/drawstuff.h>
#include <iostream>
#include <cmath>

#include "rotary.h"

#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#endif

#include "../constants.h"
extern const int HINGE; // FIXME: remove once proprioceptive sensors are fixed

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

bool ROTARY_ACTUATOR::Connect_Sensor_To_Sensor_Neuron(int sensorID, int sensorValueIndex, NEURON *sensorNeuron) {

	if ( proprioceptiveSensor )
		if ( proprioceptiveSensor->Get_ID() == sensorID ) {

			proprioceptiveSensor->Connect_To_Sensor_Neuron(sensorValueIndex, sensorNeuron);
			return true;
		}

	return false;
}

void ROTARY_ACTUATOR::Create_In_Simulator(dWorldID world, OBJECT** allObjects, int numObjects, ACTUATOR** allActuators, int numActuators) {

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

bool ROTARY_ACTUATOR::Create_Proprioceptive_Sensor(int sensorID, int evalPeriod) {

	proprioceptiveSensor = new PROPRIOCEPTIVE_SENSOR(sensorID, evalPeriod); // FIXME: where's the corresponding delete?

	return true;
}

void ROTARY_ACTUATOR::Draw(void) const {

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

		proprioceptiveSensor->Poll(joint, HINGE, t); // FIXME: change this once the sensors are fixed
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

void ROTARY_ACTUATOR::Write_To_Python(int evalPeriod) const {

	if ( proprioceptiveSensor )

		proprioceptiveSensor->Write_To_Python(evalPeriod);
}

#endif // _ACTUATOR_ROTARY_CPP
