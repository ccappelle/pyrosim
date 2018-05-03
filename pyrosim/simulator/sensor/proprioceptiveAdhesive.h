#ifndef _SENSOR_PROPRIOCEPTIVE_ADHESIVE_H
#define _SENSOR_PROPRIOCEPTIVE_ADHESIVE_H

#include <ode/ode.h>
#include "sensor.h"
#include "../object.h"

class PROPRIOCEPTIVE_ADHESIVE_SENSOR : public SENSOR {

public:
	PROPRIOCEPTIVE_ADHESIVE_SENSOR(int myID, int evalPeriod) : SENSOR(myID, evalPeriod, 1) {};
	void Poll(OBJECT* obj, int t) {values[0][t] = (double) obj->Get_Num_Adhesion_Points();};
};

#endif // _SENSOR_PROPRIOCEPTIVE_ADHESIVE_H
