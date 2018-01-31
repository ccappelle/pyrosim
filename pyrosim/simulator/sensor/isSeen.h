#ifndef _SENSOR_IS_SEEN_H
#define _SENSOR_IS_SEEN_H

#include <ode/ode.h>
#include "sensor.h"

class NEURON;

class IS_SEEN_SENSOR : public SENSOR {

public:
    IS_SEEN_SENSOR(int myID, int evalPeriod);
    ~IS_SEEN_SENSOR() {};

    void Fires(int t);
    void Update_Sensor_Neurons(int t);
};

#endif // _SENSOR_IS_SEEN_H
