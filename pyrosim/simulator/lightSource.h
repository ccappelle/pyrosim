#ifndef _LIGHT_SOURCE_H
#define _LIGHT_SOURCE_H

#include <iostream>
#include <vector>
#include <array>
#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

#ifdef dDOUBLE
#define dsDrawSphere dsDrawSphereD
#endif // dDOUBLE

#define LIGHT_SOURCE_RADIUS 0.05

// Rainbow colors from https://www.webnots.com/vibgyor-rainbow-color-codes/
// Python one-liner converter: '{' + ', '.join(["{0:.2f}".format(float(x)/255.) for x in [255, 127, 0]]) + '}'
static const unsigned numColors = 7;
static const dReal black[3] = {0.00, 0.00, 0.00};
static const dReal colors[numColors][3] = {{0.58, 0.00, 0.83}, // violet
                                           {0.29, 0.00, 0.51}, // indigo
                                           {0.00, 0.00, 1.00}, // blue
                                           {0.00, 1.00, 0.00}, // green
                                           {1.00, 1.00, 0.00}, // yellow
                                           {1.00, 0.50, 0.00}, // orange
                                           {1.00, 0.00, 0.00}}; // red

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

	void Draw(void) {
		const dReal* col = getSensorColor();
		dsSetColor(col[0], col[1], col[2]);

		const dReal* bpos = dBodyGetPosition(body);
		dReal pos[4];
		pos[0] = bpos[0] + ox; pos[1] = bpos[1] + oy; pos[2] = bpos[2] + oz; pos[3] = 0.;
		const dReal* rot = dBodyGetRotation(body);
		dsDrawSphere(pos, rot, LIGHT_SOURCE_RADIUS);
	};


private:
	dReal Square_Distance_To(dReal x, dReal y, dReal z) {
		const dReal* bodyPos = dBodyGetPosition(body);
		dReal dx, dy, dz;
		dx = bodyPos[0] + ox - x; dy = bodyPos[1] + oy - y; dz = bodyPos[2] + oz - z;
		return dx*dx + dy*dy + dz*dz;
	};

	const dReal* getSensorColor(void) {
		if(kind == 0)
			return black;
		else
			return colors[(kind-1) % numColors];
	};
};

#endif // _LIGHT_SOURCE_H
