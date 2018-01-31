#ifndef _SENSOR_POSITION_CPP
#define _SENSOR_POSITION_CPP

#include <iostream>
#include <sstream>
#include <string>
#include "position.h"
#include "neuron.h"

POSITION_SENSOR::POSITION_SENSOR(int myID, int evalPeriod) {

	ID = myID;

	x = new double[evalPeriod];
	y = new double[evalPeriod];
	z = new double[evalPeriod];

	for ( int i = 0 ; i < 3 ; i++)
		mySensorNeurons[i] = NULL;
}

void POSITION_SENSOR::Poll(dBodyID body, int t) {

	const dReal *pos;
	pos = dBodyGetPosition(body);


	x[t] = pos[0];
	y[t] = pos[1];
	z[t] = pos[2];
}

void POSITION_SENSOR::Update_Sensor_Neurons(int t) {

        if ( mySensorNeurons[0] )

                mySensorNeurons[0]->Set( x[t] );

        if ( mySensorNeurons[1] )

                mySensorNeurons[1]->Set( y[t] );

        if ( mySensorNeurons[2] )

                mySensorNeurons[2]->Set( z[t] );
}

void POSITION_SENSOR::Write_To_Python(int evalPeriod) {

	std::ostringstream oss;

	oss << ID << " " << 3 << " ";
	for(int t=0; t<evalPeriod; t++)
		oss << x[t] << " " << y[t] << " " << z[t] << " ";
	oss << "\n";

	std::cout << oss.str();
}

#endif // _SENSOR_POSITION_CPP
