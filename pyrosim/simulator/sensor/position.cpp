#ifndef _SENSOR_POSITION_CPP
#define _SENSOR_POSITION_CPP

#include "position.h"

void POSITION_SENSOR::Poll(dBodyID body, int t) {

	const dReal *pos;
	pos = dBodyGetPosition(body);

	x[t] = pos[0];
	y[t] = pos[1];
	z[t] = pos[2];
}

#endif // _SENSOR_POSITION_CPP
