#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ode/ode.h>

#include "reactionControlWheels.h"

void REACTION_CONTROL_WHEELS::Actuate(void) {

	if ( motorNeurons[0] == NULL &&
	     motorNeurons[1] == NULL &&
	     motorNeurons[2] == NULL )
		return;

	dVector3 torque;
	for(unsigned i=0; i<3; i++)
		torque[i] = motorNeurons[i] != NULL ?
		            (motorNeurons[i]->Get_Value())*maxTorque :
		            0.;

//	std::cerr << "Object: " << firstObject << " id: " << firstObjectID << " torque: " << torque[0] << " " << torque[1] << " " << torque[2] << "\n";
	dBodyAddRelTorque(firstObject->Get_Body(), torque[0], torque[1], torque[2]);
}

void REACTION_CONTROL_WHEELS::Create_In_Simulator(dWorldID world, OBJECT** allObjects, int numObjects, ACTUATOR** allActuators, int numActuators) {

	if ( firstObjectID >= 0 )
		firstObject = allObjects[firstObjectID];
	else {
		std::cerr << "Cannot attach a reaction control wheel to an object with id " << firstObjectID << "\n";
		exit(EXIT_FAILURE);
	}
//	std::cerr << "RCW created in simulator. Object id: " << firstObjectID << " pointer: " << firstObject << "\n";
}

void REACTION_CONTROL_WHEELS::Read_From_Python(void) {

	std::cin >> ID;

	std::cin >> firstObjectID;
	std::cin >> mbx;
	std::cin >> mby;
	std::cin >> mbz;
	std::cin >> maxTorque;
}
