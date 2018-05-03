#ifndef _ACTUATOR_REACTION_CONTROL_WHEELS_H
#define _ACTUATOR_REACTION_CONTROL_WHEELS_H

#include "../object.h"
#include "actuator.h"

class REACTION_CONTROL_WHEELS : public ACTUATOR {

private:
	int	firstObjectID;
	double mbx, mby, mbz; // FIXME: momentum budgets ignored for now
	double maxTorque;
	OBJECT* firstObject;

public:
	REACTION_CONTROL_WHEELS (void) : ACTUATOR(3),
	                 mbx(1.), mby(1.), mbz(1.),
	                 maxTorque(1.),
	                 firstObject(NULL) {};

	void Actuate(void);
	void Draw(void) const {};
	void Create_In_Simulator(dWorldID world, OBJECT** allObjects, int numObjects, ACTUATOR** allActuators, int numActuators);
	void Read_From_Python(void);
};

#endif // _ACTUATOR_REACTION_CONTROL_WHEELS_H
