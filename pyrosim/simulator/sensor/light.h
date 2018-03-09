#ifndef _SENSOR_LIGHT_H
#define _SENSOR_LIGHT_H

#include <ode/ode.h>
#include "sensor.h"

class LIGHT_SENSOR : public SENSOR {

private:
	int sensitivityKind;
	bool logarithmic;

public:
	LIGHT_SENSOR(int myID, int evalPeriod);
	void Read_From_Python(void);
	void Poll(dReal luminousity, int t);
	int Get_Sensitivity_Kind(void) {return sensitivityKind;};
};

#endif // _SENSOR_LIGHT_H
