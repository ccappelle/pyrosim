#ifndef _SENSOR_IS_SEEN_H
#define _SENSOR_IS_SEEN_H

#include <ode/ode.h>
#include "sensor.h"

class IS_SEEN_SENSOR : public SENSOR {

public:
    IS_SEEN_SENSOR(int myID, int evalPeriod) : SENSOR(myID, evalPeriod) {};
    void Fires(int t) {values[0][t] = 1.;};
};

#endif // _SENSOR_IS_SEEN_H
