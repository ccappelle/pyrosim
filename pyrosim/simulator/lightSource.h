#ifndef _LIGHT_SOURCE_H
#define _LIGHT_SOURCE_H

#include <iostream>
#include <ode/ode.h>

#define LIGHT_SOURCE_RADIUS 0.01

class LIGHT_SOURCE {

private:
	int ID;
	dReal ox, oy, oz;
	int kind;
	dBodyID body;
	const dReal brightness;

public:
	LIGHT_SOURCE(int myID, dBodyID myBody) : ID(myID),
	                                         ox(0.), oy(0.), oz(0.),
	                                         kind(0),
	                                         body(myBody),
	                                         brightness(1.0) {};

	void Read_From_Python(void) {
		std::cin >> ox;
		std::cin >> oy;
		std::cin >> oz;
		std::cin >> kind;
	};

	dReal Luminousity_At(dReal x, dReal y, dReal z, int kindOfLight) {
		// FIXME: ideally, luminousity should be independent of external queries, i.e. something like vector<pair<int,dReal>> containing sensor-agnostic luminousities for all light kinds
		if(kind == 0 || kindOfLight == 0 || kind == kindOfLight) { // sensors of zeroth kind see all light sources; light sources of zeroth kind are visible to all sensors
			dReal dist = Square_Distance_To(x, y, z);
			// Light decays as inverse square distance, but saturates in close proximity to source
			// std::cerr << "dbg: " << dist << " " << brightness/dist << "\n";
			return dist>LIGHT_SOURCE_RADIUS ? brightness/dist : brightness/LIGHT_SOURCE_RADIUS;
		}
		else
			return 0.;
	};

private:

	dReal Square_Distance_To(dReal x, dReal y, dReal z) {
		const dReal* bodyPos = dBodyGetPosition(body);
		dReal dx, dy, dz;
		dx = bodyPos[0] + ox - x; dy = bodyPos[1] + oy - y; dz = bodyPos[2] + oz - z;
		return dx*dx + dy*dy + dz*dz;
	};
};

#endif // _LIGHT_SOURCE_H
