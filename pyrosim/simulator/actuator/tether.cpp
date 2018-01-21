#ifndef _ACTUATOR_TETHER_CPP
#define _ACTUATOR_TETHER_CPP

#include <drawstuff/drawstuff.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

#include "tether.h"

#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#endif

void TETHER::Actuate(void) {

	static double ftol;
	static double fx;
	static double fy;
	static double fz;

	Update_Geometry();

	if(currentLength > relaxedLength) {

		// std::cerr << "CL: " << currentLength << " PL: " << previousLength << " Dampening force: " << (dampeningCoefficient * (currentLength-previousLength) / relaxedLength) << std::endl;

		ftol = ( springConstant * (currentLength-relaxedLength) / relaxedLength  + dampeningCoefficient * (currentLength-previousLength) / previousLength ) / currentLength;

		fx = (pos2[0]-pos1[0])*ftol; fy = (pos2[1]-pos1[1])*ftol; fz = (pos2[2]-pos1[2])*ftol;

		dBodyAddForce(first->Get_Body(), fx, fy, fz);
		dBodyAddForce(second->Get_Body(), -fx, -fy, -fz);
	}
}

void TETHER::Create_In_Simulator(dWorldID world, OBJECT** allObjects, int numObjects, ACTUATOR** allActuators, int numActuators) {

	if ( firstObject >= 0  && secondObject >=0 ) {
		first = allObjects[firstObject];
		second = allObjects[secondObject];
	}
	else {
		std::cerr << "Failed to create rope in simulator!\n";
		std::cerr << "Rope was intended to connect objects " << firstObject << " and " << secondObject << ", one of which does not exist\n";
		exit(EXIT_FAILURE);
	}

	currentLength = Get_Current_Length(); // this required addition to Update_Geometry() ensures that there is a sane value at currentLength when it is time to memorize it at previousLength
	Update_Geometry();
}

void TETHER::Draw() const {

	if(currentLength > relaxedLength)
		dsSetColorAlpha(0., 1., 0., 1.);
	else
		dsSetColorAlpha(1., 0., 0., 1.);

	dsDrawLine(pos1, pos2);
}

void TETHER::Read_From_Python(void) {

	std::cin >> ID;

	std::cin >> firstObject;
	std::cin >> secondObject;
	std::cin >> relaxedLength;
	std::cin >> springConstant;
	std::cin >> dampeningCoefficient;
}

/***** Private functions *****/

void TETHER::Update_Geometry(void) {

	static const dReal* body1pos = dBodyGetPosition(first->Get_Body());
	static const dReal* body2pos = dBodyGetPosition(second->Get_Body());

	for(unsigned i=0; i<3; i++) {
		pos1[i] = body1pos[i];
		pos2[i] = body2pos[i];
	}

	previousLength = currentLength;
	currentLength = Get_Current_Length();
}

#endif // _ACTUATOR_TETHER_CPP
