#ifndef _ACTUATOR_ROPE_CPP
#define _ACTUATOR_ROPE_CPP

#include <drawstuff/drawstuff.h>
#include <iostream>
#include <cmath>
#include <cstdlib>

#include "rope.h"

#ifdef dDOUBLE
#define dsDrawLine dsDrawLineD
#endif

void ROPE::Actuate(void) {

	static double ftol;
	static double fx;
	static double fy;
	static double fz;

	Update_Geometry();

	if(currentLength > relaxedLength) {

		ftol = (springConstant * (currentLength-relaxedLength) / relaxedLength) / currentLength;

		fx = (pos2[0]-pos1[0])*ftol; fy = (pos2[1]-pos1[1])*ftol; fz = (pos2[2]-pos1[2])*ftol;

		dBodyAddForce(first->Get_Body(), fx, fy, fz);
		dBodyAddForce(second->Get_Body(), -fx, -fy, -fz);
	}
}

void ROPE::Create_In_Simulator(dWorldID world, OBJECT** allObjects, int numObjects) {

	if ( firstObject >= 0  && secondObject >=0 ) {
		first = allObjects[firstObject];
		second = allObjects[secondObject];
	}
	else {
		std::cerr << "Failed to create rope in simulator!\n";
		std::cerr << "Rope was intended to connect objects " << firstObject << " and " << secondObject << ", one of which does not exist\n";
		exit(EXIT_FAILURE);
	}

	Update_Geometry();
}

void ROPE::Draw() const {

	if(currentLength > relaxedLength)
		dsSetColorAlpha(0., 1., 0., 1.);
	else
		dsSetColorAlpha(1., 0., 0., 1.);

	dsDrawLine(pos1, pos2);
}

void ROPE::Read_From_Python(void) {

	std::cin >> ID;

	std::cin >> firstObject;
	std::cin >> secondObject;
	std::cin >> relaxedLength;
	std::cin >> springConstant;
}

/***** Private functions *****/

void ROPE::Update_Geometry(void) {

	static const dReal* body1pos = dBodyGetPosition(first->Get_Body());
	static const dReal* body2pos = dBodyGetPosition(second->Get_Body());

	for(unsigned i=0; i<3; i++) {
		pos1[i] = body1pos[i];
		pos2[i] = body2pos[i];
	}

	currentLength = sqrt( (pos1[0]-pos2[0])*(pos1[0]-pos2[0]) + (pos1[1]-pos2[1])*(pos1[1]-pos2[1]) + (pos1[2]-pos2[2])*(pos1[2]-pos2[2]) );
}

#endif // _ACTUATOR_ROPE_CPP
