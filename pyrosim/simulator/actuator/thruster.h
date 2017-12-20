#ifndef _ACTUATOR_THRUSTER_H
#define _ACTUATOR_THRUSTER_H

#include "../object.h"
#include "actuator.h"

class THRUSTER : public ACTUATOR {

private:
	int	firstObject;

	double	x;
	double	y;
	double	z;
	double  lowStop;
	double  highStop;

	double lastDesired;

	OBJECT *first;

public:
	THRUSTER(void) : lastDesired(0.0) {};

	void Actuate(void);

	void Create_In_Simulator(dWorldID world, OBJECT ** allObjects, int numObjects);

	void Draw(void) const;

	void Read_From_Python(void);
};

#endif // _ACTUATOR_THRUSTER_H
