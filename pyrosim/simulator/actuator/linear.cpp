#ifndef _ACTUATOR_LINEAR_CPP
#define _ACTUATOR_LINEAR_CPP

#include <drawstuff/drawstuff.h>
#include <iostream>
#include <cmath>

#include "linear.h"
#include "../sensor/proprioceptiveLinear.h"

#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#define dsDrawBox dsDrawBoxD
#define dsDrawSphere dsDrawSphereD
#define dsDrawCylinder dsDrawCylinderD
#define dsDrawCapsule dsDrawCapsuleD
#endif

void LINEAR_ACTUATOR::Actuate(void) {

	if ( motorNeuron == NULL )
		return;

	double motorNeuronValue = motorNeuron->Get_Value();

	double zeroToOne = motorNeuronValue/2.0 + 0.5;

	double diff;

	double desiredTarget = zeroToOne * ( highStop - lowStop ) + lowStop;

	double currentTarget;

	if (positionControl) {

		currentTarget = dJointGetSliderPosition(joint);

		diff = desiredTarget - currentTarget;
		dJointSetSliderParam(joint,dParamVel, speed*diff);
	}
	else {

		dJointSetSliderParam(joint, dParamVel, speed*desiredTarget);
	}

	dJointSetSliderParam(joint, dParamFMax, strength);
}

void LINEAR_ACTUATOR::Create_In_Simulator(dWorldID world, OBJECT ** allObjects, int numObjects, ACTUATOR** allActuators, int numActuators) {

	if ( firstObject >= 0 )
		first = allObjects[firstObject];
	if ( secondObject >= 0 )
		second = allObjects[secondObject];

	joint = dJointCreateSlider(world,0);

	if (first == NULL) {

		dJointAttach( joint, 0, second->Get_Body() );
		const dReal *position = dBodyGetPosition( second->Get_Body() );
		x = position[0];
		y = position[1];
		z = position[2];
	}
	else if(second == NULL){

		dJointAttach( joint, first->Get_Body(), 0 );
		const dReal *position = dBodyGetPosition( first->Get_Body() );
		x = position[0];
		y = position[1];
		z = position[2];
	}
	else

		dJointAttach( joint , first->Get_Body() , second->Get_Body() );

	dJointSetSliderAxis(joint,normalX,normalY,normalZ);

	if ( positionControl == true ) {

		dJointSetSliderParam(joint,dParamLoStop,lowStop);
		dJointSetSliderParam(joint,dParamHiStop,highStop);
	}
}

bool LINEAR_ACTUATOR::Create_Proprioceptive_Sensor(int myID, int evalPeriod) {

	proprioceptiveSensor = static_cast<PROPRIOCEPTIVE_ROTARY_SENSOR>(new PROPRIOCEPTIVE_LINEAR_SENSOR(myID, evalPeriod));
	return true;
}

void LINEAR_ACTUATOR::Draw() const {

	dVector3 jointPosition;
	dVector3 jointAxis;
	dMatrix3 rotation;
	dReal jointActuation;
	dReal radius = .025;
	dReal length = .3;

	float r = .3;
	float g = 1.0;
	float b = .3;
	radius = .01;

	float lx = x;
	float ly = y;
	float lz = z;

	if (first != NULL && second != NULL){
		const dReal *pos1 = dBodyGetPosition(first->Get_Body());
		const dReal *pos2 = dBodyGetPosition(second->Get_Body());
		lx = (pos1[0] + pos2[0])/2.0;
		ly = (pos1[1] + pos2[1])/2.0;
		lz = (pos1[2] + pos2[2])/2.0;
	}
	dJointGetSliderAxis(joint, jointAxis);
	jointActuation = dJointGetSliderPosition(joint);
	jointPosition[0] = lx;
	jointPosition[1] = ly;
	jointPosition[2] = lz;

	dRFromZAxis(rotation, jointAxis[0], jointAxis[1], jointAxis[2]);
	dsSetColorAlpha(r,g,b,1.0);
	dsDrawCylinder(jointPosition, rotation, highStop-lowStop, radius);

	jointPosition[0] += jointActuation*jointAxis[0];
	jointPosition[1] += jointActuation*jointAxis[1];
	jointPosition[2] += jointActuation*jointAxis[2];
	dsSetColorAlpha(1.0,1.0,.3,1.0);
	dsDrawSphere(jointPosition, rotation, radius*2.0);

	jointPosition[0] -= 2*jointActuation*jointAxis[0];
	jointPosition[1] -= 2*jointActuation*jointAxis[1];
	jointPosition[2] -= 2*jointActuation*jointAxis[2];
	dsSetColorAlpha(.3,1.0,1.0,1.0);
	dsDrawSphere(jointPosition, rotation, radius*2.0);
}

void LINEAR_ACTUATOR::Read_From_Python(void) {

	std::cin >> ID;

	std::cin >> firstObject;
	std::cin >> secondObject;
	std::cin >> normalX;
	std::cin >> normalY;
	std::cin >> normalZ;
	std::cin >> lowStop;
	std::cin >> highStop;
	std::cin >> speed;
	std::cin >> strength;
	std::cin >> positionControl;
}

void LINEAR_ACTUATOR::Poll_Sensors(int t) {

	if ( proprioceptiveSensor )

		proprioceptiveSensor->Poll(joint, t);
}

#endif // _ACTUATOR_LINEAR_CPP
