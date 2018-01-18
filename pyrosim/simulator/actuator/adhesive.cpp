#ifndef _ACTUATOR_ADHESIVE_CPP
#define _ACTUATOR_ADHESIVE_CPP

#include <drawstuff/drawstuff.h>
#include <iostream>

#include "adhesive.h"

void ADHESIVE::Actuate(void) {

	// If no motor neuron is connected, do nothing
	if ( motorNeuron == NULL )
		return;

	// Update the adhesion groups according to the output of the motor neuron
	double neuVal = motorNeuron->Get_Value();

//	std::cerr << neuVal << std::endl;

	if ( enabledNow && neuVal < 0.5 ) {

//		std::cerr << "Unsetting adhesion\n";
		first->Unset_Adhesion(adhesionKind);
		enabledNow = false;
	}

	if ( !enabledNow && neuVal >= 0.5 ) {

//		std::cerr << "Setting adhesion\n";
		first->Set_Adhesion(adhesionKind);
		enabledNow = true;
	}
}

void ADHESIVE::Create_In_Simulator(dWorldID world, OBJECT ** allObjects, int numObjects, ACTUATOR** allActuators, int numActuators) {

	if ( firstObject >= 0 )
		first = allObjects[firstObject];
}

void ADHESIVE::Read_From_Python(void) {

	std::cin >> ID;

	std::cin >> firstObject;
	std::cin >> adhesionKind;
}

#endif // _ACTUATOR_ADHESIVE_CPP
