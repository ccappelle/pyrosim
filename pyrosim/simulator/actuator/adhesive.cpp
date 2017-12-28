#ifndef _ACTUATOR_ADHESIVE_CPP
#define _ACTUATOR_ADHESIVE_CPP

#include <drawstuff/drawstuff.h>
#include <iostream>

#include "adhesive.h"

void ADHESIVE::Actuate(void) {

	// Update the adhesion groups according to the output of the motor neuron
	// FIXME
}

void ADHESIVE::Create_In_Simulator(dWorldID world, OBJECT ** allObjects, int numObjects) {

	if ( firstObject >= 0 )
		first = allObjects[firstObject];

	first->Set_Adhesion(adhesionKind);
}

void ADHESIVE::Draw() const {

	// FIXME: artistic block here
}

void ADHESIVE::Read_From_Python(void) {

	std::cin >> ID;

	std::cin >> firstObject;
	std::cin >> adhesionKind;
}

#endif // _ACTUATOR_ADHESIVE_CPP
